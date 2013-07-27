// Application application cpp file.
 
// Original file name: App_RenRenApp.cpp
// Generated by TOPS Builder:Project wizard,Date:2010-12-13
 

#include  "App_RenRenApp.h"
#include  "App_RenRenMainForm.h"
#include "RenRenAPICommon.h"

#include "curl/curl.h"
#include "TCOM_ConnectManager_IIDs.h"
#include "TCOM_ObjectEventTypes.h"
#include "MTAPI/MTAPIErr.h"

#define TIMER_GET_NEW_NOTICE		30000	//定时获取新消息
#define BREAK_IF(cond)  if (cond) break

IDownLoad *		g_pDownload = NULL;
TFileDownloadMgr*  g_pFileDownMgr = NULL;

//保证相同Form只有一个
#define RefeshAPIFormID(nAccessType)\
	do{\
	if(gHwndId_RenRenApp[nAccessType] != 0)\
						{\
						this->CloseWindowByHwndId(gHwndId_RenRenApp[nAccessType]);\
						gHwndId_RenRenApp[nAccessType] = 0;\
						}\
						gHwndId_RenRenApp[nAccessType] = pWin->GetWindowHwndId();\
	}while(0)
	
TApp_RenRenApp::TApp_RenRenApp()
{
	TIClassFactory *pFactory = NULL;
	
	if(g_pDownload == NULL)
		g_pDownload = new IDownLoad;

	if(g_pFileDownMgr == NULL)
		g_pFileDownMgr = new TFileDownloadMgr;	

	// 初始化curl engine
	curl_global_init(CURL_GLOBAL_DEFAULT);

	// 初始化TCOM模块
	do 
	{
		HRESULT hr;
		hr = TCoInitialize(NULL);
		BREAK_IF(TCOM_S_FAIL(hr));

		// 查找TCOM_ConnectManager_Interface 对象的工厂方法
		hr = TCoGetClassObject(TCLSID_ConnectManager, TCOM_CLSCTX_INPROC_SERVER, NULL, TIID_IClassFactory, (LPVOID *)&pFactory);
		BREAK_IF(TCOM_S_FAIL(hr));

		// 生成TCOM_ConnectManager_Interface实例
		hr = pFactory->CreateInstance(NULL, TIID_ConnectManager, (LPVOID *)&m_pConnectManger);
		BREAK_IF(TCOM_S_FAIL(hr));
	} 
	while (0);

	if (pFactory)
	{
		pFactory->Release();
		pFactory = NULL;
	}

	m_nTimerId = -1;
	m_nMsgTimerId = -1;
	m_bReConnected = FALSE;	
	
	gHwndId_WaitingBox = 0;	
	gHwndId_Init = 0;
	gHwndId_Login = 0;
	bMessageBoxShow = FALSE;

	//起初上一次的log
	RenRen_Log_Init();
}

TApp_RenRenApp::~TApp_RenRenApp()
{
	curl_global_cleanup();

	if(g_pFileDownMgr != NULL)
		delete g_pFileDownMgr;

	if(g_pDownload != NULL)
		delete g_pDownload;

	// 释放资源
	if (m_pConnectManger)
	{
		m_pConnectManger->Release();
	}

	if(m_nTimerId != -1)
	{
		TIMER_Release(m_nTimerId);
		m_nTimerId = -1;
	}			

	if(m_nMsgTimerId != -1)
	{
		TIMER_Release(m_nMsgTimerId);
		m_nMsgTimerId = -1;
	}
}

Boolean  TApp_RenRenApp::EventHandler(EventType*  pEvent)
{
	Boolean     bHandled = FALSE;
	HRESULT hResult = -1;
	
	switch(pEvent->eType)
	{
	case EVENT_AppLoad:
		{
			//TInitialLoginForm *pWin = new TInitialLoginForm(this);
			TMainForm *pWin = new TMainForm( this );
			if (pWin)
			{
				SetActiveWindow(pWin);
				gHwndId_Init = pWin->GetWindowHwndId();
			}
			else
			{	// 窗口创建失败，退出应用。
				SendStopEvent();
			}
		}
		//缓存检测
		CheckCacheIsEnough();
		// 创建 Timer, 启动画面等待500ms开始网络连接
		m_nTimerId = TIMER_Create(500, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);	
		bHandled = TRUE;
		break;

	case EVENT_AppStopNotify:
		{
			
		}
		bHandled = FALSE;
		break;

	case MESSAGE_TIMER:
		//容错,如果TimerID == 0, 则直接忽略
		if(pEvent->sParam1 == 0)
			break;

		if (pEvent->sParam1 == m_nTimerId)
		{
			bHandled = TRUE;
			TIMER_Release(m_nTimerId);
			m_nTimerId = -1;

			// 无法拨号则退出
			if (m_pConnectManger == NULL)
			{
				TUChar  pNetErrorInfo[64] = {0};
				TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 0);
				MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
				SendStopEvent();
				//CloseAllWindows();
			}

			//自动选择账号
			CMAccount aIDs;   
			if (TCOM_S_SUCCESS(m_pConnectManger->GetDefaultAccount(&aIDs,NULL,NULL)))   
			{   
				//赋值默认账号ID为接口返回的账号ID   
				m_tAID = aIDs.uAccountID;   
			}  
			else
			{
				TUChar  pNetErrorInfo[64] = {0};
				TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 1);
				//MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
				//CloseAllWindows();
				//SendStopEvent();
				//5秒重拨一次
				m_nTimerId = TIMER_Create(5000, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);
				break;
			}

			//异步建立连接
			hResult = m_pConnectManger->ASyncConnect(m_tAID, &m_uConnectID);
			if(TCOM_S_FAIL(hResult))
			{
				// 拨号失败
				TUChar  pNetErrorInfo[64] = {0};
				TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 2);
				//MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
				//CloseAllWindows();		
				//SendStopEvent();
				//5秒重拨一次
				m_nTimerId = TIMER_Create(5000, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);
				break;
			}
		}
		else if(pEvent->sParam1 == m_nMsgTimerId)
		{
			bHandled = TRUE;		
			RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);
		}

		break;

		// 拨号的状态改变消息
	case MTE_NTY_TYPE_CM_CONN_STATUS_CHANGED:
		{
			BREAK_IF((UInt32)pEvent->sParam1 != m_uConnectID || TCOM_S_FAIL(pEvent->sParam2));

			// 根据连接的状态消息判断联网状态
			switch (pEvent->lParam4)
			{
			case CM_CONN_STATUS_RUN:
				{
					// 联网成功
				}
				break;

			case CM_CONN_STATUS_ESTABLISH:   
				{
					// 拨号中
				}
				break;
			}

			bHandled = TRUE;
		}
		break;


		//异步拨号结果消息
	case MTE_NTY_TYPE_CM_ASYNC_METHOD_RESULT_CONNECT:
		{
			BREAK_IF((UInt32)pEvent->sParam1 != m_uConnectID);
			// 根据连接的状态消息判断联网状态
			if (TCOM_S_SUCCESS(pEvent->sParam2))
			{
				// 联网成功
				//重新拨号成功则直接返回, 
				//如果是拨号过程的重拨, 则还是需要正常流程
				if( (m_bReConnected) && (gHwndId_Init == 0) )
				{
					break;
				}
				//启动主画面
				//启动主画面,先判断是否已经启动
				if(gHwndId_Login == 0)	
				{
					TInitialLoginForm *pWin = new TInitialLoginForm(this);
					if (pWin)
					{
						SetActiveWindow(pWin);
						gHwndId_Login = pWin->GetWindowHwndId();
						
						//销毁Init界面(主要是要停掉Timer)
						this->CloseWindowByHwndId(gHwndId_Init);
						gHwndId_Init = 0;
					}
				}
			} 
			else
			{
				//错误提示
				TUChar  pNetErrorInfo[64] = {0};
				TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 4);
				//MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
				//CloseAllWindows();					
				//SendStopEvent();
				//5秒重拨一次
				m_nTimerId = TIMER_Create(5000, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);
			}
			bHandled = TRUE;

		}
		break;

		// 断网的状态消息
	case MTE_NTY_TYPE_CM_ASYNC_METHOD_RESULT_DISCONNECT:
		{
			BREAK_IF((UInt32)pEvent->sParam1 != m_uConnectID);
			bHandled = TRUE;

			if (TCOM_S_FAIL(pEvent->sParam2))
			{
				// 断网失败
				break;
			}

			// 断网成功
			m_uConnectID = 0;
		}
		break;


		//服务器主动断网消息。收到此消息，表明已经拨号已经掉线
	case MTE_NTY_TYPE_CM_CONN_ABORTED:
	{ 
            CMAccount aIDs; 
            if (TCOM_S_SUCCESS(m_pConnectManger->GetDefaultAccount(&aIDs,NULL,NULL))) 
            { 
                m_tAID = aIDs.uAccountID; 
                hResult = m_pConnectManger->ASyncConnect(m_tAID, &m_uConnectID); 
                if (hResult < 0) 
                { 
                    // 拨号失败 
			TUChar  pNetErrorInfo[64] = {0};
			TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 5);
			//MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
			//CloseAllWindows();
			//SendStopEvent();
 			//5秒重拨一次
			m_nTimerId = TIMER_Create(5000, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);
               } 
            }  
            else
            { 
			TUChar  pNetErrorInfo[64] = {0};
			TUString::StrPrintF(pNetErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_DialupFail), 6);
			//MessageBox(pNetErrorInfo, TUSTR_Re_NULL, WMB_OK);
			//CloseAllWindows();
			//SendStopEvent();
			//5秒重拨一次
			m_nTimerId = TIMER_Create(5000, TIMER_MODE_AUTORUN | TIMER_MODE_NORMAL, NULL, 0, 0);

            } 
            //拨号已断，重新连接标志位 
            m_bReConnected = TRUE; 
            bHandled = TRUE;             
        } 
        break;	

	case MSG_SWITCHFORM_FEED:
		{
			bHandled = TRUE;
			break;
		}

	case MSG_SWITCHFORM_FRIEND_LIST:
		{
			bHandled = TRUE;
			break;
		}

	case MSG_SWITCHFORM_PROFILE:
		{
			bHandled = TRUE;
			break;
		}

	case MSG_SWITCHFORM_APPLIST:
		{
			bHandled = TRUE;
			break;
		}

	case MSG_SWITCHFORM_USEREVENT:
		{
			TUsersGetEventForm *pWin = new TUsersGetEventForm(this, !FormIsValid(RR_UsersGetEvent));
			RefeshAPIFormID(RR_UsersGetEvent);			
			bHandled = TRUE;
			break;		
		}

	case MSG_CONNECT_INIT_TIMEROUT:
	{
		if(m_nTimerId != -1)
		{
			TIMER_Release(m_nTimerId);
			m_nTimerId = -1;
		}
		bHandled = TRUE;
		break;
	}
	case MSG_LOGOUT:
		{

			if(MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_AreYouSureToLogout),TResource::LoadConstString(APP_RE_ID_STRING_Logout),WMB_OKCANCEL) == 0)
			{
				//先保存设置
				//RenRenUserInfoNeedSave.bAutoLogin = FALSE;
				
				//清空用户数据
				memset(&RenRenUserInfo, 0, sizeof(RenRenUserInfo));
				//RenRenAPI_SetConfig();

				//:TODO, 返回初始登录界面
				CloseAllForm();		

				//TODO::停止短消息读取定时器
				TIMER_Release(m_nMsgTimerId);
				m_nMsgTimerId = -1;
			}
			bHandled = TRUE;
			break;			
		}
	case MSG_EXIT:
		{
			if(MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_AreYouSureToExit),TResource::LoadConstString(APP_RE_ID_STRING_Exit),WMB_OKCANCEL) == 0)
			{
				this->CloseAllWindows();
			}
			
			bHandled = TRUE;
			break;			
		}

	case MSG_BACKTO_HOME:
		{
			//TMainMenuForm *pWin = new TMainMenuForm( this , !FormIsValid(RR_MainMenu));
			//RefeshAPIFormID(RR_MainMenu);

			CloseHomeForm();
			bHandled = TRUE;
			break;
		}
	case MSG_DL_THREAD_START:
		{
			NotifyMsgDataType notifyData;
			Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));

			bHandled = RR_GetApiNeedWaitingBox((RR_AccessType)notifyData.nAccessType);
			if(bHandled == TRUE)
			{
				if(gHwndId_WaitingBox == 0)
				{
					TWaitingBoxForm* g_WaitingBoxForm = new TWaitingBoxForm(this);
					gHwndId_WaitingBox = g_WaitingBoxForm->GetWindowHwndId();
				}
			}

			bHandled = TRUE;
			break;
		}

	case MSG_DL_THREAD_END:
		{
			if(gHwndId_WaitingBox != 0)
			{
				this->CloseWindowByHwndId(gHwndId_WaitingBox);
				gHwndId_WaitingBox = 0;
			}
			bHandled = TRUE;
			break;
		}		
	case MSG_DL_THREAD_PROCESS:
		{
			char buf[64];
			NotifyMsgDataType notifyData;
			Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));

			sprintf(buf, "\n下载进度:%d/%d", notifyData.nCurSize, notifyData.nTotalSize);
			bHandled = TRUE;
			break;
		}
		//下载完成消息
	case MSG_DL_THREAD_NOTIFY:
		{
			int iRet = eSucceed;
			NotifyMsgDataType notifyData;
			Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));

			//调试log
			char logTxt[128+20] = {0};
			sprintf(logTxt, "[DL finish:%2d][%d]",notifyData.nAccessType,notifyData.eDLCode);
			char* szDownloadFile=RR_GetFileName((RR_AccessType)notifyData.nAccessType);
			if(szDownloadFile != NULL)
			{
				strcat(logTxt, szDownloadFile);
			}
			RenRen_Log(logTxt);
			
			
			//清除下载列表中的数据(判断条件:accessType/nHwndId/nCtrlId/nParam 全匹配)
			TAListElement  pElement;
			MemSet(&pElement, 0, sizeof(TAListElement));
			pElement.accessType = notifyData.nAccessType;
			pElement.nHwndId = notifyData.nHwndId;
			pElement.nCtrlId = notifyData.nCtrlId;
			pElement.nParam = notifyData.nParam;
			iRet = RenRenAPICommon_DownloadComplete(&pElement);
			//如果RenRenAPICommon_DownloadComplete返回不是eSucced, 表明这个下载已被Cancel或其他异常,直接return TRUE, 不做处理
			if(iRet != eSucceed)
			{
				bHandled = TRUE;
				break;			
			}

			//如果是下载图片则让各个Form自行处理
			if(notifyData.nAccessType == RR_PhotoDownload)
			{
				bHandled = FALSE;
				break;
			}

			//定时查询新消息,单独处理
			if(notifyData.nAccessType == RR_UsersGetEvent)
			{
				Boolean bNewNotice = FALSE;
				bNewNotice = UpdateNoticeCount();

				//判断是否要弹出新消息提示框
				//若没有显示USEREVENT窗口, 则弹出提示框
				if( FormIsValid(RR_UsersGetEvent) == FALSE)
				{				
					if(bNewNotice == TRUE)
					{
						if(bMessageBoxShow == FALSE)
						{
							bMessageBoxShow = TRUE;
							if(MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_IfReadNewNotice),TResource::LoadConstString(APP_RE_ID_STRING_NewNotice),WMB_OKCANCEL) == 0)
							{
								SS_GTID			gtidMain;
								SS_GetCurrentGTID(&gtidMain);
								App_PostMessageEx(&gtidMain, MSG_SWITCHFORM_USEREVENT, NULL, 0, 0, 0);	
							}
							bMessageBoxShow = FALSE;
						}
					}
				}
				//若存在USEREVENT窗口,发送更新消息, 减少的情况也要更新
				else
				{
					SS_GTID			gtidMain;
					SS_GetCurrentGTID(&gtidMain);
					App_PostMessageEx(&gtidMain, MSG_FORM_UPDATE, NULL, 0, gHwndId_RenRenApp[RR_UsersGetEvent], 0);	
				}									
				bHandled = TRUE;
				break;
			}
			
			// 销毁正在下载提示框
			// 如果是RR_ProfileGetInfo(个人主页),则先不销毁下载提示框,
			// 马上要进行RR_FeedGet, patch...
			if(notifyData.nAccessType == RR_ProfileGetInfo)
			{
				//如果下载错误,还是要销毁下载提示框, patch again....
				if(notifyData.eDLCode != DLC_FINISHED)
				{
					SS_GTID		gtidMain;
					SS_GetCurrentGTID(&gtidMain);
					App_PostMessageEx(&gtidMain, MSG_DL_THREAD_END, NULL, 0, 0, 0);
				}
				else
				{
					TUChar ErrorInfo[ERROR_INFO_LEN] = {0};
					int errorno = RenRenAPI_JsonParse_bSuccess((RR_AccessType)notifyData.nAccessType, ErrorInfo);
					if( errorno != 0)					
					{
						SS_GTID		gtidMain;
						SS_GetCurrentGTID(&gtidMain);
						App_PostMessageEx(&gtidMain, MSG_DL_THREAD_END, NULL, 0, 0, 0);
					}
				}
			}
			else
			{
				SS_GTID		gtidMain;
				SS_GetCurrentGTID(&gtidMain);
				App_PostMessageEx(&gtidMain, MSG_DL_THREAD_END, NULL, 0, 0, 0);
			}

			//bHandled默认置为TRUE
			bHandled = TRUE;

			//下载失败/中断等统一提示处理
			if(notifyData.eDLCode != DLC_FINISHED)
			{
				//如果是不需要等待框的, 下载出错可以不弹出提示框
				if(RR_GetApiNeedWaitingBox((RR_AccessType)notifyData.nAccessType) == TRUE)
				{
					//图片上传提示单独处理
					if(notifyData.nAccessType == RR_PhotosUpload)
						MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_PhotoUploadFail),TResource::LoadConstString(APP_RE_ID_STRING_Fail),WMB_OK);
					else
						MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_DownloadFail),TResource::LoadConstString(APP_RE_ID_STRING_Fail),WMB_OK);
				}
				break;
			}
			
			//统一判断错误
			TUChar ErrorInfo[ERROR_INFO_LEN] = {0};
			int errorno = RenRenAPI_JsonParse_bSuccess((RR_AccessType)notifyData.nAccessType, ErrorInfo);
			if( errorno != 0)
			{
				//如果是session过期,则需要重新获取
				if( (errorno == 6) || (errorno == 450) )
				{
					RenRenAPICommon_Download(RR_ReGetSessionID, 0, 0, 0);
				}
				
				//如果是留言本/feedget/albums/blogs/,error_code = 200 (没有权限操作),需要单独处理,显示在对应界面
				Boolean bContinue = FALSE;
				if( (notifyData.nAccessType == RR_FeedGet) ||
					(notifyData.nAccessType == RR_PhotosGetAlbums) ||
					(notifyData.nAccessType == RR_BlogGets) ||
					(notifyData.nAccessType == RR_GuestbookGet) )
				{
					int nErrorCode = 0;
					tResponseGuestbookGet* Response;
					nErrorCode = RenRenAPI_JsonParse((RR_AccessType)notifyData.nAccessType, (void **)&Response);	
					if(nErrorCode == 200)
					{
						bContinue = TRUE;
					}
				}
				
				if(bContinue == FALSE)
				{
					if(TUString::StrLen(ErrorInfo) > 0)
					{
						//如果是不需要等待框的, 下载出错可以不弹出提示框
						if(RR_GetApiNeedWaitingBox((RR_AccessType)notifyData.nAccessType) == TRUE)
						{
							MessageBox( ErrorInfo, TResource::LoadConstString(APP_RE_ID_STRING_Error), WMB_OK);
						}
					}
					break;
				}
			}

			//如果是不需要App_kaiXin统一处理的(由各Form自行处理),直接bHandled=FALSE
			bHandled = RR_GetApiHandle((RR_AccessType)notifyData.nAccessType);
			if(bHandled == FALSE)
			{
				//判断如果Form不存在,则直接忽略
				TWindow *pWin = this->GetWindowByHwndId(notifyData.nHwndId);
				if(pWin == NULL)
					bHandled = TRUE;
				else
					bHandled = FALSE;				 
				break;
			}
				
			//每个API Json的处理
			switch(notifyData.nAccessType)
			{
			case RR_InitialLogin:
				{
					int iRet =0;
					tResponseInitialLogin* Response = NULL;
					iRet = RenRenAPI_JsonParse(RR_InitialLogin, (void **)&Response);
					if(Response != NULL)
					{
						if(Response->error_code == 0)
						{
							//先保存设置
							TUChar TUCharUid[32] = {0}; 	
							STRCPY_Ex(RenRenUserInfo.session_key, Response->session_key);
							STRCPY_Ex(RenRenUserInfo.ticket, Response->ticket);
							RenRenUserInfo.uid = Response->uid;
							TUString::StrIToA(TUCharUid,RenRenUserInfo.uid);
							TUString::StrUnicodeToStrUtf8((Char*)RenRenUserInfo.szuid, TUCharUid);
							RenRenAPI_SetConfig();
							
							//更行登录界面,如果不保存密码,则清楚密码
							SS_GTID			gtidMain;
							SS_GetCurrentGTID(&gtidMain);
							App_PostMessageEx(&gtidMain, MSG_FORM_UPDATE, NULL, 0, gHwndId_Login, 0);	
					
						
							TMainMenuForm *pWin = new TMainMenuForm( this, !FormIsValid(RR_MainMenu) );
							RefeshAPIFormID(RR_MainMenu);

							//服务器数据统计
							//RenRenAPICommon_Download(RR_PhoneclientActiveClient, 0, 0, 0);
							//TODO::启动短消息读取定时器
							RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);
							m_nMsgTimerId = TIMER_Create(TIMER_GET_NEW_NOTICE, TIMER_MODE_AUTORUN | TIMER_MODE_PERIOD, NULL, 0, 0);
						}
						
						delete Response;
						Response = NULL;
					}
					break;
				}

				case RR_ReGetSessionID:
				{
					int iRet =0;
					tResponseInitialLogin* Response = NULL;
					iRet = RenRenAPI_JsonParse(RR_ReGetSessionID, (void **)&Response);
					if(Response != NULL)
					{
						if(Response->error_code == 0)
						{
						//先保存设置
							TUChar TUCharUid[32] = {0}; 	
							STRCPY_Ex(RenRenUserInfo.session_key, Response->session_key);
							STRCPY_Ex(RenRenUserInfo.ticket, Response->ticket);
						}
						
						delete Response;
						Response = NULL;
					}
					break;
				}
				
				case RR_ProfileGetInfo:
					{
						char* uid = NULL;
						uid = Get_Url_Params(RR_ProfileGetInfo, "uid");
						Set_Url_Params(RR_FeedGet, "uid", uid);
						Set_Url_Params(RR_FeedGet, "page", "1");
						if(strcmp(uid, RenRenUserInfo.szuid) == 0)
							RenRenAPICommon_Download(RR_FeedGet, 0, 0, FEED_MODE_MYHOME);
						else
							RenRenAPICommon_Download(RR_FeedGet, 0, 0, FEED_MODE_FRIENDHOME);
					}
					break;

				case RR_FriendsGetFriends:
					{
						//短信选择收件人
						if(notifyData.nParam == FRIENDLSIT_MODE_MESSAGE)
							{
							TFriendsGetFriendsForm *pWin = new TFriendsGetFriendsForm( this, FRIENDLSIT_MODE_MESSAGE, !FormIsValid(notifyData.nAccessType) );
							RefeshAPIFormID(notifyData.nAccessType);
							}
						else
							{
							TFriendsGetFriendsForm *pWin = new TFriendsGetFriendsForm( this, FRIENDLSIT_MODE_ALL, !FormIsValid(notifyData.nAccessType) );
							RefeshAPIFormID(notifyData.nAccessType);
							}
					}
					break;
				case RR_FriendsSearch:
					{
						TFriendsSearchForm *pWin = new TFriendsSearchForm( this, !FormIsValid(notifyData.nAccessType) );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_UsersGetInfo:
					{
						TUsersGetInfoForm *pWin = new TUsersGetInfoForm( this );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;

				case RR_UsersGetVisitors:
					{
						TFriendsGetFriendsForm *pWin = new TFriendsGetFriendsForm( this, FRIENDLSIT_MODE_VISITOR, !FormIsValid(RR_FriendsGetFriends));
						RefeshAPIFormID(RR_FriendsGetFriends);
					}
					break;
				#if 0	
				case RR_UsersGetEvent:
					{
						TUsersGetEventForm *pWin = new TUsersGetEventForm(this, !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				#endif	
				case RR_MessageGet:
					{
						TMessageGetForm *pWin = new TMessageGetForm( this , !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);

						RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);//更新消息条数
					}
					break;					
				case RR_MessageGets:
					{
						TMessageGetsForm *pWin = new TMessageGetsForm( this );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;	
				case RR_BlogGets:
					{
						TBlogGetsForm *pWin = new TBlogGetsForm( this , (eFormMode)notifyData.nParam, !FormIsValid(notifyData.nAccessType) );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_BlogGet:
					{
						TBlogGetForm *pWin = new TBlogGetForm( this, !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_BlogGetComments:
					{
						TBlogGetCommentsForm *pWin = new TBlogGetCommentsForm( this, !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_PagesGetList:
					{
						//需要区分是否是有Bottom Tab条
						if(notifyData.nParam == PAGESLIST_MODE_MYHOME)
						{
							TFriendsGetFriendsForm *pWin = new TFriendsGetFriendsForm( this, FRIENDLSIT_MODE_PAGES, !FormIsValid(RR_FriendsGetFriends));
							RefeshAPIFormID(RR_FriendsGetFriends);
						}
						else
						{
							TPagesGetListForm *pWin = new TPagesGetListForm( this );
							RefeshAPIFormID(notifyData.nAccessType);
						}
					}
					break;
				case RR_PagesGetInfo:
					{
						TPagesGetInfoForm *pWin = new TPagesGetInfoForm( this );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_PhotosGetAlbums:
					{
						TPhotosGetAlbumsForm *pWin = new TPhotosGetAlbumsForm( this , (eFormMode)notifyData.nParam, !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_PhotosGetComments:
					{
						TPhotosGetCommentsForm *pWin = new TPhotosGetCommentsForm( this ,!FormIsValid(notifyData.nAccessType) );
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_FeedGet:
					{
						//拼接json文件
						RR_UpdateMoreFile(RR_FeedGet);
			
						if(notifyData.nParam == FEED_MODE_NEW)
						{
							TFeedGetForm *pWin = new TFeedGetForm( this , (eFeedMode)notifyData.nParam, !FormIsValid(RR_FeedGetNew));
							RefeshAPIFormID(RR_FeedGetNew);
						}
						else
						{
							TFeedGetForm *pWin = new TFeedGetForm( this , (eFeedMode)notifyData.nParam, !FormIsValid(notifyData.nAccessType));
							RefeshAPIFormID(notifyData.nAccessType);
						}
					}
					break;
				case RR_GuestbookGet:
					{
						TGuestbookGetForm *pWin = new TGuestbookGetForm( this, (eFormMode)notifyData.nParam, !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_PhotosGet:
					{
						char*  aid;	
						aid = Get_Url_Params(RR_PhotosGet, "aid");
						if(strlen(aid) != 0)	
						{
							TPhotosGetForm *pWin = new TPhotosGetForm( this );
							RefeshAPIFormID(notifyData.nAccessType);
						}
						else
						{
							TPhotoDetailForm* pWin = new TPhotoDetailForm(this, 0, 1, TRUE);								
							RefeshAPIFormID(notifyData.nAccessType);
						}
					}
					break;
				case RR_RequestsGetFriendRequests:
					{
						TRequestsGetFriendRequestsForm *pWin = new TRequestsGetFriendRequestsForm( this );
						RefeshAPIFormID(notifyData.nAccessType);

						RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);//更新消息条数
					}
					break;
				case RR_FeedGetFangle:
					{
						TFeedGetFangleForm *pWin = new TFeedGetFangleForm( this , !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
				case RR_FeedReadFangle:
					{
						RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);//更新消息条数
					}
					break;
				case RR_MessageMark:
					{
						RenRenAPICommon_Download(RR_UsersGetEvent, 0, 0, 0);//更新消息条数
					}
					break;
				case RR_StatusGetComment:
					{
						TStatusGetCommentForm *pWin = new TStatusGetCommentForm( this , !FormIsValid(notifyData.nAccessType));
						RefeshAPIFormID(notifyData.nAccessType);
					}
					break;
					
				default:
					break;
				}
			bHandled = TRUE;
			break;
		}
		
	}
	if (FALSE == bHandled) 
	{
		return TApplication::EventHandler(pEvent);
	}

	return bHandled;
}

/**
 * 窗口是否已经存在
 *
 * \param nAccessType
 *
 * \return 
 */
Boolean TApp_RenRenApp::FormIsValid(Int32  nAccessType)
{
	TWindow *preWin = NULL;

	if(gHwndId_RenRenApp[nAccessType] == 0)
		return FALSE;

	preWin = this->GetWindowByHwndId(gHwndId_RenRenApp[nAccessType]);
	if(preWin == NULL)
		return FALSE;

	return WindowIsValid(preWin);
}

/**
 * 关闭所有API窗口,只保留登录界面
 */
void TApp_RenRenApp::CloseAllForm(void)
{

	for(int i=0; i<ARR_SIZE(gHwndId_RenRenApp); i++)
	{
		if(gHwndId_RenRenApp[i] != 0)
		{
			this->CloseWindowByHwndId(gHwndId_RenRenApp[i]);
			gHwndId_RenRenApp[i] = 0;
		}		
	}
}

/**
 * 关闭Home界面的Form: Feed/albums/guestbook/blog/userinfo 
 */
void TApp_RenRenApp::CloseHomeForm(void)
{
	Int32 FormID[] = 
	{ 
		RR_FeedGet,
		RR_PhotosGetAlbums,
		RR_BlogGets,
		RR_GuestbookGet,
		RR_UsersGetInfo,
		RR_PagesGetInfo
	};

	TWindow* pActiveWin=GetActiveWindow();
	
	for(int i=0; i<ARR_SIZE(FormID); i++)
	{
		if(gHwndId_RenRenApp[FormID[i]] != 0)
		{
			if( pActiveWin != GetWindowByHwndId(gHwndId_RenRenApp[FormID[i]]) )
			{
				this->CloseWindowByHwndId(gHwndId_RenRenApp[FormID[i]]);
				gHwndId_RenRenApp[FormID[i]] = 0;
			}
		}		
	}


	for(int i=0; i<ARR_SIZE(FormID); i++)
	{
		if(gHwndId_RenRenApp[FormID[i]] != 0)
		{
			this->CloseWindowByHwndId(gHwndId_RenRenApp[FormID[i]]);
			gHwndId_RenRenApp[FormID[i]] = 0;
		}		
	}

}

/**
 * 更新新消息的条数 
 *
 * \return 
 */
Boolean TApp_RenRenApp::UpdateNoticeCount(void)
{
	int iRet = eSucceed;
	int i = 0;
	Boolean bNewNotice = FALSE;

	tResponseUsersGetEvent* Response = NULL;
	iRet = RenRenAPI_JsonParse(RR_UsersGetEvent, (void **)&Response);
	if(Response != NULL)
	{
		if( (Response->friend_request_count > RenRenUserInfo.nCountFriendRequest) || 
			(Response->new_message_count > RenRenUserInfo.nCountNewMessage) ||
			//(Response->poke_count > RenRenUserInfo.nCountPoke) ||//打招呼暂时不需要提醒
			(Response->fangle_count > RenRenUserInfo.nCountFangle) )
		{
			bNewNotice = TRUE;
		}

		RenRenUserInfo.nCountFriendRequest = Response->friend_request_count;
		RenRenUserInfo.nCountNewMessage= Response->new_message_count;
		RenRenUserInfo.nCountPoke= Response->poke_count;
		RenRenUserInfo.nCountFangle= Response->fangle_count;

		delete Response;
		Response = NULL;
	}

	return bNewNotice;
}

/**
 * \brief GetConnectID 
 * 获取连接的ID
 * \return 
 */
ULONG TApp_RenRenApp::GetConnectID()
{
	return m_tAID;
}
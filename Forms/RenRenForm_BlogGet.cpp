/*!
*@FileName: RenRenForm_BlogGet.cpp
*@Author: zengying@GoZone
*@Date: 2010-12-27
*@Brief: 日志详情Form
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#include "RenRenAPICommon.h"
#include "RenRenForm_BlogGet.h"

#define DL_BLOGGET_IMAGE_INDEX	(1)

//大头像坐标
#define RR_HEAD_X				(10)
#define RR_HEAD_Y				(TITLEBAR_H + 10)
#define RR_HEAD_W				(HEADMASKBUTTON_W)
#define RR_HEAD_H				(HEADMASKBUTTON_H)

//姓名 坐标
#define RR_NAME_FONT			(FONT_NORMAL)
#define RR_NAME_X				(RR_HEAD_X + RR_HEAD_W + 10)
#define RR_NAME_Y				(RR_HEAD_Y)
#define RR_NAME_W				(SCR_W - RR_NAME_X - 10)
#define RR_NAME_H				(RR_NAME_FONT + 4)

//Title 坐标
#define RR_TITLE_FONT			(FONT_MIDDLE)
#define RR_TITLE_X				(RR_NAME_X)
#define RR_TITLE_Y				(RR_NAME_Y + RR_NAME_H + 5)
#define RR_TITLE_W				(RR_NAME_W)
#define RR_TITLE_H				(RR_TITLE_FONT+4)

//Time 坐标
#define RR_TIME_FONT			(FONT_SMALL)
#define RR_TIME_X				(RR_NAME_X)
#define RR_TIME_Y				(RR_TITLE_Y + RR_TITLE_H + 5)
#define RR_TIME_W				(RR_NAME_W)
#define RR_TIME_H				(RR_TIME_FONT+4)

#define WEBPAL_Y				(RR_TIME_Y + RR_TIME_H + LINE_SPACE*2)
// 构造函数
TBlogGetForm::TBlogGetForm(TApplication* pApp, Boolean bNew):TWindow(pApp)
{
	Response = NULL;

	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	nHeadMaskBtnID = -1;
	pPhotoBmp = NULL;
	pPhotoBmpZoom = NULL;
	
	memset((void*)&BottomOptionCtrID, 0x0, sizeof(BottomOptionCtrID));
	memset(m_id, 0x0, sizeof(m_id));
	memset(m_uid, 0x0, sizeof(m_uid));
	
	m_pWebPanel = NULL;	
	
	Create(APP_RE_ID_BlogGetForm);
	if(bNew == TRUE)
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_DEFAULT, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	else
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_NONE, TG3_WINDOW_MOVIE_MODE_DEFAULT);	
}

// 析构函数
TBlogGetForm::~TBlogGetForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());
	if (m_pWebPanel)
	{
		//m_pWebPanel->stopLoad();
		m_pWebPanel->close();
		m_pWebPanel = NULL;
	}	

	if( pPhotoBmp != NULL)
	{
		pPhotoBmp->Destroy();
		pPhotoBmp = NULL;								
	}

	if( pPhotoBmpZoom != NULL)
	{
		pPhotoBmpZoom->Destroy();
		pPhotoBmpZoom = NULL;								
	}
		
	if(Response)
	{
		delete Response;
	}
}

// 窗口事件处理
Boolean TBlogGetForm::EventHandler(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;

	switch (pEvent->eType)
	{
			//窗口创建
		case EVENT_WinInit:
			{
				_OnWinInitEvent(pApp, pEvent);
				bHandled = TRUE;
			}
			break;
		
			//窗口关闭
		case EVENT_WinClose:
			{
				_OnWinClose(pApp, pEvent);
			}
			break;
		
			//点击控件
		case EVENT_CtrlSelect:
			{
				bHandled = _OnCtrlSelectEvent(pApp, pEvent);
			}
			break;
		
			//控件获取焦点
		case EVENT_CtrlSetFocus:
			{
				bHandled = _OnCtrlSetFocusEvent(pApp, pEvent);
			}
			break;
			
			//控件丢失焦点
		case EVENT_CtrlKillFocus :
			{
				bHandled = _OnCtrlKillFocusEvent(pApp, pEvent);
			}
			break;
			
			//输入框内容变化
		case EVENT_FieldChanged:
			{
				_OnFldChangedEvent(pApp, pEvent);
				bHandled = TRUE;
			}
			break;
			
			//窗口的背景
		case EVENT_WinEraseClient:
			{
				TDC dc(this);
				WinEraseClientEventType *pEraseEvent = reinterpret_cast< WinEraseClientEventType* >( pEvent );
				TRectangle rc(pEraseEvent->rc);
				dc.SetBackColor(RGB_COLOR_FORM_BACKGROUND);
				dc.EraseRectangle(&rc, 0);
				dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_bg), 0, 0, SCR_W,GUI_API_STYLE_ALIGNMENT_LEFT);		
				pEraseEvent->result = 1;				
				bHandled = TRUE;
			}
			break;

			// 右软键事件 
		case EVENT_KeyCommand: 
			{ 
				if( pEvent->sParam1 == SYS_KEY_SOFTKEY_RIGHT_UP || pEvent->sParam1 == SYS_KEY_SOFTKEY_RIGHT_LONG ) 
				{ 
					// 模拟标题栏右按钮选中消息 
					HitControl(m_TitleBtnRight); 
					bHandled = TRUE; 
				} 
			} 
			break;
			
			//下载完成事件
		case MSG_DL_THREAD_NOTIFY:
			{
				NotifyMsgDataType notifyData;
				Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));				
				bHandled = TRUE;
				switch(notifyData.nAccessType)
				{
					case RR_PhotoDownload:
					{
						TMaskButton* pMyHeadMBtn = static_cast<TMaskButton*>(GetControlPtr(nHeadMaskBtnID));
						if(pMyHeadMBtn)
						{
							if( pPhotoBmp != NULL)
							{
								pPhotoBmp->Destroy();
								pPhotoBmp = NULL;								
							}

							if( pPhotoBmpZoom != NULL)
							{
								pPhotoBmpZoom->Destroy();
								pPhotoBmpZoom = NULL;								
							}

							pPhotoBmp = LoadImgByPath(notifyData.pszFilePath);
							if(pPhotoBmp != NULL)
							{
								TRectangle rc;
								pMyHeadMBtn->GetBounds(&rc);

								//公共主页的图片100x100，需要做缩放50x50
								pPhotoBmpZoom = pPhotoBmp->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
								pPhotoBmpZoom->QuickSpecialZoom(pPhotoBmp, 0, 0);				
								pMyHeadMBtn->SetImage(pPhotoBmpZoom,(rc.Width()-pPhotoBmpZoom->GetWidth())/2, (rc.Height()-pPhotoBmpZoom->GetHeight())/2);
								pMyHeadMBtn->Draw();							
							}
						}
					}
						break;
					default:
						break;
				}
			}
			break;

		default:
			break;
	}

	if (!bHandled)
	{
		bHandled = TWindow::EventHandler(pApp, pEvent);
	}

	return bHandled;
}

Boolean TBlogGetForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;
	
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	SetAppTilte(this, APP_RE_ID_STRING_Blog);
	//从对应Json中获取数据,用于更新UI
	iRet = RenRenAPI_JsonParse(RR_BlogGet, (void **)&Response);
	//:TODO:
	if(Response != NULL)
	{
		TRectangle Rc(0, 0, 0, 0);
	
		//头像
		TMaskButton* pHead = new TMaskButton;
		if(pHead->Create(this))
		{
			const TBitmap * pHeadBG = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_head1);
			Rc.SetRect(RR_HEAD_X, RR_HEAD_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H);
			pHead->SetBounds(&Rc);		
			pHead->SetMaskImage(pHeadBG, pHeadBG, pHeadBG);
			pHead->SetEnabled(FALSE);
			nHeadMaskBtnID = pHead->GetId();
							
			//先读取磁盘cache, 无文件再下载
			pPhotoBmp = LoadImgByUrl(Response->headurl);
			if( pPhotoBmp == NULL)
			{
				const TBitmap * pBmp = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Default);
				pHead->SetImage(pBmp,(Rc.Width()-pBmp->GetWidth())/2, (Rc.Height()-pBmp->GetHeight())/2);				
				RenRenAPICommon_DownloadPhoto(Response->headurl, this->GetWindowHwndId(), DL_BLOGGET_IMAGE_INDEX);
			}
			else
			{
				//公共主页的图片100x100，需要做缩放50x50
				pPhotoBmpZoom = pPhotoBmp->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
				pPhotoBmpZoom->QuickSpecialZoom(pPhotoBmp, 0, 0);
				pHead->SetImage(pPhotoBmpZoom,(Rc.Width()-pPhotoBmpZoom->GetWidth())/2, (Rc.Height()-pPhotoBmpZoom->GetHeight())/2);					
			}
		}
		//姓名
		TUChar pszName[128] = {0};
		TUString::StrUtf8ToStrUnicode(pszName,(Char*)Response->name);								
		CtrlAddItemToWin_Label(this, RR_NAME_X, RR_NAME_Y, 0, RR_NAME_FONT, pszName, RGB_COLOR_LIGHTBLUE);

		//标题
		TUChar pszTitle[128] = {0};
		TUString::StrUtf8ToStrUnicode(pszTitle,(Char*)Response->title);		
		//CtrlAddItemToWin_Label(this, RR_TITLE_X, RR_TITLE_Y, RR_TITLE_W, RR_TITLE_FONT, pszTitle, RGB_COLOR_BLACK);
		Coord nHeight = 0;
		CtrlAddItemToWin_RichView(this, RR_TITLE_X, RR_TITLE_Y, RR_TITLE_W, &nHeight, RR_TITLE_FONT, pszTitle, RGB_COLOR_BLACK, FALSE);

		Coord nOffset = 0;
		nOffset = nHeight - RR_TITLE_H;
		//时间
		TUChar pszTime[128] = {0};
		TUString::StrUtf8ToStrUnicode(pszTime,(Char*)Response->time);
		CtrlAddItemToWin_Label(this, RR_TIME_X, RR_TIME_Y+nOffset, RR_TIME_W, RR_TIME_FONT, pszTime, RGB_COLOR_LIGHTGRAY);

		//将内容保存成本地html文件
		RR_MakeHtmlFile(RR_BlogGet, Response->content);
		//使用webpanel显示
		TRectangle WebRet;
		GetBounds( &WebRet);
		Int32 TopLeftX = 5;
		Int32 TopLeftY = WEBPAL_Y + nOffset;
		Int32 TopHeigh = SCR_H - STATUSBAR_H - TopLeftY;
		Int32 TopWidth = SCR_W - 5*2;
		WebRet.SetX(TopLeftX);
		WebRet.SetY(TopLeftY);
		WebRet.SetHeight(TopHeigh);
		WebRet.SetWidth(TopWidth);

	    	m_pWebPanel = TBrowser::TWebPanel::create(this, WebRet, NULL);

		TUChar* pszFilePath = RR_GetHtmlFileUrl(RR_BlogGet);
		if(pszFilePath != NULL)
		{
			m_pWebPanel->loadUrl(pszFilePath);

			delete pszFilePath;
			pszFilePath = NULL;
		}

		sprintf(m_id, "%d", Response->id);
		sprintf(m_uid, "%d", Response->uid);		

		TUChar pszButton[32];
		if(Response->comment_count > 99)
			TUString::StrCopy(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCountMuch));
		else
			TUString::StrPrintF(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCount), Response->comment_count);
		m_TitleBtnLeft = SetAppTitleButton(this, pszButton, TITLE_BUTTON_LEFT);
		
	}	

	return TRUE;
}

// 关闭窗口时，保存设置信息
Boolean TBlogGetForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// 控件点击事件处理
Boolean TBlogGetForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled;
	bHandled = FALSE;
	Int32 nCtrlID = pEvent->sParam1;

	if(m_TitleBtnRight == nCtrlID)
	{
		bHandled = TRUE; 
		this->CloseWindow();
		return bHandled;
	}
	else if(m_TitleBtnLeft == nCtrlID)
	{
		Set_Url_Params(RR_BlogGetComments, "id", m_id);
		Set_Url_Params(RR_BlogGetComments, "uid", m_uid);
		Set_Url_Params(RR_BlogGetComments, "page", "1");
		RenRenAPICommon_Download(RR_BlogGetComments, this->GetWindowHwndId(), 0, 0); 
		bHandled = TRUE; 		
		return bHandled;
	}

	switch(nCtrlID)
	{
		case APP_RE_ID_TestForm_Test01:
			{
				bHandled = TRUE; 	
			}
			break;
		default:
			break;
	}

	return bHandled;
}

//控件获取焦点
Boolean TBlogGetForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//控件丢失焦点
Boolean TBlogGetForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// 文本框输入处理事件
Boolean TBlogGetForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

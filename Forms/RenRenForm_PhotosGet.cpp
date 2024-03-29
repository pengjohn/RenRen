/*!
*@FileName: RenRenForm_PhotosGet.cpp
*@Author: xuhonglei@GoZone
*@Date: 2010-12-30
*@Brief: 专辑照片列表
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
#include "RenRenForm_PhotosGet.h"


#if(LCD_SIZE == LCD_HVGA )
#define RR_ALBUM_PHOTO_THUMB_IMAGE_W			(75)
#define RR_ALBUM_PHOTO_THUMB_IMAGE_H			(75)
#elif(LCD_SIZE == LCD_WVGA )
#define RR_ALBUM_PHOTO_THUMB_IMAGE_W			(110)
#define RR_ALBUM_PHOTO_THUMB_IMAGE_H			(110)
#endif

//相册图片间间隙
#define RR_ALBUM_PHOTO_ITEM_W_SPACE	((SCR_W - RR_ALBUM_PHOTO_THUMB_IMAGE_W*4)/5)
#define RR_ALBUM_PHOTO_ITEM_H_SPACE	(RR_ALBUM_PHOTO_ITEM_W_SPACE)

//相册缩略图坐标
#define RR_ALBUM_PHOTO_THUMB_IMAGE_X			(RR_ALBUM_PHOTO_ITEM_W_SPACE)//代码中动态变化
#define RR_ALBUM_PHOTO_THUMB_IMAGE_Y			(RR_ALBUM_PHOTO_ITEM_H_SPACE)//代码中动态变化


//相册照片数量标签坐标
#define RR_ALBUM_PHOTO_TOTAL_LABEL_X			(110)
#define RR_ALBUM_PHOTO_TOTAL_LABEL_H			(FONT_NORMAL + LINE_SPACE)

// 构造函数
TPhotosGetForm::TPhotosGetForm(TApplication* pApp):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	pPhotoBmp = NULL;
	memset(gItemData, 0, sizeof(gItemData));
	m_index = 0;
	m_AlbumPhotosCount = 0;
	m_CommentCount = 0;		
	char*  aid;	
	aid = Get_Url_Params(RR_PhotosGet, "aid");
	if(strlen(aid) != 0)	
		bAlbumMode = TRUE;
	else
		bAlbumMode = FALSE;
	
	if(bAlbumMode == TRUE)
	{
		Create(APP_RE_ID_PhotosGetForm);
	}
	else
	{
		Create(APP_RE_ID_PhotoDetailForm);
	}	
	
}

// 析构函数
TPhotosGetForm::~TPhotosGetForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());
	for(int i = 0;i<PHOTOLIST_MAXITEM ;i++)
	{
		if(gItemData[i].pThumbnailBmp != NULL)
		{
			gItemData[i].pThumbnailBmp->Destroy();
			gItemData[i].pThumbnailBmp = NULL;	
		}
	}

	if(pPhotoBmp != NULL)
	{
		pPhotoBmp->Destroy();
		pPhotoBmp = NULL;	
	}
}

// 窗口事件处理
Boolean TPhotosGetForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
				// 擦除
				dc.EraseRectangle(&rc, 0);
				dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_bg), 0, 0, SCR_W,GUI_API_STYLE_ALIGNMENT_LEFT);

				if(bAlbumMode == FALSE)
				{
					//如果是单张照片，底端操作栏的背景
					dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Bottom_btn_bg), 0, BOTTOM_OPTION_Y, SCR_W, GUI_API_STYLE_ALIGNMENT_LEFT);	
				}		
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
						if(bAlbumMode == TRUE)
						{
							//下载完后跟新缩略图图标
							TBitmap* pDownLoadBitmap = NULL;
							pDownLoadBitmap = LoadImgByPath(notifyData.pszFilePath);

							if(pDownLoadBitmap)
							{
								TImage* pTImage;

								pTImage = static_cast< TImage* >(this->GetControlPtr(this->gItemData[notifyData.nParam].nThumbnailImageID));
								if(pTImage)
								{
									TRectangle RC;
									pTImage->GetBounds(&RC);
									if(gItemData[notifyData.nParam].pThumbnailBmp != NULL)
									{
										gItemData[notifyData.nParam].pThumbnailBmp->Destroy();
										gItemData[notifyData.nParam].pThumbnailBmp = NULL;	
									}
									
									gItemData[notifyData.nParam].pThumbnailBmp = TBitmap::Create(RC.Width(), RC.Height(), pDownLoadBitmap->GetDepth());
									gItemData[notifyData.nParam].pThumbnailBmp->QuickZoom(pDownLoadBitmap, FALSE, TRUE,RGBA(100, 160, 160, 255));						
									pTImage->SetBitmap(gItemData[notifyData.nParam].pThumbnailBmp);
									pTImage->Draw();
								}
								pDownLoadBitmap->Destroy();
								pDownLoadBitmap = NULL;
							}
						}
						else
						{
							//下载完后更新图片
							TBitmap* pDownLoadBitmap = NULL;
							pDownLoadBitmap = LoadImgByPath(notifyData.pszFilePath);

							if(pDownLoadBitmap)
							{
								//显示保存图片按钮
								this->ShowControl(APP_RE_ID_PhotoDetailForm_SavePhotoBtn);	
								TUString::StrCopy(pszFilePath, notifyData.pszFilePath);
		
								TImage* pTImage;
								pTImage = static_cast< TImage* >(this->GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoImage));
								if(pTImage)
								{
									TRectangle RC;
									pTImage->GetBounds(&RC);

									if(pPhotoBmp != NULL)
									{
										pPhotoBmp->Destroy();
										pPhotoBmp = NULL;	
									}
									pPhotoBmp = TBitmap::Create(RC.Width(), RC.Height(), pDownLoadBitmap->GetDepth());
									pPhotoBmp->QuickZoom(pDownLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));

									pTImage->SetBitmap(pPhotoBmp);
									pTImage->Draw();
								}
								pDownLoadBitmap->Destroy();
								pDownLoadBitmap = NULL;
							}	
						}						
						break;
					}			
				case RR_SharePublish:
					{
					pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_Success),TResource::LoadConstString(APP_RE_ID_STRING_Share),WMB_OK); 
					}
					break;					
				}
				break;
				bHandled = TRUE;
			}
			break;

		case MSG_POST_THREAD_NOTIFY:
		{
			PostNotifyMsgDataType notifyData;
			Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(PostNotifyMsgDataType));
		
			char*  aid = NULL;
			char*  uid = NULL;
			aid = Get_Url_Params(RR_PhotosGet, "aid");
			Set_Url_Params(RR_PhotosAddComment, "aid", aid);

			uid = Get_Url_Params(RR_PhotosGet, "uid");
			Set_Url_Params(RR_PhotosAddComment, "uid", uid);
			
			Set_Url_Params(RR_PhotosAddComment, "content", (char*)notifyData.pszEditContent);					
			
			RenRenAPICommon_Download(RR_PhotosAddComment, this->GetWindowHwndId(), 0, 0);	
	
			bHandled = TRUE;
			break;
		}
		default:
			break;
	}

	if (!bHandled)
	{
		bHandled = TWindow::EventHandler(pApp, pEvent);
	}

	return bHandled;
}

// 窗口初始化
Boolean TPhotosGetForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;

	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	//SetAppTilte(this, APP_RE_ID_STRING_RenRen);

	//从对应Json中获取数据,用于更新UI
	tResponsePhotosGet* Response = NULL;
	iRet = RenRenAPI_JsonParse(RR_PhotosGet, (void **)&Response);//正确时返回(0) 

	if (Response != NULL)
	{   		
		if(bAlbumMode == TRUE)
		{			
			_SetFormTilte(pApp);	//设置title，同时获取评论数和专辑照片总数
			TUChar pszButton[32];
			if(m_CommentCount > 99)
				TUString::StrCopy(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCountMuch));
			else
				TUString::StrPrintF(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCount), m_CommentCount);
			m_TitleBtnLeft = SetAppTitleButton(this, pszButton, TITLE_BUTTON_LEFT);
			
			_CreateAlbumPhotos(pApp, Response);	
		}
		else
		{
			_SetCtrlsStyle(pApp);
			_SetCtrlsData(pApp, Response);			

			TUChar pszButton[32];
			if(Response->Array[0].comment_count > 99)
				TUString::StrCopy(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCountMuch));
			else
				TUString::StrPrintF(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCount), Response->Array[0].comment_count);
			m_TitleBtnLeft = SetAppTitleButton(this, pszButton, TITLE_BUTTON_LEFT);
		}

		delete Response;
		Response = NULL;		
	}
	
	return TRUE;
}

// 关闭窗口时，保存设置信息
Boolean TPhotosGetForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// 控件点击事件处理
Boolean TPhotosGetForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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
		if(bAlbumMode == TRUE)
		{
			char* aid = Get_Url_Params(RR_PhotosGet, "aid");
			char* uid = Get_Url_Params(RR_PhotosGet, "uid");

			Set_Url_Params(RR_PhotosGetComments, "uid", uid);
			Set_Url_Params(RR_PhotosGetComments, "pid", "");
			Set_Url_Params(RR_PhotosGetComments, "aid", aid);
			Set_Url_Params(RR_PhotosGetComments, "order", "1");
			Set_Url_Params(RR_PhotosGetComments, "page", "1");
			RenRenAPICommon_Download(RR_PhotosGetComments, this->GetWindowHwndId(), 0, 0); 		
			bHandled = TRUE; 
		}
		else
		{
			char* aids = Get_Url_Params(RR_PhotosGet, "aid");
			char* uids = Get_Url_Params(RR_PhotosGet, "uid");
			char* pids = Get_Url_Params(RR_PhotosGet, "pids");

			Set_Url_Params(RR_PhotosGetComments, "uid", uids);
			Set_Url_Params(RR_PhotosGetComments, "pid", pids);
			Set_Url_Params(RR_PhotosGetComments, "aid", aids);
			Set_Url_Params(RR_PhotosGetComments, "order", "1");
			Set_Url_Params(RR_PhotosGetComments, "page", "1");
			RenRenAPICommon_Download(RR_PhotosGetComments, this->GetWindowHwndId(), 0, 0); 		
			bHandled = TRUE; 
		}
		
		return bHandled;
	}

	if(nCtrlID)
	{
		if(nCtrlID == m_TitleButtonCtlID)
		{
			//返回专辑列表
			CloseWindow();
		}
		else
		{
			for(int i = 0; i < this->m_nAlbumPhotoCount; i++)
			{
				if(nCtrlID == this->gItemData[i].nThumbnailImageID)
				{
					TPhotoDetailForm* pWin = new TPhotoDetailForm(pApp, i, m_nAlbumPhotoCount);			
					bHandled = TRUE;
					break;
				}
			}
		}

	}

	switch(nCtrlID)
	{
		case APP_RE_ID_PhotoDetailForm_SharePhotoBtn:
			{
				Set_Url_Params(RR_SharePublish, "type", SHARE_TYPE_PHOTO);
				Set_Url_Params(RR_SharePublish, "share_data", "{\"link\":\"\",\"pic\":\"\",\"title\":\"\",\"summary\":\"\",\"comment\":\"\"}");
				Set_Url_Params(RR_SharePublish, "id", m_pid);
				Set_Url_Params(RR_SharePublish, "uid", m_uid);
				RenRenAPICommon_Download(RR_SharePublish, this->GetWindowHwndId(), 0, 0);
				bHandled = TRUE; 
				break;
			}
		default:
			break;
	}

	return bHandled;
}

//控件获取焦点
Boolean TPhotosGetForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//控件丢失焦点
Boolean TPhotosGetForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// 文本框输入处理事件
Boolean TPhotosGetForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

/**
 * 显示专辑图片列表
 *
 * \param pApp
 * \param Response
 *
 * \return 
 */
Int32 TPhotosGetForm::_CreateAlbumPhotos(TApplication* pApp, tResponsePhotosGet* Response)
{
		TPanel* pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_PhotosGetForm_PhotosGetPanel));
		if(pTPanel)
		{
			//设置最大显示数，放置数组溢出
			m_nAlbumPhotoCount = (Response->nArraySize < PHOTOLIST_MAXITEM)?Response->nArraySize:PHOTOLIST_MAXITEM;

			TRectangle obBtnRec(RR_ALBUM_PHOTO_THUMB_IMAGE_X, RR_ALBUM_PHOTO_THUMB_IMAGE_Y, RR_ALBUM_PHOTO_THUMB_IMAGE_W, RR_ALBUM_PHOTO_THUMB_IMAGE_H);
			TFont tFont;
			TUChar pszTotal[32]={0};
			TUChar pszShowTotalString[64]={0};

			for(int i = 0; i < m_nAlbumPhotoCount; i++)
			{
				RenRenAPICommon_DownloadPhoto(Response->Array[i].url_head, this->GetWindowHwndId(), i);
				//创建TImage 
				gItemData[i].pThumbnailImage = new TImage;	
				obBtnRec.SetRect(RR_ALBUM_PHOTO_THUMB_IMAGE_X + (RR_ALBUM_PHOTO_THUMB_IMAGE_W + RR_ALBUM_PHOTO_ITEM_H_SPACE) * (i%4), RR_ALBUM_PHOTO_THUMB_IMAGE_Y + (RR_ALBUM_PHOTO_THUMB_IMAGE_H + RR_ALBUM_PHOTO_ITEM_H_SPACE) * (i/4) ,RR_ALBUM_PHOTO_THUMB_IMAGE_W,RR_ALBUM_PHOTO_THUMB_IMAGE_H);

				if(gItemData[i].pThumbnailImage->Create(pTPanel))
				{
					gItemData[i].pThumbnailImage->SetBounds(&obBtnRec);
					gItemData[i].nThumbnailImageID = gItemData[i].pThumbnailImage->GetId();//save TImage ID
					gItemData[i].pThumbnailImage->SetBitmapByResId(APP_RE_ID_BITMAP_albumdefaultpic);
					gItemData[i].pThumbnailImage->SetEnabled(TRUE);
					gItemData[i].pThumbnailImage->SetTransVisible(TRUE);
					gItemData[i].pThumbnailImage->Show(TRUE);
				}
			}

			//创建标签"共 *  张照片"
			TLabel* pTotalPhotoLabel =new TLabel;
			//因为相册照片总数是由PhotosGetAlbums中获得的,
			//而如果跳过PhotosGetAlbums直接PhotosGe(如新鲜事中直接读相册), 
			//就获取不到总数了
			//这里做个不完美的容错,若总数小于获取的相片数,则调整总数
			if(m_AlbumPhotosCount < m_nAlbumPhotoCount)
				m_AlbumPhotosCount = m_nAlbumPhotoCount;
				
			TUString::StrPrintF(pszShowTotalString, TResource::LoadConstString(APP_RE_ID_STRING_AlbumPhotosCountFormat),m_nAlbumPhotoCount, m_AlbumPhotosCount);

			Int32 nRow = 0;
			if(m_nAlbumPhotoCount%4)
			{
				nRow = m_nAlbumPhotoCount/4 + 1;
			}
			else
			{
				nRow = m_nAlbumPhotoCount/4;
			}

			if(pTotalPhotoLabel->Create(pTPanel))
			{		
				Int32 nWidth = 0;
				pTotalPhotoLabel->SetAutoSize(FALSE);
				pTotalPhotoLabel->SetCaption(pszShowTotalString,FALSE);
				tFont = pTotalPhotoLabel->GetFont();
				tFont.Create(FONT_NORMAL, FONT_NORMAL);
				nWidth = GetShowAllStringWidth(pszShowTotalString, tFont);
				obBtnRec.SetRect(RR_ALBUM_PHOTO_TOTAL_LABEL_X,RR_ALBUM_PHOTO_THUMB_IMAGE_Y + (RR_ALBUM_PHOTO_THUMB_IMAGE_H + RR_ALBUM_PHOTO_ITEM_H_SPACE) * nRow,nWidth,RR_ALBUM_PHOTO_TOTAL_LABEL_H);
				pTotalPhotoLabel->SetFont(tFont);
				pTotalPhotoLabel->SetBounds(&obBtnRec);					
				pTotalPhotoLabel->SetColor(CTL_COLOR_TYPE_FORE, RGB_COLOR_BLACK);
				pTotalPhotoLabel->SetEnabled(FALSE);
				pTotalPhotoLabel->Show(TRUE);		
			}
		}
	return 0;
}


/**
 * 创建标题标签 
 *
 * \param pApp
 *
 * \return 
 */
Int32 TPhotosGetForm::_SetFormTilte(TApplication * pApp)
{
	TUChar pszAlbumTitle[256] = {0};

	char* szAlbumId = NULL;
	szAlbumId = (char*)malloc(32);
	memset(szAlbumId, 0, sizeof(32));

	szAlbumId = Get_Url_Params(RR_PhotosGet, "aid");

	int szAlbumIdInt = atoi(szAlbumId);

	int iRet = eFailed;
	tResponsePhotosGetAlbums* ResponsePhotosGetAlbums = NULL;
	iRet = RenRenAPI_JsonParse(RR_PhotosGetAlbums, (void **)&ResponsePhotosGetAlbums);
	iRet += 1;
	m_AlbumPhotosCount = 0;
	m_CommentCount = 0;	
	if(iRet == eSucceed)
	{
		for(int i = 0; i < ResponsePhotosGetAlbums->nArraySize; i++)
		{
			if((ResponsePhotosGetAlbums->Array[i].aid == szAlbumIdInt) || (ResponsePhotosGetAlbums->Array[i].aid == 0))
			{	
				TUString::StrUtf8ToStrUnicode(pszAlbumTitle, (const Char *)ResponsePhotosGetAlbums->Array[i].name);
				m_AlbumPhotosCount = ResponsePhotosGetAlbums->Array[i].size;
				m_CommentCount = ResponsePhotosGetAlbums->Array[i].comment_count;
				break;
			}
		}
	}

	if(ResponsePhotosGetAlbums)
	{
		delete ResponsePhotosGetAlbums;
		ResponsePhotosGetAlbums = NULL;
	}

 	//free(szAlbumId);

	return SetAppTilte(this, 0, pszAlbumTitle);;
}


/**
 * 显示图片，并显示图片的位置信息
 *
 * \param pApp
 * \param Response
 *
 * \return 
 */
Int32 TPhotosGetForm::_SetCtrlsData(TApplication* pApp, tResponsePhotosGet* Response)
{
	//保存uid/pid
	sprintf(m_uid, "%d", Response->Array[m_index].uid);
	TUChar wszPid[64]={0};
	TUString::StrI64ToA(wszPid,(Int64)Response->Array[m_index].pid);
	TUString::StrUnicodeToStrUtf8((Char*)m_pid, wszPid);
	DoubleToChar(Response->Array[m_index].pid, m_pid);

	//如果是自己的,则隐藏分享按钮
	if( strcmp(RenRenUserInfo.szuid, m_uid) == 0)
	{
		TButton* pSharePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_SharePhotoBtn)); 
		if(pSharePhotoBtn)
		{		
			pSharePhotoBtn->SetEnabled(FALSE);
			pSharePhotoBtn->Hide(TRUE);
		}
	}
	
	//下载图片
	TBitmap* pLoadBitmap = NULL;

	//先读取大图片, 如果大图片不存在则先显示小图片
	pLoadBitmap = LoadImgByUrl(Response->Array[m_index].url_large);

	if(pLoadBitmap == NULL)
	{
		RenRenAPICommon_DownloadPhoto(Response->Array[m_index].url_large, this->GetWindowHwndId(), m_index);
		//隐藏图片保存按钮
		this->HideControl(APP_RE_ID_PhotoDetailForm_SavePhotoBtn);	

		//开始下载图片，查看按钮暂时不可用
		_SetViewButtonEnabled(FALSE);

		pLoadBitmap = LoadImgByUrl(Response->Array[m_index].url_head);
	}
	else
	{	
		//显示保存图片按钮
		this->ShowControl(APP_RE_ID_PhotoDetailForm_SavePhotoBtn);	
		TUString::StrCopy(pszFilePath, FileNameFullPathFromUrl(Response->Array[m_index].url_large));
	}
	
	if(pLoadBitmap)
	{
		TImage* pTImage;	
		pTImage = static_cast< TImage* >(this->GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoImage));
		if(pTImage)
		{
			TRectangle RC;
			pTImage->GetBounds(&RC);

			if(pPhotoBmp != NULL)
			{
				pPhotoBmp->Destroy();
				pPhotoBmp = NULL;	
			}
			pPhotoBmp = TBitmap::Create(RC.Width(), RC.Height(), pLoadBitmap->GetDepth());
			pPhotoBmp->QuickZoom(pLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));

			pTImage->SetBitmap(pPhotoBmp);
			pTImage->Draw();
		}

		pLoadBitmap->Destroy();
		pLoadBitmap = NULL; 
	}

	TUChar pszTitle[RENREN_LENGTH_PHOTO_COMMENT+1]={0};
	TUString::StrUtf8ToStrUnicode(pszTitle, (Char *)Response->Array[m_index].caption);

	TPanel* pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoDetailPanel));
	Coord	nHeight = 0;
	CtrlAddItemToPanel_RichView(this, pTPanel, 5, SCR_H-STATUSBAR_H-TITLEBAR_H-BOTTOM_TAB_H-FONT_NORMAL-8, SCR_W - 5*2, &nHeight, FONT_NORMAL, pszTitle, RGB_COLOR_WHITE);

//	CtrlAddItemToPanel_RichView(this, pTPanel, 5, 560, 470, &nHeight, FONT_NORMAL, pszTitle, RGB_COLOR_WHITE);

#if 0
	TRichView* pTotalPhotoLabel =static_cast<TRichView*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoTitleRichview));	
	if(pTotalPhotoLabel)
	{						
		pTotalPhotoLabel->SetCaption(pszTitle,FALSE);		
	}
#endif
	_SetViewButtonEnabled(FALSE);

	return 0;
}

/**
 * 控制翻页按钮是否可用
 *
 * \param bEnabled
 *
 * \return 
 */
Int32 TPhotosGetForm::_SetViewButtonEnabled(Boolean bEnabled)
{
	TButton* pPrePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PrePhotoBtn)); 
	TButton* pNextPhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_NexPhotoBtn)); 
	if(pPrePhotoBtn && pNextPhotoBtn)
	{		
		pPrePhotoBtn->SetEnabled(bEnabled);
		pPrePhotoBtn->Hide(TRUE);
		pNextPhotoBtn->SetEnabled(bEnabled);
		pNextPhotoBtn->Hide(TRUE);
	}
	return 0;
}


/**
 * 设置显示风格
 *
 * \param pApp
 *
 * \return 
 */
Int32 TPhotosGetForm::_SetCtrlsStyle(TApplication* pApp)
{
	TFont tFont;

	TPanel* pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoDetailPanel));
	if(pTPanel)
	{		
		pTPanel->SetTransparent(FALSE);
		pTPanel->SetColor(CTL_COLOR_TYPE_BACK, RGB_COLOR_BLACK);
	}

#if 0
	TRichView* pTotalPhotoLabel =static_cast<TRichView*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoTitleRichview)); 
	if(pTotalPhotoLabel)
	{		
//		pTotalPhotoLabel->SetAutoSize(TRUE);				
		tFont = pTotalPhotoLabel->GetFont();
		tFont.Create(FONT_NORMAL, FONT_NORMAL);
		pTotalPhotoLabel->SetFont(tFont);
		pTotalPhotoLabel->SetColor(CTL_COLOR_TYPE_FORE, RGB_COLOR_WHITE);
		pTotalPhotoLabel->SetEnabled(FALSE);
		pTotalPhotoLabel->Show(TRUE);		
	}
#endif

	TButton* pPrePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PrePhotoBtn)); 
	if(pPrePhotoBtn)
	{		
		tFont.Create(FONT_BUTTON_CAPTION, FONT_BUTTON_CAPTION);
		pPrePhotoBtn->SetFont(tFont);
		pPrePhotoBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pPrePhotoBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pPrePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pPrePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
		pPrePhotoBtn->SetTransparent(TRUE);
	}

	TButton* pNextPhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_NexPhotoBtn)); 
	if(pNextPhotoBtn)
	{		
		tFont.Create(FONT_BUTTON_CAPTION, FONT_BUTTON_CAPTION);
		pNextPhotoBtn->SetFont(tFont);
		pNextPhotoBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pNextPhotoBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pNextPhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pNextPhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
		pNextPhotoBtn->SetTransparent(TRUE);
	}

	TButton* pSharePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_SharePhotoBtn)); 
	if(pSharePhotoBtn)
	{		
		tFont.Create(FONT_BUTTON_CAPTION, FONT_BUTTON_CAPTION);
		pSharePhotoBtn->SetFont(tFont);
		pSharePhotoBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pSharePhotoBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pSharePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pSharePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
	}

	TButton* pSavePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_SavePhotoBtn)); 
	if(pSavePhotoBtn)
	{		
		tFont.Create(FONT_BUTTON_CAPTION, FONT_BUTTON_CAPTION);
		pSavePhotoBtn->SetFont(tFont);
		pSavePhotoBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pSavePhotoBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pSavePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pSavePhotoBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
	}
	
	TImage  *pImage = NULL;
	pImage = static_cast<TImage*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoCommentBitmap));
	if (pImage)
	{
		pImage->SetTransparent(TRUE);
		pImage->Hide(FALSE);//评论按钮暂时隐藏
	}	
	return 0;
}


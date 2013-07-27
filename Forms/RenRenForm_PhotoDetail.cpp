/*!
*@FileName: RenRenForm_PhotoDetail.cpp
*@Author: xuhonglei@GoZone
*@Date: 2011-1-4
*@Brief: 照片详情
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
#include "RenRenForm_PhotoDetail.h"

//构造函数
TPhotoDetailForm::TPhotoDetailForm(TApplication* pApp, int i, int m_nAlbumPhotoCount, Boolean bSingleMode):TWindow(pApp)
{
	pPhotoBmp = NULL;
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	m_TitleCrtlID = -1;
	m_CaptionID = -1;
	m_photosCount = m_nAlbumPhotoCount;
	m_index = i;
	m_photosPos = ++i;
	m_bSingleMode = bSingleMode;
		
	memset(m_uid, 0x0, sizeof(m_uid));
	memset(m_pid, 0x0, sizeof(m_pid));
	Create(APP_RE_ID_PhotoDetailForm);
}

// 析构函数
TPhotoDetailForm::~TPhotoDetailForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());
	if( pPhotoBmp != NULL)
	{
		pPhotoBmp->Destroy();
		pPhotoBmp = NULL; 								
	}		
}

// 窗口事件处理
Boolean TPhotoDetailForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
				TRectangle rcBack(5, 142, 310, 314);
				this->GetBounds(&rcBack);

				dc.SetBackColor(RGB_COLOR_FORM_BACKGROUND);
				// 擦除
				dc.EraseRectangle(&rc, 0);
				dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_bg), 0, 0, SCR_W,GUI_API_STYLE_ALIGNMENT_LEFT);
				//底端操作栏的背景
				dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Bottom_btn_bg), 0, BOTTOM_OPTION_Y, SCR_W, GUI_API_STYLE_ALIGNMENT_LEFT);	
				
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
						//下载完后更新图片
						TBitmap* pDownLoadBitmap = NULL;
						pDownLoadBitmap = LoadImgByPath(notifyData.pszFilePath);

						if(pDownLoadBitmap)
						{
							TImage* pTImage;

							//显示保存图片按钮
							TUString::StrCopy(pszFilePath, notifyData.pszFilePath);
							this->ShowControl(APP_RE_ID_PhotoDetailForm_SavePhotoBtn);
							
							pTImage = static_cast< TImage* >(this->GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoImage));
							if(pTImage)
							{
								TRectangle RC;
								pTImage->GetBounds(&RC);

								//一定要先释放后使用
								if( pPhotoBmp != NULL)
								{
									pPhotoBmp->Destroy();
									pPhotoBmp = NULL; 								
								}	

								int ww = RC.Width();
								int hh = RC.Height();
								pPhotoBmp = TBitmap::Create(RC.Width(), RC.Height(), pDownLoadBitmap->GetDepth());
								pPhotoBmp->QuickZoom(pDownLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));

								pTImage->SetBitmap(pPhotoBmp);
								//pTImage->SetDrawColor(COLOR_BLACK,TRUE);
								pTImage->Draw();
							}
							pDownLoadBitmap->Destroy();
							pDownLoadBitmap = NULL;
						}	

						if (m_photosCount > 1)
						{
							_SetViewButtonEnabled(TRUE);//图片下载完毕，查看按钮即时变为可用
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
Boolean TPhotoDetailForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;
	tResponsePhotosGet* Response = NULL;
	//从对应Json中获取数据,用于更新UI
	iRet = RenRenAPI_JsonParse(RR_PhotosGet, (void **)&Response);
	iRet += 1;
	
	//m_TitleBtnLeft = SetAppTitleButton(this, APP_RE_ID_STRING_Comment,TITLE_BUTTON_LEFT);
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);

	if (iRet == eSucceed)
	{	
		_SetCtrlsStyle(pApp);
		_SetCtrlsData(pApp);
	}
	
	if( Response )
	{
		delete Response;	
	}
	return TRUE;
}


// 关闭窗口时，保存设置信息
Boolean TPhotoDetailForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// 控件点击事件处理
Boolean TPhotoDetailForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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

		/*char* aids = Get_Url_Params(RR_PhotosGet, "aid");
		char* uids = Get_Url_Params(RR_PhotosGet, "uid");
		char* pids = Get_Url_Params(RR_PhotosGet, "pids");

		Set_Url_Params(RR_PhotosGetComments, "uid", uids);
		Set_Url_Params(RR_PhotosGetComments, "pid", pids);
		Set_Url_Params(RR_PhotosGetComments, "aid", aids);
		Set_Url_Params(RR_PhotosGetComments, "order", "1");*/

		Set_Url_Params(RR_PhotosGetComments, "uid", m_uid);
		Set_Url_Params(RR_PhotosGetComments, "pid", m_pid);
		Set_Url_Params(RR_PhotosGetComments, "aid", "");
		Set_Url_Params(RR_PhotosGetComments, "order", "1");
		Set_Url_Params(RR_PhotosGetComments, "page", "1");
		
		RenRenAPICommon_Download(RR_PhotosGetComments, this->GetWindowHwndId(), 0, 0); 			
		bHandled = TRUE; 		
		return bHandled;
	}

	if(nCtrlID)
	{
		if(nCtrlID == m_TitleButtonCtlID)
		{
			//返回前一个页面
			CloseWindow();
		}
	}

	if (!bHandled)
	{
		switch(pEvent->sParam1)
		{	
		case APP_RE_ID_PhotoDetailForm_PrePhotoBtn:
			{
				_ViewPhoto(pApp, RR_PrePhoto);
				bHandled = TRUE;
				break;
			}
		case APP_RE_ID_PhotoDetailForm_NexPhotoBtn:
			{
				_ViewPhoto(pApp, RR_NextPhoto);
				bHandled = TRUE;
				break;
			}
		case APP_RE_ID_PhotoDetailForm_SharePhotoBtn:
			{
				Set_Url_Params(RR_SharePublish, "type", SHARE_TYPE_PHOTO);
				Set_Url_Params(RR_SharePublish, "share_data", "{\"link\":\"\",\"pic\":\"\",\"title\":\"\",\"summary\":\"\",\"comment\":\"\"}");
				Set_Url_Params(RR_SharePublish, "id", m_pid);
				Set_Url_Params(RR_SharePublish, "uid", m_uid);
				
				RenRenAPICommon_Download(RR_SharePublish, this->GetWindowHwndId(), 0, 0);
				break;
			}
		case APP_RE_ID_PhotoDetailForm_SavePhotoBtn:
			{
				PopBox_SavePhoto(pApp, pszFilePath);
				break;
			}
		default:
			break;		
		}
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

/**
 * 通过翻页按钮浏览图片
 *
 * \param pApp
 * \param ShowType
 *
 * \return 
 */
Int32 TPhotoDetailForm::_ViewPhoto(TApplication* pApp, RR_ViewPhotoType ShowType)
{
	int iRet = eFailed;
	tResponsePhotosGet* ResponseTmp = NULL;
	iRet = RenRenAPI_JsonParse(RR_PhotosGet, (void **)&ResponseTmp);
	if(m_photosCount == 1)
	{
		return 0;//无上下张可看 直接返回
	}

	if(ResponseTmp)
	{
		delete ResponseTmp;
		ResponseTmp = NULL;
		
		int CurrIndex = m_index;
		int MaxIndex =  m_photosCount - 1;
		int NewIndex = 0;
		switch(ShowType)
		{	
		case RR_PrePhoto:
			{	
				NewIndex = ((CurrIndex - 1) >=0)?(CurrIndex - 1):MaxIndex;
				break;
			}
		case RR_NextPhoto:
			{
				NewIndex = ((CurrIndex + 1) <=MaxIndex)?(CurrIndex + 1):0;
				break;
			}
		default:
			break;		
		}

		m_index = NewIndex;
		m_photosPos = ++NewIndex;
		//确保浏览图片时候显示
		_SetCtrlsData(pApp);
	}
	else
	{
		//若取不到专辑数据则查看按钮也不可使用
		_SetViewButtonEnabled(FALSE);
	}

	return 0;

}

/**
 * 显示图片，并显示图片的位置信息
 *
 * \param pApp
 *
 * \return 
 */
Int32 TPhotoDetailForm::_SetCtrlsData(TApplication* pApp)
{
	//下载图片
	tResponsePhotosGet* Response = NULL;
	RenRenAPI_JsonParse(RR_PhotosGet, (void **)&Response);

	if(Response == NULL)
		return -1;
	
	TUChar pszButton[32];
	if(Response->Array[m_index].comment_count > 99)
		TUString::StrCopy(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCountMuch));
	else
		TUString::StrPrintF(pszButton, TResource::LoadConstString(APP_RE_ID_STRING_CommentCount), Response->Array[m_index].comment_count);
	m_TitleBtnLeft = SetAppTitleButton(this, pszButton, TITLE_BUTTON_LEFT);
		
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
	
	TBitmap* pLoadBitmap = NULL;
	//先读取大图片, 如果不图片不存在则先显示小图片
	pLoadBitmap = LoadImgByUrl(Response->Array[m_index].url_large);
	if(pLoadBitmap == NULL)
	{
		RenRenAPICommon_DownloadPhoto(Response->Array[m_index].url_large, this->GetWindowHwndId(), m_index);
		//隐藏保存按钮
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
		_SetViewButtonEnabled(TRUE);
	}

	if(pLoadBitmap)
	{
		TImage* pTImage;	
		pTImage = static_cast< TImage* >(this->GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoImage));
		if(pTImage)
		{
			TRectangle RC;
			pTImage->GetBounds(&RC);

			//一定要先释放后使用
			if( pPhotoBmp != NULL)
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


	//显示照片Title
	TUChar pszTitle[RENREN_LENGTH_PHOTO_COMMENT+1]={0};
	TUChar pszNum[256]={0};
	TUChar pszPicPos[32]={0};
	TUChar pszPicNum[32]={0};

	TUString::StrUtf8ToStrUnicode(pszTitle, (Char *)Response->Array[m_index].caption);
	TUString::StrIToA(pszPicNum, Response->nArraySize);		
	TUString::StrIToA(pszPicPos, m_photosPos);

	TUString::StrCat(pszNum, TUSTR_Re_Left_Parenthesis);
	TUString::StrCat(pszNum, pszPicPos);
	TUString::StrCat(pszNum, TUSTR_Re_Anti_Slash);
	TUString::StrCat(pszNum, pszPicNum);			
	TUString::StrCat(pszNum, TUSTR_Re_Right_Parenthesis);

	//先销毁旧的title Ctrl
	if(m_TitleCrtlID != -1)
	{
		TCtrl *m_TitleContrl = this->GetControlPtr(m_TitleCrtlID);
		if(m_TitleContrl != NULL)
		{
			m_TitleContrl->SetCaption(pszNum, FALSE);
		}
	}
	else
	{
		if(m_bSingleMode == FALSE)
			m_TitleCrtlID = SetAppTilte(this, 0, pszNum);
	}

	TPanel* pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PhotoDetailPanel));
	Coord	nHeight = 0;
	if(m_CaptionID != -1)
	{
		TCtrl* captionCtrl = this->GetControlPtr(m_CaptionID);
		if(captionCtrl)
		{
			this->DestroyControl(captionCtrl);
		}
		m_CaptionID = -1;
	}
	
	m_CaptionID = CtrlAddItemToPanel_RichView(this, pTPanel, 5, SCR_H-STATUSBAR_H-TITLEBAR_H-BOTTOM_TAB_H-FONT_NORMAL-8, SCR_W - 5*2, &nHeight, FONT_NORMAL, pszTitle, RGB_COLOR_WHITE);
	pTPanel->Draw();

	if(Response)
	{
		delete Response;
		Response = NULL;
	}
	return 0;
}

/**
 * 控制翻页按钮是否可用
 *
 * \param bEnabled
 *
 * \return 
 */
Int32 TPhotoDetailForm::_SetViewButtonEnabled(Boolean bEnabled)
{
	TButton* pPrePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PrePhotoBtn)); 
	TButton* pNextPhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_NexPhotoBtn)); 
	if(pPrePhotoBtn && pNextPhotoBtn)
	{		
		pPrePhotoBtn->SetEnabled(bEnabled);
		pNextPhotoBtn->SetEnabled(bEnabled);
		//第一张不显示上一张，最后一张不显示下一张
		if(bEnabled == TRUE)
		{
			if(m_index ==0)
				pPrePhotoBtn->SetEnabled(FALSE);
			
			if(m_index ==m_photosCount-1)
				pNextPhotoBtn->SetEnabled(FALSE);
		}
	}
	return 0;
}

/**
 * 控制翻页按钮是否可见
 *
 * \param bEnabled
 *
 * \return 
 */
Int32 TPhotoDetailForm::_SetViewButtonVisiable(Boolean bEnabled)
{
	TButton* pPrePhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_PrePhotoBtn)); 
	TButton* pNextPhotoBtn =static_cast<TButton*>(GetControlPtr(APP_RE_ID_PhotoDetailForm_NexPhotoBtn)); 
	if(pPrePhotoBtn && pNextPhotoBtn)
	{	
		if(bEnabled)
		{
			pPrePhotoBtn->Show(FALSE);
			pNextPhotoBtn->Show(FALSE);
		}
		else
		{
			pPrePhotoBtn->Hide(FALSE);
			pNextPhotoBtn->Hide(FALSE);
		}
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
Int32 TPhotoDetailForm::_SetCtrlsStyle(TApplication* pApp)
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
		//pTotalPhotoLabel->SetAutoSize(TRUE);				
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

	if(m_bSingleMode == TRUE)
	{
		_SetViewButtonVisiable(FALSE);
	}
	else
	{
		if(m_photosCount == 1)//如果该相册只有一张图片
		{
			//查看按钮不可用
			_SetViewButtonEnabled(FALSE);
		}	
	}
	return 0;
}

//控件获取焦点
Boolean TPhotoDetailForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//控件丢失焦点
Boolean TPhotoDetailForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// 文本框输入处理事件
Boolean TPhotoDetailForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

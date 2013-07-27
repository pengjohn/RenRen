/*!
*@FileName: RenRenForm_GuestbookGet.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-30
*@Brief: ���Ա�Form
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
#include "RenRenForm_GuestbookGet.h"

#define FEED_PROFILE_IMAGE_INDEX		0xABCDEF

#define HEADMASK_X		(3)
#define HEADMASK_Y		(3)

#define NAME_FONT		(FONT_NORMAL)
#define NAME_X			(HEADMASKBUTTON_W + 10)
#define NAME_Y			(5)
#define NAME_H			(NAME_FONT + LINE_SPACE*2)

#define BUTTON_REPLY_W		(TITLE_BUTTON_W)
#define BUTTON_REPLY_H		(NAME_H)
#define BUTTON_REPLY_X         (SCR_W - BUTTON_REPLY_W - 20)
#define BUTTON_REPLY_Y         (NAME_Y)

#define WHISPER_X       (SCR_W/2)
#define WHISPER_Y       (NAME_Y)

//Content�ĸ߶��Ƕ�̬��
#define CONTENT_FONT	(FONT_NORMAL)
#define CONTENT_X		(NAME_X)
#define CONTENT_Y		(NAME_Y + NAME_H)
#define CONTENT_W		(SCR_W - CONTENT_X - 10)

//Time��Y��Ҫ����Content�ĸ߶ȶ�̬�仯
#define TIME_FONT		(FONT_SMALL)
#define TIME_X			(NAME_X)


//��ͷ������
#define RR_HEAD_X				(10)
#define RR_HEAD_Y				(10)
#define RR_HEAD_W				(100)
#define RR_HEAD_H				(100)

//���� ����
#define RR_NAME_X				(RR_HEAD_X + RR_HEAD_W + 10)
#define RR_NAME_Y				(10)
#define RR_NAME_W				(SCR_W - RR_NAME_X - 10)
#define RR_NAME_H				(FONT_NORMAL + 4)

//Status ����
#define RR_STATUS_X				(RR_NAME_X)
#define RR_STATUS_Y				(RR_NAME_Y + RR_NAME_H + 5)
#define RR_STATUS_W			(RR_NAME_W)
#define RR_STATUS_H				(20)

//��ʾ�ı�, ��"û��������"��
#define RR_INFO_X			(10)
#define RR_INFO_Y			(12)
#define RR_INFO_W			(SCR_W - RR_INFO_X - 10)
#define RR_INFO_H			(40)

// ���캯��
TGuestbookGetForm::TGuestbookGetForm(TApplication* pApp, eFormMode nFormMode, Boolean bNew):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	m_ProfileImageID = -1;
	pProfileImage = NULL;
	
	m_FormMode = nFormMode;
	m_uid = Get_Url_Params(RR_GuestbookGet, "uid");	
	bReply = FALSE;
	memset(gItemData, 0x0, sizeof(gItemData));
	memset((void*)&BottomTabCtrID, 0x0, sizeof(BottomTabCtrID));
	
	Create(APP_RE_ID_CommonListForm);
	if(bNew == TRUE)
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_DEFAULT, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	else
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_NONE, TG3_WINDOW_MOVIE_MODE_DEFAULT);	
}

// ��������
TGuestbookGetForm::~TGuestbookGetForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());

	if( pProfileImage != NULL)
	{
		pProfileImage->Destroy();
		pProfileImage = NULL;								
	}
	
	for(int i=0; i<GUESTBOOKLIST_MAXITEM; i++)
	{
		if( gItemData[i].pBmp != NULL)
		{
			gItemData[i].pBmp->Destroy();
			gItemData[i].pBmp = NULL;									
		}

		if( gItemData[i].pBmpZoom!= NULL)
		{
			gItemData[i].pBmpZoom->Destroy();
			gItemData[i].pBmpZoom = NULL;								
		}		
	}
}

// �����¼�����
Boolean TGuestbookGetForm::EventHandler(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;

	switch (pEvent->eType)
	{
			//���ڴ���
		case EVENT_WinInit:
			{
				_OnWinInitEvent(pApp, pEvent);
				bHandled = TRUE;
			}
			break;
		
			//���ڹر�
		case EVENT_WinClose:
			{
				_OnWinClose(pApp, pEvent);
			}
			break;
		
			//����ؼ�
		case EVENT_CtrlSelect:
			{
				bHandled = _OnCtrlSelectEvent(pApp, pEvent);
			}
			break;
		
			//�ؼ���ȡ����
		case EVENT_CtrlSetFocus:
			{
				bHandled = _OnCtrlSetFocusEvent(pApp, pEvent);
			}
			break;
			
			//�ؼ���ʧ����
		case EVENT_CtrlKillFocus :
			{
				bHandled = _OnCtrlKillFocusEvent(pApp, pEvent);
			}
			break;
			
			//��������ݱ仯
		case EVENT_FieldChanged:
			{
				_OnFldChangedEvent(pApp, pEvent);
				bHandled = TRUE;
			}
			break;
			
			//���ڵı���
		case EVENT_WinEraseClient:
			{
				TDC dc(this);
				WinEraseClientEventType *pEraseEvent = reinterpret_cast< WinEraseClientEventType* >( pEvent );
				TRectangle rc(pEraseEvent->rc);
				dc.SetBackColor(RGB_COLOR_FORM_BACKGROUND);
				dc.EraseRectangle(&rc, 0);
				dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_bg), 0, 0, SCR_W,GUI_API_STYLE_ALIGNMENT_LEFT);


				if(m_FormMode == FORM_MODE_MYHOME)
					dc.DrawBitmapsH(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Bottom_btn_bg), 0, BOTTOM_TAB_Y, SCR_W,GUI_API_STYLE_ALIGNMENT_LEFT);	
			
				pEraseEvent->result = 1;				
				bHandled = TRUE;
			}
			break;

			// �������¼� 
		case EVENT_KeyCommand: 
			{ 
				if( pEvent->sParam1 == SYS_KEY_SOFTKEY_RIGHT_UP || pEvent->sParam1 == SYS_KEY_SOFTKEY_RIGHT_LONG ) 
				{ 
					// ģ��������Ұ�ťѡ����Ϣ 
					HitControl(m_TitleBtnRight); 
					bHandled = TRUE; 
				} 
			} 
			break;
			
			//��������¼�
		case MSG_DL_THREAD_NOTIFY:
			{
				NotifyMsgDataType notifyData;
				Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));				
				bHandled = TRUE;
				switch(notifyData.nAccessType)
				{
					case RR_PhotoDownload:
					{

						if(notifyData.nParam == FEED_PROFILE_IMAGE_INDEX)
						{	
							//���������¶�Ӧ��ͼƬ
							TBitmap* pDownLoadBitmap = NULL;
							pDownLoadBitmap = LoadImgByPath(notifyData.pszFilePath);

							if(pDownLoadBitmap)
							{
								TMaskButton* pLogo = NULL;
								pLogo = static_cast<TMaskButton*>(GetControlPtr(m_ProfileImageID));
								if(pLogo)
								{
									TRectangle rc;							
									pLogo->GetBounds(&rc);
									if( pProfileImage != NULL)
									{
										pProfileImage->Destroy();
										pProfileImage = NULL; 							
									}
										
									pProfileImage = TBitmap::Create(RR_HEAD_W, RR_HEAD_W, pDownLoadBitmap->GetDepth());
									pProfileImage->QuickZoom(pDownLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));
									pLogo->SetCaption(TUSTR_Re_NULL,0,0);
									pLogo->SetImage(pProfileImage, (rc.Width()-pProfileImage->GetWidth())/2, (rc.Height()-pProfileImage->GetHeight())/2);										pLogo->Draw();
								}				

								//�ͷ�ͼƬ
								pDownLoadBitmap->Destroy();
								pDownLoadBitmap = NULL; 						
							}	
							break;
						}

						
						if( gItemData[notifyData.nParam].pBmp != NULL)
						{
							gItemData[notifyData.nParam].pBmp->Destroy();
							gItemData[notifyData.nParam].pBmp = NULL;									
						}

						if( gItemData[notifyData.nParam].pBmpZoom!= NULL)
						{
							gItemData[notifyData.nParam].pBmpZoom->Destroy();
							gItemData[notifyData.nParam].pBmpZoom = NULL;									
						}
						
						gItemData[notifyData.nParam].pBmp= LoadImgByPath(notifyData.pszFilePath);
						if(gItemData[notifyData.nParam].pBmp != NULL)
						{
							TMaskButton* pMyHeadMBtn = static_cast<TMaskButton*>(GetControlPtr(gItemData[notifyData.nParam].HeadMaskButtonID));
							if(pMyHeadMBtn != NULL)
							{
								gItemData[notifyData.nParam].pBmpZoom = gItemData[notifyData.nParam].pBmp->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
								gItemData[notifyData.nParam].pBmpZoom->QuickSpecialZoom(gItemData[notifyData.nParam].pBmp, 0, 0);

								pMyHeadMBtn->SetImage(gItemData[notifyData.nParam].pBmpZoom, (HEADMASKBUTTON_W - gItemData[notifyData.nParam].pBmpZoom->GetWidth())/2, (HEADMASKBUTTON_H - gItemData[notifyData.nParam].pBmpZoom->GetHeight())/2 );
								pMyHeadMBtn->Draw();
							}
						}
						break;
					}

					case RR_GuestbookPost:
					{
						pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_Success), TResource::LoadConstString(APP_RE_ID_STRING_SendComment), WMB_OK);
						RenRenAPICommon_Download(RR_GuestbookGet, this->GetWindowHwndId(), 0, m_FormMode);
						break;
					}

					case RR_UsersGetInfoPhoto:
						{
							tResponseUsersGetInfo* Response = NULL;
							RenRenAPI_JsonParse(RR_UsersGetInfoPhoto, (void **)&Response);
							if(Response)
							{
								_UpdateProfilePhoto(pApp, Response);
								delete Response;
								Response = NULL;
							}
						}
					break;					
					default:
						break;
				}
			}
			break;
		case MSG_POST_THREAD_NOTIFY:
		{
				PostNotifyMsgDataType notifyData;
				Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(PostNotifyMsgDataType));
			
				//if(notifyData.nEditType == NewBlogEdit)
				{	
					char* uid = Get_Url_Params(RR_GuestbookGet, "uid");
					Set_Url_Params(RR_GuestbookPost, "uid", uid);
					Set_Url_Params(RR_GuestbookPost, "content", (char*)notifyData.pszEditContent);	
					//�����Ƿ�Ϊ���Ļ���1��ʾ�ǣ�0��ʾ��ȱʡֵΪ0
					if(notifyData.bPrivacy == TRUE)
						Set_Url_Params(RR_GuestbookPost, "type", "1");
					else
						Set_Url_Params(RR_GuestbookPost, "type", "0");


					//�ظ����˵�������Ҫ��rid
					if(bReply)
					{	
						char rid[64] = {0};
						sprintf(rid, "%d", gItemData[m_SelectReplyItem].uid);
						Set_Url_Params(RR_GuestbookPost, "rid", rid);
						
					}
					else
					{		
						Set_Url_Params(RR_GuestbookPost, "rid", "");
					}

					//���ظ�������ҳ������ʱ��Ҫ��reply_gossip_id
					//TUChar sID[64]={0};
					//Char id[64]={0};
					//TUString::StrI64ToA(sID,(Int64)gItemData[m_SelectReplyItem].id);
					//TUString::StrUnicodeToStrUtf8(id,sID);
					char id[64] = {0};
					DoubleToChar(gItemData[m_SelectReplyItem].id, id);
					Set_Url_Params(RR_GuestbookPost, "reply_gossip_id", (char*)id);
				
					RenRenAPICommon_Download(RR_GuestbookPost, this->GetWindowHwndId(), 0, 0);		
				}			
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

// ���ڳ�ʼ��
Boolean TGuestbookGetForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{	
	m_TitleBtnLeft = SetAppBitmapButton(this, APP_RE_ID_BITMAP_write_but, APP_RE_ID_BITMAP_write_but_over);
	//m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	//SetAppTilte(this, APP_RE_ID_STRING_Guestbook);

	//��ʾ��ͬ��title
	if(m_FormMode == FORM_MODE_NORMAL)
	{
		SetAppTilte(this, APP_RE_ID_STRING_Guestbook);
		m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	}
	else if(m_FormMode == FORM_MODE_MYHOME)
	{
		tResponseProfile* ResponseProfile;
		RenRenAPI_JsonParse(RR_ProfileGetInfo, (void **)&ResponseProfile);	
		if(ResponseProfile)
		{
			if(ResponseProfile->uid == RenRenUserInfo.uid)
			{
				SetAppTilte(this, APP_RE_ID_STRING_MyNews);
				m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Home,TITLE_BUTTON_RIGHT);
			}
			else
			{
				TUChar  pszFName[64] = {0};
				TUString::StrUtf8ToStrUnicode(pszFName , (const Char *)ResponseProfile->name);			
				SetAppTilte(this, 0, pszFName);
				m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
			}

			delete ResponseProfile;
			ResponseProfile = NULL;	
		}
	}
	
	//����panel�߶�
	TPanel* pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonPanel));	
	TCoolBarList* pCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));

	if( (pTPanel == NULL) || (pCoolBarList == NULL) )
		return FALSE;
	
	//����Panel�߶�
	{
		TRectangle rect;
		Int32 Height_Panel = 0;		//Panel�߶�		
		
		Height_Panel = SCR_H - STATUSBAR_H - TITLEBAR_H;
		if(m_FormMode == FORM_MODE_MYHOME)
			Height_Panel -= BOTTOM_TAB_H;
			
		pTPanel->GetBounds(&rect);
		rect.SetHeight(Height_Panel);
		pTPanel->SetBounds(&rect);
	}

	if(m_FormMode == FORM_MODE_MYHOME)
		CreateProfileBottomTab(this, &BottomTabCtrID, 4);
	
	//:TODO:

	//��ʾ������Ϣ
	if(m_FormMode == FORM_MODE_MYHOME)
	{
		Int32 Height_Top = 0;			//CoolBarList������Ϣ�ĸ߶�
		tResponseProfile* ResponseProfile;
		RenRenAPI_JsonParse(RR_ProfileGetInfo, (void **)&ResponseProfile);
		if(ResponseProfile != NULL)
		{
			//ͷ��
			TBitmap* pDownLoadBitmap = NULL;
			pDownLoadBitmap = LoadImgByUrl(ResponseProfile->headurl);			
			//pProfileImage = LoadImgByUrl(ResponseProfile->headurl);
			const TBitmap * pImageDeault = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_DefaultLarge);
			const TBitmap * pBackImage = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_headmask_large);
			if(pDownLoadBitmap == NULL)
			{
				//RenRenAPICommon_DownloadPhoto(ResponseProfile->headurl, this->GetWindowHwndId(), FEED_PROFILE_IMAGE_INDEX);	
				m_ProfileImageID = CtrlAddItemToPanel_MaskButton(this, pTPanel, RR_HEAD_X, RR_HEAD_Y, RR_HEAD_W, RR_HEAD_H, (TBitmap*)pImageDeault, (TBitmap*)pImageDeault, (TBitmap*)pBackImage );	
				this->DisableControl(m_ProfileImageID);
			}
			else
			{
				//��Ҫ���Ŵ�
				pProfileImage = TBitmap::Create(RR_HEAD_W, RR_HEAD_W, pDownLoadBitmap->GetDepth());
				pProfileImage->QuickZoom(pDownLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));				
				m_ProfileImageID = CtrlAddItemToPanel_MaskButton(this, pTPanel, RR_HEAD_X, RR_HEAD_Y, RR_HEAD_W, RR_HEAD_H, (TBitmap*)pProfileImage, (TBitmap*)pProfileImage, (TBitmap*)pBackImage );	
				this->DisableControl(m_ProfileImageID);
				
				//�ͷ�ͼƬ
				pDownLoadBitmap->Destroy();
				pDownLoadBitmap = NULL; 						
			}
			//���ش�ͷ��
			tResponseUsersGetInfo* ResponseUserInfoPhoto = NULL;
			RenRenAPI_JsonParse(RR_UsersGetInfoPhoto, (void **)&ResponseUserInfoPhoto);
			if(ResponseUserInfoPhoto)
			{
				if( (ResponseUserInfoPhoto->nArraySize > 0) && (ResponseUserInfoPhoto->Array[0].uid == ResponseProfile->uid) )
				{
					_UpdateProfilePhoto(pApp, ResponseUserInfoPhoto);
				}
				else
				{
					Set_Url_Params(RR_UsersGetInfoPhoto, "uids", m_uid);
					RenRenAPICommon_Download(RR_UsersGetInfoPhoto, this->GetWindowHwndId(), 0, 0); 
				}					
							
				delete ResponseUserInfoPhoto;
				ResponseUserInfoPhoto = NULL;							
			}
			else
			{
				Set_Url_Params(RR_UsersGetInfoPhoto, "uids", m_uid);
				RenRenAPICommon_Download(RR_UsersGetInfoPhoto, this->GetWindowHwndId(), 0, 0); 
			}
			
			//����
			TUChar  pszFName[64] = {0};
			TUString::StrUtf8ToStrUnicode(pszFName , (const Char *)ResponseProfile->name);
			CtrlAddItemToPanel_Label(this, pTPanel, RR_NAME_X, RR_NAME_Y, 0, FONT_NORMAL, pszFName, RGB_COLOR_BLACK);
			Height_Top += (RR_NAME_Y+RR_NAME_H);
			//״̬
			TUChar  pszStatus[1024] = {0};
			Coord nHeight_RichView = 0;
			if(ResponseProfile->nSize_status>0)
			{
				TUString::StrUtf8ToStrUnicode(pszStatus, (const Char *)ResponseProfile->status[0].content);
				CtrlAddItemToPanel_RichView(this, pTPanel, RR_STATUS_X, RR_STATUS_Y, RR_STATUS_W, &nHeight_RichView, FONT_MIDDLE, pszStatus, RGB_COLOR_BLACK);
			}

			//λ���ݴ�����
			if(nHeight_RichView > (RR_HEAD_H-RR_NAME_H) )
				Height_Top += nHeight_RichView;
			else
				Height_Top += (RR_HEAD_H- RR_NAME_H);

			//�������
			Height_Top += 4;
			//����CoolBarList�߶�
			if(pCoolBarList)
			{
				TRectangle rect;
				Int32 nY = Height_Top;
					
				pCoolBarList->GetBounds(&rect);
				rect.SetY(nY);
				pCoolBarList->SetBounds(&rect);
			}
		
			delete ResponseProfile;
			ResponseProfile = NULL;
		}
	}

	//�Ӷ�ӦJson�л�ȡ����,���ڸ���UI
	int nErrorCode = eFailed;
	tResponseGuestbookGet* Response = NULL;
	nErrorCode = RenRenAPI_JsonParse(RR_GuestbookGet, (void **)&Response);	

	//��̬�����б�����
	TBarRowList* pRowList = NULL;
	TBarRow* pBarRow = NULL;
	
	pRowList = pCoolBarList->Rows();
	pRowList->Clear();
	pBarRow = pRowList->AppendRow();
	if(Response != NULL)
	{
		TBarListItem* pListItem = NULL;
		//��������Ϊ��ɫ��ɫ
		pCoolBarList->SetColor(CTL_COLOR_TYPE_FOCUS_BACK, RGB_COLOR_WHITE);
		pRowList->BeginUpdate();

		if(Response->nArraySize == 0)
		{
			TUChar pszCount[64] = {0};
			TUString::StrPrintF(pszCount, TResource::LoadConstString(APP_RE_ID_STRING_GuestbookCount), Response->nArraySize);
					
			TBarListItem* lpItem = NULL;
			lpItem = pBarRow->AppendItem();
			lpItem->SetCaption(pszCount);
			lpItem->SetEnabled(FALSE);
		}
		//����Ϊ�����б���
		for (int i = 0; i < Response->nArraySize; i++)
		{
			TUChar pszString[4096] = {0};
			Coord nHeigh = 0;
		
			pListItem = pBarRow->AppendItem();
			if (pListItem)
			{
				//��������
				pListItem->SetTitle(NULL);
				pListItem->SetCaption(NULL);       
				pListItem->SetIndicatorType(itNone);

				//�ȶ�ȡ����cache, ���ļ�������
				 gItemData[i].pBmp = LoadImgByUrl(Response->Array[i].headurl);
				if(  gItemData[i].pBmp != NULL)
				{
					//ͷ��,��Ҫ������
					gItemData[i].pBmpZoom = gItemData[i].pBmp->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
					gItemData[i].pBmpZoom->QuickSpecialZoom(gItemData[i].pBmp, 0, 0);
					gItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, pListItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, gItemData[i].pBmpZoom, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));					
				}
				else
				{
					gItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, pListItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_Default), (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));
					RenRenAPICommon_DownloadPhoto(Response->Array[i].headurl, this->GetWindowHwndId(), i );
				}
				
				//����
				TUString::StrUtf8ToStrUnicode(pszString , (const Char *)Response->Array[i].name);
				CtrlAddItemToCoolBarList_Lable(this, pListItem, NAME_X, NAME_Y, 0, NAME_FONT, pszString, RGB_COLOR_LIGHTBLUE);

				//���Ļ�
				if(Response->Array[i].is_whisper != 0)
				{
					gItemData[i].bWhisper = TRUE;
					CtrlAddItemToCoolBarList_Image(this,  pListItem, WHISPER_X, WHISPER_Y , 49, 15, (TBitmap*)TResource::LoadConstBitmap(APP_RE_ID_BITMAP_whisper));
				}
				//����,ȥ��html��ʽ������
				StrRemoveAtoB(Response->Array[i].content, strlen(Response->Array[i].content), "<", ">");
				TUString::StrUtf8ToStrUnicode(pszString , (const Char *)Response->Array[i].content);
				CtrlAddItemToCoolBarList_RichView(this, pListItem, CONTENT_X, CONTENT_Y, CONTENT_W, &nHeigh, CONTENT_FONT, pszString);
				//ʱ��
				TUString::StrUtf8ToStrUnicode(pszString , (const Char *)Response->Array[i].time);
				CtrlAddItemToCoolBarList_Lable(this, pListItem, TIME_X, CONTENT_Y+nHeigh, 0, TIME_FONT, pszString, RGB_COLOR_GRAY);	

				//�ظ�
				if(Response->Array[i].uid != RenRenUserInfo.uid)
				{
					TUString::StrCopy(pszString,TResource::LoadConstString(APP_RE_ID_STRING_Reply));
					gItemData[i].TButtonReplyID = CtrlAddItemToCoolBarList_Button(this,  pListItem, BUTTON_REPLY_X, BUTTON_REPLY_Y, BUTTON_REPLY_W, BUTTON_REPLY_H, pszString);
				}
				//����CoolBarList Item�ĸ߶�
				pListItem->SetHeight(60 + nHeigh);
				//����id
				gItemData[i].id = Response->Array[i].id;
				gItemData[i].uid = Response->Array[i].uid;
			}
		}
		
		pRowList->EndUpdate();

		delete Response;
		Response = NULL;
	}
	else
	{
		//��ʾ��Ȩ�޲��������(�û��������Ķ�Ȩ��)
		if(nErrorCode == 200)
		{
			TUChar ErrorInfo[ERROR_INFO_LEN] = {0};
			int iResult = RenRenAPI_JsonParse_bSuccess(RR_GuestbookGet, ErrorInfo);
			if(iResult != 0)	
			{
				if(TUString::StrLen(ErrorInfo) > 0)
				{
					//��ʾ������ʾ
					TBarListItem* lpItem = NULL;
					lpItem = pBarRow->AppendItem();
					if (lpItem)  
					{
						CtrlAddItemToCoolBarList_Lable(this, lpItem, RR_INFO_X,  RR_INFO_Y, RR_INFO_W, FONT_NORMAL, (TUChar*)ErrorInfo);
						lpItem->SetHeight(RR_INFO_H);
						lpItem->SetEnabled(FALSE);
					}
				}
			}
		}
	}
	return TRUE;
}

// �رմ���ʱ������������Ϣ
Boolean TGuestbookGetForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// �ؼ�����¼�����
Boolean TGuestbookGetForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled;
	bHandled = FALSE;
	Int32 nCtrlID = pEvent->sParam1;
	SS_GTID			gtidMain;
	SS_GetCurrentGTID(&gtidMain);
	
	if(m_TitleBtnRight == nCtrlID)
	{
		if(m_FormMode == FORM_MODE_NORMAL)
			this->CloseWindow();
		else			
			App_PostMessageEx(&gtidMain, MSG_BACKTO_HOME, NULL, 0, 0, 0);		
		bHandled = TRUE; 
		return bHandled;
	}
	else if(m_TitleBtnLeft == nCtrlID)
	{
		PostNotifyMsgDataType in_msgData = {0};
		MemSet(&in_msgData, 0, sizeof(PostNotifyMsgDataType));

		in_msgData.nCtrlId = 0;
		in_msgData.nHwndId = this->GetWindowHwndId();
		//in_msgData.nEditType = NewBlogEdit;
		in_msgData.nAppTitleStrID = APP_RE_ID_STRING_SendComment; 
		in_msgData.nPublicBtnCaptionStrID = APP_RE_ID_STRING_Publish;
		
		in_msgData.nTitleEditMaxChars = RENREN_LENGTH_BLOG_TITLE;
		in_msgData.nContentEditMaxChars = RENREN_LENGTH_BLOG_COMMENT;
		
		in_msgData.IsExpressionBtn = FALSE;
		in_msgData.IsPhotoBtn = FALSE;
		in_msgData.IsLocationBtn = FALSE;
		in_msgData.IsPrivacyCtl = TRUE;
		in_msgData.IsTitleEdit = FALSE;

		m_SelectReplyItem = -1;
		bReply = FALSE;
				
		TAppEditForm *pWin = new TAppEditForm( pApp, in_msgData);	
		bHandled = TRUE; 		
		return bHandled;
	}
	//��ȡ�û���Ϣ��������
	else if(BottomTabCtrID.nCtrID[0] == nCtrlID)
	{
		Set_Url_Params(RR_ProfileGetInfo, "uid", m_uid);
		RenRenAPICommon_Download(RR_ProfileGetInfo, this->GetWindowHwndId(), 0, 0); 				
		bHandled = TRUE;		
		return bHandled;
	}
	//���
	else if(BottomTabCtrID.nCtrID[1] == nCtrlID)
	{
		Set_Url_Params(RR_PhotosGetAlbums, "uid", m_uid);
		RenRenAPICommon_Download(RR_PhotosGetAlbums, this->GetWindowHwndId(), 0, FORM_MODE_MYHOME); 				
		bHandled = TRUE;		
		return bHandled;
	}
	//�ռ�
	else if(BottomTabCtrID.nCtrID[2] == nCtrlID)
	{
		Set_Url_Params(RR_BlogGets, "uid", m_uid);
		RenRenAPICommon_Download(RR_BlogGets, this->GetWindowHwndId(), 0, FORM_MODE_MYHOME);			
		bHandled = TRUE;
		return bHandled;
	}
	//���Ա�
	else if(BottomTabCtrID.nCtrID[3] == nCtrlID)
	{
		//Set_Url_Params(RR_GuestbookGet, "uid", m_uid);
		//RenRenAPICommon_Download(RR_GuestbookGet, this->GetWindowHwndId(), 0, FORM_MODE_MYHOME); 			
		bHandled = TRUE;		
		return bHandled;
	}
	//����
	else if(BottomTabCtrID.nCtrID[4] == nCtrlID)
	{
		if(bIsPage == TRUE)
			{
			Set_Url_Params(RR_PagesGetInfo, "page_id", m_uid);
			RenRenAPICommon_Download(RR_PagesGetInfo, this->GetWindowHwndId(), 0, 0); 				
			}
		else
			{
			Set_Url_Params(RR_UsersGetInfo, "uids", m_uid);
			RenRenAPICommon_Download(RR_UsersGetInfo, this->GetWindowHwndId(), 0, 0); 				
			}
		bHandled = TRUE;		
		return bHandled;
	}
		
	//���Իظ�����
	for(int i=0;i<ARR_SIZE(gItemData); i++)
		{
			if(gItemData[i].TButtonReplyID == nCtrlID)
			{
				PostNotifyMsgDataType in_msgData = {0};
				MemSet(&in_msgData, 0, sizeof(PostNotifyMsgDataType));
	
				in_msgData.nHwndId = this->GetWindowHwndId();
				in_msgData.nCtrlId = 0;
				in_msgData.nParam = 2;
				in_msgData.nAppTitleStrID = APP_RE_ID_STRING_ReplyGuestBook; 
				in_msgData.nPublicBtnCaptionStrID = APP_RE_ID_STRING_Publish;
				
				in_msgData.nTitleEditMaxChars = RENREN_LENGTH_BLOG_TITLE;
				in_msgData.nContentEditMaxChars = RENREN_LENGTH_BLOG_COMMENT;
				
				in_msgData.IsExpressionBtn = FALSE;
				in_msgData.IsPhotoBtn = FALSE;
				in_msgData.IsLocationBtn = FALSE;
				in_msgData.IsPrivacyCtl = TRUE;
				in_msgData.IsTitleEdit = FALSE;
				in_msgData.bPrivacy = gItemData[i].bWhisper;	//�ظ����Ļ�,Ĭ��ҲҪ�����Ļ�
				
				m_SelectReplyItem = i;
				bReply = TRUE;
				
				TAppEditForm *pWin = new TAppEditForm( pApp, in_msgData);	
				bHandled = TRUE;		
				return bHandled;
				
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

//�ؼ���ȡ����
Boolean TGuestbookGetForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//�ؼ���ʧ����
Boolean TGuestbookGetForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// �ı������봦���¼�
Boolean TGuestbookGetForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

/**
 * ����ͷ��
 *
 * \param pApp
 * \param ResponseInfoPhoto
 *
 * \return 
 */
Boolean TGuestbookGetForm::_UpdateProfilePhoto(TApplication* pApp, tResponseUsersGetInfo* ResponseInfoPhoto)
{

	if(ResponseInfoPhoto == NULL)
		return FALSE;
	
	if(ResponseInfoPhoto->nArraySize > 0)
	{
		TBitmap* pDownLoadBitmap = NULL;
		pDownLoadBitmap = LoadImgByUrl(ResponseInfoPhoto->Array[0].headurl);						
		if(pDownLoadBitmap == NULL)
		{
			RenRenAPICommon_DownloadPhoto(ResponseInfoPhoto->Array[0].headurl, this->GetWindowHwndId(), FEED_PROFILE_IMAGE_INDEX);	
		}
		else
		{
			TMaskButton* pLogo = NULL;
			pLogo = static_cast<TMaskButton*>(GetControlPtr(m_ProfileImageID));
			if(pLogo)
			{
				TRectangle rc;
				TBitmap * pProfileImageTmp = NULL;	//Profile��ͷ��

				pLogo->GetBounds(&rc);
				
				pProfileImageTmp = TBitmap::Create(RR_HEAD_W, RR_HEAD_W, pDownLoadBitmap->GetDepth());
				pProfileImageTmp->QuickZoom(pDownLoadBitmap, TRUE, TRUE,RGBA(0,0,0,255));
				pLogo->SetCaption(TUSTR_Re_NULL,0,0);
				pLogo->SetImage(pProfileImageTmp, (rc.Width()-pProfileImageTmp->GetWidth())/2, (rc.Height()-pProfileImageTmp->GetHeight())/2);
				pLogo->Draw();

				//�����Desroy,������,�����,ԭ����,��ʱ��ô����
				if( pProfileImage != NULL)
				{
					pProfileImage->Destroy();
					pProfileImage = NULL; 							
				}
				pProfileImage = pProfileImageTmp;
				
			}

			//�ͷ�ͼƬ
			pDownLoadBitmap->Destroy();
			pDownLoadBitmap = NULL; 	
		}
	}

	return TRUE;
}
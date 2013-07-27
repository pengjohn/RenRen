/*!
*@FileName: RenRenForm_RequestsGetFriendRequests.cpp
*@Author: zbingwen@GoZone
*@Date: 2010-12-29
*@Brief: ��������Form
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
#include "RenRenForm_RequestsGetFriendRequests.h"

//�û�Image Index

#define HEADMASK_X		(3)
#define HEADMASK_Y		(10)

#define NAME_FONT		(FONT_NORMAL)
#define NAME_X			(HEADMASKBUTTON_W + 10)
#define NAME_Y			(5)
#define NAME_H			(NAME_FONT + LINE_SPACE)

#define INFO_FONT		(FONT_MIDDLE)
#define INFO_X			(NAME_X)
#define INFO_Y			(NAME_Y + NAME_H)
#define INFO_H			(INFO_FONT+LINE_SPACE)

#if(LCD_SIZE == LCD_HVGA )
#define BUTTON_W		(80)
#elif(LCD_SIZE == LCD_WVGA )
#define BUTTON_W		(120)
#endif
#define BUTTON_H		(TITLE_BUTTON_H)
#define BUTTON_Y		(INFO_Y + INFO_H)

#define BUTTON_X1		(SCR_W - BUTTON_W - 20 - BUTTON_W - 20)
#define BUTTON_X2		(SCR_W - BUTTON_W - 20)


#define COOLBARLIST_HEIGHT		(BUTTON_Y + BUTTON_H + HEADMASK_Y)


#define FRIENDREQUEST_MORE_TAGVALUE (0xabcd)
// ���캯��
TRequestsGetFriendRequestsForm::TRequestsGetFriendRequestsForm(TApplication* pApp):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	memset(m_ItemData, 0x0, sizeof(m_ItemData) );
	Response = NULL;
	ItemCount = 0;
	Create(APP_RE_ID_CommonListForm);
}

// ��������
TRequestsGetFriendRequestsForm::~TRequestsGetFriendRequestsForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());	

	for(int i = 0;i<FRIENDREQUEST_MAXITEM;i++)
	{
		if( m_ItemData[i].pUserImageBmp != NULL)
		{
			m_ItemData[i].pUserImageBmp->Destroy();
			m_ItemData[i].pUserImageBmp = NULL;									
		}	
	}
	
	if( Response )
	{
		delete Response;	
	}
}

// �����¼�����
Boolean TRequestsGetFriendRequestsForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
				bHandled = TRUE;
				NotifyMsgDataType notifyData;
				Sys_GetMessageBody((MESSAGE_t *)pEvent, &notifyData, sizeof(NotifyMsgDataType));	
				switch(notifyData.nAccessType)
				{
					case RR_PhotoDownload:
					{
						if( m_ItemData[notifyData.nParam].pUserImageBmp != NULL)
						{
							m_ItemData[notifyData.nParam].pUserImageBmp->Destroy();
							m_ItemData[notifyData.nParam].pUserImageBmp = NULL;									
						}
						TBitmap* pDownLoadBitmap = NULL;							
						pDownLoadBitmap = LoadImgByPath(notifyData.pszFilePath);
						if(pDownLoadBitmap != NULL)
						{
							TMaskButton* pMyHeadMBtn = static_cast<TMaskButton*>(GetControlPtr(m_ItemData[notifyData.nParam].HeadMaskButtonID));
							if(pMyHeadMBtn != NULL)
							{
								m_ItemData[notifyData.nParam].pUserImageBmp = pDownLoadBitmap->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
								m_ItemData[notifyData.nParam].pUserImageBmp->QuickSpecialZoom(pDownLoadBitmap, 0, 0);							
							
								pMyHeadMBtn->SetImage(m_ItemData[notifyData.nParam].pUserImageBmp, (HEADMASKBUTTON_W - m_ItemData[notifyData.nParam].pUserImageBmp->GetWidth())/2, (HEADMASKBUTTON_H - m_ItemData[notifyData.nParam].pUserImageBmp->GetHeight())/2 );
								pMyHeadMBtn->Draw();
							}
							//�ͷ�ͼƬ
							pDownLoadBitmap->Destroy();
							pDownLoadBitmap = NULL; 								
						}
						break;
					}

					case RR_RequestsDealFriendRequest:
						{
							RenRenAPICommon_Download(RR_RequestsGetFriendRequests,this->GetWindowHwndId(),0,0);
							//pApp->MessageBox( TResource::LoadConstString(APP_RE_ID_STRING_Error), TResource::LoadConstString(APP_RE_ID_STRING_Error), WMB_OK);
							bHandled = TRUE;
							break;
						}
				}
			}
			break;
		case EVENT_ListPreDraw:
			{
				bHandled = TRUE;//OnLstPreDrawEvent(pApp,pEvent);
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

// ���ڳ�ʼ��
Boolean TRequestsGetFriendRequestsForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;
	int count = 0;
	
	//m_TitleBtnLeft = SetAppTitleButton(this, APP_RE_ID_STRING_OK,TITLE_BUTTON_LEFT);
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	SetAppTilte(this, APP_RE_ID_STRING_FriendRequestList);


	TBarRowList *lpRowList = NULL;
	TRectangle Rc_CoolBarList;
	TBarRow *lpRow = NULL;
	
	TCoolBarList* pCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));//ShowCoolList(ShowType)
	if (pCoolBarList)
	{
		pCoolBarList->GetBounds(&Rc_CoolBarList);
		lpRowList = pCoolBarList->Rows();
		//add row
		if (lpRowList)
		{
			lpRowList->BeginUpdate();
			lpRowList->Clear();
			lpRow = lpRowList->AppendRow();
			lpRowList->EndUpdate(); 
		}
	}
	
	//�Ӷ�ӦJson�л�ȡ����,���ڸ���UI
	iRet = RenRenAPI_JsonParse(RR_RequestsGetFriendRequests, (void **)&Response);

	if(Response != NULL)
	{
		_UpdateList(pApp, Response);
#if 0	
		//��ȡCoolBarList �ؼ�ָ�� 
		TCoolBarList* pCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));
		if (pCoolBarList)
		{
			
			TBarRowList*	lpRowList = NULL;
			TBarRow*		lpRow = NULL;
			TBarListItem* lpItem = NULL;

			lpRowList = pCoolBarList->Rows();
			//add row
			if (lpRowList)
			{
				lpRowList->BeginUpdate();

				lpRowList->Clear();
				lpRow = lpRowList->AppendRow();

				//����ͳ��
				if(Response->nSize_guest == 0)
				{
					TBarListItem* lpItemCount = NULL;
					TUChar pszCount[64] = {0};
					lpItemCount = lpRow->AppendItem();
					if (lpItemCount)
					{
						Int32 StringWidth = 0;
						TUString::StrCopy(pszCount, TResource::LoadConstString(APP_RE_ID_STRING_NothingFriend) );
						StringWidth = GetShowAllStringWidth(pszCount, FONT_MIDDLE) + 8;	//��Ԥ��һЩ
						CtrlAddItemToCoolBarList_Lable(this, lpItemCount, (COOLBARLIST_WIDTH-StringWidth)/2, INFO_Y, 0, FONT_MIDDLE, pszCount);
						lpItemCount->SetHeight(INFO_Y*2+FONT_MIDDLE);
						lpItemCount->SetEnabled(FALSE);
					}
				}
				
				for(int i = 0;i < Response->nSize_guest; ++i)
				{
					//�������������ֱ�ӷ���
					if(i >= FRIENDREQUEST_MAXITEM)
						break;

					TUChar pszString[128] = {0};
					
					if(lpRow)
					{
						lpItem = lpRow->AppendItem();
						if (lpItem)
						{
						//��������
						lpItem->SetCaption(NULL);
						lpItem->SetTitle(NULL);
						lpItem->SetIndicatorType(itNone);

						//ͷ��,�ȶ�ȡ����cache, ���ļ�������
						TBitmap* pDownLoadBitmap = NULL;
						pDownLoadBitmap = LoadImgByUrl(Response->guest[i].headurl);
						if( pDownLoadBitmap != NULL)
						{
							m_ItemData[i].pUserImageBmp = pDownLoadBitmap->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
							m_ItemData[i].pUserImageBmp->QuickSpecialZoom(pDownLoadBitmap, 0, 0);						
							m_ItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, lpItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, m_ItemData[i].pUserImageBmp, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));					
							//�ͷ�ͼƬ
							pDownLoadBitmap->Destroy();
							pDownLoadBitmap = NULL; 	
						}
						else
						{
							m_ItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, lpItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_Default), (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));
							RenRenAPICommon_DownloadPhoto(Response->guest[i].headurl, this->GetWindowHwndId(), i );
						}

						//����
						TUString::StrUtf8ToStrUnicode(pszString, (const Char *)Response->guest[i].name);
						CtrlAddItemToCoolBarList_Lable(this , lpItem ,  NAME_X, NAME_Y, 0, NAME_FONT,pszString);

						//network
						TUString::StrUtf8ToStrUnicode(pszString, (const Char *)Response->guest[i].network);
						CtrlAddItemToCoolBarList_Lable(this , lpItem ,  INFO_X, INFO_Y, 0, INFO_FONT,pszString, RGB_COLOR_GRAY);

						//���ܰ�ť
						m_CtrlID.m_ButtonYes[i] = CtrlAddItemToCoolBarList_Button(this, lpItem, BUTTON_X1, BUTTON_Y, BUTTON_W, BUTTON_H, (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_Accept));		
						//���԰�ť
						m_CtrlID.m_ButtonNo[i] = CtrlAddItemToCoolBarList_Button(this, lpItem, BUTTON_X2, BUTTON_Y, BUTTON_W, BUTTON_H, (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_Ignore));

						lpItem->SetHeight(COOLBARLIST_HEIGHT);
						
						++ItemCount;
						m_ItemData[i].id = Response->guest[i].uid;
						}
					}
				}
				lpRowList->EndUpdate();

			}
		}
#endif		
	}
	
	return TRUE;
}


// �رմ���ʱ������������Ϣ
Boolean TRequestsGetFriendRequestsForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// �ؼ�����¼�����
Boolean TRequestsGetFriendRequestsForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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
		bHandled = TRUE; 		
		return bHandled;
	}
	else if( nCtrlID == APP_RE_ID_CommonListForm_CommonCoolBarList)
	{
		TBarRow *pRow = reinterpret_cast< TBarRow* > ( pEvent->sParam2 ); 
		if( pRow->GetIndex() == 0 )	//First Row
		{
			char szFuid[32] = {0};		

			TBarListItem *pItem = reinterpret_cast < TBarListItem* >( pEvent->lParam3 ); 
			Int32 ItemIndex = 0;
			ItemIndex = pItem->GetTagValue();
			if(ItemIndex == FRIENDREQUEST_MORE_TAGVALUE)
			{
				char* szPage = Get_Url_Params(RR_RequestsGetFriendRequests, "page");
				char szPageParams[16] = {0};
				int nPage = 0;
				nPage = atoi(szPage);
				nPage ++;
				sprintf(szPageParams, "%d", nPage);
				Set_Url_Params(RR_RequestsGetFriendRequests, "page", szPageParams);
				RenRenAPICommon_Download(RR_RequestsGetFriendRequests, this->GetWindowHwndId(), 0, 0);			
			}
			else
			{
				sprintf(szFuid, "%d", m_ItemData[ItemIndex].id);
				Set_Url_Params(RR_ProfileGetInfo, "uid", szFuid);
				RenRenAPICommon_Download(RR_ProfileGetInfo, this->GetWindowHwndId(), 0, 0); 				
			}
			bHandled = TRUE;
		}						
	}	
	if(_OnHandleButtonEvent(nCtrlID))//�������ܣ����԰�ť�¼�
	{
		char cbuf[50] = {0};
		TUChar TUCbuf[50] = {0};
		TUString::StrIToA(TUCbuf,uid);
		TUString::StrUnicodeToStrUtf8((Char *)cbuf,(const TUChar *) TUCbuf);
		Set_Url_Params(RR_RequestsDealFriendRequest, "uid", cbuf);
		
		TUString::StrIToA(TUCbuf,type);
		TUString::StrUnicodeToStrUtf8((Char *)cbuf,(const TUChar *) TUCbuf);
		Set_Url_Params(RR_RequestsDealFriendRequest, "type", cbuf);

		RenRenAPICommon_Download(RR_RequestsDealFriendRequest, this->GetWindowHwndId(), 0, 0); 
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
Boolean TRequestsGetFriendRequestsForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//�ؼ���ʧ����
Boolean TRequestsGetFriendRequestsForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// �ı������봦���¼�
Boolean TRequestsGetFriendRequestsForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

/**
 * ������ť�¼�����
 *
 * \param nCtrlID
 *
 * \return 
 */
Boolean TRequestsGetFriendRequestsForm::_OnHandleButtonEvent(Int32 nCtrlID)
{
	Boolean bHandle = FALSE;
	for(int i = 0;i < ItemCount;++i)
	{
		if(m_CtrlID.m_ButtonYes[i] == nCtrlID)
		{
			uid = m_ItemData[i].id;
			type = 0;
			bHandle = TRUE;
			break;
		}else if(m_CtrlID.m_ButtonNo[i] == nCtrlID)
		{
			uid = m_ItemData[i].id;
			type = 1;
			bHandle = TRUE;
			break;
		}
	}
	return bHandle;
	
}


/**
 * �����б���ʾ
 *
 * \param pApp
 * \param Response
 *
 * \return 
 */
Boolean TRequestsGetFriendRequestsForm::_UpdateList(TApplication * pApp, tResponseRequestsGetFriendRequests* Response)
{

	if( Response == NULL)
	{
		return FALSE;
	}

	//:TODO:

	TBarRowList *lpRowList = NULL;
	TBarRow *lpRow = NULL;
	TBarListItem* lpItem = NULL;
		
	TCoolBarList* pCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));//ShowCoolList(ShowType)
	if(pCoolBarList == NULL)
		{
		return FALSE;
		}
	//��������Ϊ��ɫ��ɫ
	pCoolBarList->SetColor(CTL_COLOR_TYPE_FOCUS_BACK, RGB_COLOR_WHITE);
	
	lpRowList = pCoolBarList->Rows();
	if (lpRowList == NULL)
		{
		return FALSE;
		}
	
	lpRowList->BeginUpdate();
	lpRow = lpRowList->FirstRow();
	if(lpRow == NULL)
	{
	return FALSE;
	}

	//����б���Ϊ��,��˵�����¸���,��ɾ����һ�����ఴť
	{
		TBarListItem* lpItem = NULL;
		lpItem = lpRow->LastItem();
		if(lpItem)
		{
			lpItem->Delete(FALSE);
		}
	}

				
	//����Ϊ�����б���
	for (int i = 0; i < Response->nSize_guest;i++)
	{
		//�������������ֱ�ӷ���
		if(i >= FRIENDREQUEST_MAXITEM)
			break;

		TUChar pszString[128] = {0};
		
		if(lpRow)
		{
			lpItem = lpRow->AppendItem();
			if (lpItem)
			{
			//��������
			lpItem->SetCaption(NULL);
			lpItem->SetTitle(NULL);
			lpItem->SetIndicatorType(itNone);

			//ͷ��,�ȶ�ȡ����cache, ���ļ�������
			TBitmap* pDownLoadBitmap = NULL;
			pDownLoadBitmap = LoadImgByUrl(Response->guest[i].headurl);
			if( pDownLoadBitmap != NULL)
			{
				m_ItemData[i].pUserImageBmp = pDownLoadBitmap->Create(HEADPHOTO_W, HEADPHOTO_H, 32);
				m_ItemData[i].pUserImageBmp->QuickSpecialZoom(pDownLoadBitmap, 0, 0);						
				m_ItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, lpItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, m_ItemData[i].pUserImageBmp, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));					
				//�ͷ�ͼƬ
				pDownLoadBitmap->Destroy();
				pDownLoadBitmap = NULL; 	
			}
			else
			{
				m_ItemData[i].HeadMaskButtonID = CtrlAddItemToCoolBarList_MaskButton(this, lpItem, HEADMASK_X, HEADMASK_Y, HEADMASKBUTTON_W, HEADMASKBUTTON_H, (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_Default), (TBitmap*)TBitmap::LoadResBitmap(APP_RE_ID_BITMAP_head1));
				RenRenAPICommon_DownloadPhoto(Response->guest[i].headurl, this->GetWindowHwndId(), i );
			}

			//����
			TUString::StrUtf8ToStrUnicode(pszString, (const Char *)Response->guest[i].name);
			CtrlAddItemToCoolBarList_Lable(this , lpItem ,  NAME_X, NAME_Y, 0, NAME_FONT,pszString);

			//network
			TUString::StrUtf8ToStrUnicode(pszString, (const Char *)Response->guest[i].network);
			CtrlAddItemToCoolBarList_Lable(this , lpItem ,  INFO_X, INFO_Y, 0, INFO_FONT,pszString, RGB_COLOR_GRAY);

			//���ܰ�ť
			m_CtrlID.m_ButtonYes[i] = CtrlAddItemToCoolBarList_Button(this, lpItem, BUTTON_X1, BUTTON_Y, BUTTON_W, BUTTON_H, (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_Accept));		
			//���԰�ť
			m_CtrlID.m_ButtonNo[i] = CtrlAddItemToCoolBarList_Button(this, lpItem, BUTTON_X2, BUTTON_Y, BUTTON_W, BUTTON_H, (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_Ignore));

			lpItem->SetHeight(COOLBARLIST_HEIGHT);
			
			++ItemCount;
			m_ItemData[i].id = Response->guest[i].uid;
			lpItem->SetTagValue(i);
			}
		}
	}


	//��ʾ�޲��ҽ��
	if(Response->nSize_guest == 0)
	{
		lpItem = lpRow->AppendItem();
		lpItem->SetCaption(TResource::LoadConstString(APP_RE_ID_STRING_NothingFriend));
		lpItem->SetEnabled(FALSE);
	}
	//��ʾMore
	else
	{
		char* szPageCount = Get_Url_Params(RR_RequestsGetFriendRequests, "count");
		int nPageCount = 0;
		nPageCount = atoi(szPageCount);
		
		 if(Response->nSize_guest >= nPageCount)
		 {
			lpItem = lpRow->AppendItem();
			lpItem->SetCaption(TResource::LoadConstString(APP_RE_ID_STRING_NextPage));
			lpItem->SetTagValue(FRIENDREQUEST_MORE_TAGVALUE);
		 }
	}
	
	lpRowList->EndUpdate();
	
	if (pCoolBarList)
	{
		pCoolBarList->Update();
	}
	this->UpdateWindow(0);

	return TRUE;
}
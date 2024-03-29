/*!
*@FileName: RenRenForm_UsersGetEvent.cpp
*@Author: zbingwen@GoZone
*@Date: 2010-12-24
*@Brief: 站内信Form
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
#include "RenRenForm_UsersGetEvent.h"


// 构造函数
TUsersGetEventForm::TUsersGetEventForm(TApplication* pApp, Boolean bNew):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	Create(APP_RE_ID_CommonListForm);
	if(bNew == TRUE)
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_DEFAULT, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	else
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_NONE, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	
}

// 析构函数
TUsersGetEventForm::~TUsersGetEventForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());
}

// 窗口事件处理
Boolean TUsersGetEventForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
				bHandled = TRUE;
			}
			break;

		case MSG_FORM_UPDATE:
			{
				bHandled = TRUE;
				UpdateEvent(pApp);
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

// 窗口初始化
Boolean TUsersGetEventForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;
	
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	SetAppTilte(this, APP_RE_ID_STRING_MessageCenter);	

	//:TODO:
	UpdateEvent(pApp);
	
	return TRUE;
}

// 关闭窗口时，保存设置信息
Boolean TUsersGetEventForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// 控件点击事件处理
Boolean TUsersGetEventForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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

	switch(nCtrlID)
	{
		case APP_RE_ID_CommonListForm_CommonCoolBarList:
			{
				TBarRow* pTBarRow = reinterpret_cast<TBarRow*>(pEvent->sParam2);
				switch(pTBarRow->GetIndex())
				{
				case 0 :  //第一ROW
					{
						TBarListItem* pTBarListItem = reinterpret_cast<TBarListItem*>(pEvent->lParam3);
						switch(pTBarListItem->GetIndex())
						{
						case 0 : //好友请求
							{
								Set_Url_Params(RR_RequestsGetFriendRequests, "page", "1");
								RenRenAPICommon_Download(RR_RequestsGetFriendRequests,this->GetWindowHwndId(),0,0);
								//TRequestsGetFriendRequestsForm *pWin = new TRequestsGetFriendRequestsForm(pApp);
								bHandled = TRUE;
							}
							break;
							
						case 1 : //站内信
							{
								Set_Url_Params(RR_MessageGets, "box", RENREN_PARAM_INBOX);
								RenRenAPICommon_Download(RR_MessageGets, this->GetWindowHwndId(), 0, 0); 
								bHandled = TRUE;
							}
							break;
						case 2 : //打招呼
							{

								bHandled = TRUE;
							}
							break;
						case 3 : //回复提示数
							{
								RenRenAPICommon_Download(RR_FeedGetFangle, this->GetWindowHwndId(), 0, 0); 
								bHandled = TRUE;
							}
							break;
						}
						bHandled = TRUE;
					}
					break;
				}
				bHandled = TRUE; 	
			}
			break;
		default:
			break;
	}

	return bHandled;
}

//控件获取焦点
Boolean TUsersGetEventForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//控件丢失焦点
Boolean TUsersGetEventForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// 文本框输入处理事件
Boolean TUsersGetEventForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

/**
 * 更新列表显示
 *
 * \param pApp
 *
 * \return 
 */
Boolean TUsersGetEventForm::UpdateEvent(TApplication * pApp)
{
	int iRet = eFailed;

	TCoolBarList *pTCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));
	if(pTCoolBarList == FALSE)
	{
		return FALSE;
	}
	
	TBarRowList *pTBarRowList = pTCoolBarList->Rows();
	if(pTBarRowList)
	{
		pTBarRowList->BeginUpdate();
		pTBarRowList->Clear();
		TBarRow *pTBarRow = NULL;
		pTBarRow = pTBarRowList->AppendRow();
		if(pTBarRow)
		{
			TUChar pItemsTitle[128] = {0};
			TUChar buf[20] = {0};
			pTBarRow->SetCaption(NULL);
			TBarListItem *pTBarListItem = NULL;
			//好友请求
			pTBarListItem = pTBarRow->InsertItem(pTBarListItem);
			if(pTBarListItem)
			{
				TUString::StrCopy(pItemsTitle,TResource::LoadConstString(APP_RE_ID_STRING_Friend_Request_Count));
				TUString::StrPrintF(buf, TResource::LoadConstString(APP_RE_ID_STRING_NewCount), RenRenUserInfo.nCountFriendRequest);					
				pTBarListItem->SetTitle(pItemsTitle);
				pTBarListItem->SetCaptionAlign(tasRightJustify);
				pTBarListItem->SetIndicatorType(itDefault);
				pTBarListItem->SetValue(1);
				if(RenRenUserInfo.nCountFriendRequest> 0)
					{ 
					pTBarListItem->SetCaption(buf);					
					pTBarListItem->SetCaptionFontColor(COLOR_RED);
					}
			
			}
			//站内信
			pTBarListItem = pTBarRow->InsertItem(pTBarListItem);
			if(pTBarListItem)
			{
				TUString::StrCopy(pItemsTitle,TResource::LoadConstString(APP_RE_ID_STRING_New_Message_Count));
				TUString::StrPrintF(buf, TResource::LoadConstString(APP_RE_ID_STRING_NewCount), RenRenUserInfo.nCountNewMessage);
				pTBarListItem->SetTitle(pItemsTitle);
				pTBarListItem->SetCaptionAlign(tasRightJustify);
				pTBarListItem->SetIndicatorType(itDefault);
				pTBarListItem->SetValue(1);
				if(RenRenUserInfo.nCountNewMessage> 0)
				{ 
					pTBarListItem->SetCaption(buf);				
					pTBarListItem->SetCaptionFontColor(COLOR_RED);
				}
			
			}
			//打招呼
			pTBarListItem = pTBarRow->InsertItem(pTBarListItem);
			if(pTBarListItem)
			{
					
				Int32 nLabelNameID = pTBarListItem->AddCtrl(CTL_CLASS_LABEL, 0, 0);
				TLabel* pLabelName = static_cast<TLabel*>(GetControlPtr(nLabelNameID));
				TRectangle nTRectangle(0,0,0,0);
				nTRectangle.SetRect(220, 10, 5, 5);
				pLabelName->SetBounds(&nTRectangle);
				TUString::StrCopy(pItemsTitle,TResource::LoadConstString(APP_RE_ID_STRING_Poke_Count));
				TUString::StrPrintF(buf, TResource::LoadConstString(APP_RE_ID_STRING_NewCount), RenRenUserInfo.nCountPoke);
				pTBarListItem->SetTitle(pItemsTitle);
				pLabelName->SetCaption(buf,FALSE);
				//pTBarListItem->SetCaption(buf);
				pTBarListItem->SetCaptionAlign(tasRightJustify);
				pTBarListItem->SetIndicatorType(itNone);
				pTBarListItem->SetValue(1);
				pTBarListItem->SetVisible(FALSE);	//暂时不要打招呼
				if(RenRenUserInfo.nCountPoke> 0)
				 {	
				 	pTBarListItem->SetCaptionFontColor(COLOR_RED);
				 }
			
			}
			//回复提示数
			pTBarListItem = pTBarRow->InsertItem(pTBarListItem);
			if(pTBarListItem)
			{
				TUString::StrCopy(pItemsTitle,TResource::LoadConstString(APP_RE_ID_STRING_Fangle_Count));
				TUString::StrPrintF(buf, TResource::LoadConstString(APP_RE_ID_STRING_NewCount), RenRenUserInfo.nCountFangle);
				pTBarListItem->SetTitle(pItemsTitle);
				pTBarListItem->SetCaptionAlign(tasRightJustify);
				pTBarListItem->SetIndicatorType(itDefault);
				pTBarListItem->SetValue(1);
				if(RenRenUserInfo.nCountFangle> 0)
					{
					pTBarListItem->SetCaption(buf);					
					pTBarListItem->SetCaptionFontColor(COLOR_RED);
					}
			}

		}
	
		pTBarRowList->EndUpdate();
	}

	return TRUE;
}
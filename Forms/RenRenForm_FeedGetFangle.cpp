/*!
*@FileName: RenRenForm_FeedGetFangle.cpp
*@Author: zbingwen@GoZone
*@Date: 2011-1-5
*@Brief: 留言及回复Form
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
#include "RenRenForm_FeedGetFangle.h"

#define USERBLOG_COMMENT 110
#define USERPHOTO_COMMENT 120
#define USERGUESTBOOK_COMMENT 100
#define SOMEONEBLOG_COMMENT 111
#define SOMEONEPHOTO_COMMENT 121
#define SOMEONEGUESTBOOK_COMMENT 101
#define USERSTATUS_COMMENT 130
#define USERALBUM_COMMENT 140


// 构造函数
TFeedGetFangleForm::TFeedGetFangleForm(TApplication* pApp,Boolean bNew):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	Response = NULL;
	Create(APP_RE_ID_CommonListForm);
	if(bNew == TRUE)
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_DEFAULT, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	else
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_NONE, TG3_WINDOW_MOVIE_MODE_DEFAULT);	
	
}

// 析构函数
TFeedGetFangleForm::~TFeedGetFangleForm(void)
{

	if( Response )
	{
		delete Response;	
	}


}

// 窗口事件处理
Boolean TFeedGetFangleForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
Boolean TFeedGetFangleForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	int iRet = eFailed;
	
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	SetAppTilte(this, APP_RE_ID_STRING_Fangle_Count);

	//从对应Json中获取数据,用于更新UI
	iRet = RenRenAPI_JsonParse(RR_FeedGetFangle, (void **)&Response);	
	if( Response == NULL || iRet != 0 )
	{
		return FALSE;
	}

	TCoolBarList* pTCoolBarList = NULL;
	TRectangle mTRectangle(0,0,0,0);

	if(Response->nArraySize == 0)
	{
		//无回复提示
		TPanel* pTPanel = NULL;
		pTPanel = static_cast<TPanel*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonPanel));

		TCtrl* mControlTImage;
		TCtrl* mControlTLabel;
		TImage* pTImage = NULL;
		TLabel* pTLabel = NULL;
		mControlTImage = new TImage;
		if(mControlTImage->Create(pTPanel))
		{
			mTRectangle.SetRect(8,10,50,50);
			mControlTImage->SetBounds(&mTRectangle);
			pTImage = static_cast<TImage*>(mControlTImage);
			pTImage->SetBitmap(TResource::LoadConstBitmap(APP_RE_ID_BITMAP_comment_img_with_frame));
			pTImage->SetBounds(&mTRectangle);
		}
		mControlTLabel = new TLabel;
		if(mControlTLabel->Create(pTPanel))
		{
			mTRectangle.SetRect((mTRectangle.X() + mTRectangle.Width()),(mTRectangle.Y()+ 10) ,200,50);
			mControlTLabel->SetBounds(&mTRectangle);
			pTLabel = static_cast<TLabel*>(mControlTLabel);
			pTLabel->SetCaption(TResource::LoadConstString(APP_RE_ID_STRING_NothingFangle), FALSE);
		}
		 
		goto OutCoolBarList;
	}


	pTCoolBarList = static_cast<TCoolBarList*>(GetControlPtr(APP_RE_ID_CommonListForm_CommonCoolBarList));
	if(pTCoolBarList)
	{
		mTRectangle.SetRect(6,0,308,412);
		TBarListItem* pTBarListItem = NULL;
		TBarRowList* pTBarRowList = NULL;
		TBarRow* pTBarRow = NULL;
		TFont mTFont;
		pTBarRowList = pTCoolBarList->Rows();
		if(pTBarRowList)
		{
			pTBarRowList->BeginUpdate();
			pTBarRowList->Clear();
			pTBarRow = pTBarRowList->AppendRow();
			if(pTBarRow)
			{
				pTBarRow->SetCaption(NULL);
				for(int i = 0; i < Response->nArraySize; ++i)
				{
					
					Coord mItemHeight = 0;
					pTBarListItem = pTBarRow->AppendItem();
					if(pTBarListItem)
					{	
												
						pTBarListItem->SetCaption(NULL);
						pTBarListItem->SetTitle(NULL);
						pTBarListItem->SetIndicatorType(itDefault);
						pTBarListItem->SetValue(1);

						CtrlAddItemToCoolBarList_Image(this, pTBarListItem, 10,10, 16, 16, (TBitmap*)TResource::LoadConstBitmap(APP_RE_ID_BITMAP_FeedType_comment));

						TUChar buf[256] = {0};
						char   szbuf[512] = {0};
						strcpy(szbuf,Response->Array[i].name);
						strcat(szbuf,Response->Array[i].prefix);
						if(strcmp(Response->Array[i].title,"") != 0)
						{
							strcat(szbuf,"'");
							strcat(szbuf,Response->Array[i].title);
							strcat(szbuf,"'");
						}
						strcat(szbuf,Response->Array[i].sufix);
						TUString::StrUtf8ToStrUnicode(buf, (Char *)szbuf);

						CtrlAddItemToCoolBarList_RichView(this, pTBarListItem, 30, 10, SCR_W - 70, &mItemHeight, FONT_NORMAL, buf, RGB_COLOR_BLACK);
						
						TUString::StrUtf8ToStrUnicode(buf, (const Char *)Response->Array[i].time);
						CtrlAddItemToCoolBarList_Lable(this, pTBarListItem, 30, mItemHeight + 20, 0, FONT_SMALL, buf, RGB_COLOR_LIGHTBLUE);

						mItemHeight = mItemHeight + 40;
						pTBarListItem->SetHeight(mItemHeight);
						//pTBarListItem->SetHeight(60);
					}
				}
			}
			
			pTBarRowList->EndUpdate();
		}
	}

OutCoolBarList:

	return TRUE;
}

// 关闭窗口时，保存设置信息
Boolean TFeedGetFangleForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// 控件点击事件处理
Boolean TFeedGetFangleForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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
	else if(APP_RE_ID_CommonListForm_CommonCoolBarList == nCtrlID)
	{
		TBarListItem* pListItem = NULL;
		Int32 Index = 0;
		char cbuf[50] = {0};
		TUChar Tbuf[50] = {0};
		pListItem = (TBarListItem*)(pEvent->lParam3);
		Index = pListItem->GetIndex();

		//设为已读
		char szType[16] = {0};
		char szSourceID[50] = {0};

		sprintf(szType, "%d", Response->Array[Index].type);
		DoubleToChar(Response->Array[Index].source_id, szSourceID);
		
		Set_Url_Params(RR_FeedReadFangle, "source_id",szSourceID);
		Set_Url_Params(RR_FeedReadFangle, "type", szType);
		RenRenAPICommon_Download(RR_FeedReadFangle, this->GetWindowHwndId(), 0, 0);
					
		switch(Response->Array[Index].type)
		{	
			case USERBLOG_COMMENT:
				{
					DoubleToChar(Response->Array[Index].object_id, cbuf);
					Set_Url_Params(RR_BlogGetComments, "id", cbuf);

					sprintf(cbuf, "%d", Response->Array[Index].owner_id);
					Set_Url_Params(RR_BlogGetComments, "uid", cbuf);

					Set_Url_Params(RR_BlogGetComments, "page", "1");
					Index = Index + 1;
					RenRenAPICommon_Download(RR_BlogGetComments, this->GetWindowHwndId(), 0, Index);

					bHandled = TRUE; 	
				}
				break;
			case USERPHOTO_COMMENT:
				{
					DoubleToChar(Response->Array[Index].object_id, cbuf);
					Set_Url_Params(RR_PhotosGetComments, "pid", cbuf);

					TUString::StrIToA(Tbuf, Response->Array[Index].owner_id);
					TUString::StrUnicodeToStrUtf8((Char *)cbuf, (const TUChar*)Tbuf);
					
					Set_Url_Params(RR_PhotosGetComments, "uid", cbuf);
					Set_Url_Params(RR_PhotosGetComments, "aid", "");
					Set_Url_Params(RR_PhotosGetComments, "order", "1");
					Set_Url_Params(RR_PhotosGetComments, "page", "1");
					RenRenAPICommon_Download(RR_PhotosGetComments, this->GetWindowHwndId(), 0, 0); 		
					bHandled = TRUE; 		
					return bHandled;	
				}
				break;
			case USERGUESTBOOK_COMMENT:
				{
					Set_Url_Params(RR_GuestbookGet, "uid", RenRenUserInfo.szuid);
					RenRenAPICommon_Download(RR_GuestbookGet, this->GetWindowHwndId(), 0, 0); 	
					bHandled = TRUE; 	
				}
				break;
			case SOMEONEBLOG_COMMENT:
				{
					bHandled = TRUE; 	
				}
				break;
				case SOMEONEPHOTO_COMMENT:
				{
					bHandled = TRUE; 	
				}
				break;
			case SOMEONEGUESTBOOK_COMMENT:
				{
					bHandled = TRUE; 	
				}
				break;
			case USERSTATUS_COMMENT:
				{
					DoubleToChar(Response->Array[Index].object_id, cbuf);
					Set_Url_Params(RR_StatusGetComment, "status_id", cbuf);

					TUString::StrIToA(Tbuf, Response->Array[Index].owner_id);
					TUString::StrUnicodeToStrUtf8((Char *)cbuf, (const TUChar*)Tbuf);
					Set_Url_Params(RR_StatusGetComment, "owner_id", cbuf);

					Set_Url_Params(RR_StatusGetComment, "order", "1");
					RenRenAPICommon_Download(RR_StatusGetComment, this->GetWindowHwndId(), 0, 0);
					bHandled = TRUE; 	
				}
				break;
			case USERALBUM_COMMENT:
				{
					DoubleToChar(Response->Array[Index].object_id, cbuf);
					Set_Url_Params(RR_PhotosGetComments, "aid", cbuf);

					TUString::StrIToA(Tbuf, Response->Array[Index].owner_id);
					TUString::StrUnicodeToStrUtf8((Char *)cbuf, (const TUChar*)Tbuf);
					Set_Url_Params(RR_PhotosGetComments, "uid", cbuf);
					
					Set_Url_Params(RR_PhotosGetComments, "pid", "");
				
					Set_Url_Params(RR_PhotosGetComments, "order", "1");
					Set_Url_Params(RR_PhotosGetComments, "page", "1");
					RenRenAPICommon_Download(RR_PhotosGetComments, this->GetWindowHwndId(), 0, 0); 		
					bHandled = TRUE; 		
					return bHandled;	
					bHandled = TRUE; 	
				}
				break;
			default:
				break;
		}
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
Boolean TFeedGetFangleForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//控件丢失焦点
Boolean TFeedGetFangleForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// 文本框输入处理事件
Boolean TFeedGetFangleForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}

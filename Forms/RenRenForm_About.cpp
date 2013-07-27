/*!
*@FileName: RenRenForm_About.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
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
#include "RenRenForm_About.h"


// ���캯��
TAboutForm::TAboutForm(TApplication* pApp, Boolean bNew):TWindow(pApp)
{
	m_TitleBtnLeft = -1;
	m_TitleBtnRight = -1;
	Create(APP_RE_ID_AboutForm);
	if(bNew == TRUE)
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_DEFAULT, TG3_WINDOW_MOVIE_MODE_DEFAULT);
	else
		this->SetWindowMovieMode(TG3_WINDOW_MOVIE_MODE_NONE, TG3_WINDOW_MOVIE_MODE_DEFAULT);
}

// ��������
TAboutForm::~TAboutForm(void)
{
	RenRenAPICommon_DownloadCancelByHwndId(this->GetWindowHwndId());
}

// �����¼�����
Boolean TAboutForm::EventHandler(TApplication * pApp, EventType * pEvent)
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
Boolean TAboutForm::_OnWinInitEvent(TApplication * pApp, EventType * pEvent)
{
	m_TitleBtnRight = SetAppTitleButton(this, APP_RE_ID_STRING_Back,TITLE_BUTTON_RIGHT);
	SetAppTilte(this, APP_RE_ID_STRING_About);

	return TRUE;
}

// �رմ���ʱ������������Ϣ
Boolean TAboutForm::_OnWinClose(TApplication * pApp, EventType * pEvent)
{
	return TRUE;
}

// �ؼ�����¼�����
Boolean TAboutForm::_OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent)
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
Boolean TAboutForm::_OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;	
}

//�ؼ���ʧ����
Boolean TAboutForm::_OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent)
{
	Boolean bHandled = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandled;
}

// �ı������봦���¼�
Boolean TAboutForm::_OnFldChangedEvent(TApplication * pApp, EventType* pEvent)
{
	Boolean bHandle = FALSE;
	Int32 CtrlID = pEvent->sParam1; 

	//:TODO:

	return bHandle;
}
/*!
*@FileName: RenRenForm_MainMenu.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-17
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_MAINMENU_H__
#define  __RENRENFORM_MAINMENU_H__

#include "RenRenAPI.h"

class TMainMenuForm : public TWindow
{
public:
	TMainMenuForm(TApplication* pApp, Boolean bNew = TRUE);
	virtual ~TMainMenuForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
	Int32 _CreateFunctionButtons(TApplication* pApp)	;
private:
	Int32 m_TitleBtnLeft;	//������߰�ťID
	Int32 m_TitleBtnRight;	//�����ұ߰�ťID
	Int32 m_QuickBarTakePhotoID;	//�ϴ���Ƭ
	Int32 m_QuickBarStatusID;	//��״̬
	
	// 0: ������	1: ������ҳ	2: ����
	// 3: վ����	4: �ص�		5: ���
	// 6: Ӧ��		7: ������ҳ	8: ��־
	Int32 m_nFunMenuBtnID[9];	
	Int32 m_TakePhotoID;
	Int32 m_EditID;
};

#endif
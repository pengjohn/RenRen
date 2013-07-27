/*!
*@FileName: RenRenForm_WaitingBox.h
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-23
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_WAITINGBOX_H__
#define  __RENRENFORM_WAITINGBOX_H__

#include "RenRenAPI.h"
#include "TProgressDialog.h" 

class TWaitingBoxForm : public TWindow
{
public:
	TWaitingBoxForm(TApplication* pApp);
	virtual ~TWaitingBoxForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
private:
	TProgressDialog* m_pProDialog;	
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID
};

#endif

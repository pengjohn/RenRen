/*!
*@FileName: RenRenForm_FeedGetFangle.h
*@Author: zbingwen@GoZone
*@Date: 2011-1-5
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_FEEDGETFANGLE_H__
#define  __RENRENFORM_FEEDGETFANGLE_H__

#include "RenRenAPI.h"

class TFeedGetFangleForm : public TWindow
{
public:
	TFeedGetFangleForm(TApplication* pApp, Boolean bNew);
	virtual ~TFeedGetFangleForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
private:
	Int32 m_TitleBtnLeft;	//������߰�ťID
	Int32 m_TitleBtnRight;	//�����ұ߰�ťID
	tResponseFeedGetFangle* Response;
	Int32 CoolBarListID;
};

#endif
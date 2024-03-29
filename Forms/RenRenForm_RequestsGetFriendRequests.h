/*!
*@FileName: RenRenForm_RequestsGetFriendRequests.h
*@Author: zbingwen@GoZone
*@Date: 2010-12-29
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_REQUESTSGETFRIENDREQUESTS_H__
#define  __RENRENFORM_REQUESTSGETFRIENDREQUESTS_H__

#include "RenRenAPI.h"
#define FRIENDREQUEST_MAXITEM (100)

typedef struct 
{
	Int32 m_Panel;
	Int32 m_CoolBarList;
	Int32 m_ButtonYes[FRIENDREQUEST_MAXITEM];
	Int32 m_ButtonNo[FRIENDREQUEST_MAXITEM];
}GetFriendRequestsFormCtrlID;

typedef struct	
{
	Int32 id;
	Int32 HeadMaskButtonID;	//	
	TBitmap* pUserImageBmp;
}GetFriendRequestsItemData;

class TRequestsGetFriendRequestsForm : public TWindow
{
public:
	TRequestsGetFriendRequestsForm(TApplication* pApp);
	virtual ~TRequestsGetFriendRequestsForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
	Boolean _OnHandleButtonEvent(Int32 nCtrlID);	
	Boolean _UpdateList(TApplication * pApp, tResponseRequestsGetFriendRequests* Response);
	
	//Boolean OnLstPreDrawEvent(TApplication * pApp, EventType* pEvent);
private:
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID
	//Int32 m_ImageCount; 
	tResponseRequestsGetFriendRequests* Response;
	GetFriendRequestsFormCtrlID m_CtrlID;
	GetFriendRequestsItemData  m_ItemData[FRIENDREQUEST_MAXITEM];
	Int32 ItemCount;
	Int32 type;//零代表接受请求；1代表拒绝或者忽略请求；缺省为0
	Int32 uid;
};

#endif

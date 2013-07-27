/*!
*@FileName: RenRenForm_MessageGets.h
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-27
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_MESSAGEGETS_H__
#define  __RENRENFORM_MESSAGEGETS_H__

#include "RenRenAPI.h"

#define MESSAGELIST_MAXITEM 	(500)

#define MESSAGE_TYPE_INBOX		(1)
#define MESSAGE_TYPE_OUTBOX		(2)

typedef struct
{
	Int32 Panel;
	Int32 CoolBarList;
}sMessageGetsFormCtrID;

typedef struct
{
	Int32 id;				//id
	Int32 HeadMaskButtonID;	//	
	TBitmap* pBmp;		//Bmp
	Int32 TitleIDUnread;	//未读的title id
	Int32 TitleIDRead;	//已读的title id
}tMessageList_ItemData;

class TMessageGetsForm : public TWindow
{
public:
	TMessageGetsForm(TApplication* pApp);
	virtual ~TMessageGetsForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
private:
	int	nMessageBoxType;
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID

	sMessageGetsFormCtrID m_CtrID;
	tMessageList_ItemData gItemData[MESSAGELIST_MAXITEM];	
};

#endif

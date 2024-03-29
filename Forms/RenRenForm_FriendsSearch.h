/*!
*@FileName: RenRenForm_FriendsSearch.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-2-25
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_FRIENDSSEARCH_H__
#define  __RENRENFORM_FRIENDSSEARCH_H__

#include "RenRenAPI.h"

#define FRIENDSSEARCH_MAXITEM  (200)

typedef struct
{
	Int32 id;				//id
	Int32 ButtonId;		//
	TBitmap* pBmp;		//Bmp
	Int32 MaskButtonID;
}tFriendsSearch_ItemData;

class TFriendsSearchForm : public TWindow
{
public:
	TFriendsSearchForm(TApplication* pApp, Boolean bNew = TRUE);
	virtual ~TFriendsSearchForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
	Boolean _UpdateList(TApplication * pApp, tResponseFriendsSearch* Response);
private:
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID

	Int32 m_ItemCount;
	tFriendsSearch_ItemData gItemData[FRIENDSSEARCH_MAXITEM];		
};

#endif

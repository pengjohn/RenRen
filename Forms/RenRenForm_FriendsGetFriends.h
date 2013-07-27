/*!
*@FileName: RenRenForm_FriendsGetFriends.h
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-21
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_FRIENDSGETFRIENDS_H__
#define  __RENRENFORM_FRIENDSGETFRIENDS_H__

#include "RenRenAPI.h"


#define FRIENDLIST_MAXITEM  (500)
#define FRIENDLIST_MAXINPUT  (16)
typedef struct
{
	Int32 id;				//id
	TUChar  pszName[64];
	Int32 HeadMaskButtonID;	//		
	TBitmap* pBmp;		//Bmp
	Int32 nOptionButtonID;	//������ťid
}tFriendList_ItemData;

enum
{
	FRIENDLSIT_MODE_ALL = 0,
	FRIENDLSIT_MODE_VISITOR,
	FRIENDLSIT_MODE_PAGES,
	FRIENDLSIT_MODE_MESSAGE,		//������Ϣ
};

class TFriendsGetFriendsForm : public TWindow
{
public:
	TFriendsGetFriendsForm(TApplication* pApp, int  nListMode = FRIENDLSIT_MODE_ALL, Boolean bNew = TRUE);
	virtual ~TFriendsGetFriendsForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
	Boolean _CreateListAll(TApplication * pApp);	
	Boolean _CreateListVisitor(TApplication * pApp);	
	Boolean _CreateListPages(TApplication * pApp);
	Boolean _UpdateListByInput(TApplication * pApp);
private:
	int  m_nListMode;
	
	Int32 m_TitleBtnLeft;	//������߰�ťID
	Int32 m_TitleBtnRight;	//�����ұ߰�ťID
	sBottomTabCtrID 	BottomTabCtrID;	//�ײ�Tab�Ŀؼ�ID
	
	tFriendList_ItemData gItemData[FRIENDLIST_MAXITEM];	

	TUChar	m_SearchInput[FRIENDLIST_MAXINPUT+1];
};

#endif
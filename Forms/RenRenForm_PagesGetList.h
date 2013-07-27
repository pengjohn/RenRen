/*!
*@FileName: RenRenForm_PagesGetList.h
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-28
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_PAGESGETLIST_H__
#define  __RENRENFORM_PAGESGETLIST_H__

#include "RenRenAPI.h"

typedef enum
{
	PAGESLIST_MODE_NORMAL = 0,	//����
	PAGESLIST_MODE_MYHOME,		//��Bottom_Tab��
}ePagesListMode;

#define PAGESLIST_MAXITEM	(500)
typedef struct
{
	Int32 Panel;
	Int32 CoolBarList;
}sPagesGetListFormCtrID;

typedef struct
{
	Int32 id;				//id
	TBitmap* pBmp;		//Bmp
	Int32 MaskButtonID;
}tPagesList_ItemData;

class TPagesGetListForm : public TWindow
{
public:
	TPagesGetListForm(TApplication* pApp);
	virtual ~TPagesGetListForm(void);
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

	sPagesGetListFormCtrID m_CtrID;
	tPagesList_ItemData gItemData[PAGESLIST_MAXITEM];		
};

#endif
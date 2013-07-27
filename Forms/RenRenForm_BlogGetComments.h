/*!
*@FileName: RenRenForm_BlogGetComments.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-7
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_BLOGGETCOMMENTS_H__
#define  __RENRENFORM_BLOGGETCOMMENTS_H__

#include "RenRenAPI.h"

#define BLOGGETCOMMENTS_MAXITEM	(200)

typedef struct
{
	Int32 uid;				//id
	Boolean bWhisper;		//���Ļ�,�ظ�Ĭ��ҲҪ�����Ļ�
	Int32 HeadMaskButtonID;	//	
	Int32 OptionButtonID;
	Int32 NameID;
	TBitmap* pBmp;		//Bmp
}tBlogGetComments_ItemData;

class TBlogGetCommentsForm : public TWindow
{
public:
	TBlogGetCommentsForm(TApplication* pApp, Boolean bNew = TRUE);
	virtual ~TBlogGetCommentsForm(void);
	virtual Boolean EventHandler(TApplication * pApp, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	

	Boolean _UpdateList(TApplication * pApp, tResponseBlogGetComments* Response);
private:
	Int32 m_TitleBtnLeft;	//������߰�ťID
	Int32 m_TitleBtnRight;	//�����ұ߰�ťID
	Int32 m_MoreBtn;
	Coord m_OffsetY;			//�¼�������Ŀ����ʼλ��
	Int32 m_ItemCount;		//����ͳ��,���������
	
	tBlogGetComments_ItemData gItemData[BLOGGETCOMMENTS_MAXITEM];
};

#endif
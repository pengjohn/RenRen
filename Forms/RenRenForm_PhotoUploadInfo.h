/*!
*@FileName: RenRenForm_PhotoUploadInfo.h
*@Author: lee-5959f8a390f@GoZone
*@Date: 2011-6-8
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENFORM_PHOTOUPLOADINFO_H__
#define  __RENRENFORM_PHOTOUPLOADINFO_H__

#include "RenRenAPI.h"

class TPhotoUploadInfoForm : public TWindow
{
public:
	TPhotoUploadInfoForm(TApplication* pApp, Int32 nHwndId, Boolean bNew = TRUE);
	virtual ~TPhotoUploadInfoForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);	
private:
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID

	TBitmap* pPhotoBmp;	
	PostNotifyMsgDataType m_msgData;	
};

#endif

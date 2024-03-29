/*!
*@FileName: RenRenForm_UsersGetInfo.h
*@Author: zengying@GoZone
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

#ifndef  __RENRENFORM_USERSGETINFO_H__
#define  __RENRENFORM_USERSGETINFO_H__

#include "RenRenAPI.h"

typedef enum
{
	RR_BaseInfo,
	RR_SchoolInfo,
	RR_WorkInfo,
	RR_PersonalInfo,
	RR_ContactInfo,
}RR_UsersInfoType;

typedef struct
{
	Int32 LabelStingID; //标签所对应的字串ID
	TUChar ValueString[128]; //值，应用自己从Response 获取
	Boolean			IsShow;	//是否显示，应用从Response有获得相应的数据则显示
}sUsersInfoItem;

typedef struct
{
	RR_UsersInfoType	nType;
	Int32 RowCaptionStingID; //行标题所对应的字串ID
	sUsersInfoItem		*Items;
	int 				nItemsNum;
	Boolean			IsShow;			//是否显示，应用从Response有获得相应的数据则显示
}sUsersInfoRow;


class TUsersGetInfoForm : public TWindow
{
public:
	TUsersGetInfoForm(TApplication* pApp);
	virtual ~TUsersGetInfoForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);
	void _SetDataToCtrls(TApplication* pApp);	
	Int32 _SetCoolBarList(TApplication* pApp);
	
private:
	tResponseUsersGetInfo* Response;
	char* m_uid;
	//用户图标
	TBitmap* pUserImageBmp;
	
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID
	sBottomTabCtrID 	BottomTabCtrID;	//底部Tab的控件ID	
	Int32 m_ProfileImageID;
	
	sUsersInfoItem* ArrUsers_WorkInfo;//工作信息

};

#endif

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
	Int32 LabelStingID; //��ǩ����Ӧ���ִ�ID
	TUChar ValueString[128]; //ֵ��Ӧ���Լ���Response ��ȡ
	Boolean			IsShow;	//�Ƿ���ʾ��Ӧ�ô�Response�л����Ӧ����������ʾ
}sUsersInfoItem;

typedef struct
{
	RR_UsersInfoType	nType;
	Int32 RowCaptionStingID; //�б�������Ӧ���ִ�ID
	sUsersInfoItem		*Items;
	int 				nItemsNum;
	Boolean			IsShow;			//�Ƿ���ʾ��Ӧ�ô�Response�л����Ӧ����������ʾ
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
	//�û�ͼ��
	TBitmap* pUserImageBmp;
	
	Int32 m_TitleBtnLeft;	//������߰�ťID
	Int32 m_TitleBtnRight;	//�����ұ߰�ťID
	sBottomTabCtrID 	BottomTabCtrID;	//�ײ�Tab�Ŀؼ�ID	
	Int32 m_ProfileImageID;
	
	sUsersInfoItem* ArrUsers_WorkInfo;//������Ϣ

};

#endif
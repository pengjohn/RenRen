// Application application header file.
 
// Original file name: App_KaiXinAPI.h
// Generated by TOPS Builder:Project wizard,Date:2010-8-24
 

#ifndef  __KaiXinAPI_Edit_H__
#define  __KaiXinAPI_Edit_H__
#include "TG3.h"


#define EDIT_MAX_LENGTH_TITLE		(128)	//必须要大于所有模式的最大值
#define EDIT_MAX_LENGTH_CONTENT	(1024)	//必须要大于所有模式的最大值
#define EDIT_MAX_LENGTH_RECEIVER	(128)


//表情图片字串信息,保存用于表情编辑
typedef struct
{
	Int32 nExpIcon;//表情图片ID
	Int32 nExpStringId;//表情对应的字串ID	
}tExpression;

struct PostNotifyMsgDataType
{
	//AppEditType       nEditType;	//编辑类型,对应AppEditType
	
	Int32 nAppTitleStrID; //应用窗口标题资源邋ID
	Int32 nPublicBtnCaptionStrID;//发表按钮字串资源ID
	
	Int32 nTitleEditMaxChars;//标题编辑框最多字符数
	Int32 nContentEditMaxChars;//正文编辑框最多字符数
	
	Boolean IsExpressionBtn;//窗口是否有表情编辑
	Boolean IsPhotoBtn;//窗口是否有上传图片功能
	Boolean IsLocationBtn;//是否有GPS定位功能
	Boolean IsPrivacyCtl;//是否有私密设置控件
	Boolean IsTitleEdit;//有没有标题编辑框
	Boolean IsReceiverEdit;//有没有收件人编辑框
	Boolean IsPopEdit;//是否为弹出式编辑框
	Boolean IsPwdInput;//是否密码输入
	Boolean IsAllowsNull;	//是否允许为空
	
	Int32	nHwndId;		//主程序的窗口ID
	Int32	nCtrlId;			// 主程序的控件ID //没有用到
	Int32	nParam;			//参数1: 
	void* 	pThisHandle;	//保存应用new IDownLoad的类指针,用于返给给应用delete

	char    	pszReceiver[EDIT_MAX_LENGTH_RECEIVER*3 +1]; //保存收件人内容     
	char    	pszEditTitle[EDIT_MAX_LENGTH_TITLE*3 + 1]; //保存编辑标题 内容    
	char		pszEditContent[EDIT_MAX_LENGTH_CONTENT*3 + 1]; //保存编辑内容 	
	Boolean	bPrivacy;//是否私密TRUE：是 FALSE：否
		
	//GPS定位数据储存
	char	  pszLocation[128]; 	//地点
	char	  pszLat[16];			//纬度
	char	  pszLon[16];			//经度

};

class TAppEditForm : public TWindow
{
public:
	TAppEditForm( TApplication *pApp );
	TAppEditForm( TApplication *pApp, PostNotifyMsgDataType in_msgData);
	virtual ~TAppEditForm(void);
	virtual Boolean EventHandler( TApplication * pApp, EventType * pEvent );
private:
// define private method here
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Int32 _OnCreateEditInterface(TApplication * pApp);
	Int32 _OnNotifyToMain();
	Int32 _OnSetMsgData();
private:
	
Boolean bModified;

#if 0 //变成以传参形式赋值
const TUChar* m_AppTitleString;
const TUChar* m_AppEditTitleString;
const TUChar* m_AppPublishBtnCaptionString;

Int32 m_nTitleEditMaxChars;//标题编辑框最多字符数
Int32 m_nContentEditMaxChars;//正文编辑框最多字符数

Boolean IsExpressionBtn;
Boolean IsPhotoBtn;
Boolean IsLocationBtn;
Boolean IsWhisperCtl;
Boolean IsTitleEdit;//记录有没有收件人，或者标题
#endif

//Save special control ID
Int32 m_TitleBtnLeft;	//标题左边按钮ID
Int32 m_TitleBtnRight;	//标题右边按钮ID

Int32 m_nExpressionBtnID;//表情按钮
Int32 m_nPhotoBtnID;//照片按钮
Int32 m_nPrivacyCtlID;//私密控件

Int32 m_nLocationBtnID;//位置按钮
Int32 m_nLocationLblID;//位置文字标签

Int32 m_nTitleEditID;
Int32 m_nContentEditID;//内容编辑控件
Int32 m_nExpressionTPanelID;//表情TPanel编辑控件
Boolean m_bShowExpress;	//是否显示表情

Int32 m_nExpCtrlID[32*3];//暂定3页，每页32个

PostNotifyMsgDataType m_msgData;

/** 主程序GTID */
SS_GTID 		gtidMain;
};
 

#endif

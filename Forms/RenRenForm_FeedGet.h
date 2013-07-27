/*!
*@FileName: RenRenForm_FeedGet.h
*@Author: zengying@GoZone
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

#ifndef  __RENRENFORM_FEEDGET_H__
#define  __RENRENFORM_FEEDGET_H__

#include "RenRenAPI.h"

typedef enum
{
	FEED_MODE_NEW = 0,	//新鲜事
	FEED_MODE_MYHOME,	//个人主页
	FEED_MODE_FRIENDHOME,	//好友主页
}eFeedMode;

typedef enum
{
	POP_OPTION_NULL = 0,
	POP_OPTION_SET_READ,		//设为已读,对于新鲜事就是所谓的"删除"		
	POP_OPTION_DELETE_ME,		//删除自己的新鲜事
	POP_OPTION_TRANSMIT,		//转发
	POP_OPTION_REPLY_STATUS,			//回复状态
	POP_OPTION_SHARE,			//共享
	POP_OPTION_TAKE_PHOTO,	//照相
	POP_OPTION_SELECT_FILE	//选择文件
}ePopOption;

typedef struct
{
	ePopOption nOptionID;
	Int32	   nStrID;
}PopMenu;

//Feed 最大条数
#define RR_FEED_ITEM_COUNT_MAX		(100)
//缩略图片最大数
#define RR_FEED_ITEM_THUMBNAIL_COUNT_MAX (4)//每条最多显示缩略图

typedef struct
{
	//用户logo
	Int32 nLogoID;
	TBitmap* pLogoBmp;
	//Option Ctrls
	Int32 nOptionID;
	//Fname Ctrls
	Int32 FnameCtrlID;
	//Title Ctrls
	Int32 TitleCtrlID;
	//Content Ctrls
	Int32 ContentCtrlID;
	//Comment Ctrls
	Int32 CommentContCtrlID;
	//RowItemValue作为删除行的关键字,对应TBarListItem->SetValue/GetValue
	Int32 RowItemValue;
	//缩略图	
	TBitmap* pThumbnailBmp[RR_FEED_ITEM_THUMBNAIL_COUNT_MAX];
	Int32 ThumbnailCtrlID[RR_FEED_ITEM_THUMBNAIL_COUNT_MAX];
	double Thumbnail_media_id[RR_FEED_ITEM_THUMBNAIL_COUNT_MAX];
	int Thumbnail_owner_id[RR_FEED_ITEM_THUMBNAIL_COUNT_MAX];

	int actor_id;
	double source_id;
	double post_id;
	int feed_type;
	char message[1024 + 1];
	char href[256+1];
	double Attachment0_media_id;	//第0个Attachment的media_id
	int Attachment0_owner_id;		//第0个Attachment的owner_id
	
	//PopMenu
	PopMenu* tPopMenu;
	int  nPopMenuCount;	
	Boolean bPage;		//是否是公共主页
}tFeedGet_ItemData;


//新鲜事类型
typedef enum
{
	RR_FeedTypeStart,		
	RR_FeedTypeError = -1,	//下载信息不完整或者有误	

	RR_FeedUpdateStatus = 10,		//10：个人更新状态
	RR_FeedUpdateStatusForPage = 11,		//11：公共主页更新状态

	RR_FeedPostBlog = 20,		//20：发表日志
	RR_FeedShareBlog = 21,		//21：分享日志
	RR_FeedPublicPostBlog = 22, 	//22：网络发表日志
	RR_FeedShareBlogForPage = 23,	//23.    blog share for page

	RR_FeedUploadPhoto = 30,		//30：上传了* 张照片、
	RR_FeedPublicUploadPhoto = 31,		//31：网络上传了* 张照片、
	RR_FeedSharePhoto = 32,			//32: 分享照片
	RR_FeedShareAlbum = 33, 	//33：分享相册
	RR_FeedProfilePictureUpdated = 34,//34	profile picture updated
	RR_FeedProfilePictureUpdatedForPage = 35,//35	profile picture updated for page
	RR_FeedPhotoSharedForPage = 36,	//36	photo shared for page

	RR_FeedAddAsFriends = 40,//40	add as friends
	RR_FeedAddPublicWeb = 41,		//41：加入了公共主页

	RR_FeedShareVideo = 50,		//54：分享视频
	RR_FeedLinkShared = 51, //51	link shared
	RR_FeedAudioShared = 52, //52	audio shared
	RR_FeedVideoShareForPage = 53, //53	video shared for page
	RR_FeedLinkSharedForPage = 54,		//54：分享链接
	RR_FeedAudioShareForPage = 55,//55	audio shared for page

	RR_FeedComment = 60,		//60：留言
	
	//其他类型（不属于某种应用，比如好友头像更新等）
	RR_FeedPraise,			//赞
	RR_FeedTypeEnd
}RR_FeedType;

class TFeedGetForm : public TWindow
{
public:
	TFeedGetForm(TApplication* pApp, eFeedMode nFeedMode, Boolean bNew = TRUE);
	virtual ~TFeedGetForm(void);
	virtual Boolean EventHandler(TApplication * appP, EventType * eventP);

private:
	Boolean _OnWinInitEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnWinClose(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSelectEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlSetFocusEvent(TApplication * pApp, EventType * pEvent);
	Boolean _OnCtrlKillFocusEvent(TApplication * pApp, EventType * pEvent);	
	Boolean _OnFldChangedEvent(TApplication * pApp, EventType* pEvent);
	Int32 _SetCoolBarList(TApplication* pApp);
	Int32 _UpdateCoolBarList(TApplication* pApp, tResponseFeedGet* Response);	
	Int32 _AddMoreLabel(TBarListItem* lpItem, TLabel* pIntroLabel, TFont objFontType,TUChar* pChar,Int32 ItemHeight,Int32 CoolBarDiffTop, TRectangle Rc_Temp);	
	Boolean _OnCoolBarListPreDraw(TApplication * pApp, EventType * pEvent);
	Boolean _OnCoolCtrlSelected(TApplication * pApp, Int32 nCtrlID);
	Boolean _UpdateProfilePhoto(TApplication* pApp, tResponseUsersGetInfo* ResponseInfoPhoto);	
private:
	eFeedMode 	m_FeedMode;	//Feed模式
	Boolean bHaveBottomTab;	//是否有底部Tab
	Boolean bHaveProfile;		//是否有个人资料
	Boolean bHaveQuickBar;	//是否有快速发表栏
	
	//tResponseFeedGet* Response;
	char m_uid[32];
	char  m_pszName[64];
	
	Int32 m_TitleBtnLeft;	//标题左边按钮ID
	Int32 m_TitleBtnRight;	//标题右边按钮ID
	Int32 m_QuickBarTakePhotoID;	//上传照片
	Int32 m_QuickBarStatusID;	//新状态
	Int32 m_MoreBtnID;			//更多按钮
	sBottomTabCtrID 	BottomTabCtrID;	//底部Tab的控件ID

	tFeedGet_ItemData gItemData[RR_FEED_ITEM_COUNT_MAX];

	TBitmap * pProfileImage;	//Profile的头像
	Int32       m_ProfileImageID;
	
	//用户小图标
	Int32 nThumImgIndex;//缩略图部分的index 必须从RR_FEED_ITEM_COUNT_MAX 开始

	Int32 PreSelectRichViewID;//保存前一个被选中的richview	ID	
	
	Int32 m_ItemCount;	//条数统计,包括更多的
};

#endif

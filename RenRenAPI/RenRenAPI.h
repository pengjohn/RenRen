/*!
*@FileName: RenRenAPI.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/
#ifndef  __RenRenAPI_H__
#define  __RenRenAPI_H__
#include "TG3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "CommonMacro.h"


#define RenRen_API_Key 	"92db9d3a2657490dbaff10f942b2d9f9"
#define RenRen_Secret_Key "ac6831a97c6f48c29021d2f5acf00b4a"

#define RENREN_PARAM_INBOX	"1"
#define RENREN_PARAM_OUTBOX	"0"


#define RENREN_FOLDER		"RenRen/"
#define RENREN_CONFIG_FILE		"renren.cfg"

#define RENREN_URLPATH_LENGTH		(1024)	//url最大长度
#define ERROR_INFO_LEN				(100)		//错误信息最大长度

#define RENREN_LENGTH_BLOG_TITLE		(99)		//日记标题最大长度
#define RENREN_LENGTH_BLOG_CONTENT	(1023)	//日记内容最大长度
#define RENREN_LENGTH_BLOG_COMMENT	(139)	//日记评论长度

#define RENREN_LENGTH_PHOTO_INFO		(99)	//照片说明
#define RENREN_LENGTH_PHOTO_COMMENT		(499)//照片评论

#define RENREN_LENGTH_PASSWORD		(16)	//密码
#define RENREN_LENGTH_MESSAGE		(499)//短信


#define RENREN_LENGTH_STATUS		(140)	//状态内容最大长度

#define WAITING_BOX_TRUE	(TRUE)
#define WAITING_BOX_FALSE	(FALSE)

typedef enum
{
	FORM_MODE_NORMAL = 0,	//正常
	FORM_MODE_MYHOME,	//带Bottom_Tab的
}eFormMode;


//日志为1、照片为2、链接为6、相册为8、视频为10、音频为11
#define SHARE_TYPE_BLOG	"1"
#define SHARE_TYPE_PHOTO	"2"
#define SHARE_TYPE_LINK	"6"
#define SHARE_TYPE_ALBUM	"8"
#define SHARE_TYPE_VIDEO	"10"
#define SHARE_TYPE_AUDIO	"11"
	
enum
{
	MSG_POST_THREAD_NOTIFY = EVENT_FirstUser + 1000,
	MSG_SWITCHFORM_FEED,			//新鲜事
	MSG_SWITCHFORM_FRIEND_LIST,	//好友列表
	MSG_SWITCHFORM_PROFILE,		//我的地盘
	MSG_SWITCHFORM_APPLIST,		//应用列表
	MSG_LOGOUT,					//退出登录
	MSG_EXIT,						//退出应用
	MSG_BACKTO_HOME,				//返回主界面
	MSG_SWITCHFORM_USEREVENT,	//用户提醒界面
	MSG_FORM_UPDATE,				//界面更新,如消息中心等
	MSG_CONNECT_INIT_TIMEROUT,	//启动联网超时
};

//AppID (keynumber)
typedef enum
{
	RR_AccessTypeStart = 0,

	RR_InitialLogin,			//01. Initial Login	
	RR_ReGetSessionID,		//01. Initial Login引申	

	RR_PhoneclientActiveClient,
	
	RR_BatchRun,			//02. 
	RR_BlogAddBlog,			//03. 
	RR_BlogGets,			//04. 
	RR_BlogGetCategory,		//05. 
	RR_BlogGet,				//06. 
	RR_BlogAddComment,	//07. 
	RR_BlogGetComments,	//08. 
	RR_BlogGetCommentsMore,	//08. 	
	
	RR_FeedRead,			//09. 
	RR_FeedGetFangle,		//10. 
	RR_FeedReadFangle,		//11. 
	RR_FeedGet,				//12. 
	RR_FeedGetMore,		//RR_FeedGet引申
	
	RR_FriendsGetFriends,	//13. 
	RR_FriendsAreFriends,	//14. 
	RR_FriendsSearch,		//15. 
	RR_FriendsSearchMore,		//15. 
	
	RR_GuestbookGet,		//16. 
	RR_GuestbookPost,		//17.
	
	RR_MessageGets,		//18. 
	RR_MessageGet,		//19.
	RR_MessageSent,		//20.
	RR_MessageMark,		//

	RR_PagesIsFan,		//21.
	RR_PagesBecomeFan,	//22.
	RR_PagesGetList,		//23.
	RR_PagesGetInfo,		//24.

	RR_PhotosGet, 			//25.
	RR_PhotosGetMore,	//25.
	
	RR_PhotosGetAlbums,		//26.
	RR_PhotosGetComments,	//27.
	RR_PhotosGetCommentsMore,	//27.
	RR_PhotosUpload,		//28.
	RR_PhotosAddComment,	//29.

	RR_ProfileGetInfo,				//30. RR_ProfileGetInfo返回后会马上下载RR_FeedGet

	RR_RequestsDealFriendRequest,	//31.
	RR_RequestsGetFriendRequests,	//32.
	RR_RequestsSendFriendRequest,	//33.
	
	RR_SharePublish, 			//34.

	RR_StatusGets,			//35.
	RR_StatusSet,			//36
	RR_StatusAddComment,	//37.
	RR_StatusGet,			//38.
	RR_StatusGetComment,	//39.
	RR_StatusGetCommentMore,	//39.	
	RR_StatusGetEmoticons,	//40.

	RR_UsersGetInfo,		//41.
	RR_UsersGetInfoPhoto,
	RR_UsersGetVisitors,	//42
	RR_UsersSetHeadPhoto,	//43.
	RR_UsersGetEvent,		//44.	

	//以下非API，仅用于UI
	RR_More, 				//more  app 没有对应的api
	RR_MainMenu,			//主菜单
	RR_FeedGetNew,			//新鲜事
	
	RR_AccessTypeEnd,

	RR_PhotoDownload,			//专门用于图片下载,需要放在RR_AccessTypeEnd之后
} RR_AccessType;


typedef struct
{
	char paramName[32]; //至少需要多留一个字符
	char paramValue[4096]; //TODO, 需要扩展大小
}sUrlparams;


typedef struct
{
	RR_AccessType	nType;
	Boolean			bHandle;			//是否需要App统一处理
	Boolean			bNeedWaitingBox;	//是否需要等待动画
	char				FilePath[32];
	char				Address[RENREN_URLPATH_LENGTH];
	sUrlparams		*Params;
	int 				nParamsNum;
}sRRAppUrl;


//用户信息,需要保存用于自动登录
typedef struct
{
	Boolean	bAutoLogin;
	Boolean	bSavePwd;
	Boolean	bShowPwd;
	char user[128];
	char password[32];
}tUserInfoNeedSave;

//用户信息,不需要保存
typedef struct
{
	char session_key[128];
	char ticket[64];
	int	uid;
	char szuid[32];
	char status[1024];//状态内容	
	int nCountFriendRequest;	//好友请求数
	int nCountNewMessage;	//新消息数
	int nCountPoke;	//打招呼数
	int nCountFangle;	//回复数
}tUserInfo;



extern tUserInfo RenRenUserInfo;
extern tUserInfoNeedSave	 RenRenUserInfoNeedSave;
extern Int32 gHwndId_RenRenApp[RR_AccessTypeEnd];
extern Boolean bIsPage;
	
extern int RenRenAPI_GetConfig(void);
extern int RenRenAPI_SetConfig(void);
#endif

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

#define RENREN_URLPATH_LENGTH		(1024)	//url��󳤶�
#define ERROR_INFO_LEN				(100)		//������Ϣ��󳤶�

#define RENREN_LENGTH_BLOG_TITLE		(99)		//�ռǱ�����󳤶�
#define RENREN_LENGTH_BLOG_CONTENT	(1023)	//�ռ�������󳤶�
#define RENREN_LENGTH_BLOG_COMMENT	(139)	//�ռ����۳���

#define RENREN_LENGTH_PHOTO_INFO		(99)	//��Ƭ˵��
#define RENREN_LENGTH_PHOTO_COMMENT		(499)//��Ƭ����

#define RENREN_LENGTH_PASSWORD		(16)	//����
#define RENREN_LENGTH_MESSAGE		(499)//����


#define RENREN_LENGTH_STATUS		(140)	//״̬������󳤶�

#define WAITING_BOX_TRUE	(TRUE)
#define WAITING_BOX_FALSE	(FALSE)

typedef enum
{
	FORM_MODE_NORMAL = 0,	//����
	FORM_MODE_MYHOME,	//��Bottom_Tab��
}eFormMode;


//��־Ϊ1����ƬΪ2������Ϊ6�����Ϊ8����ƵΪ10����ƵΪ11
#define SHARE_TYPE_BLOG	"1"
#define SHARE_TYPE_PHOTO	"2"
#define SHARE_TYPE_LINK	"6"
#define SHARE_TYPE_ALBUM	"8"
#define SHARE_TYPE_VIDEO	"10"
#define SHARE_TYPE_AUDIO	"11"
	
enum
{
	MSG_POST_THREAD_NOTIFY = EVENT_FirstUser + 1000,
	MSG_SWITCHFORM_FEED,			//������
	MSG_SWITCHFORM_FRIEND_LIST,	//�����б�
	MSG_SWITCHFORM_PROFILE,		//�ҵĵ���
	MSG_SWITCHFORM_APPLIST,		//Ӧ���б�
	MSG_LOGOUT,					//�˳���¼
	MSG_EXIT,						//�˳�Ӧ��
	MSG_BACKTO_HOME,				//����������
	MSG_SWITCHFORM_USEREVENT,	//�û����ѽ���
	MSG_FORM_UPDATE,				//�������,����Ϣ���ĵ�
	MSG_CONNECT_INIT_TIMEROUT,	//����������ʱ
};

//AppID (keynumber)
typedef enum
{
	RR_AccessTypeStart = 0,

	RR_InitialLogin,			//01. Initial Login	
	RR_ReGetSessionID,		//01. Initial Login����	

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
	RR_FeedGetMore,		//RR_FeedGet����
	
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

	RR_ProfileGetInfo,				//30. RR_ProfileGetInfo���غ����������RR_FeedGet

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

	//���·�API��������UI
	RR_More, 				//more  app û�ж�Ӧ��api
	RR_MainMenu,			//���˵�
	RR_FeedGetNew,			//������
	
	RR_AccessTypeEnd,

	RR_PhotoDownload,			//ר������ͼƬ����,��Ҫ����RR_AccessTypeEnd֮��
} RR_AccessType;


typedef struct
{
	char paramName[32]; //������Ҫ����һ���ַ�
	char paramValue[4096]; //TODO, ��Ҫ��չ��С
}sUrlparams;


typedef struct
{
	RR_AccessType	nType;
	Boolean			bHandle;			//�Ƿ���ҪAppͳһ����
	Boolean			bNeedWaitingBox;	//�Ƿ���Ҫ�ȴ�����
	char				FilePath[32];
	char				Address[RENREN_URLPATH_LENGTH];
	sUrlparams		*Params;
	int 				nParamsNum;
}sRRAppUrl;


//�û���Ϣ,��Ҫ���������Զ���¼
typedef struct
{
	Boolean	bAutoLogin;
	Boolean	bSavePwd;
	Boolean	bShowPwd;
	char user[128];
	char password[32];
}tUserInfoNeedSave;

//�û���Ϣ,����Ҫ����
typedef struct
{
	char session_key[128];
	char ticket[64];
	int	uid;
	char szuid[32];
	char status[1024];//״̬����	
	int nCountFriendRequest;	//����������
	int nCountNewMessage;	//����Ϣ��
	int nCountPoke;	//���к���
	int nCountFangle;	//�ظ���
}tUserInfo;



extern tUserInfo RenRenUserInfo;
extern tUserInfoNeedSave	 RenRenUserInfoNeedSave;
extern Int32 gHwndId_RenRenApp[RR_AccessTypeEnd];
extern Boolean bIsPage;
	
extern int RenRenAPI_GetConfig(void);
extern int RenRenAPI_SetConfig(void);
#endif

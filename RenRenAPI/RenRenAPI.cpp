/*!
*@FileName: RenRenAPI.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 人人API主结构
*@Log: 	Author 			Date 		Description
*
*/

#include "RenRenAPI.h"
#include "RenRenAPICommon.h"
#include "md5.h"

//01. 
sUrlparams  Urlparam_InitalLogin[] = 
{
	{"api_key",	RenRen_API_Key},
	{"v",		"1.0"},
	{"format",	"JSON"},	
	{"fromid",	"8000801"},
	{"login_info",	"{\"cellid\":0, \"ver\":\"\", \"from\":8000801}"},
	
	{"user",	""},	
	{"password",""},
};


sUrlparams  Urlparam_PhoneclientActiveClient[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	//{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"phoneclient.activeClient"},
	
	{"from",		"8000801"},
	{"data",		"[{\"type\":1,\"num\":1}]"},
	
};

//02. 
sUrlparams	Urlparam_BatchRun[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	//{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"batch.run"},
	
	{"method_feed",		"1"},
};


//03. 
sUrlparams	Urlparam_BlogAddBlog[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.addBlog"},
	
	{"title", 	""},
	{"content",	""},
	{"visable",	""},
	{"password", ""},//用户设置的密码	
};

//04. 
sUrlparams	Urlparam_BlogGets[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.gets"},

	{"uid", 	""},//用户的ID或公共主页的ID
	{"page",		"1"},
	{"count",		"50"},
};

//05. 
sUrlparams	Urlparam_BlogGetCategory[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.getCategory"},
};

//06.
sUrlparams	Urlparam_BlogGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.get"},
	
	{"id",		""},
	{"uid", 	""},//日志所有者的用户ID或公共主页的ID
	{"comment",		"0"},//返回评论数，最大值为50，默认值0
	{"password",		""},//日志的密码（当日志有密码时）
};

//07. 
sUrlparams	Urlparam_BlogAddComment[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.addComment"},
	
	{"id",		""},
	{"content",		""},
	{"uid",		""},//日志所有者的用户ID或公共主页的ID
	{"rid", 	""},
	{"type", 	""},
};

//08. 
sUrlparams	Urlparam_BlogGetComments[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"blog.getComments"},
	
	{"id",		""},
	{"uid", 	""},//日志所有者的用户ID或公共主页的ID
	{"page",		"1"},
	{"count",		"20"},
	{"order",	"1"},
};

//09. 
sUrlparams	Urlparam_FeedRead[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"feed.read"},
	
	{"fid",		""},//新鲜事的ID。如果希望标记全部新鲜事已读，传递值为"all"
	{"type",	""},
};

//10. 
sUrlparams	Urlparam_FeedGetFangle[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"feed.getFangle"},
	
	{"types",	"110,120,100,111,121,101,130,140"},
	{"page",		"1"},
	{"count",		"50"},
	{"read",	"0"},	
};

//11. 
sUrlparams	Urlparam_FeedReadFangle[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"feed.readFangle"},
	
	{"source_id",		""},	
	{"type",	""},

	//{"",		""},//Notify_id 只把当前标记已读。Typ	//文档不全
};

//12. 
sUrlparams	Urlparam_FeedGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"feed.get"},
	
	{"uid",		""},	
	{"type",		"10,11,20,21,22,23,30,31,32,33,34,35,36,40,41,51,52,55"},	//暂不支持:50&53, Video相关, 54 链接共享,34&35头像更新
	{"page",		"1"},
	{"count",		"10"},
};

//13. 
sUrlparams  Urlparam_FriendsGetFriends[] = 
{
	{"api_key",	RenRen_API_Key},
	{"call_id",	"1000"},
	{"sig",		""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"friends.getFriends"},
	
	{"page",		"1"},
	{"count",		"200"},
};

//14.
sUrlparams	Urlparam_FriendsAreFriends[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"friends.areFriends"},
	
	{"uids1",		""},
	{"uids2",		""},
};

//15. 
sUrlparams	Urlparam_FriendsSearch[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"friends.search"},
	
	{"name",		""},
	{"condition",	""},
	
	{"page",		"1"},
	{"count",		"20"},
};

//16.
sUrlparams	Urlparam_GuestbookGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"guestbook.get"},
	
	{"uid",		""},	
	{"page",		"1"},
	{"count",		"20"},	//Default-10 ; Max-20
};

//17.
sUrlparams	Urlparam_GuestbookPost[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"guestbook.post"},
	
	{"uid", 	""},
	{"content", ""},	
	{"rid",		""},
	{"type",		""},
	{"reply_gossip_id",		""},
};

//18. 
sUrlparams	Urlparam_MessageGets[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"message.gets"},
	
	{"box", 	""},	
	{"page",		"1"},
	{"count",		"50"},
};

//19.
sUrlparams	Urlparam_MessageGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"message.get"},
	
	{"id", 	""},	
	{"box",		"1"},
};

//20.
sUrlparams	Urlparam_MessageSent[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"message.send"},
	
	{"uid", 	""},
	{"title", ""},	
	{"content", ""},	
	{"rid",	""},	
	{"box", 	"1"},
};

//
sUrlparams	Urlparam_MessageMark[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},

	{"method",	"message.mark"},
	
	{"ids", 	""},		//一组站内信的id，以“,”分割
	{"type",	"1"},	//表示未读操作，1表示已读操作
};

//21.
sUrlparams	Urlparam_PagesIsFan[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"pages.isFan"},
	
	{"page_id", 	""},
	{"uid", 	""},
};

//22.
sUrlparams	Urlparam_PagesBecomeFan[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"Pages.becomeFan"},
	
	{"page_id", 	""},
};

//23.
sUrlparams	Urlparam_PagesGetList[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"pages.getList"},
	
	{"uid", 	""},
	{"page", 	"1"},
	{"count", 	"50"},
	{"category", 	""},
};

//24.
sUrlparams	Urlparam_PagesGetInfo[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"pages.getInfo"},
	
	{"page_id", 	""},
	{"fields", 	"status,desc,base_info,albums_count,blogs_count,fans_count,detail_info,contact_info"},
};

//25.
sUrlparams	Urlparam_PhotosGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"photos.get"},
	
	{"uid", 	""},
	{"aid", 	""},
	{"pids",	""},
	{"page", 	"1"},
	{"count", "240"},
	{"password", 	""},
};

//26.
sUrlparams	Urlparam_PhotosGetAlbums[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"photos.getAlbums"},
	
	{"uid", 	""},
	{"aids", 	""},
};

//27.
sUrlparams	Urlparam_PhotosGetComments[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"photos.getComments"},
	
	{"uid", 	""},
	{"page",	"1"},
	{"count",	"20"},
	{"pid", 	""},
	{"aid",	""},
	{"order", ""},
};

//28.
sUrlparams	Urlparam_PhotosUpload[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"photos.upload"},
	
	{"aid", ""},
	{"caption", ""},
	{"upload", 	""},
	//{"base_data",	""},
};

//29.
sUrlparams	Urlparam_PhotosAddComment[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"photos.addComment"},
	
	{"pid", ""},
	{"aid", ""},
	{"content",	""},
	{"uid",	""},
	{"rid", ""},
	{"type", ""},
};

//30.
sUrlparams	Urlparam_ProfileGetInfo[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"profile.getInfo"},
	
	{"uid", 	""},
	{"fields",	"status,visitors_count,blogs_count,albums_count,friends_count,guestbook_count"},
};

//30-2. 
sUrlparams	Urlparam_ProfileGetInfoFriend[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"profile.getInfo"},
	
	{"uid", 	""},
	{"fields",	"status,visitors_count,blogs_count,albums_count,friends_count,guestbook_count"},
};

//31.
sUrlparams	Urlparam_RequestsDealFriendRequest[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"requests.dealFriendRequest"},
	
	{"uid", 	""},
	{"type",	"0"},
};

//32.
sUrlparams	Urlparam_RequestsGetFriendRequests[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"requests.getGuestRequests"},
	
	{"page",	"1"},
	{"count",	"20"},
};

//33.
sUrlparams	Urlparam_RequestsSendFriendRequest[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"requests.sendFriendRequest"},
	
	{"uid",	""},
	{"reason",	""},
};

//34.
sUrlparams	Urlparam_SharePublish[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"share.publish"},
	
	{"type", ""},
	{"share_data",	""},
	{"id", ""},
	{"uid",	""},
};

//35.
sUrlparams	Urlparam_StatusGets[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.gets"},

	{"uid",	""},
	{"page",	"1"},
	{"count",	"20"},
};

//36.
sUrlparams	Urlparam_StatusSet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.set"},

	{"status", ""},
	{"forward_id",	""},
	{"forward_owner",	""},
	{"place_data",	""},
};

//37.
sUrlparams	Urlparam_StatusAddComment[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.addComment"},

	{"owner_id", ""},
	{"content",	""},
	{"status_id",	""},
	{"rid",	""},
};

//38.
sUrlparams	Urlparam_StatusGet[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.get"},

	{"status_id",	""},
	{"owner_id", ""},
};

//39.
sUrlparams	Urlparam_StatusGetComment[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.getComment"},

	{"status_id",	""},
	{"owner_id", ""},
	{"page",	"1"},
	{"count",	"20"},
	{"order", "0"},
};

//40.
sUrlparams	Urlparam_StatusGetEmoticons[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"status.getEmoticons"},

	{"type",	""},
};

//41.
sUrlparams	Urlparam_UsersGetInfo[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"users.getInfo"},
	
	{"uids", 	""},
	{"fields",	"name,sex,star,birthday,tinyurl,headurl,mainurl,hometown_location,hs_history,university_history,work_history,contact_info"},
};

//42
sUrlparams	Urlparam_UsersGetVisitors[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"users.getVisitors"},

	{"page",	"1"},
	{"count",	"20"},
};

//43.
sUrlparams	Urlparam_UsersSetHeadPhoto[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"users.setHeadPhoto"},

	{"pid",	"1"},
};

//44.	
sUrlparams	Urlparam_UsersGetEvent[] = 
{
	{"api_key", RenRen_API_Key},
	{"call_id", "1000"},
	{"sig", 	""},
	{"v",		"1.0"},
	{"session_key",""},
	{"format",	"JSON"},
	
	{"method",	"users.getEvent"},

	{"fields", "friend_request_count,new_message_count,poke_count,fangle_count"},
	{"fangle_types", "110,120,100,111,121,101,130,140"},
};

//App的主结构体
sRRAppUrl   RRAppUrl[] =
{
	{RR_InitialLogin, 			TRUE,	WAITING_BOX_TRUE, "InitalLogin.jso", 		"http://login.api.renren.com/CL.do", Urlparam_InitalLogin, ARR_SIZE(Urlparam_InitalLogin)},	
	{RR_ReGetSessionID, 		TRUE,	WAITING_BOX_FALSE, "ReGetSessionID.jso", 		"http://login.api.renren.com/CL.do", Urlparam_InitalLogin, ARR_SIZE(Urlparam_InitalLogin)},	
	{RR_PhoneclientActiveClient, 	TRUE,	WAITING_BOX_FALSE, "PhoneclientActiveClient.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhoneclientActiveClient, ARR_SIZE(Urlparam_PhoneclientActiveClient)},	

	{RR_BatchRun, 			TRUE,	WAITING_BOX_TRUE, "BatchRun.jso", 		"http://api.renren.com/restserver.do", Urlparam_BatchRun, ARR_SIZE(Urlparam_BatchRun)},	
	{RR_BlogAddBlog, 			FALSE,	WAITING_BOX_TRUE, "BlogAddBlog.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogAddBlog, ARR_SIZE(Urlparam_BlogAddBlog)},	 
	{RR_BlogGets, 			TRUE,	WAITING_BOX_TRUE, "BlogGets.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogGets, ARR_SIZE(Urlparam_BlogGets)},	 
	{RR_BlogGetCategory, 			TRUE,	WAITING_BOX_TRUE, "BlogGetCategory.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogGetCategory, ARR_SIZE(Urlparam_BlogGetCategory)},
	{RR_BlogGet, 			TRUE,	WAITING_BOX_TRUE, "BlogGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogGet, ARR_SIZE(Urlparam_BlogGet)},	
	{RR_BlogAddComment, 			FALSE,	WAITING_BOX_TRUE, "BlogAddComment.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogAddComment, ARR_SIZE(Urlparam_BlogAddComment)},	
	{RR_BlogGetComments, 			TRUE,	WAITING_BOX_TRUE, "BlogGetComments.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogGetComments, ARR_SIZE(Urlparam_BlogGetComments)},	
	{RR_BlogGetCommentsMore, 		FALSE,	WAITING_BOX_TRUE, "BlogGetCommentsMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_BlogGetComments, ARR_SIZE(Urlparam_BlogGetComments)},	

	{RR_FeedRead, 			FALSE,	WAITING_BOX_TRUE, "FeedRead.jso", 		"http://api.renren.com/restserver.do", Urlparam_FeedRead, ARR_SIZE(Urlparam_FeedRead)},	 
	{RR_FeedGetFangle, 			TRUE,	WAITING_BOX_TRUE, "FeedGetFangle.jso", 		"http://api.renren.com/restserver.do", Urlparam_FeedGetFangle, ARR_SIZE(Urlparam_FeedGetFangle)},	
	{RR_FeedReadFangle, 			TRUE,	WAITING_BOX_FALSE, "FeedReadFangle.jso", 		"http://api.renren.com/restserver.do", Urlparam_FeedReadFangle, ARR_SIZE(Urlparam_FeedReadFangle)},	
	{RR_FeedGet, 			TRUE,	WAITING_BOX_TRUE, "FeedGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_FeedGet, ARR_SIZE(Urlparam_FeedGet)},	
	{RR_FeedGetMore, 		FALSE,	WAITING_BOX_TRUE, "FeedGetMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_FeedGet, ARR_SIZE(Urlparam_FeedGet)},

	{RR_FriendsGetFriends, 	TRUE,	WAITING_BOX_TRUE, "FriendsGetFriends.jso", 	"http://api.renren.com/restserver.do", Urlparam_FriendsGetFriends, ARR_SIZE(Urlparam_FriendsGetFriends)},	
	{RR_FriendsAreFriends, 			FALSE,	WAITING_BOX_FALSE, "FriendsAreFriends.jso", 		"http://api.renren.com/restserver.do", Urlparam_FriendsAreFriends, ARR_SIZE(Urlparam_FriendsAreFriends)},	
	{RR_FriendsSearch, 			TRUE,	WAITING_BOX_TRUE, "FriendsSearch.jso", 		"http://api.renren.com/restserver.do", Urlparam_FriendsSearch, ARR_SIZE(Urlparam_FriendsSearch)},	
	{RR_FriendsSearchMore, 			FALSE,	WAITING_BOX_TRUE, "FriendsSearchMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_FriendsSearch, ARR_SIZE(Urlparam_FriendsSearch)},	

	{RR_GuestbookGet, 			TRUE,	WAITING_BOX_TRUE, "GuestbookGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_GuestbookGet, ARR_SIZE(Urlparam_GuestbookGet)},	
	{RR_GuestbookPost, 			FALSE,	WAITING_BOX_TRUE, "GuestbookPost.jso", 		"http://api.renren.com/restserver.do", Urlparam_GuestbookPost, ARR_SIZE(Urlparam_GuestbookPost)},	
	
	{RR_MessageGets, 			TRUE,	WAITING_BOX_TRUE, "MessageGets.jso", 		"http://api.renren.com/restserver.do", Urlparam_MessageGets, ARR_SIZE(Urlparam_MessageGets)},	
	{RR_MessageGet, 			TRUE,	WAITING_BOX_TRUE, "MessageGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_MessageGet, ARR_SIZE(Urlparam_MessageGet)},	
	{RR_MessageSent, 			FALSE,	WAITING_BOX_TRUE, "MessageSent.jso", 		"http://api.renren.com/restserver.do", Urlparam_MessageSent, ARR_SIZE(Urlparam_MessageSent)},	
	{RR_MessageMark, 			TRUE,	WAITING_BOX_FALSE, "MessageMark.jso", 		"http://api.renren.com/restserver.do", Urlparam_MessageMark, ARR_SIZE(Urlparam_MessageMark)},	
	
	{RR_PagesIsFan, 			TRUE,	WAITING_BOX_TRUE, "PagesIsFan.jso", 		"http://api.renren.com/restserver.do", Urlparam_PagesIsFan, ARR_SIZE(Urlparam_PagesIsFan)},	
	{RR_PagesBecomeFan, 			TRUE,	WAITING_BOX_TRUE, "PagesBecomeFan.jso", 		"http://api.renren.com/restserver.do", Urlparam_PagesBecomeFan, ARR_SIZE(Urlparam_PagesBecomeFan)},	
	{RR_PagesGetList, 			TRUE,	WAITING_BOX_TRUE, "PagesGetList.jso", 		"http://api.renren.com/restserver.do", Urlparam_PagesGetList, ARR_SIZE(Urlparam_PagesGetList)},	
	{RR_PagesGetInfo, 			TRUE,	WAITING_BOX_TRUE, "PagesGetInfo.jso", 		"http://api.renren.com/restserver.do", Urlparam_PagesGetInfo, ARR_SIZE(Urlparam_PagesGetInfo)},	
	
	{RR_PhotosGet, 			TRUE,	WAITING_BOX_TRUE, "PhotosGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosGet, ARR_SIZE(Urlparam_PhotosGet)},	
	{RR_PhotosGetMore, 			FALSE,	WAITING_BOX_TRUE, "PhotosGetMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosGet, ARR_SIZE(Urlparam_PhotosGet)},	
	{RR_PhotosGetAlbums, 			TRUE,	WAITING_BOX_TRUE, "PhotosGetAlbums.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosGetAlbums, ARR_SIZE(Urlparam_PhotosGetAlbums)},	
	{RR_PhotosGetComments, 			TRUE,	WAITING_BOX_TRUE, "PhotosGetComments.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosGetComments, ARR_SIZE(Urlparam_PhotosGetComments)},	
	{RR_PhotosGetCommentsMore, 		FALSE,	WAITING_BOX_TRUE, "PhotosGetCommentsMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosGetComments, ARR_SIZE(Urlparam_PhotosGetComments)},	
	{RR_PhotosUpload, 			FALSE,	WAITING_BOX_TRUE, "PhotosUpload.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosUpload, ARR_SIZE(Urlparam_PhotosUpload)},	
	{RR_PhotosAddComment, 			FALSE,	WAITING_BOX_TRUE, "PhotosAddComment.jso", 		"http://api.renren.com/restserver.do", Urlparam_PhotosAddComment, ARR_SIZE(Urlparam_PhotosAddComment)},	
	
	{RR_ProfileGetInfo, 			TRUE,	WAITING_BOX_TRUE, "ProfileGetInfo.jso", 		"http://api.renren.com/restserver.do", Urlparam_ProfileGetInfo, ARR_SIZE(Urlparam_ProfileGetInfo)},	
	
	{RR_RequestsDealFriendRequest, 			FALSE,	WAITING_BOX_TRUE, "RequestsDealFriendRequest.jso", 		"http://api.renren.com/restserver.do", Urlparam_RequestsDealFriendRequest, ARR_SIZE(Urlparam_RequestsDealFriendRequest)},	
	{RR_RequestsGetFriendRequests, 			TRUE,	WAITING_BOX_TRUE, "RequestsGetFriendRequests.jso", 		"http://api.renren.com/restserver.do", Urlparam_RequestsGetFriendRequests, ARR_SIZE(Urlparam_RequestsGetFriendRequests)},	
	{RR_RequestsSendFriendRequest, 			FALSE,	WAITING_BOX_TRUE, "RequestsSendFriendRequest.jso", 		"http://api.renren.com/restserver.do", Urlparam_RequestsSendFriendRequest, ARR_SIZE(Urlparam_RequestsSendFriendRequest)},	
	
	{RR_SharePublish, 			FALSE,	WAITING_BOX_TRUE, "SharePublish.jso", 		"http://api.renren.com/restserver.do", Urlparam_SharePublish, ARR_SIZE(Urlparam_SharePublish)},	
	
	{RR_StatusGets, 			TRUE,	WAITING_BOX_TRUE, "StatusGets.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusGets, ARR_SIZE(Urlparam_StatusGets)},	
	{RR_StatusSet, 			FALSE,	WAITING_BOX_TRUE, "StatusSet.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusSet, ARR_SIZE(Urlparam_StatusSet)},	
	{RR_StatusAddComment, 			FALSE,	WAITING_BOX_TRUE, "StatusAddComment.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusAddComment, ARR_SIZE(Urlparam_StatusAddComment)},	
	{RR_StatusGet, 			FALSE,	WAITING_BOX_TRUE, "StatusGet.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusGet, ARR_SIZE(Urlparam_StatusGet)},	
	{RR_StatusGetComment, 			TRUE,	WAITING_BOX_TRUE, "StatusGetComment.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusGetComment, ARR_SIZE(Urlparam_StatusGetComment)},	
	{RR_StatusGetCommentMore, 		FALSE,	WAITING_BOX_TRUE, "StatusGetCommentMore.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusGetComment, ARR_SIZE(Urlparam_StatusGetComment)},	
	{RR_StatusGetEmoticons, 			TRUE,	WAITING_BOX_TRUE, "StatusGetEmoticons.jso", 		"http://api.renren.com/restserver.do", Urlparam_StatusGetEmoticons, ARR_SIZE(Urlparam_StatusGetEmoticons)},	
	
	{RR_UsersGetInfo, 			TRUE,	WAITING_BOX_TRUE, "UsersGetInfo.jso", 		"http://api.renren.com/restserver.do", Urlparam_UsersGetInfo, ARR_SIZE(Urlparam_UsersGetInfo)},	
	{RR_UsersGetInfoPhoto, 		FALSE,	WAITING_BOX_FALSE, "UsersGetInfoPhoto.jso", 		"http://api.renren.com/restserver.do", Urlparam_UsersGetInfo, ARR_SIZE(Urlparam_UsersGetInfo)},	
	
	{RR_UsersGetVisitors, 			TRUE,	WAITING_BOX_TRUE, "UsersGetVisitors.jso", 		"http://api.renren.com/restserver.do", Urlparam_UsersGetVisitors, ARR_SIZE(Urlparam_UsersGetVisitors)},	
	{RR_UsersSetHeadPhoto, 			FALSE,	WAITING_BOX_TRUE, "UsersSetHeadPhoto.jso", 		"http://api.renren.com/restserver.do", Urlparam_UsersSetHeadPhoto, ARR_SIZE(Urlparam_UsersSetHeadPhoto)},	
	{RR_UsersGetEvent, 			TRUE,	WAITING_BOX_FALSE, "UsersGetEvent.jso", 		"http://api.renren.com/restserver.do", Urlparam_UsersGetEvent, ARR_SIZE(Urlparam_UsersGetEvent)},	
};


//需要保存的全局信息,需要保存到手机
tUserInfoNeedSave	 RenRenUserInfoNeedSave = 
{
	FALSE,	//bAutoLogin
	TRUE,	//bSavePwd
	FALSE,	//bShowPwd
	"",		//user
	"",		//password
};

tUserInfo	RenRenUserInfo = 
{
	"",		//session_key
	"",		//ticket
	0,		//uid
	"",		//szuid
	"", 		//satus, 默认为空	,进profile后进行，保存最新内容
	0,		//nCountFriendRequest;	//好友请求数
	0,		//nCountNewMessage;	//新消息数
	0,		//nCountPoke;	//打招呼数
	0,		//nCountFangle;	//回复数	
};

Int32	gHwndId_RenRenApp[RR_AccessTypeEnd] = {0};//保存各个窗口的ID，初始为0

Boolean	bIsPage = FALSE;

/**
 * 获得App对应的POST表单
 *
 * \param accessType
 * \param aFormData
 *
 * \return 
 */
int Url_GenerateFormData(RR_AccessType accessType, FormDataType* aFormData)
{	
	int result = eFailed;

	sRRAppUrl AppUrl;

	int arrSize = 0; //KX_Url_Params_Num[accessType];
	int i = 0;
	int nUrlLen = 0;

	//-------------------------------------------------------------
	if(aFormData == NULL)
		return eFailed;
	
	if(accessType >= RR_AccessTypeEnd || accessType <= RR_AccessTypeStart)
	{
		return eFailed;
	}

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			AppUrl = RRAppUrl[i];
			result = eSucceed;
		}
	}
	//未发现对应的结构体
	if(result == eFailed)
		return eFailed;

	aFormData->nParamDataCount = 0;
	arrSize = AppUrl.nParamsNum;
	for(i=0;i<arrSize;i++)
	{
		if( AppUrl.Params[i].paramName != NULL && AppUrl.Params[i].paramValue != NULL )
		{
			aFormData->pParamData[i].pszParamName = AppUrl.Params[i].paramName;
			aFormData->pParamData[i].pszParamContent = AppUrl.Params[i].paramValue;
			aFormData->pParamData[i].nParamType = CURLFORM_COPYCONTENTS;
			//上传图片的文件名参数单独处理
			if(accessType == RR_PhotosUpload)
			{
				if(strcmp(AppUrl.Params[i].paramName, "upload") == 0)
					aFormData->pParamData[i].nParamType = CURLFORM_FILE;
			}
			aFormData->nParamDataCount++;

		}
	}

	if(aFormData->nParamDataCount == 0)
		result = eFailed;
	else
		result = eSucceed;
	
	return result;
}

/**
 * 获得API对应的sig
 *
 * \param accessType
 * \param sSig
 *
 * \return 
 */
int Url_GenerateSig(RR_AccessType accessType, char** sSig)
{	
	int result = eFailed;
	sRRAppUrl AppUrl;

	int arrSize = 0; //KX_Url_Params_Num[accessType];
	int i = 0;
	int j = 0;
	int nUrlLen = 0;

	//-------------------------------------------------------------
	
	if(accessType >= RR_AccessTypeEnd || accessType <= RR_AccessTypeStart)
	{
		return eFailed;
	}

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			AppUrl = RRAppUrl[i];
			result = eSucceed;
		}
	}
	//未发现对应的结构体
	if(result == eFailed)
		return eFailed;

	arrSize = AppUrl.nParamsNum;
	if(arrSize<=0)
		return eFailed;
	
	//参数按照字典序排序
	for(i=0;i<arrSize;i++)
	{
		for(j = 0; j<arrSize-1; j++)
		{
			if( AppUrl.Params[j].paramName != NULL && AppUrl.Params[j+1].paramName != NULL )
			{
				if(strcmp(AppUrl.Params[j].paramName, AppUrl.Params[j+1].paramName) > 0)
				{
					sUrlparams	tmpParam = {0};
					STRCPY_Ex(tmpParam.paramName, AppUrl.Params[j].paramName);
					STRCPY_Ex(tmpParam.paramValue, AppUrl.Params[j].paramValue);

					STRCPY_Ex(AppUrl.Params[j].paramName, AppUrl.Params[j+1].paramName);
					STRCPY_Ex(AppUrl.Params[j].paramValue, AppUrl.Params[j+1].paramValue);

					STRCPY_Ex(AppUrl.Params[j+1].paramName, tmpParam.paramName);
					STRCPY_Ex(AppUrl.Params[j+1].paramValue, tmpParam.paramValue);
				}
			}
		}
	}

	//::TODO,  需要足够长度
	char sparam[10240] = {0};	
	for(i=0;i<arrSize;i++)
	{
		if( AppUrl.Params[i].paramName != NULL && AppUrl.Params[i].paramValue != NULL )
		{
			//"sig"不参与, PhotoUpload的"upload"也不参与计算
			if( (strcmp(AppUrl.Params[i].paramName, "sig") != 0) && (strcmp(AppUrl.Params[i].paramName, "upload") != 0) )
			{
				strcat(sparam, AppUrl.Params[i].paramName);
				strcat(sparam, "=");
				strcat(sparam, AppUrl.Params[i].paramValue);
			}
		}
	}

	if(strlen(sparam) == 0)
		return eFailed;
	
	strcat(sparam, RenRen_Secret_Key);

	*sSig = MD5Create((char*)sparam);
			
	return result;
}

/**
 * 设置URL的参数
 *
 * \param accessType
 * \param ParamName
 * \param ParamValue
 *
 * \return 
 */
int Set_Url_Params(RR_AccessType accessType, char* ParamName, char* ParamValue)
{
	sRRAppUrl AppUrl;

	int i = 0;
	int arrSize = 0;

	int result = eFailed;
	//---------------------------------

	if(ParamName == NULL || ParamValue == NULL)
	{
		return eFailed;		
	}

	if(accessType >= RR_AccessTypeEnd || accessType <= RR_AccessTypeStart)
	{
		return eFailed;
	}

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			AppUrl = RRAppUrl[i];
			result = eSucceed;
		}
	}
	//未发现对应的结构体
	if(result == eFailed)
		return eFailed;

	arrSize = AppUrl.nParamsNum;

	for(i=0; i<arrSize; i++)
	{	if( AppUrl.Params[i].paramName != NULL && AppUrl.Params[i].paramValue != NULL )
	{		
		if(strcmp(AppUrl.Params[i].paramName, ParamName)==0)
		{
			memset(AppUrl.Params[i].paramValue, 0, sizeof(AppUrl.Params[i].paramValue));
			strcpy(AppUrl.Params[i].paramValue, ParamValue);				
			result= eSucceed;
			break;
		}
	}
	else
	{
		result= eFailed;
		break;		
	}
	}

	return result;
}

/**
 * 获取URL的参数,返回全局字符串指针,不需要释放
 *
 * \param accessType
 * \param ParamName
 *
 * \return: 错误返回NULL 
 */
char* Get_Url_Params(RR_AccessType accessType, char* ParamName)
{
	sRRAppUrl AppUrl;

	int i = 0;
	int arrSize = 0;

	int result = eFailed;
	//---------------------------------

	if(ParamName == NULL)
	{
		return NULL;		
	}
	
	if(accessType >= RR_AccessTypeEnd || accessType <= RR_AccessTypeStart)
	{
		return NULL;
	}

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			AppUrl = RRAppUrl[i];
			result = eSucceed;
		}
	}
	//未发现对应的结构体
	if(result == eFailed)
		return NULL;

	arrSize = AppUrl.nParamsNum;

	for(i=0; i<arrSize; i++)
	{	if( AppUrl.Params[i].paramName != NULL && AppUrl.Params[i].paramValue != NULL )
		{		
			if(strcmp(AppUrl.Params[i].paramName, ParamName)==0)
			{
				return AppUrl.Params[i].paramValue;
			}
		}
		else
		{
			break;		
		}
	}

	return NULL;
}


/**
 * 获得保存数据的文件名
 *
 * \param accessType
 *
 * \return 
 * 若无对应的则返回NULL
 * 返回的是全局变量,调用者不需要释放
 */
char* RR_GetFileName(RR_AccessType accessType)
{
	int i = 0;

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			if(strlen(RRAppUrl[i].FilePath) ==0)
				return NULL;
			else
				return RRAppUrl[i].FilePath;
		}
	}
	//未发现对应的结构体
	return NULL;
}

/**
 * 获取json文件全路径,需要调用者释放
 *
 * \param accessType
 *
 * \return 
 */
TUChar* RR_GetFileNameFullPath(RR_AccessType accessType)
{
	char* FilePath = NULL;
	TUChar pszTemp[EOS_FILE_MAX_FNAME];
	TUChar* pszFilePath = NULL;

	FilePath = RR_GetFileName(accessType);
	if(FilePath == NULL)
		return NULL;
	
	pszFilePath = (TUChar*)malloc(EOS_FILE_MAX_PATH *sizeof(TUChar));
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);	

	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);	
	
	TUString::StrGBToUnicode(pszTemp,(Char*)FilePath);
	TUString::StrCat(pszFilePath,pszTemp);

	
	return pszFilePath;
}

/**
 * 获取json备份文件全路径, 主要用来json拼接
 *
 * \param accessType
 *
 * \return 
 */
TUChar* RR_GetBakFileNameFullPath(RR_AccessType accessType)
{
	int nLen = 0;
	char* FilePath = NULL;
	char FilePathBak[EOS_FILE_MAX_FNAME];
	TUChar pszTemp[EOS_FILE_MAX_FNAME];
	TUChar* pszFilePath = NULL;

	FilePath = RR_GetFileName(accessType);
	if(FilePath == NULL)
		return NULL;

	strcpy(FilePathBak, FilePath);
	nLen = strlen(FilePathBak);
	strcpy(&FilePathBak[nLen - 3], "bak");
				
	pszFilePath = (TUChar*)malloc(EOS_FILE_MAX_PATH *sizeof(TUChar));
	
	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);	
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);
	
	TUString::StrGBToUnicode(pszTemp,(Char*)FilePathBak);
	TUString::StrCat(pszFilePath,pszTemp);

	return pszFilePath;
}

/**
 * 获取api对应的url
 *
 * \param accessType
 *
 * \return 
 */
char* RR_GetUrl(RR_AccessType accessType)
{
	int i = 0;

	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			if(strlen(RRAppUrl[i].Address) ==0)
				return NULL;
			else
				return RRAppUrl[i].Address;
		}
	}
	//未发现对应的结构体
	return NULL;
}

/**
 * 判断api是否需要统一处理
 *
 * \param accessType
 *
 * \return 
 */
Boolean RR_GetApiHandle(RR_AccessType accessType)
{
	Boolean bHandle = FALSE;
	for(int i=0; i<ARR_SIZE(RRAppUrl); i++)
	{
		if(accessType == RRAppUrl[i].nType)
		{
			bHandle = RRAppUrl[i].bHandle;
			break;
		}
	}

	return bHandle;
}

/**
 * 是否需要等待对话框
 *
 * \param accessType
 *
 * \return 
 */
Boolean RR_GetApiNeedWaitingBox(RR_AccessType accessType)
{
	Boolean bNeedWaitingBox = FALSE;
	for(int i=0; i<ARR_SIZE(RRAppUrl); i++)
	{
		if(accessType == RRAppUrl[i].nType)
		{
			bNeedWaitingBox = RRAppUrl[i].bNeedWaitingBox;
			break;
		}
	}

	return bNeedWaitingBox;
}

/**
 * Url下载后保存的文件名(带路径)
 * 由调用者释放
 * \param pURL
 *
 * \return 
 */
TUChar* FileNameFullPathFromUrl(char* pURL)
{
	if(pURL==NULL && *pURL == 0)
		return NULL;

	char* FilePath1 = NULL;	
	char FilePath2[EOS_FILE_MAX_FNAME];	
	char* FilePath3 = NULL;

	//获得'/'之后的字符串
	FilePath1 = strrchr(pURL, '/');
	if(FilePath1 == NULL)
		return NULL;
	*FilePath1 ++;

	//加强带验证码url的处理,如http://wap.kaixin001.com/123.jpg?234253245435
	strcpy(FilePath2, FilePath1);
	FilePath3 = strchr(FilePath2, '?');
	if(FilePath3 != NULL)
		*FilePath3 = '\0';

	TUChar* pszFilePath = NULL;
	TUChar pszTemp[EOS_FILE_MAX_FNAME];

	pszFilePath = (TUChar*)malloc(EOS_FILE_MAX_PATH*sizeof(TUChar));
	memset(pszFilePath, 0, EOS_FILE_MAX_PATH*sizeof(TUChar));
	
	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);
	
	TUString::StrGBToUnicode(pszTemp,(Char*)FilePath2);
	TUString::StrCat(pszFilePath,pszTemp);

	return pszFilePath;
}

/**
 * 读取url对应的图片
 *
 * \param pURL
 *
 * \return 
 */
TBitmap* LoadImgByUrl(char* pURL)
{
	TBitmap             *m_pRetBmp = NULL;

	if(pURL!=NULL && *pURL != 0)
	{
		TUChar* pszFilePath = NULL;

		pszFilePath = FileNameFullPathFromUrl(pURL);
		if(pszFilePath != NULL)
		{
			m_pRetBmp = LoadImgByPath(pszFilePath);
			free(pszFilePath);
		}
	}	

	return m_pRetBmp;
}

/**
 * Json解析
 * 返回解析后的数据结构, 使用者需要类型强转
 * 正确时返回(0) ,注意: 调用者需要释放
 * 失败时返回错误errorCode , 调用者不需要释放
 * \param accessType
 * \param Response
 *
 * \return 
 */
int RenRenAPI_JsonParse(RR_AccessType accessType, void**Response)
{
	int error_code = 0;
	TUChar* pszFilePath = NULL;

	*Response = NULL;
	pszFilePath = RR_GetFileNameFullPath(accessType);
	if(pszFilePath == NULL)
		return -1;

	if (EOS_IsFileExist(pszFilePath) == FALSE)
		return -1;
		
	Handle hFile = NULL;			// file handle
	Int32 nFileSize = 0;			// 文件大小
	Int32 nReadByte = 0;			// 具体读出的字节数
	void * szReadContent = NULL;

	hFile = EOS_Open(pszFilePath,PO_RDONLY|PO_BINARY,PS_IREAD);
	free(pszFilePath);
	if (hFile < 0)
	{
		return -1;
	}	                    		

	// 获得文件大小
	nFileSize = EOS_GetFileSize(hFile);
	szReadContent = malloc(nFileSize+1024);
	memset(szReadContent, 0, nFileSize+1024);
	nReadByte= EOS_Read(hFile, szReadContent, nFileSize);	
	EOS_Close(hFile);

	//统一判断空串的情况
	if(*((char*)szReadContent) == 0)
	{
		free(szReadContent);
		return -1;
	}

	//统一提取error code
	cJSON *json;
	cJSON *tmp;
	json=cJSON_Parse((const char *)szReadContent);
	if(json == NULL)
	{
		free(szReadContent);
		return -1;
	}
	tmp = cJSON_GetObjectItem(json, "error_code");
	if(tmp)
	{
		error_code = tmp->valueint;
	}
	cJSON_Delete(json);	
	if(error_code != 0)
	{
		free(szReadContent);
		return error_code;		
	}

	//Json格式中的Array纠正
	StrReplaceAtoB((char*)szReadContent, nFileSize+1024, ":{", ":[{");
	StrReplaceAtoB((char*)szReadContent, nFileSize+1024, "},\"", "}],\"");
	StrReplaceAtoB((char*)szReadContent, nFileSize+1024, "}}", "}]}");
	switch(accessType)
	{
		case RR_InitialLogin:
		case RR_ReGetSessionID:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_InitialLogin((char*)szReadContent);
			break;
		}
		case RR_ProfileGetInfo:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_Profile((char*)szReadContent);
			break;
		}
		case RR_FriendsGetFriends:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_FriendsGetFriends((char*)szReadContent);
			break;
		}
		case RR_FriendsAreFriends:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_FriendsAreFriends((char*)szReadContent);
			break;
		}
		case RR_FriendsSearch:
		case RR_FriendsSearchMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_FriendsSearch((char*)szReadContent);
			break;
		}

		case RR_UsersGetVisitors:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_UsersGetVisitors((char*)szReadContent);
			break;
		}
		case RR_UsersGetInfo:
		case RR_UsersGetInfoPhoto:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_UsersGetInfo((char*)szReadContent);
			break;
		}		
		case RR_MessageGet:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_MessageGet((char*)szReadContent);
			break;
		}	
		case RR_MessageGets:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_MessageGets((char*)szReadContent);
			break;
		}			
		case RR_UsersGetEvent:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_UsersGetEvent((char*)szReadContent);
			break;
		}
		case RR_BlogGets:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_BlogGets((char*)szReadContent);
			break;
		}
		case RR_BlogGet:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_BlogGet((char*)szReadContent);
			break;
		}
		case RR_BlogGetComments:
		case RR_BlogGetCommentsMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_BlogGetComments((char*)szReadContent);
			break;
		}
		case RR_StatusGet:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_StatusGet((char*)szReadContent);
			break;
		}		
		case RR_PagesGetList:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_PagesGetList((char*)szReadContent);
			break;
		}		
		case RR_PagesGetInfo:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_PagesGetInfo((char*)szReadContent);
			break;
		}	
		case RR_PhotosGetAlbums:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_PhotosGetAlbums((char*)szReadContent);
			break;
		}	
		case RR_PhotosGetComments:
		case RR_PhotosGetCommentsMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_PhotosGetComments((char*)szReadContent);
			break;
		}
		case RR_FeedGet:
		case RR_FeedGetMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_FeedGet((char*)szReadContent);
			break;
		}
		case RR_GuestbookGet:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_GuestbookGet((char*)szReadContent);
			break;
		}
		case RR_GuestbookPost:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_GuestbookPost((char*)szReadContent);
			break;
		}
		case RR_PhotosGet:
		case RR_PhotosGetMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_PhotosGet((char*)szReadContent);
			break;
		}
		case RR_RequestsGetFriendRequests:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_RequestsGetFriendRequests((char*)szReadContent);
			break;
		}	
		case RR_RequestsDealFriendRequest:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_dealFriendRequest((char*)szReadContent);
			break;
		}
		case RR_FeedGetFangle:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_FeedGetFangle((char*)szReadContent);
			break;
		}
		case RR_StatusGetComment:
		case RR_StatusGetCommentMore:
		{
			int iRet = eFailed;
			*Response = RenRenAPI_JsonParse_StatusGetComment((char*)szReadContent);
			break;
		}

		
		default:
		{
			//未实现的
			break;			
		}
	}
	
	free(szReadContent);
	
//	if(error_code == -1)
//	{
//		delete (*Response);
//		*Response = NULL;
//	}
	return error_code;
}

/**
 * 判断Json解析是否正确
 * 正确为0; 其他为error code
 * \param accessType
 * \param errorinfo
 *
 * \return 
 */
int RenRenAPI_JsonParse_bSuccess(RR_AccessType accessType, TUChar * errorinfo)
{
	int error_code = 0;
	TUChar* pszFilePath = NULL;

	pszFilePath = RR_GetFileNameFullPath(accessType);
	if(pszFilePath == NULL)
		return -1;

	if (EOS_IsFileExist(pszFilePath) == FALSE)
	{
		free(pszFilePath);
		return -1;
	}	
	Handle hFile = NULL;			// file handle
	Int32 nFileSize = 0;			// 文件大小
	Int32 nReadByte = 0;			// 具体读出的字节数
	void * szReadContent = NULL;

	hFile = EOS_Open(pszFilePath,PO_RDONLY|PO_BINARY,PS_IREAD);
	free(pszFilePath);
	if (hFile < 0)
	{
		return -1;
	}	                    		

	// 获得文件大小
	nFileSize = EOS_GetFileSize(hFile);
	szReadContent = malloc(nFileSize+1);
	memset(szReadContent, 0, nFileSize+1);
	nReadByte= EOS_Read(hFile, szReadContent, nFileSize);	
	EOS_Close(hFile);

	//统一判断空串的情况
	if(*((char*)szReadContent) == 0)
	{
		free(szReadContent);
		return -1;
	}

	//统一判断"ret"
	cJSON *json;
	cJSON *tmp;
	json=cJSON_Parse((const char *)szReadContent);
	if(json == NULL)
	{
		free(szReadContent);
		return -1;
	}
	
	tmp = cJSON_GetObjectItem(json, "error_code");
	if(tmp)
	{
		error_code = tmp->valueint;
	}
	tmp = cJSON_GetObjectItem(json, "error_msg");
	if(tmp)
	{
		char errorstring[ERROR_INFO_LEN] = {0};
		STRCPY_Ex(errorstring, tmp->valuestring);
		TUString::StrUtf8ToStrUnicode (errorinfo, (const Char *)errorstring);
	}

	cJSON_Delete(json);

	free(szReadContent);

	return error_code;
}

/**
 * 读取配置
 *
 * \return 
 */
int RenRenAPI_GetConfig(void)
{
	int iRet = eFailed;
	TUChar pszFilePath[EOS_FILE_MAX_PATH];
	TUChar pszFolderPath[EOS_FILE_MAX_PATH];
	TUChar pszTemp[EOS_FILE_MAX_FNAME];

	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);
	//TUChar   pszAppID[128] = {0};
	//UInt32  pCmdType = 0;
	//SS_AppRequest_GetAppName(pszAppID, &pCmdType);
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_DATA, pszFilePath);
	
	TUString::StrCopy(pszFolderPath,pszFilePath);
	TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_CONFIG_FILE);
	TUString::StrCat(pszFilePath,pszTemp);
	if (EOS_IsFileExist(pszFilePath))
	{
		Handle hFile = NULL;			// file handle
		Int32 nFileSize = 0;			// 文件大小
		Int32 nReadByte = 0;			// 具体读出的字节数

		hFile = EOS_Open(pszFilePath,PO_RDONLY|PO_BINARY,PS_IREAD);
		if (hFile < 0)
		{
			return eFailed;
		}	                    		
		
		nFileSize = EOS_GetFileSize(hFile);

		//如果设置文件和结构体大小不一致,则直接返回
		if(nFileSize != sizeof(tUserInfoNeedSave))
		{
			iRet = eFailed;
		}
		else
		{
			nReadByte= EOS_Read(hFile, (void*)&RenRenUserInfoNeedSave, sizeof(tUserInfoNeedSave));
			iRet = eSucceed;
		}
		EOS_Close(hFile);
	}
	else if(EOS_IsFileExist(pszFolderPath) == FALSE )
	{
		//目录不存在的话先创建目录
		if(EOS_MakeDir(pszFolderPath))
		{
			iRet = eSucceed;
		}
	}

	return iRet;
}


/**
 * 保存配置
 *
 * \return 
 */
int RenRenAPI_SetConfig(void)
{
	int iRet = eFailed;
	TUChar pszFilePath[EOS_FILE_MAX_PATH];
	TUChar pszTemp[EOS_FILE_MAX_FNAME];

	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);
	//TUChar   pszAppID[128] = {0};
	//UInt32  pCmdType = 0;
	//SS_AppRequest_GetAppName(pszAppID, &pCmdType);
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_DATA, pszFilePath);
	
	TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_CONFIG_FILE);
	TUString::StrCat(pszFilePath,pszTemp);
	if (EOS_IsFileExist(pszFilePath))
		EOS_Delete(pszFilePath);

	Handle hFile = NULL;			// file handle
	Int32 nFileSize = 0;			// 文件大小
	Int32 nReadByte = 0;			// 具体读出的字节数

	hFile = EOS_Open(pszFilePath, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
	if (hFile < 0)
	{
		return eFailed;
	}	                    		
		
	EOS_Write(hFile, (void*)&RenRenUserInfoNeedSave, sizeof(tUserInfoNeedSave));
	EOS_Close(hFile);
	return eSucceed;
}


/**
 * 获得Html格式数据的文件名
 * 需要调用释放
 *
 * \param accessType
 *
 * \return 
 */
static char* RR_GetHtmlContentFileName(RR_AccessType accessType)
{
	int i = 0;
	char* pszDes = NULL;
	
	//查找对应的结构体
	for(i=0; i<ARR_SIZE(RRAppUrl);i++)
	{
		if(RRAppUrl[i].nType == accessType)
		{
			if(strlen(RRAppUrl[i].FilePath) ==0)
				return NULL;
			else
			{
				int nLen = 0;
				pszDes = StrDump(RRAppUrl[i].FilePath);
				nLen = strlen(pszDes);
				strcpy(&pszDes[nLen - 3], "htm");
				return pszDes;
			}
		}
	}
	//未发现对应的结构体
	return NULL;
}

/**
 * 生成Html文件
 *
 * \param accessType
 * \param pszContent
 *
 * \return 
 */
int RR_MakeHtmlFile(RR_AccessType accessType, char* pszContent)
{
	char* pszUrl = NULL;
	char* pszFilename = NULL;
	 TUChar pszFilePath[EOS_FILE_MAX_PATH];    // 文件路径 
	TUChar pszTemp[EOS_FILE_MAX_FNAME];

	if (!pszContent)
	{
	    return eFailed;
	}

	pszFilename = RR_GetHtmlContentFileName(accessType);
	if(pszFilename == NULL)
		return eFailed;
        MemSet(pszFilePath, 0, EOS_FILE_MAX_PATH * sizeof(TUChar));

	//TUString::StrCopy(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);
	
	TUString::StrGBToUnicode(pszTemp,(Char*)pszFilename);
	TUString::StrCat(pszFilePath,pszTemp);

	delete pszFilename;
	pszFilename = NULL;

	//删除旧的文件
	if (EOS_IsFileExist(pszFilePath))
		EOS_Delete(pszFilePath);

        Handle hHtmlFile = EOS_Open(pszFilePath, PO_CREAT | PO_BINARY | PO_WRONLY | PO_TRUNC, PS_IWRITE);
                if(hHtmlFile < 0)
                {
			return eFailed;
                }
                else
                {
                	//增加UTF-8的文件头
                	//EOS_Write(hHtmlFile, "\xEF\xBB\xBF", 3);
			EOS_Write(hHtmlFile, pszContent, strlen(pszContent));
			EOS_Close(hHtmlFile);
                }

    return eSucceed;
}

/**
 * 本地html文件的url格式
 * 调用释放
 *
 * \param accessType
 *
 * \return 
 */
TUChar* RR_GetHtmlFileUrl(RR_AccessType accessType)
{
	char* pszFilename = NULL;
	TUChar* pszFilePath = NULL;
	TUChar pszTemp[EOS_FILE_MAX_FNAME];
	char pszUrlHead[EOS_FILE_MAX_PATH];
	
	pszFilePath = new TUChar[EOS_FILE_MAX_PATH];
        MemSet(pszFilePath, 0, EOS_FILE_MAX_PATH * sizeof(TUChar));

	pszFilename = RR_GetHtmlContentFileName(accessType);
	if(pszFilename == NULL)
		return NULL;
	
	memset(pszUrlHead, 0, EOS_FILE_MAX_PATH*sizeof(char));
#ifdef _TRANZDA_VM_
        strcpy(pszUrlHead, "file://");
	//TUString::StrCopy(pszFilePath,(TUChar*)L"file://");
#else
        strcpy(pszUrlHead, "file:///");
#endif

	TUString::StrGBToUnicode(pszTemp,(Char*)pszUrlHead);
	TUString::StrCopy(pszFilePath,pszTemp);

	//TUString::StrCat(pszFilePath,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
	//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
	//TUString::StrCat(pszFilePath,pszTemp);
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszTemp);
	TUString::StrCat(pszFilePath,pszTemp);
	
	TUString::StrGBToUnicode(pszTemp,(Char*)pszFilename);
	TUString::StrCat(pszFilePath,pszTemp);

	delete pszFilename;
	pszFilename = NULL;	

	return pszFilePath;
}


/**
 * 上传文件不能有中文, 转成英文
 *
 * \param pszFileSrc
 * \param FileDesc
 *
 * \return 
 */
#define RENREN_PHOTO_UPLOAD_FILE		"FileUpload"
Int32 ConvertUploadFile(TUChar* pszFileSrc, char* FileDesc)
{
	Handle hFile = NULL;
       TUChar szExt[10];
	TUChar pszFileDesc[EOS_FILE_MAX_PATH];		

        EOS_ParsePath(NULL, NULL, szExt, (TUChar*)pszFileSrc);


        hFile = EOS_Open((TUChar*)pszFileSrc,  PO_RDONLY|PO_BINARY, PS_IREAD);
        if (hFile >= 0)
	{
		Int32 nlength = 0;
		void* pBuf = NULL;
		nlength = EOS_GetFileSize(hFile);

		pBuf = malloc(nlength);
		if (pBuf)
		{
			MemSet(pBuf, 0, nlength);
			if ( EOS_Read(hFile, pBuf, nlength) == nlength )
			{
				Handle hFileDes = NULL;

				TUChar pszTemp[EOS_FILE_MAX_FNAME];

				//TUString::StrCopy(pszFileDesc,EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_LOCAL_DATA));
				//TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_FOLDER);
				//TUString::StrCat(pszFileDesc,pszTemp);
				SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFileDesc);
	
				TUString::StrGBToUnicode(pszTemp,(Char*)RENREN_PHOTO_UPLOAD_FILE);
				TUString::StrCat(pszFileDesc,pszTemp);
				TUString::StrCat(pszFileDesc,szExt);

				//删除已有文件
				if (EOS_IsFileExist(pszFileDesc))
					EOS_Delete(pszFileDesc);	
				
				 hFileDes = EOS_Open(pszFileDesc, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
				 if(hFileDes > 0)
				 {
					EOS_Write(hFileDes, pBuf, nlength);
					EOS_Close(hFileDes);							 	
				 }
			}
			EOS_Close(hFile);

			free(pBuf);
		}       

		TUString::StrUnicodeToStrUtf8( (Char *)FileDesc, pszFileDesc);
	}

				
	return eSucceed;
}

/**
 * 拼接Json文件
 * 必须是array类型
 * \param accessType
 *
 * \return 
 */
Int32 RR_UpdateMoreFile(RR_AccessType accessType)
{
	TUChar* pszFilePathBak = NULL;
	TUChar* pszFilePath = NULL;
	pszFilePathBak = RR_GetBakFileNameFullPath(accessType);
	pszFilePath = RR_GetFileNameFullPath(accessType);

	//如果存在bak文件, 则进行拼接
	if (EOS_IsFileExist(pszFilePathBak))
	{
		Handle hFile = NULL;
		Handle hFileBak = NULL;

		hFile = EOS_Open(pszFilePath, PO_BINARY|PO_RDWR, PS_IWRITE);
		hFileBak = EOS_Open(pszFilePathBak, PO_BINARY|PO_RDWR, PS_IWRITE);

		if (hFile < 0 || hFileBak < 0)
		{
			free(pszFilePath);
			free(pszFilePathBak);		
			return eFailed;
		}	                    		

		Int32 nFileSize = 0;
		Int32 nReadByte = 0;
		void * szReadContent = NULL;
		nFileSize = EOS_GetFileSize(hFile);
		szReadContent = malloc(nFileSize+1);
		memset(szReadContent, 0, nFileSize+1);

		//去掉新文件起始的"["
		EOS_Seek(hFile, 1, SEEK_SET);
		nReadByte= EOS_Read(hFile, szReadContent, nFileSize);	

		//去掉旧文件结束的"]"
		EOS_Seek(hFileBak, -1, SEEK_END);
		//增加连接的","
		EOS_Write(hFileBak, ",", 1);
		
		EOS_Write(hFileBak, szReadContent, nReadByte);
		EOS_Close(hFile);
		EOS_Close(hFileBak);

		EOS_Delete(pszFilePath);
		EOS_Rename(pszFilePathBak, pszFilePath);
	}
	
	free(pszFilePath);
	free(pszFilePathBak);
	
	return eSucceed;
}


#define NAPPNEEDSIZE	2048	//kByte
	
/**
 * 检查Cache空间是否足够
 *
 * \return 
 */
Boolean CheckCacheIsEnough(void)
{
    Int32 nSectorsPerClusterP;
    Int32 nBytesPerSectorP;
    Int32 nNumberOfFreeClustersP;
    Int32 nTotalNumberOfClustersP;
    Int64 nStorageFreeSize;
    TUChar        szFilePath[EOS_FILE_MAX_FNAME] = {0};    // 文件路径
    Boolean       bResult;

    bResult = FALSE;
    nSectorsPerClusterP=0;
    nBytesPerSectorP=0;
    nNumberOfFreeClustersP=0;
    nTotalNumberOfClustersP=0;
    nStorageFreeSize = 0;

    // 获得缓存目录
    TUString::StrNCopy(szFilePath, EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_CACHE),EOS_FILE_MAX_FNAME-1);
    //获得目录存储空间情况
    EOS_GetDiskFreeSpace(szFilePath,&nSectorsPerClusterP,&nBytesPerSectorP,  &nNumberOfFreeClustersP,&nTotalNumberOfClustersP);
    //计算可用空间
    nStorageFreeSize = nNumberOfFreeClustersP*nSectorsPerClusterP;
    nStorageFreeSize *= nBytesPerSectorP;
    //单位换算成KB
    nStorageFreeSize /= 1024;

    //判断是否空闲目录是否达到应用需要。NAPPNEEDSIZE为商城运行需要的Cache大小(KB) 
    if (nStorageFreeSize > NAPPNEEDSIZE)
    {
        bResult = TRUE;
    }
    else
    {
        //否则首先删除自己Cache目录下的文件。
        TUChar        szSelfCachePath[EOS_FILE_MAX_PATH] = {0};
        DSTAT         objStat;
        Boolean       bFindResult;

        // 填写查找文件名
        bFindResult = FALSE;
        SS_GetApplicationPath(AppName_App_RenRen,SS_APP_PATH_TYPE_CACHE,szSelfCachePath);
	TUString::StrCat(szSelfCachePath, TUSTR_Re_AllFileFilter);
	
        //ustrncat_utf8(szSelfCachePath, "*.*", EOS_FILE_MAX_PATH-1-TUString::StrLen(szSelfCachePath));

        // 查找删除文件
        bFindResult = EOS_FindFirst(&objStat, szSelfCachePath);
        while (bFindResult)
        {
            // 判断不是.和..和cfg文件才删除
            if (TUString::StrCompare(objStat.lfname, TUSTR_Re_Dot) != 0
                && TUString::StrCompare(objStat.lfname, TUSTR_Re_DDot) != 0
                && TUString::StrCompare(objStat.lfname, TUSTR_Re_RenRenCfg) != 0
                )
            {
                MemSet(szSelfCachePath,0,EOS_FILE_MAX_PATH);
                SS_GetApplicationPath(AppName_App_RenRen,SS_APP_PATH_TYPE_CACHE,szSelfCachePath);
                TUString::StrNCat(szSelfCachePath, objStat.lfname, 
                    EOS_FILE_MAX_PATH-1-TUString::StrLen(szSelfCachePath));
                EOS_Delete(szSelfCachePath);
            }                               

            bFindResult = EOS_FindNext(&objStat);
        }
        EOS_FindDone(&objStat);

        //获得目录存储空间情况
        EOS_GetDiskFreeSpace(szFilePath,&nSectorsPerClusterP,&nBytesPerSectorP, &nNumberOfFreeClustersP,&nTotalNumberOfClustersP);
        //计算可用空间
        nStorageFreeSize = nNumberOfFreeClustersP*nSectorsPerClusterP;
        nStorageFreeSize *= nBytesPerSectorP;
        //单位换算成KB
        nStorageFreeSize /= 1024;

        //判断是否空闲目录是否达到应用需要。NAPPNEEDSIZE为商城运行需要的Cache大小(KB) 
        if (nStorageFreeSize > NAPPNEEDSIZE)
        {
            bResult = TRUE;
        }
        else
        {
            Int32 hHandle;
            hHandle=EOS_RequestMoreCacheRoom(AppName_App_RenRen, NAPPNEEDSIZE);
            if (EOS_RequestMoreCacheRoomCheck(hHandle)>=0)
            {
                bResult = TRUE;
            }
        }

    }
    return bResult;
}

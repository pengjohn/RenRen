/*!
*@FileName: RenRenAPICommon.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/

#ifndef  __RenRenAPICommon_H__
#define  __RenRenAPICommon_H__
#include "TG3.h"
#include "TResolution.h"
#include  "App_RenRenUnicodeScript_str.h"//include for show string

#include "TDownload.h"
#include "TFileDownloadMrg.h"
#include  "TImgData.h"
#include "cUnixTime.h"
#include  "CommonMacro.h"
#include  "StrComm.h"
#include  "gui_common.h"
#include  "OtherComm.h"


#include "RenRenForm_Test.h"

#include "RenRenAPI_InitialLogin.h"
#include "RenRenAPI_FriendsGetFriends.h"
#include "RenRenAPI_Profile.h"
#include "RenRenAPI_UsersGetVisitors.h"
#include "RenRenAPI_UsersGetInfo.h"
#include "RenRenAPI_MessageGet.h"
#include "RenRenAPI_UsersGetEvent.h"
#include "RenRenAPI_StatusGet.h"
#include "RenRenAPI_BlogGets.h"
#include "RenRenAPI_MessageGets.h"
#include "RenRenAPI_BlogGet.h"
#include "RenRenAPI_PagesGetList.h"
#include "RenRenAPI_PhotosGetAlbums.h"
#include "RenRenAPI_PagesGetInfo.h"
#include "RenRenAPI_FeedGet.h"
#include "RenRenAPI_GuestbookGet.h"
#include "RenRenAPI_PhotosGet.h"
#include "RenRenAPI_RequestsGetFriendRequests.h"
#include "RenRenAPI_dealFriendRequest.h"
#include "RenRenAPI_GuestbookPost.h"
#include "RenRenAPI_BlogGetComments.h"
#include "RenRenAPI_PhotosGetComments.h"
#include "RenRenAPI_FeedGetFangle.h"
#include "RenRenAPI_StatusGetComment.h"
#include "RenRenAPI_FriendsSearch.h"

#include "RenRenForm_InitialLogin.h"
#include "RenRenForm_FriendsGetFriends.h"
#include "RenRenForm_UsersGetInfo.h"
#include "RenRenForm_WebPanel.h"
#include "RenRenForm_Edit.h"
#include "RenRenForm_WaitingBox.h"
#include "RenRenForm_MessageGet.h"
#include "RenRenForm_UsersGetEvent.h"
#include "RenRenForm_BlogGets.h"
#include "RenRenForm_MessageGets.h"
#include "RenRenForm_BlogGet.h"
#include "RenRenForm_PagesGetList.h"
#include "RenRenForm_PagesGetInfo.h"
#include "RenRenForm_PhotosGetAlbums.h"
#include "RenRenForm_FeedGet.h"
#include "RenRenForm_GuestbookGet.h"
#include "RenRenForm_PhotosGet.h"
#include "RenRenForm_RequestsGetFriendRequests.h"
#include "RenRenForm_PhotoDetail.h"
#include "RenRenForm_BlogGetComments.h"
#include "RenRenForm_PhotosGetComments.h"
#include "RenRenForm_FeedGetFangle.h"
#include "RenRenForm_About.h"
#include "RenRenForm_StatusGetComment.h"
#include "RenRenForm_MainMenu.h"
#include "RenRenForm_FriendsSearch.h"
#include "RenRenForm_PhotoUploadInfo.h"
#include "RenRenAPI_FriendsAreFriends.h"

extern IDownLoad *		g_pDownload;
extern TFileDownloadMgr*  g_pFileDownMgr;

extern int Set_Url_Params(RR_AccessType accessType, char* ParamName, char* ParamValue);
extern char* Get_Url_Params(RR_AccessType accessType, char* ParamName);
extern int Url_GenerateFormData(RR_AccessType accessType, FormDataType* aFormData);
extern int Url_GenerateSig(RR_AccessType accessType, char** sSig);
extern char* RR_GetFileName(RR_AccessType accessType);
extern char* RR_GetUrl(RR_AccessType accessType);
extern TUChar* RR_GetFileNameFullPath(RR_AccessType accessType);
extern TUChar* RR_GetBakFileNameFullPath(RR_AccessType accessType);
extern Boolean RR_GetApiHandle(RR_AccessType accessType);
extern Boolean RR_GetApiNeedWaitingBox(RR_AccessType accessType);
extern TUChar* FileNameFullPathFromUrl(char* pURL);
extern TBitmap* LoadImgByUrl(char* pURL);
extern int RenRenAPI_JsonParse(RR_AccessType accessType, void**Response);
extern int RenRenAPI_JsonParse_bSuccess(RR_AccessType accessType, TUChar * errorinfo);
extern int RR_MakeHtmlFile(RR_AccessType accessType, char* pszContent);
extern TUChar* RR_GetHtmlFileUrl(RR_AccessType accessType);
extern Int32 ConvertUploadFile(TUChar* pszFileSrc, char* FileDesc);
		
extern int RenRenAPICommon_Download(RR_AccessType  accessType, Int32 in_nHwndId = 0, Int32 in_nCtrlId = 0, Int32 in_nParam = 0, Boolean bOverWrite = TRUE);
extern int RenRenAPICommon_DownloadPhoto(char* pURL, Int32 in_nHwndId,Int32 in_nParam );
extern int RenRenAPI_Download(Int32 in_nAccessType, char* pURL, const TUChar* pszFilePath, Int32 in_nHwndId, Int32 in_nCtrlId, Int32 in_nParam, FormDataType aFormData);
extern int RenRenAPI_DownloadAbort(void);
extern int RenRenAPICommon_DownloadComplete(TAListElement*  pElement);
extern int RenRenAPICommon_DownloadCancelByHwndId(Int32 nHwndId);


extern Int32 PopBox_UploadPhoto(TApplication* pApp, Int32 nHwndId);
extern Int32 PopBox_SavePhoto(TApplication* pApp, TUChar* pszFilePath);

extern Int32 RR_UpdateMoreFile(RR_AccessType accessType);

extern Boolean CheckCacheIsEnough(void);

extern Int32 RenRen_Log_Init(void);
extern Int32 RenRen_Log(char* logTxt);	
#endif

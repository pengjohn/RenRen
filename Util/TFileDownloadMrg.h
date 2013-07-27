/*!
*@FileName: TFileDownloadMrg.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/

#ifndef __TFILEDOWNLOADMRG_H__
#define __TFILEDOWNLOADMRG_H__
#include "TG3.h"
#include "TDownload.h"
#include "CommonMacro.h"

#define RENREN_URLPATH_LENGTH  (1024)

typedef struct TAListElement TAListElement;
struct TAListElement
	{
    	Int32  accessType;
	Int32 nHwndId;
	Int32 nCtrlId;
	Int32 nParam;
	FormDataType aFormData;
	char pURL[RENREN_URLPATH_LENGTH];//TODO, 需要扩展大小
	TUChar pszFilePath[EOS_FILE_MAX_PATH]; 
     };

typedef struct TAList TAList;
struct TAList{
       TAListElement* pTAListElement; 
       TAList*        pNext;       
     };


class TFileDownloadMgr
{
public:

    TFileDownloadMgr();

    ~TFileDownloadMgr();

public:
	int Add(TAListElement* pElement);
	int Insert(TAListElement* pElement, int nPosition);	//TODO::
	int CancelByAccessType(Int32  accessType);			//TODO::	
	int CancelByHwndId(Int32 nHwndId);
	int CancelAll(void);							//TODO::
	int DownLoadStart(void);
	int DownLoadComplete(TAListElement*  pElement);
	int DownLoadStop(void);						//TODO::
private:
	TAList* pHeadNode;    //head node pointer
	TAList* pEndNode;     	//head End pointer
	int nSize;			//
	bool bDownLoad;		//是否正在下载
};

#endif //__TWEBIMAGESMGR_H__

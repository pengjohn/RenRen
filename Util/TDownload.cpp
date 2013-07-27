/*!
* 
* \file   TDownload.cpp
* \author Yusz
* \date	  2010.7.5
* \brief  Curl下载接口
* 
* \ref    CopyRight
*/

#include "TDownload.h"
#include "RenRenAPICommon.h"
//////////////////////////////////////////////////////////////////////////
//                        DoDownLoadParam结构体函数
//////////////////////////////////////////////////////////////////////////
IDownLoad::IDownLoad():gCurl(NULL)
, nAccessType(0)
, nHwndId(0)
, nCtrlId(0)
, bCheckMsg(TRUE)
,szFileName(NULL)
, szUrl(NULL)
, bUseProxy(FALSE)
, szProxyUrl(NULL)
, nProxyPort(80)
//, bCheckModifity(FALSE)
, nTimeout(10)
, bNoProNotify(TRUE)
{

}

IDownLoad::~IDownLoad()
{
	//if (szFileName)
	//{
	//    delete[] szFileName;
	//    szFileName = NULL;
	//}

	//if (szUrl)
	//{
	//    delete[] szUrl;
	//    szUrl =NULL;
	//}

	//if (szProxyUrl)
	//{
	//    delete[] szProxyUrl;
	//    szProxyUrl =NULL;
	//}
}

/** 释放DoDownLoadParam结构体中new出来的内存空间
* 
* \return void
*/
void IDownLoad::clearMem()
{
	if (szFileName)
	{
		delete[] szFileName;
		szFileName = NULL;
	}

	if (szUrl)
	{
		delete[] szUrl;
		szUrl =NULL;
	}

	if (szProxyUrl)
	{
		delete[] szProxyUrl;
		szProxyUrl =NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//                        通知主程序进度的消息结构
//////////////////////////////////////////////////////////////////////////
//NotifyMsgDataType::NotifyMsgDataType():pszFilePath(NULL)
//{
//
//}
//
//NotifyMsgDataType::~NotifyMsgDataType()
//{
//    //if (pszFilePath)
//    //{
//    //    delete[] pszFilePath;
//    //    pszFilePath = NULL;
//    //}
//}

//////////////////////////////////////////////////////////////////////////
//                        TDownload类函数区
//////////////////////////////////////////////////////////////////////////
//TDownload::TDownload(void)
//{
//}
//
//TDownload::~TDownload(void)
//{
//}

/** 写数据函数
*
* \param ptr 文件指针
* \param size 大小
* \param nmemb 块大小
* \param data 数据缓冲区
* 
* \return 写入文件的实际大小
*/
size_t IDownLoad::WriteProductData (void *ptr, size_t size, size_t nmemb, void *data)
{
	register int realsize = size * nmemb;
	WriteFunArg* pobjWriteFunArg = (WriteFunArg*) data;

	if(pobjWriteFunArg)
	{
		EOS_Write(pobjWriteFunArg->hFile, ptr, realsize);
	}

	return realsize;
}

/** 进度通知函数
*
* \param clientp 数据缓冲区
* \param dltotal 总大小
* \param dlnow 当前下载大小
* \param ultotal 上传总大小
* \param ulnow 已上传大小
* 
* \return >=0 成功 | <0 失败 
*/
int IDownLoad::ProgressNotifyFunc(void *clientp, double dltotal,double dlnow,double ultotal,double ulnow)
{
	WriteFunArg *mem = (WriteFunArg *)clientp;
	if (!mem)
	{
		return -1;
	}

	NotifyMsgDataType notifyData;
	MemSet(&notifyData, 0, sizeof(NotifyMsgDataType));

	notifyData.nCurSize   = (Int32)dlnow;
	notifyData.nTotalSize = (dltotal <= 0)? ((mem->nTotalSize > 0)? mem->nTotalSize: 250000): (Int32)dltotal;
	notifyData.nAccessType  = mem->nAccessType;

	SS_printf("==[ProgressNotifyFunc] total:%d, now:%d\n", dltotal, dlnow);

	App_PostMessageEx(&mem->objGtidMain, MSG_DL_THREAD_PROCESS, &notifyData, sizeof(NotifyMsgDataType), mem->nHwndId, mem->nCtrlId);

	return 0;
}

/** 获取文件长度函数
*
* \param ptr 文件指针
* \param size 大小
* \param nmemb 块大小
* \param stream 数据缓冲区
* 
* \return 读取的文件大小
*/
size_t IDownLoad::GetHeadForLength (void *ptr, size_t size, size_t nmemb, void *stream) 
{
	WriteFunArg *mem = (WriteFunArg *) stream;
	long len = 0;
	const char* pszStr = (char*)ptr;

	if (sscanf(pszStr, "Content-Length: %ld\n", &len))
	{
		mem->nTotalSize = len;
	}

	return size * nmemb;
}

/** 通知主程序进度函数
*
* \param pGtid 主程序线程ID
* \param nHwndId 窗口句柄
* \param eState 请求状态
* \param nCurSize 当前下载大小
* \param nTotalSize 总大小
* \param nDwnIndex 线程索引
* 
* \return >=0 成功 | <0 失败 
*/
/*static */Int32 IDownLoad::NotifyToMain(DownloadCode eDLCode)
{
	NotifyMsgDataType notifyData;
	MemSet(&notifyData, 0, sizeof(NotifyMsgDataType));

	notifyData.eDLCode     = eDLCode;
	//notifyData.nCurSize   = nCurSize;
	//notifyData.nTotalSize = nTotalSize;
	notifyData.nAccessType  = nAccessType;
	notifyData.nHwndId	= nHwndId;
	notifyData.nCtrlId		= nCtrlId;
	notifyData.nParam      	= nParam;
	notifyData.pThisHandle = pThisHandle;
	TUString::StrCopy(notifyData.pszFilePath, szFileName);

	SS_printf("==[NotifyToMain] eDLCode:%d\r\n", eDLCode);

	return App_PostMessageEx(&gtidMain, MSG_DL_THREAD_NOTIFY, &notifyData, sizeof(NotifyMsgDataType), nHwndId, nCtrlId);
}

/** 下载线程入口
*
* \param pAppID 应用ID描述串
* \param nCmd 入口参数
* \param pCmdParam 入口数据
* 
* \return >=0 成功 | <0 失败
*/
Int32 IDownLoad::GetProductProc(const TUChar * pAppID, UInt32 uCmd, void * puCmd)
{
	WriteFunArg         objWriteFunArg;
	IDownLoad     *pDwnParam;

	pDwnParam = (IDownLoad *)puCmd;

	//如果文件已经存在,则先删除
	if (EOS_IsFileExist(pDwnParam->szFileName))
		EOS_Delete(pDwnParam->szFileName);

	objWriteFunArg.hFile     = EOS_Open(pDwnParam->szFileName, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
	if(objWriteFunArg.hFile < 0)
	{
		pDwnParam->NotifyToMain(DLC_ABORT);
		pDwnParam->clearMem();
		return -1;
	}

	MemCopy(&objWriteFunArg.objGtidMain, &pDwnParam->gtidMain, sizeof(SS_GTID));
	objWriteFunArg.nHwndId      = pDwnParam->nHwndId;
	objWriteFunArg.nAccessType    = pDwnParam->nAccessType;
	objWriteFunArg.nCtrlId      = pDwnParam->nCtrlId;
	objWriteFunArg.nTotalSize = 0;
	objWriteFunArg.nCurrentSize = 0;

	curl_easy_reset(pDwnParam->gCurl);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_URL, pDwnParam->szUrl);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_VERBOSE, 1L);         // log输出
	curl_easy_setopt (pDwnParam->gCurl,  CURLOPT_NOSIGNAL,1);
	//curl_easy_setopt (pDwnParam->gCurl, CURLOPT_FAILONERROR, TRUE );  // 状态码大于300时不输出数据，返回错误
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_FOLLOWLOCATION, 1);

	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_HEADERFUNCTION, GetHeadForLength);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_HEADERDATA, &objWriteFunArg);
	//curl_easy_setopt (pDwnParam->gCurl, CURLOPT_RESUME_FROM, bUseResume?nLocalfileSize:0);

	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEFUNCTION, WriteProductData);    
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEDATA, &objWriteFunArg);  

	curl_easy_setopt(pDwnParam->gCurl, CURLOPT_NOPROGRESS, pDwnParam->bNoProNotify);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_PROGRESSFUNCTION, ProgressNotifyFunc);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_PROGRESSDATA, &objWriteFunArg);

	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_TIMEOUT, pDwnParam->nTimeout);

	DownloadCode eDlc = DLC_IDLE;
	if (CURLE_OK == curl_easy_perform (pDwnParam->gCurl))
	{
		eDlc = DLC_FINISHED;
	}
	else
	{
		eDlc = DLC_ABORT;
	}

	if (objWriteFunArg.hFile > 0)
	{
		EOS_Close(objWriteFunArg.hFile);
	}

	//完成后通知应用
	pDwnParam->NotifyToMain(eDlc);

	// 清理
	curl_easy_cleanup (pDwnParam->gCurl);
	pDwnParam->clearMem();

	return 0;
}


Int32 IDownLoad::PostProductProc(const TUChar * pAppID, UInt32 uCmd, void * puCmd)
{
	WriteFunArg         objWriteFunArg;
	IDownLoad     *pDwnParam;
	curl_httppost       *post = NULL;	
	curl_httppost       *last = NULL;	
	
	pDwnParam = (IDownLoad *)puCmd;

	//如果文件已经存在,则先删除
	if (EOS_IsFileExist(pDwnParam->szFileName))
		EOS_Delete(pDwnParam->szFileName);

	objWriteFunArg.hFile     = EOS_Open(pDwnParam->szFileName, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
	if(objWriteFunArg.hFile < 0)
	{
		pDwnParam->NotifyToMain(DLC_ABORT);
		pDwnParam->clearMem();
		return -1;
	}

	MemCopy(&objWriteFunArg.objGtidMain, &pDwnParam->gtidMain, sizeof(SS_GTID));
	objWriteFunArg.nHwndId      = pDwnParam->nHwndId;
	objWriteFunArg.nAccessType    = pDwnParam->nAccessType;
	objWriteFunArg.nCtrlId      = pDwnParam->nCtrlId;
	objWriteFunArg.nTotalSize = 0;
	objWriteFunArg.nCurrentSize = 0;

	curl_easy_reset(pDwnParam->gCurl);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_POST, 1);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_URL, pDwnParam->szUrl);

	Handle hFile = NULL;			// file handle
	void* PhotoData = NULL;
	Int32 nFileSize = 0;			// 文件大小
	Int32 nReadByte = 0;			// 具体读出的字节数


	for(int i=0;i <pDwnParam->aFormData.nParamDataCount; i++)
	{
		//图片上传API的图片二进制数据需要单独处理
		if(pDwnParam->aFormData.pParamData[i].nParamType == CURLFORM_FILE)
		{
	            //中文文件名会出错,需要先转成asc-II文件名, TODO, 暂时传入时就先转了
			char Photofile[256] = {0};

			#ifdef _TRANZDA_VM_
	                		strcpy(Photofile, "D:/Work7");
			#endif
				strcat(Photofile, pDwnParam->aFormData.pParamData[i].pszParamContent);

			//curl会根据Photofile，直接调用标准C函数进行读,所以中文不认
	                 curl_formadd(&post, &last, 
	                 				CURLFORM_COPYNAME,  
	                 				pDwnParam->aFormData.pParamData[i].pszParamName, 
	                                    	CURLFORM_FILE, 
	                                    	Photofile, 
	                                    	CURLFORM_END);				
		}
		else
		{
			curl_formadd(&post, &last, 
				CURLFORM_COPYNAME, 
				pDwnParam->aFormData.pParamData[i].pszParamName, 
				pDwnParam->aFormData.pParamData[i].nParamType, 
				pDwnParam->aFormData.pParamData[i].pszParamContent, 
				CURLFORM_END);	
		}
	}


	curl_easy_setopt(pDwnParam->gCurl, CURLOPT_HTTPPOST, post);

	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_VERBOSE, 1L);         // log输出
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_NOSIGNAL,1);          // 忽略所有的curl传递给php进行的信号，否则多线程环境使用可能会出现段错误
//    curl_easy_setopt (pDwnParam->gCurl, CURLOPT_FAILONERROR, TRUE );  // 状态码大于300时不输出数据，返回错误
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEFUNCTION, WriteProductData);            // 下载数据的函数
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEDATA, &objWriteFunArg);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_TIMEOUT, pDwnParam->nTimeout);
	
#if 0
    // 需要身份验证
    if (pDwnParam->bAuthorization)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, pDwnParam->nAuthorizationType);
        curl_easy_setopt(curl, CURLOPT_USERPWD, pDwnParam->pszUserPw); 
    }  

    //是否需要添加If-Modified-Since头信息
    if (pDwnParam->bCheckModifity)
    {        
        char szModifiedTime[512];

        DateTimeToStr(szModifiedTime,512,pDwnParam->dtFileModified);
        http_headers = curl_slist_append(http_headers, szModifiedTime);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);       
    }
#endif

	DownloadCode eDlc = DLC_IDLE;
	if (CURLE_OK == curl_easy_perform (pDwnParam->gCurl))
	{
		eDlc = DLC_FINISHED;
	}
	else
	{
		eDlc = DLC_ABORT;
	}

	if (objWriteFunArg.hFile > 0)
	{
		EOS_Close(objWriteFunArg.hFile);
	}

	//完成后通知应用
	pDwnParam->NotifyToMain(eDlc);

	// 清理
	curl_easy_cleanup (pDwnParam->gCurl);
	pDwnParam->clearMem();

	if(PhotoData != NULL)
		free(PhotoData);
	
	return 0;
}

/** 下载文件到本地
* 
* \param pszUrl 下载的URL
* \param pszFilepath 本地保存数据的文件路径
* \param nTimeout 超时时长
* 
* \return 下载状态码
*/
DownloadError IDownLoad::DownloadStart(const char* in_pszUrl, const TUChar* in_pszFilepath, Int32 in_nAccessType, Int32 in_nHwndId, Int32 in_nCtrlId, Int32 in_nParam, Int32 in_nTimeout, Boolean in_bNoProNotify, void* in_pThisHandle, int nMethod, FormDataType in_aFormData)
{
	DownloadError eErr = ERR_NONE;

	do
	{
		if (!in_pszUrl)
		{
			eErr = ERR_INVALID_URL;
			break;
		}

		if (!in_pszFilepath)
		{
			eErr = ERR_INVALID_FILEPATH;
			break;
		}

		Int32 nLen = 0;
		//获得主进程GTID
		SS_GetCurrentGTID(&gtidMain);
		nAccessType 	= in_nAccessType;
		nTimeout 	= in_nTimeout;
		nHwndId 		= in_nHwndId;
		nCtrlId 		= in_nCtrlId;
		bNoProNotify 	= in_bNoProNotify;
		pThisHandle 	= in_pThisHandle;
		nParam  		= in_nParam;
		aFormData.nParamDataCount	= in_aFormData.nParamDataCount;
		if(aFormData.nParamDataCount > 0)
		{
			for(int i=0; i<aFormData.nParamDataCount; i++)
			{
				aFormData.pParamData[i].pszParamName = in_aFormData.pParamData[i].pszParamName;
				aFormData.pParamData[i].pszParamContent = in_aFormData.pParamData[i].pszParamContent;
				aFormData.pParamData[i].nParamType = in_aFormData.pParamData[i].nParamType;
			}
		}
		//Url
		nLen = strlen(in_pszUrl) + 1;
		szUrl = new char[nLen];
		MemSet(szUrl, 0, nLen);
		strncpy(szUrl, in_pszUrl, nLen);

		//Filepath
		nLen = TUString::StrLen( in_pszFilepath ) + 1;
		szFileName = new TUChar[nLen];
		MemSet(szFileName, 0, nLen * sizeof(TUChar));
		TUString::StrNCopy(szFileName, in_pszFilepath, nLen);

		//初始化Curl
		if (!(gCurl = curl_easy_init ()))
		{
			clearMem();
			return ERR_NONE;
		}

		//创建新线程用来下载
		SS_GTID sGtidThread;
		if(nMethod == METHOD_POST)
		{
			if(SS_App_CreateThreadEx(&sGtidThread, 2, this, sizeof(IDownLoad), PostProductProc) < 0)
			{
				eErr = ERR_FAILED_CREATE_THREAD;
				break;
			}
		}
		else
		{
			if(SS_App_CreateThreadEx(&sGtidThread, 2, this, sizeof(IDownLoad), GetProductProc) < 0)
			{
				eErr = ERR_FAILED_CREATE_THREAD;
				break;
			}
		}
	} while (0);

	return eErr;
}

DownloadError IDownLoad::DownloadAbort(void)
{
	DownloadError eErr = ERR_NONE;


	if(gCurl != NULL)
	{
		curl_easy_pause(gCurl, CURLPAUSE_ALL);
		curl_easy_reset(gCurl);	
	}

	return eErr;
}


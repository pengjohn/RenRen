/*!
* 
* \file   TDownload.cpp
* \author Yusz
* \date	  2010.7.5
* \brief  Curl���ؽӿ�
* 
* \ref    CopyRight
*/

#include "TDownload.h"
#include "RenRenAPICommon.h"
//////////////////////////////////////////////////////////////////////////
//                        DoDownLoadParam�ṹ�庯��
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

/** �ͷ�DoDownLoadParam�ṹ����new�������ڴ�ռ�
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
//                        ֪ͨ��������ȵ���Ϣ�ṹ
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
//                        TDownload�ຯ����
//////////////////////////////////////////////////////////////////////////
//TDownload::TDownload(void)
//{
//}
//
//TDownload::~TDownload(void)
//{
//}

/** д���ݺ���
*
* \param ptr �ļ�ָ��
* \param size ��С
* \param nmemb ���С
* \param data ���ݻ�����
* 
* \return д���ļ���ʵ�ʴ�С
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

/** ����֪ͨ����
*
* \param clientp ���ݻ�����
* \param dltotal �ܴ�С
* \param dlnow ��ǰ���ش�С
* \param ultotal �ϴ��ܴ�С
* \param ulnow ���ϴ���С
* 
* \return >=0 �ɹ� | <0 ʧ�� 
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

/** ��ȡ�ļ����Ⱥ���
*
* \param ptr �ļ�ָ��
* \param size ��С
* \param nmemb ���С
* \param stream ���ݻ�����
* 
* \return ��ȡ���ļ���С
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

/** ֪ͨ��������Ⱥ���
*
* \param pGtid �������߳�ID
* \param nHwndId ���ھ��
* \param eState ����״̬
* \param nCurSize ��ǰ���ش�С
* \param nTotalSize �ܴ�С
* \param nDwnIndex �߳�����
* 
* \return >=0 �ɹ� | <0 ʧ�� 
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

/** �����߳����
*
* \param pAppID Ӧ��ID������
* \param nCmd ��ڲ���
* \param pCmdParam �������
* 
* \return >=0 �ɹ� | <0 ʧ��
*/
Int32 IDownLoad::GetProductProc(const TUChar * pAppID, UInt32 uCmd, void * puCmd)
{
	WriteFunArg         objWriteFunArg;
	IDownLoad     *pDwnParam;

	pDwnParam = (IDownLoad *)puCmd;

	//����ļ��Ѿ�����,����ɾ��
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
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_VERBOSE, 1L);         // log���
	curl_easy_setopt (pDwnParam->gCurl,  CURLOPT_NOSIGNAL,1);
	//curl_easy_setopt (pDwnParam->gCurl, CURLOPT_FAILONERROR, TRUE );  // ״̬�����300ʱ��������ݣ����ش���
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

	//��ɺ�֪ͨӦ��
	pDwnParam->NotifyToMain(eDlc);

	// ����
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

	//����ļ��Ѿ�����,����ɾ��
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
	Int32 nFileSize = 0;			// �ļ���С
	Int32 nReadByte = 0;			// ����������ֽ���


	for(int i=0;i <pDwnParam->aFormData.nParamDataCount; i++)
	{
		//ͼƬ�ϴ�API��ͼƬ������������Ҫ��������
		if(pDwnParam->aFormData.pParamData[i].nParamType == CURLFORM_FILE)
		{
	            //�����ļ��������,��Ҫ��ת��asc-II�ļ���, TODO, ��ʱ����ʱ����ת��
			char Photofile[256] = {0};

			#ifdef _TRANZDA_VM_
	                		strcpy(Photofile, "D:/Work7");
			#endif
				strcat(Photofile, pDwnParam->aFormData.pParamData[i].pszParamContent);

			//curl�����Photofile��ֱ�ӵ��ñ�׼C�������ж�,�������Ĳ���
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

	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_VERBOSE, 1L);         // log���
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_NOSIGNAL,1);          // �������е�curl���ݸ�php���е��źţ�������̻߳���ʹ�ÿ��ܻ���ֶδ���
//    curl_easy_setopt (pDwnParam->gCurl, CURLOPT_FAILONERROR, TRUE );  // ״̬�����300ʱ��������ݣ����ش���
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEFUNCTION, WriteProductData);            // �������ݵĺ���
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_WRITEDATA, &objWriteFunArg);
	curl_easy_setopt (pDwnParam->gCurl, CURLOPT_TIMEOUT, pDwnParam->nTimeout);
	
#if 0
    // ��Ҫ�����֤
    if (pDwnParam->bAuthorization)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, pDwnParam->nAuthorizationType);
        curl_easy_setopt(curl, CURLOPT_USERPWD, pDwnParam->pszUserPw); 
    }  

    //�Ƿ���Ҫ���If-Modified-Sinceͷ��Ϣ
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

	//��ɺ�֪ͨӦ��
	pDwnParam->NotifyToMain(eDlc);

	// ����
	curl_easy_cleanup (pDwnParam->gCurl);
	pDwnParam->clearMem();

	if(PhotoData != NULL)
		free(PhotoData);
	
	return 0;
}

/** �����ļ�������
* 
* \param pszUrl ���ص�URL
* \param pszFilepath ���ر������ݵ��ļ�·��
* \param nTimeout ��ʱʱ��
* 
* \return ����״̬��
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
		//���������GTID
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

		//��ʼ��Curl
		if (!(gCurl = curl_easy_init ()))
		{
			clearMem();
			return ERR_NONE;
		}

		//�������߳���������
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


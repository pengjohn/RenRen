/*!
* 
* \file   TDownload.h
* \author Yusz
* \date	  2010.7.8
* \brief  
* 
* \ref    CopyRight
*/
#pragma once

#include "TG3.h"
#include "curl.h"

/** ����״̬��
* 
*/

enum
{
	METHOD_GET	= 0,
	METHOD_POST,
};

typedef enum
{
	DLC_IDLE,                   //����
	DLC_HEADER,                 //��ȡ��Header
	DLC_NONEEDUPFATE,           //û�и���
	DLC_DOWNLOADING,            //������������
	DLC_ABORT,                  //��ֹ
	DLC_FINISHED                //�������
}DownloadCode;

typedef enum
{
	ERR_NONE = 0,               //û�д���
	ERR_INVALID_URL,            //URL��Ч
	ERR_INVALID_FILEPATH,       //�ļ�·����Ч
	ERR_INVALID_GTID_HANDLE,    //GTID�����Ч
	ERR_FAILED_NEW_DLP,         //new����ʧ��
	ERR_FAILED_CREATE_THREAD    //�����̳߳���
}DownloadError;

/** �����̷߳�������Ϣ
* 
*/
typedef enum
{
	MSG_DL_THREAD_NOTIFY = EVENT_FirstUser + 100,
	MSG_DL_THREAD_NEWTASK,
	MSG_DL_THREAD_PROCESS,
	MSG_DL_THREAD_START,
	MSG_DL_THREAD_END,
}DownloadThreadMsg;


// Post�ı������������� 
typedef struct
{
    char *pszParamName;		// �������� 
    char *pszParamContent;	// �������� 
    Int32 nParamType;			// �������� 
}ParamDataType;

// Post�ı����� 
typedef struct
{
    ParamDataType pParamData[20];	// �������� ,����ݶ�Ϊ20��
    Int32 nParamDataCount;		// �������� 
}FormDataType;

/** ���������̵߳Ĳ���*/
class IDownLoad
{
public:
	IDownLoad(void);
	~IDownLoad(void);

	/** ֻ�е�Ҫ�ͷ�szFileName��szUrl��szProxyUrlָ����ڴ�ʱ���� */
	void clearMem();

	DownloadError DownloadStart(const char* pszUrl, const TUChar* pszFilepath, Int32 in_nAccessType, Int32 nHwndId, Int32 nCtrlId, Int32 in_nParam, Int32 nTimeout, Boolean bNoProNotify, void* pThisHandle, int nMethod, FormDataType in_aFormData);
	DownloadError DownloadAbort(void);
	static Int32 GetProductProc(const TUChar * pAppID, UInt32 uCmd, void * puCmd);
	static Int32 PostProductProc(const TUChar * pAppID, UInt32 uCmd, void * puCmd);	
	
	static int ProgressNotifyFunc(void *clientp, double dltotal,double dlnow,double ultotal,double ulnow);
	static size_t WriteProductData (void *ptr, size_t size, size_t nmemb, void *data);
	static size_t GetHeadForLength (void *ptr, size_t size, size_t nmemb, void *stream);
	Int32 NotifyToMain(DownloadCode eDLCode);

	CURL               *gCurl;	//Curl
	Int32           	nAccessType;	// �߳����� ,����ID�Ĺؼ��ֶ�
	SS_GTID		gtidMain;// ������GTID 
	Int32		nHwndId;	// ������Ĵ���ID 
	Int32		nCtrlId;		// ������Ŀؼ�ID 
	Int32 		nParam;	//����
	Boolean		bCheckMsg;	// �Ƿ������Ϣ 
	TUChar*		szFileName;	// ���������ļ����� 
	char*		szUrl;		// ��Ҫ���ص�URL 
	FormDataType   aFormData;	// Post�ı����� 
	Boolean		bUseProxy;	// �Ƿ���Ҫʹ�ô��� 
	char*		szProxyUrl;	// ����URL 
	Int32		nProxyPort;	// ����˿� 
	//DateTimeType    dtFileModified; 	// �����ļ���������ʱ�� 
	//Boolean			bCheckModifity;	// �Ƿ���Ҫ������ 
	Int32           	nTimeout;			// ��ʱʱ������ 
	Boolean 		bNoProNotify;		//�Ƿ���ҪProgress��Ϣ
	void* 		pThisHandle;		//����Ӧ��new����ָ��,���ڷ�����Ӧ��delete
};

/** ֪ͨ��������ȵ���Ϣ�ṹ */
struct NotifyMsgDataType
{
	Int32           	nAccessType;	//��������,��ӦRR_AccessType
	Int32		nHwndId;		//������Ĵ���ID
	Int32		nCtrlId;			// ������Ŀؼ�ID
	Int32		nParam;			//����
	DownloadCode      eDLCode;	//״̬
	Int32		nCurSize;		//�����ش�С
	Int32		nTotalSize;		//�ܵ���Ҫ���صĴ�С
	TUChar         	pszFilePath[EOS_FILE_MAX_PATH];//�����ļ���
	void* 		pThisHandle;	//����Ӧ��new IDownLoad����ָ��,���ڷ�����Ӧ��delete
};

/** �ص������Ĳ��� */
struct WriteFunArg
{
	Int32           	nAccessType;	// �����߳�����ֵ 
	SS_GTID		objGtidMain;		// ������GTID 
	Int32		nHwndId;		// ������Ĵ���ID 
	Int32		nCtrlId;			// ������Ŀؼ�ID 
	Int32		nCurrentSize;	// �Ѿ����صĴ�С 
	Int32		nTotalSize;		// ��Ҫ���صĴ�С 
	Handle          	hFile;			// ��Ҫд����ļ���� 
	//Int32          nLocalFileSize;	// �Ѵ��ڵ��ļ���С 
};


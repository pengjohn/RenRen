/*!
*@FileName: RenRenAPICommon.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: ���ýӿں���
*@Log: 	Author 			Date 		Description
*
*/

#include "RenRenAPI.h"
#include "RenRenAPICommon.h"
#include  "TDownload.h"
#include "TFileDownloadMrg.h"

/**
 * Ӧ��API���� 
 *
 * \param accessType
 * \param in_nHwndId
 * \param in_nCtrlId
 * \param in_nParam
 * \param bOverWrite
 *
 * \return 
 */
int RenRenAPICommon_Download(RR_AccessType  accessType, Int32 in_nHwndId, Int32 in_nCtrlId, Int32 in_nParam, Boolean bOverWrite)
{
	//����log
	char logTxt[128+20] = {0};
	sprintf(logTxt, "[DL ready:%2d]",accessType);
	strcat(logTxt, RR_GetFileName((RR_AccessType)accessType));
	RenRen_Log(logTxt);
			
	DownloadError eErr = ERR_NONE;
	TUChar* pszFilePath = NULL;
	char* AppUrl = NULL;
	pszFilePath = RR_GetFileNameFullPath(accessType);
	AppUrl = RR_GetUrl(accessType);
	if( (pszFilePath == NULL) || (*pszFilePath == 0) || (AppUrl == NULL) || (*AppUrl == 0) )
		return eFailed;

	//ɾ�������ļ�
	if (EOS_IsFileExist(pszFilePath))
	{
		if(bOverWrite)
		{
			EOS_Delete(pszFilePath);
		}
		else
		{
			TUChar* pszFilePathBak = NULL;
			pszFilePathBak = RR_GetBakFileNameFullPath(accessType);
			if (EOS_IsFileExist(pszFilePathBak))
				EOS_Delete(pszFilePathBak);
			
			EOS_Rename(pszFilePath, pszFilePathBak);
			free(pszFilePathBak);
		}
	}		
	//ͨ�ò�����ֵ
	Set_Url_Params(accessType, "session_key", RenRenUserInfo.session_key);

	char pszCallId[32] = {0};
	sprintf(pszCallId, "%ul", GetTickCount());
	Set_Url_Params(accessType, "call_id", pszCallId);
	

	//����Sig
	char*	sSig = NULL;
	Url_GenerateSig(accessType, &sSig);
	Set_Url_Params(accessType, "sig", sSig);
		
	//����POST��
	FormDataType aFormData = {0};
	Url_GenerateFormData(accessType, &aFormData);

	//��������Ĳ���
	TAListElement  pElement;
	MemSet(&pElement, 0, sizeof(TAListElement));
	pElement.accessType = accessType;
	STRCPY_Ex(pElement.pURL,AppUrl);
	pElement.nHwndId = in_nHwndId;
	pElement.nCtrlId = in_nCtrlId;
	pElement.nParam = in_nParam;
	memcpy(&pElement.aFormData, &aFormData,sizeof(pElement.aFormData));
	TUString::StrCopy(pElement.pszFilePath, pszFilePath);
	free(pszFilePath);

	if(g_pFileDownMgr == NULL)
		g_pFileDownMgr = new TFileDownloadMgr;
	
	g_pFileDownMgr->Add(&pElement);
	g_pFileDownMgr->DownLoadStart();

	return eSucceed;

}	

/**
 * ͼƬ���� 
 *
 * \param pURL
 * \param in_nHwndId
 * \param in_nParam
 *
 * \return 
 */
int RenRenAPICommon_DownloadPhoto(char* pURL, Int32 in_nHwndId,Int32 in_nParam )
{
	
	if(pURL!=NULL && *pURL != 0)
	{
		TUChar* pszFilePath = NULL;
		pszFilePath = FileNameFullPathFromUrl(pURL);
		
		if(pszFilePath != NULL)
		{
			TAListElement  pElement;
			MemSet(&pElement, 0, sizeof(TAListElement));
			pElement.accessType = RR_PhotoDownload;
			STRCPY_Ex(pElement.pURL,pURL);
			pElement.nHwndId = in_nHwndId;
			pElement.nCtrlId = 0;
			pElement.nParam = in_nParam;
			TUString::StrCopy(pElement.pszFilePath, pszFilePath);
			free(pszFilePath);
			
			if(g_pFileDownMgr == NULL)
				g_pFileDownMgr = new TFileDownloadMgr;
			
			g_pFileDownMgr->Insert(&pElement, 0);
			g_pFileDownMgr->DownLoadStart();
		}
	}
	return eSucceed;
}

/**
 * ȡ������AccessType��ͬ������
 *
 * \param accessType
 *
 * \return 
 */
int RenRenAPICommon_DownloadCancelByAccessType(RR_AccessType  accessType)
{
	return g_pFileDownMgr->CancelByAccessType(accessType);
}

/**
 * ȡ������HwndId��ͬ������ 
 *
 * \param nHwndId
 *
 * \return 
 */
int RenRenAPICommon_DownloadCancelByHwndId(Int32 nHwndId)
{
	return g_pFileDownMgr->CancelByHwndId(nHwndId);
}

/**
 * ȡ���������� 
 *
 * \return 
 */
int RenRenAPICommon_DownloadCancelAll(void)
{
	return g_pFileDownMgr->CancelAll();
}

/**
 * ������� 
 *
 * \param pElement
 *
 * \return 
 */
int RenRenAPICommon_DownloadComplete(TAListElement*  pElement)
{
	if(g_pFileDownMgr != NULL)
		return g_pFileDownMgr->DownLoadComplete(pElement);
	else
		return eFailed;
}

/**
 * ���ؽӿ� 
 * ����ṩ��TFileDownloadMrg����
 * \param in_nAccessType
 * \param pURL
 * \param pszFilePath
 * \param in_nHwndId
 * \param in_nCtrlId
 * \param in_nParam
 * \param aFormData
 *
 * \return 
 */
int RenRenAPI_Download(Int32 in_nAccessType, char* pURL, const TUChar* pszFilePath, Int32 in_nHwndId, Int32 in_nCtrlId, Int32 in_nParam, FormDataType aFormData)
{
	DownloadError eErr = ERR_NONE;
	char* FilePath = NULL;

	if( (pURL == NULL) || (*pURL ==0) || (pszFilePath == NULL) || (*pszFilePath == 0) )
		return eFailed;

	//TODO:: ����ļ�����, ���ж�ʱ�� or ��Сor else (����), �Ծ����Ƿ���Ҫ��������
	if (EOS_IsFileExist(pszFilePath))
	{
		NotifyMsgDataType notifyData;
		MemSet(&notifyData, 0, sizeof(NotifyMsgDataType));

		SS_GTID			gtidMain;
		SS_GetCurrentGTID(&gtidMain);
		notifyData.eDLCode     = DLC_FINISHED;
		notifyData.nAccessType  = in_nAccessType;
		notifyData.nHwndId = in_nHwndId;
		notifyData.nCtrlId = in_nCtrlId;
		notifyData.nParam = in_nParam;
		notifyData.pThisHandle = NULL;
		TUString::StrCopy(notifyData.pszFilePath, pszFilePath);
		App_PostMessageEx(&gtidMain, MSG_DL_THREAD_NOTIFY, &notifyData, sizeof(NotifyMsgDataType), in_nHwndId, in_nCtrlId);
		return eSucceed;
	}

	{
		if(in_nAccessType == RR_PhotoDownload)
			eErr = g_pDownload->DownloadStart(pURL, pszFilePath, in_nAccessType, in_nHwndId, in_nCtrlId, in_nParam, 30,  TRUE, NULL, METHOD_GET, aFormData);
		else if(in_nAccessType == RR_PhotosUpload)
		{
			eErr = g_pDownload->DownloadStart(pURL, pszFilePath, in_nAccessType, in_nHwndId, in_nCtrlId, in_nParam, 60,  TRUE, NULL, METHOD_POST, aFormData);
		}
		else
		{
			Boolean bNeedWaitingBox = FALSE;
			bNeedWaitingBox = RR_GetApiNeedWaitingBox((RR_AccessType)in_nAccessType);
			//�������Ҫ�ȴ����API���س�ʱΪ10��,��ͨ��API���س�ʱΪ30��
			if(bNeedWaitingBox == TRUE)
				eErr = g_pDownload->DownloadStart(pURL, pszFilePath, in_nAccessType, in_nHwndId, in_nCtrlId, in_nParam, 30,  TRUE, NULL, METHOD_POST, aFormData);
			else
				eErr = g_pDownload->DownloadStart(pURL, pszFilePath, in_nAccessType, in_nHwndId, in_nCtrlId, in_nParam, 10,  TRUE, NULL, METHOD_POST, aFormData);
		}
		
		// ��url��������,�������
		if(in_nAccessType != RR_PhotoDownload)
		{
			int iRet = eFailed;
			Handle hFile = NULL;		
			TUChar pszTempPath[EOS_FILE_MAX_PATH] ={0};
			TUChar* pszTempPath1 = NULL;
				
			TUString::StrNCopy(pszTempPath, pszFilePath, EOS_FILE_MAX_PATH);
			pszTempPath1 = TUString::StrRChr(pszTempPath, 0x002e);	//'.'
			pszTempPath1[1] = 0x0074;	//'t'
			pszTempPath1[2] = 0x0078;	//'x'	
			pszTempPath1[3] = 0x0074;	//'t'
				
			if (EOS_IsFileExist(pszTempPath))
				EOS_Delete(pszTempPath);

			hFile = EOS_Open(pszTempPath, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
			if (hFile < 0)
			{
				return eFailed;
			}	                    		
				
			EOS_Write(hFile, (void*)pURL, strlen(pURL)*sizeof(char));
			for(int i=0;i <aFormData.nParamDataCount; i++)
			{
				EOS_Write(hFile, (void*)"\n[", strlen("\n[")*sizeof(char));
				EOS_Write(hFile, (void*)aFormData.pParamData[i].pszParamName, strlen(aFormData.pParamData[i].pszParamName)*sizeof(char));
				EOS_Write(hFile, (void*)"]", strlen("]")*sizeof(char));
				EOS_Write(hFile, (void*)aFormData.pParamData[i].pszParamContent, strlen(aFormData.pParamData[i].pszParamContent)*sizeof(char));
			}
			EOS_Close(hFile);

			//����log
			char logTxt[128+20] = {0};
			sprintf(logTxt, "[DL Start:%2d]",in_nAccessType);
			strcat(logTxt, RR_GetFileName((RR_AccessType)in_nAccessType));
			RenRen_Log(logTxt);
			
		}
	}

	//������س���,��Ҫ�����ض�����Remove
	if( eErr  != ERR_NONE)
	{
		//��������б��е�����(�ж�����:accessType/nHwndId/nCtrlId/nParam ȫƥ��)
		TAListElement  pElement;
		MemSet(&pElement, 0, sizeof(TAListElement));
		pElement.accessType = in_nAccessType;
		pElement.nHwndId = in_nHwndId;
		pElement.nCtrlId = in_nCtrlId;
		pElement.nParam = in_nParam;
		RenRenAPICommon_DownloadComplete(&pElement);		
		return eFailed;
	}

	//API json����ʱ��ʾ�ȴ���ʾ��
	if(in_nAccessType != RR_PhotoDownload)
	{
		SS_GTID		gtidMain;
		NotifyMsgDataType notifyData;
		MemSet(&notifyData, 0, sizeof(NotifyMsgDataType));
		SS_GetCurrentGTID(&gtidMain);
		notifyData.nAccessType  = in_nAccessType;
		App_PostMessageEx(&gtidMain, MSG_DL_THREAD_START, &notifyData, sizeof(NotifyMsgDataType), in_nHwndId, in_nCtrlId);
	}

	return eSucceed;

}

/**
 * �ж�����
 *
 * \return 
 */
int RenRenAPI_DownloadAbort(void)
{
	return  g_pDownload->DownloadAbort();
}

#include "TCOM_FileBrowser_Interface.h"
#include "TCOM_FileBrowser_IIDs.h"     // ��ͷ�ļ�������Ŀ�н��ܰ���һ��
#include "TCOM_DigitalCamera_DataType.h"
#include "TCOM_DigitalCamera_IIDs.h"	// ��ͷ�ļ�������Ŀ�н��ܰ���һ��
#include "ImageToolKit/IT_ImageSaver.h"
#include "ssHwSdCard.h"

/**
 * ͼƬ�ϴ�ͳһUI�ӿ� 
 *
 * \param pApp
 * \param nHwndId
 *
 * \return 
 */
Int32 PopBox_UploadPhoto(TApplication* pApp, Int32 nHwndId)
{

	//�ȼ��T���Ƿ����
	if(EOS_FileIsInAvailabilityPath( EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_CARD) ) <= 0)
	{
		Int32 nSDCardState = SDCard_GetCardState();

		if( (nSDCardState == TG3_CARD_STATE_EXIST_OK) && (UsbMgr_GetCurrentFunction() == TG3_USB_FUNCTION_USBDISK))
		{
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_UDiskMode),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
			//T����U��			
		}
		else if(nSDCardState == TG3_CARD_STATE_NOEXIST)
		{
			//û��T��
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SDNotReady),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
		}
		else
		{
			//δ֪����
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SDNotReady),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
		}
		
		return -1;
	}
	
	int Ret = -1;
	Int32  nRetPopBoxID = 0;
	TPopupBox	*pPopBox = new TPopupBox(pApp, TResource::LoadConstString(APP_RE_ID_STRING_Cancel), NULL);
	pPopBox->AddItem (1, TResource::LoadConstString(APP_RE_ID_STRING_TakePhotoAndSend), TG3_POPUPBOX_ITEM_TYPE_NORMAL);
	pPopBox->AddItem (2, TResource::LoadConstString(APP_RE_ID_STRING_SelectPhotoAndSend), TG3_POPUPBOX_ITEM_TYPE_NORMAL);
	nRetPopBoxID = pPopBox->Popup();

	delete pPopBox;
	pPopBox = NULL;
	
	//Take Photo
	if(nRetPopBoxID == 1)
	{
		//�ж�T���ռ��Ƿ��㹻, >4096KByte
		Int32 nSectorsPerClusterP = 0;
		Int32 nBytesPerSectorP = 0;
		Int32 nNumberOfFreeClustersP = 0;
		Int32 nTotalNumberOfClustersP = 0;
		Int64 nStorageFreeSize = 0;

		//���Ŀ¼�洢�ռ����
		EOS_GetDiskFreeSpace(EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_CARD),&nSectorsPerClusterP,&nBytesPerSectorP,  &nNumberOfFreeClustersP,&nTotalNumberOfClustersP);
		//������ÿռ�
		nStorageFreeSize = nNumberOfFreeClustersP*nSectorsPerClusterP;
		nStorageFreeSize *= nBytesPerSectorP;
		//��λ�����KB
		nStorageFreeSize /= 1024;
		if(nStorageFreeSize < 4096)
		{
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SDNotSpace),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
			return -1;		
		}

		
		TUChar ImageFile[EOS_FILE_MAX_PATH] = {0};
		HRESULT hr = 0;

		//UI�ӿ�
		TCOM_DigitalCamera_DataType *pCamera;
		TCOM_SYS_Method_Get *pCameraMethod;
		TCoInitialize(NULL);

		// �������ݴ��ڲ��ҽӿ�
		TCOM_MethodEmun methodEmun(NULL);
		if(methodEmun.EnumMethod( TIID_DigitalCamera_DataType ,0,TIID_SYS_Method_Get)<= 0)  
		{ 
			SS_printf("EnumMethod failed! \n");
			return -1;
		}

		// ���UI�ӿ�
		if(methodEmun.GetDataTypeInterface(0, TCOM_CLSCTX_INPROC_SERVER, (LPVOID *)&pCamera) > 0)  
		{		
			// ��ò���UI�ӿ�
			hr = pCamera->QueryInterface(TIID_SYS_Method_Get,( LPVOID * )&pCameraMethod );
			if(!TCOM_S_SUCCESS(hr))
			{
				SS_printf("pCamera->QueryInterface! \n");
			}
		}

		if(pCameraMethod && pCamera)
		{
			// ��Camera UI�ӿ�
			pCamera->SetImageSize(SCR_W, SCR_H);
			pCamera->SetOperationMode(PHOTO_MODE);
			if ( pCameraMethod->Invoke() )
			{
				TUString::StrCopy(ImageFile, pCamera->GetImageFileName ());	  
			}							
			else
			{
			}
		}

		if(pCamera)
		{
			pCamera->Release();
			pCamera = NULL;
		}

		if(pCameraMethod)
		{
			pCameraMethod->Release();
			pCameraMethod = NULL;
		}

		TCoFreeUnusedLibraries();
		TCoUninitialize();

		//�ϴ��ļ�
		if ( TUString::StrLen(ImageFile) >0 )
		{
			char FileName[EOS_FILE_MAX_PATH] = {0};				
			ConvertUploadFile(ImageFile, FileName);
					
			Set_Url_Params(RR_PhotosUpload, "upload", FileName);
			Ret = 1;
		       //RenRenAPICommon_Download(RR_PhotosUpload,  this->GetWindowHwndId(), 0, 0);	
		}
	}
	//Select Photo
	else if(nRetPopBoxID == 2)
	{
		    // Invoke sample :
		    HRESULT      hr;
		    TCOM_SYS_Method_Open  *pOpen;
		    TCOM_FileBrowser_DataType_FileDialog *pFileDialog;

		    TCoInitialize(NULL);

		    //���ҷ���
		    TCOM_MethodEmun methodEmun(TResource::LoadConstString(APP_RE_ID_STRING_NoState));

		    if(methodEmun.EnumMethod(TIID_FileBrowser_DataType_FileDialog, 0, TIID_SYS_Method_Open) <= 0)  // >0 ��ʾ�ҵ�����
		    {
		        TCoUninitialize();
		        return -1;
		    }
		    if(methodEmun.GetDataTypeInterface(0, TCOM_CLSCTX_INPROC_SERVER, (LPVOID *)&pFileDialog) >= 0)
		    {
		        // ׼������
		       pFileDialog->SetDialogTitle( (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_UploadPhoto) );
			pFileDialog->SetExtFilter((TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_PhotoFormat));
			pFileDialog->SetOpenType(FMB_FILE_SELECT_OPEN_MODE_FILE_OPEN);
		       hr = pFileDialog->QueryInterface(TIID_SYS_Method_Open, (LPVOID *)&pOpen);
		       if(!TCOM_S_SUCCESS(hr))
		       {
		           //������
		           pFileDialog->Release();
		           TCoUninitialize();
		           return -1;
		       }
		       pOpen->Invoke();
		       pOpen->Release();


		        // ��ѯ�û�ѡ���� 
		        if (pFileDialog->GetFilePathName() != NULL)
		        {
				char FileName[EOS_FILE_MAX_PATH] = {0};				
				ConvertUploadFile((TUChar*)pFileDialog->GetFilePathName(), FileName);
					
				Set_Url_Params(RR_PhotosUpload, "upload", FileName);
				Ret = 1;
			       //RenRenAPICommon_Download(RR_PhotosUpload,  this->GetWindowHwndId(), 0, 0);
		        }

		        pFileDialog->Release(); // �ͷ��������Դ
		    }
		    TCoFreeUnusedLibraries();
		    TCoUninitialize();
	
	}

	if(Ret == 1)
	{
#if 0	
		//����˵��
		PostNotifyMsgDataType in_msgData = {0};
		MemSet(&in_msgData, 0, sizeof(PostNotifyMsgDataType));
		
		in_msgData.nCtrlId = 0;
		in_msgData.nHwndId = nHwndId;

		in_msgData.nAppTitleStrID = APP_RE_ID_STRING_AddPhotoTitle; 
		in_msgData.nPublicBtnCaptionStrID = APP_RE_ID_STRING_UploadPhoto;
		
		in_msgData.nTitleEditMaxChars = RENREN_LENGTH_BLOG_TITLE;
		in_msgData.nContentEditMaxChars = RENREN_LENGTH_PHOTO_INFO;
		
		in_msgData.IsReceiverEdit = FALSE;
		in_msgData.IsExpressionBtn = FALSE;
		in_msgData.IsTitleEdit = FALSE;
		in_msgData.IsPrivacyCtl= FALSE;
		in_msgData.IsPopEdit = TRUE;
		in_msgData.IsAllowsNull = TRUE;
		in_msgData.nParam = RR_PhotosUpload;

		TAppEditForm *pWin = new TAppEditForm( pApp, in_msgData);
#endif
		TPhotoUploadInfoForm *pWin = new TPhotoUploadInfoForm( pApp, nHwndId );
	}

	return Ret;							
}


/**
 * ͼƬ����
 *
 * \param pApp
 * \param pszFilePath
 *
 * \return 
 */
Int32 PopBox_SavePhoto(TApplication* pApp, TUChar* pszFilePath)
{

	//�ȼ��T���Ƿ����
	if(EOS_FileIsInAvailabilityPath( EOS_GetSpecialPath(EOS_FILE_SPECIAL_PATH_CARD) ) <= 0)
	{
		Int32 nSDCardState = SDCard_GetCardState();

		if( (nSDCardState == TG3_CARD_STATE_EXIST_OK) && (UsbMgr_GetCurrentFunction() == TG3_USB_FUNCTION_USBDISK))
		{
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_UDiskMode),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
			//T����U��			
		}
		else if(nSDCardState == TG3_CARD_STATE_NOEXIST)
		{
			//û��T��
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SDNotReady),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
		}
		else
		{
			//δ֪����
			pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SDNotReady),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
		}
		
		return -1;
	}

	int Ret = -1;	
	    // Invoke sample :
	    HRESULT      hr;
	    TCOM_SYS_Method_Open  *pOpen;
	    TCOM_FileBrowser_DataType_FileDialog *pFileDialog;

	    TCoInitialize(NULL);

	    //���ҷ���
	    TCOM_MethodEmun methodEmun(TResource::LoadConstString(APP_RE_ID_STRING_NoState));

	    if(methodEmun.EnumMethod(TIID_FileBrowser_DataType_FileDialog, 0, TIID_SYS_Method_Open) <= 0)  // >0 ��ʾ�ҵ�����
	    {
	        TCoUninitialize();
	        return -1;
	    }
	    if(methodEmun.GetDataTypeInterface(0, TCOM_CLSCTX_INPROC_SERVER, (LPVOID *)&pFileDialog) >= 0)
	    {
	        // ׼������
	       pFileDialog->SetDialogTitle( (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_SavePhotoWhere) );
		pFileDialog->SetExtFilter((TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_PhotoFormat));
		pFileDialog->SetOpenType(FMB_FILE_SELECT_OPEN_MODE_FILE_FLODER_ONLY);
	       hr = pFileDialog->QueryInterface(TIID_SYS_Method_Open, (LPVOID *)&pOpen);
	       if(!TCOM_S_SUCCESS(hr))
	       {
	           //������
	           pFileDialog->Release();
	           TCoUninitialize();
	           return -1;
	       }
	       pOpen->Invoke();
	       pOpen->Release();


	        // ��ѯ�û�ѡ���� 
	        if (pFileDialog->GetFilePathName() != NULL)
	        {
	        	TUChar pFileName[EOS_FILE_MAX_FNAME];
			TUChar pExt[10];
			TUChar pDstFileName[EOS_FILE_MAX_PATH];
			EOS_ParsePath(NULL, pFileName, pExt, pszFilePath);

			TUString::StrCopy(pDstFileName, pFileDialog->GetFilePathName() );
			TUString::StrCat(pDstFileName, pFileName);
			TUString::StrCat(pDstFileName, pExt);

			Handle hFile = NULL;
			hFile = EOS_Open((TUChar*)pszFilePath,  PO_RDONLY|PO_BINARY, PS_IREAD);
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

						//ɾ�������ļ�
						if (EOS_IsFileExist(pDstFileName))
							EOS_Delete(pDstFileName);	
						
						 hFileDes = EOS_Open(pDstFileName, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
						 if(hFileDes > 0)
						 {
							EOS_Write(hFileDes, pBuf, nlength);
							EOS_Close(hFileDes);

							pApp->MessageBox(TResource::LoadConstString(APP_RE_ID_STRING_SaveSuccess),TResource::LoadConstString(APP_RE_ID_STRING_Info),WMB_OK);
						 }
					}
					EOS_Close(hFile);

					free(pBuf);
				}       

			}
	        }

	        pFileDialog->Release(); // �ͷ��������Դ
	    }
	    TCoFreeUnusedLibraries();
	    TCoUninitialize();

	return Ret;							
}



/**
 * log��ʼ�� 
 *
 * \return 
 */
Int32 RenRen_Log_Init(void)
{
	int iRet = eFailed;
	Handle hFile = NULL;		
	TUChar pszTempPath[EOS_FILE_MAX_PATH] ={0};
	TUChar pszFilePath[EOS_FILE_MAX_PATH] = {0};    // �ļ�·�� 
	
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);
	TUString::StrGBToUnicode(pszTempPath,(Char*)"log.txt");
	TUString::StrCat(pszFilePath,pszTempPath);
	
	if (EOS_IsFileExist(pszFilePath))	
		EOS_Delete(pszFilePath);

	return iRet;
}
	
/**
 * log��� 
 *
 * \param logTxt
 *
 * \return 
 */
Int32 RenRen_Log(char* logTxt)
{
	int iRet = eFailed;
	Handle hFile = NULL;		
	TUChar pszTempPath[EOS_FILE_MAX_PATH] ={0};
	TUChar pszFilePath[EOS_FILE_MAX_PATH] = {0};    // �ļ�·�� 
	
	SS_GetApplicationPath(AppName_App_RenRen, SS_APP_PATH_TYPE_CACHE, pszFilePath);
	TUString::StrGBToUnicode(pszTempPath,(Char*)"log.txt");
	TUString::StrCat(pszFilePath,pszTempPath);
	
	if (EOS_IsFileExist(pszFilePath))			
		hFile = EOS_Open(pszFilePath, PO_BINARY|PO_RDWR|PO_APPEND, PS_IWRITE);
	else
		hFile = EOS_Open(pszFilePath, PO_TRUNC|PO_BINARY|PO_CREAT|PO_RDWR, PS_IWRITE);
		
	if (hFile < 0)
	{
		return eFailed;
	}	                    		

	EOS_Write(hFile, (void*)logTxt, strlen(logTxt)*sizeof(char));
	EOS_Write(hFile, (void*)"\n", strlen("\n")*sizeof(char));

	EOS_Close(hFile);

	return eSucceed;
}


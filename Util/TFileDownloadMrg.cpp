/*!
*@FileName: TFileDownMrg.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: ���ع���
*@Log: 	Author 			Date 		Description
*
*/

#include "TFileDownloadMrg.h"
#include "RenRenAPICommon.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type: TFileDownloadMrg����
// 1. Add/Insert�������������
// 2. DownLoadStart��ʼ����
// 3. ���ؽ�����(�ɹ���ʧ��),Ӧ�ñ������DownLoadComplete֪ͨTFileDownloadMrg�Ѿ����
// 4. ��������ﻹ������,���Զ���ʼ��һ����������
////////////////////////////////////////////////////////////////////////////////////////////////////

/** ���캯��
*
* \return ��
*/
TFileDownloadMgr::TFileDownloadMgr()
{
	//��֤������һ����һ��	
	pHeadNode = new  TAList;
	pHeadNode->pNext = NULL;
	pHeadNode->pTAListElement = NULL;

	//init the end node
	pEndNode = pHeadNode;

	nSize = 0;
	bDownLoad = FALSE;
}

/** ��������
*
* \return ��
*/
TFileDownloadMgr::~TFileDownloadMgr()
{
	TAList* pTemp = NULL;
	TAList* pDelete = NULL;
	
	for(pTemp = pHeadNode; pTemp != NULL; )
	{
		pDelete 	= pTemp;
		pTemp	= pDelete->pNext;
		
		if(pDelete->pTAListElement != NULL)
		{
			delete pDelete->pTAListElement;
			pDelete->pTAListElement = NULL;
		}
		delete pDelete;
	}
}

extern int RenRenAPI_Download(Int32 in_nAccessType, char* pURL, const TUChar* pszFilePath, Int32 in_nHwndId,Int32 in_nCtrlId, Int32 in_nParam);
/**
 * ���ؽ�������
 *
 *
 * \return 
 */
int TFileDownloadMgr::DownLoadStart(void)
{

	if(bDownLoad == FALSE)
	{
		if(nSize >0)
		{
			int iRet = eFailed;
			iRet = RenRenAPI_Download(pEndNode->pTAListElement->accessType, 
								pEndNode->pTAListElement->pURL, 
								pEndNode->pTAListElement->pszFilePath,
								pEndNode->pTAListElement->nHwndId,
								pEndNode->pTAListElement->nCtrlId,
								pEndNode->pTAListElement->nParam,
								pEndNode->pTAListElement->aFormData
								);

			//���Download����ʧ��,��ֱ���Ƴ�Node
			if(iRet == eFailed)
			{
				DownLoadComplete(pEndNode->pTAListElement);
			}
			else
				bDownLoad = TRUE;
		}
	}
	return eSucceed;
}

/**
 * ���ؽ���ֹͣ
 *
 *
 * \return 
 */
int TFileDownloadMgr::DownLoadStop(void)
{
	
	bDownLoad = FALSE;

	return eSucceed;
}

/**
 * ������������
 *
 * \param pElement
 *
 * \return 
 */
int TFileDownloadMgr::Add(TAListElement* pElement)
{
    TAList* pAddNote = NULL;
	
	pAddNote = new TAList;

    //add new node
	pAddNote->pTAListElement = new TAListElement;
      memcpy(pAddNote->pTAListElement, pElement, sizeof(TAListElement));
	pAddNote->pNext          = NULL;

	pEndNode->pNext   = pAddNote;

	//move the end pointer to the new end node.
	pEndNode          = pAddNote;
	++nSize;

	return eSucceed;
}

/**
 * �����������
 *
 * \param pElement
 * \param nPosition
 *
 * \return 
 */	
int TFileDownloadMgr::Insert(TAListElement* pElement, int nPosition)
{
	int	nCount  = 0;
	TAList* pFront  = NULL;
	TAList* pBack   = NULL;
	TAList* pInsert = NULL;

	if(NULL == pElement)
		return eFailed;

    //build the insert node!
	pInsert = new TAList;

	 pInsert->pTAListElement = new TAListElement;
	memcpy(pInsert->pTAListElement, pElement, sizeof(TAListElement));

	//���ֻ��һ��
	if(0 == nSize)
	{
		pHeadNode->pNext = pInsert;
		pInsert->pNext          = NULL;
		pEndNode         = pInsert;
		++nSize;
		return eSucceed;
	}

	//�������һ��
	if(nPosition > (nSize - 1))
	{
		pEndNode->pNext = pInsert;
		pInsert->pNext         = NULL;
		
		//move the end pointer to the new end node.
		pEndNode        = pInsert;

		++nSize;
		return eSucceed;
	}
 
	//find the position that will insert!
	for(pFront = pHeadNode; nCount < nPosition; nCount++, pFront = pFront->pNext);

	pBack          = pFront->pNext;

	//insert the node
	pFront->pNext  = pInsert;
	pInsert->pNext = pBack;
	++nSize;


	return eSucceed;
}


//eSucceed -- 
//eFailed -- ������û���ҵ���Ӧ�Ľڵ�, �������Ѿ���Cancel��
int TFileDownloadMgr::DownLoadComplete(TAListElement*  in_pElement)
{
	TAList* pFront  = NULL;
	TAList* pRemove = NULL;
	int iRect = eFailed;
	
	//if(nSize <1)
	//	return eFailed;

	pFront = pHeadNode;
	while(pFront->pNext != NULL)
	{
		if(pFront->pNext->pTAListElement == NULL)
			break;
		
		if( (pFront->pNext->pTAListElement->accessType == in_pElement->accessType) &&
		    (pFront->pNext->pTAListElement->nHwndId == in_pElement->nHwndId) &&
		    (pFront->pNext->pTAListElement->nCtrlId == in_pElement->nCtrlId) &&
		    (pFront->pNext->pTAListElement->nParam == in_pElement->nParam) )
		{
			pRemove = pFront->pNext;
			pFront->pNext = pFront->pNext->pNext;
			
			if(pEndNode == pRemove)
				pEndNode =  pFront;

			if(pRemove->pTAListElement != NULL)
			{
				delete pRemove->pTAListElement;
				pRemove->pTAListElement = NULL;
			}
			delete pRemove;
			--nSize;

			iRect = eSucceed;
			break;
		}
		pFront = pFront->pNext;
	};


	if(bDownLoad == TRUE)
	{
		if(nSize >0)
		{
			RenRenAPI_Download(pEndNode->pTAListElement->accessType, 
							pEndNode->pTAListElement->pURL, 
							pEndNode->pTAListElement->pszFilePath,
							pEndNode->pTAListElement->nHwndId,
							pEndNode->pTAListElement->nCtrlId,
							pEndNode->pTAListElement->nParam,
							pEndNode->pTAListElement->aFormData
							);
		}
		else
		{
			bDownLoad = FALSE;
		}
	}
	
	return iRect;
}

/**
 * ȡ���ض����͵���������
 *
 * \param accssType
 *
 * \return 
 */
int TFileDownloadMgr::CancelByAccessType(Int32  accessType)
{
	TAList* pFront  = NULL;
	TAList* pRemove = NULL;

	if(nSize <1)
		return eFailed;

	pFront = pHeadNode;
	while(pFront->pNext != NULL)
	{
		if(pFront->pNext->pTAListElement == NULL)
			break;
		
		if(pFront->pNext->pTAListElement->accessType == accessType)
		{
			pRemove = pFront->pNext;
			pFront->pNext = pFront->pNext->pNext;
			
			if(pEndNode == pRemove)
				pEndNode =  pFront;

			if(pRemove->pTAListElement != NULL)
			{
				delete pRemove->pTAListElement;
				pRemove->pTAListElement = NULL;
			}
			delete pRemove;
			--nSize;

			continue;
		}
		pFront = pFront->pNext;
	};

	return eSucceed;
}

/**
 * ȡ���ض�HwnId����������
 *
 * \param nHwdId
 *
 * \return 
 */
int TFileDownloadMgr::CancelByHwndId(Int32 nHwndId)
{
	TAList* pFront  = NULL;
	TAList* pRemove = NULL;

	if(nSize <1)
		return eFailed;

	pFront = pHeadNode;
	while(pFront->pNext != NULL)
	{
		if(pFront->pNext->pTAListElement == NULL)
			break;
		
		if(pFront->pNext->pTAListElement->nHwndId == nHwndId)
		{
			pRemove = pFront->pNext;
			pFront->pNext = pFront->pNext->pNext;
			
			if(pEndNode == pRemove)
				pEndNode =  pFront;

			if(pRemove->pTAListElement != NULL)
			{
				delete pRemove->pTAListElement;
				pRemove->pTAListElement = NULL;
			}
			delete pRemove;
			--nSize;

			continue;
		}
		pFront = pFront->pNext;
	};

	return eSucceed;
}

/**
 * ȡ��������������
 *
 *
 * \return 
 */
int TFileDownloadMgr::CancelAll(void)
{
    TAList* pRemove = NULL;
	TAList* pTemp   = NULL;
	if(nSize <1)
		return eFailed;
	
    for(pTemp = pHeadNode->pNext; pTemp != NULL; )
    {
		pRemove = pTemp;
		pTemp   = pRemove->pNext;

		if(pRemove->pTAListElement != NULL)
		{
			delete pRemove->pTAListElement;
			pRemove->pTAListElement = NULL;
		}
		delete pRemove;
    }

	pHeadNode->pNext = NULL;
	pEndNode         = pHeadNode;
	nSize            = 0;
	return eSucceed;
}


/*!
*@FileName: TFileDownMrg.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 下载管理
*@Log: 	Author 			Date 		Description
*
*/

#include "TFileDownloadMrg.h"
#include "RenRenAPICommon.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type: TFileDownloadMrg函数
// 1. Add/Insert来添加下载任务
// 2. DownLoadStart开始下载
// 3. 下载结束后(成功或失败),应用必须调用DownLoadComplete通知TFileDownloadMrg已经完成
// 4. 如果队列里还有任务,则自动开始下一个下载任务
////////////////////////////////////////////////////////////////////////////////////////////////////

/** 构造函数
*
* \return 无
*/
TFileDownloadMgr::TFileDownloadMgr()
{
	//保证链表中一定有一个	
	pHeadNode = new  TAList;
	pHeadNode->pNext = NULL;
	pHeadNode->pTAListElement = NULL;

	//init the end node
	pEndNode = pHeadNode;

	nSize = 0;
	bDownLoad = FALSE;
}

/** 析构函数
*
* \return 无
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
 * 下载进程启动
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

			//如果Download调用失败,则直接移除Node
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
 * 下载进程停止
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
 * 下载任务增加
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
 * 下载任务插入
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

	//如果只有一个
	if(0 == nSize)
	{
		pHeadNode->pNext = pInsert;
		pInsert->pNext          = NULL;
		pEndNode         = pInsert;
		++nSize;
		return eSucceed;
	}

	//大于最后一个
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
//eFailed -- 链表中没有找到对应的节点, 可能是已经被Cancel了
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
 * 取消特定类型的下载任务
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
 * 取消特定HwnId的下载任务
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
 * 取消所有下载任务
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


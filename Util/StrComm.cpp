/*!
*@FileName: StrComm.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 字符串操作函数
*@Log: 	Author 			Date 		Description
*
*/
#include "StrComm.h"



//Char复制,需要调用释放
char* StrDump(char* pszSrc)
{
	int	nLen = 0;
	char* pszDes = NULL;
	
	if( (pszSrc == NULL) || (*pszSrc == 0) )
		return NULL;

	nLen = strlen(pszSrc);
	pszDes = new char[nLen+1];
	if(pszDes == NULL)
		return NULL;
	
	MemSet(pszDes, 0, (nLen+1) * sizeof(char));
	strcpy(pszDes,pszSrc);
	return pszDes;
}

/***************************************************************************
##Method Name	  : StrReplaceAtoB
##Input parameters : 
	pSrc : 需要进行操作的字符串 
	size: 缓冲区大小(char 计数)
	strA/strB: 将strA替换成strB
##Return Value	: 替换后的字符串指针(同传入) 
##Description	 : 替换字符
******************************************************************************/
char* StrReplaceAtoB(char* pSrc, int size, char* strA, char* strB)
{
	int len = 0;
	int lenA = 0;
	int lenB = 0;
	int i = 0;
	char *tmpStr = NULL;
	char *pLocate = NULL;

	if( (pSrc == NULL) || (strA == NULL) || (strB == NULL) )
		return pSrc;

	len = strlen(pSrc);
	lenA = strlen(strA);
	lenB = strlen(strB);
	if( (len==0) || (lenA == 0) || (lenA > len))
		return pSrc;

	tmpStr = (char*)malloc(size * sizeof(char));
	MemSet(tmpStr, 0, size * sizeof(char));

	pLocate = &pSrc[0];
	for(i=0;i<len;i++)
	{
		if(strncmp(&pSrc[i], strA, lenA) == 0)
		{
			pSrc[i] = '\0';
			strcat(tmpStr, pLocate);
			strcat(tmpStr, strB);
			i = i+lenA;
			pLocate = &pSrc[i];
		}
	}
	strcat(tmpStr, pLocate);

	strcpy(pSrc, tmpStr);
	
	free(tmpStr);
	return pSrc;
}
//TUChar复制, 需要调用释放
TUChar* WStrDump(TUChar* pszSrc)
{
	int	nLen = 0;
	TUChar* pszDes = NULL;
	
	if( (pszSrc == NULL) || (*pszSrc == 0) )
		return NULL;

	nLen = TUString::StrLen(pszSrc);
	pszDes = new TUChar[nLen+1];
	if(pszDes == NULL)
		return NULL;
	
	MemSet(pszDes, 0, (nLen+1) * sizeof(TUChar));
	TUString::StrCopy(pszDes,pszSrc);
	return pszDes;
}


/******************************************************************************
##Method Name	  : StrRemoveAtoB
##Input parameters : 
	pSrc : 需要进行操作的字符串
	size: 缓冲区大小(char 计数)
	strA/strB: 截掉strA/strB之间的字符串
##Return Value: 替换后的字符串指针(同传入) 
##Description	 :截掉两个关键串间的字符串(包含关键串)
******************************************************************************/
char* StrRemoveAtoB(char* pSrc, int size, char* strA, char* strB)
{
	int len = 0;
	int lenA = 0;
	int lenB = 0;
	char *pStart = NULL;
	char *pEnd = NULL;
	char *tmpStr = NULL;

	if(pSrc == NULL)
	{
		return pSrc;
	}
	
	if( (strA == NULL) || (strB == NULL) )
	{
		return pSrc;
	}
	
	len = strlen(pSrc);
	lenA = strlen(strA);
	lenB = strlen(strB);
	if( (len==0)  || (lenA==0) || (lenB==0) || (lenA+lenB > len) )
	{
		return pSrc;
	}

	tmpStr = (char*)malloc(size * sizeof(char));
	MemSet(tmpStr, 0, size * sizeof(char));
	while(1)
	{
		pStart = strstr(pSrc, strA);
		if(pStart == NULL)
		{
			break;
		}

		pEnd = strstr(pStart+lenA, strB);
		if(pEnd == NULL)
		{
			break;
		}
		
		pEnd = pEnd+lenB;
		pStart[0] = '\0';
		MemSet(tmpStr, 0, size * sizeof(char));
		strcat(tmpStr, pSrc);
		strcat(tmpStr, &pEnd[0]);
		strcpy(pSrc, tmpStr);
	}

	free(tmpStr);
	return pSrc;
}

//double转成char
char* DoubleToChar(double n, char* c)
{
	TUChar wszC[64]={0};
	Char szC[64]={0};

	if(c == NULL)
		return NULL;
	
	TUString::StrI64ToA(wszC,(Int64)n);
	TUString::StrUnicodeToStrUtf8((Char*)c,wszC);	

	return c;
}

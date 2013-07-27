/*!
*@FileName: StrComm.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/
//StrComm.h
#ifndef  __StrComm_H__
#define  __StrComm_H__

#include "TG3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Char复制,需要调用释放
extern char* StrDump(char* pszSrc);
//替换字符
char* StrReplaceAtoB(char* pSrc, int size, char* strA, char* strB);
//TUChar复制, 需要调用释放
extern TUChar* WStrDump(TUChar* pszSrc);
//截掉两个关键串间的字符串(包含关键串)
extern char* StrRemoveAtoB(char* pSrc, int size, char* strA, char* strB);

extern char* DoubleToChar(double n, char* c);
#endif
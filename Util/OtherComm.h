/*!
*@FileName: OtherComm.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/
//OtherComm.h
#ifndef  __OtherComm_H__
#define  __OtherComm_H__

#include "TG3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���ļ��ж�ȡTBitmap, ��������Ҫ�ֶ��ͷ�
extern TBitmap* LoadImgByPath(const TUChar* pszPath);
extern TBitmap* LoadImgByPath(const char* sPath);

//url��������Ϊ%xx��ʽ; �ɵ������ͷ�
extern char* urlEncode(char* pszText);
#endif
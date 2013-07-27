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

//从文件中读取TBitmap, 调用者需要手动释放
extern TBitmap* LoadImgByPath(const TUChar* pszPath);
extern TBitmap* LoadImgByPath(const char* sPath);

//url参数编码为%xx格式; 由调用者释放
extern char* urlEncode(char* pszText);
#endif
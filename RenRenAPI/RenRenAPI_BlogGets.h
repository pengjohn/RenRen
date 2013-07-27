/*!
*@FileName: RenRenAPI_BlogGets.h
*@Author: zengying@GoZone
*@Date: 2010-12-24
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_BLOGGETS_H__
#define  __RENRENAPI_BLOGGETS_H__

#include "RenRenAPI.h"

typedef struct
{
	int id;
	char time[64 + 1];
	char cate[32 + 1];
	char title[128 + 1];
	char content[1024 + 1];
	int view_count;
	int comment_count;
	int visable;	// 4��intֵ: 99(������),4(���뱣��) ,1(����), -1(���Լ��ɼ�)
}BlogGets_blogs;

typedef struct
{
	int  nSize_blogs;  //Save the array size by code_gen tools. 
	BlogGets_blogs*  blogs;
	char uid[32+1];
	char total[32 + 1];
	char name[32 + 1];
}tResponseBlogGets;


extern void* RenRenAPI_JsonParse_BlogGets(char *text);
#endif
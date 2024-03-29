/*!
*@FileName: RenRenAPI_BlogGet.h
*@Author: zengying@GoZone
*@Date: 2010-12-27
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_BLOGGET_H__
#define  __RENRENAPI_BLOGGET_H__

#include "RenRenAPI.h"

typedef struct
{
	int uid;
	int id;
	int view_count;
	int comment_count;
	int visable;
	char title[256 + 1];
	char content[40960 + 1];
	char name[32 + 1];
	char headurl[128 + 1];
	char time[64 + 1];
}tResponseBlogGet;


extern void* RenRenAPI_JsonParse_BlogGet(char *text);
#endif

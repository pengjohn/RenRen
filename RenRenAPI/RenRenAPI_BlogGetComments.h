/*!
*@FileName: RenRenAPI_BlogGetComments.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-7
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_BLOGGETCOMMENTS_H__
#define  __RENRENAPI_BLOGGETCOMMENTS_H__

#include "RenRenAPI.h"

typedef struct
{
	int id;
	int uid;
	char name[64 + 1];
	char headurl[256 + 1];
	char time[64 + 1];
	char content[1024 + 1];
	char is_whisper[16];
}tResponseBlogGetCommentsItem;

typedef struct
{
	int	nArraySize;
	tResponseBlogGetCommentsItem*	Array;
}tResponseBlogGetComments;


extern void* RenRenAPI_JsonParse_BlogGetComments(char *text);
#endif

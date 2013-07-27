/*!
*@FileName: RenRenAPI_PhotosGetComments.h
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

#ifndef  __RENRENAPI_PHOTOSGETCOMMENTS_H__
#define  __RENRENAPI_PHOTOSGETCOMMENTS_H__

#include "RenRenAPI.h"

typedef struct
{
	int uid;
	char name[64 + 1];
	char headurl[256 + 1];
	int comment_id;
	char time[64 + 1];
	char text[1024 + 1];
	int is_whisper;
	char source[256 + 1];
}tResponsePhotosGetCommentsItem;

typedef struct
{
	int	nArraySize;
	tResponsePhotosGetCommentsItem*	Array;
}tResponsePhotosGetComments;


extern void* RenRenAPI_JsonParse_PhotosGetComments(char *text);
#endif
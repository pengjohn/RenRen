/*!
*@FileName: RenRenAPI_GuestbookPost.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-4
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_GUESTBOOKPOST_H__
#define  __RENRENAPI_GUESTBOOKPOST_H__

#include "RenRenAPI.h"

typedef struct
{
	int id;
}tResponseGuestbookPost;


extern void* RenRenAPI_JsonParse_GuestbookPost(char *text);
#endif
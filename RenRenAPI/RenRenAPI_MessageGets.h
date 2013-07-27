/*!
*@FileName: RenRenAPI_MessageGets.h
*@Author: pengzhixiong@GoZone
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

#ifndef  __RENRENAPI_MESSAGEGETS_H__
#define  __RENRENAPI_MESSAGEGETS_H__

#include "RenRenAPI.h"

typedef struct
{
	int id;
	int uid;
	char name[128 + 1];
	char head_url[256 + 1];
	char time[64 + 1];
	char title[128 + 1];
	char content[256 + 1];
	int is_read;
}MessageGets_messages;

typedef struct
{
	int unread_count;
	int count;
	int  nSize_messages;  //Save the array size by code_gen tools. 
	MessageGets_messages*  messages;
}tResponseMessageGets;


extern void* RenRenAPI_JsonParse_MessageGets(char *text);
#endif
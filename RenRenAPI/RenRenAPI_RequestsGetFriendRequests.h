/*!
*@FileName: RenRenAPI_RequestsGetFriendRequests.h
*@Author: zbingwen@GoZone
*@Date: 2010-12-29
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_REQUESTSGETFRIENDREQUESTS_H__
#define  __RENRENAPI_REQUESTSGETFRIENDREQUESTS_H__

#include "RenRenAPI.h"

typedef struct
{
	int uid;
	char headurl[256 + 1];
	char name[255 + 1];
	char network[256 + 1];
}RequestsGetFriendRequests_guest;

typedef struct
{
	int total;
	int  nSize_guest;  //Save the array size by code_gen tools. 
	RequestsGetFriendRequests_guest*  guest;
}tResponseRequestsGetFriendRequests;


extern void* RenRenAPI_JsonParse_RequestsGetFriendRequests(char *text);
#endif

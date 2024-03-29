/*!
*@FileName: RenRenAPI_InitialLogin.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-25
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by RenRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#ifndef  __RENRENAPI_INITIALLOGIN_H__
#define  __RENRENAPI_INITIALLOGIN_H__

#include "RenRenAPI.h"

typedef struct
{
	char session_key[64 + 1];
	int uid;
	int error_code;
	char error_msg[128 + 1];
	char ticket[64 + 1];
}tResponseInitialLogin;


extern void* RenRenAPI_JsonParse_InitialLogin(char *text);
#endif

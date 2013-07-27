/*!
*@FileName: RenRenAPI_UsersGetVisitors.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-22
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* The file is generated by ReeRen_Component Wizard for Tranzda Mobile Platform <br>
* =======================================================================<br>
*/

#include "RenRenAPICommon.h"

/**
 * Json�ļ�����
 *
 * \param text
 *
 * \return 
 */
void* RenRenAPI_JsonParse_UsersGetVisitors(char *text)
{
	cJSON *json;
	cJSON *pTemp0;
	tResponseUsersGetVisitors*  Response = new tResponseUsersGetVisitors;
	memset(Response, 0 , sizeof(tResponseUsersGetVisitors));

	json = cJSON_Parse(text);

	pTemp0 = cJSON_GetObjectItem(json, "visitors");
	if(pTemp0)
	{
		int nSize1 = 0, i = 0;
		nSize1 = cJSON_GetArraySize(pTemp0);
		Response->nSize_visitors = nSize1;
		if( nSize1 != 0 )
		{
			Response->visitors = NULL;
			Response->visitors = (UsersGetVisitors_visitors*) malloc(sizeof( UsersGetVisitors_visitors ) * nSize1 );
			memset(Response->visitors, 0 , sizeof(UsersGetVisitors_visitors) * nSize1 );
		}
		for ( i = 0; i < nSize1; i++ )
		{
			cJSON *Item1 = NULL, *pTemp1 = NULL;
			Item1 = cJSON_GetArrayItem(pTemp0,i);
			pTemp1 = cJSON_GetObjectItem(Item1, "uid");
			if(pTemp1)
			{
				Response->visitors[i].uid = pTemp1->valueint;
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "name");
			if(pTemp1)
			{
				STRCPY_Ex(Response->visitors[i].name, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "time");
			if(pTemp1)
			{
				STRCPY_Ex(Response->visitors[i].time, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "headurl");
			if(pTemp1)
			{
				STRCPY_Ex(Response->visitors[i].headurl, pTemp1->valuestring);
			}

		}
	}


	cJSON_Delete(json);
	return Response;
}
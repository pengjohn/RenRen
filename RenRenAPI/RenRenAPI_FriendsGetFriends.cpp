/*!
*@FileName: RenRenAPI_FriendsGetFriends.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-21
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

void* RenRenAPI_JsonParse_FriendsGetFriends(char *text)
{
	int i = 0;
	cJSON *jsonall;
	cJSON *json;
	cJSON *pTemp0;
	tResponseFriendsGetFriends*  Response = new tResponseFriendsGetFriends;
	memset(Response, 0 , sizeof(tResponseFriendsGetFriends));

	jsonall = cJSON_Parse(text);

	Response->nArraySize = cJSON_GetArraySize(jsonall);
	if(Response->nArraySize>0)
	{
		Response->Array = (tResponseFriendsGetFriendsItem*) malloc(sizeof( tResponseFriendsGetFriendsItem ) * Response->nArraySize );
		memset(Response->Array, 0, sizeof( tResponseFriendsGetFriendsItem ) * Response->nArraySize );
		for(i = 0; i<Response->nArraySize; i++)
		{
			json = cJSON_GetArrayItem(jsonall, i);

			pTemp0 = cJSON_GetObjectItem(json, "id");
			if(pTemp0)
			{
				Response->Array[i].id = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "name");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].name, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "headurl");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].headurl, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "tinyurl");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].tinyurl, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "tinyurl_with_logo");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].tinyurl_with_logo, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "headurl_with_logo");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].headurl_with_logo, pTemp0->valuestring);
			}
		}
	}

	cJSON_Delete(jsonall);
	return Response;
}
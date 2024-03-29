/*!
*@FileName: RenRenAPI_FriendsAreFriends.cpp
*@Author: lee-5959f8a390f@GoZone
*@Date: 2011-6-16
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
void* RenRenAPI_JsonParse_FriendsAreFriends(char *text)
{
	int i = 0;
	cJSON *jsonall;
	cJSON *json;
	cJSON *pTemp0;
	tResponseFriendsAreFriends*  Response = new tResponseFriendsAreFriends;
	memset(Response, 0 , sizeof(tResponseFriendsAreFriends));

	jsonall = cJSON_Parse(text);

	Response->nArraySize = cJSON_GetArraySize(jsonall);
	if(Response->nArraySize>0)
	{
		Response->Array = (tResponseFriendsAreFriendsItem*) malloc(sizeof( tResponseFriendsAreFriendsItem) * Response->nArraySize );
		memset(Response->Array, 0 , sizeof(tResponseFriendsAreFriendsItem) * Response->nArraySize);
		for(i = 0; i<Response->nArraySize; i++)
		{
			json = cJSON_GetArrayItem(jsonall, i);

			pTemp0 = cJSON_GetObjectItem(json, "uid1");
			if(pTemp0)
			{
				Response->Array[i].uid1 = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "uid2");
			if(pTemp0)
			{
				Response->Array[i].uid2 = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "are_friends");
			if(pTemp0)
			{
				Response->Array[i].are_friends = pTemp0->valueint;
			}



		}
	}
	cJSON_Delete(jsonall);
	return Response;
}

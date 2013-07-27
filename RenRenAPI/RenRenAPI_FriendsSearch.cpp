/*!
*@FileName: RenRenAPI_FriendsSearch.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-2-25
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
void* RenRenAPI_JsonParse_FriendsSearch(char *text)
{
	cJSON *json;
	cJSON *pTemp0;
	tResponseFriendsSearch*  Response = new tResponseFriendsSearch;
	memset(Response, 0 , sizeof(tResponseFriendsSearch));

	json = cJSON_Parse(text);

	pTemp0 = cJSON_GetObjectItem(json, "total");
	if(pTemp0)
	{
		Response->total = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "friends");
	if(pTemp0)
	{
		int nSize1 = 0, i = 0;
		nSize1 = cJSON_GetArraySize(pTemp0);
		Response->nSize_friends = nSize1;
		if( nSize1 != 0 )
		{
			Response->friends = NULL;
			Response->friends = (FriendsSearch_friends*) malloc(sizeof( FriendsSearch_friends ) * nSize1 );
			memset(Response->friends, 0 , sizeof(FriendsSearch_friends) * nSize1 );
		}
		for ( i = 0; i < nSize1; i++ )
		{
			cJSON *Item1 = NULL, *pTemp1 = NULL;
			Item1 = cJSON_GetArrayItem(pTemp0,i);
			pTemp1 = cJSON_GetObjectItem(Item1, "id");
			if(pTemp1)
			{
				Response->friends[i].id = pTemp1->valueint;
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "name");
			if(pTemp1)
			{
				STRCPY_Ex(Response->friends[i].name, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "tinyurl");
			if(pTemp1)
			{
				STRCPY_Ex(Response->friends[i].tinyurl, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "info");
			if(pTemp1)
			{
				STRCPY_Ex(Response->friends[i].info, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "isFriend");
			if(pTemp1)
			{
				Response->friends[i].isFriend = pTemp1->valueint;
			}

		}
	}


	cJSON_Delete(json);
	return Response;
}
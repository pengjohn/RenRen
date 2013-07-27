/*!
*@FileName: RenRenAPI_Profile.cpp
*@Author: zengying@GoZone
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
void* RenRenAPI_JsonParse_Profile(char *text)
{
	cJSON *json;
	cJSON *pTemp0;
	tResponseProfile*  Response = new tResponseProfile;
	memset(Response, 0 , sizeof(tResponseProfile));

	json = cJSON_Parse(text);

	pTemp0 = cJSON_GetObjectItem(json, "uid");
	if(pTemp0)
	{
		Response->uid = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "name");
	if(pTemp0)
	{
		STRCPY_Ex(Response->name, pTemp0->valuestring);
	}
	pTemp0 = cJSON_GetObjectItem(json, "headurl");
	if(pTemp0)
	{
		STRCPY_Ex(Response->headurl, pTemp0->valuestring);
	}
	pTemp0 = cJSON_GetObjectItem(json, "star");
	if(pTemp0)
	{
		Response->star = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "network_name");
	if(pTemp0)
	{
		STRCPY_Ex(Response->network_name, pTemp0->valuestring);
	}
	pTemp0 = cJSON_GetObjectItem(json, "base_info");
	if(pTemp0)
	{
		int nSize1 = 0, i = 0;
		nSize1 = cJSON_GetArraySize(pTemp0);
		Response->nSize_base_info = nSize1;
		if( nSize1 != 0 )
		{
			Response->base_info = NULL;
			Response->base_info = (Profile_base_info*) malloc(sizeof( Profile_base_info ) * nSize1 );
			memset(Response->base_info, 0 , sizeof(Profile_base_info) * nSize1 );
		}
		for ( i = 0; i < nSize1; i++ )
		{
			cJSON *Item1 = NULL, *pTemp1 = NULL;
			Item1 = cJSON_GetArrayItem(pTemp0,i);
			pTemp1 = cJSON_GetObjectItem(Item1, "gender");
			if(pTemp1)
			{
				STRCPY_Ex(Response->base_info[i].gender, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "birth");
			if(pTemp1)
			{
				int nSize2 = 0, j = 0;
				nSize2 = cJSON_GetArraySize(pTemp1);
				Response->base_info[i].nSize_birth = nSize2;
				if( nSize2 != 0 )
				{
					Response->base_info[i].birth = NULL;
					Response->base_info[i].birth = (Profile_birth*) malloc(sizeof( Profile_birth ) * nSize2 );
					memset(Response->base_info[i].birth, 0 , sizeof(Profile_birth) * nSize2 );
				}
				for ( j = 0; j < nSize2; j++ )
				{
					cJSON *Item2 = NULL, *pTemp2 = NULL;
					Item2 = cJSON_GetArrayItem(pTemp1,j);
					pTemp2 = cJSON_GetObjectItem(Item2, "birth_year");
					if(pTemp2)
					{
						STRCPY_Ex(Response->base_info[i].birth[j].birth_year, pTemp2->valuestring);
					}
					pTemp2 = cJSON_GetObjectItem(Item2, "birth_month");
					if(pTemp2)
					{
						Response->base_info[i].birth[j].birth_month = pTemp2->valueint;
					}
					pTemp2 = cJSON_GetObjectItem(Item2, "birth_day");
					if(pTemp2)
					{
						Response->base_info[i].birth[j].birth_day = pTemp2->valueint;
					}

				}
			}			pTemp1 = cJSON_GetObjectItem(Item1, "hometown");
			if(pTemp1)
			{
				int nSize2 = 0, j = 0;
				nSize2 = cJSON_GetArraySize(pTemp1);
				Response->base_info[i].nSize_hometown = nSize2;
				if( nSize2 != 0 )
				{
					Response->base_info[i].hometown = NULL;
					Response->base_info[i].hometown = (Profile_hometown*) malloc(sizeof( Profile_hometown ) * nSize2 );
					memset(Response->base_info[i].hometown, 0 , sizeof(Profile_hometown) * nSize2 );
				}
				for ( j = 0; j < nSize2; j++ )
				{
					cJSON *Item2 = NULL, *pTemp2 = NULL;
					Item2 = cJSON_GetArrayItem(pTemp1,j);
					pTemp2 = cJSON_GetObjectItem(Item2, "province");
					if(pTemp2)
					{
						STRCPY_Ex(Response->base_info[i].hometown[j].province, pTemp2->valuestring);
					}
					pTemp2 = cJSON_GetObjectItem(Item2, "city");
					if(pTemp2)
					{
						STRCPY_Ex(Response->base_info[i].hometown[j].city, pTemp2->valuestring);
					}

				}
			}
		}
	}	pTemp0 = cJSON_GetObjectItem(json, "status");
	if(pTemp0)
	{
		int nSize1 = 0, i = 0;
		nSize1 = cJSON_GetArraySize(pTemp0);
		Response->nSize_status = nSize1;
		if( nSize1 != 0 )
		{
			Response->status = NULL;
			Response->status = (Profile_status*) malloc(sizeof( Profile_status ) * nSize1 );
			memset(Response->status, 0 , sizeof(Profile_status) * nSize1 );
		}
		for ( i = 0; i < nSize1; i++ )
		{
			cJSON *Item1 = NULL, *pTemp1 = NULL;
			Item1 = cJSON_GetArrayItem(pTemp0,i);
			pTemp1 = cJSON_GetObjectItem(Item1, "content");
			if(pTemp1)
			{
				STRCPY_Ex(Response->status[i].content, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "time");
			if(pTemp1)
			{
				STRCPY_Ex(Response->status[i].time, pTemp1->valuestring);
			}

		}
	}	pTemp0 = cJSON_GetObjectItem(json, "visitors_count");
	if(pTemp0)
	{
		Response->visitors_count = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "blogs_count");
	if(pTemp0)
	{
		Response->blogs_count = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "albums_count");
	if(pTemp0)
	{
		Response->albums_count = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "friends_count");
	if(pTemp0)
	{
		Response->friends_count = pTemp0->valueint;
	}
	pTemp0 = cJSON_GetObjectItem(json, "guestbook_count");
	if(pTemp0)
	{
		Response->guestbook_count = pTemp0->valueint;
	}



	cJSON_Delete(json);
	return Response;
}
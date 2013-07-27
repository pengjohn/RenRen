/*!
*@FileName: RenRenAPI_MessageGet.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2010-12-24
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
void* RenRenAPI_JsonParse_MessageGet(char *text)
{
	cJSON *json;
	cJSON *pTemp0;
	tResponseMessageGet*  Response = new tResponseMessageGet;
	memset(Response, 0 , sizeof(tResponseMessageGet));

	json = cJSON_Parse(text);

	pTemp0 = cJSON_GetObjectItem(json, "message");
	if(pTemp0)
	{
		int nSize1 = 0, i = 0;
		nSize1 = cJSON_GetArraySize(pTemp0);
		Response->nSize_message = nSize1;
		if( nSize1 != 0 )
		{
			Response->message = NULL;
			Response->message = (MessageGet_message*) malloc(sizeof( MessageGet_message ) * nSize1 );
			memset(Response->message, 0 , sizeof(MessageGet_message) * nSize1 );
		}
		for ( i = 0; i < nSize1; i++ )
		{
			cJSON *Item1 = NULL, *pTemp1 = NULL;
			Item1 = cJSON_GetArrayItem(pTemp0,i);
			pTemp1 = cJSON_GetObjectItem(Item1, "uid");
			if(pTemp1)
			{
				Response->message[i].uid = pTemp1->valueint;
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "name");
			if(pTemp1)
			{
				STRCPY_Ex(Response->message[i].name, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "head_url");
			if(pTemp1)
			{
				STRCPY_Ex(Response->message[i].head_url, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "time");
			if(pTemp1)
			{
				STRCPY_Ex(Response->message[i].time, pTemp1->valuestring);
			}
			pTemp1 = cJSON_GetObjectItem(Item1, "content");
			if(pTemp1)
			{
				STRCPY_Ex(Response->message[i].content, pTemp1->valuestring);
			}

		}
	}


	cJSON_Delete(json);
	return Response;
}
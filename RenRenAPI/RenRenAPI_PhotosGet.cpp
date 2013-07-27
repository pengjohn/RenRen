/*!
*@FileName: RenRenAPI_PhotosGet.cpp
*@Author: xuhonglei@GoZone
*@Date: 2010-12-30
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
void* RenRenAPI_JsonParse_PhotosGet(char *text)
{
	int i = 0;
	cJSON *jsonall;
	cJSON *json;
	cJSON *pTemp0;
	tResponsePhotosGet*  Response = new tResponsePhotosGet;
	memset(Response, 0 , sizeof(tResponsePhotosGet));

	jsonall = cJSON_Parse(text);

	Response->nArraySize = cJSON_GetArraySize(jsonall);
	if(Response->nArraySize>0)
	{
		Response->Array = (tResponsePhotosGetItem*) malloc(sizeof( tResponsePhotosGetItem) * Response->nArraySize );
		for(i = 0; i<Response->nArraySize; i++)
		{
			json = cJSON_GetArrayItem(jsonall, i);

			pTemp0 = cJSON_GetObjectItem(json, "pid");
			if(pTemp0)
			{
				Response->Array[i].pid = pTemp0->valuedouble;
			}
			pTemp0 = cJSON_GetObjectItem(json, "aid");
			if(pTemp0)
			{
				Response->Array[i].aid = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "uid");
			if(pTemp0)
			{
				Response->Array[i].uid = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "url_tiny");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].url_tiny, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "url_head");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].url_head, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "url_large");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].url_large, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "caption");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].caption, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "time");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].time, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "view_count");
			if(pTemp0)
			{
				Response->Array[i].view_count = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "comment_count");
			if(pTemp0)
			{
				Response->Array[i].comment_count = pTemp0->valueint;
			}



		}
	}
	cJSON_Delete(jsonall);
	return Response;
}

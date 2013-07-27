/*!
*@FileName: RenRenAPI_PhotosGetComments.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-7
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
void* RenRenAPI_JsonParse_PhotosGetComments(char *text)
{
	int i = 0;
	cJSON *jsonall;
	cJSON *json;
	cJSON *pTemp0;
	tResponsePhotosGetComments*  Response = new tResponsePhotosGetComments;
	memset(Response, 0 , sizeof(tResponsePhotosGetComments));

	jsonall = cJSON_Parse(text);

	Response->nArraySize = cJSON_GetArraySize(jsonall);
	if(Response->nArraySize>0)
	{
		Response->Array = (tResponsePhotosGetCommentsItem*) malloc(sizeof( tResponsePhotosGetCommentsItem) * Response->nArraySize );
		memset(Response->Array, 0 , sizeof(tResponsePhotosGetCommentsItem) * Response->nArraySize);
		for(i = 0; i<Response->nArraySize; i++)
		{
			json = cJSON_GetArrayItem(jsonall, i);

			pTemp0 = cJSON_GetObjectItem(json, "uid");
			if(pTemp0)
			{
				Response->Array[i].uid = pTemp0->valueint;
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
			pTemp0 = cJSON_GetObjectItem(json, "comment_id");
			if(pTemp0)
			{
				Response->Array[i].comment_id = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "time");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].time, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "text");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].text, pTemp0->valuestring);
			}
			pTemp0 = cJSON_GetObjectItem(json, "is_whisper");
			if(pTemp0)
			{
				Response->Array[i].is_whisper = pTemp0->valueint;
			}
			pTemp0 = cJSON_GetObjectItem(json, "source");
			if(pTemp0)
			{
				STRCPY_Ex(Response->Array[i].source, pTemp0->valuestring);
			}



		}
	}
	cJSON_Delete(jsonall);
	return Response;
}

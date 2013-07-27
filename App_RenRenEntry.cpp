// Application main file.
 
// Original file name: App_RenRenEntry.cpp
// Generated by TOPS Builder:Project wizard,Date:2010-12-13
 

 
#include  "App_RenRenEntry.h"
#include  "App_RenRenApp.h"
#include "TResolution.h"     
#if defined(UPHONE_HVGA)     
#include "Res_HVGA/app_renren_res_c.h"     
#elif defined(UPHONE_WVGA)     
#include "Res_WVGA/app_renren_res_c.h"     
#else     
#error resolution     
#endif 

const  ResourceRegisterEntry		ResRegList_App_RenRen[]  =
{
	TG_RESOURCE_DEFINE
};

const  AppResourceEntry		App_RenRenResourceEntry  =
{
	(ResourceRegisterEntry*)ResRegList_App_RenRen,                    //  res  list  in  this  app
		sizeof(ResRegList_App_RenRen) /  sizeof(ResourceRegisterEntry),    //number  of  item  in  res
};

Int32 TG3AppMain(const TUChar * pAppID, UInt32 nCmd, void * pCmdParam)
{
	switch(nCmd)
	{
	case 0: // 主入口
		{
			// UI应用程序例子：
			TApp_RenRenApp  *  pApp= new  TApp_RenRenApp();
			pApp->WM_SetResourceEntry(&App_RenRenResourceEntry);
			pApp->Run();

			delete pApp;
			break;
		}
	}
	return 1;
}



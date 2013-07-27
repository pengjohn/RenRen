#define       UPHONE_HVGA                  ///<  分辨率为HVGA 
//#define UPHONE_WVGA                 ///<  分辨率为WVGA 
   
#if defined(UPHONE_HVGA)  
#include "Res_HVGA/app_renren_res_def.h"  
#elif defined(UPHONE_WVGA)  
#include "Res_WVGA/app_renren_res_def.h"  
#else  
#error resolution  
#endif 
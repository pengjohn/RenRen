/*!
*@FileName: CommonMacro.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/
//#include "CommonMacro.h"
#ifndef  __CommonMacro_H__
#define  __CommonMacro_H__

#define ARR_SIZE(a)  (sizeof((a))/sizeof((a[0])))

//des一定要是数组,不能是指针
#define STRCPY_Ex(des, soure)\
			do\
			{\
				memset(des, 0, sizeof(des));\
				if(soure == NULL)	\
					strcpy(des, "");\
				else	\
					strncpy(des, soure, sizeof(des)-1);\
			}\
			while(0)

#define FREEIF(p)     if (p) { free((void*)p); (p) = 0; }
enum
{
	eSucceed = 1,
	eFailed = -1,
};


//#define assert(bCondition) if (!(bCondition)) *(int*)NULL=0
#define assert(bCondition)

#endif


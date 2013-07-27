/*!
*@FileName: cUnixTime.h
*@Author: Huxianxiang@GoZone
*@Date:
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* =======================================================================<br>
*/

#ifndef cUnixTime__h
#define cUnixTime__h

#include "ssTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TIME_UNIXTIME_QUAD_YEAR        (366+(3*365))
#define TIME_UNIXTIME_BASE_YEAR        (1980)


typedef struct
{
	UInt16 Year;            /* Year [1970..XXXX]                            */
	UInt16 Month;           /* Month of year [1..12]                        */
	UInt16 Day;             /* Day of month [1..31] */
	UInt16 Hour;            /* Hour of day [0..23]                          */
	UInt16 Minute;          /* Minute of hour [0..59]                       */
	UInt16 Second;          /* Second of minute [0..59]                   */
	UInt16 Dayofweek;       /* Day of week [0..6]                */	
}TDateAndTime;

typedef enum
{
	KX_TIME_FORMAT_MM_DD = 0,		/*MM-DD*/
	KX_TIME_FORMAT_YY_MM_DD,		/*YY-MM-DD*/
	KX_TIME_FORMAT_MM_DD_HH_MM,		/*MM-DD HH:MM*/
	KX_TIME_FORMAT_YY_MM_DD_HH_MM,	/*YY-MM-DD HH:MM*/
} TKX_TIME_FORMAT;

extern void cUnixTime_ConvertUnixTimeToDateTimeAndWeek( UInt32 UTime, TDateAndTime* pDateAndTime );
extern unsigned long cUnixTime_ConvertDateTimeAndWeekToUnixTime( UInt32* pUTime, TDateAndTime* pDateAndTime );
extern TUChar* cUnixTime_ConvertUnixTimeToFormatString( UInt32 UTime, TKX_TIME_FORMAT nFormat, TUChar** ppString );

#ifdef __cplusplus
}
#endif

#endif

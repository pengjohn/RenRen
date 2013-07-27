/*!
*@FileName: cUnixTime.c
*@Author: Huxianxiang@GoZone
*@Date: 2011-1-10
*@Brief: 时间显示格式转换
*@Log: 	Author 			Date 		Description
*
*@section Copyright
* =======================================================================<br>
*          Copyright ? 2010-2012 GOZONE <br>
*                All Rights Reserved.<br>
* =======================================================================<br>
*/

#include "cUnixTime.h"
#include "RenRenAPICommon.h"

static const UINT16 year_tab[] = {
  0,                              /* Year 0 (leap year) */
  366,                            /* Year 1             */
  366+365,                        /* Year 2             */
  366+365+365,                    /* Year 3             */
  366+365+365+365                 /* Bracket year       */
};

static const UINT16 norm_month_tab[] = {
  0,                                    /* --- */
  31,                                   /* Jan */
  31+28,                                /* Feb */
  31+28+31,                             /* Mar */
  31+28+31+30,                          /* Apr */
  31+28+31+30+31,                       /* May */
  31+28+31+30+31+30,                    /* Jun */
  31+28+31+30+31+30+31,                 /* Jul */
  31+28+31+30+31+30+31+31,              /* Aug */
  31+28+31+30+31+30+31+31+30,           /* Sep */
  31+28+31+30+31+30+31+31+30+31,        /* Oct */
  31+28+31+30+31+30+31+31+30+31+30,     /* Nov */
  31+28+31+30+31+30+31+31+30+31+30+31   /* Dec */
};

static const UINT16 leap_month_tab[] = {
  0,                                    /* --- */
  31,                                   /* Jan */
  31+29,                                /* Feb */
  31+29+31,                             /* Mar */
  31+29+31+30,                          /* Apr */
  31+29+31+30+31,                       /* May */
  31+29+31+30+31+30,                    /* Jun */
  31+29+31+30+31+30+31,                 /* Jul */
  31+29+31+30+31+30+31+31,              /* Aug */
  31+29+31+30+31+30+31+31+30,           /* Sep */
  31+29+31+30+31+30+31+31+30+31,        /* Oct */
  31+29+31+30+31+30+31+31+30+31+30,     /* Nov */
  31+29+31+30+31+30+31+31+30+31+30+31   /* Dec */
};

static const UINT16 day_offset[] = {
  1,                                    /* Year 0 (leap year) */
  1+2,                                  /* Year 1             */
  1+2+1,                                /* Year 2             */
  1+2+1+1                               /* Year 3             */
};

/**
 * 转换Unix时间到结构体中
 *
 * \param UTime
 * \param pDateAndTime
 *
 * \return 
 */
void cUnixTime_ConvertUnixTimeToDateTimeAndWeek( UInt32 UTime, TDateAndTime* pDateAndTime )
{
	int i = 0;
	unsigned short 	days;
	unsigned int 	quad_years;
	const unsigned short *month_table;

	if( UTime < 315504000 || pDateAndTime == NULL )
	{
		return;
	}
		
	UTime = UTime - 315504000; //since 1980.1.1 00:00:00
	
	pDateAndTime->Second = UTime % 60;
	UTime = UTime/60;

	pDateAndTime->Minute = UTime % 60;
	UTime = UTime/60;

	pDateAndTime->Hour = UTime % 24;
	UTime = UTime/24;

	days = UTime % TIME_UNIXTIME_QUAD_YEAR;
	quad_years = UTime/TIME_UNIXTIME_QUAD_YEAR;
	pDateAndTime->Year = TIME_UNIXTIME_BASE_YEAR + (4 * quad_years);

  	for ( i = 0; days >= year_tab[ i + 1 ]; i++ )
  	{
		/* No-op. Table seach. */
  	}
	days = days - year_tab[ i ];
  	pDateAndTime->Year = pDateAndTime->Year + i;  
	pDateAndTime->Dayofweek = (day_offset[3] * quad_years + day_offset[i] + days) % 7;

	month_table = (i == 0) ? leap_month_tab : norm_month_tab;
	for ( i = 0; days >= month_table[ i + 1 ]; i++ )
	{
		/* No-op. Table seach. */
	}

	pDateAndTime->Day = days - month_table[ i ] + 1;  
	pDateAndTime->Month = i + 1;
	
}

/**
 * 转换结构体为Unix时间
 *
 * \param UTime
 * \param pDateAndTime
 *
 * \return 
 */
unsigned long cUnixTime_ConvertDateTimeAndWeekToUnixTime( UInt32* pUTime, TDateAndTime* pDateAndTime )
{
	unsigned long time = 0;
	
	if( pUTime == NULL || pDateAndTime == NULL )
	{
		return 0;
	}

	time = ( ( pDateAndTime->Year - TIME_UNIXTIME_BASE_YEAR) / 4 ) * TIME_UNIXTIME_QUAD_YEAR;
	if ( (pDateAndTime->Year & 0x3) == 0 )
	{
		time += leap_month_tab[ pDateAndTime->Month - 1 ];
	}
	else
	{
		time += year_tab[ pDateAndTime->Year & 0x3 ];
		time += norm_month_tab[ pDateAndTime->Month - 1 ];
	}

	time += pDateAndTime->Day - 1;

	time = time * 24  +  pDateAndTime->Hour;
	time = time * 60  +  pDateAndTime->Minute;
	time = time * 60  +  pDateAndTime->Second;

	time += 315504000;
	*pUTime = time;
	return time;
}

/**
    @brief  按照格式将Unix 时间转换成字符串输出
    @注意：请自行执行释放操作
    @ppString  [out]  目的字符串。
    @nFormat  格式化控制串。
    @return 转换结果字符串。
*/
TUChar* cUnixTime_ConvertUnixTimeToFormatString( UInt32 UTime, TKX_TIME_FORMAT nFormat, TUChar** ppString )
{
	TUChar* 		pFormatStr = NULL;
	TDateAndTime LocalTime = {0};
	Int32 		nSize = 0;
	
	if( ppString == NULL || *ppString != NULL )
	{
		return NULL;
	}

	cUnixTime_ConvertUnixTimeToDateTimeAndWeek(UTime,&LocalTime);

	switch(nFormat)
	{
		case KX_TIME_FORMAT_MM_DD:	/*MM-DD*/
			{
				pFormatStr = (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_UTimeFormat_MM_DD);
				nSize = TUString::StrLen(pFormatStr) + 2 + 1 + 2 + 1;
				*ppString = new TUChar[nSize];
				memset(*ppString, 0 , sizeof(TUChar) * nSize );
				TUString::StrPrintF( *ppString, pFormatStr, LocalTime.Month, LocalTime.Day );
			}
			break;

		case KX_TIME_FORMAT_YY_MM_DD: 	/*YY-MM-DD*/
			{
				pFormatStr = (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_UTimeFormat_YY_MM_DD);
				nSize = TUString::StrLen(pFormatStr) + 4 + 1 + 2 + 1 + 2 + 1;
				*ppString = new TUChar[nSize];
				memset(*ppString, 0 , sizeof(TUChar) * nSize );
				TUString::StrPrintF( *ppString, pFormatStr, LocalTime.Year, LocalTime.Month, LocalTime.Day );
			}
			break;

		case KX_TIME_FORMAT_MM_DD_HH_MM:	/*MM-DD HH:MM*/
			{
				pFormatStr = (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_UTimeFormat_MM_DD_HH_MM);
				nSize = TUString::StrLen(pFormatStr) + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1;
				*ppString = new TUChar[nSize];
				memset(*ppString, 0 , sizeof(TUChar) * nSize );
				TUString::StrPrintF( *ppString, pFormatStr, LocalTime.Month, LocalTime.Day, LocalTime.Hour/10, LocalTime.Hour%10, LocalTime.Minute/10, LocalTime.Minute%10 );
			}
			break;

		case KX_TIME_FORMAT_YY_MM_DD_HH_MM:	/*YY-MM-DD HH:MM*/
			{
				pFormatStr = (TUChar*)TResource::LoadConstString(APP_RE_ID_STRING_UTimeFormat_YY_MM_DD_HH_MM);
				nSize = TUString::StrLen(pFormatStr) + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1;
				*ppString = new TUChar[nSize];
				memset(*ppString, 0 , sizeof(TUChar) * nSize );
				TUString::StrPrintF( *ppString, pFormatStr, LocalTime.Year, LocalTime.Month, LocalTime.Day, LocalTime.Hour/10, LocalTime.Hour%10, LocalTime.Minute/10, LocalTime.Minute%10 );
			}
			break;

		default:
			break;		
	}
	return *ppString;
}

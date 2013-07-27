
/*!
* 
* \file 
* \author HuangYin
* \date	  2010.3.10
* \brief  全局变量、函数、类型和公用宏定义
* 
* \ref    CopyRight
*/

#ifndef __TGLOBAL_H__
#define __TGLOBAL_H__

#include "TG3.h"
#include <string>
#include <list>

typedef UInt32 STATUS_ID;

/** 一次显示的条目个数 */
#define  ITEM_COUNTS_PAGE 20

/** _T_stricmp宏，不分大小写的字符串比较 */
#ifdef WIN32
#  define _T_stricmp _stricmp
#else
#  define _T_stricmp strcasecmp
#endif

enum CharType
{
    ControlChar,

    DigestChar,

    LetterChar,

    SpecialSign,

    OtherChar
};


/** 将字符串转为以分隔符隔开的字符串列表 */
void TokenizeString(const std::string& strInput, const std::string& strDelim, std::list<std::string>& tokenList);

/** 将“ab|dc|aa”格式的字符串以"|"分割开，转为字符串列表 */
bool StringToItems(const std::string& s, std::list<std::string>& itemList);


/** 将字符串转为布尔值 */
bool StringToBool(const char* s, bool& flag);

/** 将字符串转为有符号整数 */
bool StringToInteger(const char*  p, int& val);

/** 将字符串转为有符号整数 */
bool StringToInteger(const char*  p, Int16& val);

/** 将字符串转为无符号整数 */
bool StringToUnsignedInt(const char*  p, UInt32& val, Int32 nRadix);

/** 将字符串转为无符号64位整数 */
bool StringToUInt64(const char*  p, STATUS_ID& val, Int32 nRadix);

/** 将字符串转为有符号整数对 */
bool StringToIntPair(const char* s1, const char* s2, Int32& nInt1, Int32& nInt2);

/** 将字符串转为TRectangle */
bool StringToRect(const char* x, const char*  y, const char*  w, const char*  h, TRectangle& rc);


bool StringToDateTime(const char* pszText, DateTimeType &dt);


Boolean IsFileVaild(const char* pszFilePath, Boolean bIsUtf8 = FALSE);

Boolean IsFileVaild(const TUChar* pszFilePath);

Boolean IsImageFileVaild(const char* pszFilePath, Boolean bIsUtf8 = FALSE);

Boolean IsImageFileVaild(const TUChar* pszFilePath);


char *ustrncat (char * dst, const char * src, int nCount);

char *ustrncat_utf8 (char * dst, const TUChar * src, int nCount);

TUChar *ustrncat_utf8 (TUChar * dst, const char * src, int nCount);

TUChar *ustrncat_gb (TUChar * dst, const char * src, int nCount);

char *ustrncpy_utf8 (char * dst, const TUChar * src, int n);

TUChar *ustrncpy_utf8 (TUChar * dst, const char * src, int n);

void LogoutError(const TUChar* pszErrorU);

void LogoutError(const char* pszError, ...);


std::string char2hex( char dec );

std::string urlencode(const std::string &c);


CharType TestCharType(char c);

#endif //__TGLOBAL_H__


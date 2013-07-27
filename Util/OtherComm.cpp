/*!
*@FileName: OtherComm.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 其他通用接口
*@Log: 	Author 			Date 		Description
*
*/
#include "OtherComm.h"

#include  "TImgData.h"
#include "ImageToolKit/IT_ImageLoader.h"
#include "ImageToolKit/ImageTools.h"
#include "TGlobal.h"

// Image
using namespace ImageToolKit;

/**
 * 获取图片格式
 *
 * \param pszExtName
 *
 * \return 
 */
IT_LoadableImageFormat GetImageFormatEx(const TUChar* pszExtName)
{
    IT_LoadableImageFormat Format;
    char szExt[20];

    Format = IT_LOAD_FMT_UNKNOWN;
    if (pszExtName == NULL || TUString::StrLen(pszExtName) >= 20 || TUString::StrLen(pszExtName) == 0)
    {
        return Format;
    }

    ustrncpy_utf8(szExt, pszExtName, 20-1);

    if (_T_stricmp(szExt, ".png") == 0)
    {
        Format = IT_LOAD_FMT_PNG;
    } 
    else if (_T_stricmp(szExt, ".gif") == 0)
    {
        Format = IT_LOAD_FMT_GIF;
    }
    else if (_T_stricmp(szExt, ".jpg") == 0)
    {
        Format = IT_LOAD_FMT_JPG;
    }
    else if (_T_stricmp(szExt, ".bmp") == 0)
    {
        Format = IT_LOAD_FMT_BMP;
    }
    else if (_T_stricmp(szExt, ".tif") == 0)
    {
        Format = IT_LOAD_FMT_TIF;
    }
    else if (_T_stricmp(szExt, ".ebp") == 0)
    {
        Format = IT_LOAD_FMT_EBP;
    }
    else if (_T_stricmp(szExt, ".wbmp") == 0)
    {
        Format = IT_LOAD_FMT_WBMP;
    }

    return Format;
}

/**
 * 通过文件路径获取图片Bitmap
 *
 * \param pszPath
 *
 * \return 
 */
TBitmap* LoadImgByPath(const TUChar* pszPath)
{
    bool                   bRes;
    IT_LoadableImageFormat fm;
Image m_objImg;

    ImageLoader            objImgLoader;
     /** Image对象，保存从文件载入的图片 */
    /** 由Image转化的TBitmap指针 */
    TBitmap             *m_pBmp = NULL;
    TBitmap             *m_pRetBmp = NULL;
    /** 是PNG图片吗？ */
    Boolean             m_bIsPng = FALSE;

    bRes = FALSE;
    fm   = IT_LOAD_FMT_UNKNOWN;

    m_objImg.clearImage();	

    if (pszPath && *pszPath
        && IsImageFileVaild(pszPath) 
        && (fm = GetImageFormatEx(TUString::StrRChr(pszPath, '.'))) != IT_LOAD_FMT_UNKNOWN)
    {   
        if (fm == IT_LOAD_FMT_PNG)
        {
            bRes = IT_LoadPngFileToTBitmap(pszPath, &m_pBmp) >= 0;
            if(bRes)
            {
                m_bIsPng = TRUE;
            }
            else
            {
                m_pBmp = NULL;
                m_bIsPng = FALSE;
            }
        }
        else
        {
                bRes = objImgLoader.loadImage(m_objImg, pszPath, fm);      
        }
    }  

    if (bRes && !m_bIsPng)
    {
        m_pBmp = const_cast<TBitmap *>(m_objImg.GetTBitmap());
    }

	if(m_pBmp != NULL)
	{
		m_pRetBmp = m_pBmp->Clone();
		//PNG需要手动释放
		if(m_bIsPng == TRUE)
			m_pBmp->Destroy();
	}
	
    return m_pRetBmp;
}

/**
 * 通过文件路径获取图片Bitmap
 *
 * \param pszPath
 *
 * \return 
 */
TBitmap* LoadImgByPath(const char* sPath)
{
	TUChar pszPath[EOS_FILE_MAX_PATH];
	if(sPath == NULL)
		return NULL;

	TUString::StrUtf8ToStrUnicode(pszPath, (Char*)sPath);
	return LoadImgByPath(pszPath);
}

//Url Encode, 需要调用者释放
char* urlEncode(char* pszText)
{
	char* pszTextEncode = NULL;
	int nLen = 0;
	if(pszText == NULL || *pszText == 0)
		return NULL;

	nLen = strlen((char*)pszText);
	pszTextEncode = (char*)malloc(nLen*3 + 1);
	MemSet(pszTextEncode, 0, nLen*3 + 1);
	for(int i=0;i <nLen; i++)
	{
		if( (pszText[i] >= 'a' && pszText[i] <= 'z') || 
		    (pszText[i] >= 'A' && pszText[i] <= 'Z') || 
		    (pszText[i] >= '0' && pszText[i] <= '9') )
			sprintf( pszTextEncode, "%s%c", pszTextEncode, pszText[i] );
		else
			sprintf( pszTextEncode, "%s%%%02x", pszTextEncode, (UInt8)(pszText[i]) );
	}

	return pszTextEncode;
}


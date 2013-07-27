
/*!
* 
* \file 
* \author HuangYin
* \date	  2010.3.22
* \brief  图片数据类
* 
* \ref    CopyRight
*/

#include "TImgData.h"
#include "TGlobal.h"
#include "ImageToolKit/ImageTools.h"
#include <string>

// Image
using namespace ImageToolKit;

// std
using namespace std;


///////////////////////////////////////////////////////////////////////////
//                                                                       // 
//     TImgData     函数定义                                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////


/** 构造函数
*
* \return 无
*/
TImgData::TImgData()
{
    m_pBmp       = NULL;
    m_eDwnStatus = NoData;
    m_pszURL     = NULL;
    m_pszPath    = NULL;
    m_bIsPng     = FALSE;

    m_nWidth = 0;
    m_nHeight = 0;
    m_bKeepRatio = FALSE;
}

/** 构造函数
*
* \param nW 缩放后图片的宽度
* \param nH 缩放后图片的宽度
* \return 
*/
TImgData::TImgData( Int32 nW, Int32 nH, Boolean bKeepRatio)
{
    m_pBmp       = NULL;
    m_eDwnStatus = NoData;
    m_pszURL     = NULL;
    m_pszPath    = NULL;
    m_bIsPng     = FALSE;

    m_nWidth = nW;
    m_nHeight = nH;
    m_bKeepRatio = bKeepRatio;
}

/** 析构函数
*
* \return 无
*/
TImgData::~TImgData()
{
    _ClearData();
}

/** 重新载入商品数据图片
*
* \return 载入是否成功
*/
bool TImgData::ReLoadDataImg()
{
    bool   bRes;

    bRes = false;

    //// 清理已有数据
    //m_objImg.clearImage();
    //if (NULL != m_pBmp)
    //{
    //    if(m_bIsPng)
    //    {
    //        m_pBmp->Destroy();
    //        m_bIsPng = FALSE;
    //    }
    //    m_pBmp = NULL;
    //}

    // 图片等待下载并且保存有URL地址
    if (TImgData::DownloadingStart == m_eDwnStatus && m_pszPath != NULL)
    {
        bRes = _LoadImgByPath(m_pszPath);

        if (bRes)
        {
            delete [] m_pszURL;
            m_pszURL = NULL;

            delete [] m_pszPath;
            m_pszPath = NULL;

            m_eDwnStatus = TImgData::DownloadingFinish;  
        }
        else
        {
            m_eDwnStatus = TImgData::DownloadingError;  
        }
    }

    return bRes;
}

/** 获取图片的 TBitmap 指针
*
* \return 图片的 TBitmap 指针
*/
const TBitmap* TImgData::GetBitmap() const
{
    return m_pBmp;
}

/** 返回图片URL地址
*
* \return 图片URL地址，char字符串
*/
const char* TImgData::GetURL()
{
    return m_pszURL;
}

/** 返回图片的下载状态
*
* \return 图片的下载状态
*/
TImgData::ImgDwnStatus TImgData::GetDwnStatus()
{
    return m_eDwnStatus;
}

/** 设置图片开始下载
*
* \return 无
*/
void TImgData::SetImgStartDownloading()
{
    m_eDwnStatus = TImgData::DownloadingStart;  
}

/** 根据路径载入图片
*
* \param pszPath 图片路径，TUChar字符串
* \return 载入是否成功
*/
bool TImgData::_LoadImgByPath(const TUChar* pszPath)
{
    bool                   bRes;
    IT_LoadableImageFormat fm;
    ImageLoader            objImgLoader;

    bRes = FALSE;
    fm   = IT_LOAD_FMT_UNKNOWN;

    // 清除原来的图片
    m_objImg.clearImage();
    if (NULL != m_pBmp)
    {
        if(m_bIsPng)
        {
            m_pBmp->Destroy();
            m_bIsPng = FALSE;
        }
        m_pBmp = NULL;
    }

    if (pszPath && *pszPath
        && IsImageFileVaild(pszPath)
        && (fm = GetImageFormat(TUString::StrRChr(pszPath, '.'))) != IT_LOAD_FMT_UNKNOWN)
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
            if (m_nHeight > 0 && m_nWidth > 0)
            {
                bRes = objImgLoader.loadImageAndScaleEx(m_objImg, pszPath, m_nWidth, m_nHeight, fm,
                    m_bKeepRatio ? ImageProcessTool::KEEP_RATIO : ImageProcessTool::ACTUAL_SIZE);
            }
            else
            {
                bRes = objImgLoader.loadImage(m_objImg, pszPath, fm);
            }            
        }
    }  

    if (bRes && !m_bIsPng)
    {
        m_pBmp = const_cast<TBitmap *>(m_objImg.GetTBitmap());
    }

    return bRes;
}

/** 获取图像文件扩展名对应的IT_LoadableImageFormat枚举值
* 
* \param pszExtName TUChar字符串，文件扩展名
* \return  IT_LoadableImageFormat枚举值
*/ 
IT_LoadableImageFormat TImgData::GetImageFormat(const TUChar* pszExtName)
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

/** 清理数据
*
* \return 无
*/
void TImgData::_ClearData()
{
    m_eDwnStatus = NoData;
    m_objImg.clearImage();
    if (NULL != m_pBmp)
    {
        if(m_bIsPng)
        {
            m_pBmp->Destroy();
            m_bIsPng = FALSE;
        }
        m_pBmp = NULL;
    }

    if (m_pszURL)
    {
        delete [] m_pszURL;
        m_pszURL = NULL;
    }

    if (m_pszPath)
    {
        delete [] m_pszPath;
        m_pszPath = NULL;
    }
}

/** 返回图片文件路径，图片文件未成功载入前不为NULL
*
* \return 图片文件路径 TUChar*字符串
*/
const TUChar* TImgData::GetFilePath()
{
    return m_pszPath;
}

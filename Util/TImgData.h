
/*!
* 
* \file 
* \author HuangYin
* \date	  2010.3.22
* \brief  图片数据类
* 
* \ref    CopyRight
*/

#ifndef __TIMGDATA_H__
#define __TIMGDATA_H__

#include "ImageToolKit/IT_ImageLoader.h"

/** 图片数据类，char*字符串为UTF8编码
* 
* \ingroup UIBuilder
*/
class TImgData
{
public:

    /** 图片的下载状态 */
    enum ImgDwnStatus
    {
        /** 还没有数据 */
        NoData            = -1,

        /** 不需要下载 */
        NoNeedDownloading = 0,

        /** 等待下载 */
        WaitForDownloading = 1,

        /** 开始下载 */
        DownloadingStart = 2,

        /** 下载完成 */
        DownloadingFinish = 3,

        /** 下载错误 */
        DownloadingError  =4
    };

    TImgData();

    TImgData(Int32 nW, Int32 nH, Boolean bKeepRatio);

    ~TImgData();

public:

    bool           ReLoadDataImg();

    const TBitmap* GetBitmap() const;

    const char*    GetURL();

    const TUChar*  GetFilePath();

    ImgDwnStatus   GetDwnStatus();

    void           SetImgStartDownloading();

    static ImageToolKit::IT_LoadableImageFormat GetImageFormat(const TUChar* pszExtName);

    bool           _LoadImgByPath(const TUChar* pszPath);

private:

    void           _ClearData();

private:

     /** Image对象，保存从文件载入的图片 */
    ImageToolKit::Image m_objImg;

    /** 由Image转化的TBitmap指针 */
    TBitmap             *m_pBmp;

    /** 图片的下载状态 */
    ImgDwnStatus        m_eDwnStatus;

    /** 图片的URL地址 */
    char                *m_pszURL;

    /** 图片的路径 */
    TUChar              *m_pszPath;

    /** 是PNG图片吗？ */
    Boolean             m_bIsPng;

    /** 缩放后图片的宽度 */
    Int32 m_nWidth;

    /** 缩放后图片的高度 */
    Int32 m_nHeight;

    /** 缩放后图片是否要保持比例 */
    Boolean m_bKeepRatio;
};


#endif //__TIMGDATA_H__

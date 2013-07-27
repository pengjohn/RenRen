/*!
*@FileName: gui_common.h
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: 
*@Log: 	Author 			Date 		Description
*
*/
//gui_common.h
#ifndef  __gui_common_H__
#define  __gui_common_H__

#include "TG3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TResolution.h"

//
#define RGB_COLOR_GRAY					   (RGB(150, 150, 150))
#define RGB_COLOR_BLACK 				   (RGB(1, 1, 1))
#define RGB_COLOR_YELLOW				   (RGB(255, 255, 0))
#define RGB_COLOR_BLUE					   (RGB(0, 0, 255))
#define RGB_COLOR_LIGHTBLUE					(RGB(67, 67, 135))
#define RGB_COLOR_GREEN 				   (RGB(0, 128, 0))
#define RGB_COLOR_RED					   (RGB(255, 0, 0))
#define RGB_COLOR_PURPLE					(RGB(128, 0, 255))
#define RGB_COLOR_LIGHT_PURPLE				(RGB(128, 128, 255))
#define RGB_COLOR_AQUA					   (RGB(0, 255, 255))
#define RGB_COLOR_DISABLE				   (RGB(150, 150, 150))
#define RGB_COLOR_WHITE				   (RGB(254, 254, 254))
#define RGB_COLOR_ORANGE				   (RGB(255, 68, 1))
#define RGB_COLOR_LIGHTGRAY                      (RGB(204, 204, 204))
//
#define RGB_COLOR_HIGHTLINE				(RGB(80,100,220))
#define RGB_COLOR_INFO_BACKGROUND         (RGB(253, 255, 206))
#define RGB_COLOR_FORM_BACKGROUND         (RGB(254, 254, 254))

#define LCD_HVGA   	0  //LCD(320*480)
#define LCD_WVGA   	1  //LCD(480*800)

#if defined(UPHONE_HVGA)  
#define LCD_SIZE		LCD_HVGA
#elif defined(UPHONE_WVGA)  
#define LCD_SIZE		LCD_WVGA
#else  
#error resolution  
#endif 

#if(LCD_SIZE == LCD_HVGA )
#elif(LCD_SIZE == LCD_WVGA )
#endif


#if(LCD_SIZE == LCD_HVGA )
//��Ļ��С
#define SCR_W	(320)
#define SCR_H	(480)
#define STATUSBAR_H	(24)
#define TITLEBAR_H	(44)
#define BOTTOM_TAB_H	(44)

//�ײ�Tab�Ĵ�С
#define BOTTOM_TAB_Y	( SCR_H - STATUSBAR_H - BOTTOM_TAB_H )
#define BOTTOM_TAB_W	(SCR_W/BOTTOM_TAB_NUM)

//�ײ�Option�Ĵ�С
#define BOTTOM_OPTION_Y	( SCR_H - STATUSBAR_H - BOTTOM_TAB_H )
#define BOTTOM_OPTION_W	(SCR_W)
#define BOTTOM_OPTION_H	(44)
#define BOTTOM_OPTION_CAPTION_X		(12)
#define BOTTOM_OPTION_CAPTION_Y		(12)

//����Tab�Ĵ�С
#define TOP_TAB_Y	(44)
#define TOP_TAB_W	(SCR_W)
#define TOP_TAB_H	(43)
#define TOP_TAB_CAPTION_X		(12)
#define TOP_TAB_CAPTION_Y		(12)

//CoolBarList
#define COOLBARLIST_WIDTH		(SCR_W - 12)

//title button
#define TITLE_BUTTON_X	(5)
#define TITLE_BUTTON_W	(60)
#define TITLE_BUTTON_H	(30)
#define TITLE_BUTTON_Y	((TITLEBAR_H - TITLE_BUTTON_H)/2)

//�����С
#define FONT_LARGE		(18)
#define FONT_NORMAL	(16)
#define FONT_MIDDLE		(14)
#define FONT_SMALL		(12)

#define FONT_APP_TITLE					(FONT_LARGE)
#define FONT_BUTTON_CAPTION				(FONT_MIDDLE)


//�м��
#define LINE_SPACE		(4)

//ͷ��߿�Ĵ�С
#define HEADMASKBUTTON_W		(58)
#define HEADMASKBUTTON_H		(58)
//ͷ���С
#define HEADPHOTO_W		(50)
#define HEADPHOTO_H		(50)

#define KEYBOARD_H  (208)
#elif(LCD_SIZE == LCD_WVGA )
//��Ļ��С
#define SCR_W	(480)
#define SCR_H	(800)
#define STATUSBAR_H	(36)
#define TITLEBAR_H	(66)
#define BOTTOM_TAB_H	(66)

//�ײ�Tab�Ĵ�С
#define BOTTOM_TAB_Y	( SCR_H - STATUSBAR_H - BOTTOM_TAB_H )
#define BOTTOM_TAB_W	(SCR_W/BOTTOM_TAB_NUM)

//�ײ�Option�Ĵ�С
#define BOTTOM_OPTION_Y	( SCR_H - STATUSBAR_H - BOTTOM_TAB_H )
#define BOTTOM_OPTION_W	(SCR_W)
#define BOTTOM_OPTION_H	(BOTTOM_TAB_H)
#define BOTTOM_OPTION_CAPTION_X		(12)
#define BOTTOM_OPTION_CAPTION_Y		(12)

//����Tab�Ĵ�С
#define TOP_TAB_Y	(66)
#define TOP_TAB_W	(SCR_W)
#define TOP_TAB_H	(66)
#define TOP_TAB_CAPTION_X		(12)
#define TOP_TAB_CAPTION_Y		(12)

//CoolBarList
#define COOLBARLIST_WIDTH		(SCR_W - 18)

//title button
#define TITLE_BUTTON_X	(5)
#define TITLE_BUTTON_W	(90)
#define TITLE_BUTTON_H	(45)
#define TITLE_BUTTON_Y	((TITLEBAR_H - TITLE_BUTTON_H)/2)

//�����С
#define FONT_LARGE		(28)
#define FONT_NORMAL	(24)
#define FONT_MIDDLE		(20)
#define FONT_SMALL		(16)

#define FONT_APP_TITLE				(FONT_LARGE)
#define FONT_BUTTON_CAPTION		(FONT_MIDDLE)


//�м��
#define LINE_SPACE		(6)


//ͷ��߿�Ĵ�С
#define HEADMASKBUTTON_W		(87)
#define HEADMASKBUTTON_H		(87)
//ͷ���С
#define HEADPHOTO_W		(75)
#define HEADPHOTO_H		(75)

#define KEYBOARD_H  (320)
#endif


enum
{
	TITLE_BUTTON_LEFT = 0,
	TITLE_BUTTON_RIGHT,
};

//�ײ�Tab�������
#define BOTTOM_TAB_NUM	(5)
typedef struct
{
	Int32  nCtrID[BOTTOM_TAB_NUM];
}sBottomTabCtrID;

typedef struct
{
	Int32 ResID_Normal;
	Int32 ResID_Down;
	Int32 ResID_Title;
}sTabButtonItem;

//�ײ�Option�������(���)
#define BOTTOM_OPTION_NUM	(5)
typedef struct
{
	Int32  nCtrID[BOTTOM_OPTION_NUM];
}sBottomOptionCtrID;

typedef struct
{
	Int32 ResID_Button_Normal;
	Int32 ResID_Button_Selected;
	Int32 ResID_Icon_Normal;
	Int32 ResID_Icon_Selected;
	Int32 ResID_Title;
}sBottomOptionItem;

typedef struct
{
	TUChar pszName[32];			//����
	TUChar pszContent[40960];	//����
	TUChar pszTime[64];			//ʱ��
	Int32    OptionButtonResID;	//��ť�ı���ԴID
	Int32    FlagIconResID;		//Сͼ����ԴID
	Coord   nY;					//��ʼY����
	Boolean bLeft;				//�Ƿ�ͷ����
	TBitmap * pImage;			//ͷ��Image	
}tBubbleDialogItemData;

extern Int32 GetShowAllStringWidth(TUChar* pCaptionString, TFont objFontType);
extern Int32 GetShowAllStringWidthEx(TUChar* pCaptionString, Int32 FontSize);
extern void  ShowAsShort(TCtrl *pTCtrl, TUChar* pCaptionString, TFont objFontType);
extern Int32 SetAppTilte(TWindow * pWindow, Int32 TitleStringResId = 0,  TUChar* pTitleString = NULL, ColorRefType Color = RGB_COLOR_WHITE, Int32 nFontSize = FONT_APP_TITLE);
extern Int32 SetAppBitmapButton(TWindow * pWindow, Int32 NomalBtnID, Int32 OverBtnID);
extern Int32 SetAppTitleButton(TWindow * pWindow, Int32 nResId, int ButtonPosition);
extern Int32 SetAppTitleButton(TWindow * pWindow, const TUChar* pszResTxt, int ButtonPosition);
extern Int32 CreateBottomOption(TWindow * pWindow, sBottomOptionCtrID* BtnCtrID, sBottomOptionItem* BottomOptionItem, int nItemCount);
extern Int32 CreateProfileBottomTab(TWindow * pWindow, sBottomTabCtrID* BtnCtrID, int nFocusItem);
extern Int32 CreateFriendListBottomTab(TWindow * pWindow, sBottomTabCtrID* BtnCtrID, int nFocusItem);

extern TFont CtrlSetFont(TCtrl *pCtrl, Int32 nFontSize, ColorRefType nColor = RGB_COLOR_BLACK);

extern Int32 CtrlAddItemToCoolBarList_Lable(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color = RGB_COLOR_BLACK);
extern Int32 CtrlAddItemToCoolBarList_RichView(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color = RGB_COLOR_BLACK, Boolean bEnable = FALSE);
extern Int32 CtrlAddItemToCoolBarList_MaskButton(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pBackImage);
extern Int32 CtrlAddItemToCoolBarList_Button(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TUChar* pszString);
extern Int32 CtrlAddItemToCoolBarList_Image(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TBitmap * pBmp);
extern Int32 CtrlAddItemToCoolBarList_CommentInfo(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TUChar* pszString);

extern Int32 CtrlAddItemToPanel_Edit(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight);
extern Int32 CtrlAddItemToPanel_MaskButton(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pImageSel, TBitmap * pBackImage);
extern Int32 CtrlAddItemToPanel_Label(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color = RGB_COLOR_BLACK);
extern Int32 CtrlAddItemToPanel_RichView(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color = RGB_COLOR_BLACK);
extern Int32 CtrlAddItemToPanel_Image(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pBmp);
extern Int32 CtrlAddItemToPanel_Button(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TUChar* pszString);

extern Int32 CtrlCreateToPanel_BubbleDialog(TWindow*pWin,  TPanel* pPanel, tBubbleDialogItemData  BubbleItemData, Coord* nHeight, Int32* OptionButtonID, Int32* HeadMaskID);


extern Int32 CtrlAddItemToWin_MaskButton(TWindow*pWin,  Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pImageSel);
extern Int32 CtrlAddItemToWin_Label(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color = RGB_COLOR_BLACK);
extern Int32 CtrlAddItemToWin_Edit(TWindow*pWin,  Coord nX, Coord nY, Coord nWidth, Coord nHeight);
extern Int32 CtrlAddItemToWin_Image(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pBmp);
extern Int32 CtrlAddItemToWin_RichView(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color, Boolean bEnable );

extern Coord GetCtrlWidth(TWindow*pWin, Int32 nCtrlID);
#endif

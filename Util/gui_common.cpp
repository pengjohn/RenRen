/*!
*@FileName: gui_common.cpp
*@Author: pengzhixiong@GoZone
*@Date: 2011-1-10
*@Brief: UI通用函数
*@Log: 	Author 			Date 		Description
*
*/
#include "gui_common.h"
#include "TResolution.h"
#include  "App_RenRenUnicodeScript_str.h"

//根据字体计算出显示整个字串所需的像素
//Todo::WordWrapNoLF有问题, 如果第一个字节为空格,返回的nShowLen长度有问题，需要做去头空格处理
Int32 GetShowAllStringWidth(TUChar* pCaptionString, TFont objFontType)
{
	Int32 Width = 1;
	Int32 tempWidth = 0;//从像素0开始
	Int32 nStringLen = TUString::StrLen(pCaptionString);
	Int32 nShowLen = 0;
	do
	{
		tempWidth ++;
		nShowLen = objFontType.WordWrapNoLF(TUString::StrTrimUnVisible(pCaptionString), tempWidth);
		
	}while(nShowLen < nStringLen);

	return tempWidth + 4;//刚刚好的长度显示字串时有问题，故再加两个像素
}

//根据字体计算出显示整个字串所需的像素
Int32 GetShowAllStringWidthEx(TUChar* pCaptionString, Int32 FontSize)
{
	TFont objFontType;
	objFontType.Create(FontSize, FontSize);
	return GetShowAllStringWidth(pCaptionString, objFontType);
}

//如果内容太长显示不完则用省略号表示
void  ShowAsShort(TCtrl *pTCtrl, TUChar* pCaptionString, TFont objFontType)
{
	TRectangle Rc_TCtrl;
	pTCtrl->GetBounds(&Rc_TCtrl);
	
	int nStrLen = TUString::StrLen(TUString::StrTrimUnVisible(pCaptionString));
	//int nShortWidth = GetShowAllStringWidth((TUChar*)L"...",objFontType);
	int nShortWidth = GetShowAllStringWidth((TUChar*)TUSTR_Re_Ellipsis,objFontType);	
	int nShowLen = objFontType.WordWrapNoLF(TUString::StrTrimUnVisible(pCaptionString), Rc_TCtrl.Width()- nShortWidth*2);

	if (nShowLen < nStrLen)
	{
		//Add one more label to show
		
		TUChar* pszTemp = new TUChar[nShowLen + 10];
		MemSet( (void *)pszTemp, 0x0, sizeof(TUChar) * (nShowLen + 10) );
		TUString::StrNCopy(pszTemp,TUString::StrTrimUnVisible(pCaptionString),nShowLen);
		TUString::StrCat(pszTemp,TUSTR_Re_Ellipsis);//(const TUChar*)L"..."
		pTCtrl->SetCaption(pszTemp,FALSE);
		delete pszTemp;	
	}
	else
	{
		pTCtrl->SetCaption(TUString::StrTrimUnVisible(pCaptionString),FALSE);//StrTrim
	}	
}

//创建应用标题默认居中
//pApp --应用指针
//TitleStringResId --标题字串ID, 默认为0
//TitleString --标题字串，默认为空NULL
//color--标题颜色，默认为白:RGB_COLOR_WHITE
//nFontSize 字体大小
//返回控件ID
Int32 SetAppTilte(TWindow * pWindow, Int32 nTitleStringResId, TUChar* pTitleString, ColorRefType Color, Int32 nFontSize)
{
	Int32 m_TitleCtlID = 0;

	TCtrl *m_TitleContrl;
	TRectangle TitleRC(0,0,0,0);
	TRectangle WindowRC(0, 0, 0, 0);
	TFont tFont;
	Int32 nTitleWidth = 0;
	Boolean bNeedShort = FALSE;
	TUChar sTitleString[128] ={0};

	if(nTitleStringResId == 0)//说明传进的是字串
	{
		if(pTitleString)
		{
			TUString::StrNCopy(sTitleString, pTitleString, sizeof(sTitleString));
		}
	}
	else
	{
		TUString::StrNCopy(sTitleString, TResource::LoadConstString(nTitleStringResId), sizeof(sTitleString));
	}
	
	tFont.Create(nFontSize, nFontSize,  FONT_STYLE_BOLD);
	nTitleWidth = GetShowAllStringWidth(sTitleString,tFont);
	if(nTitleWidth > SCR_W - (TITLE_BUTTON_X + TITLE_BUTTON_W)*2 )
	{
		bNeedShort = TRUE;
		nTitleWidth = SCR_W - (TITLE_BUTTON_X + TITLE_BUTTON_W)*2 - 10;
	}
	
	pWindow->GetBounds(&WindowRC);	
	TitleRC.SetRect((WindowRC.Width()-nTitleWidth)/2, (TITLEBAR_H- nFontSize)/2, nTitleWidth, nFontSize);
	
	m_TitleContrl = new TLabel;
	
	if(m_TitleContrl->Create(pWindow))
	{		
		m_TitleContrl->SetBounds(&TitleRC);	
		m_TitleCtlID = m_TitleContrl->GetId();	

		m_TitleContrl->SetFont(tFont);
		m_TitleContrl->SetColor(CTL_COLOR_TYPE_FORE,Color);
		m_TitleContrl->SetEnabled(FALSE);
		m_TitleContrl->SetCaption(sTitleString,FALSE);
		if(bNeedShort)
			ShowAsShort(m_TitleContrl, sTitleString, tFont);
		m_TitleContrl->Show(TRUE);
	}

	return m_TitleCtlID;
}

//创建应用编辑按钮
//pApp --应用指针
//返回控件ID
Int32 SetAppBitmapButton(TWindow * pWindow, Int32 NomalBtnID, Int32 OverBtnID)
{
	Int32 m_BtnID = 0;
	TButton *pBtn;
	TRectangle BtnRC(TITLE_BUTTON_X, TITLE_BUTTON_Y, TITLE_BUTTON_W, TITLE_BUTTON_H);

	const TBitmap * pNormalBmp = TResource::LoadConstBitmap(NomalBtnID);//APP_RE_ID_BITMAP_write_but
	const TBitmap * pOverBmp = TResource::LoadConstBitmap(OverBtnID);//APP_RE_ID_BITMAP_write_but_over
		
	pBtn = new TButton;	
	if(pBtn->Create(pWindow))
	{		
		pBtn->SetBounds(&BtnRC);	
		m_BtnID = pBtn->GetId();	
		
		pBtn->SetCaption(TUSTR_Re_NULL,FALSE);
		pBtn->SetStyles(BTN_STYLES_GRAPGICS);
		pBtn->SetImage(pNormalBmp,0);
		pBtn->SetImage(pOverBmp,1);		
		pBtn->Show(TRUE);
	}
	return m_BtnID;
}

//设置应用标题栏按钮
Int32 SetAppTitleButton(TWindow * pWindow, Int32 nResId, int ButtonPosition)
{
	Int32 m_BtnID = 0;
	TButton *pBtn;
	TRectangle BtnRC_Left(TITLE_BUTTON_X, TITLE_BUTTON_Y, TITLE_BUTTON_W, TITLE_BUTTON_H);	
	TRectangle BtnRC_Right(SCR_W-TITLE_BUTTON_X-TITLE_BUTTON_W, TITLE_BUTTON_Y, TITLE_BUTTON_W, TITLE_BUTTON_H);
	TFont tFont;

	const TBitmap * pNormalBmp = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_button60);
	const TBitmap * pOverBmp = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_title_button60_over);

	if(TUString::StrLen(TResource::LoadConstString(nResId)) <= 2)
		tFont.Create(FONT_NORMAL, FONT_NORMAL, FONT_STYLE_BOLD);
	else
		tFont.Create(FONT_MIDDLE, FONT_MIDDLE, FONT_STYLE_BOLD);
	pBtn = new TButton;	
	if(pBtn->Create(pWindow))
	{		
		if(ButtonPosition == TITLE_BUTTON_LEFT)
			pBtn->SetBounds(&BtnRC_Left);	
		else
			pBtn->SetBounds(&BtnRC_Right);	
		m_BtnID = pBtn->GetId();	

		pBtn->SetFont(tFont);
		pBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
		
		pBtn->SetCaption(TResource::LoadConstString(nResId),FALSE);
		pBtn->SetStyles(BTN_STYLES_GRAPGICS);
		pBtn->SetImage(pNormalBmp,0);
		pBtn->SetImage(pOverBmp,1);		
		pBtn->Show(TRUE);
	}

	return m_BtnID;
}

//设置应用标题栏按钮
Int32 SetAppTitleButton(TWindow * pWindow, const TUChar* pszResTxt, int ButtonPosition)
{
	Int32 m_BtnID = 0;
	
	TRectangle BtnRC_Left(TITLE_BUTTON_X, TITLE_BUTTON_Y, TITLE_BUTTON_W, TITLE_BUTTON_H);
	TRectangle BtnRC_Right(SCR_W-TITLE_BUTTON_X-TITLE_BUTTON_W, TITLE_BUTTON_Y, TITLE_BUTTON_W, TITLE_BUTTON_H);
	
	Coord  nWidth = 0;
	Int32 nResIDNormal = 0;
	Int32 nResIDOver = 0;
	int nFontSize = 0;

	//根据显示文本的长短设置图标及文字大小
	switch( TUString::StrLen(pszResTxt) )
	{
		case 2:
			{
			 nWidth = TITLE_BUTTON_W;
			nFontSize = FONT_NORMAL;
			nResIDNormal = APP_RE_ID_BITMAP_title_button60;
			nResIDOver = APP_RE_ID_BITMAP_title_button60_over;				
			}
			break;
		case 3:
			{
			 nWidth = TITLE_BUTTON_W;
			nFontSize = FONT_NORMAL;
			nResIDNormal = APP_RE_ID_BITMAP_title_button60;
			nResIDOver = APP_RE_ID_BITMAP_title_button60_over;				
			}
			break;
		case 4:
			{
			 nWidth = TITLE_BUTTON_W;
			nFontSize = FONT_NORMAL;
			nResIDNormal = APP_RE_ID_BITMAP_title_button60;
			nResIDOver = APP_RE_ID_BITMAP_title_button60_over;				
			}
			break;
		case 5:
			{
			 nWidth = TITLE_BUTTON_W;
			nFontSize = FONT_MIDDLE;
			nResIDNormal = APP_RE_ID_BITMAP_title_button60;
			nResIDOver = APP_RE_ID_BITMAP_title_button60_over;				
			}
			break;
		case 6:
		default:
			{
			 nWidth = TITLE_BUTTON_W;
			nFontSize = FONT_SMALL;
			nResIDNormal = APP_RE_ID_BITMAP_title_button60;
			nResIDOver = APP_RE_ID_BITMAP_title_button60_over;				
			}
			break;

	}

	TFont tFont;	
	const TBitmap* pNormalBmp = TResource::LoadConstBitmap(nResIDNormal);
	const TBitmap* pOverBmp = TResource::LoadConstBitmap(nResIDOver);
	tFont.Create(nFontSize, nFontSize, FONT_STYLE_BOLD);
	
	TButton* pBtn = new TButton;	
	if(pBtn->Create(pWindow))
	{		
		BtnRC_Left.SetWidth(nWidth);
		BtnRC_Right.SetWidth(nWidth);
		BtnRC_Right.SetX(SCR_W-TITLE_BUTTON_X-nWidth);
			
		if(ButtonPosition == TITLE_BUTTON_LEFT)
			pBtn->SetBounds(&BtnRC_Left);	
		else
			pBtn->SetBounds(&BtnRC_Right);
		
		m_BtnID = pBtn->GetId();	

		pBtn->SetFont(tFont);
		pBtn->SetColor(CTL_COLOR_TYPE_FORE,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_BACK,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_FOCUS_FORE,RGB_COLOR_WHITE);
		pBtn->SetColor(CTL_COLOR_TYPE_FOCUS_BACK,RGB_COLOR_WHITE);
		
		pBtn->SetCaption(pszResTxt,FALSE);
		pBtn->SetStyles(BTN_STYLES_GRAPGICS);
		pBtn->SetImage(pNormalBmp,0);
		pBtn->SetImage(pOverBmp,1);		
		pBtn->Show(TRUE);
	}

	return m_BtnID;
}

//----------------------------------------------------------------------------

//个人主页的BottomTab
sTabButtonItem	 ProfileBottomTabBtn[BOTTOM_TAB_NUM] = 
{
	{APP_RE_ID_BITMAP_profile_tab01, APP_RE_ID_BITMAP_profile_tab01hl, APP_RE_ID_STRING_Feed},
	{APP_RE_ID_BITMAP_profile_tab02, APP_RE_ID_BITMAP_profile_tab02hl, APP_RE_ID_STRING_FriendsList},
	{APP_RE_ID_BITMAP_profile_tab03, APP_RE_ID_BITMAP_profile_tab03hl, APP_RE_ID_STRING_MyProfile},
	{APP_RE_ID_BITMAP_profile_tab04, APP_RE_ID_BITMAP_profile_tab04hl, APP_RE_ID_STRING_Applications},
	{APP_RE_ID_BITMAP_profile_tab05, APP_RE_ID_BITMAP_profile_tab05hl, APP_RE_ID_STRING_More}
};

#if(LCD_SIZE == LCD_HVGA )
#define PROFILE_BOTTOM_TAB_W	(62)
#elif(LCD_SIZE == LCD_WVGA )
#define PROFILE_BOTTOM_TAB_W	(93)
#endif
#define PROFILE_BOTTOM_TAB_Y	(BOTTOM_TAB_Y)
#define PROFILE_BOTTOM_TAB_H	(BOTTOM_TAB_H)
#define PROFILE_BOTTOM_TAB_X	((SCR_W - PROFILE_BOTTOM_TAB_W*BOTTOM_TAB_NUM)/2)

// 注: 控件的背景需要Form自行在EVENT_WinEraseClient填充
Int32 CreateProfileBottomTab(TWindow * pWindow, sBottomTabCtrID* BtnCtrID, int nFocusItem)
{
	Int32 bnRetCod = 0; 
	TRectangle obBtnRec(0,0,0,0);
	TFont tFont;
	
	//按钮背景
	//const TBitmap * pImgBG = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Bottom_btn_bg);

	const TBitmap * pImgNormal[BOTTOM_TAB_NUM];
	const TBitmap * pImgDown[BOTTOM_TAB_NUM];
	TMaskButton* pMaskBtn[BOTTOM_TAB_NUM];
	
	for(int i=0; i<BOTTOM_TAB_NUM; i++)
	{
		BtnCtrID->nCtrID[i] = 0;
		//默认Focus高亮项
		if(i == (nFocusItem-1))
			pImgNormal[i] = TResource::LoadConstBitmap(ProfileBottomTabBtn[i].ResID_Down);
		else
			pImgNormal[i] = TResource::LoadConstBitmap(ProfileBottomTabBtn[i].ResID_Normal);
		
		pImgDown[i] = TResource::LoadConstBitmap(ProfileBottomTabBtn[i].ResID_Down);
		obBtnRec.SetRect(PROFILE_BOTTOM_TAB_X + PROFILE_BOTTOM_TAB_W*i, PROFILE_BOTTOM_TAB_Y, PROFILE_BOTTOM_TAB_W, PROFILE_BOTTOM_TAB_H);
		
		pMaskBtn[i] =new TMaskButton;
		if(pMaskBtn[i]->Create(pWindow))
		{
			pMaskBtn[i]->SetBounds(&obBtnRec); 	
			BtnCtrID->nCtrID[i] = pMaskBtn[i]->GetId();
			pMaskBtn[i]->SetImage(pImgNormal[i],(PROFILE_BOTTOM_TAB_W-pImgNormal[i]->GetWidth())/2,(PROFILE_BOTTOM_TAB_H-pImgNormal[i]->GetHeight())/2); 
			pMaskBtn[i]->SetSelImage(pImgDown[i],(PROFILE_BOTTOM_TAB_W-pImgDown[i]->GetWidth())/2,(PROFILE_BOTTOM_TAB_H-pImgDown[i]->GetHeight())/2);	
		}
	}
	
	return bnRetCod;
}


//好友列表的BottomTab
#define FRIENDLIST_BOTTOM_TAB_NUM	(3)
sTabButtonItem	 FriendListBottomTabBtn[FRIENDLIST_BOTTOM_TAB_NUM] = 
{
	{APP_RE_ID_BITMAP_Friedlist_tab01, APP_RE_ID_BITMAP_Friedlist_tab01hl, APP_RE_ID_STRING_FriendsList},
	{APP_RE_ID_BITMAP_Friedlist_tab02, APP_RE_ID_BITMAP_Friedlist_tab02hl, APP_RE_ID_STRING_Pages},
	{APP_RE_ID_BITMAP_Friedlist_tab03, APP_RE_ID_BITMAP_Friedlist_tab03hl, APP_RE_ID_STRING_Visitors},
};

#if(LCD_SIZE == LCD_HVGA )
#define FRIENDLIST_BOTTOM_TAB_W	(103)
#elif(LCD_SIZE == LCD_WVGA )
#define FRIENDLIST_BOTTOM_TAB_W	(155)
#endif
#define FRIENDLIST_BOTTOM_TAB_Y	(BOTTOM_OPTION_Y)
#define FRIENDLIST_BOTTOM_TAB_H	(BOTTOM_OPTION_H)
#define FRIENDLIST_BOTTOM_TAB_X	((SCR_W - FRIENDLIST_BOTTOM_TAB_W*FRIENDLIST_BOTTOM_TAB_NUM)/2)

// 注: 控件的背景需要Form自行在EVENT_WinEraseClient填充
Int32 CreateFriendListBottomTab(TWindow * pWindow, sBottomTabCtrID* BtnCtrID, int nFocusItem)
{
	Int32 bnRetCod = 0; 
	TRectangle obBtnRec(0,0,0,0);
	TFont tFont;
	
	//按钮背景
	//const TBitmap * pImgBG = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Bottom_btn_bg);

	const TBitmap * pImgNormal[FRIENDLIST_BOTTOM_TAB_NUM];
	const TBitmap * pImgDown[FRIENDLIST_BOTTOM_TAB_NUM];
	TMaskButton* pMaskBtn[FRIENDLIST_BOTTOM_TAB_NUM];
	
	for(int i=0; i<FRIENDLIST_BOTTOM_TAB_NUM; i++)
	{
		BtnCtrID->nCtrID[i] = 0;
		//默认Focus高亮项
		if(i == (nFocusItem-1))
			pImgNormal[i] = TResource::LoadConstBitmap(FriendListBottomTabBtn[i].ResID_Down);
		else
			pImgNormal[i] = TResource::LoadConstBitmap(FriendListBottomTabBtn[i].ResID_Normal);
		
		pImgDown[i] = TResource::LoadConstBitmap(FriendListBottomTabBtn[i].ResID_Down);
		obBtnRec.SetRect(FRIENDLIST_BOTTOM_TAB_X + FRIENDLIST_BOTTOM_TAB_W*i, FRIENDLIST_BOTTOM_TAB_Y, FRIENDLIST_BOTTOM_TAB_W, FRIENDLIST_BOTTOM_TAB_H);
		
		pMaskBtn[i] =new TMaskButton;
		if(pMaskBtn[i]->Create(pWindow))
		{
			pMaskBtn[i]->SetBounds(&obBtnRec); 	
			BtnCtrID->nCtrID[i] = pMaskBtn[i]->GetId();
			//pMaskBtn[i]->SetMaskImage(pImgBG,pImgBG,pImgBG);
			pMaskBtn[i]->SetImage(pImgNormal[i],(FRIENDLIST_BOTTOM_TAB_W-pImgNormal[i]->GetWidth())/2,(FRIENDLIST_BOTTOM_TAB_H-pImgNormal[i]->GetHeight())/2); 
			pMaskBtn[i]->SetSelImage(pImgDown[i],(FRIENDLIST_BOTTOM_TAB_W-pImgDown[i]->GetWidth())/2,(FRIENDLIST_BOTTOM_TAB_H-pImgDown[i]->GetHeight())/2);	

		}
	}
	
	return bnRetCod;
}


// 创建页面下端的option栏, 
// BtnCtrID -- 保存创建的控件ID
// BottomOptionItem -- 传入的控件信息,如按键图标/文本
// nItemCount -- 控件个数
// 注: 控件的背景需要Form自行在EVENT_WinEraseClient填充
Int32 CreateBottomOption(TWindow * pWindow, sBottomOptionCtrID* BtnCtrID, sBottomOptionItem* BottomOptionItem, int nItemCount)
{
	Int32 bnRetCod = 0; 
	TRectangle obBtnRec(100,50,150,30); //初始(left, top, w, h)
	Int16 TabItemWidth = 0; 
	TFont tFont;
	tFont.Create(FONT_BUTTON_CAPTION, FONT_BUTTON_CAPTION);
	
	if(nItemCount > BOTTOM_OPTION_NUM)
		nItemCount = BOTTOM_OPTION_NUM;

	//宽度不能整除时做处理,保证满屏
	TabItemWidth = ((BOTTOM_OPTION_W+nItemCount-1)/nItemCount);
	
	//按钮背景
	const TBitmap * pImgBG = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_TopTab_Bg);

	const TBitmap * pImgButtonNormal[BOTTOM_OPTION_NUM] = {0};
	const TBitmap * pImgButtonSelected[BOTTOM_OPTION_NUM] = {0};
	const TBitmap * pImgIconNormal[BOTTOM_OPTION_NUM] = {0};
	const TBitmap * pImgIconSelected[BOTTOM_OPTION_NUM] = {0};
	TMaskButton* pMaskBtn[BOTTOM_OPTION_NUM] = {0};

	for(int i=0; i<nItemCount; i++)
	{
		BtnCtrID->nCtrID[i] = 0;
		//先判断是否是保留空位置
		if( (BottomOptionItem[i].ResID_Button_Normal ==0) || (BottomOptionItem[i].ResID_Button_Selected ==0) )
			continue;
		
		pImgButtonNormal[i] = TResource::LoadConstBitmap(BottomOptionItem[i].ResID_Button_Normal);
		pImgButtonSelected[i] = TResource::LoadConstBitmap(BottomOptionItem[i].ResID_Button_Selected);
		pImgIconNormal[i] = TResource::LoadConstBitmap(BottomOptionItem[i].ResID_Icon_Normal);
		pImgIconSelected[i] = TResource::LoadConstBitmap(BottomOptionItem[i].ResID_Icon_Selected);
		
		obBtnRec.SetRect(TabItemWidth*i, BOTTOM_OPTION_Y, TabItemWidth, BOTTOM_OPTION_H);
		
		pMaskBtn[i] =new TMaskButton;
		if(pMaskBtn[i]->Create(pWindow))
		{
			Int32 CaptionX = 0;
			Int32 CaptionW = 0;
			
			BtnCtrID->nCtrID[i] = pMaskBtn[i]->GetId();
			pMaskBtn[i]->SetBounds(&obBtnRec); 	

			pMaskBtn[i]->SetMaskImage(pImgButtonNormal[i], pImgButtonSelected[i], pImgButtonNormal[i]);
			if(pImgIconNormal[i])
				pMaskBtn[i]->SetImage(pImgIconNormal[i], 6, (pImgButtonNormal[i]->GetHeight() - pImgIconNormal[i]->GetHeight() )/2);
			if(pImgIconSelected[i])
				pMaskBtn[i]->SetSelImage(pImgIconSelected[i], 6, (pImgButtonSelected[i]->GetHeight())/2 - (pImgIconSelected[i]->GetHeight())/2);

 			CaptionW = GetShowAllStringWidth((TUChar*)TResource::LoadConstString(BottomOptionItem[i].ResID_Title), tFont);
			//区分是否有Icon的情况
			if(pImgIconNormal[i])
				CaptionX = (pImgIconNormal[i]->GetWidth())/2 + (TabItemWidth - CaptionW)/2;
			else
				CaptionX = (TabItemWidth - CaptionW)/2;
			pMaskBtn[i]->SetFont(tFont);
			pMaskBtn[i]->SetCaption(TResource::LoadConstString(BottomOptionItem[i].ResID_Title), CaptionX,  BOTTOM_OPTION_CAPTION_Y);
			pMaskBtn[i]->SetColor(RGB_COLOR_WHITE,RGB_COLOR_WHITE,RGB_COLOR_WHITE,RGB_COLOR_WHITE,RGB_COLOR_WHITE,RGB_COLOR_WHITE);	
		}
	}
	
	return bnRetCod;
}

//设置控件文本大小颜色
TFont CtrlSetFont(TCtrl *pCtrl, Int32 nFontSize, ColorRefType nColor)
{
	TFont tFont;
	tFont.Create(nFontSize, nFontSize);
	pCtrl->SetColor(CTL_COLOR_TYPE_FORE, nColor);
	return pCtrl->SetFont(tFont);
}

//添加Label控件到CoolBarList, 
// pszString必须是动态的(非Static)
// nWidth 为0 -- 自动长度
Int32 CtrlAddItemToCoolBarList_Lable(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color)
{
	TRectangle obBtnRec(0,0,0,0);
	Int32 nLabelNameId = pListItem->AddCtrl(CTL_CLASS_LABEL, 0, 0);
	TLabel* pLabelName = static_cast<TLabel*>(pWin->GetControlPtr(nLabelNameId));
	if(nWidth == 0)
	{
		nWidth = 80;
		obBtnRec.SetRect(nX, nY, nWidth, FontSize+LINE_SPACE);
		pLabelName->SetBounds(&obBtnRec);
		pLabelName->SetAutoSize(TRUE);

		CtrlSetFont((TCtrl*)pLabelName, FontSize, Color);
		pLabelName->SetCaption(pszString,FALSE);
	}
	else
	{
		obBtnRec.SetRect(nX, nY, nWidth, FontSize+LINE_SPACE);	//调整高度,防止字体底部被截断
		pLabelName->SetBounds(&obBtnRec);
		pLabelName->SetAutoSize(FALSE);
		pLabelName->SetScrollMode(lsmNone);

		TFont tFont;
		tFont.Create(FontSize, FontSize);
		ShowAsShort((TCtrl *)pLabelName, pszString, tFont);
		pLabelName->SetFont(tFont);
		pLabelName->SetColor(CTL_COLOR_TYPE_FORE, Color);
	}

	pLabelName->SetEnabled(FALSE);
	
	return nLabelNameId;
}


//添加Button控件到CoolBarList, 
// pszString必须是动态的(非Static)
// nWidth 为0 -- 自动长度
Int32 CtrlAddItemToCoolBarList_Button(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TUChar* pszString)
{
	TRectangle obBtnRec(0,0,0,0);
	Int32 nButtonId = pListItem->AddCtrl(CTL_CLASS_BUTTON, 20, 5);
	TButton* pButton = static_cast<TButton*>(pWin->GetControlPtr(nButtonId));

	obBtnRec.SetRect(nX, nY, nWidth, nHeight);
	pButton->SetBounds(&obBtnRec);
	pButton->SetEnabled(TRUE);
	pButton->SetCaption(pszString,FALSE);
	return nButtonId;
}

//添加Image控件到CoolBarList, 
// pszString必须是动态的(非Static)
// nWidth 为0 -- 自动长度
Int32 CtrlAddItemToCoolBarList_Image(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TBitmap * pBmp)
{
	TRectangle obBtnRec(0,0,0,0);
	Int32 nImageId = pListItem->AddCtrl(CTL_CLASS_IMAGE, 0, 0);
	TImage* pImage = static_cast<TImage*>(pWin->GetControlPtr(nImageId));

	obBtnRec.SetRect(nX, nY, nWidth, nHeight);
	pImage->SetBounds(&obBtnRec);
	pImage->SetBitmap(pBmp);
	pImage->SetEnabled(TRUE);
	return nImageId;
}

//添加RichView控件到CoolBarList
//nHeight -- 传出参数,RichView控件的高度
Int32 CtrlAddItemToCoolBarList_RichView(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color, Boolean bEnable )
{
	TRectangle Rc_Temp;
	TRectangle obBtnRec(0,0,0,0);
	Int32 nRichViewId = pListItem->AddCtrl(CTL_CLASS_RICHVIEW, 0, 0);
	TRichView* pRichView = static_cast<TRichView*>(pWin->GetControlPtr(nRichViewId));
	obBtnRec.SetRect(nX, nY, nWidth, *nHeight);
	pRichView->SetBounds(&obBtnRec);
	pRichView->SetFrameWidth(0);
	CtrlSetFont((TCtrl*)pRichView, FontSize, Color);

	
	//暂时可点击的只处理单行的
	if(bEnable == TRUE)
		{
		pRichView->SetEnabled(TRUE);
		pRichView->SetMaxVisibleLines(1, TRUE);
		pRichView->SetWordWrapAttr(TRUE);
		pRichView->SetColor(CTL_COLOR_TYPE_BACK, LTBLUE);
		pRichView->SetCaption(pszString,FALSE);
		}
	else
		{
		pRichView->SetCaption(pszString,FALSE);
		pRichView->SetEnabled(FALSE);
		pRichView->SetMaxVisibleLines(pRichView->GetLinesCount(), TRUE);
		pRichView->SetWordWrapAttr(TRUE);
		}
	
	pRichView->SetTransparent(TRUE);
	pRichView->SetScrollBarMode(CTL_SCL_MODE_NONE);
	pRichView->GetBounds(&Rc_Temp);
	
	*nHeight = Rc_Temp.Height();

	return nRichViewId;
}

//添加Mask_Button控件到CoolBarList
Int32 CtrlAddItemToCoolBarList_MaskButton(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pBackImage)
{
	TRectangle obBtnRec(0,0,0,0);
	Int32 nMaskButtonId = pListItem->AddCtrl(CTL_CLASS_MASK_BUTTON, 0, 0);
	TMaskButton* pMaskButton = static_cast<TMaskButton*>(pWin->GetControlPtr(nMaskButtonId));
	
	obBtnRec.SetRect(nX, nY, nWidth, nHeight);
	pMaskButton->SetBounds(&obBtnRec);

	pMaskButton->SetImage(pImage, (nWidth - pImage->GetWidth())/2, (nHeight - pImage->GetHeight())/2 );
	pMaskButton->SetBackImage(pBackImage, (nWidth - pBackImage->GetWidth())/2, (nHeight - pBackImage->GetHeight())/2 );

	return nMaskButtonId;
}

//仅RenRen
//添加评论信息到CoolBarList
Int32 CtrlAddItemToCoolBarList_CommentInfo(TWindow*pWin,  TBarListItem* pListItem, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TUChar* pszString)
{
	TRectangle obBtnRec(0,0,0,0);
	Int32 nMaskButtonId = pListItem->AddCtrl(CTL_CLASS_MASK_BUTTON, 0, 0);
	TMaskButton* pMaskButton = static_cast<TMaskButton*>(pWin->GetControlPtr(nMaskButtonId));
	
	obBtnRec.SetRect(nX, nY, nWidth, nHeight);
	pMaskButton->SetBounds(&obBtnRec);

	const TBitmap * pImgButtonNormal = NULL;
	const TBitmap * pImgButtonSelected = NULL;
	const TBitmap * pImgIconNormal = NULL;
	const TBitmap * pImgIconSelected = NULL;

	pImgButtonNormal = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_CommentInfoBG);
	pImgButtonSelected = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_CommentInfoBG_hl);
	pImgIconNormal = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_CommentInfoIcon);
	pImgIconSelected = TResource::LoadConstBitmap(APP_RE_ID_BITMAP_CommentInfoIcon);
		
	{
		Int32 CaptionX = 0;
		Int32 CaptionW = 0;
		TFont tFont;
		tFont.Create(14, 14);
	
		pMaskButton->SetMaskImage(pImgButtonNormal, pImgButtonSelected, pImgButtonNormal);
		pMaskButton->SetImage(pImgIconNormal, 12, 16);
		pMaskButton->SetSelImage(pImgIconSelected, 12, 16);

		pMaskButton->SetFont(tFont);
		pMaskButton->SetCaption(pszString, 32,  16);
		pMaskButton->SetColor(RGB_COLOR_BLACK, RGB_COLOR_BLACK, RGB_COLOR_BLACK, RGB_COLOR_BLACK, RGB_COLOR_BLACK, RGB_COLOR_BLACK);	
	}

	return nMaskButtonId;
}


//添加Mask_Button控件到Win
Int32 CtrlAddItemToWin_MaskButton(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pImageSel)
{
	TMaskButton* pMaskButton = new TMaskButton();
	Int32 nMaskButtonId = 0;
	
	if(pMaskButton->Create(pWin))
	{
		TRectangle obBtnRec(0,0,0,0);

		nMaskButtonId=pMaskButton->GetId();
		obBtnRec.SetRect(nX, nY, nWidth,  nHeight);
		pMaskButton->SetBounds(&obBtnRec);
		pMaskButton->SetImage(pImage, (nWidth - pImage->GetWidth())/2, (nHeight - pImage->GetHeight())/2 );
		pMaskButton->SetSelImage(pImageSel,(nWidth - pImageSel->GetWidth())/2,(nHeight - pImageSel->GetHeight())/2);		
		pMaskButton->SetEnabled(TRUE);
	}
	
	return nMaskButtonId;
}

//添加Edit控件到Win
Int32 CtrlAddItemToWin_Edit(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord nHeight)
{
	TEdit* pEdit = new TEdit();
	Int32 nEditId = 0;
	
	if(pEdit->Create(pWin))
	{
		TRectangle obBtnRec(0,0,0,0);

		nEditId=pEdit->GetId();
		obBtnRec.SetRect(nX, nY, nWidth,  nHeight);
		pEdit->SetBounds(&obBtnRec);

		pEdit->SetMaxChars(20);
		pEdit->SetUnderline(TRUE);
	}
	
	return nEditId;
}

//添加Image控件到Win
Int32 CtrlAddItemToWin_Image(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pBmp)
{
	TImage* pImage = new TImage();
	Int32 nImageId = 0;
	
	if(pImage->Create(pWin))
	{
		TRectangle obBtnRec(0,0,0,0);

		nImageId=pImage->GetId();
		obBtnRec.SetRect(nX, nY, nWidth, nHeight);
		pImage->SetBounds(&obBtnRec);
		pImage->SetBitmap(pBmp);
		pImage->SetEnabled(TRUE);
	}
	
	return nImageId;
}

//添加Label控件到Win
Int32 CtrlAddItemToWin_Label(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color)
{
	TLabel* pLabel = new TLabel();
	Int32  nLabelID = 0;

	if(pLabel->Create(pWin))	
	{
		nLabelID=pLabel->GetId();
		TRectangle obBtnRec(0,0,0,0);
		if(nWidth == 0)
		{
			nWidth = 80;
			obBtnRec.SetRect(nX, nY, nWidth, FontSize);
			pLabel->SetBounds(&obBtnRec);
			pLabel->SetAutoSize(TRUE);

			CtrlSetFont((TCtrl*)pLabel, FontSize, Color);
			pLabel->SetCaption(pszString,FALSE);
		}
		else
		{
			obBtnRec.SetRect(nX, nY, nWidth, FontSize+LINE_SPACE);
			pLabel->SetBounds(&obBtnRec);
			pLabel->SetAutoSize(FALSE);
			pLabel->SetScrollMode(lsmNone);

			TFont tFont;
			tFont.Create(FontSize, FontSize);
			ShowAsShort((TCtrl *)pLabel, pszString, tFont);
			pLabel->SetFont(tFont);
			pLabel->SetColor(CTL_COLOR_TYPE_FORE, Color);
		}
	}
	return nLabelID;
}


//添加RichView控件到Win
//nHeight -- 传出参数,RichView控件的高度
Int32 CtrlAddItemToWin_RichView(TWindow*pWin, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color, Boolean bEnable )
{
	Int32 nRichViewId = 0;
	TRichView* pRichView = new TRichView();
	if(pRichView->Create(pWin))
	{
		nRichViewId=pRichView->GetId();
		
		TRectangle Rc_Temp;
		TRectangle obBtnRec(0,0,0,0);
		obBtnRec.SetRect(nX, nY, nWidth, *nHeight);
		pRichView->SetBounds(&obBtnRec);
		pRichView->SetFrameWidth(0);
		CtrlSetFont((TCtrl*)pRichView, FontSize, Color);

		
		//暂时可点击的只处理单行的
		if(bEnable == TRUE)
			{
			pRichView->SetEnabled(TRUE);
			pRichView->SetMaxVisibleLines(1, TRUE);
			pRichView->SetWordWrapAttr(TRUE);
			pRichView->SetColor(CTL_COLOR_TYPE_BACK, LTBLUE);
			pRichView->SetCaption(pszString,FALSE);
			}
		else
			{
			pRichView->SetCaption(pszString,FALSE);
			pRichView->SetEnabled(FALSE);
			pRichView->SetMaxVisibleLines(pRichView->GetLinesCount(), TRUE);
			pRichView->SetWordWrapAttr(TRUE);
			}
		
		pRichView->SetTransparent(TRUE);
		pRichView->SetScrollBarMode(CTL_SCL_MODE_NONE);
		pRichView->GetBounds(&Rc_Temp);
		
		*nHeight = Rc_Temp.Height();
	}
	return nRichViewId;
}


//添加Mask_Button控件到Panel
Int32 CtrlAddItemToPanel_MaskButton(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pImage, TBitmap * pImageSel, TBitmap * pBackImage)
{
	TMaskButton* pMaskButton = new TMaskButton();
	Int32 nMaskButtonId = 0;
	
	if(pMaskButton->Create(pPanel))
	{
		TRectangle obBtnRec(0,0,0,0);

		nMaskButtonId=pMaskButton->GetId();
		obBtnRec.SetRect(nX, nY, nWidth,  nHeight);
		pMaskButton->SetBounds(&obBtnRec);
		pMaskButton->SetImage(pImage, (nWidth - pImage->GetWidth())/2, (nHeight - pImage->GetHeight())/2 );
		pMaskButton->SetSelImage(pImageSel,(nWidth - pImageSel->GetWidth())/2,(nHeight - pImageSel->GetHeight())/2);		
		pMaskButton->SetBackImage(pBackImage, (nWidth - pBackImage->GetWidth())/2, (nHeight - pBackImage->GetHeight())/2 );
		pMaskButton->SetEnabled(TRUE);
	}
	
	return nMaskButtonId;
}

//添加Edit控件到Panel
Int32 CtrlAddItemToPanel_Edit(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight)
{
	TEdit* pEdit = new TEdit();
	Int32 nEditId = 0;
	
	if(pEdit->Create(pPanel))
	{
		TRectangle obBtnRec(0,0,0,0);

		nEditId=pEdit->GetId();
		obBtnRec.SetRect(nX, nY, nWidth,  nHeight);
		pEdit->SetBounds(&obBtnRec);

		pEdit->SetMaxChars(20);
		pEdit->SetUnderline(TRUE);
	}
	
	return nEditId;
}


//添加Label控件到Panel
Int32 CtrlAddItemToPanel_Label(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Int32 FontSize, TUChar* pszString, ColorRefType Color)
{
	TLabel* pLabel = new TLabel();
	Int32  nLabelID = 0;

	if(pLabel->Create(pPanel))	
	{
		TRectangle obBtnRec(0,0,0,0);

		nLabelID = pLabel->GetId();
		if(nWidth == 0)
		{
			nWidth = 80;
			obBtnRec.SetRect(nX, nY, nWidth, FontSize);
			pLabel->SetBounds(&obBtnRec);
			pLabel->SetAutoSize(TRUE);

			CtrlSetFont((TCtrl*)pLabel, FontSize, Color);
			pLabel->SetCaption(pszString,FALSE);
		}
		else
		{
			obBtnRec.SetRect(nX, nY, nWidth, FontSize+LINE_SPACE);
			pLabel->SetBounds(&obBtnRec);
			pLabel->SetAutoSize(FALSE);
			pLabel->SetScrollMode(lsmNone);

			TFont tFont;
			tFont.Create(FontSize, FontSize);
			ShowAsShort((TCtrl *)pLabel, pszString, tFont);
			pLabel->SetFont(tFont);
			pLabel->SetColor(CTL_COLOR_TYPE_FORE, Color);
		}
	}
	return nLabelID;
}

//添加RichView控件到Panel
Int32 CtrlAddItemToPanel_RichView(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord* nHeight, Int32 FontSize, TUChar* pszString, ColorRefType Color)
{
	TRichView* pRichView = new TRichView();
	Int32 nRichViewId = 0;

	if(pRichView->Create(pPanel))	
	{
		nRichViewId = pRichView->GetId();
			
		TRectangle Rc_Temp;
		TRectangle obBtnRec(0,0,0,0);
		obBtnRec.SetRect(nX, nY, nWidth, *nHeight);
		pRichView->SetBounds(&obBtnRec);
		CtrlSetFont((TCtrl*)pRichView, FontSize, Color);
		pRichView->SetCaption(pszString,FALSE);

		pRichView->SetEnabled(FALSE);
		pRichView->SetWordWrapAttr(TRUE);
		pRichView->SetTransparent(TRUE);
		pRichView->SetScrollBarMode(CTL_SCL_MODE_NONE);
		pRichView->SetMaxVisibleLines(pRichView->GetLinesCount(), TRUE);
		pRichView->GetBounds(&Rc_Temp);
		
		*nHeight = Rc_Temp.Height();
	}
	return nRichViewId;
}

//添加Image控件到Panel
Int32 CtrlAddItemToPanel_Image(TWindow*pWin, TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Coord nHeight, TBitmap * pBmp)
{
	TImage* pImage = new TImage();
	Int32 nImageId = 0;
	
	if(pImage->Create(pPanel))
	{
		TRectangle obBtnRec(0,0,0,0);

		nImageId=pImage->GetId();
		obBtnRec.SetRect(nX, nY, nWidth, nHeight);
		pImage->SetBounds(&obBtnRec);
		pImage->SetBitmap(pBmp);
		pImage->SetEnabled(TRUE);
	}
	
	return nImageId;
}


//添加Button控件到Panel
Int32 CtrlAddItemToPanel_Button(TWindow*pWin,  TPanel* pPanel, Coord nX, Coord nY, Coord nWidth, Int32 nHeight, TUChar* pszString)
{
	TRectangle obBtnRec(0,0,0,0);
	TButton* pButton = new TButton();
	Int32 nButtonId = -1;
	if(pButton->Create(pPanel))
	{
		 nButtonId = pButton->GetId();

		obBtnRec.SetRect(nX, nY, nWidth, nHeight);
		pButton->SetBounds(&obBtnRec);
		pButton->SetEnabled(TRUE);
		pButton->SetCaption(pszString,FALSE);
	}
	return nButtonId;
}

#if(LCD_SIZE == LCD_HVGA )
#define BUBBLE_OFFSET_X	(5)
#define BUBBLE_LOGO_W		(HEADMASKBUTTON_W)
#define BUBBLE_LOGO_H		(HEADMASKBUTTON_H)
#define BUBBLE_COOLBAR_W	(240)
#define BUBBLE_FIX_W		(10)
#elif(LCD_SIZE == LCD_WVGA )
#define BUBBLE_OFFSET_X	(5)
#define BUBBLE_LOGO_W		(HEADMASKBUTTON_W)
#define BUBBLE_LOGO_H		(HEADMASKBUTTON_H)
#define BUBBLE_COOLBAR_W	(SCR_W - BUBBLE_LOGO_W - 20)
#define BUBBLE_FIX_W		(10)
#endif

//添加BubbleDialog控件到Panel
Int32 CtrlCreateToPanel_BubbleDialog(TWindow*pWin,  TPanel* pPanel, tBubbleDialogItemData  BubbleItemData, Coord* nHeight, Int32* OptionButtonID, Int32* HeadMaskID)
{
	
	TMaskButton* pLogo =new TMaskButton();
	TCoolBarList* pCoolBar = new TCoolBarList();
	TImage* pFix = new TImage();
	Int32 nLogoID = 0;
	Int32 nNameID = 0;
	//头像
	if(pLogo->Create(pPanel))
	{
		TBitmap * pBackImage = (TBitmap*)TResource::LoadConstBitmap(APP_RE_ID_BITMAP_head1);
		TRectangle Rc_Logo(0, 0, 0,  0);

		nLogoID=pLogo->GetId();
		if(BubbleItemData.bLeft)
			Rc_Logo.SetRect(BUBBLE_OFFSET_X, BubbleItemData.nY,  BUBBLE_LOGO_W,  BUBBLE_LOGO_H);
		else
			Rc_Logo.SetRect(BUBBLE_OFFSET_X+BUBBLE_COOLBAR_W+BUBBLE_FIX_W, BubbleItemData.nY, BUBBLE_LOGO_W,  BUBBLE_LOGO_H);
		pLogo->SetBounds(&Rc_Logo);
	    	pLogo->SetImage(BubbleItemData.pImage, (BUBBLE_LOGO_W - BubbleItemData.pImage->GetWidth())/2, (BUBBLE_LOGO_H - BubbleItemData.pImage->GetHeight())/2 );
		pLogo->SetBackImage(pBackImage, (BUBBLE_LOGO_W - pBackImage->GetWidth())/2, (BUBBLE_LOGO_H - pBackImage->GetHeight())/2 );
	}

	//对话图标
	if(pFix->Create(pPanel))
	{
		TBitmap * pImage = NULL;
		TRectangle Rc_Fix(0,0,0,0);
		if(BubbleItemData.bLeft)
		{
			pImage = (TBitmap *)TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Comment_Left);
			Rc_Fix.SetRect(BUBBLE_OFFSET_X+BUBBLE_LOGO_W+1, BubbleItemData.nY+30, pImage->GetWidth(), pImage->GetHeight());
		}
		else
		{
			pImage = (TBitmap *)TResource::LoadConstBitmap(APP_RE_ID_BITMAP_Comment_Right);
			Rc_Fix.SetRect(BUBBLE_OFFSET_X+BUBBLE_COOLBAR_W-1, BubbleItemData.nY+30, pImage->GetWidth(), pImage->GetHeight());
		}
		pFix->SetBounds(&Rc_Fix);
	    	pFix->SetBitmap(pImage);
	}
	
	//Add CoolBarList
	if(pCoolBar->Create(pPanel))
	{
		TBarRowList* 	lpRowList = NULL;
		TBarRow* 		lpRow = NULL;
		TBarListItem* 	lpItem = NULL;
		TBitmap*		lpBitmap = NULL;
		TRectangle 		obBtnRec(0, 0, 0, 0);
		if(BubbleItemData.bLeft)
			obBtnRec.SetRect(BUBBLE_OFFSET_X+BUBBLE_LOGO_W+BUBBLE_FIX_W, BubbleItemData.nY, BUBBLE_COOLBAR_W, 100);
		else
			obBtnRec.SetRect(BUBBLE_OFFSET_X, BubbleItemData.nY, BUBBLE_COOLBAR_W, 100);
		TRectangle 		Rc_CoolBarList;
		pCoolBar->SetBounds(&obBtnRec);
		pCoolBar->GetBounds(&Rc_CoolBarList);
		pCoolBar->SetEnabled(TRUE);
		lpRowList = pCoolBar->Rows();
		if (lpRowList)
		{
			lpRowList->BeginUpdate();
			lpRow = lpRowList->AppendRow();
			lpRowList->EndUpdate(); 
			if(lpRow)
			{
				lpRow->SetEnabled(TRUE);
				lpItem = lpRow->AppendItem();
				if (lpItem)
				{
					lpItem->SetCaption(NULL);                       
					lpItem->SetIndicatorType(itNone);
					lpItem->SetEnabled(TRUE);
					//名字
					//CtrlAddItemToCoolBarList_Lable(pWin, lpItem, 5, 10, 0, FONT_NORMAL, BubbleItemData.pszName);
					Coord NameHeight = 0;
					int nWidth = GetShowAllStringWidthEx(BubbleItemData.pszName, FONT_NORMAL);
					nNameID = CtrlAddItemToCoolBarList_RichView(pWin, lpItem, 5, 10, nWidth, &NameHeight, FONT_NORMAL, BubbleItemData.pszName, RGB_COLOR_BLUE, TRUE);

					//标志图标(如悄悄话等)
					if(BubbleItemData.FlagIconResID != 0)
						CtrlAddItemToCoolBarList_Image(pWin,  lpItem, (BUBBLE_COOLBAR_W - 60 - 5*2), 10, 60, 20, (TBitmap*)TResource::LoadConstBitmap(BubbleItemData.FlagIconResID));
					
					//评论内容
					*nHeight = 0;
					CtrlAddItemToCoolBarList_RichView(pWin,  lpItem, 5, 40, (BUBBLE_COOLBAR_W - 5*2), nHeight, FONT_MIDDLE, BubbleItemData.pszContent);

					//时间
					CtrlAddItemToCoolBarList_Lable(pWin, lpItem, 5, (*nHeight + 40 + 5), 0, FONT_SMALL, BubbleItemData.pszTime, RGB_COLOR_LIGHTBLUE);

					//操作按钮(如回复等)
					if(BubbleItemData.OptionButtonResID != 0)
						*OptionButtonID = CtrlAddItemToCoolBarList_Button(pWin,  lpItem, (BUBBLE_COOLBAR_W - TITLE_BUTTON_W - 20), (*nHeight + 60), TITLE_BUTTON_W, TITLE_BUTTON_H, (TUChar*)TResource::LoadConstString(BubbleItemData.OptionButtonResID));

					//如果有标志按钮或操作按钮,高度要加大
					if (BubbleItemData.OptionButtonResID != 0) 
						(*nHeight) = (*nHeight) + 60 + TITLE_BUTTON_H;
					else
						(*nHeight) = (*nHeight) + 60;
						
					lpItem->SetHeight(*nHeight );							
				}

			}
		}
		pCoolBar->GetBounds(&Rc_CoolBarList);
		//Item_Offset_Y = Rc_CoolBarList.Y() + Rc_CoolBarList.Height() + 10;
	}

	*HeadMaskID = nLogoID;
	return nNameID;
}

//获取控件宽度
Coord GetCtrlWidth(TWindow*pWin, Int32 nCtrlID)
{
	if(pWin == NULL)
		return 0;
	
	TCtrl *pCtrl = static_cast<TCtrl*>(pWin->GetControlPtr(nCtrlID));
	TRectangle Rc_Temp;
	pCtrl->GetBounds(&Rc_Temp);

	return Rc_Temp.Width();
}

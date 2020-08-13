
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'initialisation & de manipulation des menus
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Locale.h"
#include "Menus.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'initialisation					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation des éléments des menus «««««««««««««««««««««««««««»

HMENU Menu_Create(CUSTOMMENUTEMPLATE *pTemplate)
{
	HMENU			hMenu;
	CUSTOMMENUTEMPLATE*	pTemplatePtr;

	hMenu = CreateMenu();
	if (hMenu)
		{
		pTemplatePtr = pTemplate;
		if (!Menu_Create_Append(hMenu,&pTemplatePtr))
			{
			Menu_Release(hMenu,pTemplate);
			return(NULL);
			}
		}
	return(hMenu);
}

//--- Ajoute un élément ---

BOOL Menu_Create_Append(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	while (1)
		{
		switch((*pTemplatePtr)->uType)
			{
			case MENU_POPUP:
				if (!Menu_Create_AppendMenu(hMenu,pTemplatePtr)) return(FALSE);
				break;
			case MENU_SUBPOPUP:
				if (!Menu_Create_AppendSubMenu(hMenu,pTemplatePtr)) return(FALSE);
				break;
			case MENU_ITEM:
				if (!Menu_Create_AppendItem(hMenu,pTemplatePtr)) return(FALSE);
				break;
			case MENU_SEPARATOR:
				if (!Menu_Create_AppendSeparator(hMenu,pTemplatePtr)) return(FALSE);
				break;
			case MENU_BLANK:
				if (!Menu_Create_AppendBlank(hMenu,pTemplatePtr)) return(FALSE);
				break;
			case MENU_STOP:
				(*pTemplatePtr)++;
				return(TRUE);
			default:(*pTemplatePtr)++;
				break;
			}
		}

	return(TRUE);
}

//--- Ajoute un menu ---

BOOL Menu_Create_AppendMenu(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	HMENU		hNewMenu;
	MENUITEMINFO	MenuItem;

	hNewMenu = CreatePopupMenu();
	if (hNewMenu)
		{
		MenuItem.cbSize = sizeof(MENUITEMINFO);
		MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_SUBMENU|MIIM_DATA;
		MenuItem.fType = MFT_STRING;
		MenuItem.fState = (*pTemplatePtr)->uFlags;
		MenuItem.wID = (*pTemplatePtr)->uId;
		MenuItem.hSubMenu = hNewMenu;
		MenuItem.dwItemData = (ULONG_PTR)*pTemplatePtr;
		MenuItem.dwTypeData = Locale_GetText((*pTemplatePtr)->uId);
		MenuItem.cch = wcslen(MenuItem.dwTypeData);
		(*pTemplatePtr)++;
		if (!InsertMenuItem(hMenu,-1,TRUE,&MenuItem)) return(FALSE);
		return(Menu_Create_Append(hNewMenu,pTemplatePtr));
		}

	(*pTemplatePtr)++;
	return(FALSE);
}

//--- Ajoute un sous-menu ---

BOOL Menu_Create_AppendSubMenu(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	HMENU		hNewMenu;
	MENUITEMINFO	MenuItem;

	hNewMenu = CreatePopupMenu();
	if (hNewMenu)
		{
		MenuItem.cbSize = sizeof(MENUITEMINFO);
		MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_SUBMENU|MIIM_DATA;
		MenuItem.fType = MFT_OWNERDRAW;
		MenuItem.fState = (*pTemplatePtr)->uFlags;
		MenuItem.wID = (*pTemplatePtr)->uId;
		MenuItem.hSubMenu = hNewMenu;
		MenuItem.dwItemData = (ULONG_PTR)*pTemplatePtr;
		MenuItem.dwTypeData = NULL;
		(*pTemplatePtr)->pszText = Locale_GetText((*pTemplatePtr)->uId);
		(*pTemplatePtr)->uTextLen = wcslen((*pTemplatePtr)->pszText);
		(*pTemplatePtr)++;
		if (!InsertMenuItem(hMenu,-1,TRUE,&MenuItem)) return(FALSE);
		return(Menu_Create_Append(hNewMenu,pTemplatePtr));
		}

	(*pTemplatePtr)++;
	return(FALSE);
}


//--- Insère une entrée dans un menu ---

BOOL Menu_Create_AppendItem(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	MENUITEMINFO	MenuItem;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
	MenuItem.fType = MFT_OWNERDRAW;
	MenuItem.fState = (*pTemplatePtr)->uFlags;
	MenuItem.wID = (*pTemplatePtr)->uId;
	MenuItem.dwItemData = (ULONG_PTR)*pTemplatePtr;
	MenuItem.dwTypeData = NULL;

	(*pTemplatePtr)->pszText = Locale_GetText((*pTemplatePtr)->uId);
	(*pTemplatePtr)->uTextLen = wcslen((*pTemplatePtr)->pszText);

	if ((*pTemplatePtr)->uKeyId) (*pTemplatePtr)->pszKey = Locale_GetText((*pTemplatePtr)->uKeyId);
	if ((*pTemplatePtr)->uIconId) (*pTemplatePtr)->hIcon = LoadImage(App.hInstance,MAKEINTRESOURCE((*pTemplatePtr)->uIconId),IMAGE_ICON,MENU_IMAGEWIDTH,MENU_IMAGEHEIGHT,LR_DEFAULTCOLOR);
	if ((*pTemplatePtr)->uSpecialFlags&MENU_FLAG_CHECKBOX) (*pTemplatePtr)->hImage = LoadImage(NULL,MAKEINTRESOURCE(OBM_CHECK),IMAGE_BITMAP,MENU_IMAGEWIDTH,MENU_IMAGEHEIGHT,LR_DEFAULTCOLOR);
	if ((*pTemplatePtr)->uSpecialFlags&MENU_FLAG_SEPARATOR) MenuItem.fType |= MFT_SEPARATOR;

	(*pTemplatePtr)++;
	return(InsertMenuItem(hMenu,-1,TRUE,&MenuItem));
}

//--- Insère un séparateur ---

BOOL Menu_Create_AppendSeparator(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	MENUITEMINFO	MenuItem;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_TYPE|MIIM_ID|MIIM_STATE;
	MenuItem.fType = MFT_SEPARATOR;
	MenuItem.fState = (*pTemplatePtr)->uFlags;
	MenuItem.wID = (*pTemplatePtr)->uId;

	(*pTemplatePtr)++;
	return(InsertMenuItem(hMenu,-1,TRUE,&MenuItem));
}

//--- Insère un séparateur vide ---

BOOL Menu_Create_AppendBlank(HMENU hMenu, CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	MENUITEMINFO	MenuItem;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
	MenuItem.fType = MFT_OWNERDRAW|MFT_SEPARATOR;
	MenuItem.fState = MFS_GRAYED;
	MenuItem.wID = (*pTemplatePtr)->uId;
	MenuItem.dwItemData = (ULONG_PTR)*pTemplatePtr;
	MenuItem.dwTypeData = NULL;

	(*pTemplatePtr)->bIsBlank = TRUE;

	(*pTemplatePtr)++;
	return(InsertMenuItem(hMenu,-1,TRUE,&MenuItem));
}


// «»»» Libération des éléments des menus «««««««««««««««««««««««««««««««»

void Menu_Release(HMENU hMenu, CUSTOMMENUTEMPLATE *pTemplate)
{
	CUSTOMMENUTEMPLATE*	pTemplatePtr;

	if (hMenu) DestroyMenu(hMenu);
	pTemplatePtr = pTemplate;
	Menu_Release_ObjectsLoop(&pTemplatePtr);
	return;
}

//--- Boucle de vérification ---

void Menu_Release_ObjectsLoop(CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	while (1)
		{
		switch((*pTemplatePtr)->uType)
			{
			case MENU_POPUP:
			case MENU_SUBPOPUP:
				Menu_Release_Objects(pTemplatePtr);
				Menu_Release_ObjectsLoop(pTemplatePtr);
				break;
			case MENU_ITEM:
			case MENU_SEPARATOR:
			case MENU_BLANK:
				Menu_Release_Objects(pTemplatePtr);
				break;
			case MENU_STOP:
				(*pTemplatePtr)++;
				return;
			default:(*pTemplatePtr)++;
				break;
			}
		}
	return;
}

//--- Libération des ressources ---

void Menu_Release_Objects(CUSTOMMENUTEMPLATE **pTemplatePtr)
{
	if ((*pTemplatePtr)->hIcon) DestroyIcon((*pTemplatePtr)->hIcon);
	if ((*pTemplatePtr)->hImage) DeleteObject((*pTemplatePtr)->hImage);
	(*pTemplatePtr)->hIcon = NULL;
	(*pTemplatePtr)->hImage = NULL;
	(*pTemplatePtr)++;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Calcul la taille des éléments «««««««««««««««««««««««««««««««««««»

void Menu_MeasureItems(HWND hWnd, UINT uCtlId, MEASUREITEMSTRUCT *pMeasure)
{
	HDC			hDC;
	HFONT			hDefFont;
	SIZE			Size;
	CUSTOMMENUTEMPLATE*	pMenuItem;

	if (uCtlId) return;

	pMenuItem = (CUSTOMMENUTEMPLATE *)pMeasure->itemData;

	if (pMenuItem->bIsBlank)
		{
		pMeasure->itemWidth = 0;
		pMeasure->itemHeight = MENU_BLANKHEIGHT;
		return;
		}

	hDC = GetDC(hWnd);
	hDefFont = SelectObject(hDC,App.Font.hFont);

	GetTextExtentPoint32(hDC,pMenuItem->pszText,pMenuItem->uTextLen,&Size);
	if (pMenuItem->uSpecialFlags&MENU_FLAG_FILENAME && Size.cx > MENU_FILENAMEMAXWIDTH) Size.cx = MENU_FILENAMEMAXWIDTH;
	pMeasure->itemWidth = Size.cx+MENU_RIGHTPADDING;
	pMeasure->itemHeight = Size.cy;
	if (Size.cy < 21) pMeasure->itemHeight = 21;
	if (pMenuItem->uSpecialFlags&MENU_FLAG_CAPTION) pMeasure->itemHeight += 2;

	if (pMenuItem->uKeyId)
		{
		GetTextExtentPoint32(hDC,pMenuItem->pszKey,wcslen(pMenuItem->pszKey),&Size);
		pMeasure->itemWidth += Size.cx+12;
		}

	SelectObject(hDC,hDefFont);
	ReleaseDC(hWnd,hDC);
	return;
}


// «»»» Affichage des éléments ««««««««««««««««««««««««««««««««««««««««««»

void Menu_DrawItems(DRAWITEMSTRUCT *pDraw)
{
	HFONT			hfontDefault;
	TEXTMETRIC		tmFontInfo;
	MENUITEMINFO	 	MenuItemInfo;
	CUSTOMMENUTEMPLATE*	pMenuItem;
	COLORREF		crTextDefault;
	COLORREF		crBackDefault;
	COLORREF		crBack;
	RECT			rcWindow;
	SIZE			Size;
	UINT			uTextFlags;
	UINT			uImageFlags;
	LONG			Width,Height;
	LONG			X,Y;
	int			iOldMode;

	MenuItemInfo.cbSize = sizeof(MenuItemInfo);
	MenuItemInfo.fMask = MIIM_DATA;
	GetMenuItemInfo((HMENU)pDraw->hwndItem,pDraw->itemID,FALSE,&MenuItemInfo);
	pMenuItem = (CUSTOMMENUTEMPLATE *)MenuItemInfo.dwItemData;
	if (!pMenuItem) return;
	Width = 16;
	Height = 16;

	if (pMenuItem->bIsBlank)
		{
		FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_MENU));
		return;
		}

	if (!(pMenuItem->uSpecialFlags&MENU_FLAG_SEPARATOR) && pDraw->itemState&ODS_SELECTED)
		{
		crBack = COLOR_HIGHLIGHT;
		crTextDefault = SetTextColor(pDraw->hDC,GetSysColor(COLOR_HIGHLIGHTTEXT));
		crBackDefault = SetBkColor(pDraw->hDC,GetSysColor(COLOR_HIGHLIGHT));
		}
	else
		{
		crBack = COLOR_MENU;
		crTextDefault = SetTextColor(pDraw->hDC,GetSysColor(COLOR_MENUTEXT));
		crBackDefault = SetBkColor(pDraw->hDC,GetSysColor(COLOR_MENU));
		}

	// Affichage du fond

	uTextFlags = 0;
	uImageFlags = 0;

	if (pDraw->itemState & ODS_GRAYED)
		{
		uImageFlags = DSS_DISABLED;
		if (pDraw->itemState&ODS_SELECTED) SetTextColor(pDraw->hDC,GetSysColor(COLOR_GRAYTEXT));
		else uTextFlags = DSS_DISABLED;
		}

	CopyRect(&rcWindow,&pDraw->rcItem);
	if (pMenuItem->hIcon) rcWindow.left += 8+Width;
	FillRect(pDraw->hDC,&rcWindow,(HBRUSH)((DWORD_PTR)crBack+1));
	rcWindow.right = rcWindow.left-2;
	rcWindow.left = pDraw->rcItem.left;
	if (pMenuItem->hIcon && pDraw->itemState&ODS_SELECTED) DrawEdge(pDraw->hDC,&rcWindow,BDR_RAISEDINNER,BF_RECT);
	else if (pMenuItem->hIcon) FillRect(pDraw->hDC,&rcWindow,(HBRUSH)((DWORD_PTR)crBack+1));

	// Affichage des images

	if (pMenuItem->hImage && pDraw->itemState&ODS_CHECKED)
		{
		X = pDraw->rcItem.left+4;
		Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-Height)/2;
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pMenuItem->hImage,0,X,Y,Width,Height,DST_BITMAP|uImageFlags);
		}

	if (pMenuItem->hIcon)
		{
		X = pDraw->rcItem.left+4;
		Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-Height)/2;
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pMenuItem->hIcon,0,X,Y,Width,Height,DST_ICON|uImageFlags);
		}

	if (pMenuItem->uSpecialFlags&MENU_FLAG_COLORICON && pMenuItem->crColor)
		{
		HBRUSH	hBrush;
		RECT	rcIcon;
		rcIcon.left = pDraw->rcItem.left+4;
		rcIcon.top = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-Height)/2;
		rcIcon.right = rcIcon.left+16;
		rcIcon.bottom = rcIcon.top+16;
		hBrush = CreateSolidBrush(*pMenuItem->crColor);
		if (hBrush)
			{
			FillRect(pDraw->hDC,&rcIcon,hBrush);
			DeleteObject(hBrush);
			}
		}

	hfontDefault = SelectObject(pDraw->hDC,App.Font.hFont);
	GetTextMetrics(pDraw->hDC,&tmFontInfo);
	X = pDraw->rcItem.left;
	Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-tmFontInfo.tmHeight-tmFontInfo.tmExternalLeading)/2;
	X += 10+Width;

	// Affichage du texte

	if (pMenuItem->uSpecialFlags&MENU_FLAG_SEPARATOR)
		{
		HPEN		hpenLine;
		HPEN		hpenPrev;
		LONG		LineY;

		LineY = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top)/2;
		hpenLine = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DSHADOW));
		GetTextExtentPoint32(pDraw->hDC,pMenuItem->pszText,pMenuItem->uTextLen,&Size);
		if (hpenLine)
			{
			hpenPrev = SelectObject(pDraw->hDC,hpenLine);
			if (hpenPrev)
				{
				MoveToEx(pDraw->hDC,0,LineY,NULL);
				LineTo(pDraw->hDC,pDraw->rcItem.left+8,LineY);
				MoveToEx(pDraw->hDC,pDraw->rcItem.left+Size.cx+16,LineY,NULL);
				LineTo(pDraw->hDC,pDraw->rcItem.right,LineY);
				SelectObject(pDraw->hDC,hpenPrev);
				}
			DeleteObject(hpenLine);
			}
		hpenLine = CreatePen(PS_SOLID,0,GetSysColor(COLOR_3DHILIGHT));
		if (hpenLine)
			{
			hpenPrev = SelectObject(pDraw->hDC,hpenLine);
			if (hpenPrev)
				{
				MoveToEx(pDraw->hDC,0,LineY+1,NULL);
				LineTo(pDraw->hDC,pDraw->rcItem.left+8,LineY+1);
				MoveToEx(pDraw->hDC,pDraw->rcItem.left+Size.cx+16,LineY+1,NULL);
				LineTo(pDraw->hDC,pDraw->rcItem.right,LineY+1);
				SelectObject(pDraw->hDC,hpenPrev);
				}
			DeleteObject(hpenLine);
			}
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pMenuItem->pszText,0,pDraw->rcItem.left+12,Y,pDraw->rcItem.right-pDraw->rcItem.left,pDraw->rcItem.bottom-pDraw->rcItem.top,DST_PREFIXTEXT|DSS_DISABLED);
		}

	else if (pMenuItem->uSpecialFlags&MENU_FLAG_CAPTION)
		{
		HFONT		hfontBold;
		HFONT		hfontPrev;
		LOGFONT 	lf;

		CopyRect(&rcWindow,&pDraw->rcItem);
		rcWindow.bottom -= 2;
		FillRect(pDraw->hDC,&rcWindow,GetStockObject(BLACK_BRUSH));
		GetObject(App.Font.hFont,sizeof(LOGFONT),&lf);
		lf.lfWeight = FW_BOLD;
		hfontBold = CreateFontIndirect(&lf);
		if (hfontBold) hfontPrev = SelectObject(pDraw->hDC,hfontBold);
		iOldMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		SetTextColor(pDraw->hDC,0x00FFFFFF);
		GetTextExtentPoint32(pDraw->hDC,pMenuItem->pszText,pMenuItem->uTextLen,&Size);
		TextOut(pDraw->hDC,pDraw->rcItem.right-Size.cx-4,Y-1,pMenuItem->pszText,pMenuItem->uTextLen);
		SetBkMode(pDraw->hDC,iOldMode);
		if (hfontBold)
			{
			if (hfontPrev) SelectObject(pDraw->hDC,hfontPrev);
			DeleteObject(hfontBold);
			}
		}

	else if (pMenuItem->uSpecialFlags&MENU_FLAG_FILENAME)
		{
		RECT	rcText;

		rcText.left = (pMenuItem->uSpecialFlags&MENU_FLAG_NOICON)?(pDraw->rcItem.left+8):X;
		rcText.top = pDraw->rcItem.top;
		rcText.right = pDraw->rcItem.right-MENU_RIGHTPADDING;
		rcText.bottom = pDraw->rcItem.bottom;
		DrawText(pDraw->hDC,pMenuItem->pszText,-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
		}

	else DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pMenuItem->pszText,0,(pMenuItem->uSpecialFlags&MENU_FLAG_NOICON)?(pDraw->rcItem.left+8):X,Y,pDraw->rcItem.right-pDraw->rcItem.left,pDraw->rcItem.bottom-pDraw->rcItem.top,DST_PREFIXTEXT|uTextFlags);

	// Affichage des touches de raccourci

	if (pMenuItem->pszKey)
		{
		GetTextExtentPoint32(pDraw->hDC,pMenuItem->pszKey,wcslen(pMenuItem->pszKey),&Size);
		X = pDraw->rcItem.right-8-Size.cx;
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pMenuItem->pszKey,0,X,Y,pDraw->rcItem.right-X,pDraw->rcItem.bottom-pDraw->rcItem.top,DST_TEXT|uTextFlags);
		}

	SelectObject(pDraw->hDC,hfontDefault);
	SetBkColor(pDraw->hDC,crBackDefault);
	SetTextColor(pDraw->hDC,crTextDefault);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de modification					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Modifie l'état d'une entrée «««««««««««««««««««««««««««««««««««««»

void Menu_SetFlag(HMENU hMenu, UINT uId, BOOL bSet)
{
	MENUITEMINFO	MenuItem;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_STATE;
	MenuItem.fState = bSet?MFS_CHECKED:MFS_UNCHECKED;
	SetMenuItemInfo(hMenu,uId,FALSE,&MenuItem);
	return;
}


// «»»» Retrouve l'état d'une entrée ««««««««««««««««««««««««««««««««««««»

BOOL Menu_GetFlag(HMENU hMenu, UINT uId)
{
	MENUITEMINFO	MenuItem;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_STATE;
	MenuItem.fState = 0;
	GetMenuItemInfo(hMenu,uId,FALSE,&MenuItem);
	return(MenuItem.fState&MFS_CHECKED?TRUE:FALSE);
}

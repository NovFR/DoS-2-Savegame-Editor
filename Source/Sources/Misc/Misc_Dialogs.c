
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines des boîtes de dialogue
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Dialogs.h"
#include "Requests.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage d'une boîte de dialogue				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ouverture ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Dialog_Open(DIALOG *pDialog)
{
	INT_PTR		iResult;

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(pDialog->uResID),pDialog->hwndParent,Dialog_Proc,(LPARAM)pDialog);
	if (!iResult || iResult == -1)
		{
		Request_PrintError(pDialog->hwndParent,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return(0);
		}
	if (iResult == IDCANCEL) return(0);
	return(1);
}


// «»»» Messages ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Dialog_Proc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	DIALOG*		pDialog;

	if (uMsgId == WM_INITDIALOG)
		{
		RECT	rcDialog;
		int	Height = 0;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pDialog = (DIALOG *)lParam;

		if (IsWindow(GetDlgItem(hDlg,100)))
			{
			Height = Dialog_GetInfoHeight(hDlg,100,pDialog->pszText);
			GetWindowRect(hDlg,&rcDialog);
			SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
			GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
			SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);
			}

		if (pDialog->fnInit) { if (!pDialog->fnInit(hDlg,&rcDialog,Height,pDialog)) { EndDialog(hDlg,-1); return(FALSE); } }

		if (IsWindow(GetDlgItem(hDlg,IDOK)))
			{
			Dialog_OffsetY(hDlg,IDOK,Height);
			SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
			}
		if (IsWindow(GetDlgItem(hDlg,IDCANCEL)))
			{
			Dialog_OffsetY(hDlg,IDCANCEL,Height);
			SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
			}
		SetWindowText(hDlg,pDialog->pszTitle);
		Dialog_CenterWindow(hDlg,pDialog->hwndParent);
		return(FALSE);
		}

	pDialog = (DIALOG *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(pDialog->pszText,(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				}
			if (pDialog->fnDraw) return(pDialog->fnDraw(wParam,(DRAWITEMSTRUCT *)lParam,pDialog));
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							if (pDialog->fnOk) { if (!pDialog->fnOk(hDlg,pDialog)) return(TRUE); }
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
							if (pDialog->fnCancel) { if (!pDialog->fnCancel(hDlg,pDialog)) return(TRUE); }
							EndDialog(hDlg,IDCANCEL);
							return(TRUE);
						}
					if (pDialog->fnClick)
						{
						pDialog->fnClick(hDlg,LOWORD(wParam),pDialog);
						return(TRUE);
						}
					break;
				}
			break;

		case WM_CLOSE:
			EndDialog(hDlg,IDCANCEL);
			return(TRUE);
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions d'affichage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'un bouton composé d'un texte ««««««««««««««««««««««««»

void Dialog_DrawTextButton(WCHAR *pszText, DRAWITEMSTRUCT *pDraw)
{
	UINT		uState;
	RECT		rcDrawArea;
	SIZE		Size;
	COLORREF	crText;
	int		X,Y;
	int		iBkMode;

	CopyRect(&rcDrawArea,&pDraw->rcItem);

	if (App.hThemeButton)
		{
		if (pDraw->itemState&ODS_DISABLED) uState = PBS_DISABLED;
		else if (pDraw->itemState&ODS_SELECTED) uState = PBS_PRESSED;
		else if (pDraw->itemState&ODS_HOTLIGHT) uState = PBS_HOT;
		else if (pDraw->itemState&ODS_FOCUS) uState = PBS_DEFAULTED;
		else uState = PBS_NORMAL;
		DrawThemeBackground(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,NULL);
		GetThemeBackgroundContentRect(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,&rcDrawArea);
		DrawThemeText(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,pszText,-1,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS,0,&rcDrawArea);
		}
	else
		{
		uState = DFCS_ADJUSTRECT;
		if (pDraw->itemState&ODS_DISABLED) uState |= DFCS_INACTIVE;
		if (pDraw->itemState&ODS_SELECTED) uState |= DFCS_PUSHED;
		DrawFrameControl(pDraw->hDC,&rcDrawArea,DFC_BUTTON,DFCS_BUTTONPUSH|uState);

		crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
		X = rcDrawArea.left+(rcDrawArea.right-rcDrawArea.left-Size.cx)/2;
		Y = rcDrawArea.top+(rcDrawArea.bottom-rcDrawArea.top-Size.cy)/2;
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pszText,0,X,Y,Size.cx,Size.cy,DST_TEXT|((uState&DFCS_INACTIVE)?DSS_DISABLED:0));
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crText);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&rcDrawArea);
	return;
}


// «»»» Affichage d'un bouton composé d'une flèche ««««««««««««««««««««««»

void Dialog_DrawArrowButton(UINT uScroll, DRAWITEMSTRUCT *pDraw)
{
	HPEN		hPen;
	RECT		rcDraw;
	UINT		uState;
	int		iColor;

	CopyRect(&rcDraw,&pDraw->rcItem);

	if (App.hThemeButton)
		{
		if (pDraw->itemState&ODS_DISABLED) uState = PBS_DISABLED;
		else if (pDraw->itemState&ODS_SELECTED) uState = PBS_PRESSED;
		else if (pDraw->itemState&ODS_HOTLIGHT) uState = PBS_HOT;
		else if (pDraw->itemState&ODS_FOCUS) uState = PBS_DEFAULTED;
		else uState = PBS_NORMAL;
		DrawThemeBackground(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,NULL);
		GetThemeBackgroundContentRect(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,&rcDraw);
		}
	else
		{
		uState = DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;
		if (pDraw->itemState&ODS_DISABLED) uState |= DFCS_INACTIVE;
		if (pDraw->itemState&ODS_SELECTED) uState |= DFCS_PUSHED;
		DrawFrameControl(pDraw->hDC,&rcDraw,DFC_BUTTON,uState);
		}

	iColor = (pDraw->itemState&ODS_DISABLED)?COLOR_GRAYTEXT:COLOR_BTNTEXT;
	hPen = CreatePen(PS_SOLID,1,GetSysColor(iColor));
	if (hPen)
		{
		HPEN		hDefPen;
		HBRUSH		hDefBrush;
		POINT		ptPoly[3];
		int		X,Y,W,H,S;

		W = rcDraw.right-rcDraw.left;
		H = rcDraw.bottom-rcDraw.top;
		X = rcDraw.left+W/2;
		Y = rcDraw.top+H/2;
		S = 4;

		hDefPen = SelectObject(pDraw->hDC,hPen);
		hDefBrush = SelectObject(pDraw->hDC,GetSysColorBrush(iColor));

		switch(uScroll)
			{
			case DFCS_SCROLLUP:
				ptPoly[0].x = X;
				ptPoly[0].y = Y-S;
				ptPoly[1].x = X+S;
				ptPoly[1].y = Y+S;
				ptPoly[2].x = X-S;
				ptPoly[2].y = Y+S;
				break;
			case DFCS_SCROLLDOWN:
				ptPoly[0].x = X;
				ptPoly[0].y = Y+S;
				ptPoly[1].x = X+S;
				ptPoly[1].y = Y-S;
				ptPoly[2].x = X-S;
				ptPoly[2].y = Y-S;
				break;
			case DFCS_SCROLLRIGHT:
				ptPoly[0].x = X+S;
				ptPoly[0].y = Y;
				ptPoly[1].x = X-S;
				ptPoly[1].y = Y-S;
				ptPoly[2].x = X-S;
				ptPoly[2].y = X+S;
				break;
			case DFCS_SCROLLLEFT:
				ptPoly[0].x = X-S;
				ptPoly[0].y = Y;
				ptPoly[1].x = X+S;
				ptPoly[1].y = Y-S;
				ptPoly[2].x = X+S;
				ptPoly[2].y = Y+S;
				break;
			}
		Polygon(pDraw->hDC,ptPoly,3);

		SelectObject(pDraw->hDC,hDefPen);
		SelectObject(pDraw->hDC,hDefBrush);
		DeleteObject(hPen);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&rcDraw);
	return;
}


// «»»» Affichage d'un bouton composé uniquement d'une icône ««««««««««««»

void Dialog_DrawIconButton(UINT uIconId, DRAWITEMSTRUCT *pDraw)
{
	UINT	uState;
	RECT	rcDrawArea;
	int	X,Y;

	CopyRect(&rcDrawArea,&pDraw->rcItem);

	if (App.hThemeButton)
		{
		if (pDraw->itemState&ODS_DISABLED) uState = PBS_DISABLED;
		else if (pDraw->itemState&ODS_SELECTED) uState = PBS_PRESSED;
		else if (pDraw->itemState&ODS_HOTLIGHT) uState = PBS_HOT;
		else if (pDraw->itemState&ODS_FOCUS) uState = PBS_DEFAULTED;
		else uState = PBS_NORMAL;
		DrawThemeBackground(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,NULL);
		GetThemeBackgroundContentRect(App.hThemeButton,pDraw->hDC,BP_PUSHBUTTON,uState,&pDraw->rcItem,&rcDrawArea);
		uState = uState == PBS_DISABLED?DSS_DISABLED:0;
		}
	else
		{
		uState = DFCS_ADJUSTRECT;
		if (pDraw->itemState&ODS_DISABLED) uState |= DFCS_INACTIVE;
		if (pDraw->itemState&ODS_SELECTED) uState |= DFCS_PUSHED;
		DrawFrameControl(pDraw->hDC,&rcDrawArea,DFC_BUTTON,DFCS_BUTTONPUSH|uState);
		uState = uState&DFCS_INACTIVE?DSS_DISABLED:0;
		}

	X = rcDrawArea.left+(rcDrawArea.right-rcDrawArea.left-16)/2;
	Y = rcDrawArea.top+(rcDrawArea.bottom-rcDrawArea.top-16)/2;
	DrawState(pDraw->hDC,NULL,NULL,(LPARAM)App.hIcons[uIconId],0,X,Y,16,16,DST_ICON|uState);

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&rcDrawArea);
	return;
}


// «»»» Affichage d'une étiquette «««««««««««««««««««««««««««««««««««««««»

void Dialog_DrawLabel(WCHAR *pszText, DRAWITEMSTRUCT *pDraw, HBRUSH hBckgnd, UINT uFormat)
{
	COLORREF	crText;
	int		iBkMode;

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	FillRect(pDraw->hDC,&pDraw->rcItem,hBckgnd?hBckgnd:GetSysColorBrush(COLOR_BTNFACE));
	DrawText(pDraw->hDC,pszText,-1,&pDraw->rcItem,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|uFormat);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);
	return;
}


// «»»» Affichage de l'icône "information" ««««««««««««««««««««««««««««««»

void Dialog_DrawInfo(WCHAR *pszInfoText, DRAWITEMSTRUCT *pDraw, UINT uFlags)
{
	RECT		rcText;
	COLORREF	crText;
	int		iBkMode;

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_WINDOW));
	DrawEdge(pDraw->hDC,&pDraw->rcItem,EDGE_SUNKEN,uFlags);
	if (App.hIconInfo) DrawIconEx(pDraw->hDC,pDraw->rcItem.left+12,pDraw->rcItem.top+12,App.hIconInfo,32,32,0,NULL,DI_NORMAL);
	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 32+24;
	rcText.right -= 12;
	rcText.top += 12;
	rcText.bottom -= 12;
	DrawText(pDraw->hDC,pszInfoText,-1,&rcText,DT_LEFT|DT_NOPREFIX|DT_WORDBREAK);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);
	return;
}


// «»»» Affichage d'un titre ««««««««««««««««««««««««««««««««««««««««««««»

void Dialog_DrawTitle(WCHAR *pszText, DRAWITEMSTRUCT *pDraw)
{
	RECT	rcDraw;
	SIZE	sizeText;

	CopyRect(&rcDraw,&pDraw->rcItem);
	rcDraw.left = 0;
	rcDraw.top = rcDraw.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-2)/2+1;
	rcDraw.bottom = rcDraw.top+2;
	if (pszText) rcDraw.right = 24;
	DrawEdge(pDraw->hDC,&rcDraw,BDR_SUNKENOUTER,BF_TOP|BF_BOTTOM);

	if (pszText)
		{
		GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&sizeText);
		DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pszText,0,30,pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-sizeText.cy)/2,0,0,DST_TEXT|DSS_DISABLED);
		rcDraw.left = 30+sizeText.cx+6;
		rcDraw.right = pDraw->rcItem.right;
		DrawEdge(pDraw->hDC,&rcDraw,BDR_SUNKENOUTER,BF_TOP|BF_BOTTOM);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Type d'affichage des listes					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialise les types d'affichage ««««««««««««««««««««««««««««««««»

int Dialog_ViewComboInit(HWND hDlg, UINT uCtlID, UINT uCurrent, UINT uNumViews, ...)
{
	LRESULT	lResult;
	UINT	uViewType;
	va_list	vl;

	va_start(vl,uNumViews);

	while(uNumViews)
		{
		uViewType = va_arg(vl,UINT);
		lResult = SendDlgItemMessage(hDlg,uCtlID,CB_ADDSTRING,0,(LPARAM)uViewType);
		if (lResult == CB_ERR || lResult == CB_ERRSPACE)
			{
			va_end(vl);
			return(0);
			}
		if (uCurrent == uViewType) SendDlgItemMessage(hDlg,uCtlID,CB_SETCURSEL,(WPARAM)lResult,0);
		uNumViews--;
		}

	va_end(vl);
	return(1);
}


// «»»» Change l'affichage de la liste ««««««««««««««««««««««««««««««««««»

int Dialog_ViewComboChanged(HWND hDlg, UINT uCtlID, UINT uListID, UINT *uValue)
{
	UINT	uViewType;
	LRESULT	lResult;

	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETCURSEL,0,0);
	if (lResult == CB_ERR) return(0);
	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETITEMDATA,(WPARAM)lResult,0);
	if (lResult == CB_ERR) return(0);
	uViewType = (UINT)lResult;
	if (uListID) SendDlgItemMessage(hDlg,uListID,LVM_SETVIEW,(WPARAM)uViewType,(LPARAM)0);
	*uValue = uViewType;
	return(1);
}


// «»»» Calcul la hauteur de la COMBOBOX ««««««««««««««««««««««««««««««««»

int Dialog_ViewComboMeasureItem(UINT uCtlID, MEASUREITEMSTRUCT *pMeasure)
{
	if (pMeasure->CtlType != ODT_COMBOBOX) return(0);
	if (pMeasure->CtlID != uCtlID) return(0);
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = App.Font.uFontHeight+4;
	return(1);
}


// «»»» Dessine un type d'affichage «««««««««««««««««««««««««««««««««««««»

void Dialog_ViewComboDrawItem(DRAWITEMSTRUCT *pDraw)
{
	UINT		uViewMode;
	UINT		uViewText;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	uViewText = 0;
	uViewMode = pDraw->itemData;
	switch(uViewMode)
		{
		case LV_VIEW_DETAILS:
			uViewText = TEXT_VIEW_DETAILS;
			break;
		case LV_VIEW_ICON:
			uViewText = TEXT_VIEW_ICONS;
			break;
		case LV_VIEW_TILE:
			uViewText = TEXT_VIEW_TILES;
			break;
		}

	if (uViewText)
		{
		RECT		rcText;
		HFONT		hFont;
		COLORREF	crText;
		int		iBkMode;

		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:(pDraw->itemState&ODS_DISABLED?COLOR_GRAYTEXT:COLOR_WINDOWTEXT)));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 2;
		rcText.right -= 2;
		DrawText(pDraw->hDC,Locale_GetText(uViewText),-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crText);
		SelectObject(pDraw->hDC,hFont);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions diverses						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Interception des messages «««««««««««««««««««««««««««««««««««««««»

LRESULT CALLBACK Dialog_SubDlgProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch(uMsgId)
		{
		case WM_KEYUP:
			switch(uIdSubclass)
				{
				case DIALOG_SUBCLASS_FILTER:
					if (wParam == VK_ESCAPE) DestroyWindow(GetParent(hDlg));
					break;
				}
			break;

		case WM_NCDESTROY:
			RemoveWindowSubclass(hDlg,Dialog_SubDlgProc,uIdSubclass);
			break;
		}

	return(DefSubclassProc(hDlg,uMsgId,wParam,lParam));
}


// «»»» Modifie la position Y d'un élément «««««««««««««««««««««««««««««««»

void Dialog_OffsetY(HWND hDlg, UINT uCtlId, int y)
{
	HWND		hwndCtrl;
	RECT		rcDialog;

	hwndCtrl = GetDlgItem(hDlg,uCtlId);
	GetWindowRect(hwndCtrl,&rcDialog);
	MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
	rcDialog.top += y;
	SetWindowPos(hwndCtrl,NULL,rcDialog.left,rcDialog.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
	return;
}


// «»»» Détermine la hauteur du texte d'information «««««««««««««««««««««»

int Dialog_GetInfoHeight(HWND hDlg, UINT uCtlId, WCHAR *InfoText)
{
	HDC		hDC;
	HFONT		hFont;
	HFONT		hDefFont;
	RECT		rcText;
	int		iHeight;

	iHeight = 0;
	hDC = GetDC(hDlg);
	if (hDC)
		{
		hFont = (HFONT)SendMessage(hDlg,WM_GETFONT,0,0);
		hDefFont = SelectObject(hDC,hFont);
		GetClientRect(GetDlgItem(hDlg,uCtlId),&rcText);
		rcText.left += 56;
		rcText.right -= 12;
		rcText.top += 12;
		iHeight = DrawText(hDC,InfoText,-1,&rcText,DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_WORDBREAK|DT_WORD_ELLIPSIS);
		iHeight += 28;
		if (iHeight < 32+28) iHeight = 32+28;
		SelectObject(hDC,hDefFont);
		ReleaseDC(hDlg,hDC);
		}

	return(iHeight);
}


// «»»» Centre une boîte de dialogue ««««««««««««««««««««««««««««««««««««»

void Dialog_CenterWindow(HWND hDlg, HWND hwndParent)
{
	RECT	rcWindow;
	RECT	rcDialog;
	int	X,Y;

	GetWindowRect(hwndParent,&rcWindow);
	GetWindowRect(hDlg,&rcDialog);
	rcDialog.bottom -= rcDialog.top;
	rcDialog.right -= rcDialog.left;
	X = rcWindow.left+(rcWindow.right-rcWindow.left-rcDialog.right)/2;
	Y = rcWindow.top+(rcWindow.bottom-rcWindow.top-rcDialog.bottom)/2;
	SetWindowPos(hDlg,NULL,X,Y,0,0,SWP_NOZORDER|SWP_NOSIZE);
	return;
}


// «»»» Vérifie et change une valeur ««««««««««««««««««««««««««««««««««««»

int Dialog_GetIntValue(HWND hDlg, UINT uCtlId, int iMin, int iMax, int *pVal)
{
	int	iVal;

	iVal = (int)GetDlgItemInt(hDlg,uCtlId,NULL,TRUE);
	if (iVal < iMin)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}
	if (iVal > iMax)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}

	*pVal = (int)iVal;
	return(1);
}

//--- Non signé ---

int Dialog_GetUIntValue(HWND hDlg, UINT uCtlId, UINT uMin, UINT uMax, UINT *pVal)
{
	UINT	uVal;

	uVal = GetDlgItemInt(hDlg,uCtlId,NULL,FALSE);
	if (uVal < uMin)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}
	if (uVal > uMax)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}

	*pVal = uVal;
	return(1);
}


// «»»» Processus pour les requêtes de TaskDialogIndirect «««««««««««««««»

HRESULT Dialog_TaskDialogProc(HWND hWnd, UINT uMsgID, WPARAM wParam, LPARAM lParam, LONG_PTR pRefData)
{
	switch(uMsgID)
		{
		case TDN_HYPERLINK_CLICKED: {
			TASKDIALOGCONFIG	dialog;
			TASKDIALOG_BUTTON	dialogButtons[2];
			int			iResult;

			ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
			dialog.cbSize = sizeof(TASKDIALOGCONFIG);
			dialog.hwndParent = hWnd;
			dialog.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION|TDF_EXPANDED_BY_DEFAULT|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
			dialog.dwCommonButtons = TDCBF_CLOSE_BUTTON;
			dialog.pszWindowTitle = Locale_GetText(TEXT_TITLE_LINK);
			dialog.pszMainIcon = NULL;
			dialog.pszContent = (WCHAR *)lParam;
			dialog.nDefaultButton = IDCLOSE;
			dialog.pszExpandedInformation = Locale_GetText(TEXT_LINK);
			dialog.cButtons = 2;
			dialog.pButtons = dialogButtons;
			dialogButtons[0].nButtonID = TEXT_COPY;
			dialogButtons[0].pszButtonText = Locale_GetText(TEXT_COPY);
			dialogButtons[1].nButtonID = TEXT_OPEN;
			dialogButtons[1].pszButtonText = Locale_GetText(TEXT_OPEN);;
			TaskDialogIndirect(&dialog,&iResult,NULL,NULL);

			//--- Copy to clipboard
			switch(iResult)
				{
				case TEXT_OPEN:
					ShellExecute(NULL,L"open",(WCHAR *)lParam,NULL,NULL,SW_SHOWNORMAL);
					break;

				case TEXT_COPY:
					if (OpenClipboard(hWnd))
						{
						HGLOBAL hHandle = GlobalAlloc(GMEM_MOVEABLE,wcslen((WCHAR *)lParam)*sizeof(WCHAR)+sizeof(WCHAR));
						if (hHandle)
							{
							WCHAR *pszLink = GlobalLock(hHandle);
							if (pszLink)
								{
								wcscpy(pszLink,(WCHAR *)lParam);
								GlobalUnlock(pszLink);
								EmptyClipboard();
								SetClipboardData(CF_UNICODETEXT,hHandle);
								}
							else GlobalFree(hHandle);
							}
						CloseClipboard();
						}
					break;
				}
			} break;
		}

	return(S_OK);
}


// «»»» Fonction non implantée ««««««««««««««««««««««««««««««««««««««««««»

void Dialog_NotImplemented(HWND hWnd)
{
	MessageBox(hWnd,Locale_GetText(TEXT_ERR_NOTIMPLEMENTED),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
	return;
}

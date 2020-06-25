
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
							if (pDialog->fnOk) { if (!pDialog->fnOk(hDlg,pDialog)) break; }
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
							if (pDialog->fnCancel) { if (!pDialog->fnCancel(hDlg,pDialog)) break; }
							EndDialog(hDlg,IDCANCEL);
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

	uState = DFCS_ADJUSTRECT;
	if (pDraw->itemState&ODS_DISABLED) uState = DFCS_INACTIVE;
	if (pDraw->itemState&ODS_GRAYED) uState = DFCS_INACTIVE;
	if (pDraw->itemState&ODS_SELECTED) uState = DFCS_PUSHED;
	DrawFrameControl(pDraw->hDC,&rcDrawArea,DFC_BUTTON,DFCS_BUTTONPUSH|uState);

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
	X = rcDrawArea.left+(rcDrawArea.right-rcDrawArea.left-Size.cx)/2;
	Y = rcDrawArea.top+(rcDrawArea.bottom-rcDrawArea.top-Size.cy)/2;
	DrawState(pDraw->hDC,NULL,NULL,(LPARAM)pszText,0,X,Y,Size.cx,Size.cy,DST_TEXT|((uState&DFCS_INACTIVE)?DSS_DISABLED:0));
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&rcDrawArea);
	return;
}



// «»»» Affichage d'un bouton composé d'une flèche ««««««««««««««««««««««»

void Dialog_DrawArrowButton(UINT uScroll, DRAWITEMSTRUCT *pDraw)
{
	HDC		hDC;
	UINT		uState;
	RECT		rcDraw;

	CopyRect(&rcDraw,&pDraw->rcItem);

	uState = DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;
	if (pDraw->itemState&ODS_DISABLED) uState |= DFCS_INACTIVE;
	if (pDraw->itemState&ODS_SELECTED) uState |= DFCS_PUSHED;
	DrawFrameControl(pDraw->hDC,&rcDraw,DFC_BUTTON,uState);

	hDC = CreateCompatibleDC(pDraw->hDC);
	if (hDC)
		{
		HBITMAP		hBmp,hDefBmp;
		HPEN		hPen,hDefPen;
		HBRUSH		hDefBrush;
		RECT		rcIcon;
		POINT		Points[4];
		int		iColor;

		iColor = (pDraw->itemState&ODS_DISABLED)?COLOR_GRAYTEXT:COLOR_BTNTEXT;
		hBmp = CreateCompatibleBitmap(pDraw->hDC,16,16);
		if (hBmp)
			{
			hDefBmp = SelectObject(hDC,hBmp);
			hPen = CreatePen(PS_SOLID,1,GetSysColor(iColor));
			if (hPen)
				{
				rcIcon.left = 0;
				rcIcon.top = 0;
				rcIcon.right = 16;
				rcIcon.bottom = 16;
				FillRect(hDC,&rcIcon,GetSysColorBrush(COLOR_BTNFACE));
				hDefPen = SelectObject(hDC,hPen);
				hDefBrush = SelectObject(hDC,GetSysColorBrush(iColor));
				switch(uScroll)
					{
					case DFCS_SCROLLRIGHT:
						Points[0].x = 10;
						Points[0].y = 7;
						Points[1].x = 6;
						Points[1].y = 11;
						Points[2].x = 6;
						Points[2].y = 3;
						break;
					case DFCS_SCROLLDOWN:
						Points[0].x = 11;
						Points[0].y = 5;
						Points[1].x = 7;
						Points[1].y = 9;
						Points[2].x = 3;
						Points[2].y = 5;
						break;
					case DFCS_SCROLLLEFT:
						Points[0].x = 5;
						Points[0].y = 7;
						Points[1].x = 9;
						Points[1].y = 3;
						Points[2].x = 9;
						Points[2].y = 11;
						break;
					case DFCS_SCROLLUP:
						Points[0].x = 7;
						Points[0].y = 5;
						Points[1].x = 11;
						Points[1].y = 9;
						Points[2].x = 3;
						Points[2].y = 9;
						break;
					}
				Points[3].x = Points[0].x;
				Points[3].y = Points[0].y;
				Polyline(hDC,Points,4);
				FloodFill(hDC,7,7,GetSysColor(iColor));
				SelectObject(hDC,hDefBrush);
				SelectObject(hDC,hDefPen);
				DeleteObject(hPen);
				}
			BitBlt(pDraw->hDC,rcDraw.left+(rcDraw.right-rcDraw.left-16)/2,rcDraw.top+(rcDraw.bottom-rcDraw.top-16)/2,16,16,hDC,0,0,SRCCOPY);
			SelectObject(hDC,hDefBmp);
			DeleteObject(hBmp);
			}
		DeleteDC(hDC);
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

	uState = DFCS_ADJUSTRECT;
	if (pDraw->itemState&ODS_DISABLED) uState = DFCS_INACTIVE;
	if (pDraw->itemState&ODS_GRAYED) uState = DFCS_INACTIVE;
	if (pDraw->itemState&ODS_SELECTED) uState = DFCS_PUSHED;
	DrawFrameControl(pDraw->hDC,&pDraw->rcItem,DFC_BUTTON,DFCS_BUTTONPUSH|uState);

	X = rcDrawArea.left+(pDraw->rcItem.right-pDraw->rcItem.left-16)/2;
	Y = rcDrawArea.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-16)/2;
	DrawState(pDraw->hDC,NULL,NULL,(LPARAM)App.hIcons[uIconId],0,X,Y,16,16,DST_ICON|((uState&DFCS_INACTIVE)?DSS_DISABLED:0));

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&rcDrawArea);
	return;
}


// «»»» Affichage d'une étiquette «««««««««««««««««««««««««««««««««««««««»

void Dialog_DrawLabel(WCHAR *pszText, DRAWITEMSTRUCT *pDraw, UINT uFormat)
{
	COLORREF	crText;
	int		iBkMode;

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	DrawText(pDraw->hDC,pszText,-1,&pDraw->rcItem,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|uFormat);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);
	return;
}


// «»»» Affichage de l'icône "information" ««««««««««««««««««««««««««««««»

void Dialog_DrawInfo(WCHAR *pszInfoText, DRAWITEMSTRUCT *pDraw, UINT grfFlags)
{
	RECT		rcText;
	COLORREF	crText;
	int		iBkMode;

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_WINDOW));
	DrawEdge(pDraw->hDC,&pDraw->rcItem,EDGE_SUNKEN,grfFlags);
	DrawIconEx(pDraw->hDC,pDraw->rcItem.left+12,pDraw->rcItem.top+12,LoadImage(NULL,MAKEINTRESOURCE(IDI_INFORMATION),IMAGE_ICON,32,32,LR_DEFAULTCOLOR|LR_SHARED),32,32,0,NULL,DI_NORMAL);
	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 56;
	rcText.right -= 12;
	rcText.top += 12;
	rcText.bottom -= 12;
	DrawText(pDraw->hDC,pszInfoText,-1,&rcText,DT_LEFT|DT_NOPREFIX|DT_WORDBREAK);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);
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

void Dialog_ViewComboChanged(HWND hDlg, UINT uCtlID, UINT uListID, UINT *uValue)
{
	UINT	uViewType;
	LRESULT	lResult;

	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETCURSEL,0,0);
	if (lResult == CB_ERR) return;
	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETITEMDATA,(WPARAM)lResult,0);
	if (lResult == CB_ERR) return;
	uViewType = (UINT)lResult;
	SendDlgItemMessage(hDlg,uListID,LVM_SETVIEW,(WPARAM)uViewType,(LPARAM)0);
	*uValue = uViewType;
	return;
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
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
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
		rcText.bottom -= 12;
		iHeight = DrawText(hDC,InfoText,-1,&rcText,DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_WORDBREAK);
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

	GetWindowRect(hwndParent,&rcWindow);
	GetClientRect(hDlg,&rcDialog);
	SetWindowPos(hDlg,NULL,rcWindow.left+(rcWindow.right-rcWindow.left-rcDialog.right)/2,rcWindow.top+(rcWindow.bottom-rcWindow.top-rcDialog.bottom)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);
	return;
}


// «»»» Vérifie et change une valeur ««««««««««««««««««««««««««««««««««««»

int Dialog_GetIntValue(HWND hDlg, UINT uCtlId, int Min, int Max, int *pVal)
{
	int	iVal;

	iVal = (int)GetDlgItemInt(hDlg,uCtlId,NULL,TRUE);
	if (iVal < Min)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}
	if (iVal > Max)
		{
		SetFocus(GetDlgItem(hDlg,uCtlId));
		return(0);
		}

	*pVal = (int)iVal;
	return(1);
}

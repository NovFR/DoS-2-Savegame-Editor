
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Recherche
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Dialogs.h"
#include "Requests.h"
#include "Texts.h"

extern APPLICATION		App;
static WCHAR*			lsfTypes[] = { L"DT_None", L"DT_Byte", L"DT_Short", L"DT_UShort", L"DT_Int", L"DT_UInt", L"DT_Float", L"DT_Double", L"DT_IVec2", L"DT_IVec3", L"DT_IVec4", L"DT_Vec2", L"DT_Vec3", L"DT_Vec4", L"DT_Mat2", L"DT_Mat3", L"DT_Mat3x4", L"DT_Mat4x3", L"DT_Mat4", L"DT_Bool", L"DT_String", L"DT_Path", L"DT_FixedString", L"DT_LSString", L"DT_ULongLong", L"DT_ScratchBuffer", L"DT_Long", L"DT_Int8", L"DT_TranslatedString", L"DT_WString", L"DT_LSWString", L"DT_UUID", L"DT_Unknown32", L"DT_TranslatedFSString", NULL };


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_SearchDialog(HWND hWnd)
{
	if (App.xmlTree.search.hDlg)
		{
		if (!Tree_SearchIsEnabled())
			{
			SetActiveWindow(App.xmlTree.search.hDlg);
			return;
			}
		Tree_Search(hWnd);
		return;
		}

	App.xmlTree.search.hDlg = CreateDialog(App.hInstance,MAKEINTRESOURCE(1015),hWnd,Tree_SearchDialogProc);
	if (!App.xmlTree.search.hDlg) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR|MB_OK);
	return;
}


// «»»» Messages de la boîte de dialogue ««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Tree_SearchDialogProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	if (uMsgId == WM_INITDIALOG)
		{
		TCITEM		tcItem;
		RECT		rcDialog;
		RECT		rcTab;
		int		i;
		int		iHeight;
		int		iTabHeight;

		Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_HIDE);

		//--- Tabs ---

		GetClientRect(hDlg,&rcDialog);
		rcDialog.top = iHeight = 0;
		GetWindowRect(GetDlgItem(hDlg,IDOK),&rcTab);
		MapWindowPoints(NULL,hDlg,(POINT *)&rcTab,2);
		rcDialog.bottom = rcTab.top-TREEVIEW_TAB_VMARGIN;
		App.xmlTree.search.hwndTab = CreateWindowEx(0,WC_TABCONTROL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|TCS_FOCUSONBUTTONDOWN,0,rcDialog.top,rcDialog.right,rcDialog.bottom-rcDialog.top,hDlg,(HMENU)999,NULL,NULL);
		if (!App.xmlTree.search.hwndTab)
			{
			Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR|MB_OK);
			DestroyWindow(hDlg);
			return(FALSE);
			}
		SendMessage(App.xmlTree.search.hwndTab,WM_SETFONT,(WPARAM)App.Font.hFont,0);
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = Locale_GetText(TEXT_DIALOG_TV_SEARCH_NODE);
		SendMessage(App.xmlTree.search.hwndTab,TCM_INSERTITEM,0,(LPARAM)&tcItem);
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = Locale_GetText(TEXT_DIALOG_TV_SEARCH_ATTR);
		SendMessage(App.xmlTree.search.hwndTab,TCM_INSERTITEM,1,(LPARAM)&tcItem);
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = Locale_GetText(TEXT_DIALOG_TV_SEARCH_OPTIONS);
		SendMessage(App.xmlTree.search.hwndTab,TCM_INSERTITEM,2,(LPARAM)&tcItem);
		#if _DEBUG
		if (TV_SEARCH_NODE != 0) Request_MessageBoxEx(hDlg,L"TV_SEARCH_NODE has bad value (should be 0 is %1!i!)",NULL,MB_OK,TV_SEARCH_NODE);
		if (TV_SEARCH_ATTR != 1) Request_MessageBoxEx(hDlg,L"TV_SEARCH_ATTR has bad value (should be 1 is %1!i!)",NULL,MB_OK,TV_SEARCH_ATTR);
		#endif

		SendMessage(App.xmlTree.search.hwndTab,TCM_GETITEMRECT,(WPARAM)0,(LPARAM)&rcTab);
		iTabHeight = rcTab.bottom-rcTab.top;

		//--- Controls positions ---

		GetWindowRect(hDlg,&rcDialog);
		rcDialog.bottom += iTabHeight+TREEVIEW_TAB_VPADDING*2+TREEVIEW_TAB_VMARGIN;
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(App.xmlTree.search.hwndTab,&rcDialog);
		rcDialog.bottom += iTabHeight+TREEVIEW_TAB_VPADDING*2+TREEVIEW_TAB_VMARGIN;
		SetWindowPos(App.xmlTree.search.hwndTab,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);

		Dialog_OffsetY(hDlg,778,iTabHeight+TREEVIEW_TAB_VPADDING*2+TREEVIEW_TAB_VMARGIN);
		Dialog_OffsetY(hDlg,IDOK,iTabHeight+TREEVIEW_TAB_VPADDING*2+TREEVIEW_TAB_VMARGIN);
		Dialog_OffsetY(hDlg,IDCANCEL,iTabHeight+TREEVIEW_TAB_VPADDING*2+TREEVIEW_TAB_VMARGIN);

		iHeight += iTabHeight+TREEVIEW_TAB_VPADDING;
		Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_SETPOS,iHeight);

		//--- Dialog position ---

		if (App.xmlTree.search.bMoved)
			{
			SetWindowPos(hDlg,NULL,App.xmlTree.search.lLeft,App.xmlTree.search.lTop,0,0,SWP_NOZORDER|SWP_NOSIZE);
			}
		else
			{
			HMONITOR	hMonitor;
			MONITORINFO	mi;
			RECT		rcParent;
			int		iDialogWidth;

			mi.cbSize = sizeof(MONITORINFO);
			hMonitor = MonitorFromWindow(App.xmlTree.hWnd,MONITOR_DEFAULTTOPRIMARY);
			GetWindowRect(App.xmlTree.hWnd,&rcParent);
			GetWindowRect(hDlg,&rcDialog);
			GetMonitorInfo(hMonitor,&mi);
			iDialogWidth = rcDialog.right-rcDialog.left;
			rcDialog.left = rcParent.right-iDialogWidth/2;
			rcDialog.right = rcDialog.left+iDialogWidth;
			rcDialog.top = rcParent.top+GetSystemMetrics(SM_CYCAPTION)+40;
			if (rcDialog.right > mi.rcMonitor.right) rcDialog.left = mi.rcMonitor.right-iDialogWidth;
			SetWindowPos(hDlg,NULL,rcDialog.left,rcDialog.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			}

		//--- Controls values ---

		SetDlgItemText(hDlg,200,App.xmlTree.search.pszId);
		SetDlgItemText(hDlg,202,App.xmlTree.search.pszType);
		SetDlgItemText(hDlg,204,App.xmlTree.search.pszValue);
		SetDlgItemText(hDlg,206,App.xmlTree.search.pszHandle);
		SetDlgItemText(hDlg,300,App.xmlTree.search.pszAll);
		CheckDlgButton(hDlg,210,App.xmlTree.search.bMatchAny?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,400,App.Config.bTVSearchOpacity?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,402,App.Config.bTVSearchHistory?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,778,App.Config.bTVSearchCaseSensitive?BST_CHECKED:BST_UNCHECKED);
		SendDlgItemMessage(hDlg,242,CB_ADDSTRING,0,(LPARAM)L"");
		for (i = 0; lsfTypes[i] != NULL; i++) SendDlgItemMessage(hDlg,242,CB_ADDSTRING,0,(LPARAM)lsfTypes[i]);
		SendDlgItemMessage(hDlg,401,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)50);
		SendDlgItemMessage(hDlg,401,TBM_SETRANGEMAX,(WPARAM)FALSE,(LPARAM)255);
		SendDlgItemMessage(hDlg,401,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)App.Config.uTVSearchAlpha);

		SetDlgItemText(hDlg,210,Locale_GetText(TEXT_DIALOG_TV_SEARCH_ANY));
		SetDlgItemText(hDlg,400,Locale_GetText(TEXT_DIALOG_TV_SEARCH_OPACITY));
		SetDlgItemText(hDlg,402,Locale_GetText(TEXT_DIALOG_TV_SEARCH_HISTORY));
		SetDlgItemText(hDlg,403,Locale_GetText(TEXT_DIALOG_TV_SEARCH_HISTORYCLEAR));
		SetDlgItemText(hDlg,778,Locale_GetText(TEXT_DIALOG_TV_SEARCH_CASE));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_DIALOG_TV_SEARCH_NEXT));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CLOSE));

		Tree_SearchHistoryLoad(hDlg);

		//--- Last step ---

		EnableWindow(GetDlgItem(hDlg,403),App.xmlTree.search.history.next?TRUE:FALSE);
		SendMessage(App.xmlTree.search.hwndTab,TCM_SETCURSEL,(WPARAM)App.xmlTree.search.uMode,0);
		Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_SHOW);
		Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_FOCUS);
		Tree_SearchGetType(hDlg,TRUE);
		Tree_SearchEnableOk(hDlg);
		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TV_SEARCH_TITLE));
		SetLayeredWindowAttributes(hDlg,0,255,LWA_ALPHA);
		return(FALSE);
		}

	switch(uMsgId)
		{
		case WM_CTLCOLORSTATIC:
			if ((HWND)lParam == GetDlgItem(hDlg,778)) return((INT_PTR)GetSysColorBrush(COLOR_BTNFACE));
			return((INT_PTR)GetSysColorBrush(COLOR_WINDOW));

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 201:
					Dialog_DrawLabel(szXMLid,(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				case 203:
					Dialog_DrawLabel(szXMLtype,(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				case 205:
					Dialog_DrawLabel(szXMLvalue,(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				case 207:
					Dialog_DrawLabel(szXMLhandle,(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				case 211:
					Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_TV_SEARCH_INFO),(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				case 301:
					Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_TV_SEARCH_STRING),(DRAWITEMSTRUCT *)lParam,GetSysColorBrush(COLOR_WINDOW),DT_RIGHT);
					return(TRUE);
				}

		case WM_MOVING:
			App.xmlTree.search.bMoved = TRUE;
			App.xmlTree.search.lLeft = ((RECT *)lParam)->left;
			App.xmlTree.search.lTop = ((RECT *)lParam)->top;
			break;

		case WM_ACTIVATE:
			switch(LOWORD(wParam))
				{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					SetLayeredWindowAttributes(hDlg,0,255,LWA_ALPHA);
					break;
				case WA_INACTIVE:
					if (!App.Config.bTVSearchOpacity) break;
					KillTimer(hDlg,1);
					Tree_SearchGetParams(hDlg);
					SetLayeredWindowAttributes(hDlg,0,App.Config.uTVSearchAlpha,LWA_ALPHA);
					break;
				}
			break;

		case WM_HSCROLL:
			if (!lParam) break;
			if ((HWND)lParam == GetDlgItem(hDlg,401))
				{
				App.Config.uTVSearchAlpha = SendDlgItemMessage(hDlg,401,TBM_GETPOS,0,0);
				SetLayeredWindowAttributes(hDlg,0,App.Config.uTVSearchAlpha,LWA_ALPHA);
				SetTimer(hDlg,1,1000,NULL);
				}
			break;

		case WM_TIMER:
			if (wParam == 1)
				{
				KillTimer(hDlg,1);
				SetLayeredWindowAttributes(hDlg,0,255,LWA_ALPHA);
				}
			break;

		case WM_NOTIFY:
			if (App.xmlTree.search.hwndTab)
				{
				if (((NMHDR *)lParam)->hwndFrom != App.xmlTree.search.hwndTab) break;
				switch(((NMHDR *)lParam)->code)
					{
					case TCN_SELCHANGING:
						Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_HIDE);
						break;
					case TCN_SELCHANGE:
						Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_SHOW);
						Tree_SearchEnableOk(hDlg);
						break;
					}
				break;
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_EDITCHANGE:
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
						case 202:
						case 204:
						case 206:
						case 300:
							Tree_SearchGetType(hDlg,TRUE);
							Tree_SearchEnableOk(hDlg);
							break;
						}
					break;
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
						case 202:
						case 204:
						case 206:
						case 300:
							EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
							break;
						case 242:
							Tree_SearchGetType(hDlg,FALSE);
							break;
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 403:
							Tree_SearchHistoryClear(hDlg);
							break;

						case 400:
							App.Config.bTVSearchOpacity = IsDlgButtonChecked(hDlg,400) == BST_CHECKED?TRUE:FALSE;
							break;
						case 402:
							App.Config.bTVSearchHistory = IsDlgButtonChecked(hDlg,402) == BST_CHECKED?TRUE:FALSE;
							break;

						case IDOK:
							Tree_SearchGetParams(hDlg);
							Tree_Search(hDlg);
							return(TRUE);

						case IDCANCEL:
							DestroyWindow(hDlg);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_CLOSE:
			DestroyWindow(hDlg);
			return(TRUE);

		case WM_DESTROY:
			KillTimer(hDlg,1);
			Tree_SearchGetParams(hDlg);
			Tree_SearchHistoryRelease();
			App.xmlTree.search.hDlg = NULL;
			App.xmlTree.search.hwndTab = NULL;
			SetActiveWindow(App.xmlTree.hWnd);
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Type ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Tree_SearchGetType(HWND hDlg, BOOL bSet)
{
	UINT	uType = 0;

	if (bSet)
		{
		if (SendDlgItemMessage(hDlg,202,WM_GETTEXTLENGTH,0,0))
			{
			uType = GetDlgItemInt(hDlg,202,NULL,FALSE);
			if (uType+1 > sizeof(lsfTypes)/sizeof(WCHAR *)-1) uType = 0;
			else uType++;
			}
		SendDlgItemMessage(hDlg,242,CB_SETCURSEL,(WPARAM)uType,0);
		}
	else
		{
		uType = SendDlgItemMessage(hDlg,242,CB_GETCURSEL,0,0);
		if (uType == CB_ERR) return;
		if (!uType) return;
		uType--;
		SetDlgItemInt(hDlg,202,uType,FALSE);
		}

	return;
}


// «»»» Stockage des paramètres «««««««««««««««««««««««««««««««««««««««««»

void Tree_SearchGetParams(HWND hDlg)
{
	Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_GETSEL,&App.xmlTree.search.uMode);
	App.xmlTree.search.bMatchAny = IsDlgButtonChecked(hDlg,210) == BST_CHECKED?TRUE:FALSE;
	App.Config.bTVSearchOpacity = IsDlgButtonChecked(hDlg,400) == BST_CHECKED?TRUE:FALSE;
	App.Config.bTVSearchHistory = IsDlgButtonChecked(hDlg,402) == BST_CHECKED?TRUE:FALSE;
	App.Config.bTVSearchCaseSensitive = IsDlgButtonChecked(hDlg,778) == BST_CHECKED?TRUE:FALSE;
	Tree_SearchGetString(hDlg,200);
	Tree_SearchGetString(hDlg,202);
	Tree_SearchGetString(hDlg,204);
	Tree_SearchGetString(hDlg,206);
	Tree_SearchGetString(hDlg,300);
	return;
}


// «»»» Stockage des chaînes ««««««««««««««««««««««««««««««««««««««««««««»

void Tree_SearchGetString(HWND hDlg, UINT uCtlId)
{
	WCHAR**	pszBuffer;
	UINT	uLen;
	UINT	uHistory;

	switch(uCtlId)
		{
		case 200:
			pszBuffer = &App.xmlTree.search.pszId;
			uHistory = TV_SEARCH_HISTORY_ID;
			break;
		case 202:
			pszBuffer = &App.xmlTree.search.pszType;
			uHistory = TV_SEARCH_HISTORY_NONE;
			break;
		case 204:
			pszBuffer = &App.xmlTree.search.pszValue;
			uHistory = TV_SEARCH_HISTORY_VALUE;
			break;
		case 206:
			pszBuffer = &App.xmlTree.search.pszHandle;
			uHistory = TV_SEARCH_HISTORY_HANDLE;
			break;
		case 300:
			pszBuffer = &App.xmlTree.search.pszAll;
			uHistory = TV_SEARCH_HISTORY_ALL;
			break;
		default:return;
		}

	if (*pszBuffer)
		{
		HeapFree(App.hHeap,0,*pszBuffer);
		*pszBuffer = NULL;
		}
	uLen = SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXTLENGTH,0,0);
	if (uLen)
		{
		*pszBuffer = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
		if (*pszBuffer)
			{
			SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXT,(WPARAM)uLen,(LPARAM)*pszBuffer);
			Tree_SearchHistoryAdd(hDlg,uCtlId,*pszBuffer,uHistory);
			}
		}

	return;
}


// «»»» Vérifie qu'il y a quelque chose à rechercher ««««««««««««««««««««»

//--- Active le bouton de recherche ---

void Tree_SearchEnableOk(HWND hDlg)
{
	UINT	uMode;
	UINT	uLen;
	BOOL	bActivated = TRUE;

	Tree_SearchTabControl(hDlg,TV_SEARCH_TAB_GETSEL,&uMode);

	switch(uMode)
		{
		case TV_SEARCH_NODE:
			uLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
			if (uLen) break;
			uLen = SendDlgItemMessage(hDlg,202,WM_GETTEXTLENGTH,0,0);
			if (uLen) break;
			uLen = SendDlgItemMessage(hDlg,204,WM_GETTEXTLENGTH,0,0);
			if (uLen) break;
			uLen = SendDlgItemMessage(hDlg,206,WM_GETTEXTLENGTH,0,0);
			if (uLen) break;
			bActivated = FALSE;
			break;
		case TV_SEARCH_ATTR:
			uLen = SendDlgItemMessage(hDlg,300,WM_GETTEXTLENGTH,0,0);
			if (uLen) break;
			bActivated = FALSE;
			break;
		case TV_SEARCH_OPTIONS:
		default:bActivated = FALSE;
			break;
		}

	EnableWindow(GetDlgItem(hDlg,IDOK),bActivated);
	return;
}

//--- Retourne les options de recherche ---

DWORD Tree_SearchIsEnabled()
{
	DWORD	dwWatch = 0;

	switch (App.xmlTree.search.uMode)
		{
		case TV_SEARCH_NODE:
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszId,TREEVIEW_WATCH_ID);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszType,TREEVIEW_WATCH_TYPE);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszValue,TREEVIEW_WATCH_VALUE);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszHandle,TREEVIEW_WATCH_HANDLE);
			break;
		case TV_SEARCH_ATTR:
			dwWatch = Tree_SearchStringIsOk(App.xmlTree.search.pszAll,TREEVIEW_WATCH_VALUE);
			break;
		}

	return(dwWatch);
}


// «»»» Onglets «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

// Extra parameters:
//
// 	TV_SEARCH_TAB_HIDE - If tab control not set hides all children, else hides children of selected tab
//		None
//
//	TV_SEARCH_TAB_SHOW - Shows children of selected tab
//		None
//
//	TV_SEARCH_TAB_FOCUS - Set focus to the first control of the selected tab
//		None
//
//	TV_SEARCH_TAB_SETPOS - Adjust position of all controls
//		int > y offset
//
//	TV_SEARCH_TAB_GETSEL - Get the current selected tab
//		UINT* > selected tab index
//

void Tree_SearchTabControl(HWND hDlg, UINT uCmd, ...)
{
	va_list		vl;
	int		i,j;

	static UINT	uTabsPage1[] = { 200, 201, 202, 242, 203, 204, 205, 206, 207, 210, 211, 0 };
	static UINT	uTabsPage2[] = { 300, 301, 0 };
	static UINT	uTabsPage3[] = { 400, 401, 402, 403, 0 };
	static UINT*	uTabsPages[] = { uTabsPage1, uTabsPage2, uTabsPage3, NULL };

	switch (uCmd)
		{
		case TV_SEARCH_TAB_HIDE:
			if (!App.xmlTree.search.hwndTab)
				{
				for (i = 0; uTabsPages[i] != NULL; i++)
					for (j = 0; uTabsPages[i][j] != 0; j++)
						ShowWindow(GetDlgItem(hDlg,uTabsPages[i][j]),FALSE);
				}
			else
				{
				i = SendMessage(App.xmlTree.search.hwndTab,TCM_GETCURSEL,0,0);
				if (i >= sizeof(uTabsPages)/sizeof(UINT *)-1) break;
				for (j = 0; uTabsPages[i][j] != 0; j++)
					ShowWindow(GetDlgItem(hDlg,uTabsPages[i][j]),FALSE);
				}
			break;

		case TV_SEARCH_TAB_SHOW:
			if (App.xmlTree.search.hwndTab)
				{
				i = SendMessage(App.xmlTree.search.hwndTab,TCM_GETCURSEL,0,0);
				if (i >= sizeof(uTabsPages)/sizeof(UINT *)-1) break;
				for (j = 0; uTabsPages[i][j] != 0; j++)
					ShowWindow(GetDlgItem(hDlg,uTabsPages[i][j]),TRUE);
				}
			break;

		case TV_SEARCH_TAB_FOCUS:
			if (App.xmlTree.search.hwndTab)
				{
				i = SendMessage(App.xmlTree.search.hwndTab,TCM_GETCURSEL,0,0);
				if (i >= sizeof(uTabsPages)/sizeof(UINT *)-1) break;
				SetFocus(GetDlgItem(hDlg,uTabsPages[i][0]));
				}
			break;

		case TV_SEARCH_TAB_SETPOS: {
			int		iHeight;

			va_start(vl,uCmd);
			iHeight = va_arg(vl,int);
			va_end(vl);

			for (i = 0; uTabsPages[i] != NULL; i++)
				for (j = 0; uTabsPages[i][j] != 0; j++)
					Dialog_OffsetY(hDlg,uTabsPages[i][j],iHeight);

			} break;

		case TV_SEARCH_TAB_GETSEL: {
			UINT*		uMode;

			va_start(vl,uCmd);
			uMode = va_arg(vl,UINT *);
			va_end(vl);

			if (App.xmlTree.search.hwndTab) *uMode = SendMessage(App.xmlTree.search.hwndTab,TCM_GETCURSEL,0,0);
			else *uMode = TV_SEARCH_NONE;
			} break;
		}

	return;
}


// «»»» Historique ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

//--- Chargement de l'historique ---

void Tree_SearchHistoryLoad(HWND hDlg)
{
	TVSEARCHSTRING*	pString;
	HANDLE		hFile = INVALID_HANDLE_VALUE;
	BYTE*		pBuffer = NULL;
	DWORD		dwFileSize;
	DWORD		dwBytes;
	DWORD		dwCursor;
	UINT		uCtlId;
	UINT		uHistory;
	UINT		i;

	hFile = CreateFile(szTVSearchHistoryPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Error;

	dwFileSize = GetFileSize(hFile,NULL);
	if (dwFileSize == INVALID_FILE_SIZE) goto Error;

	pBuffer = HeapAlloc(App.hHeap,0,dwFileSize);
	if (!pBuffer) goto Error;

	if (!ReadFile(hFile,pBuffer,dwFileSize,&dwBytes,NULL)) goto Error;
	if (dwBytes != dwFileSize) goto Error;
	if (dwBytes < sizeof(UINT)) goto Error;

	for (dwCursor = 0; dwCursor < dwBytes;)
		{
		uHistory = *((UINT *)(pBuffer+dwCursor));
		dwCursor += sizeof(UINT);
		if (dwCursor >= dwBytes) break; // Unexpected EOF
		for (i = 0; dwCursor+i < dwBytes; i += sizeof(WCHAR)) if (*(WCHAR *)(pBuffer+dwCursor+i) == L'\0') break;
		if (dwCursor+i >= dwBytes) break; // Last string is not null terminated, ignored
		pString = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(TVSEARCHSTRING)+i+sizeof(WCHAR));
		if (!pString) break; // Memory error
		pString->node.type = uHistory;
		wcscpy(pString->szText,(WCHAR *)&pBuffer[dwCursor]);
		List_AddEntry((NODE *)pString,&App.xmlTree.search.history);
		switch (uHistory)
			{
			case TV_SEARCH_HISTORY_ID:
				uCtlId = 200;
				break;
			case TV_SEARCH_HISTORY_VALUE:
				uCtlId = 204;
				break;
			case TV_SEARCH_HISTORY_HANDLE:
				uCtlId = 206;
				break;
			case TV_SEARCH_HISTORY_ALL:
				uCtlId = 300;
				break;
			default:uCtlId = 0;
				break;
			}
		if (uCtlId) SendDlgItemMessage(hDlg,uCtlId,CB_INSERTSTRING,0,(LPARAM)pString->szText);
		dwCursor += i+sizeof(WCHAR);
		}

Error:	if (pBuffer) HeapFree(App.hHeap,0,pBuffer);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	return;
}

//--- Libération de l'historique ---

void Tree_SearchHistoryClear(HWND hDlg)
{
	DeleteFile(szTVSearchHistoryPath);
	Tree_SearchHistoryRelease();
	if (hDlg)
		{
		SendDlgItemMessage(hDlg,200,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(hDlg,204,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(hDlg,206,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(hDlg,300,CB_RESETCONTENT,0,0);
		EnableWindow(GetDlgItem(hDlg,403),FALSE);
		}
	return;
}

void Tree_SearchHistoryRelease()
{
	List_ReleaseMemory(&App.xmlTree.search.history);
	return;
}

//--- Ajoute un texte à l'historique ---

void Tree_SearchHistoryAdd(HWND hDlg, UINT uCtlId, WCHAR *pszText, UINT uHistory)
{
	TVSEARCHSTRING*	pString;
	HANDLE		hFile;
	UINT		uLen;
	DWORD		dwBytes;

	if (!App.Config.bTVSearchHistory) return;
	if (uHistory == TV_SEARCH_HISTORY_NONE) return;
	if (!pszText) return;
	if (!(uLen = wcslen(pszText))) return;

	//--- N'ajoute pas un texte déjà existant
	for (pString = (TVSEARCHSTRING *)App.xmlTree.search.history.next; pString != NULL; pString = (TVSEARCHSTRING *)pString->node.next)
		{
		if (pString->node.type != uHistory) continue;
		if (!wcscmp(pString->szText,pszText)) return;
		}

	//--- Ajoute le nouveau texte dans la liste
	pString = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(TVSEARCHSTRING)+uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pString) return;
	pString->node.type = uHistory;
	wcscpy(pString->szText,pszText);
	List_AddEntry((NODE *)pString,&App.xmlTree.search.history);
	if (uCtlId)
		{
		SendDlgItemMessage(hDlg,uCtlId,CB_INSERTSTRING,0,(LPARAM)pString->szText);
		EnableWindow(GetDlgItem(hDlg,403),TRUE);
		}

	//--- Ajoute le nouveau texte dans le fichier
	if (!PathFileExists(szConfigFolderPath)) CreateDirectory(szConfigFolderPath,NULL);
	hFile = CreateFile(szTVSearchHistoryPath,FILE_APPEND_DATA,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) return;
	SetEndOfFile(hFile);
	WriteFile(hFile,&uHistory,sizeof(UINT),&dwBytes,NULL);
	WriteFile(hFile,pszText,uLen*sizeof(WCHAR)+sizeof(WCHAR),&dwBytes,NULL);
	CloseHandle(hFile);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Recherche							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_Search(HWND hWnd)
{
	TVITEMEX	tvItem;
	HTREEITEM	hItem;
	XML_NODE*	pxnRoot;
	NODE*		pCursor;
	NODE*		pNode;
	BOOL		bMatch;
	DWORD		dwMatch;
	DWORD		dwWatch;
	BOOL		bCaseSensitive;

	hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
	if (!hItem) return;

	tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
	tvItem.hItem = hItem;
	tvItem.lParam = 0;
	SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
	if (!(pNode = pCursor = (NODE *)tvItem.lParam)) return;

	pxnRoot = Tree_GetRootNode(NULL);
	dwWatch = Tree_SearchIsEnabled();
	if (!dwWatch) return;

	for (bMatch = FALSE, bCaseSensitive = App.Config.bTVSearchCaseSensitive;;)
		{
		pNode = Tree_SearchNextItem(pNode);
		if (pNode == pCursor) break;

		switch(pNode->type)
			{
			case XML_TYPE_NODE:
				if (App.xmlTree.search.uMode != TV_SEARCH_NODE) break;
				if (!((XML_NODE *)pNode)->attributes.next) break;
				dwMatch  = 0;
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLid),App.xmlTree.search.pszId,TREEVIEW_WATCH_ID,FALSE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLtype),App.xmlTree.search.pszType,TREEVIEW_WATCH_TYPE,TRUE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLvalue),App.xmlTree.search.pszValue,TREEVIEW_WATCH_VALUE,FALSE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLhandle),App.xmlTree.search.pszHandle,TREEVIEW_WATCH_HANDLE,FALSE,bCaseSensitive);
				if (App.xmlTree.search.bMatchAny && dwMatch) bMatch = TRUE;
				else if (dwMatch == dwWatch) bMatch = TRUE;
				break;
			case XML_TYPE_ATTR:
				if (App.xmlTree.search.uMode != TV_SEARCH_ATTR) break;
				bMatch = Tree_SearchMatch(xml_GetThisAttrValue((XML_ATTR *)pNode),App.xmlTree.search.pszAll,TREEVIEW_WATCH_VALUE,FALSE,bCaseSensitive)?TRUE:FALSE;
				break;
			}

		if (bMatch)
			{
			Tree_SearchSelectItem(pNode);
			SetActiveWindow(App.xmlTree.hWnd);
			break;
			}

		if (pNode == (NODE *)pxnRoot)
			{
			if (MessageBox(hWnd,Locale_GetText(TEXT_DIALOG_TV_SEARCH_EOF),Locale_GetText(TEXT_TITLE_INFO),MB_ICONQUESTION|MB_YESNO) != IDYES)
				{
				bMatch = TRUE; // No need to display another msg
				break;
				}
			}
		}

	if (!bMatch) MessageBox(hWnd,Locale_GetText(TEXT_DIALOG_TV_SEARCH_NOTFOUND),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
	return;
}

//--- Vérifie si une chaîne de recherche est valide ---

DWORD Tree_SearchStringIsOk(WCHAR *pszSearch, DWORD dwFlag)
{
	if (!pszSearch) return(0);
	if (!*pszSearch) return(0);
	return(dwFlag);
}

//--- Vérifie si une chaîne correspond ---

DWORD Tree_SearchMatch(WCHAR *pszValue, WCHAR *pszSearch, DWORD dwFlag, BOOL bStrict, BOOL bCaseSensitive)
{
	if (!pszValue) return(0);
	if (!pszSearch) return(0);
	if (!*pszSearch) return(0);

	if (bStrict) return(wcscmp(pszValue,pszSearch) != 0?0:dwFlag);
	else if (!bCaseSensitive) return(StrStrI(pszValue,pszSearch) == NULL?0:dwFlag);
	else return(wcsstr(pszValue,pszSearch) == NULL?0:dwFlag);
}


// «»»» Recherche la prochaine entrée «««««««««««««««««««««««««««««««««««»

NODE* Tree_SearchNextItem(NODE *pEntry)
{
	XML_NODE*	pxnRoot;
	NODE*		pParent;

	if (!pEntry) return(NULL);

	switch(pEntry->type)
		{
		case XML_TYPE_NODE:
			if (((XML_NODE *)pEntry)->attributes.next) return(((XML_NODE *)pEntry)->attributes.next); // first attribute
			if (((XML_NODE *)pEntry)->children.next) return(((XML_NODE *)pEntry)->children.next); // first child
			if (pEntry->next) return(pEntry->next); // next entry
			pParent = ((XML_NODE *)pEntry)->parent; // parent node
			pxnRoot = Tree_GetRootNode(NULL); // root node
			if (!pxnRoot) return(NULL); // Should not happen !
			while (1) // Search for next parent
				{
				if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
				if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
				if (pParent->next) return(pParent->next); // Returns next parent
				pParent = ((XML_NODE *)pParent)->parent;
				}
			break;
		case XML_TYPE_ATTR:
			if (pEntry->next) return(pEntry->next); // next entry
			pParent = ((XML_ATTR *)pEntry)->parent; // parent node
			pxnRoot = Tree_GetRootNode(NULL); // root node
			if (!pxnRoot) return(NULL); // Should not happen !
			if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
			if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
			if (((XML_NODE *)pParent)->children.next) return(((XML_NODE *)pParent)->children.next); // Returns parent child
			while (1) // Search for next parent
				{
				if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
				if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
				if (pParent->next) return(pParent->next); // Returns next parent
				pParent = ((XML_NODE *)pParent)->parent;
				}
			break;
		}

	return(NULL);
}


// «»»» Sélectionne une entrée ««««««««««««««««««««««««««««««««««««««««««»

void Tree_SearchSelectItem(NODE *pEntry)
{
	TVITEMEX	tvItem;
	XML_NODE*	pxnRoot;
	NODE*		pParent;
	NODE**		pParents;
	UINT		uCount;
	int		i;

	if (!pEntry) return;

	uCount = 0;
	pxnRoot = Tree_GetRootNode(&tvItem);
	if (!pxnRoot) return;

	// --- Builds the parents list ---
	//
	// pParents[0] = entry parent
	// pParents[x] = root

	for (pParent = pEntry;;)
		{
		uCount++;
		if (pParent->type == XML_TYPE_NODE) pParent = ((XML_NODE *)pParent)->parent;
		else if (pParent->type == XML_TYPE_ATTR) pParent = ((XML_ATTR *)pParent)->parent;
		if (pParent == pxnRoot->parent) break;
		if (!pParent) break;
		}

	pParents = HeapAlloc(App.hHeap,0,sizeof(NODE *)*uCount);
	if (!pParents) return;

	for (pParent = pEntry, i = 0; i < uCount; i++)
		{
		if (pParent->type == XML_TYPE_NODE) pParents[i] = pParent = ((XML_NODE *)pParent)->parent;
		else if (pParent->type == XML_TYPE_ATTR) pParents[i] = pParent = ((XML_ATTR *)pParent)->parent;
		if (pParent == pxnRoot->parent) break;
		if (!pParent) break;
		}

	// --- Selects the entry ---
	//
	// uCount = 1, entry is in root list
	// uCount > 1, entry is somewhere else. We need to expand parents to get the TreeView informations.

	if (uCount > 1) tvItem.hItem = Tree_SearchSelectedItemParent(tvItem.hItem,pParents,uCount-1);
	while (tvItem.hItem)
		{
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (!tvItem.lParam) break;
		if ((NODE *)tvItem.lParam != pEntry)
			{
			tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)tvItem.hItem);
			continue;
			}
		//--- Sélectionne l'entrée
		SendMessage(App.xmlTree.hwndTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)tvItem.hItem);
		//--- Ajustements supplémentaires
		if (pEntry->type == XML_TYPE_NODE)
			{
			HTREEITEM	hItem;
			HTREEITEM	hPrevItem;

			//--- Déplie la liste
			SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)tvItem.hItem);
			//--- Fait en sorte que le dernier attribut soit visible
			hPrevItem = tvItem.hItem;
			hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)tvItem.hItem);
			while (hItem)
				{
				tvItem.hItem = hItem;
				tvItem.lParam = 0;
				SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
				if (tvItem.lParam)
					{
					if (((NODE *)tvItem.lParam)->type == XML_TYPE_NODE) break;
					hPrevItem = hItem;
					}
				hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)hItem);
				}
			SendMessage(App.xmlTree.hwndTree,TVM_ENSUREVISIBLE,0,(LPARAM)hPrevItem);
			}
		break;
		}

	HeapFree(App.hHeap,0,pParents);
	return;
}

//--- Retrouve la liste de l'entrée et déplie les parents ---

HTREEITEM Tree_SearchSelectedItemParent(HTREEITEM hItem, NODE **pParents, UINT uCursor)
{
	TVITEMEX	tvItem;

	tvItem.hItem = hItem;

	while (tvItem.hItem)
		{
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (!tvItem.lParam) return(NULL);
		if (
			(((NODE *)tvItem.lParam)->type == XML_TYPE_NODE)
			&& (
				(((XML_NODE *)tvItem.lParam)->attributes.next && ((XML_ATTR *)((XML_NODE *)tvItem.lParam)->attributes.next)->parent == pParents[uCursor-1])
				||
				(((XML_NODE *)tvItem.lParam)->children.next && ((XML_NODE *)((XML_NODE *)tvItem.lParam)->children.next)->parent == pParents[uCursor-1])
			)
		)
			{
			SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)tvItem.hItem);
			tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)tvItem.hItem);
			if (--uCursor < 1) return(tvItem.hItem);
			}

		else tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)tvItem.hItem);
		}

	return(NULL);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Libère la mémoire utilisée ««««««««««««««««««««««««««««««««««««««»

void Tree_SearchRelease()
{
	if (App.xmlTree.search.pszId) HeapFree(App.hHeap,0,App.xmlTree.search.pszId);
	if (App.xmlTree.search.pszType) HeapFree(App.hHeap,0,App.xmlTree.search.pszType);
	if (App.xmlTree.search.pszValue) HeapFree(App.hHeap,0,App.xmlTree.search.pszValue);
	if (App.xmlTree.search.pszHandle) HeapFree(App.hHeap,0,App.xmlTree.search.pszHandle);
	if (App.xmlTree.search.pszAll) HeapFree(App.hHeap,0,App.xmlTree.search.pszAll);

	App.xmlTree.search.pszId = NULL;
	App.xmlTree.search.pszType = NULL;
	App.xmlTree.search.pszValue = NULL;
	App.xmlTree.search.pszHandle = NULL;
	App.xmlTree.search.pszAll = NULL;
	return;
}

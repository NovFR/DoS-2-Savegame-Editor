
//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration - Langage
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Startup.h"
#include "Locale.h"
#include "Texts.h"
#include "Menus.h"
#include "Game.h"
#include "LastFiles.h"
#include "Requests.h"
#include "Dialogs.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	MainMenu;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection de la langue					  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélection de la langue «««««««««««««««««««««««««««««««««««««««»

void Config_SelectLanguage()
{
	NODE	Root;
	INT_PTR	iResult;

	if (!Locale_Enum(App.hWnd,szLangPath,&Root)) return;

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1100),App.hWnd,Config_SelectLanguageProc,(LPARAM)&Root);
	if (!iResult || iResult == -1)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOCALE_SELECT),NULL,MB_ICONERROR);
		Locale_EnumRelease(&Root);
		return;
		}

	Locale_EnumRelease(&Root);
	return;
}

//--- Boîte de dialogue ---

INT_PTR CALLBACK Config_SelectLanguageProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	NODE*		pRoot;

	if (uMsgId == WM_INITDIALOG)
		{
		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pRoot = (NODE *)lParam;

		if (!Config_SelectLanguageInit(hDlg,200,pRoot,TRUE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
		SetWindowText(hDlg,Locale_GetText(IDM_CONFIGLANGUAGE));
		Dialog_CenterWindow(hDlg,App.hWnd);
		return(FALSE);
		}

	pRoot = (NODE *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			switch(wParam)
				{
				case 200:
					((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
					((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
					if (((MEASUREITEMSTRUCT *)lParam)->itemHeight < 24) ((MEASUREITEMSTRUCT *)lParam)->itemHeight = 24;
					return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 200:
					Config_SelectLanguageDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 200:
							if (!Config_SelectLanguageSet(hDlg)) return(TRUE);
							EndDialog(hDlg,IDOK);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							if (!Config_SelectLanguageSet(hDlg)) return(TRUE);
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
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

//--- Initialisation ---

BOOL Config_SelectLanguageInit(HWND hDlg, UINT uCtlId, NODE *pRoot, BOOL bSetOK)
{
	LOCALE_ENUM*	pEnum;
	WCHAR*		pszText;
	LRESULT		lResult;
	DWORD_PTR	vl[1];

	for (pEnum = (LOCALE_ENUM *)pRoot->next; pEnum != NULL; pEnum = (LOCALE_ENUM *)pEnum->node.next)
		{
		//--- Add string
		lResult = SendDlgItemMessage(hDlg,uCtlId,LB_ADDSTRING,0,(LPARAM)pEnum);
		if (lResult == LB_ERR || lResult == LB_ERRSPACE) return(FALSE);
		//--- Load icon
		vl[0] = (DWORD_PTR)pEnum->szLang;
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szLangIcoPath,0,0,(WCHAR *)&pszText,1,(va_list *)&vl))
			{
			pEnum->hIcon = LoadImage(NULL,pszText,IMAGE_ICON,16,16,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
			LocalFree(pszText);
			}
		//--- Select current language
		if (!wcscmp(pEnum->szLang,App.Config.pszLocaleName))
			{
			SendDlgItemMessage(hDlg,uCtlId,LB_SETCURSEL,(WPARAM)lResult,0);
			if (bSetOK) EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
			}
		}

	return(TRUE);
}

//--- Affichage ---

BOOL Config_SelectLanguageDrawItem(DRAWITEMSTRUCT *pDraw)
{
	LOCALE_ENUM*	pEnum;

	pEnum = (LOCALE_ENUM *)pDraw->itemData;
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	if (pEnum)
		{
		RECT		rcClient;
		HFONT		hDefFont;
		COLORREF	crText;
		int		iBkMode;

		CopyRect(&rcClient,&pDraw->rcItem);
		rcClient.left += 2;
		rcClient.top += 2;
		rcClient.right -= 2;
		rcClient.bottom -= 2;
		hDefFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		if (pEnum->hIcon) DrawIconEx(pDraw->hDC,rcClient.left,rcClient.top+(rcClient.bottom-rcClient.top-16)/2,pEnum->hIcon,16,16,0,NULL,DI_NORMAL);
		rcClient.left += 20;
		DrawText(pDraw->hDC,pEnum->pszName,-1,&rcClient,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetTextColor(pDraw->hDC,crText);
		SetBkMode(pDraw->hDC,iBkMode);
		SelectObject(pDraw->hDC,hDefFont);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return(TRUE);
}

//--- Sélection du langage ---

int Config_SelectLanguageSet(HWND hDlg)
{
	LOCALE_ENUM*	pEnum;
	int		iSelected;

	iSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (iSelected == LB_ERR) return(0);

	pEnum = (LOCALE_ENUM *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)iSelected,0);
	if (pEnum == (LOCALE_ENUM *)LB_ERR) return(0);

	// Language not changed
	if (!wcscmp(pEnum->szLang,App.Config.pszLocaleName)) return(1);

	// Change language
	return(Config_SetLanguage(hDlg,pEnum->szLang));
}


// «»»» Modification de la langue «««««««««««««««««««««««««««««««««««««««»

int Config_SetLanguage(HWND hWnd, WCHAR *pszLang)
{
	LVGROUP		lvGroup;
	LOCALE_TEXT*	pLocale;
	WCHAR*		pszLocaleName;
	WCHAR*		pszLangCpy;

	pszLangCpy = HeapAlloc(App.hHeap,0,wcslen(pszLang)*sizeof(WCHAR)+sizeof(WCHAR));
	if (pszLangCpy)
		{
		wcscpy(pszLangCpy,pszLang);
		if (Locale_Load(hWnd,szLangPath,pszLang,LOCALE_TYPE_APPLICATION,(void **)&pLocale,&pszLocaleName))
			{
			//--- Destroy the menu ---
			SetMenu(App.hWnd,NULL);
			DrawMenuBar(App.hWnd);
			Menu_Release(App.hMenu,&MainMenu);
			App.hMenu = NULL;

			//--- Destroy the XML view ---
			Tree_Destroy();

			//--- Apply the new language ---
			Locale_Unload(LOCALE_TYPE_APPLICATION,(void **)&App.pLocaleTexts,&App.pszLocaleName);
			if (App.Config.pszLocaleName) HeapFree(App.hHeap,0,App.Config.pszLocaleName);
			App.Config.pszLocaleName = pszLangCpy;
			App.pLocaleTexts = pLocale;
			App.pszLocaleName = pszLocaleName;

			Locale_Unload(LOCALE_TYPE_MISC,(void **)&App.Game.pItemsLocale,NULL);
			Locale_Load(hWnd,szLangPath,pszLang,LOCALE_TYPE_MISC,(void **)&App.Game.pItemsLocale,NULL);
			Game_ResetDisplayNames();

			//--- Recreate the menu ---
			App.hMenu = Menu_Create(&MainMenu);
			if (App.hMenu)
				{
				SetMenu(App.hWnd,App.hMenu);
				DrawMenuBar(App.hWnd);
				}
			if (App.Game.Save.pszSaveName)
				{
				Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_FILE);
				if (App.Game.pdcCurrent) Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_TREE);
				}
			Menu_SetFlag(App.hMenu,IDM_CONFIGSAVEONEXIT,App.Config.bSaveOnExit);

			//--- Update the accelerators ---
			if (App.hShortcuts) DestroyAcceleratorTable(App.hShortcuts);
			App.hShortcuts = NULL;
			Initialise_Accelerators();

			//--- Append last files ---
			LastFiles_AppendItems();

			//--- Update buttons texts ---
			SetWindowText(App.Game.Layout.hwndAbilitiesBtn,Locale_GetText(TEXT_BUTTON_ABILITIES));
			SetWindowText(App.Game.Layout.hwndTagsBtn,Locale_GetText(TEXT_BUTTON_TAGS));
			SetWindowText(App.Game.Layout.hwndTalentsBtn,Locale_GetText(TEXT_BUTTON_TALENTS));
			SetWindowText(App.Game.Layout.hwndSkillsBtn,Locale_GetText(TEXT_BUTTON_SKILLS));
			SetWindowText(App.Game.Layout.hwndInfosBtn,Locale_GetText(TEXT_BUTTON_STATS));
			SetWindowText(App.Game.Layout.hwndMenuBtn,Locale_GetText(TEXT_BUTTON_MENU));

			//--- Update inventory list groups names ---
			lvGroup.cbSize = sizeof(LVGROUP);
			lvGroup.mask = LVGF_HEADER;
			lvGroup.pszHeader = Locale_GetText(TEXT_INV_NAV);
			SendMessage(App.Game.Layout.hwndInventory,LVM_SETGROUPINFO,(WPARAM)0,(LPARAM)&lvGroup);
			lvGroup.pszHeader = Locale_GetText(TEXT_INV_BAGS);
			SendMessage(App.Game.Layout.hwndInventory,LVM_SETGROUPINFO,(WPARAM)1,(LPARAM)&lvGroup);
			lvGroup.pszHeader = Locale_GetText(TEXT_INV_EQUIPPED);
			SendMessage(App.Game.Layout.hwndInventory,LVM_SETGROUPINFO,(WPARAM)2,(LPARAM)&lvGroup);
			lvGroup.pszHeader = Locale_GetText(TEXT_INV_ITEMS);
			SendMessage(App.Game.Layout.hwndInventory,LVM_SETGROUPINFO,(WPARAM)3,(LPARAM)&lvGroup);

			//--- Update window ---
			SendMessage(App.Game.Layout.hwndInventory,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_ItemsListSort);
			InvalidateRect(App.Game.Layout.hwndInventory,NULL,FALSE);
			InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
			InvalidateRect(App.Game.Layout.hwndList,NULL,FALSE);
			InvalidateRect(App.hWnd,NULL,FALSE);
			UpdateWindow(App.hWnd);
			}
		else
			{
			HeapFree(App.hHeap,0,pszLangCpy);
			return(0);
			}
		}
	else
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_LOCALE_SELECT),NULL,MB_ICONERROR);
		return(0);
		}

	return(1);
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Locale.h"
#include "Texts.h"
#include "Menus.h"
#include "Startup.h"
#include "LastFiles.h"
#include "Dialogs.h"
#include "Requests.h"
#include "Utils.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	MainMenu;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement et sauvegarde					  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Chargement «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Config_Load(CONFIG *pConfig)
{
	NODE		Root;
	FILEHEADER	Header;
	HANDLE		hFile;
	CONFIGENTRY*	pEntry;
	void*		pData;
	WCHAR*		pszErrorMsg;
	UINT		uLen;
	UINT		uIdent;
	UINT		uType;
	DWORD		dwRead;
	BOOL		bCompleted;

	if (!PathFileExists(szConfigPath)) return;

	ZeroMemory(&Root,sizeof(NODE));
	bCompleted = FALSE;
	pszErrorMsg = Locale_GetText(TEXT_ERR_CONFIGLOAD);
	if (!pszErrorMsg) pszErrorMsg = szConfigLoadErr;

	hFile = CreateFile(szConfigPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Done;

	//--- Vérification de l'en-tête ---

	if (!ReadFile(hFile,&Header,sizeof(FILEHEADER),&dwRead,NULL)) goto Done;
	if (dwRead != sizeof(FILEHEADER)) goto Done;

	if (Header.head != FILE_HEADER_CONFIG || Header.version == 0 || Header.version > CONFIG_THIS_VERSION)
		{
		SetLastError(ERROR_INVALID_DATA);
		goto Done;
		}

	//--- Chargement de la configuration ---

	while (1)
		{
		if (!ReadFile(hFile,&uIdent,sizeof(UINT),&dwRead,NULL)) goto Done;
		if (dwRead != sizeof(UINT)) break;
		if (!ReadFile(hFile,&uType,sizeof(UINT),&dwRead,NULL)) goto Done;
		if (dwRead != sizeof(UINT)) goto Done;

		pEntry = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(CONFIGENTRY));
		if (!pEntry)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}
		pEntry->node.type = uType;
		pEntry->uIdent = uIdent;
		List_AddEntry((NODE *)pEntry,&Root);

		switch(uType)
			{
			case CONFIG_TYPE_TEXT:
				if (!ReadFile(hFile,&uLen,sizeof(UINT),&dwRead,NULL)) goto Done;
				if (dwRead != sizeof(UINT)) goto Done;
				if (!uLen) break;
				pEntry->pszValue = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
				if (!pEntry->pszValue) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }
				if (!ReadFile(hFile,pEntry->pszValue,uLen*sizeof(WCHAR),&dwRead,NULL)) goto Done;
				if (dwRead != uLen*sizeof(WCHAR)) goto Done;
				pEntry->pszValue[uLen] = 0;
				break;
			case CONFIG_TYPE_UINT:
				if (!ReadFile(hFile,&pEntry->uValue,sizeof(UINT),&dwRead,NULL)) goto Done;
				if (dwRead != sizeof(UINT)) goto Done;
				break;
			case CONFIG_TYPE_BOOL:
				if (!ReadFile(hFile,&pEntry->bValue,sizeof(BOOL),&dwRead,NULL)) goto Done;
				if (dwRead != sizeof(BOOL)) goto Done;
				break;
			}
		}

	//--- Applique la configuration en conservant les paramètres par défaut ---

	for (pEntry = (CONFIGENTRY *)Root.next; pEntry != NULL; pEntry = (CONFIGENTRY *)pEntry->node.next)
		{
		switch(pEntry->uIdent)
			{
			case CONFIG_IDENT_SAVEONEXIT_V1:
				pData = &pConfig->bSaveOnExit;
				break;
			case CONFIG_IDENT_LOCALENAME_V1:
				pData = &pConfig->pszLocaleName;
				break;
			case CONFIG_IDENT_GAME_V1:
				pData = &pConfig->uGame;
				break;
			case CONFIG_IDENT_PROFILE_V1:
				pData = &pConfig->pszProfile;
				break;
			case CONFIG_IDENT_SHOWHIDDENTAGS_V1:
				pData = &pConfig->bShowHiddenTags;
				break;
			case CONFIG_IDENT_RUNESGROUPS_V1:
				pData = &pConfig->bRunesGroups;
				break;
			case CONFIG_IDENT_RUNESVIEW_V1:
				pData = &pConfig->uRunesView;
				break;
			case CONFIG_IDENT_SKILLSGROUPS_V1:
				pData = &pConfig->bSkillsGroups;
				break;
			case CONFIG_IDENT_SKILLSVIEW_V1:
				pData = &pConfig->uSkillsView;
				break;
			case CONFIG_IDENT_BOOSTERSGROUPS_V1:
				pData = &pConfig->bBoostersGroups;
				break;
			default:pData = NULL;
			}

		if (!pData) continue;

		switch(pEntry->node.type)
			{
			case CONFIG_TYPE_TEXT:
				if (*(WCHAR **)pData) HeapFree(App.hHeap,0,*(WCHAR **)pData);
				*(WCHAR **)pData = pEntry->pszValue;
				pEntry->pszValue = NULL;
				break;
			case CONFIG_TYPE_UINT:
				*(UINT *)pData = pEntry->uValue;
				break;
			case CONFIG_TYPE_BOOL:
				*(BOOL *)pData = pEntry->bValue;
				break;
			}
		}

	bCompleted = TRUE;

	//--- Terminé ---

Done:	if (!bCompleted) Request_PrintError(App.hWnd,pszErrorMsg,NULL,MB_ICONERROR);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	for (pEntry = (CONFIGENTRY *)Root.next; pEntry != NULL; pEntry = (CONFIGENTRY *)pEntry->node.next)
		{
		switch(pEntry->node.type)
			{
			case CONFIG_TYPE_TEXT:
				if (pEntry->pszValue) HeapFree(App.hHeap,0,pEntry->pszValue);
				break;
			}
		}
	List_ReleaseMemory(&Root);
	return;
}


// «»»» Sauvegarde «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL Config_Save(BOOL bQuiet, CONFIG *pConfig)
{
	HANDLE		hFile;
	DWORD		dwWrite;
	BOOL		bCompleted;

	bCompleted = FALSE;
	hFile = CreateFile(szConfigPath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Done;
	if (!WriteFile(hFile,&pConfig->header,sizeof(FILEHEADER),&dwWrite,NULL)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SAVEONEXIT_V1,&pConfig->bSaveOnExit)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_LOCALENAME_V1,&pConfig->pszLocaleName)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_GAME_V1,&pConfig->uGame)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_PROFILE_V1,&pConfig->pszProfile)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SHOWHIDDENTAGS_V1,&pConfig->bShowHiddenTags)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_RUNESGROUPS_V1,&pConfig->bRunesGroups)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_RUNESVIEW_V1,&pConfig->uRunesView)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SKILLSGROUPS_V1,&pConfig->bSkillsGroups)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_SKILLSVIEW_V1,&pConfig->uSkillsView)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_BOOSTERSGROUPS_V1,&pConfig->bBoostersGroups)) goto Done;
	bCompleted = TRUE;

Done:	if (!bCompleted) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_CONFIGWRITE),NULL,MB_ICONERROR);
	if (hFile != INVALID_HANDLE_VALUE)
		{
		CloseHandle(hFile);
		if (!bCompleted) DeleteFile(szConfigPath);
		}
	if (bCompleted && !bQuiet) MessageBox(App.hWnd,Locale_GetText(TEXT_CONFIG_WRITTEN),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
	return(bCompleted);
}

//--- Ecriture d'une entrée ---

int Config_WriteEntry(HANDLE hFile, UINT uType, UINT uIdent, void *pData)
{
	DWORD	dwWrite;
	UINT	uLen;

	if (!WriteFile(hFile,&uIdent,sizeof(UINT),&dwWrite,NULL)) return(0);
	if (!WriteFile(hFile,&uType,sizeof(UINT),&dwWrite,NULL)) return(0);

	switch(uType)
		{
		case CONFIG_TYPE_TEXT:
			uLen = 0;
			if (*(WCHAR **)pData) uLen = wcslen(*(WCHAR **)pData);
			if (!WriteFile(hFile,&uLen,sizeof(UINT),&dwWrite,NULL)) return(0);
			if (*(WCHAR **)pData && !WriteFile(hFile,*(WCHAR **)pData,uLen*sizeof(WCHAR),&dwWrite,NULL)) return(0);
			break;
		case CONFIG_TYPE_UINT:
			if (!WriteFile(hFile,pData,sizeof(UINT),&dwWrite,NULL)) return(0);
			break;
		case CONFIG_TYPE_BOOL:
			if (!WriteFile(hFile,pData,sizeof(BOOL),&dwWrite,NULL)) return(0);
			break;
		}

	return(1);
}


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
		LOCALE_ENUM*	pEnum;
		WCHAR*		pszText;
		LRESULT		lResult;
		DWORD_PTR	vl[1];

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pRoot = (NODE *)lParam;

		for (pEnum = (LOCALE_ENUM *)pRoot->next; pEnum != NULL; pEnum = (LOCALE_ENUM *)pEnum->node.next)
			{
			//--- Add string
			lResult = SendDlgItemMessage(hDlg,200,LB_ADDSTRING,0,(LPARAM)pEnum);
			if (lResult == LB_ERR || lResult == LB_ERRSPACE)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
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
				SendDlgItemMessage(hDlg,200,LB_SETCURSEL,(WPARAM)lResult,0);
				EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
				}
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

//--- Sélection du language ---

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

			//--- Apply the new language ---
			Locale_Unload(LOCALE_TYPE_APPLICATION,(void **)&App.pLocaleTexts,&App.pszLocaleName);
			if (App.Config.pszLocaleName) HeapFree(App.hHeap,0,App.Config.pszLocaleName);
			App.Config.pszLocaleName = pszLangCpy;
			App.pLocaleTexts = pLocale;
			App.pszLocaleName = pszLocaleName;

			//--- Recreate the menu ---
			App.hMenu = Menu_Create(&MainMenu);
			if (App.hMenu)
				{
				SetMenu(App.hWnd,App.hMenu);
				DrawMenuBar(App.hWnd);
				}

			//--- Update the accelerators ---
			if (App.hShortcuts) DestroyAcceleratorTable(App.hShortcuts);
			App.hShortcuts = NULL;
			Initialise_Accelerators();

			//--- Update everything else ---
			LastFiles_AppendItems();

			SetWindowText(App.Game.Layout.hwndAbilitiesBtn,Locale_GetText(TEXT_BUTTON_ABILITIES));
			SetWindowText(App.Game.Layout.hwndTagsBtn,Locale_GetText(TEXT_BUTTON_TAGS));
			SetWindowText(App.Game.Layout.hwndTalentsBtn,Locale_GetText(TEXT_BUTTON_TALENTS));
			SetWindowText(App.Game.Layout.hwndMenuBtn,Locale_GetText(TEXT_BUTTON_MENU));

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
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_LOCALE_SELECT),NULL,MB_ICONERROR);
		return(0);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Valeurs par défaut ««««««««««««««««««««««««««««««««««««««««««««««»

int Config_Defaults(CONFIG *pConfig)
{
	static const GUID	FOLDERID_Documents = { 0xfdd39ad0, 0x238f, 0x46af, { 0xad, 0xb4, 0x6c, 0x85, 0x48, 0x03, 0x69, 0xc7 } };
	WCHAR*			pszError;
	WCHAR*			pszTemp;
	int			iSize;

	pszError = Locale_GetText(TEXT_ERR_CONFIG);
	if (!pszError) pszError = szConfigErr;

	//--- Header ---

	pConfig->header.head = FILE_HEADER_CONFIG;
	pConfig->header.version = CONFIG_THIS_VERSION;
	pConfig->header.size = sizeof(CONFIG);

	//--- Config ---

	pConfig->bSaveOnExit = TRUE;

	//--- Locale database ---

	iSize = GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SISO639LANGNAME2,NULL,0);
	pConfig->pszLocaleName = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR));
	if (!pConfig->pszLocaleName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		return(0);
		}
	GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SISO639LANGNAME2,pConfig->pszLocaleName,iSize);

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szLangPath,0,0,(WCHAR *)&pszTemp,1,(va_list *)&pConfig->pszLocaleName))
		{
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		return(0);
		}

	if (!PathFileExists(pszTemp))
		{
		HeapFree(App.hHeap,0,pConfig->pszLocaleName);
		pConfig->pszLocaleName = Misc_StrCpyAlloc(szLangDefault);
		if (!pConfig->pszLocaleName)
			{
			Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
			LocalFree(pszTemp);
			return(0);
			}
		}

	LocalFree(pszTemp);

	//--- Jeu ---

	SHGetKnownFolderPath(&FOLDERID_Documents,KF_FLAG_DEFAULT,NULL,&pszTemp);
	if (!pszTemp)
		{
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		return(0);
		}
	pConfig->pszLarianPath = HeapAlloc(App.hHeap,0,wcslen(pszTemp)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(szLarianStudios)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pConfig->pszLarianPath)
		{
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		CoTaskMemFree(pszTemp);
		return(0);
		}
	wcscpy(pConfig->pszLarianPath,pszTemp);
	PathAppend(pConfig->pszLarianPath,szLarianStudios);
	CoTaskMemFree(pszTemp);

	iSize = GetTempPath(0,NULL);
	pConfig->pszTempPath = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pConfig->pszTempPath)
		{
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		return(0);
		}
	GetTempPath(iSize,pConfig->pszTempPath);

	//--- Edition ---

	pConfig->bRunesGroups = TRUE;
	pConfig->uRunesView = LV_VIEW_TILE;
	pConfig->bSkillsGroups = TRUE;
	pConfig->uSkillsView = LV_VIEW_ICON;
	pConfig->bBoostersGroups = TRUE;

	//--- Configuration sauvegardée ---

	Config_Load(pConfig);
	return(1);
}


// «»»» Libère les données utilisées par la configuration «««««««««««««««»

void Config_Release(CONFIG *pConfig)
{
	if (pConfig->pszLocaleName) HeapFree(App.hHeap,0,pConfig->pszLocaleName);
	if (pConfig->pszProfile) HeapFree(App.hHeap,0,pConfig->pszProfile);
	if (pConfig->pszLarianPath) HeapFree(App.hHeap,0,pConfig->pszLarianPath);
	if (pConfig->pszTempPath) HeapFree(App.hHeap,0,pConfig->pszTempPath);
	pConfig->pszLocaleName = NULL;
	pConfig->pszProfile = NULL;
	pConfig->pszLarianPath = NULL;
	pConfig->pszTempPath = NULL;
	return;
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration - Routines principales
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Texts.h"
#include "Requests.h"
#include "Utils.h"

extern APPLICATION		App;


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
	BOOL		bTempPathIsValid;
	BOOL		bLarianPathIsValid;

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
			case CONFIG_TYPE_WINDOW:
				if (!ReadFile(hFile,&pEntry->window,sizeof(CONFIGWINDOW),&dwRead,NULL)) goto Done;
				if (dwRead != sizeof(CONFIGWINDOW)) goto Done;
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
			case CONFIG_IDENT_LOCALENAME_V1:				// V2: Ignored
				pData = NULL;
				break;
			case CONFIG_IDENT_LOCALENAME_V2:
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
			case CONFIG_IDENT_CAPOVERRIDE_V1:
				pData = &pConfig->bCapOverride;
				break;
			case CONFIG_IDENT_SAVELOCATION_V1:
				pData = &pConfig->pszLarianPath;
				break;
			case CONFIG_IDENT_TEMPLOCATION_V1:
				pData = &pConfig->pszTempPath;
				break;
			case CONFIG_IDENT_ITEMSDISPLAYNAME_V1:
				pData = &pConfig->bItemsDisplayName;
				break;
			case CONFIG_IDENT_ITEMSRESOLVE_V1:
				pData = &pConfig->bItemsResolve;
				break;
			case CONFIG_IDENT_WINDOW_MAIN_V1:
				pData = &pConfig->windowMain;
				break;
			case CONFIG_IDENT_WINDOW_TV_V1:
				pData = &pConfig->windowTreeView;
				break;
			case CONFIG_IDENT_TVSEARCHCASESENSITIVE_V1:
				pData = &pConfig->bTVSearchCaseSensitive;
				break;
			case CONFIG_IDENT_TVSEARCHOPACITY_V1:
				pData = &pConfig->bTVSearchOpacity;
				break;
			case CONFIG_IDENT_TVSEARCHALPHA_V1:
				pData = &pConfig->uTVSearchAlpha;
				break;
			case CONFIG_IDENT_TVSEARCHHISTORY_V1:
				pData = &pConfig->bTVSearchHistory;
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
			case CONFIG_TYPE_WINDOW:
				CopyMemory(pData,&pEntry->window,sizeof(CONFIGWINDOW));
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

	//--- Vérifie que les chemins sont toujours valides ---

	bLarianPathIsValid = PathFileExists(pConfig->pszLarianPath);
	bTempPathIsValid = PathFileExists(pConfig->pszTempPath);

	if (bTempPathIsValid && !bLarianPathIsValid) // SAVEGAME path is NOT OK, TEMP path is OK
		{
		WCHAR*	pszWarningMsg;
		WCHAR*	pszWarningTitle;

		pszWarningMsg = Locale_GetText(TEXT_ERR_CONFIGCUSTOMSAVELOCATION);
		pszWarningTitle = Locale_GetText(TEXT_TITLE_WARNING);
		if (!pszWarningMsg) pszWarningMsg = szConfigCustSaveLocationErr;
		if (!pszWarningTitle) pszWarningTitle = szWarning;
		MessageBox(App.hWnd,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		Config_DefaultSaveLocation(&pConfig->pszLarianPath,FALSE);
		}

	else if (!bTempPathIsValid && bLarianPathIsValid) // SAVEGAME path is OK, TEMP path is NOT OK
		{
		WCHAR*	pszWarningMsg;
		WCHAR*	pszWarningTitle;

		pszWarningMsg = Locale_GetText(TEXT_ERR_CONFIGCUSTOMTEMPLOCATION);
		pszWarningTitle = Locale_GetText(TEXT_TITLE_WARNING);
		if (!pszWarningMsg) pszWarningMsg = szConfigCustTempLocationErr;
		if (!pszWarningTitle) pszWarningTitle = szWarning;
		MessageBox(App.hWnd,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		Config_DefaultTempLocation(&pConfig->pszTempPath,FALSE);
		}

	else if (!bTempPathIsValid && !bLarianPathIsValid) // BOTH paths are NOT OK
		{
		WCHAR*	pszWarningMsg;
		WCHAR*	pszWarningTitle;

		pszWarningMsg = Locale_GetText(TEXT_ERR_CONFIGCUSTOMLOCATION);
		pszWarningTitle = Locale_GetText(TEXT_TITLE_WARNING);
		if (!pszWarningMsg) pszWarningMsg = szConfigCustLocationErr;
		if (!pszWarningTitle) pszWarningTitle = szWarning;
		MessageBox(App.hWnd,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		Config_DefaultSaveLocation(&pConfig->pszLarianPath,FALSE);
		Config_DefaultTempLocation(&pConfig->pszTempPath,FALSE);
		}

	return;
}


// «»»» Sauvegarde «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL Config_Save(BOOL bOnExit, CONFIG *pConfig)
{
	HANDLE		hFile;
	DWORD		dwWrite;
	BOOL		bCompleted;
	DWORD		dwLastError;

	if (!PathFileExists(szConfigFolderPath))
		{
		CreateDirectory(szConfigFolderPath,NULL);
		SetLastError(ERROR_SUCCESS);
		}

	bCompleted = FALSE;
	hFile = CreateFile(szConfigPath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Done;
	if (!WriteFile(hFile,&pConfig->header,sizeof(FILEHEADER),&dwWrite,NULL)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SAVEONEXIT_V1,&pConfig->bSaveOnExit)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_LOCALENAME_V2,&pConfig->pszLocaleName)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_GAME_V1,&pConfig->uGame)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_PROFILE_V1,&pConfig->pszProfile)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SHOWHIDDENTAGS_V1,&pConfig->bShowHiddenTags)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_RUNESGROUPS_V1,&pConfig->bRunesGroups)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_RUNESVIEW_V1,&pConfig->uRunesView)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_SKILLSGROUPS_V1,&pConfig->bSkillsGroups)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_SKILLSVIEW_V1,&pConfig->uSkillsView)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_BOOSTERSGROUPS_V1,&pConfig->bBoostersGroups)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_CAPOVERRIDE_V1,&pConfig->bCapOverride)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_SAVELOCATION_V1,&pConfig->pszLarianPath)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_TEXT,CONFIG_IDENT_TEMPLOCATION_V1,&pConfig->pszTempPath)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_ITEMSDISPLAYNAME_V1,&pConfig->bItemsDisplayName)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_ITEMSRESOLVE_V1,&pConfig->bItemsResolve)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_WINDOW,CONFIG_IDENT_WINDOW_MAIN_V1,&pConfig->windowMain)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_WINDOW,CONFIG_IDENT_WINDOW_TV_V1,&pConfig->windowTreeView)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_TVSEARCHCASESENSITIVE_V1,&pConfig->bTVSearchCaseSensitive)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_TVSEARCHOPACITY_V1,&pConfig->bTVSearchOpacity)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_UINT,CONFIG_IDENT_TVSEARCHALPHA_V1,&pConfig->uTVSearchAlpha)) goto Done;
	if (!Config_WriteEntry(hFile,CONFIG_TYPE_BOOL,CONFIG_IDENT_TVSEARCHHISTORY_V1,&pConfig->bTVSearchHistory)) goto Done;
	bCompleted = TRUE;

Done:	dwLastError = GetLastError();
	if (hFile != INVALID_HANDLE_VALUE)
		{
		CloseHandle(hFile);
		if (!bCompleted) DeleteFile(szConfigPath);
		}
	SetLastError(dwLastError);

	if (!bCompleted)
		{
		if (bOnExit) bCompleted = Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_CONFIGWRITEONEXIT),NULL,MB_ICONERROR|MB_YESNO) == IDYES?TRUE:FALSE;
		else Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_CONFIGWRITE),NULL,MB_ICONERROR);
		}
	else if (!bOnExit) MessageBox(App.hWnd,Locale_GetText(TEXT_CONFIG_WRITTEN),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);

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
		case CONFIG_TYPE_WINDOW:
			if (!WriteFile(hFile,pData,sizeof(CONFIGWINDOW),&dwWrite,NULL)) return(0);
			break;
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Libère les données utilisées par la configuration «««««««««««««««»

void Config_Release(CONFIG *pConfig)
{
	if (pConfig->pszLocaleNameLS) HeapFree(App.hHeap,0,pConfig->pszLocaleNameLS);
	if (pConfig->pszLocaleName) HeapFree(App.hHeap,0,pConfig->pszLocaleName);
	if (pConfig->pszProfile) HeapFree(App.hHeap,0,pConfig->pszProfile);
	if (pConfig->pszLarianPath) HeapFree(App.hHeap,0,pConfig->pszLarianPath);
	if (pConfig->pszTempPath) HeapFree(App.hHeap,0,pConfig->pszTempPath);
	pConfig->pszLocaleNameLS = NULL;
	pConfig->pszLocaleName = NULL;
	pConfig->pszProfile = NULL;
	pConfig->pszLarianPath = NULL;
	pConfig->pszTempPath = NULL;
	return;
}

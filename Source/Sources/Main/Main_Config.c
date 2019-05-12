
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

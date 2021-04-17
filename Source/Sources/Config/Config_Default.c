
//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration - Valeurs par défaut
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Requests.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Valeurs par défaut ««««««««««««««««««««««««««««««««««««««««««««««»

int Config_Defaults(CONFIG *pConfig)
{
	WCHAR*			pszError;
	WCHAR*			pszTemp;

	pszError = Locale_GetText(TEXT_ERR_CONFIG);
	if (!pszError) pszError = szConfigErr;

	//--- Header ---

	pConfig->header.head = FILE_HEADER_CONFIG;
	pConfig->header.version = CONFIG_THIS_VERSION;
	pConfig->header.size = sizeof(CONFIG);

	//--- Config ---

	pConfig->bSaveOnExit = TRUE;

	//--- Locale database ---

	pConfig->pszLocaleName = HeapAlloc(App.hHeap,0,LOCALE_NAME_MAX_LENGTH*sizeof(WCHAR));
	if (!pConfig->pszLocaleName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(NULL,pszError,NULL,MB_ICONHAND);
		return(0);
		}
	GetUserDefaultLocaleName(pConfig->pszLocaleName,LOCALE_NAME_MAX_LENGTH);

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

	//--- LS default locale ---

	pConfig->pszLocaleNameLS = Config_DefaultLocaleLS(pConfig->pszLocaleName);

	//--- Jeu ---

	Config_DefaultSaveLocation(&pConfig->pszLarianPath,FALSE);
	Config_DefaultTempLocation(&pConfig->pszTempPath,FALSE);

	//--- Fenêtre ---

	pConfig->windowMain.position.iWidth = MAIN_WINDOW_WIDTH;
	pConfig->windowMain.position.iHeight = MAIN_WINDOW_HEIGHT;
	pConfig->windowMain.usedefault.bCoords = TRUE;
	pConfig->windowMain.usedefault.bSize = TRUE;
	pConfig->windowTreeView.position.iWidth = TREEVIEW_WIDTH;
	pConfig->windowTreeView.position.iHeight = TREEVIEW_HEIGHT;
	pConfig->windowTreeView.usedefault.bCoords = TRUE;
	pConfig->windowTreeView.usedefault.bSize = TRUE;
	#if _DEBUG
	pConfig->bTVDebug = TRUE;
	#else
	pConfig->bTVDebug = FALSE;
	#endif

	//--- Affichage ---

	pConfig->bItemsDisplayName = TRUE;
	pConfig->bItemsResolve = TRUE;
	pConfig->crStats = RGB(180,180,180);

	//--- Recherche (TreeView) ---

	pConfig->bTVSearchOpacity = TRUE;
	pConfig->uTVSearchAlpha = 200;
	pConfig->bTVSearchHistory = TRUE;

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


// «»»» Répertoire temporaire par défaut ««««««««««««««««««««««««««««««««»

BOOL Config_DefaultTempLocation(WCHAR **pszPath, BOOL bQuiet)
{
	WCHAR*			pszWarningMsg;
	WCHAR*			pszWarningTitle;
	int			iSize;

	pszWarningMsg = Locale_GetText(TEXT_ERR_CONFIGTEMPLOCATION);
	pszWarningTitle = Locale_GetText(TEXT_TITLE_WARNING);
	if (!pszWarningMsg) pszWarningMsg = szConfigTempLocationErr;
	if (!pszWarningTitle) pszWarningTitle = szWarning;

	if (*pszPath) HeapFree(App.hHeap,0,*pszPath);

	iSize = GetTempPath(0,NULL);
	*pszPath = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR)+sizeof(WCHAR));
	if (!*pszPath)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		if (!bQuiet) Request_PrintError(NULL,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		return(FALSE);
		}
	GetTempPath(iSize,*pszPath);
	return(TRUE);
}


// «»»» Répertoire par défaut pour les sauvegardes ««««««««««««««««««««««»

BOOL Config_DefaultSaveLocation(WCHAR **pszPath, BOOL bQuiet)
{
	static const GUID	FOLDERID_Documents = { 0xfdd39ad0, 0x238f, 0x46af, { 0xad, 0xb4, 0x6c, 0x85, 0x48, 0x03, 0x69, 0xc7 } };
	WCHAR*			pszTemp;
	WCHAR*			pszWarningMsg;
	WCHAR*			pszWarningTitle;

	pszWarningMsg = Locale_GetText(TEXT_ERR_CONFIGSAVELOCATION);
	pszWarningTitle = Locale_GetText(TEXT_TITLE_WARNING);
	if (!pszWarningMsg) pszWarningMsg = szConfigSaveLocationErr;
	if (!pszWarningTitle) pszWarningTitle = szWarning;

	SHGetKnownFolderPath(&FOLDERID_Documents,KF_FLAG_DEFAULT,NULL,&pszTemp);
	if (!pszTemp)
		{
		if (!bQuiet) Request_PrintError(NULL,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		return(FALSE);
		}
	if (*pszPath) HeapFree(App.hHeap,0,*pszPath);
	*pszPath = HeapAlloc(App.hHeap,0,wcslen(pszTemp)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(szLarianStudios)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!*pszPath)
		{
		if (!bQuiet) Request_PrintError(NULL,pszWarningMsg,pszWarningTitle,MB_ICONWARNING|MB_OK);
		CoTaskMemFree(pszTemp);
		return(FALSE);
		}
	wcscpy(*pszPath,pszTemp);
	PathAppend(*pszPath,szLarianStudios);
	CoTaskMemFree(pszTemp);
	return(TRUE);
}


// «»»» Fichier de langues par défaut (LarianStudios) «««««««««««««««««««»

WCHAR* Config_DefaultLocaleLS(WCHAR *pszLocaleName)
{
	static WCHAR* defaults[] = {	L"amlatspanish", 	L"es-AR", // Spanish (Argentina)
								L"es-BO", // Spanish (Bolivia)
								L"es-CL", // Spanish (Chile)
								L"es-CO", // Spanish (Colombia)
								L"es-CR", // Spanish (Costa Rica)
								L"es-DO", // Spanish (Dominican Republic)
								L"es-EC", // Spanish (Ecuador)
								L"es-GT", // Spanish (Guatemala)
								L"es-HN", // Spanish (Honduras)
								L"es-MX", // Spanish (Mexico)
								L"es-NI", // Spanish (Nicaragua)
								L"es-PA", // Spanish (Panama)
								L"es-PE", // Spanish (Peru)
								L"es-PR", // Spanish (Puerto Rico)
								L"es-PY", // Spanish (Paraguay)
								L"es-SV", // Spanish (El Salvador)
								L"es-UY", // Spanish (Uruguay)
								L"es-VE", // Spanish (Venezuela)
								NULL,
					L"chinese", 		L"zh-*",
								L"zh",
								NULL,
					L"czech",		L"cs-*",
								L"cs",
								NULL,
					L"french",		L"fr-*",
								L"fr",
								NULL,
					L"german",		L"de-*",
								L"de",
								NULL,
					L"italian",		L"it-*",
								L"it",
								NULL,
					L"japonese",		L"ja-*",
								L"ja",
								NULL,
					L"korean",		L"ko-*",
								L"ko",
								NULL,
					L"polish",		L"pl-*",
								L"pl",
								NULL,
					L"russian",		L"ru-*",
								L"ru",
								NULL,
					L"spanish",		L"es-*",
								L"es",
								NULL,
					NULL };

	WCHAR*	pszLSName;
	int	i = 0;

	while (defaults[i] != NULL)
		{
		pszLSName = defaults[i++];
		while (defaults[i] != NULL)
			{
			if (wcschr(defaults[i],L'*'))
				{
				if (!wcsncmp(pszLocaleName,defaults[i],3)) return(Misc_StrCpyAlloc(pszLSName));
				}
			else if (!wcscmp(pszLocaleName,defaults[i])) return(Misc_StrCpyAlloc(pszLSName));
			i++;
			}
		i++;
		}

	return(Misc_StrCpyAlloc(szLangDefaultLS));
}

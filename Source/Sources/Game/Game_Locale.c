
//<<>-<>>---------------------------------------------------------------------()
/*
	Traductions de Larian Studios
									      */
//()-------------------------------------------------------------------<<>-<>>//

#include "_Global.h"
#include "Application.h"
#include "GameLocale.h"
#include "Texts.h"
#include "Utils.h"
#include "Requests.h"
#include "Debug.h"

extern APPLICATION		App;
extern LOCALE_GAMEUITEXT	UIDisplayNames[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Conversion							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la base de données ««««««««««««««««««««««««««««««««««»

void Game_LocaleRebuild(HWND hWnd)
{
	Locale_Unload(LOCALE_TYPE_GAME,(void **)&App.pLanguage,NULL);
	defs_Rebuild(hWnd,IDM_DB_REBUILDLANGUAGES,FALSE,szDefsLanguagesPath,(LPTHREAD_START_ROUTINE)Game_LocaleRebuildThread);
	Locale_Load(NULL,szLSLPath,App.Config.pszLocaleNameLS,LOCALE_TYPE_GAME,(void **)&App.pLanguage,NULL);
	return;
}


// «»»» Tâche de création «««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Game_LocaleRebuildThread(DEFSCONTEXT *pDefs)
{
	defs_CreateMsgQueue();
	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildStarts);

	if ((pDefs->iResult = defs_CreateFileList(szDefsLanguagesPath,pDefs)) == DEFS_RESULT_OK)
		{
		DEFSFILE*	pFile;
		float		i,iMax;

		i = 0.0f;
		iMax = (float)List_EntryCount(&pDefs->files);

		for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
			{
			if ((pDefs->iResult = defs_ThreadAbort())) break;
			if ((pDefs->iResult = defs_LoadData(pFile))) break;
			if ((pDefs->iResult = defs_CreateLanguage(pFile,pDefs))) break;
			HeapFree(App.hHeap,0,pFile->pData);
			pFile->pData = NULL;
			pDefs->iPos[0] = (int)(++i/iMax*100.0f);
			}
		}

	if (pDefs->iResult == DEFS_RESULT_ABORTED) Debug_Log(DEBUG_LOG_INFO,szDebugAborted);
	if (pDefs->iResult == DEFS_RESULT_FAILED) pDefs->iShowLogs = Request_TaskDialog(pDefs->hwndParent,Locale_GetText(TEXT_DIALOG_DEFS_FAILED),Locale_GetText(TEXT_TITLE_ERROR),MB_ICONERROR|MB_YESNO);
	else pDefs->iShowLogs = Request_TaskDialog(pDefs->hwndParent,Locale_GetText(TEXT_DIALOG_DEFS_DONE),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_YESNO);

	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildEnds);
	App.hThread = NULL;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve une traduction de l'interface ««««««««««««««««««««««««««»

WCHAR* Game_LocaleNameFromLocaleID(UINT uLocaleID)
{
	int	i;

	for (i = 0; UIDisplayNames[i].uLocaleID != 0; i++)
		{
		if (UIDisplayNames[i].uLocaleID == uLocaleID)
			{
			if (UIDisplayNames[i].pszText) return(UIDisplayNames[i].pszText);
			UIDisplayNames[i].pszText = Game_LocaleStripTags(Game_LocaleNameFromHandleA(UIDisplayNames[i].pszHandle));
			if (UIDisplayNames[i].pszText) return(UIDisplayNames[i].pszText);
			break;
			}
		}

	return(Locale_GetText(uLocaleID));
}

//--- Supprime les données parasites d'un texte ---

WCHAR* Game_LocaleStripTags(WCHAR *pszText)
{
	WCHAR*	pszFinal;
	int	i,j;

	if (!pszText) return(NULL);
	if (!wcspbrk(pszText,L"<>[]")) return(pszText);

	//--- Strip tags <*> and Params [?]
	for (pszFinal = pszText, i = j = 0; pszText[i] != 0;)
		{
		for (; pszText[i] != 0; i++)
			{
			if (pszText[i] == '<') break;
			if (pszText[i] == '[') break;
			pszFinal[j++] = pszText[i];
			}
		// '\0'
		if (pszText[i] == 0) break;
		// '<tag>'
		else if (pszText[i] == '<')
			{
			for (; pszText[i] != 0; i++)
				{
				if (pszText[i] == '>') break;
				}
			if (pszText[i] == 0) break;
			i++;
			}
		// '[n]'
		else if (pszText[i] == '[')
			{
			if (pszText[i+1] == 0) // '[' at end of line
				{
				pszFinal[j++] = pszText[i];
				pszFinal[j] = 0;
				break;
				}
			if (pszText[i+2] == 0) // '[?' at end of line
				{
				pszFinal[j++] = pszText[i++];
				pszFinal[j++] = pszText[i];
				pszFinal[j] = 0;
				break;
				}
			if (pszText[i+1] >= '0' && pszText[i+1] <= '9' && pszText[i+2] == ']')
				{
				if (j > 0 && pszFinal[j-1] == '"' && pszText[i+3] == '"') // '"[n]"'
					{
					j--;
					i += 4;
					continue;
					}
				i += 3;
				continue;
				}
			}
		}
	pszFinal[j] = 0;

	//--- Remove extra spaces
	for (pszFinal = pszText, i = j = 0; pszText[i] != 0;)
		{
		if (pszText[i] != ' ')
			{
			pszFinal[j++] = pszText[i++];
			continue;
			}
		pszFinal[j++] = pszText[i++];
		while (pszText[i++] == ' ');
		--i;
		}
	pszFinal[j] = 0;

	//--- Remove extra space at the end
	if (j > 0 && pszFinal[j-1] == ' ') pszFinal[j-1] = 0;

	return(pszText);
}


// «»»» Retrouve une traduction en fonction de son handle «««««««««««««««»

WCHAR* Game_LocaleNameFromHandleA(char *pszHandle)
{
	if (App.pLanguage)
		{
		sqlite3_stmt*	stmt;
		WCHAR*		pszDisplayName = NULL;

		if (sqlite3_prepare_v2(App.pLanguage,"SELECT text FROM contentList WHERE contentuid=? LIMIT 1",-1,&stmt,NULL) == SQLITE_OK)
			{
			sqlite3_bind_text(stmt,1,pszHandle,strlen(pszHandle),NULL);
			if (sqlite3_step(stmt) == SQLITE_ROW) pszDisplayName = Misc_UTF8ToWideChar((char *)sqlite3_column_text(stmt,0));
			sqlite3_finalize(stmt);
			}

		if (pszDisplayName && !wcslen(pszDisplayName))
			{
			HeapFree(App.hHeap,0,pszDisplayName);
			pszDisplayName = NULL;
			}

		return(pszDisplayName);
		}

	return(NULL);
}


// «»»» Conversion d'un nom de fichier en nom de langue «««««««««««««««««»

WCHAR* Game_LocaleFileToNameLS(WCHAR *pszFile)
{
	static WCHAR*	pszNames[] = {	L"amlatspanish", L"Español latino",
					L"chinese", L"中文",
					L"chinesetraditional", L"繁體中文",
					L"czech", L"čeština",
					L"english", L"English",
					L"german", L"Deutsche",
					L"french", L"Français",
					L"italian", L"Italiano",
					L"japanese", L"日本語",
					L"korean", L"한국어",
					L"polish", L"Polskie",
					L"russian", L"русский",
					L"spanish", L"Español",
					NULL };
	int		i;

	for (i = 0; pszNames[i] != NULL; i += 2)
		{
		if (wcscmp(pszNames[i],pszFile)) continue;
		return(pszNames[i+1]);
		}

	return(Locale_GetText(TEXT_UNKNOWN));
}


// «»»» Fichier de langues par défaut (LarianStudios) «««««««««««««««««««»

WCHAR* Game_LocaleDefaultLS(WCHAR *pszLocaleName)
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


// «»»» Langue de tri en fonction d'un nom LS «««««««««««««««««««««««««««»

WCHAR* Game_LocaleSortLanguage(WCHAR *pszLocaleNameLS)
{
	static WCHAR*	sort[] = {	L"amlatspanish", L"es",
					L"chinese", L"zh",
					L"czech", L"cs",
					L"french", L"fr",
					L"german", L"de",
					L"italian", L"it",
					L"japonese", L"ja",
					L"korean", L"ko",
					L"polish", L"pl",
					L"russian", L"ru",
					L"spanish", L"es",
					NULL };
	int		i;

	if (pszLocaleNameLS)
		{
		for (i = 0; sort[i] != NULL; i += 2)
			{
			if (wcscmp(sort[i],pszLocaleNameLS)) continue;
			return(sort[i+1]);
			}
		}

	return(L"en");
}

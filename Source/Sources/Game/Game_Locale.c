
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

// «»»» Conversion d'un nom de fichier en nom de langue «««««««««««««««««»

WCHAR* Game_LocaleFileToNameLS(WCHAR *pszFile)
{
	static WCHAR*	pszNames[] = {	L"amlatspanish", L"Español latino",
					L"chinese", L"中国人",
					L"chinesetraditional", L"中国传统的）",
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

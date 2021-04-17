
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des définitions
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Definitions.h"
#include "XML.h"
#include "zlib.h"
#include "Utils.h"
#include "Taskbar.h"
#include "Requests.h"
#include "Debug.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Fonctions principales						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void defs_Rebuild(HWND hWnd, UINT uLocaleTitle, BOOL bForceWipe, WCHAR *pszPath, LPTHREAD_START_ROUTINE pThread)
{
	TASKDIALOGCONFIG	dialog;
	DEFSCONTEXT*		pDefs;
	int			iResult;

	//--- Vérifications ---

	if (App.hThread)
		{
		Request_TaskDialog(hWnd,Locale_GetText(TEXT_ERR_TASKRUNNING),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
		return;
		}

	if (App.Game.Save.pszSaveName)
		{
		Request_TaskDialog(hWnd,Locale_GetText(TEXT_ERR_OPENEDSAVEGAME),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
		return;
		}

	//--- Création de la structure ---

	pDefs = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DEFSCONTEXT));
	if (!pDefs)
		{
		Request_TaskDialog(hWnd,Locale_GetText(TEXT_ERR_MEMORY),Locale_GetText(TEXT_TITLE_ERROR),MB_ICONERROR|MB_OK);
		return;
		}

	iResult = wcslen(Locale_GetText(bForceWipe?TEXT_DIALOG_DEFS_CONFIRMNOWIPE:TEXT_DIALOG_DEFS_CONFIRM))*sizeof(WCHAR)+wcslen(pszPath)*sizeof(WCHAR)+sizeof(WCHAR);
	pDefs->pszConfirm = HeapAlloc(App.hHeap,0,iResult);
	if (!pDefs->pszConfirm)
		{
		defs_ReleaseAll(pDefs);
		Request_TaskDialog(hWnd,Locale_GetText(TEXT_ERR_MEMORY),Locale_GetText(TEXT_TITLE_ERROR),MB_ICONERROR|MB_OK);
		return;
		}
	Misc_Printf(pDefs->pszConfirm,iResult,Locale_GetText(bForceWipe?TEXT_DIALOG_DEFS_CONFIRMNOWIPE:TEXT_DIALOG_DEFS_CONFIRM),pszPath);

	//--- Fenêtre de confirmation ---

	ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
	dialog.cbSize = sizeof(TASKDIALOGCONFIG);
	dialog.hwndParent = hWnd;
	dialog.dwFlags = TDF_USE_HICON_MAIN|TDF_USE_HICON_FOOTER|TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
	dialog.dwCommonButtons = TDCBF_OK_BUTTON|TDCBF_CANCEL_BUTTON;
	dialog.pszWindowTitle = Locale_GetText(uLocaleTitle);
	dialog.hMainIcon = App.hShellIcons[APP_SHELLICON_INFO];
	dialog.pszContent = pDefs->pszConfirm;
	dialog.pszVerificationText = bForceWipe?NULL:Locale_GetText(TEXT_DIALOG_DEFS_WIPE);
	dialog.hFooterIcon = App.hShellIcons[APP_SHELLICON_WARNING];
	dialog.pszFooter = Locale_GetText(TEXT_DEVTOOL);
	iResult = 0;
	TaskDialogIndirect(&dialog,&iResult,NULL,&pDefs->bWipe);
	if (iResult != IDOK)
		{
		defs_ReleaseAll(pDefs);
		return;
		}

	//--- Exécution de la procédure ---

	if (bForceWipe) pDefs->bWipe = TRUE;
	pDefs->hwndParent = hWnd;

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)pThread,pDefs,0,NULL);
	if (!App.hThread)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_THREAD),NULL,MB_ICONERROR);
		defs_ReleaseAll(pDefs);
		return;
		}

	//--- Fenêtre d'attente ---

	Taskbar_SetProgressState(TBPF_INDETERMINATE);
	ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
	dialog.cbSize = sizeof(TASKDIALOGCONFIG);
	dialog.hwndParent = hWnd;
	dialog.dwFlags = TDF_USE_HICON_MAIN|TDF_SHOW_PROGRESS_BAR|TDF_CALLBACK_TIMER|TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
	dialog.dwCommonButtons = TDCBF_CANCEL_BUTTON;
	dialog.pszWindowTitle = Locale_GetText(uLocaleTitle);
	dialog.hMainIcon = App.hShellIcons[APP_SHELLICON_STACK];
	dialog.pszContent = Locale_GetText(TEXT_DIALOG_DEFS_WAIT);
	dialog.pfCallback = defs_RebuildProc;
	dialog.lpCallbackData = (INT_PTR)pDefs;
	TaskDialogIndirect(&dialog,NULL,NULL,NULL);
	Taskbar_SetProgressState(TBPF_NOPROGRESS);

	if (pDefs->iShowLogs == IDYES) ShellExecute(pDefs->hwndParent,L"open",L"notepad.exe",szDebugLogFileName,NULL,SW_SHOWNORMAL);
	defs_ReleaseAll(pDefs);
	return;
}


// «»»» Mise à jour de la progression «««««««««««««««««««««««««««««««««««»

HRESULT defs_RebuildProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData)
{
	switch(msg)
		{
		case TDN_BUTTON_CLICKED:
			if (!App.hThread) return(S_OK);
			PostThreadMessage(GetThreadId(App.hThread),WM_THREADABORT,0,0);
			break;

		case TDN_TIMER:
			if (!App.hThread) SendMessage(hWnd,TDM_CLICK_BUTTON,TDCBF_CANCEL_BUTTON,0);
			if (((DEFSCONTEXT *)lpRefData)->iPos[0] != ((DEFSCONTEXT *)lpRefData)->iPos[1])
				{
				Taskbar_SetProgressValue(((DEFSCONTEXT *)lpRefData)->iPos[0],100);
				SendMessage(hWnd,TDM_SET_PROGRESS_BAR_POS,((DEFSCONTEXT *)lpRefData)->iPos[0],0);
				((DEFSCONTEXT *)lpRefData)->iPos[1] = ((DEFSCONTEXT *)lpRefData)->iPos[0];
				}
			break;
		}

	return(S_FALSE);
}


// «»»» Messages de fin «««««««««««««««««««««««««««««««««««««««««««««««««»

void defs_DoneMessage(DEFSCONTEXT *pDefs)
{
	if (pDefs->iResult == DEFS_RESULT_ABORTED) Debug_Log(DEBUG_LOG_INFO,szDebugAborted);
	if (pDefs->iResult == DEFS_RESULT_FAILED) pDefs->iShowLogs = Request_TaskDialog(pDefs->hwndParent,Locale_GetText(TEXT_DIALOG_DEFS_FAILED),Locale_GetText(TEXT_TITLE_ERROR),MB_ICONERROR|MB_YESNO);
	else pDefs->iShowLogs = Request_TaskDialog(pDefs->hwndParent,Locale_GetText(TEXT_DIALOG_DEFS_DONE),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_YESNO);
	return;
}


// «»»» "Statistiques" de fin «««««««««««««««««««««««««««««««««««««««««««»

void defs_Stats(DEFSTATS *pStats, UINT uMode)
{
	switch(uMode)
		{
		case DEFS_STATS_BEGIN:
			ZeroMemory(pStats,sizeof(DEFSTATS));
			pStats->dwTicks = GetTickCount();
			break;

		case DEFS_STATS_END:
			Debug_Log(DEBUG_LOG_INFO,L" (%ld ms - %ld entries)",GetTickCount()-pStats->dwTicks,pStats->dwEntries);
			break;

		case DEFS_STATS_SINGLE:
			Debug_Log(DEBUG_LOG_INFO,L" (1 entry)");
			break;
		}

	return;
}


// «»»» Libération de la mémoire ««««««««««««««««««««««««««««««««««««««««»

//--- Libère toutes les définitions ---

void defs_ReleaseAll(DEFSCONTEXT *pDefs)
{
	DEFSFILE*	pFile;

	if (pDefs->pszConfirm) HeapFree(App.hHeap,0,pDefs->pszConfirm);

	for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
		{
		if (pFile->pszPath) HeapFree(App.hHeap,0,pFile->pszPath);
		if (pFile->pData) HeapFree(App.hHeap,0,pFile->pData);
		xml_ReleaseAll(&pFile->xmlRoot);
		}

	List_ReleaseMemory(&pDefs->files);
	HeapFree(App.hHeap,0,pDefs);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	GAME: Language							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int defs_CreateLanguage(DEFSFILE *pFile, DEFSCONTEXT *pDefs)
{
	DEFSTATS	Stats;
	DWORD_PTR	vl[1];
	WCHAR*		pszTempPath;
	char*		pszDataBasePath;
	char*		pszContentUID;
	char*		pszContent;
	char*		pszLine;
	int		iResult;
	int		i,j;

	//--- Création du nom de la base de données ---

	vl[0] = (DWORD_PTR)PathFindFileName(pFile->pszPath);
	if (!vl[0]) return(DEFS_RESULT_OK);
	pszTempPath = wcschr(pFile->pszPath,L'.');
	if (pszTempPath) *pszTempPath = 0;
	pszTempPath = NULL;

	if (strncmp((char *)pFile->pData,szXMLcontentList,strlen(szXMLcontentList)))
		{
		Debug_Log(DEBUG_LOG_WARNING,szDebugContentList);
		return(DEFS_RESULT_OK);
		}

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szLSLPath,0,0,(WCHAR *)&pszTempPath,1,(va_list *)vl))
		{
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}
	pszDataBasePath = Misc_WideCharToUTF8(pszTempPath);
	LocalFree(pszTempPath);
	if (!pszDataBasePath)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}

	//--- Ouverture et peuplage de la base de données ---

	pszContentUID = NULL;
	pszContent = NULL;
	j = pFile->iSize;

	if (sqlite3_open_v2(pszDataBasePath,&pDefs->db.pSql,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,NULL) == SQLITE_OK)
		{
		if (sqlite3_exec(pDefs->db.pSql,"PRAGMA journal_mode = MEMORY",NULL,NULL,NULL) != SQLITE_OK) goto Error;
		if (sqlite3_exec(pDefs->db.pSql,"CREATE TABLE IF NOT EXISTS 'contentList' ('contentuid' TEXT UNIQUE, 'text' TEXT)",NULL,NULL,NULL) != SQLITE_OK) goto Error;
		if (sqlite3_exec(pDefs->db.pSql,"CREATE INDEX IF NOT EXISTS 'contentList.index' ON 'contentList' ('contentuid' ASC)",NULL,NULL,NULL) != SQLITE_OK) goto Error;
		defs_SQLWipeTables(&pDefs->db,pDefs->bWipe,1,"contentList");
		sqlite3_exec(pDefs->db.pSql,"BEGIN TRANSACTION",NULL,NULL,NULL);
		defs_Stats(&Stats,DEFS_STATS_BEGIN);
		for (pszLine = (char *)pFile->pData; *pszLine != 0;)
			{
			i = (int)((UINT64)pszLine-(UINT64)pFile->pData);
			xml_UpdateProgress(i,j,0);
			if ((iResult = defs_ThreadAbort())) break;
			if ((iResult = defs_NextLine(&pszLine)) != DEFS_RESULT_OK) break;
			if ((iResult = defs_GetContentUID(&pszLine,&pszContentUID)) != DEFS_RESULT_OK) break;
			if ((iResult = defs_GetContent(&pszLine,&pszContent)) != DEFS_RESULT_OK) break;
			if (!pszLine || !pszContentUID || !pszContent) break;
			if ((iResult = defs_SQLQuery(&pDefs->db,"INSERT OR IGNORE INTO contentList (contentuid,text) VALUES(?,?)",2,DEFS_SQL_TEXT,pszContentUID,DEFS_SQL_TEXT,pszContent)) != DEFS_RESULT_OK) break;
			HeapFree(App.hHeap,0,pszContentUID);
			HeapFree(App.hHeap,0,pszContent);
			pszContentUID = NULL;
			pszContent = NULL;
			Stats.dwEntries++;
			}
		defs_Stats(&Stats,DEFS_STATS_END);
		sqlite3_exec(pDefs->db.pSql,"END TRANSACTION",NULL,NULL,NULL);
		iResult = DEFS_RESULT_OK;
		}
	else
		{
Error:		defs_SQLErrorMsg(pDefs->db.pSql);
		iResult = DEFS_RESULT_FAILED;
		}
	sqlite3_close(pDefs->db.pSql);
	xml_UpdateProgress(-1,-1,0);

	if (pszContentUID) HeapFree(App.hHeap,0,pszContentUID);
	if (pszContent) HeapFree(App.hHeap,0,pszContent);
	return(iResult);
}


// «»»» Prochaine ligne «««««««««««««««««««««««««««««««««««««««««««««««««»

int defs_NextLine(char **pszLine)
{
	for (; **pszLine != 0; *pszLine += 1)
		{
		if (**pszLine == 0x0D) break;
		if (**pszLine == 0x0A) break;
		}
	if (!**pszLine) { *pszLine = NULL; return(DEFS_RESULT_OK); }
	if (**pszLine == 0x0D) *pszLine += 1;
	if (**pszLine == 0x0A) *pszLine += 1;
	return(DEFS_RESULT_OK);
}


// «»»» Copie l'identifiant «««««««««««««««««««««««««««««««««««««««««««««»

int defs_GetContentUID(char **pszLine, char **pszContentUID)
{
	char*	pszBegin;
	int	i,j;

	if (!*pszLine) return(DEFS_RESULT_OK);

	for (i = 0,*pszContentUID = NULL; **pszLine != 0; *pszLine += 1, i++)
		if (**pszLine == '"') break;
	if (**pszLine != '"') return(DEFS_RESULT_OK);
	j = strlen(szXMLcontentuid);
	if (i < j) return(DEFS_RESULT_OK);
	if (strncmp(szXMLcontentuid,(*pszLine)-j,j)) return(DEFS_RESULT_OK);
	for (*pszLine += 1, pszBegin = *pszLine; **pszLine != 0; *pszLine += 1)
		if (**pszLine == '"') break;
	if (**pszLine != '"') return(DEFS_RESULT_OK);
	**pszLine = 0;
	*pszLine += 1;

	if (strlen(pszBegin) != strlen(szXMLuuid)) return(DEFS_RESULT_OK);

	*pszContentUID = defs_StrCpyAlloc(pszBegin);
	if (!*pszContentUID)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}

	return(DEFS_RESULT_OK);
}


// «»»» Copie le contenu ««««««««««««««««««««««««««««««««««««««««««««««««»

int defs_GetContent(char **pszLine, char **pszContent)
{
	char*	pszBegin;
	UINT	uLen;

	if (!*pszLine) return(DEFS_RESULT_OK);

	for (*pszContent = NULL; **pszLine != 0; *pszLine += 1)
		if (**pszLine == '>') break;
	if (**pszLine != '>') return(DEFS_RESULT_OK);
	for (*pszLine += 1, pszBegin = *pszLine; **pszLine != 0; *pszLine += 1)
		if (**pszLine == '<') break;
	if (**pszLine != '<') return(DEFS_RESULT_OK);
	**pszLine = 0;
	*pszLine += 1;

	uLen = defs_HtmlSpecialCharsDecode(NULL,pszBegin);
	*pszContent = HeapAlloc(App.hHeap,0,++uLen);
	if (!*pszContent)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}
	defs_HtmlSpecialCharsDecode(*pszContent,pszBegin);

	return(DEFS_RESULT_OK);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	ITEMS: Localization						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int defs_UpdateLocalization(DEFSQL *pDB, DEFSFILE *pFile)
{
	DEFSTATS	Stats;
	XML_NODE*	pxnKey;
	char*		pszContent;
	char*		pszUUID;
	int		iResult;
	int		i,j;

	Debug_Log(DEBUG_LOG_INFO,szDebugStats);

	pxnKey = xml_GetNodeFromPathFirstChild((XML_NODE *)pFile->xmlRoot.next,szLocalizationPath);
	if (!pxnKey)
		{
		Debug_Log(DEBUG_LOG_ERROR,szDebugXMLNoPath,szXMLTranslatedStringKeys);
		return(DEFS_RESULT_FAILED);
		}

	sqlite3_exec(pDB->pSql,"BEGIN TRANSACTION",NULL,NULL,NULL);
	defs_Stats(&Stats,DEFS_STATS_BEGIN);

	for (i = 0, j = xml_TotalNodesCount(pxnKey,FALSE), iResult = DEFS_RESULT_OK; pxnKey != NULL; pxnKey = (XML_NODE *)pxnKey->node.next, i++)
		{
		if ((iResult = defs_ThreadAbort())) break;
		xml_UpdateProgress(i,j,0);
		pszContent = Misc_WideCharToUTF8(xml_GetAttrValue(xml_GetNode((XML_NODE *)pxnKey->children.next,szXMLattribute,szXMLid,L"Content"),szXMLhandle));
		pszUUID = Misc_WideCharToUTF8(xml_GetAttrValue(xml_GetNode((XML_NODE *)pxnKey->children.next,szXMLattribute,szXMLid,L"UUID"),szXMLvalue));
		if (pszContent && pszUUID)
			{
			iResult = defs_SQLQuery(pDB,"INSERT OR IGNORE INTO TranslatedStringKeys (UUID,handle) VALUES(?,?)",2,DEFS_SQL_TEXT,pszUUID,DEFS_SQL_TEXT,pszContent);
			if (iResult != DEFS_RESULT_OK) break;
			Stats.dwEntries++;
			}
		if (pszContent) HeapFree(App.hHeap,0,pszContent);
		if (pszUUID) HeapFree(App.hHeap,0,pszUUID);
		}

	sqlite3_exec(pDB->pSql,"END TRANSACTION",NULL,NULL,NULL);
	defs_Stats(&Stats,DEFS_STATS_END);
	xml_UpdateProgress(-1,-1,0);
	return(iResult);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	ITEMS: Statistiques						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int defs_UpdateStats(DEFSQL *pDB, DEFSFILE *pFile)
{
	static WCHAR*	pszAllowedFiles[] = { L"ItemProgressionNames.", L"ItemProgressionVisuals.", L"Armor.", L"Object.", L"Potion.", L"Shield.", L"Weapon.", NULL };
	WCHAR*		pszFile;
	int		iResult;
	int		i;

	pszFile = PathFindFileName(pFile->pszPath);
	if (!pszFile) return(DEFS_RESULT_OK);

	Debug_Log(DEBUG_LOG_INFO,szDebugStats);
	iResult = DEFS_RESULT_OK;

	for (i = 0; pszAllowedFiles[i] != NULL; i++)
		{
		if (!wcsncmp(pszAllowedFiles[i],pszFile,wcslen(pszAllowedFiles[i])))
			{
			sqlite3_exec(pDB->pSql,"BEGIN TRANSACTION",NULL,NULL,NULL);
			iResult = defs_UpdateStatsTable(pDB,pFile);
			sqlite3_exec(pDB->pSql,"END TRANSACTION",NULL,NULL,NULL);
			break;
			}
		}
	if (pszAllowedFiles[i] == NULL) Debug_Log(DEBUG_LOG_WARNING,szDebugStatsSkip);

	return(iResult);
}


// «»»» Mise-à-jour des stats «««««««««««««««««««««««««««««««««««««««««««»

int defs_UpdateStatsTable(DEFSQL *pDB, DEFSFILE *pFile)
{
	DEFSTATS	stats;
	DOS2ITEMGROUP	group;
	DOS2ITEMENTRY	entry;
	char*		pszPointer;
	char*		pszParentGroup;
	char*		pszParentEntry;
	char**		pParts;
	int		iResult;
	int		iIndex0,iIndex1;
	int		i,j;

	ZeroMemory(&group,sizeof(DOS2ITEMGROUP));
	ZeroMemory(&entry,sizeof(DOS2ITEMENTRY));
	defs_Stats(&stats,DEFS_STATS_BEGIN);
	iResult = DEFS_RESULT_OK;
	pParts = NULL;
	pszParentGroup = pszParentEntry = NULL;
	iIndex0 = iIndex1 = 0;
	j = pFile->iSize;

	for (pszPointer = (char *)pFile->pData; pszPointer != NULL;)
		{
		i = (int)((UINT64)pszPointer-(UINT64)pFile->pData);
		xml_UpdateProgress(i,j,0);
		if ((iResult = defs_ThreadAbort())) break;
		pszPointer = defs_UpdateStatsTableExplode(pszPointer,&pParts);
		if (!pParts || !pParts[0]) continue;

		if (!strcmp(pParts[0],"new"))
			{
			if (!pParts[1])
				{
				Debug_Log(DEBUG_LOG_ERROR,L"Empty command new");
				iResult = DEFS_RESULT_FAILED;
				break;
				}
			//--- new namegroup "NameGroup"
			else if (!strcmp(pParts[1],"namegroup"))
				{
				if (!pParts[2])
					{
					Debug_Log(DEBUG_LOG_ERROR,L"new namegroup has no name");
					iResult = DEFS_RESULT_FAILED;
					break;
					}
				if (!pszPointer) break;
				if (pszParentGroup) HeapFree(App.hHeap,0,pszParentGroup);
				iIndex0 = iIndex1 = 0;
				pszParentGroup = defs_StrCpyAlloc(pParts[2]);
				if (!pszParentGroup)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Debug_LogSystemError();
					iResult = DEFS_RESULT_FAILED;
					break;
					}
				}
			//--- new itemgroup "ItemGroup"
			else if (!strcmp(pParts[1],"itemgroup"))
				{
				if (!pParts[2])
					{
					Debug_Log(DEBUG_LOG_ERROR,L"new itemgroup has no name");
					iResult = DEFS_RESULT_FAILED;
					break;
					}
				if (!pszPointer) break;
				if (pszParentGroup) HeapFree(App.hHeap,0,pszParentGroup);
				iIndex0 = iIndex1 = 0;
				defs_UpdateStatsTableFlush(NULL,0,NULL,DEFS_FLUSH_GROUP,&group);
				pszParentGroup = defs_StrCpyAlloc(pParts[2]);
				if (!pszParentGroup)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Debug_LogSystemError();
					iResult = DEFS_RESULT_FAILED;
					break;
					}
				}
			//--- new entry "Entry"
			else if (!strcmp(pParts[1],"entry"))
				{
				iResult = defs_UpdateStatsTableFlush(pDB,0,pszParentEntry,DEFS_FLUSH_ENTRY,&entry);
				if (iResult != DEFS_RESULT_OK) break;
				if (pszParentEntry) HeapFree(App.hHeap,0,pszParentEntry);
				stats.dwEntries++;
				pszParentEntry = defs_StrCpyAlloc(pParts[2]);
				if (!pszParentEntry)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Debug_LogSystemError();
					iResult = DEFS_RESULT_FAILED;
					break;
					}
				}
			//--- new UNKNOWN
			else
				{
				defs_UpdateStatsCommandToWideChar(pParts);
				Debug_Log(DEBUG_LOG_ERROR,L"Unknown key '%s' for new",pParts[1]);
				defs_UpdateStatsCommandPrint(DEBUG_LOG_ERROR,pParts);
				iResult = DEFS_RESULT_FAILED;
				break;
				}
			}
		else if (!strcmp(pParts[0],"add"))
			{
			// add name "Name","Description"
			if (!strcmp(pParts[1],"name"))
				{
				iResult = defs_SQLQuery(pDB,"INSERT OR IGNORE INTO ItemNames (NameGroup,Name,NameIndex) VALUES(?,?,?)",3,DEFS_SQL_TEXT,pszParentGroup,DEFS_SQL_TEXT,pParts[2],DEFS_SQL_INT32,iIndex0);
				iIndex0++;
				stats.dwEntries++;
				}
			// add namecool "Name","Description"
			else if (!strcmp(pParts[1],"namecool"))
				{
				iResult = defs_SQLQuery(pDB,"UPDATE ItemNames SET NameCool=? WHERE NameGroup=? AND NameIndex=?",3,DEFS_SQL_TEXT,pParts[2],DEFS_SQL_TEXT,pszParentGroup,DEFS_SQL_INT32,iIndex1,DEFS_SQL_INT32,1);
				iIndex1++;
				stats.dwEntries++;
				}
			// add levelgroup MinLevel,MaxLevel,"Quality"
			else if (!strcmp(pParts[1],"levelgroup"))
				{
				if (pszParentGroup)
					{
					group.minLevel = strtol(pParts[2],NULL,10);
					group.maxLevel = strtol(pParts[3],NULL,10);
					group.quality = defs_StrCpyAlloc(pParts[4]);
					}
				}
			// add rootgroup "UUID","Color"
			else if (!strcmp(pParts[1],"rootgroup"))
				{
				if (pszParentGroup)
					group.root = defs_StrCpyAlloc(pParts[2]);
				}
			// add namegroup "NameGroup","Cool","Icon"
			else if (!strcmp(pParts[1],"namegroup"))
				{
				if (pszParentGroup)
					{
					group.name = defs_StrCpyAlloc(pParts[2]);
					group.cool = !defs_IsEmpty(pParts[3]);
					group.icon = defs_StrCpyAlloc(pParts[4]);
					iResult = defs_UpdateStatsTableFlush(pDB,iIndex0,pszParentGroup,DEFS_FLUSH_GROUP,&group);
					iIndex0++;
					stats.dwEntries++;
					}
				}
			}
		else if (!strcmp(pParts[0],"type"))
			{
			if (pszParentEntry)
				entry.Type = defs_StrCpyAlloc(pParts[1]);
			}
		else if (!strcmp(pParts[0],"using"))
			{
			if (pszParentEntry)
				entry.Using = defs_StrCpyAlloc(pParts[1]);
			}
		else if (!strcmp(pParts[0],"data"))
			{
			if (pszParentEntry)
				{
				if (!strcmp(pParts[1],"ItemGroup")) entry.ItemGroup = defs_StrCpyAlloc(pParts[2]);
				else if (!strcmp(pParts[1],"Slot")) entry.Slot = defs_StrCpyAlloc(pParts[2]);
				else if (!strcmp(pParts[1],"InventoryTab")) entry.InventoryTab = defs_StrCpyAlloc(pParts[2]);
				else if (!strcmp(pParts[1],"ArmorType")) entry.ArmorType = defs_StrCpyAlloc(pParts[2]);
				else if (!strcmp(pParts[1],"ItemColor")) entry.ItemColor = defs_StrCpyAlloc(pParts[2]);
				else if (!strcmp(pParts[1],"RuneSlots")) entry.RuneSlots = strtol(pParts[2],NULL,10);
				else if (!strcmp(pParts[1],"RuneSlots_V1")) entry.RuneSlots_V1 = strtol(pParts[2],NULL,10);
				}
			}
		else
			{
			defs_UpdateStatsCommandToWideChar(pParts);
			Debug_Log(DEBUG_LOG_WARNING,L"Skipped command '%s'",pParts[1]);
			defs_UpdateStatsCommandPrint(DEBUG_LOG_WARNING,pParts);
			}
		if (iResult != DEFS_RESULT_OK) break;
		}

	if (iResult == DEFS_RESULT_OK) iResult = defs_UpdateStatsTableFlush(pDB,0,pszParentEntry,DEFS_FLUSH_ENTRY,&entry);
	else defs_UpdateStatsTableFlush(NULL,0,pszParentEntry,DEFS_FLUSH_ENTRY,&entry);
	defs_UpdateStatsTableFlush(NULL,0,NULL,DEFS_FLUSH_GROUP,&group);
	defs_UpdateStatsTableExplode(NULL,&pParts);
	if (pszParentGroup) HeapFree(App.hHeap,0,pszParentGroup);
	if (pszParentEntry) HeapFree(App.hHeap,0,pszParentEntry);

	defs_Stats(&stats,DEFS_STATS_END);
	Status_SetText(NULL);
	xml_UpdateProgress(-1,-1,0);
	return(iResult);
}


// «»»» Sauvegarde d'un groupe ««««««««««««««««««««««««««««««««««««««««««»

int defs_UpdateStatsTableFlush(DEFSQL *pDB, int iIndex, char *pszParent, UINT uSource, void *pStruct)
{
	DOS2ITEMGROUP*	pGroup;
	DOS2ITEMENTRY*	pEntry;
	int		iResult = DEFS_RESULT_OK;

	switch (uSource)
		{
		case DEFS_FLUSH_GROUP:
			pGroup = (DOS2ITEMGROUP *)pStruct;
			if (pDB) iResult = defs_SQLQuery(pDB,"INSERT OR IGNORE INTO ItemGroups (ItemGroup,MinLevel,MaxLevel,Quality,Root,Name,Cool,Icon,LevelGroupIndex) VALUES(?,?,?,?,?,?,?,?,?)",9,DEFS_SQL_TEXT,pszParent,DEFS_SQL_INT32,pGroup->minLevel,DEFS_SQL_INT32,pGroup->maxLevel,DEFS_SQL_NULLTEXT,pGroup->quality,DEFS_SQL_TEXT,pGroup->root,DEFS_SQL_TEXT,pGroup->name,DEFS_SQL_INT32,pGroup->cool,DEFS_SQL_NULLTEXT,pGroup->icon,DEFS_SQL_INT32,iIndex);
			if (pGroup->quality) HeapFree(App.hHeap,0,pGroup->quality);
			if (pGroup->root) HeapFree(App.hHeap,0,pGroup->root);
			if (pGroup->name) HeapFree(App.hHeap,0,pGroup->name);
			if (pGroup->icon) HeapFree(App.hHeap,0,pGroup->icon);
			ZeroMemory(pGroup,sizeof(DOS2ITEMGROUP));
			break;
		case DEFS_FLUSH_ENTRY:
			if (!pszParent) break;
			pEntry = (DOS2ITEMENTRY *)pStruct;
			if (pDB) iResult = defs_SQLQuery(pDB,"INSERT OR IGNORE INTO ItemStats (Entry,Type,Parent,ItemGroup,Slot,InventoryTab,ArmorType,ItemColor,RuneSlots,RuneSlots_V1) VALUES(?,?,?,?,?,?,?,?,?,?)",10,DEFS_SQL_TEXT,pszParent,DEFS_SQL_TEXT,pEntry->Type,DEFS_SQL_NULLTEXT,pEntry->Using,DEFS_SQL_NULLTEXT,pEntry->ItemGroup,DEFS_SQL_NULLTEXT,pEntry->Slot,DEFS_SQL_NULLTEXT,pEntry->InventoryTab,DEFS_SQL_NULLTEXT,pEntry->ArmorType,DEFS_SQL_NULLTEXT,pEntry->ItemColor,DEFS_SQL_INT32,pEntry->RuneSlots,DEFS_SQL_INT32,pEntry->RuneSlots_V1);
			if (pEntry->Type) HeapFree(App.hHeap,0,pEntry->Type);
			if (pEntry->Using) HeapFree(App.hHeap,0,pEntry->Using);
			if (pEntry->ItemGroup) HeapFree(App.hHeap,0,pEntry->ItemGroup);
			if (pEntry->Slot) HeapFree(App.hHeap,0,pEntry->Slot);
			if (pEntry->InventoryTab) HeapFree(App.hHeap,0,pEntry->InventoryTab);
			if (pEntry->ArmorType) HeapFree(App.hHeap,0,pEntry->ArmorType);
			if (pEntry->ItemColor) HeapFree(App.hHeap,0,pEntry->ItemColor);
			ZeroMemory(pEntry,sizeof(DOS2ITEMENTRY));
			break;
		}

	return(iResult);
}


// «»»» Séparation d'une ligne d'arguments ««««««««««««««««««««««««««««««»

char* defs_UpdateStatsTableExplode(char *pszPointer, char ***pParts)
{
	char**	pTable;
	char*	pszPartStart;
	char*	pszPartEnd;
	char*	pszEnd;
	int	i,n;

	if (*pParts)
		{
		for (i = 0; (*pParts)[i] != NULL; i++) HeapFree(App.hHeap,0,(*pParts)[i]);
		HeapFree(App.hHeap,0,*pParts);
		*pParts = NULL;
		}

	if (!pszPointer) return(NULL);
	if (!*pszPointer) return(NULL);

	for (; *pszPointer != 0; pszPointer++)
		{
		if (*pszPointer == 0x0D) break;
		if (*pszPointer == 0x0A) break;
		if (*pszPointer > ' ') break;
		}

	pszEnd = strchr(pszPointer,0x0D);
	if (pszEnd)
		{
		*pszEnd++ = 0;
		if (*pszEnd == 0x0A) *pszEnd++ = 0;
		}
	else
		{
		pszEnd = strchr(pszPointer,0x0A);
		if (pszEnd) *pszEnd++ = 0;
		}

	if (!strlen(pszPointer)) return(pszEnd); // Empty line

	pTable = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(char *)*(strlen(pszPointer)+1));
	if (!pTable) return(NULL);

	pszPartStart = pszPointer;
	i = 0;

	while (*pszPartStart)
		{
		for (; *pszPartStart != 0; pszPartStart++)
			{
			if (*pszPartStart == ' ') continue;
			if (*pszPartStart == ',') continue;
			break;
			}
		if (*pszPartStart == '"')
			{
			pszPartStart++;
			for (pszPartEnd = pszPartStart; *pszPartEnd != 0; pszPartEnd++)
				if (*pszPartEnd == '"') break;
			n = pszPartEnd-pszPartStart;
			if (*pszPartEnd == '"') pszPartEnd++;
			}
		else
			{
			for (pszPartEnd = pszPartStart; *pszPartEnd != 0; pszPartEnd++)
				{
				if (*pszPartEnd == ' ') break;
				if (*pszPartEnd == ',') break;
				}
			n = pszPartEnd-pszPartStart;
			if (*pszPartEnd == ' ') pszPartEnd++;
			else if (*pszPartEnd == ',') pszPartEnd++;
			}

		pTable[i] = HeapAlloc(App.hHeap,0,n+1);
		if (!pTable[i]) return(defs_UpdateStatsTableExplode(NULL,&pTable));
		CopyMemory(pTable[i],pszPartStart,n);
		pTable[i][n] = 0;
		pszPartStart = pszPartEnd;
		i++;
		}

	*pParts = HeapReAlloc(App.hHeap,HEAP_ZERO_MEMORY,pTable,sizeof(char *)*(i+1));
	if (!*pParts) *pParts = pTable;

	return(pszEnd?pszEnd:NULL);
}


// «»»» Affichage (debug) «««««««««««««««««««««««««««««««««««««««««««««««»

//--- Affichage de la commande ---

void defs_UpdateStatsCommandPrint(UINT uLogType, char **pParts)
{
	if (pParts[1])
		{
		if (pParts[2]) Debug_Log(uLogType,L"> %s %s %s",pParts[0],pParts[1],pParts[2]);
		else Debug_Log(uLogType,L"> %s %s",pParts[0],pParts[1]);
		}

	return;
}

//--- Conversion en widechar ---

void defs_UpdateStatsCommandToWideChar(char **pParts)
{
	int	i;

	for (i = 0; pParts[i] != NULL; i++)
		{
		WCHAR *pszTemp = Misc_UTF8ToWideChar(pParts[i]);
		if (pszTemp)
			{
			HeapFree(App.hHeap,0,pParts[i]);
			pParts[i] = (char *)pszTemp;
			}
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	ITEMS: Modèles de base (Root Templates)				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int defs_UpdateRootTemplates(DEFSQL *pDB, DEFSFILE *pFile)
{
	DEFSTATS	Stats;
	DOS2OBJECT	Object;
	sqlite3_stmt*	stmt;
	XML_NODE*	pxnObjects;
	XML_NODE*	pxnAttribute;
	XML_NODE*	pxnTags;
	WCHAR*		pszValue;
	WCHAR*		pszAttrValue;
	int		iResult;
	int		i,j;

	Debug_Log(DEBUG_LOG_INFO,szDebugGameObjects);

	pxnObjects = xml_GetNodeFromPathFirstChild((XML_NODE *)pFile->xmlRoot.next,szGameObjectsPath);
	if (!pxnObjects)
		{
		Debug_Log(DEBUG_LOG_ERROR,szDebugXMLNoPath,szXMLGameObjects);
		return(DEFS_RESULT_FAILED);
		}

	ZeroMemory(&Object,sizeof(DOS2OBJECT));
	defs_ResetObject(&Object);
	sqlite3_exec(pDB->pSql,"BEGIN TRANSACTION",NULL,NULL,NULL);
	defs_Stats(&Stats,DEFS_STATS_BEGIN);

	for (iResult = DEFS_RESULT_OK, i = 0, j = List_EntryCount((NODE *)pxnObjects); pxnObjects != NULL; pxnObjects = (XML_NODE *)pxnObjects->node.next, i++)
		{
		xml_UpdateProgress(i,j,0);
		if ((iResult = defs_ThreadAbort())) break;

		pszValue = xml_GetAttrValue(pxnObjects,szXMLid);
		if (!pszValue) continue;
		if (wcscmp(pszValue,szXMLGameObjects)) continue;

		defs_ResetObject(&Object);

		//--- Récupère les attributs
		for (pxnAttribute = (XML_NODE *)pxnObjects->children.next; pxnAttribute != NULL; pxnAttribute = (XML_NODE *)pxnAttribute->node.next)
			{
			pszValue = xml_GetAttrValue(pxnAttribute,szXMLid);
			if (!pszValue) continue;
			pszAttrValue = xml_GetAttrValue(pxnAttribute,szXMLvalue);
			if (!wcscmp(pszValue,szXMLMapKey)) Object.MapKey = Misc_WideCharToUTF8(pszAttrValue);
			else if (!wcscmp(pszValue,szXMLType)) Object.Type = Misc_WideCharToUTF8(pszAttrValue);
			else if (!wcscmp(pszValue,szXMLName)) { Object.Name = Misc_WideCharToUTF8(pszAttrValue); Object.NameW = pszAttrValue; }
			else if (!wcscmp(pszValue,szXMLStats)) Object.Stats = Misc_WideCharToUTF8(pszAttrValue);
			else if (!wcscmp(pszValue,szXMLDisplayName)) Object.DisplayName = Misc_WideCharToUTF8(xml_GetAttrValue(pxnAttribute,szXMLhandle));
			else if (!wcscmp(pszValue,szXMLDescription)) Object.Description = Misc_WideCharToUTF8(xml_GetAttrValue(pxnAttribute,szXMLhandle));
			else if (!wcscmp(pszValue,szXMLIcon)) Object.Icon = Misc_WideCharToUTF8(pszAttrValue);
			else if (!wcscmp(pszValue,L"maxStackAmount")) Object.maxStackAmount = pszAttrValue?wcstol(pszAttrValue,NULL,10):1;
			}

		//--- Ignores items when important infos are missing
		if (!Object.Name) continue;
		if (!Object.Stats) continue;
		if (!Object.Type) { Debug_Log(DEBUG_LOG_WARNING,szDebugXMLMissing,szXMLType,Object.NameW); continue; }
		if (strcmp(Object.Type,"item")) continue;

		//--- Trigger warnings for missing infos
		if (!Object.MapKey) Debug_Log(DEBUG_LOG_WARNING,szDebugXMLMissing,szXMLMapKey,Object.NameW);
		if (!Object.DisplayName) Debug_Log(DEBUG_LOG_WARNING,szDebugXMLMissing,szXMLDisplayName,Object.NameW);
		if (!Object.Description) Debug_Log(DEBUG_LOG_WARNING,szDebugXMLMissing,szXMLDescription,Object.NameW);
		if (!Object.Icon) Debug_Log(DEBUG_LOG_WARNING,szDebugXMLMissing,szXMLIcon,Object.NameW);

		//--- Ignores objects with no MapKey
		if (!Object.MapKey)
			{
			Debug_Log(DEBUG_LOG_WARNING,szDebugXMLIgnored,Object.NameW);
			continue;
			}

		//--- GM SubSection
		pszValue = xml_GetAttrValue(xml_GetNodeFromPath((XML_NODE *)pxnObjects->children.next,szGMSpawnSubSectionPath),szXMLhandle);
		if (pszValue) Object.SubSection = Misc_WideCharToUTF8(pszValue);

		//--- ORGANIZE_ tag
		pxnTags = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnObjects->children.next,szTagsPath);
		for (; pxnTags != NULL; pxnTags = (XML_NODE *)pxnTags->node.next)
			{
			pszValue = xml_GetAttrValue(pxnTags,szXMLid);
			if (!pszValue) continue;
			if (wcscmp(pszValue,L"Tag")) continue;
			pszValue = xml_GetAttrValue((XML_NODE *)pxnTags->children.next,szXMLvalue); // We assume there is only one attribute[id="Object"] in the node[id="Tag"]
			if (!pszValue) continue;
			if (wcsncmp(pszValue,szXMLOrganize,wcslen(szXMLOrganize))) continue;
			Object.Tags = Misc_WideCharToUTF8(pszValue);
			break;
			}
		if (Object.maxStackAmount == 0) Object.maxStackAmount = 1;

		//--- SQL update
		if (sqlite3_prepare_v2(pDB->pSql,"INSERT OR IGNORE INTO RootObjects (MapKey,Name,Stats,DisplayName,Description,Icon,maxStackAmount,SubSection,Tags) VALUES(?,?,?,?,?,?,?,?,?)",-1,&stmt,NULL) == SQLITE_OK)
			{
			defs_BindText(stmt,1,Object.MapKey);
			defs_BindText(stmt,2,Object.Name);
			defs_BindText(stmt,3,Object.Stats);
			defs_BindText(stmt,4,Object.DisplayName);
			defs_BindText(stmt,5,Object.Description);
			defs_BindText(stmt,6,Object.Icon);
			sqlite3_bind_int(stmt,7,Object.maxStackAmount);
			defs_BindText(stmt,8,Object.SubSection);
			defs_BindText(stmt,9,Object.Tags);
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			Stats.dwEntries++;
			}
		else
			{
			defs_SQLErrorMsg(pDB->pSql);
			iResult = DEFS_RESULT_FAILED;
			break;
			}
		}

	defs_Stats(&Stats,DEFS_STATS_END);
	sqlite3_exec(pDB->pSql,"END TRANSACTION",NULL,NULL,NULL);
	defs_ResetObject(&Object);
	xml_UpdateProgress(-1,-1,0);
	return(iResult);
}


// «»»» Ajoute un texte dans la base de données «««««««««««««««««««««««««»

// text may be NULL

void defs_BindText(sqlite3_stmt *stmt, int key, char *text)
{
	if (!text) sqlite3_bind_null(stmt,key);
	else sqlite3_bind_text(stmt,key,text,strlen(text),NULL);
	return;
}


// «»»» Réinitialise d'un object ««««««««««««««««««««««««««««««««««««««««»

void defs_ResetObject(DOS2OBJECT *pObject)
{
	if (pObject->MapKey) HeapFree(App.hHeap,0,pObject->MapKey);
	if (pObject->Type) HeapFree(App.hHeap,0,pObject->Type);
	if (pObject->Name) HeapFree(App.hHeap,0,pObject->Name);
	if (pObject->Stats) HeapFree(App.hHeap,0,pObject->Stats);
	if (pObject->DisplayName) HeapFree(App.hHeap,0,pObject->DisplayName);
	if (pObject->Description) HeapFree(App.hHeap,0,pObject->Description);
	if (pObject->Icon) HeapFree(App.hHeap,0,pObject->Icon);
	if (pObject->SubSection) HeapFree(App.hHeap,0,pObject->SubSection);
	if (pObject->Tags) HeapFree(App.hHeap,0,pObject->Tags);

	ZeroMemory(pObject,sizeof(DOS2OBJECT));
	pObject->maxStackAmount = 1;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	ICONS: Atlas de textures					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mise-à-jour des icônes ««««««««««««««««««««««««««««««««««««««««««»

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
int defs_UpdateIconsUV(DEFSQL *pDB, DEFSFILE *pFile)
{
	sqlite3_stmt*	stmt;
	DEFSTATS	stats;
	XML_NODE*	pxnIconUV;
	WCHAR*		pszValue;
	WCHAR*		pszMapKey;
	DWORD		dwFound;
	float		fU1,fU2,fV1,fV2;
	char*		pszUUID;
	int		iResult;
	int		i,j;

	Debug_Log(DEBUG_LOG_INFO,szDebugIconUV);

	pxnIconUV = xml_GetNodeFromPathFirstChild((XML_NODE *)pFile->xmlRoot.next,szIconUVListPath);
	if (!pxnIconUV)
		{
		Debug_Log(DEBUG_LOG_ERROR,szDebugXMLNoPath,szXMLIconUVList);
		return(DEFS_RESULT_FAILED);
		}

	pszUUID = Misc_WideCharToUTF8(pFile->atlas.Path.pszUUID);
	if (!pszUUID)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}

	sqlite3_exec(pDB->pSql,"BEGIN TRANSACTION",NULL,NULL,NULL);
	defs_Stats(&stats,DEFS_STATS_BEGIN);

	for (iResult = DEFS_RESULT_OK, i = 0, j = List_EntryCount((NODE *)pxnIconUV); pxnIconUV != NULL; pxnIconUV = (XML_NODE *)pxnIconUV->node.next, i++)
		{
		xml_UpdateProgress(i,j,0);
		if ((iResult = defs_ThreadAbort())) break;
		dwFound = 0;
		pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnIconUV->children.next,szXMLattribute,szXMLid,szXMLMapKey));
		if (pszValue) { dwFound |= 0x00000001; pszMapKey = pszValue; }
		pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnIconUV->children.next,szXMLattribute,szXMLid,szXMLU1));
		if (pszValue) { dwFound |= 0x00000002; fU1 = wcstof(pszValue,NULL); }
		pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnIconUV->children.next,szXMLattribute,szXMLid,szXMLU2));
		if (pszValue) { dwFound |= 0x00000004; fU2 = wcstof(pszValue,NULL); }
		pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnIconUV->children.next,szXMLattribute,szXMLid,szXMLV1));
		if (pszValue) { dwFound |= 0x00000008; fV1 = wcstof(pszValue,NULL); }
		pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnIconUV->children.next,szXMLattribute,szXMLid,szXMLV2));
		if (pszValue) { dwFound |= 0x00000010; fV2 = wcstof(pszValue,NULL); }

		if (!(dwFound&0x00000001)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLMapKey,szXMLIconUV);
		if (!(dwFound&0x00000002)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLU1,szXMLIconUV);
		if (!(dwFound&0x00000004)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLU2,szXMLIconUV);
		if (!(dwFound&0x00000008)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLV1,szXMLIconUV);
		if (!(dwFound&0x00000010)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLV2,szXMLIconUV);

		if (dwFound != 0x000001F) continue;

		if (sqlite3_prepare_v2(pDB->pSql,"INSERT OR IGNORE INTO IconUVList (MapKey,U1,U2,V1,V2,UUID) VALUES(?,?,?,?,?,?)",-1,&stmt,NULL) == SQLITE_OK)
			{
			char* pszTemp = Misc_WideCharToUTF8(pszMapKey);
			if (pszTemp)
				{
				sqlite3_bind_text(stmt,1,pszTemp,strlen(pszTemp),NULL);
				sqlite3_bind_double(stmt,2,(double)fU1);
				sqlite3_bind_double(stmt,3,(double)fU2);
				sqlite3_bind_double(stmt,4,(double)fV1);
				sqlite3_bind_double(stmt,5,(double)fV2);
				sqlite3_bind_text(stmt,6,pszUUID,strlen(pszUUID),NULL);
				sqlite3_step(stmt);
				}
			sqlite3_finalize(stmt);
			stats.dwEntries++;
			}
		else
			{
			defs_SQLErrorMsg(pDB->pSql);
			iResult = DEFS_RESULT_FAILED;
			break;
			}
		}

	defs_Stats(&stats,DEFS_STATS_END);
	sqlite3_exec(pDB->pSql,"END TRANSACTION",NULL,NULL,NULL);
	xml_UpdateProgress(-1,-1,0);
	HeapFree(App.hHeap,0,pszUUID);
	return(iResult);
}
#pragma GCC diagnostic pop


// «»»» Mise-à-jour des atlas «««««««««««««««««««««««««««««««««««««««««««»

int defs_UpdateTextureAtlas(DEFSQL *pDB, DEFSFILE *pFile)
{
	sqlite3_stmt*	stmt;
	XML_NODE*	pxnAtlas;
	WCHAR*		pszId;
	DWORD		dwFound;
	int		iResult;

	Debug_Log(DEBUG_LOG_INFO,szDebugAtlas);

	pxnAtlas = xml_GetNodeFromPathFirstChild((XML_NODE *)&pFile->xmlRoot.next,szTextureAtlasInfoPath);
	if (!pxnAtlas)
		{
		Debug_Log(DEBUG_LOG_ERROR,szDebugXMLNoPath,szXMLTextureAtlasInfo);
		return(DEFS_RESULT_FAILED);
		}

	for (dwFound = 0; pxnAtlas != NULL; pxnAtlas = (XML_NODE *)pxnAtlas->node.next)
		{
		pszId = xml_GetAttrValue(pxnAtlas,szXMLid);
		if (!pszId) continue;
		if (!wcscmp(pszId,szXMLTextureAtlasIconSize))
			{
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLHeight));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLHeight,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.IconSize.iWidth = wcstol(pszId,NULL,10);
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLWidth));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLWidth,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.IconSize.iHeight = wcstol(pszId,NULL,10);
			dwFound |= 0x00000001;
			continue;
			}
		else if (!wcscmp(pszId,szXMLTextureAtlasPath))
			{
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLPath));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLPath,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.Path.pszPath = pszId;
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLUUID));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLUUID,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.Path.pszUUID = pszId;
			dwFound |= 0x00000002;
			continue;
			}
		else if (!wcscmp(pszId,szXMLTextureAtlasTextureSize))
			{
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLHeight));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLHeight,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.TextureSize.iWidth = wcstol(pszId,NULL,10);
			pszId = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnAtlas->children.next,szXMLattribute,szXMLid,szXMLWidth));
			if (!pszId) { Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLWidth,szXMLTextureAtlasIconSize); break; }
			pFile->atlas.TextureSize.iHeight = wcstol(pszId,NULL,10);
			dwFound |= 0x00000004;
			continue;
			}
		}

	if (!(dwFound&0x00000001)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLTextureAtlasIconSize,szXMLTextureAtlasInfo);
	if (!(dwFound&0x00000002)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLTextureAtlasPath,szXMLTextureAtlasInfo);
	if (!(dwFound&0x00000004)) Debug_Log(DEBUG_LOG_ERROR,szDebugXMLMissing,szXMLTextureAtlasTextureSize,szXMLTextureAtlasInfo);
	if (dwFound != 0x00000007) return(DEFS_RESULT_FAILED);

	iResult = DEFS_RESULT_FAILED;
	if (sqlite3_prepare_v2(pDB->pSql,"INSERT OR IGNORE INTO TextureAtlasInfo (UUID,'IconSize.Width','IconSize.Height','TextureSize.Width','TextureSize.Height','File') VALUES(?,?,?,?,?,?)",-1,&stmt,NULL) == SQLITE_OK)
		{
		char*	pszTemp1 = Misc_WideCharToUTF8(pFile->atlas.Path.pszUUID);
		char*	pszTemp2 = Misc_WideCharToUTF8(PathFindFileName(pFile->atlas.Path.pszPath));
		char*	pszTemp3;
		if (pszTemp1 && pszTemp2)
			{
			pszTemp3 = strrchr(pszTemp2,'.');
			if (pszTemp3) *pszTemp3 = 0;
			sqlite3_bind_text(stmt,1,pszTemp1,strlen(pszTemp1),NULL);
			sqlite3_bind_int(stmt,2,pFile->atlas.IconSize.iWidth);
			sqlite3_bind_int(stmt,3,pFile->atlas.IconSize.iHeight);
			sqlite3_bind_int(stmt,4,pFile->atlas.TextureSize.iWidth);
			sqlite3_bind_int(stmt,5,pFile->atlas.TextureSize.iHeight);
			sqlite3_bind_text(stmt,6,pszTemp2,strlen(pszTemp2),NULL);
			sqlite3_step(stmt);
			}
		if (pszTemp1) HeapFree(App.hHeap,0,pszTemp1);
		if (pszTemp2) HeapFree(App.hHeap,0,pszTemp2);
		iResult = DEFS_RESULT_OK;
		defs_Stats(NULL,DEFS_STATS_SINGLE);
		}
	if (iResult == DEFS_RESULT_FAILED) defs_SQLErrorMsg(pDB->pSql);
	sqlite3_finalize(stmt);

	return(iResult);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Fichiers							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'une liste de fichiers ««««««««««««««««««««««««««««««««»

int defs_CreateFileList(WCHAR *pszPath, DEFSCONTEXT *pDefs)
{
	WIN32_FIND_DATA		Find;
	DEFSFILE*		pFile;
	HANDLE			hFile;
	WCHAR*			pszSearch;

	Debug_Log(DEBUG_LOG_INFO,szDebugCreateFileList,pszPath);

	pszSearch = HeapAlloc(App.hHeap,0,wcslen(pszPath)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(szWild)*sizeof(WCHAR)+sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszSearch)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}

	//--- Récupère la liste des dossiers dans le répertoire ---

	wcscpy(pszSearch,pszPath);
	PathAppend(pszSearch,szWild);
	hFile = FindFirstFile(pszSearch,&Find);
	HeapFree(App.hHeap,0,pszSearch);
	if (hFile == INVALID_HANDLE_VALUE)
		{
		Debug_LogSystemError();
		return(DEFS_RESULT_FAILED);
		}

	do {

		if (!wcscmp(Find.cFileName,szRootPath)) continue;
		if (!wcscmp(Find.cFileName,szParentPath)) continue;

		if (Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			WCHAR*	pszInnerPath;
			int	iResult;

			pszInnerPath = HeapAlloc(App.hHeap,0,wcslen(pszPath)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(Find.cFileName)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pszInnerPath)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				Debug_LogSystemError();
				FindClose(hFile);
				return(DEFS_RESULT_FAILED);
				}
			wcscpy(pszInnerPath,pszPath);
			PathAppend(pszInnerPath,Find.cFileName);
			iResult = defs_CreateFileList(pszInnerPath,pDefs);
			HeapFree(App.hHeap,0,pszInnerPath);
			if (iResult != DEFS_RESULT_OK)
				{
				FindClose(hFile);
				return(iResult);
				}
			continue;
			}

		if (!defs_ExtIsKnown(Find.cFileName,DEFS_EXT_ALL)) continue;

		pFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DEFSFILE));
		if (!pFile)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Debug_LogSystemError();
			FindClose(hFile);
			return(DEFS_RESULT_FAILED);
			}
		List_AddEntry((NODE *)pFile,&pDefs->files);

		pFile->pszPath = HeapAlloc(App.hHeap,0,wcslen(pszPath)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(Find.cFileName)*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pFile->pszPath)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Debug_LogSystemError();
			FindClose(hFile);
			return(DEFS_RESULT_FAILED);
			}
		wcscpy(pFile->pszPath,pszPath);
		PathAppend(pFile->pszPath,Find.cFileName);

		Debug_Log(DEBUG_LOG_INFO,szDebugAdded,pFile->pszPath);

	} while (FindNextFile(hFile,&Find));

	FindClose(hFile);
	return(DEFS_RESULT_OK);
}


// «»»» Chargement d'un fichier «««««««««««««««««««««««««««««««««««««««««»

int defs_LoadData(DEFSFILE *pFile)
{
	DWORD	dwLastError;
	int	iResult;

	Debug_Log(DEBUG_LOG_INFO,szDebugLoading,pFile->pszPath);

	iResult = DEFS_RESULT_FAILED;
	dwLastError = 0;
	//--- XML/LZX/TXT file
	if (defs_ExtIsKnown(pFile->pszPath,DEFS_EXT_UNCOMPRESSED))
		{
		HANDLE	hFile;
		DWORD	dwSize;
		DWORD	dwBytes;

		Debug_Log(DEBUG_LOG_INFO,szDebugFileLSX);
		hFile = CreateFile(pFile->pszPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			{
			dwSize = GetFileSize(hFile,NULL);
			if (dwSize != INVALID_FILE_SIZE)
				{
				Debug_Log(DEBUG_LOG_INFO,szDebugFileSize,dwSize);
				pFile->pData = HeapAlloc(App.hHeap,0,dwSize+1);
				if (pFile->pData)
					{
					if (!ReadFile(hFile,pFile->pData,dwSize,&dwBytes,NULL)) dwLastError = GetLastError();
					else if (dwSize != dwBytes) dwLastError = GetLastError();
					else { pFile->iSize = dwSize; pFile->pData[dwSize] = 0; iResult = DEFS_RESULT_OK; }
					}
				else dwLastError = ERROR_NOT_ENOUGH_MEMORY;
				CloseHandle(hFile);
				}
			else dwLastError = GetLastError();
			}
		}
	//--- GZIP file
	else if (defs_ExtIsKnown(pFile->pszPath,DEFS_EXT_COMPRESSED))
		{
		Debug_Log(DEBUG_LOG_INFO,szDebugFileGZ);
		HANDLE hFile = CreateFile(pFile->pszPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			{
			if (SetFilePointer(hFile,-4,NULL,FILE_END) != INVALID_SET_FILE_POINTER)
				{
				gzFile	gzFile;
				char*	pszFileName;
				int	iBufferSize;
				int	iBytes;

				if (ReadFile(hFile,&iBufferSize,4,(DWORD *)&iBytes,NULL) && iBytes == 4)
					{
					Debug_Log(DEBUG_LOG_INFO,szDebugFileSizes,GetFileSize(hFile,NULL),iBufferSize);
					CloseHandle(hFile);
					hFile = INVALID_HANDLE_VALUE;
					pszFileName = Misc_WideCharToUTF8(pFile->pszPath);
					if (pszFileName)
						{
						gzFile = gzopen(pszFileName,"rb");
						if (gzFile)
							{
							pFile->pData = HeapAlloc(App.hHeap,0,iBufferSize+1);
							if (pFile->pData)
								{
								iBytes = gzread(gzFile,pFile->pData,iBufferSize);
								if (iBytes != iBufferSize)
									{
									WCHAR*		pszError;
									const char*	pszErrorString;
									int		iError;

									pszErrorString = gzerror(gzFile,&iError);
									pszError = Misc_UTF8ToWideChar(pszErrorString);
									if (pszError)
										{
										Debug_Log(DEBUG_LOG_ERROR,szDebugZLIB,pszError);
										HeapFree(App.hHeap,0,pszError);
										dwLastError = ERROR_INVALID_DATA;
										}
									}
								else { pFile->iSize = iBufferSize; pFile->pData[iBufferSize] = 0; iResult = DEFS_RESULT_OK; }
								}
							else dwLastError = ERROR_NOT_ENOUGH_MEMORY;
							gzclose(gzFile);
							}
						else dwLastError = GetLastError();
						HeapFree(App.hHeap,0,pszFileName);
						}
					else dwLastError = ERROR_NOT_ENOUGH_MEMORY;
					}
				}
				else dwLastError = GetLastError();
			if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
			}
		else dwLastError = GetLastError();
		}
	//--- Unknown file
	else iResult = DEFS_RESULT_FAILED;

	//--- Done ---

	if (iResult == DEFS_RESULT_OK) Debug_Log(DEBUG_LOG_INFO,szDebugFileLoaded);
	else Debug_Log(DEBUG_LOG_ERROR,szDebugFileNotLoaded);

	if (dwLastError)
		{
		SetLastError(dwLastError);
		Debug_LogSystemError();
		}

	return(iResult);
}


// «»»» Conversion du fichier LSX/XML «««««««««««««««««««««««««««««««««««»

int defs_LoadXML(DEFSFILE *pFile)
{
	int	iResult;

	Debug_Log(DEBUG_LOG_INFO,szDebugXML);
	Status_SetText(Locale_GetText(TEXT_LOADING),pFile->pszPath);
	xml_UpdateProgress(-1,-1,0);
	iResult = xml_LoadFile(NULL,(WCHAR *)pFile->pData,&pFile->xmlRoot,XML_FLAG_HASHEADER|XML_FLAG_FROMMEMORY|XML_FLAG_NOSTATUSMSG|XML_FLAG_DEBUGMSG|XML_FLAG_UTF8MEMORY);
	Status_SetText(NULL);
	pFile->pData = NULL;
	return(iResult?DEFS_RESULT_OK:DEFS_RESULT_FAILED);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Vérifie si une chaîne est vide ««««««««««««««««««««««««««««««««««»

int defs_IsEmpty(char *pszText)
{
	if (!pszText) return(1);
	if (!*pszText) return(1);
	return(0);
}


// «»»» Copie une chaîne de caractères (Version ANSI) «««««««««««««««««««»

char* defs_StrCpyAlloc(char *src)
{
	char*	res;

	if (!src) return(NULL);
	res = HeapAlloc(App.hHeap,0,strlen(src)*sizeof(char)+sizeof(char));
	if (!res) return(NULL);
	strcpy(res,src);
	return(res);
}


// «»»» Conversion d'un texte XML (Version ANSI) ««««««««««««««««««««««««»

UINT defs_HtmlSpecialCharsDecode(char *pszCopy, char *pszText)
{
	UINT		i,j,k,l;
	UINT		uLen;

	if (!pszText) return(0);

	for (i = 0, j = 0, uLen = 0; pszText[i] != 0; i++)
		{
		if (pszText[i] == '&')
			{
			for (k = 0; htmlEntitiesA[k] != NULL; k += 2)
				{
				l = strlen(htmlEntitiesA[k]);
				if (!strncmp(htmlEntitiesA[k],&pszText[i],l))
					{
					i += l-1;
					l = strlen(htmlEntitiesA[k+1]);
					uLen += l;
					if (pszCopy)
						{
						CopyMemory(&pszCopy[j],htmlEntitiesA[k+1],l*sizeof(char));
						j += l;
						}
					break;
					}
				}
			}
		else
			{
			if (pszCopy) pszCopy[j++] = pszText[i];
			uLen++;
			}
		}
	if (pszCopy) pszCopy[j] = '\0';

	return(uLen);
}


// «»»» Vérifie si une extension est connue «««««««««««««««««««««««««««««»

int defs_ExtIsKnown(WCHAR *pszPath, DWORD dwFlags)
{
	if (pszPath)
		{
		int n = wcslen(pszPath);

		if (n >= 4 && dwFlags & DEFS_EXT_UNCOMPRESSED)
			{
			if (!wcsncmp(&pszPath[n-4],L".xml",4)) return(1);
			if (!wcsncmp(&pszPath[n-4],L".lsx",4)) return(1);
			if (!wcsncmp(&pszPath[n-4],L".txt",4)) return(1);
			}
		if (n >= 7 && dwFlags & DEFS_EXT_COMPRESSED)
			{
			if (!wcsncmp(&pszPath[n-7],L".xml.gz",7)) return(1);
			if (!wcsncmp(&pszPath[n-7],L".lsx.gz",7)) return(1);
			if (!wcsncmp(&pszPath[n-7],L".txt.gz",7)) return(1);
			}
		}

	return(0);
}


// «»»» Création d'un chemin pour un fichier ««««««««««««««««««««««««««««»

WCHAR* defs_GetFilePath(WCHAR *pszBasePath, WCHAR *pszFile)
{
	WCHAR*	pszPath;

	pszPath = HeapAlloc(App.hHeap,0,wcslen(pszBasePath)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(pszFile)*sizeof(WCHAR)+wcslen(szGZext)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszPath)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Debug_LogSystemError();
		return(NULL);
		}
	wcscpy(pszPath,pszBasePath);
	PathAppend(pszPath,pszFile);

	if (PathFileExists(pszPath))
		{
		wcscpy(pszPath,pszFile);
		return(pszPath);
		}

	wcscat(pszPath,szGZext);
	if (PathFileExists(pszPath))
		{
		wcscpy(pszPath,pszFile);
		wcscat(pszPath,szGZext);
		return(pszPath);
		}

	HeapFree(App.hHeap,0,pszPath);
	Debug_Log(DEBUG_LOG_ERROR,szDebugFileNotFound,pszFile);
	return(NULL);
}


// «»»» Vide entièrement une base de données ««««««««««««««««««««««««««««»

void defs_SQLWipeTables(DEFSQL *pDB, BOOL bWipe, int iTables, ...)
{
	va_list	vl;

	if (!bWipe) return;

	va_start(vl,iTables);
	while (iTables--)
		{
		char *table = va_arg(vl,char *);
		char *exec = HeapAlloc(App.hHeap,0,strlen(szDeleteFromQuery)+strlen(table)+1);
		if (exec)
			{
			strcpy(exec,szDeleteFromQuery);
			strcat(exec,table);
			sqlite3_exec(pDB->pSql,exec,NULL,NULL,NULL);
			HeapFree(App.hHeap,0,exec);
			}
		}
	va_end(vl);

	return;
}


// «»»» Requête SQL «««««««««««««««««««««««««««««««««««««««««««««««««««««»

int defs_SQLQuery(DEFSQL *pDB, char *pszQuery, int iNumArgs, ...)
{
	sqlite3_stmt*	stmt;
	va_list		vl;
	UINT		uType;
	char*		pszVal;
	int		i;

	if (sqlite3_prepare_v2(pDB->pSql,pszQuery,-1,&stmt,NULL) != SQLITE_OK)
		{
		defs_SQLErrorMsg(pDB->pSql);
		return(DEFS_RESULT_FAILED);
		}

	va_start(vl,iNumArgs);
	i = 1;

	while (iNumArgs--)
		{
		uType = va_arg(vl,int);
		switch(uType)
			{
			case DEFS_SQL_TEXT:
				pszVal = va_arg(vl,char *);
				sqlite3_bind_text(stmt,i,pszVal,strlen(pszVal),NULL);
				i++;
				break;
			case DEFS_SQL_NULLTEXT:
				pszVal = va_arg(vl,char *);
				if (!pszVal || !*pszVal) sqlite3_bind_null(stmt,i);
				else sqlite3_bind_text(stmt,i,pszVal,strlen(pszVal),NULL);
				i++;
				break;
			case DEFS_SQL_INT32:
				sqlite3_bind_int(stmt,i,va_arg(vl,int));
				i++;
				break;
			}
		}

	va_end(vl);

	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	return(DEFS_RESULT_OK);
}


// «»»» Erreurs SQLite3 «««««««««««««««««««««««««««««««««««««««««««««««««»

void defs_SQLErrorMsg(sqlite3 *db)
{
	WCHAR *pszErrMsg = Misc_UTF8ToWideChar(sqlite3_errmsg(db));
	if (pszErrMsg)
		{
		Debug_Log(DEBUG_LOG_ERROR,szDebugSQL,pszErrMsg);
		HeapFree(App.hHeap,0,pszErrMsg);
		}
	return;
}


// «»»» Messages ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

//--- Création des messages pour la tâche ---

void defs_CreateMsgQueue()
{
	MSG	msg;

	PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	return;
}

//--- Vérifie si la tâche doit abandonner ---

int defs_ThreadAbort()
{
	MSG	msg;

	return(PeekMessage(&msg,NULL,WM_THREADABORT,WM_THREADABORT,PM_REMOVE)?DEFS_RESULT_ABORTED:DEFS_RESULT_OK);
}

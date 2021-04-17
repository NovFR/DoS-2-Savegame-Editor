
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des objets du jeu
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Game.h"
#include "Definitions.h"
#include "Locale.h"
#include "Requests.h"
#include "Taskbar.h"
#include "Utils.h"
#include "Debug.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie une information dans le presse-papier «««««««««««««««««««««»

void Game_ItemsCopyToClipboard(void *pItem, UINT uMenuId)
{
	WCHAR*	pszText = NULL;

	switch (uMenuId)
		{
		case IDM_COPYDISPLAYNAME:
			pszText = ((DOS2ITEM *)pItem)->pszDisplayName;
			break;
		case IDM_COPYSTATSNAME:
			pszText = xml_GetThisAttrValue(((DOS2ITEM *)pItem)->pxaStats);
			break;
		case IDM_COPYTEMPLATENAME:
			break;
		default:return;
		}

	if (pszText && OpenClipboard(NULL))
		{
		HGLOBAL hHandle = GlobalAlloc(GMEM_MOVEABLE,wcslen(pszText)*sizeof(WCHAR)+sizeof(WCHAR));
		if (hHandle)
			{
			WCHAR *pszClipText = GlobalLock(hHandle);
			if (pszClipText)
				{
				wcscpy(pszClipText,pszText);
				GlobalUnlock(pszClipText);
				EmptyClipboard();
				SetClipboardData(CF_UNICODETEXT,hHandle);
				}
			else GlobalFree(hHandle);
			}
		CloseClipboard();
		}
	return;
}


// «»»» Retrouve le nom réel d'un objet «««««««««««««««««««««««««««««««««»

WCHAR* Game_ItemsGetDisplayName(DOS2ITEM *pItem)
{
	if (App.Config.bItemsResolve && App.Game.pStats && App.Game.pTemplates && App.Game.pLocalization && App.pLanguage)
		{
		sqlite3_stmt*		stmt;
		char*			pszStats;
		char*			pszHandle;
		char*			pszName;
		char*			pszItemGroup;
		WCHAR*			pszDisplayName;
		WCHAR*			pszValue;
		UINT			uLevelGroupIndex;
		UINT			uNameIndex;

		pszName = NULL;
		pszHandle = NULL;
		pszItemGroup = NULL;
		pszDisplayName = NULL;
		pszStats = Misc_WideCharToUTF8(xml_GetThisAttrValue(pItem->pxaStats));
		if (!pszStats) goto Done;

		pszValue = xml_GetThisAttrValue(pItem->pxaLevelGroupIndex);
		if (!pszValue) goto Stats;
		uLevelGroupIndex = wcstoul(pszValue,NULL,10);
		pszValue = xml_GetThisAttrValue(pItem->pxaNameIndex);
		if (!pszValue) goto Stats;
		uNameIndex = wcstoul(pszValue,NULL,10);

		//--- GET ItemGroup FROM Stats (Savegame)
		//
		// Recurse into parents until ItemGroup is NOT NULL (not sure it's what the game is doing)
		// Abort if error occurs or no result.
		// Maybe it is better to check for SQLITE_DONE in every sqlite3_step() and goto Stats; in that case.

		if (sqlite3_prepare_v2(App.Game.pStats,"WITH cte AS (SELECT Entry,Parent,ItemGroup,1 as [Level] FROM ItemStats WHERE Entry = ? UNION ALL SELECT T.Entry,T.Parent,T.ItemGroup,[Level]+1 FROM cte INNER JOIN ItemStats T ON T.Entry = cte.Parent) SELECT C.ItemGroup FROM cte C WHERE C.ItemGroup IS NOT NULL ORDER BY [Level] LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
		sqlite3_bind_text(stmt,1,pszStats,strlen(pszStats),NULL);
		if (sqlite3_step(stmt) != SQLITE_ROW) { sqlite3_finalize(stmt); goto Done; }
		pszItemGroup = defs_StrCpyAlloc((char *)sqlite3_column_text(stmt,0));
		sqlite3_finalize(stmt);

		if (pszItemGroup)
			{
			//--- GET NameGroup FROM ItemGroup (DB) AND LevelGroupIndex (Savegame) (OR LevelGroupIndex 0 if out of range)
			//--- GET Name FROM NameGroup (DB) AND NameIndex (Savegame) (OR NameIndex 0 if out of range)
			if (sqlite3_prepare_v2(App.Game.pStats,"SELECT ITN.Name FROM ItemNames AS ITN WHERE ITN.NameGroup = (SELECT IGR.Name FROM ItemGroups AS IGR WHERE IGR.ItemGroup = ? AND (IGR.LevelGroupIndex = ? OR IGR.LevelGroupIndex = 0) ORDER BY IGR.LevelGroupIndex DESC LIMIT 1) AND (ITN.NameIndex = ? OR ITN.NameIndex = 0) ORDER BY ITN.NameIndex DESC LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
			sqlite3_bind_text(stmt,1,pszItemGroup,strlen(pszItemGroup),NULL);
			sqlite3_bind_int(stmt,2,uLevelGroupIndex);
			sqlite3_bind_int(stmt,3,uNameIndex);
			if (sqlite3_step(stmt) != SQLITE_ROW) { sqlite3_finalize(stmt); goto Done; }
			pszName = defs_StrCpyAlloc((char *)sqlite3_column_text(stmt,0));
			sqlite3_finalize(stmt);
			}

		//--- GET handle FROM Name (DB)
		if (pszName)
			{
			if (sqlite3_prepare_v2(App.Game.pLocalization,"SELECT handle FROM TranslatedStringKeys WHERE UUID=? LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
			sqlite3_bind_text(stmt,1,pszName,strlen(pszName),NULL);
			if (sqlite3_step(stmt) != SQLITE_ROW) { sqlite3_finalize(stmt); goto Done; }
			pszHandle = defs_StrCpyAlloc((char *)sqlite3_column_text(stmt,0));
			sqlite3_finalize(stmt);
			}
		//--- GET handle FROM Stats (Savegame)
		else
			{
Stats:			if (sqlite3_prepare_v2(App.Game.pLocalization,"SELECT handle FROM TranslatedStringKeys WHERE UUID=? LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
			sqlite3_bind_text(stmt,1,pszStats,strlen(pszStats),NULL);
			if (sqlite3_step(stmt) != SQLITE_ROW) { sqlite3_finalize(stmt); goto Done; }
			pszHandle = defs_StrCpyAlloc((char *)sqlite3_column_text(stmt,0));
			sqlite3_finalize(stmt);
			}

		//--- GET DisplayName FROM handle (DB)
		if (pszHandle)
			{
			if (sqlite3_prepare_v2(App.pLanguage,"SELECT text FROM contentList WHERE contentuid=? LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
			sqlite3_bind_text(stmt,1,pszHandle,strlen(pszHandle),NULL);
			if (sqlite3_step(stmt) != SQLITE_ROW) { sqlite3_finalize(stmt); goto Done; }
			pszDisplayName = Misc_UTF8ToWideChar((char *)sqlite3_column_text(stmt,0));
			sqlite3_finalize(stmt);
			}

Done:		if (pszDisplayName && !wcslen(pszDisplayName))
			{
			HeapFree(App.hHeap,0,pszDisplayName);
			pszDisplayName = NULL;
			}

		if (pszHandle) HeapFree(App.hHeap,0,pszHandle);
		if (pszName) HeapFree(App.hHeap,0,pszName);
		if (pszItemGroup) HeapFree(App.hHeap,0,pszItemGroup);
		if (pszStats) HeapFree(App.hHeap,0,pszStats);
		return(pszDisplayName);
		}

	return(NULL);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Localization							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la base de données ««««««««««««««««««««««««««««««««««»

void Game_ItemsLocalizationRebuild(HWND hWnd)
{
	defs_Rebuild(hWnd,IDM_DB_REBUILDITEMSSTATS,FALSE,szDefsItemsLocalizationPath,(LPTHREAD_START_ROUTINE)Game_ItemsLocalizationRebuildThread);
	return;
}


// «»»» Tâche de création «««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Game_ItemsLocalizationRebuildThread(DEFSCONTEXT *pDefs)
{
	defs_CreateMsgQueue();
	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildStarts);

	pDefs->db.uId = sqlite3_open_v2(szItemsLocalizationDataBasePath,&pDefs->db.pSql,SQLITE_OPEN_READWRITE,NULL);
	if (pDefs->db.uId == SQLITE_OK)
		{
		sqlite3_exec(pDefs->db.pSql,"PRAGMA journal_mode = MEMORY",NULL,NULL,NULL);
		defs_SQLWipeTables(&pDefs->db,pDefs->bWipe,1,"TranslatedStringKeys");
		if ((pDefs->iResult = defs_CreateFileList(szDefsItemsLocalizationPath,pDefs)) == DEFS_RESULT_OK)
			{
			DEFSFILE*	pFile;
			float		i,iMax;

			i = 0.0f;
			iMax = (float)List_EntryCount(&pDefs->files);

			for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
				{
				if ((pDefs->iResult = defs_ThreadAbort())) break;
				if ((pDefs->iResult = defs_LoadData(pFile))) break;
				if ((pDefs->iResult = defs_LoadXML(pFile))) break;
				if ((pDefs->iResult = defs_UpdateLocalization(&pDefs->db,pFile))) break;
				xml_ReleaseAll(&pFile->xmlRoot);
				pDefs->iPos[0] = (int)(++i/iMax*100.0f);
				}
			}
		}
	else
		{
		defs_SQLErrorMsg(pDefs->db.pSql);
		pDefs->iResult = DEFS_RESULT_FAILED;
		}
	sqlite3_close(pDefs->db.pSql);

	defs_DoneMessage(pDefs);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildEnds);
	App.hThread = NULL;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Statistiques							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la base de données ««««««««««««««««««««««««««««««««««»

void Game_ItemsStatsRebuild(HWND hWnd)
{
	defs_Rebuild(hWnd,IDM_DB_REBUILDITEMSSTATS,TRUE,szDefsItemsStatsPath,(LPTHREAD_START_ROUTINE)Game_ItemsStatsRebuildThread);
	return;
}


// «»»» Tâche de création «««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Game_ItemsStatsRebuildThread(DEFSCONTEXT *pDefs)
{
	defs_CreateMsgQueue();
	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildStarts);

	pDefs->db.uId = sqlite3_open_v2(szItemsStatsDataBasePath,&pDefs->db.pSql,SQLITE_OPEN_READWRITE,NULL);
	if (pDefs->db.uId == SQLITE_OK)
		{
		sqlite3_exec(pDefs->db.pSql,"PRAGMA journal_mode = MEMORY",NULL,NULL,NULL);
		defs_SQLWipeTables(&pDefs->db,pDefs->bWipe,3,"ItemGroups","ItemNames","ItemStats");
		if ((pDefs->iResult = defs_CreateFileList(szDefsItemsStatsPath,pDefs)) == DEFS_RESULT_OK)
			{
			DEFSFILE*	pFile;
			float		i,iMax;

			i = 0.0f;
			iMax = (float)List_EntryCount(&pDefs->files);

			for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
				{
				if ((pDefs->iResult = defs_ThreadAbort())) break;
				if ((pDefs->iResult = defs_LoadData(pFile))) break;
				if ((pDefs->iResult = defs_UpdateStats(&pDefs->db,pFile))) break;
				HeapFree(App.hHeap,0,pFile->pData);
				pFile->pData = NULL;
				pDefs->iPos[0] = (int)(++i/iMax*100.0f);
				}
			}
		}
	else
		{
		defs_SQLErrorMsg(pDefs->db.pSql);
		pDefs->iResult = DEFS_RESULT_FAILED;
		}
	sqlite3_close(pDefs->db.pSql);

	defs_DoneMessage(pDefs);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildEnds);
	App.hThread = NULL;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modèles de base							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la base de données ««««««««««««««««««««««««««««««««««»

void Game_ItemsRootRebuild(HWND hWnd)
{
	defs_Rebuild(hWnd,IDM_DB_REBUILDITEMSROOTS,FALSE,szDefsItemsRootPath,(LPTHREAD_START_ROUTINE)Game_ItemsRootRebuildThread);
	return;
}


// «»»» Tâche de création «««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Game_ItemsRootRebuildThread(DEFSCONTEXT *pDefs)
{
	defs_CreateMsgQueue();
	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildStarts);

	pDefs->db.uId = sqlite3_open_v2(szItemsTemplatesDataBasePath,&pDefs->db.pSql,SQLITE_OPEN_READWRITE,NULL);
	if (pDefs->db.uId == SQLITE_OK)
		{
		sqlite3_exec(pDefs->db.pSql,"PRAGMA journal_mode = MEMORY",NULL,NULL,NULL);
		defs_SQLWipeTables(&pDefs->db,pDefs->bWipe,1,"RootObjects");
		if ((pDefs->iResult = defs_CreateFileList(szDefsItemsRootPath,pDefs)) == DEFS_RESULT_OK)
			{
			DEFSFILE*	pFile;
			float		i,iMax;

			i = 0.0f;
			iMax = (float)List_EntryCount(&pDefs->files);

			for (pFile = (DEFSFILE *)pDefs->files.next; pFile != NULL; pFile = (DEFSFILE *)pFile->node.next)
				{
				if ((pDefs->iResult = defs_ThreadAbort())) break;
				if ((pDefs->iResult = defs_LoadData(pFile))) break;
				if ((pDefs->iResult = defs_LoadXML(pFile))) break;
				if ((pDefs->iResult = defs_UpdateRootTemplates(&pDefs->db,pFile))) break;
				xml_ReleaseAll(&pFile->xmlRoot);
				pDefs->iPos[0] = (int)(++i/iMax*100.0f);
				}
			}
		}
	else
		{
		defs_SQLErrorMsg(pDefs->db.pSql);
		pDefs->iResult = DEFS_RESULT_FAILED;
		}
	sqlite3_close(pDefs->db.pSql);

	defs_DoneMessage(pDefs);
	Debug_Log(DEBUG_LOG_INFO,szDebugRebuildEnds);
	App.hThread = NULL;
	return(0);
}

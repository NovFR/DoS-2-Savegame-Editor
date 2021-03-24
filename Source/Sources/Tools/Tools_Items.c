
//<<>-<>>---------------------------------------------------------------------()
/*
	Internal tools
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Tools.h"
#include "XML.h"
#include "Game.h"
#include "Taskbar.h"
#include "Requests.h"
#include "Utils.h"
#include "Texts.h"
#include "Debug.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Traduction des objets						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation ««««««««««««««««««««««««««««««««««««««««««««««««««»

void Tools_TranslateItems(HWND hWnd, WCHAR *pszStatsLSXPath, WCHAR *pszLocaleXMLPath, WCHAR *pszLocale)
{
	TLSCONTEXT*	ctx;

	ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(TLSCONTEXT));
	if (!ctx)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Tools_TranslateItemsRelease(ctx);
		return;
		}

	ctx->hwndParent = hWnd;
	ctx->pszStatsLSXPath = pszStatsLSXPath;
	ctx->pszLocaleXMLPath = pszLocaleXMLPath;
	ctx->pszLocale = pszLocale;

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Tools_TranslateItemsThread,ctx,0,NULL);
	if (!App.hThread)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Tools_TranslateItemsRelease(ctx);
		return;
		}

	return;
}


// «»»» Libère la mémoire utilisée ««««««««««««««««««««««««««««««««««««««»

void Tools_TranslateItemsRelease(TLSCONTEXT *ctx)
{
	if (!ctx) return;

	Locale_Unload(LOCALE_TYPE_MISC_WRITE,(void **)&ctx->pLocale,NULL);
	xml_ReleaseAll(&ctx->nodeStats);
	xml_ReleaseAll(&ctx->nodeLocale);

	if (ctx->pTable)
		{
		TLSCONTENT*	pTable;

		for (pTable = ctx->pTable; pTable->pszText != NULL; pTable++) if (pTable->pszText) HeapFree(App.hHeap,0,pTable->pszText);
		HeapFree(App.hHeap,0,ctx->pTable);
		}

	HeapFree(App.hHeap,0,ctx);
	return;
}


// «»»» Traduction ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Tools_TranslateItemsThread(TLSCONTEXT *ctx)
{
	XML_NODE*	pxn;
	WCHAR*		pszValue;
	TLSCONTENT*	pTable;
	UINT		uTotal;
	UINT		uCount;
	GUID		guid;
	int		i;

	static WCHAR*		szTLSContentPath[] = { L"save",NULL, L"region",L"TranslatedStringKeys", L"node",L"root", L"children",NULL, NULL };
	static WCHAR*		szTLSLocalePath[] = { L"contentList",NULL, NULL };

	// Those items won't be in player's inventory
	// Uses wcsncmp
	static TLSBLACKLIST	ItemsBlackList[] = {	{ 0, L"Arena_" },
							{ 0, L"SKILLBOOST_" },
							{ 0, L"TOTEM_" },
							{ 0, L"Stats_" },
							{ 0, L"ObjectCategories_" },
							{ 0, L"ItemCombinationProperty_" },
							{ 0, NULL },
						};

	Game_Lock(GAME_LOCK_DISABLED|GAME_LOCK_APP);
	Taskbar_SetProgressState(TBPF_INDETERMINATE);

	Debug_Log(0,NULL);
	Debug_Log(DEBUG_LOG_INFO,L"- ITEMS LOCALIZATION -");
	Debug_Log(DEBUG_LOG_INFO,L"Items: %s",ctx->pszStatsLSXPath);
	Debug_Log(DEBUG_LOG_INFO,L"Translation: %s",ctx->pszLocaleXMLPath);
	if (ctx->pLocale) Debug_Log(DEBUG_LOG_INFO,L"Database: '%s'",ctx->pszLocale);
	else Debug_Log(DEBUG_LOG_INFO,L"Simulation mode - No DB");

	for (i = 0; ItemsBlackList[i].pszBegin != NULL; i++) ItemsBlackList[i].uLen = wcslen(ItemsBlackList[i].pszBegin);

	//--- Target DB ---

	if (ctx->pszLocale && !Locale_Load(ctx->hwndParent,szLangPath,ctx->pszLocale,LOCALE_TYPE_MISC_WRITE,(void **)&ctx->pLocale,NULL)) goto Done;

	//--- Load and parse XML locale ---

	if (!xml_LoadFile(ctx->pszLocaleXMLPath,&ctx->nodeLocale,XML_FLAG_HASCONTENT)) goto Done;

	pxn = xml_GetNodeFromPathFirstChild((XML_NODE *)ctx->nodeLocale.next,szTLSLocalePath);
	if (pxn)
		{
		Status_SetText(L"Building translation table...");
		uTotal = xml_TotalNodesCount(pxn,FALSE);
		ctx->pTable = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,++uTotal*sizeof(TLSCONTENT));
		if (ctx->pTable)
			{
			UINT		uLen;

			pTable = ctx->pTable;
			uTotal--;
			uCount = 0;
			for (; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
				{
				xml_UpdateProgress(uCount++,uTotal);
				pszValue = xml_GetAttrValue(pxn,L"contentuid");
				if (!pszValue) continue;
				if (!Tools_LarianStringToGUID(pszValue,&pTable->guid))
					{
					Debug_Log(DEBUG_LOG_ERROR,L"Failed to create GUID for HANDLE '%s'",pszValue);
					continue;
					}
				uLen = Misc_HtmlSpecialCharsDecode(NULL,pxn->content);
				if (!uLen)
					{
					Debug_Log(DEBUG_LOG_WARNING,L"HANDLE '%s' has no string",pszValue);
					continue;
					}
				pTable->pszText = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
				if (!pTable->pszText)
					{
					Debug_Log(DEBUG_LOG_ERROR,L"Not enough memory for HANDLE '%s'",pszValue);
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					goto Done;
					}
				Misc_HtmlSpecialCharsDecode(pTable->pszText,pxn->content);
				pTable++;
				}
			}
		}

	//--- Load and parse LSX items ---

	if (!xml_LoadFile(ctx->pszStatsLSXPath,&ctx->nodeStats,XML_FLAG_HASHEADER)) goto Done;

	pxn = xml_GetNodeFromPathFirstChild((XML_NODE *)ctx->nodeStats.next,szTLSContentPath);
	if (pxn)
		{
		BOOL		bMatch;
		char*		pszUTF8ID;
		char*		pszUTF8Text;
		sqlite3_stmt*	stmt;

		Status_SetText(L"Checking and adding items translations...");
		uTotal = xml_TotalNodesCount(pxn,FALSE);
		uCount = 0;

		for (; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
			{
			xml_UpdateProgress(uCount++,uTotal);
			pszValue = xml_GetAttrValue(pxn,szXMLid);
			if (!pszValue) continue;
			if (wcscmp(pszValue,L"TranslatedStringKey")) continue;
			pszValue = xml_GetAttrValue(xml_GetNode((XML_NODE *)pxn->children.next,szXMLattribute,szXMLid,L"Content"),L"handle");
			if (!pszValue) continue;
			if (!Tools_LarianStringToGUID(pszValue,&guid)) continue;
			// GUID
			for (bMatch = FALSE, pTable = ctx->pTable; pTable->pszText != NULL; pTable++)
				{
				if (pTable->guid.Data1 != guid.Data1) continue;
				if (pTable->guid.Data2 != guid.Data2) continue;
				if (pTable->guid.Data3 != guid.Data3) continue;
				if (pTable->guid.Data4[0] != guid.Data4[0]) continue;
				if (pTable->guid.Data4[1] != guid.Data4[1]) continue;
				if (pTable->guid.Data4[2] != guid.Data4[2]) continue;
				if (pTable->guid.Data4[3] != guid.Data4[3]) continue;
				if (pTable->guid.Data4[4] != guid.Data4[4]) continue;
				if (pTable->guid.Data4[5] != guid.Data4[5]) continue;
				if (pTable->guid.Data4[6] != guid.Data4[6]) continue;
				if (pTable->guid.Data4[7] != guid.Data4[7]) continue;
				bMatch = TRUE;
				break;
				}
			if (!bMatch) Debug_Log(DEBUG_LOG_WARNING,L"%s not found in translation table",pszValue);
			// UUID
			pszValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxn->children.next,szXMLattribute,szXMLid,L"UUID"));
			if (!pszValue) continue;
			if (!bMatch)
				{
				Debug_Log(DEBUG_LOG_WARNING,L"%s has no translation",pszValue);
				continue;
				}
			// Blacklist
			for (bMatch = FALSE, i = 0; ItemsBlackList[i].pszBegin != NULL; i++) if (!wcsncmp(pszValue,ItemsBlackList[i].pszBegin,ItemsBlackList[i].uLen)) bMatch = TRUE;
			if (bMatch)
				{
				Debug_Log(DEBUG_LOG_INFO,L"%s has been ignored",pszValue);
				continue;
				}
			// Add item into DB
			if (!ctx->pLocale) continue;
			pszUTF8ID = Misc_WideCharToUTF8(pszValue);
			pszUTF8Text = Misc_WideCharToUTF8(pTable->pszText);
			if (pszUTF8ID && pszUTF8Text && sqlite3_prepare_v2(ctx->pLocale->db,"INSERT OR IGNORE INTO items (id,text) VALUES(?,?)",-1,&stmt,NULL) == SQLITE_OK)
				{
				bMatch = FALSE;
				if (sqlite3_bind_text(stmt,1,pszUTF8ID,strlen(pszUTF8ID),NULL) == SQLITE_OK)
					if (sqlite3_bind_text(stmt,2,pszUTF8Text,strlen(pszUTF8Text),NULL) == SQLITE_OK)
						if (sqlite3_step(stmt) == SQLITE_DONE) bMatch = TRUE;
				if (!bMatch)
					{
					WCHAR *pszError = Misc_UTF8ToWideChar(sqlite3_errmsg(ctx->pLocale->db));
					if (pszError)
						{
						Debug_Log(DEBUG_LOG_ERROR,L"SQLITE %s",pszError);
						HeapFree(App.hHeap,0,pszError);
						}
					else Debug_Log(DEBUG_LOG_ERROR,L"SQLITE FAILED");
					}
				sqlite3_finalize(stmt);
				}
			if (pszUTF8Text) HeapFree(App.hHeap,0,pszUTF8Text);
			if (pszUTF8ID) HeapFree(App.hHeap,0,pszUTF8ID);
			}
		Debug_Log(DEBUG_LOG_INFO,L"DONE: Parsed %ld items",uCount-1);
		}

	//--- Done ---

Done:	xml_UpdateProgress(-1,-1);
	Taskbar_SetProgressState(TBPF_NOPROGRESS);
	Status_SetText(Locale_GetText(TEXT_DONE));
	Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_APP);
	Tools_TranslateItemsRelease(ctx);
	App.hThread = NULL;
	return(0);
}


// «»»» Conversion d'un UID de Larian en GUID «««««««««««««««««««««««««««»

int Tools_LarianStringToGUID(WCHAR *pszUID, GUID *pGUID)
{
	WCHAR		IID[40];

	if (wcslen(pszUID) != 37) return(0);

	wcscpy(IID,pszUID);
	IID[0] = L'{';
	IID[9] = L'-';
	IID[14] = L'-';
	IID[19] = L'-';
	IID[24] = L'-';
	IID[37] = L'}';
	IID[38] = 0;
	if (IIDFromString(IID,pGUID) != S_OK) return(0);

	return(1);
}

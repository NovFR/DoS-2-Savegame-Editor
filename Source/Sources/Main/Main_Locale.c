
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des textes
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Locale.h"
#include "Requests.h"
#include "Texts.h"
#include "Utils.h"

extern APPLICATION	App;
extern WCHAR*		TextsIds[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Gestion des fichiers & des structures				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Énumération des langues disponibles «««««««««««««««««««««««««««««»

int Locale_Enum(HWND hWnd, WCHAR *pszPathFmt, NODE *pRoot)
{
	WIN32_FIND_DATA		Find;
	HANDLE			hFile;
	LOCALE_MISC*		pLocale;
	LOCALE_ENUM*		pEnum;
	WCHAR*			pszExt;
	WCHAR*			pszSearchPath;
	DWORD_PTR		vl[1];

	ZeroMemory(pRoot,sizeof(NODE));

	//--- Création du chemin de recherche ---

	vl[0] = (DWORD_PTR)L"*";
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszPathFmt,0,0,(WCHAR *)&pszSearchPath,1,(va_list *)vl))
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_LOCALE_ENUM),NULL,MB_ICONERROR);
		return(0);
		}

	//--- Obtention de la liste des fichiers ---

	hFile = FindFirstFile(pszSearchPath,&Find);
	if (hFile == INVALID_HANDLE_VALUE)
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_LOCALE_ENUM),NULL,MB_ICONERROR);
		LocalFree(pszSearchPath);
		return(0);
		}

	LocalFree(pszSearchPath);

	do {

		if (Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
		if (wcslen(Find.cFileName) > LOCALE_LANG_MAX_LENGTH) continue;

		pszExt = StrRChr(Find.cFileName,NULL,L'.');
		if (!pszExt) continue;

		pEnum = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LOCALE_ENUM));
		if (!pEnum)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			break;
			}
		wcsncpy(pEnum->szLang,Find.cFileName,pszExt-Find.cFileName);

		if (!Locale_Load(hWnd,pszPathFmt,pEnum->szLang,LOCALE_TYPE_MISC,(void **)&pLocale,&pEnum->pszName))
			{
			FindClose(hFile);
			Locale_EnumRelease(pRoot);
			return(0);
			}
		Locale_Unload(LOCALE_TYPE_MISC,(void **)&pLocale,NULL);
		List_AddEntry((NODE *)pEnum,pRoot);

	} while (FindNextFile(hFile,&Find));

	if (GetLastError() != ERROR_NO_MORE_FILES)
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_LOCALE_ENUM),NULL,MB_ICONERROR);
		FindClose(hFile);
		Locale_EnumRelease(pRoot);
		return(0);
		}

	FindClose(hFile);
	return(1);
}

//--- Libération de la structure ---

void Locale_EnumRelease(NODE *pRoot)
{
	LOCALE_ENUM*	pEnum;

	for (pEnum = (LOCALE_ENUM *)pRoot->next; pEnum != NULL; pEnum = (LOCALE_ENUM *)pEnum->node.next)
		{
		if (pEnum->hIcon) DestroyIcon(pEnum->hIcon);
		if (pEnum->pszName) HeapFree(App.hHeap,0,pEnum->pszName);
		}

	List_ReleaseMemory(pRoot);
	return;
}


// «»»» Chargement des textes «««««««««««««««««««««««««««««««««««««««««««»

int Locale_Load(HWND hWnd, WCHAR *pszPathFmt, WCHAR *pszLang, LONG lType, void **pLocalePtr, WCHAR **pszLocaleNamePtr)
{
	LOCALE_PARSER	Parser;
	DWORD_PTR	dwpLangFile[] = { (DWORD_PTR)pszLang };
	UINT		uId;

	ZeroMemory(&Parser,sizeof(LOCALE_PARSER));
	Parser.pszLastError = Locale_GetText(TEXT_ERR_LOCALE_MISC);
	if (!Parser.pszLastError) Parser.pszLastError = szLocaleFatalErr;

	//--- Alloue le buffer pour les textes ---

	switch(lType)
		{
		case LOCALE_TYPE_APPLICATION:
			*pLocalePtr = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LOCALE_TEXT)*TEXT_LAST);
			if (!*pLocalePtr)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				goto Done;
				}
			// Initialise les identifiants numériques & textuels
			for (uId = 0; uId != TEXT_LAST; uId++)
				{
				((LOCALE_TEXT *)*pLocalePtr)[uId].lUID = uId;
				((LOCALE_TEXT *)*pLocalePtr)[uId].pszUID = TextsIds[uId];
				}
			break;
		case LOCALE_TYPE_MISC:
			*pLocalePtr = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LOCALE_MISC));
			if (!*pLocalePtr)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				goto Done;
				}
			break;
		}

	//--- Création du nom (Path\%1.txt) ---

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszPathFmt,0,0,(WCHAR *)&Parser.pszFilePath,1,(va_list *)dwpLangFile)) goto Done;

	//--- Ouverture de la base de données ---

	Parser.pszDataBasePath = Misc_WideCharToUTF8(Parser.pszFilePath);
	if (!Parser.pszDataBasePath) goto Done;
	Parser.pszLastError = Locale_GetText(TEXT_ERR_LOCALE);
	if (!Parser.pszLastError) Parser.pszLastError = szLocaleErr;

	uId = sqlite3_open_v2(Parser.pszDataBasePath,&Parser.db,SQLITE_OPEN_READONLY,NULL);
	if (uId != SQLITE_OK)
		{
		DWORD_PTR	vl[3];

		vl[0] = (DWORD_PTR)Parser.pszLastError;
		vl[1] = (DWORD_PTR)uId;
		vl[2] = (DWORD_PTR)Misc_UTF8ToWideChar(sqlite3_errmsg(Parser.db));
		if (vl[2])
			{
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szSQLiteErr,0,0,(WCHAR *)&Parser.pszSQLiteErr,1,(va_list *)vl)) Parser.pszLastError = Parser.pszSQLiteErr;
			HeapFree(App.hHeap,0,(WCHAR *)vl[2]);
			}
		SetLastError(ERROR_SUCCESS);
		sqlite3_close(Parser.db);
		goto Done;
		}

	//--- Récupération du nom de la langue ---

	if (pszLocaleNamePtr)
		{
		uId = sqlite3_exec(Parser.db,szLocaleLangQuery,Locale_Language,pszLocaleNamePtr,NULL);
		if (uId != SQLITE_OK || !*pszLocaleNamePtr)
			{
			SetLastError(ERROR_SUCCESS);
			Parser.pszLastError = Locale_GetText(TEXT_ERR_LOCALE_LANG);
			if (!Parser.pszLastError) Parser.pszLastError = szLocaleLangErr;
			sqlite3_close(Parser.db);
			goto Done;
			}
		}

	//--- Récupération des textes ---

	Parser.pszLastError = NULL;
	Parser.iResult = 1;

	switch(lType)
		{
		case LOCALE_TYPE_APPLICATION:
			uId = sqlite3_exec(Parser.db,szLocaleAppQuery,Locale_Parse,*pLocalePtr,NULL);
			if (uId != SQLITE_OK)
				{
				SetLastError(ERROR_SUCCESS);
				Parser.pszLastError = Locale_GetText(TEXT_ERR_LOCALE);
				if (!Parser.pszLastError) Parser.pszLastError = szLocaleErr;
				Parser.iResult = 0;
				break;
				}
			for (uId = 0; uId != TEXT_LAST; uId++)
				if (((LOCALE_TEXT *)*pLocalePtr)[uId].pszText == NULL)
					{
					Parser.iResult = 0;
					Parser.pszLastError = Locale_GetText(TEXT_ERR_LOCALE_MISSINGAPP);
					if (!Parser.pszLastError) Parser.pszLastError = szLocaleMissingAppErr;
					if (Request_MessageBoxEx(hWnd,Parser.pszLastError,NULL,MB_ICONERROR|MB_OKCANCEL,Parser.pszFilePath,TextsIds[uId]) == IDOK) continue;
					Parser.pszLastError = NULL;
					break;
					}
			Parser.pszLastError = NULL;
			sqlite3_close(Parser.db);
			break;
		case LOCALE_TYPE_MISC:
			((LOCALE_MISC *)*pLocalePtr)->db = Parser.db;
			break;
		default:sqlite3_close(Parser.db);
			break;
		}

	//--- Terminé ---

Done:	if (Parser.pszLastError)
		{
		if (GetLastError() == ERROR_SUCCESS) Request_MessageBoxEx(hWnd,Parser.pszLastError,NULL,MB_ICONHAND,Parser.pszFilePath);
		else Request_PrintErrorEx(hWnd,Parser.pszLastError,NULL,MB_ICONHAND,Parser.pszFilePath);
		Locale_Unload(lType,pLocalePtr,pszLocaleNamePtr);
		}
	if (Parser.pszFilePath) LocalFree(Parser.pszFilePath);
	if (Parser.pszSQLiteErr) LocalFree(Parser.pszSQLiteErr);
	if (Parser.pszDataBasePath) HeapFree(App.hHeap,0,Parser.pszDataBasePath);
	return(Parser.iResult);
}


// «»»» Libération de la mémoire ««««««««««««««««««««««««««««««««««««««««»

void Locale_Unload(LONG lType, void **pLocalePtr, WCHAR **pLocaleNamePtr)
{
	UINT	uId;

	if (pLocaleNamePtr)
		{
		if (*pLocaleNamePtr)
			{
			HeapFree(App.hHeap,0,*pLocaleNamePtr);
			*pLocaleNamePtr = NULL;
			}
		}

	if (!*pLocalePtr) return;

	switch(lType)
		{
		case LOCALE_TYPE_APPLICATION:
			for (uId = 0; uId != TEXT_LAST; uId++)
				{
				if (((LOCALE_TEXT *)*pLocalePtr)[uId].pszText) HeapFree(App.hHeap,0,((LOCALE_TEXT *)*pLocalePtr)[uId].pszText);
				((LOCALE_TEXT *)*pLocalePtr)[uId].pszText = NULL;
				}
			break;
		case LOCALE_TYPE_MISC:
			sqlite3_close(((LOCALE_MISC *)pLocalePtr)->db);
			break;
		}

	HeapFree(App.hHeap,0,*pLocalePtr);
	*pLocalePtr = NULL;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Parser								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Récupère la langue de la base de données ««««««««««««««««««««««««»

int Locale_Language(void *pLocaleName, int iRowsCount, char **pszRowsTextsPtr, char **pszRowsNamesPtr)
{
	*(WCHAR **)pLocaleName = Misc_UTF8ToWideChar(pszRowsTextsPtr[0]);
	return(SQLITE_OK);
}


// «»»» Récupère les textes de l'application ««««««««««««««««««««««««««««»

int Locale_Parse(void *pLocale, int iRowsCount, char **pszRowsTextsPtr, char **pszRowsNamesPtr)
{
	WCHAR*	pszNameUID;
	WCHAR*	pszTextUID;
	LONG	lUID;

	if (iRowsCount != 2) return(SQLITE_ABORT);
	if (strcmp(pszRowsNamesPtr[0],"id")) return(SQLITE_ABORT);
	if (strcmp(pszRowsNamesPtr[1],"text")) return(SQLITE_ABORT);

	pszNameUID = Misc_UTF8ToWideChar(pszRowsTextsPtr[0]);
	pszTextUID = Misc_UTF8ToWideChar(pszRowsTextsPtr[1]);
	if (pszNameUID && pszTextUID)
		{
		lUID = Locale_GetTextUID((LOCALE_TEXT *)pLocale,pszNameUID);
		if (lUID >= TEXT_LAST)
			{
			#if _DEBUG
			MessageBox(NULL,pszNameUID,L"Debug - Unused Text",MB_OK);
			#endif
			HeapFree(App.hHeap,0,pszNameUID);
			HeapFree(App.hHeap,0,pszTextUID);
			return(SQLITE_OK); // Ignores unused texts
			}
		Locale_SetText((LOCALE_TEXT *)pLocale,lUID,pszTextUID);
		pszTextUID = NULL;
		}
	if (pszNameUID) HeapFree(App.hHeap,0,pszNameUID);
	if (pszTextUID) HeapFree(App.hHeap,0,pszTextUID);

	return(SQLITE_OK);
}


// «»»» Récupère la traduction d'un identifiant «««««««««««««««««««««««««»

BOOL Locale_QueryID(sqlite3 *db, WCHAR *pszDataBaseName, WCHAR *pszId, WCHAR *pszRows, WCHAR *pszResults[], UINT iNumRows)
{
	sqlite3_stmt*	stmt;
	BOOL		bResult;
	DWORD_PTR	vl[2];
	WCHAR*		pszQueryTmp;
	char*		pszQueryId;
	char*		pszQuery;
	int		i;

	stmt = NULL;
	pszQueryId = NULL;
	pszQuery = NULL;
	bResult = FALSE;

	//--- Réinitialise tous les résultats ---

	for (i = 0; i != iNumRows; i++) pszResults[i] = NULL;

	//--- Création de la requête ---

	vl[0] = (DWORD_PTR)pszDataBaseName;
	vl[1] = (DWORD_PTR)(pszRows?pszRows:L"text");
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szLocaleTextQuery,0,0,(WCHAR *)&pszQueryTmp,1,(va_list *)&vl)) return(FALSE);
	pszQuery = Misc_WideCharToUTF8(pszQueryTmp);
	LocalFree(pszQueryTmp);
	if (!pszQuery) goto Done;

	//--- Préparation de la requête ---

	if (sqlite3_prepare_v2(db,pszQuery,-1,&stmt,NULL) != SQLITE_OK) goto Done;
	if (!(pszQueryId = Misc_WideCharToUTF8(pszId))) goto Done;
	if (sqlite3_bind_text(stmt,1,pszQueryId,strlen(pszQueryId),NULL) != SQLITE_OK) goto Done;

	//--- Récupération des colonnes ---

	if (sqlite3_step(stmt) == SQLITE_ROW)
		for (i = 0; i != iNumRows; i++)
			pszResults[i] = Misc_UTF8ToWideChar((const char *)sqlite3_column_text(stmt,i));

	bResult = TRUE;

	//--- Libération de la mémoire ---

Done:	if (pszQueryId) HeapFree(App.hHeap,0,pszQueryId);
	if (pszQuery) HeapFree(App.hHeap,0,pszQuery);
	sqlite3_finalize(stmt);
	return(bResult);
}


// «»»» Récupère les bonus d'une rune «««««««««««««««««««««««««««««««««««»

void Locale_QueryBonuses(sqlite3 *db, WCHAR *pszRuneId, WCHAR *pszResults[])
{
	sqlite3_stmt*	stmt;
	char*		pszQueryID = NULL;
	int		i;

	for (i = 0; i != 3; i++)
		{
		if (pszResults[i]) HeapFree(App.hHeap,0,pszResults[i]);
		pszResults[i] = NULL;
		}
	if (sqlite3_prepare_v2(db,"SELECT bonus1,bonus2,bonus3 FROM runes WHERE id = ? LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
	if (!(pszQueryID = Misc_WideCharToUTF8(pszRuneId))) goto Done;
	if (sqlite3_bind_text(stmt,1,pszQueryID,strlen(pszQueryID),NULL) != SQLITE_OK) goto Done;
	if (sqlite3_step(stmt) != SQLITE_ROW) goto Done;
	for (i = 0; i != 3; i++)
		{
		if (pszResults[i]) HeapFree(App.hHeap,0,pszResults[i]);
		pszResults[i] = Misc_UTF8ToWideChar((const char *)sqlite3_column_text(stmt,i));
		if (!pszResults[i])
			{
			for (i = 0; i != 3; i++)
				{
				if (pszResults[i]) HeapFree(App.hHeap,0,pszResults[i]);
				pszResults[i] = NULL;
				}
			break;
			}
		}
Done:	if (pszQueryID) HeapFree(App.hHeap,0,pszQueryID);
	sqlite3_finalize(stmt);
	return;
}


// «»»» Récupère la traduction d'un talent ««««««««««««««««««««««««««««««»

void Locale_QueryTalent(sqlite3 *db, DWORD dwTalentFlag, UINT uTalentIndex, WCHAR **pszName, WCHAR **pszDescription)
{
	sqlite3_stmt*	stmt;

	if (sqlite3_prepare_v2(db,"SELECT name,description FROM talents WHERE flag = ? AND idx = ? LIMIT 1",-1,&stmt,NULL) != SQLITE_OK) goto Done;
	if (sqlite3_bind_int64(stmt,1,dwTalentFlag) != SQLITE_OK) goto Done;
	if (sqlite3_bind_int(stmt,2,uTalentIndex) != SQLITE_OK) goto Done;
	if (sqlite3_step(stmt) != SQLITE_ROW) goto Done;
	*pszName = Misc_UTF8ToWideChar((const char *)sqlite3_column_text(stmt,0));
	*pszDescription = Misc_UTF8ToWideChar((const char *)sqlite3_column_text(stmt,1));
Done:	sqlite3_finalize(stmt);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ 	Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Gestion des textes de l'application «««««««««««««««««««««««««««««»

//--- Récupération d'un texte en fonction de son identifiant numérique ---

WCHAR* Locale_GetText(LONG lUID)
{
	if (!App.pLocaleTexts) return(NULL);
	if (lUID >= TEXT_LAST) return(NULL);
	else return(App.pLocaleTexts[lUID].pszText);
}

//--- Récupération d'un texte en fonction de son identifiant texte ---

LONG Locale_GetTextUID(LOCALE_TEXT *pLocale, WCHAR *pszText)
{
	LONG	lUID;

	if (!pszText) return(TEXT_LAST);

	for (lUID = 0; lUID != TEXT_LAST; lUID++)
		if (!wcscmp(pszText,pLocale[lUID].pszUID)) break;

	return(lUID);
}

//--- Application d'un texte ---

void Locale_SetText(LOCALE_TEXT *pLocale, LONG lUID, WCHAR *pszText)
{
	if (lUID >= TEXT_LAST) return;
	if (pLocale[lUID].pszText) HeapFree(App.hHeap,0,pLocale[lUID].pszText);
	pLocale[lUID].pszText = pszText;
	return;
}

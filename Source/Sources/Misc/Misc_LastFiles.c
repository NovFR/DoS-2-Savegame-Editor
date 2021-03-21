
//<<>-<>>---------------------------------------------------------------------()
/*
	Fichiers récents
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "LastFiles.h"
#include "Locale.h"
#include "Lists.h"
#include "Divine.h"
#include "Requests.h"
#include "Texts.h"
#include "Utils.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des listes						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Libération de la mémoire utilisée «««««««««««««««««««««««««««««««»

void LastFiles_ReleaseAll()
{
	LASTFILE*	pFile;
	LASTFILE*	pNext;

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL;)
		{
		pNext = (LASTFILE *)pFile->node.next;
		List_RemEntry((NODE *)pFile);
		LastFiles_Release(pFile);
		pFile = pNext;
		}
	return;
}

//--- Libération de la mémoire utilisée par un fichier ---

void LastFiles_Release(LASTFILE *pFile)
{
	if (pFile->pszProfile) HeapFree(App.hHeap,0,pFile->pszProfile);
	if (pFile->pszSaveName) HeapFree(App.hHeap,0,pFile->pszSaveName);
	if (pFile->pszCustomSavePath) HeapFree(App.hHeap,0,pFile->pszCustomSavePath);
	if (pFile->pszDisplay) HeapFree(App.hHeap,0,pFile->pszDisplay);
	HeapFree(App.hHeap,0,pFile);
	return;
}


// «»»» Ajoute un fichier «««««««««««««««««««««««««««««««««««««««««««««««»

void LastFiles_Add(UINT uGame, WCHAR *pszProfile, WCHAR *pszSaveName, WCHAR *pszCustomSavePath)
{
	LASTFILE*	pFile;
	UINT		uCustomLen = 0;
	UINT		uProfileLen = 0;
	UINT		uSaveLen = 0;
	UINT		uLen;

	//--- Vérifie que le fichier n'existe pas déjà dans la liste ---

	if (pszCustomSavePath) uCustomLen = wcslen(pszCustomSavePath);
	if (pszProfile) uProfileLen = wcslen(pszProfile);
	uSaveLen = wcslen(pszSaveName);

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next)
		{
		if (pFile->uGame != uGame) continue;

		if (pszCustomSavePath)
			{
			if (!pFile->pszCustomSavePath) continue;
			if (wcsncmp(pFile->pszCustomSavePath,pszCustomSavePath,uCustomLen)) continue;
			return;
			}

		if (pszProfile)
			{
			if (!pFile->pszProfile) continue;
			if (wcsncmp(pFile->pszProfile,pszProfile,uProfileLen)) continue;
			}

		if (wcsncmp(pFile->pszSaveName,pszSaveName,uSaveLen)) continue;
		return;
		}

	//--- Ajoute la nouvelle entrée ---

	pFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LASTFILE));
	if (!pFile) return;

	pFile->uGame = uGame;
	pFile->pszSaveName = Misc_StrCpyAlloc(pszSaveName);
	if (!pFile->pszSaveName) goto Failed;

	if (uCustomLen)
		{
		pFile->pszCustomSavePath = Misc_StrCpyAlloc(pszCustomSavePath);
		if (!pFile->pszCustomSavePath) goto Failed;
		}
	else if (uProfileLen)
		{
		pFile->pszProfile = Misc_StrCpyAlloc(pszProfile);
		if (!pFile->pszProfile) goto Failed;
		}

	uLen = LastFiles_SetDisplayText(NULL,pFile);
	pFile->pszDisplay = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pFile->pszDisplay) goto Failed;
	LastFiles_SetDisplayText(pFile->pszDisplay,pFile);

	List_AddEntry((NODE *)pFile,&App.nodeLastFiles);
	LastFiles_InsertMenuItem(pFile);

	//--- Supprime la première entrée si la liste est trop longue ---

	if (List_EntryCount(&App.nodeLastFiles) > LASTFILES_MAX)
		{
		pFile = (LASTFILE *)App.nodeLastFiles.next;
		LastFiles_RemoveMenuItem(pFile);
		List_RemEntry((NODE *)pFile);
		LastFiles_Release(pFile);
		}

	LastFiles_SaveList();
	return;

Failed:	HeapFree(App.hHeap,0,pFile);
	return;
}

//--- Supprime une entrée dans le menu ---

void LastFiles_RemoveMenuItem(LASTFILE *pFile)
{
	MENUITEMINFO	MenuItem;
	HMENU		hSubMenu;
	UINT		uPosition;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_SUBMENU;
	MenuItem.hSubMenu = NULL;
	GetMenuItemInfo(App.hMenu,MENU_RECENT,FALSE,&MenuItem);
	hSubMenu = MenuItem.hSubMenu;

	RemoveMenu(MenuItem.hSubMenu,pFile->cMenu.uId,MF_BYCOMMAND);

	for (uPosition = 0; uPosition != IDM_LASTFILES; uPosition++)
		{
		MenuItem.cbSize = sizeof(MENUITEMINFO);
		MenuItem.fMask = MIIM_ID|MIIM_DATA;
		MenuItem.dwItemData = (ULONG_PTR)NULL;
		GetMenuItemInfo(hSubMenu,uPosition,TRUE,&MenuItem);
		if (MenuItem.wID < IDM_LASTFILES) break;
		if (!MenuItem.dwItemData) break;
		MenuItem.fMask = MIIM_ID;
		MenuItem.wID = ((CUSTOMMENUTEMPLATE *)MenuItem.dwItemData)->uId = IDM_LASTFILES+uPosition;
		SetMenuItemInfo(hSubMenu,uPosition,TRUE,&MenuItem);
		}

	DrawMenuBar(App.hWnd);
	return;
}

//--- Ajoute l'entrée dans le menu ---

void LastFiles_InsertMenuItem(LASTFILE *pFile)
{
	MENUITEMINFO	MenuItem;
	HMENU		hSubMenu;
	LONG		lCount;

	lCount = List_EntryCount(&App.nodeLastFiles);

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_SUBMENU;
	MenuItem.hSubMenu = NULL;
	GetMenuItemInfo(App.hMenu,MENU_RECENT,FALSE,&MenuItem);
	hSubMenu = MenuItem.hSubMenu;

	pFile->cMenu.uType = MENU_ITEM;
	pFile->cMenu.uId = IDM_LASTFILES+lCount-1;
	pFile->cMenu.pszText = pFile->pszDisplay;
	pFile->cMenu.uTextLen = wcslen(pFile->pszDisplay);
	pFile->cMenu.uSpecialFlags = MENU_FLAG_FILENAME;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
	MenuItem.fType = MFT_OWNERDRAW;
	MenuItem.fState = 0;
	MenuItem.wID = IDM_LASTFILES+lCount-1;
	MenuItem.dwItemData = (ULONG_PTR)&pFile->cMenu;
	MenuItem.dwTypeData = NULL;
	InsertMenuItem(hSubMenu,lCount-1,TRUE,&MenuItem);

	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEOBSOLETE,MF_BYCOMMAND|MF_ENABLED);
	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEALL,MF_BYCOMMAND|MF_ENABLED);

	DrawMenuBar(App.hWnd);
	return;
}


// «»»» Ajoute tous les fichiers au menu ««««««««««««««««««««««««««««««««»

void LastFiles_AppendItems()
{
	LASTFILE*	pFile;
	MENUITEMINFO	MenuItem;
	HMENU		hSubMenu;
	LONG		lCount;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_SUBMENU;
	MenuItem.hSubMenu = NULL;
	GetMenuItemInfo(App.hMenu,MENU_RECENT,FALSE,&MenuItem);
	hSubMenu = MenuItem.hSubMenu;

	for (lCount = 0, pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next, lCount++)
		{
		MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
		MenuItem.fType = MFT_OWNERDRAW;
		MenuItem.fState = 0;
		MenuItem.wID = IDM_LASTFILES+lCount;
		MenuItem.dwItemData = (ULONG_PTR)&pFile->cMenu;
		MenuItem.dwTypeData = NULL;
		InsertMenuItem(hSubMenu,lCount,TRUE,&MenuItem);
		}

	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEOBSOLETE,MF_BYCOMMAND|MF_ENABLED);
	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEALL,MF_BYCOMMAND|MF_ENABLED);

	DrawMenuBar(App.hWnd);
	return;
}


// «»»» Retire les fichiers obsoletes de la liste «««««««««««««««««««««««»

void LastFiles_RemoveObsolete()
{
	LASTFILE*	pFile;
	LASTFILE*	pNext;
	WCHAR*		pszPath;
	UINT		uCount;
	BOOL		bExists;

	for (uCount = 0, pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = pNext)
		{
		pNext = (LASTFILE *)pFile->node.next;
		bExists = FALSE;

		pszPath = Divine_GetSaveGamePath(pFile->uGame,pFile->pszProfile,pFile->pszSaveName,pFile->pszCustomSavePath);
		if (pszPath)
			{
			if (PathFileExists(pszPath)) bExists = TRUE;
			HeapFree(App.hHeap,0,pszPath);
			}
		if (bExists) continue;

		LastFiles_RemoveMenuItem(pFile);
		List_RemEntry((NODE *)pFile);
		LastFiles_Release(pFile);
		uCount++;
		}

	if (List_EntryCount(&App.nodeLastFiles) == 0)
		{
		EnableMenuItem(App.hMenu,IDM_RECENTREMOVEOBSOLETE,MF_BYCOMMAND|MF_GRAYED);
		EnableMenuItem(App.hMenu,IDM_RECENTREMOVEALL,MF_BYCOMMAND|MF_GRAYED);
		}

	MessageBox(App.hWnd,Locale_GetText(uCount?TEXT_REMOVE_OBSOLETE:TEXT_REMOVE_OBSNONE),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
	LastFiles_SaveList();
	return;
}


// «»»» Retire tous les fichiers de la liste ««««««««««««««««««««««««««««»

void LastFiles_RemoveAll()
{
	LASTFILE*	pFile;

	if (MessageBox(App.hWnd,Locale_GetText(TEXT_REMOVE_ALL),Locale_GetText(TEXT_TITLE_REQUEST),MB_ICONQUESTION|MB_YESNO) != IDYES) return;

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next) DeleteMenu(App.hMenu,pFile->cMenu.uId,MF_BYCOMMAND);
	LastFiles_ReleaseAll();
	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEOBSOLETE,MF_BYCOMMAND|MF_GRAYED);
	EnableMenuItem(App.hMenu,IDM_RECENTREMOVEALL,MF_BYCOMMAND|MF_GRAYED);
	DrawMenuBar(App.hWnd);
	LastFiles_SaveList();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement & Sauvegarde						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Charge la liste «««««««««««««««««««««««««««««««««««««««««««««««««»

void LastFiles_LoadList()
{
	LASTFILE*	pFile;
	FILEHEADER	Header;
	HANDLE		hFile;
	DWORD		dwBytes;
	DWORD		dwValue;
	UINT		uLen;

	if (!PathFileExists(szLastFilesPath)) return;

	//--- Chargement de la liste ---

	hFile = CreateFile(szLastFilesPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile == INVALID_HANDLE_VALUE) goto Done;

	dwValue = GetFileSize(hFile,NULL);
	if (dwValue == 0xFFFFFFFF) goto Done;
	if (dwValue < sizeof(FILEHEADER)) goto Done;

	ReadFile(hFile,&Header,sizeof(FILEHEADER),&dwBytes,NULL);
	if (dwBytes != sizeof(FILEHEADER)) goto Done;
	if (Header.head != FILE_HEADER_LASTFILES) goto Done;
	if (Header.version > LASTFILES_VERSION) goto Done;

	do {

		pFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LASTFILE));
		if (!pFile) break;

		//--- Version 1 ---
		if (Header.version == 1)
			{
			WCHAR*	pszPath;

			ReadFile(hFile,&dwValue,sizeof(DWORD),&dwBytes,NULL);
			if (dwBytes != sizeof(DWORD)) break;

			pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,dwValue+sizeof(WCHAR));
			if (!pszPath) break;

			ReadFile(hFile,pszPath,dwValue,&dwBytes,NULL);
			if (dwBytes != dwValue)
				{
				HeapFree(App.hHeap,0,pszPath);
				break;
				}
			LastFiles_Explode(pszPath,&pFile->uGame,&pFile->pszProfile,&pFile->pszSaveName);
			HeapFree(App.hHeap,0,pszPath);
			}
		//--- Version 2 ---
		else if (Header.version == 2)
			{
			if (!LastFiles_LoadData(hFile,LASTFILE_DATA_UINT,(void **)&pFile->uGame)) break;
			if (!LastFiles_LoadData(hFile,LASTFILE_DATA_WCHAR,(void **)&pFile->pszProfile)) break;
			if (!LastFiles_LoadData(hFile,LASTFILE_DATA_WCHAR,(void **)&pFile->pszSaveName)) break;
			if (!LastFiles_LoadData(hFile,LASTFILE_DATA_WCHAR,(void **)&pFile->pszCustomSavePath)) break;
			}

		uLen = LastFiles_SetDisplayText(NULL,pFile);
		pFile->pszDisplay = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pFile->pszDisplay) break;
		LastFiles_SetDisplayText(pFile->pszDisplay,pFile);

		List_AddEntry((NODE *)pFile,&App.nodeLastFiles);
		LastFiles_InsertMenuItem(pFile);
		pFile = NULL;

	} while (--Header.size);

	if (pFile) LastFiles_Release(pFile);

	//--- Terminé ---

Done:	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	SetLastError(ERROR_SUCCESS);
	return;
}

//--- Chargement d'une donnée ---

BOOL LastFiles_LoadData(HANDLE hFile, UINT uType, void **pData)
{
	DWORD	dwLen;
	DWORD	dwBytes;

	switch(uType)
		{
		case LASTFILE_DATA_UINT:
			ReadFile(hFile,pData,sizeof(UINT),&dwBytes,NULL);
			if (dwBytes != sizeof(UINT)) return(FALSE);
			break;
		case LASTFILE_DATA_WCHAR:
			ReadFile(hFile,&dwLen,sizeof(DWORD),&dwBytes,NULL);
			if (dwBytes != sizeof(DWORD)) return(FALSE);
			if (!dwLen) return(TRUE);
			*pData = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,dwLen*sizeof(WCHAR)+sizeof(WCHAR));
			if (!*pData) return(FALSE);
			ReadFile(hFile,*pData,dwLen*sizeof(WCHAR),&dwBytes,NULL);
			if (dwBytes != dwLen*sizeof(WCHAR)) return(FALSE);
			break;
		}

	return(TRUE);
}


// «»»» Sauvegarde la liste «««««««««««««««««««««««««««««««««««««««««««««»

void LastFiles_SaveList()
{
	LASTFILE*	pLastFile;
	HANDLE		hFile;
	DWORD		dwBytes;
	FILEHEADER	Header;

	if (List_EntryCount(&App.nodeLastFiles) == 0)
		{
		if (!PathFileExists(szLastFilesPath)) return;
		DeleteFile(szLastFilesPath);
		return;
		}

	hFile = CreateFile(szLastFilesPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		{
		SetLastError(ERROR_SUCCESS);
		return;
		}

	Header.head = FILE_HEADER_LASTFILES;
	Header.version = LASTFILES_VERSION;
	Header.size = List_EntryCount(&App.nodeLastFiles);
	if (!WriteFile(hFile,&Header,sizeof(FILEHEADER),&dwBytes,NULL)) goto Write_Error;

	for (pLastFile = (LASTFILE *)App.nodeLastFiles.next; pLastFile != NULL; pLastFile = (LASTFILE *)pLastFile->node.next)
		{
		if (!LastFiles_SaveData(hFile,LASTFILE_DATA_UINT,pLastFile->uGame)) goto Write_Error;
		if (!LastFiles_SaveData(hFile,LASTFILE_DATA_WCHAR,pLastFile->pszProfile)) goto Write_Error;
		if (!LastFiles_SaveData(hFile,LASTFILE_DATA_WCHAR,pLastFile->pszSaveName)) goto Write_Error;
		if (!LastFiles_SaveData(hFile,LASTFILE_DATA_WCHAR,pLastFile->pszCustomSavePath)) goto Write_Error;
		}

	CloseHandle(hFile);
	return;

Write_Error:
	CloseHandle(hFile);
	DeleteFile(szLastFilesPath);
	SetLastError(ERROR_SUCCESS);
	return;
}

//--- Sauvegarde d'une donnée ---

BOOL LastFiles_SaveData(HANDLE hFile, UINT uType, ...)
{
	DWORD	dwBytes;
	DWORD	dwLen;
	UINT	uData;
	WCHAR*	pszData;
	BOOL	bResult = TRUE;
	va_list	vl;

	//--- Écriture des données

	va_start(vl,uType);

	switch(uType)
		{
		case LASTFILE_DATA_UINT:
			uData = va_arg(vl,UINT);
			bResult = WriteFile(hFile,&uData,sizeof(UINT),&dwBytes,NULL);
			break;

		case LASTFILE_DATA_WCHAR:
			pszData = va_arg(vl,WCHAR *);
			if (pszData)
				{
				dwLen = wcslen(pszData);
				bResult = WriteFile(hFile,&dwLen,sizeof(DWORD),&dwBytes,NULL);
				if (!bResult) break;
				bResult = WriteFile(hFile,pszData,dwLen*sizeof(WCHAR),&dwBytes,NULL);
				}
			else
				{
				dwLen = 0;
				bResult = WriteFile(hFile,&dwLen,sizeof(DWORD),&dwBytes,NULL);
				}
			break;
		}

	va_end(vl);
	return(bResult);
}


// «»»» Recharge un fichier récent ««««««««««««««««««««««««««««««««««««««»

void LastFiles_Reload(UINT uId)
{
	LASTFILE*	pFile;

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next) if (pFile->cMenu.uId == uId) break;
	if (!pFile) return;

	Divine_Open(pFile->uGame,pFile->pszProfile,pFile->pszSaveName,pFile->pszCustomSavePath);
	return;
}


// «»»» Sépare un fichier récent en données exploitables ««««««««««««««««»

int LastFiles_Explode(WCHAR *pszPath, UINT *puGame, WCHAR **pszProfilePtr, WCHAR **pszSaveNamePtr)
{
	WCHAR*		pszProfile;
	WCHAR*		pszSaveName;
	UINT		uLen;

	pszProfile = wcschr(pszPath,L'\\');
	if (!pszProfile) return(0);
	if (!pszProfile++) return(0);
	pszSaveName = wcschr(pszProfile,L'\\');
	if (!pszSaveName) return(0);
	if (!pszSaveName++) return(0);

	*puGame = Divine_GetGameFromName(pszPath,pszProfile-pszPath-1);

	uLen = pszSaveName-pszProfile-1;
	*pszProfilePtr = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!*pszProfilePtr) return(0);
	CopyMemory(*pszProfilePtr,pszProfile,uLen*sizeof(WCHAR));
	*(*pszProfilePtr+uLen) = 0;

	*pszSaveNamePtr = HeapAlloc(App.hHeap,0,wcslen(pszSaveName)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!*pszSaveNamePtr) { HeapFree(App.hHeap,0,*pszProfilePtr); return(0); }
	wcscpy(*pszSaveNamePtr,pszSaveName);

	return(1);
}


// «»»» Texte pour affichage ««««««««««««««««««««««««««««««««««««««««««««»

int LastFiles_SetDisplayText(WCHAR *pszText, LASTFILE *pFile)
{
	UINT	uLen;

	//--- Save name

	if (pszText) wcscpy(pszText,pFile->pszSaveName);
	uLen = wcslen(pFile->pszSaveName);

	//--- Profile

	if (pFile->pszProfile)
		{
		if (pszText)
			{
			wcscat(pszText,L" (");
			wcscat(pszText,pFile->pszProfile);
			wcscat(pszText,L")");
			}
		uLen += wcslen(pFile->pszProfile);
		uLen += 3;
		}

	//--- Game

	if (pszText) wcscat(pszText,pFile->uGame == 2?szDefinitiveEdition:szStandardEdition);
	uLen+= wcslen(pFile->uGame == 2?szDefinitiveEdition:szStandardEdition);

	//--- Path

	if (pFile->pszCustomSavePath)
		{
		if (pszText)
			{
			wcscat(pszText,L" - ");
			wcscat(pszText,pFile->pszCustomSavePath);
			}
		uLen += wcslen(pFile->pszCustomSavePath);
		uLen += 3;
		}

	return(uLen);
}

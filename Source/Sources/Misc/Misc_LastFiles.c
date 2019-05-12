
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
	if (pFile->pszPath) HeapFree(App.hHeap,0,pFile->pszPath);
	HeapFree(App.hHeap,0,pFile);
	return;
}


// «»»» Ajoute un fichier «««««««««««««««««««««««««««««««««««««««««««««««»

void LastFiles_Add(WCHAR *pszGame, WCHAR *pszProfile, WCHAR *pszSaveName)
{
	LASTFILE*	pFile;
	WCHAR*		pszNext;
	UINT		uGameLen = wcslen(pszGame);
	UINT		uProfileLen = wcslen(pszProfile);

	//--- Vérifie que le fichier n'existe pas déjà dans la liste ---

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next)
		{
		if (wcsncmp(pFile->pszPath,pszGame,uGameLen)) continue;
		pszNext = wcschr(pFile->pszPath,L'\\');
		if (!pszNext++) continue;
		if (wcsncmp(pszNext,pszProfile,uProfileLen)) continue;
		pszNext = wcschr(pszNext,L'\\');
		if (!pszNext++) continue;
		if (wcscmp(pszNext,pszSaveName)) continue;
		return;
		}

	//--- Copie le chemin et le nom du fichier ---

	pFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LASTFILE));
	if (!pFile) return;

	pFile->pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,wcslen(pszGame)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(pszProfile)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(pszSaveName)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pFile->pszPath)
		{
		LastFiles_Release(pFile);
		return;
		}

	wcscpy(pFile->pszPath,pszGame);
	wcscat(pFile->pszPath,L"\\");
	wcscat(pFile->pszPath,pszProfile);
	wcscat(pFile->pszPath,L"\\");
	wcscat(pFile->pszPath,pszSaveName);

	//--- Ajoute l'entrée à la fin ---

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
	pFile->cMenu.pszText = pFile->pszPath;
	pFile->cMenu.uTextLen = wcslen(pFile->pszPath);
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
	LASTFILE*	pLastFile;
	MENUITEMINFO	MenuItem;
	HMENU		hSubMenu;
	LONG		lCount;

	MenuItem.cbSize = sizeof(MENUITEMINFO);
	MenuItem.fMask = MIIM_SUBMENU;
	MenuItem.hSubMenu = NULL;
	GetMenuItemInfo(App.hMenu,MENU_RECENT,FALSE,&MenuItem);
	hSubMenu = MenuItem.hSubMenu;

	for (lCount = 0, pLastFile = (LASTFILE *)App.nodeLastFiles.next; pLastFile != NULL; pLastFile = (LASTFILE *)pLastFile->node.next, lCount++)
		{
		MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
		MenuItem.fType = MFT_OWNERDRAW;
		MenuItem.fState = 0;
		MenuItem.wID = IDM_LASTFILES+lCount;
		MenuItem.dwItemData = (ULONG_PTR)&pLastFile->cMenu;
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
	LASTFILE*	pLastFile;
	LASTFILE*	pNext;
	UINT		uGame;
	WCHAR*		pszProfile;
	WCHAR*		pszSaveName;
	WCHAR*		pszPath;
	UINT		uCount;
	BOOL		bExists;

	for (uCount = 0, pLastFile = (LASTFILE *)App.nodeLastFiles.next; pLastFile != NULL; pLastFile = pNext)
		{
		pNext = (LASTFILE *)pLastFile->node.next;

		bExists = FALSE;
		if (LastFiles_Explode(pLastFile->pszPath,&uGame,&pszProfile,&pszSaveName))
			{
			pszPath = Divine_GetSaveGamePath(uGame,pszProfile,pszSaveName);
			if (pszPath)
				{
				if (PathFileExists(pszPath)) bExists = TRUE;
				HeapFree(App.hHeap,0,pszPath);
				}
			HeapFree(App.hHeap,0,pszProfile);
			HeapFree(App.hHeap,0,pszSaveName);
			}
		if (bExists) continue;

		LastFiles_RemoveMenuItem(pLastFile);
		List_RemEntry((NODE *)pLastFile);
		LastFiles_Release(pLastFile);
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
	LASTFILE*	pLastFile;

	if (MessageBox(App.hWnd,Locale_GetText(TEXT_REMOVE_ALL),Locale_GetText(TEXT_TITLE_REQUEST),MB_ICONQUESTION|MB_YESNO) != IDYES) return;

	for (pLastFile = (LASTFILE *)App.nodeLastFiles.next; pLastFile != NULL; pLastFile = (LASTFILE *)pLastFile->node.next) DeleteMenu(App.hMenu,pLastFile->cMenu.uId,MF_BYCOMMAND);
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
	LASTFILE*	pLastFile;
	FILEHEADER	Header;
	HANDLE		hFile;
	DWORD		dwBytes;
	DWORD		dwValue;

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
	if (Header.version != 1) goto Done;

	do {

		ReadFile(hFile,&dwValue,sizeof(DWORD),&dwBytes,NULL);
		if (dwBytes != sizeof(DWORD)) break;

		pLastFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LASTFILE));
		if (!pLastFile) break;
		pLastFile->pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,dwValue+sizeof(WCHAR));
		if (!pLastFile->pszPath)
			{
			HeapFree(App.hHeap,0,pLastFile);
			break;
			}
		ReadFile(hFile,pLastFile->pszPath,dwValue,&dwBytes,NULL);
		if (dwBytes != dwValue)
			{
			HeapFree(App.hHeap,0,pLastFile->pszPath);
			HeapFree(App.hHeap,0,pLastFile);
			break;
			}
		List_AddEntry((NODE *)pLastFile,&App.nodeLastFiles);
		LastFiles_InsertMenuItem(pLastFile);

	} while (--Header.size);

	//--- Terminé ---

Done:	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	SetLastError(ERROR_SUCCESS);
	return;
}


// «»»» Sauvegarde la liste «««««««««««««««««««««««««««««««««««««««««««««»

void LastFiles_SaveList()
{
	LASTFILE*	pLastFile;
	HANDLE		hFile;
	DWORD		dwBytes;
	DWORD		dwSize;
	DWORD		dwLen;
	DWORD		dwPad;
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
	Header.version = 1;
	Header.size = List_EntryCount(&App.nodeLastFiles);
	if (!WriteFile(hFile,&Header,sizeof(FILEHEADER),&dwBytes,NULL)) goto Write_Error;

	for (dwPad = 0, pLastFile = (LASTFILE *)App.nodeLastFiles.next; pLastFile != NULL; pLastFile = (LASTFILE *)pLastFile->node.next)
		{
		dwLen = wcslen(pLastFile->pszPath);
		dwSize = dwLen*sizeof(WCHAR);
		dwSize += dwSize%sizeof(DWORD);
		if (!WriteFile(hFile,&dwSize,sizeof(DWORD),&dwBytes,NULL)) goto Write_Error;
		if (!WriteFile(hFile,pLastFile->pszPath,dwLen*sizeof(WCHAR),&dwBytes,NULL)) goto Write_Error;
		dwSize = (dwLen*sizeof(WCHAR))%sizeof(DWORD);
		if (!dwSize) continue;
		if (!WriteFile(hFile,&dwPad,dwSize,&dwBytes,NULL)) goto Write_Error;
		}

	CloseHandle(hFile);
	return;

Write_Error:
	CloseHandle(hFile);
	DeleteFile(szLastFilesPath);
	SetLastError(ERROR_SUCCESS);
	return;
}


// «»»» Recharge un fichier récent ««««««««««««««««««««««««««««««««««««««»

void LastFiles_Reload(UINT uId)
{
	LASTFILE*	pFile;
	UINT		uGame;
	WCHAR*		pszProfile;
	WCHAR*		pszSaveName;

	for (pFile = (LASTFILE *)App.nodeLastFiles.next; pFile != NULL; pFile = (LASTFILE *)pFile->node.next) if (pFile->cMenu.uId == uId) break;
	if (!pFile) return;

	if (LastFiles_Explode(pFile->pszPath,&uGame,&pszProfile,&pszSaveName))
		{
		Divine_Open(uGame,pszProfile,pszSaveName);
		HeapFree(App.hHeap,0,pszProfile);
		HeapFree(App.hHeap,0,pszSaveName);
		}
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

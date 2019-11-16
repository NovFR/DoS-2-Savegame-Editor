
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des fichiers LSV
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Files.h"
#include "Requests.h"
#include "Lists.h"
#include "LZ4.h"
#include "zlib.h"
#include "Texts.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Charge et décompresse l'archive «««««««««««««««««««««««««««««««««»

BOOL lsv_Load(HWND hWnd, WCHAR *pszArchivePath, NODE *pRoot, DWORD dwMode)
{
	LSVREADER*	pReader;
	WCHAR*		pszArchiveName;
	BOOL		bIsDone = FALSE;

	//--- Création de la structure de travail ---

	pszArchiveName = PathFindFileName(pszArchivePath);

	pReader = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LSVREADER));
	if (!pReader)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	//--- Chargement du fichier ---

	pReader->hFile = CreateFile(pszArchivePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (pReader->hFile == INVALID_HANDLE_VALUE) goto Done;

	pReader->dwFileSize = GetFileSize(pReader->hFile,NULL);
	if (pReader->dwFileSize == 0xFFFFFFFF) goto Done;

	pReader->pFileBuffer = HeapAlloc(App.hHeap,0,pReader->dwFileSize);
	if (!pReader->pFileBuffer)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	if (!ReadFile(pReader->hFile,pReader->pFileBuffer,pReader->dwFileSize,&pReader->dwBytes,NULL)) goto Done;

	//--- Vérifie que le fichier est du bon format ---

	if (strncmp("LSPK",(char *)&pReader->pFileBuffer[pReader->dwFileSize-4],4))
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_UNKNOWN;
		goto Done;
		}

	//--- Récupère les données importantes ---

	pReader->pHeader = (LSPKHEADER13 *)&pReader->pFileBuffer[pReader->dwFileSize-pReader->pFileBuffer[pReader->dwFileSize-8]];
	pReader->pFileList = (BYTE *)&pReader->pFileBuffer[pReader->pHeader->FileListOffset];
	pReader->iNumFiles = *(DWORD *)pReader->pFileList;

	//--- Vérifie que l'archive est de la bonne version ---

	if (pReader->pHeader->Version != 13)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_VERSION;
		goto Done;
		}

	if (pReader->iNumFiles < 1)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_EMPTY;
		goto Done;
		}

	//--- Décompresse la liste des fichiers ---

	pReader->pFileListBuffer = (FILEENTRY13 *)HeapAlloc(App.hHeap,0,sizeof(FILEENTRY13)*pReader->iNumFiles);
	if (!pReader->pFileListBuffer) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }

	pReader->iDecompressed = LZ4_decompress_safe((const char *)pReader->pFileList+4,(char *)pReader->pFileListBuffer,pReader->pHeader->FileListSize-4,sizeof(FILEENTRY13)*pReader->iNumFiles);
	if (pReader->iDecompressed != sizeof(FILEENTRY13)*pReader->iNumFiles)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_FILESLIST;
		goto Done;
		}

	//--- Décompresse les fichiers ---

	while (pReader->iNumFiles--)
		{
		LSFILE*		pFile;
		ULONG		crc;
		int		iSize;

		//--- Ne charge que les informations de la sauvegarde ?
		if (dwMode&LS_MODE_SAVEINFO)
			{
			char* pszName = pReader->pFileListBuffer[pReader->iNumFiles].Name;
			if (strcmp("meta.lsf",pszName) && strncmp(".png",pszName+strlen(pszName)-4,4)) continue;
			}

		//--- Structure du fichier
		pFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LSFILE));
		if (!pFile)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}
		List_AddEntry((NODE *)pFile,pRoot);

		//--- Données du fichier
		pFile->uSize = pReader->pFileListBuffer[pReader->iNumFiles].UncompressedSize;
		pFile->pData = HeapAlloc(App.hHeap,0,pFile->uSize);
		if (!pFile->pData)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}

		//--- Nom du fichier
		iSize = MultiByteToWideChar(CP_ACP,0,pReader->pFileListBuffer[pReader->iNumFiles].Name,-1,NULL,0);
		if (!iSize) goto Done;
		pFile->pszName = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR));
		if (!pFile->pszName) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }
		iSize = MultiByteToWideChar(CP_ACP,0,pReader->pFileListBuffer[pReader->iNumFiles].Name,-1,pFile->pszName,iSize);
		if (!iSize) goto Done;

		//--- CRC32
		crc = crc32(0,NULL,0);
		crc = crc32(crc,pReader->pFileBuffer+pReader->pFileListBuffer[pReader->iNumFiles].OffsetInFile,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk);
		if (crc != pReader->pFileListBuffer[pReader->iNumFiles].CRC)
			{
			SetLastError(ERROR_SUCCESS);
			pReader->uLastError = TEXT_ERR_LSV_CRC32;
			goto Done;
			}

		//--- Décompression
		pReader->uLastError = lsa_Decompress(pReader->pFileListBuffer[pReader->iNumFiles].Flags,pReader->pFileBuffer+pReader->pFileListBuffer[pReader->iNumFiles].OffsetInFile,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk,pFile->pData,pFile->uSize,0);
		if (pReader->uLastError)
			{
			SetLastError(ERROR_SUCCESS);
			goto Done;
			}

		//--- Détermine le type de fichier
		if ((*(DWORD *)pFile->pData&0xFFFFFF00) == 0x474E5000) pFile->dwType |= LS_TYPE_PNG;
		if (*(DWORD *)pFile->pData == 0x464F534C) pFile->dwType |= LS_TYPE_LSF;
		if (!wcscmp(szMetaLSF,pFile->pszName)) pFile->dwType |= LS_TYPE_META;
		if (!wcscmp(szGlobalsLSF,pFile->pszName)) pFile->dwType |= LS_TYPE_GLOBALS;
		}

	SetLastError(ERROR_SUCCESS);
	bIsDone = TRUE;

	//--- Terminé ---

Done:	if (GetLastError() != ERROR_SUCCESS && !(dwMode&LS_MODE_QUIET)) Request_PrintErrorEx(hWnd,Locale_GetText(TEXT_ERR_LSV_LOAD),NULL,MB_ICONHAND,pszArchiveName);

	if (pReader)
		{
		if (pReader->uLastError && !(dwMode&LS_MODE_QUIET)) Request_MessageBoxEx(hWnd,Locale_GetText(TEXT_ERR_LSV_LOADEX),NULL,MB_ICONHAND,pszArchiveName,Locale_GetText(pReader->uLastError));
		if (pReader->pFileListBuffer) HeapFree(App.hHeap,0,pReader->pFileListBuffer);
		if (pReader->pFileBuffer) HeapFree(App.hHeap,0,pReader->pFileBuffer);
		if (pReader->hFile != INVALID_HANDLE_VALUE) CloseHandle(pReader->hFile);
		HeapFree(App.hHeap,0,pReader);
		}

	if (!bIsDone) lsv_Release(pRoot);
	return(bIsDone);
}


// «»»» Libère la mémoire utilisée par la liste des fichiers ««««««««««««»

void lsv_Release(NODE *pRoot)
{
	LSFILE*	pFile;

	for (pFile = (LSFILE *)pRoot->next; pFile != NULL; pFile = (LSFILE *)pFile->node.next)
		{
		if (pFile->hBitmap) DeleteObject(pFile->hBitmap);
		if (pFile->pData) HeapFree(App.hHeap,0,pFile->pData);
		if (pFile->pszName) HeapFree(App.hHeap,0,pFile->pszName);
		xml_ReleaseAll(&pFile->nodeXMLRoot);
		}

	List_ReleaseMemory(pRoot);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve un fichier dans la liste «««««««««««««««««««««««««««««««»

LSFILE* lsv_FindFile(NODE *pRoot, WCHAR *pszName, WCHAR *pszExt)
{
	if (pRoot && (pszName || pszExt))
		{
		LSFILE*	pFile;
		UINT	uExtLen = 0;
		UINT	uNameLen = 0;

		if (pszExt) uExtLen = wcslen(pszExt);

		for (pFile = (LSFILE *)pRoot->next; pFile != NULL; pFile = (LSFILE *)pFile->node.next)
			{
			if (!pFile->pszName) continue;
			if (pszName && wcscmp(pFile->pszName,pszName)) continue;
			uNameLen = wcslen(pFile->pszName);
			if (pszExt && uNameLen > uExtLen && wcsncmp(pFile->pszName+uNameLen-uExtLen,pszExt,uExtLen)) continue;
			return(pFile);
			}
		}

	return(NULL);
}


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

	//--- Ouverture du fichier ---

	pReader->hFile = CreateFile(pszArchivePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_RANDOM_ACCESS,NULL);
	if (pReader->hFile == INVALID_HANDLE_VALUE) goto Done;

	//--- Vérifie que le fichier est du bon format ---

	if (SetFilePointer(pReader->hFile,-4,NULL,FILE_END) == INVALID_SET_FILE_POINTER) goto Done;
	if (!ReadFile(pReader->hFile,&pReader->dwSignature,4,&pReader->dwBytes,NULL)) goto Done;
	if (pReader->dwBytes != 4) goto Done;
	if (pReader->dwSignature != 0x4B50534C) // 'LSPK' (KPSL in the Intel world)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_UNKNOWN;
		goto Done;
		}

	//--- Charge l'en-tête ---

	if (SetFilePointer(pReader->hFile,-8,NULL,FILE_END) == INVALID_SET_FILE_POINTER) goto Done;
	if (!ReadFile(pReader->hFile,&pReader->dwHeaderSize,4,&pReader->dwBytes,NULL)) goto Done;
	if (pReader->dwBytes != 4) goto Done;
	if (pReader->dwHeaderSize-8 != sizeof(LSPKHEADER13))
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_UNKNOWN;
		goto Done;
		}

	if (SetFilePointer(pReader->hFile,-pReader->dwHeaderSize,NULL,FILE_END) == INVALID_SET_FILE_POINTER) goto Done;
	if (!ReadFile(pReader->hFile,&pReader->header,pReader->dwHeaderSize-8,&pReader->dwBytes,NULL)) goto Done;
	if (pReader->dwBytes != pReader->dwHeaderSize-8) goto Done;

	//--- Vérifie que l'archive est de la bonne version ---

	if (pReader->header.Version != 13)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_VERSION;
		goto Done;
		}

	//--- Charge la liste des fichiers ---

	if (SetFilePointer(pReader->hFile,pReader->header.FileListOffset,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) goto Done;

	if (!ReadFile(pReader->hFile,&pReader->iNumFiles,4,&pReader->dwBytes,NULL)) goto Done;
	if (pReader->dwBytes != 4) goto Done;
	if (pReader->iNumFiles < 1)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSV_EMPTY;
		goto Done;
		}
	pReader->header.FileListSize -= 4; // Remove ListSize (4) from TotalSize

	pReader->pFileList = HeapAlloc(App.hHeap,0,pReader->header.FileListSize);
	if (!pReader->pFileList) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }
	if (!ReadFile(pReader->hFile,pReader->pFileList,pReader->header.FileListSize,&pReader->dwBytes,NULL)) goto Done;
	if (pReader->dwBytes != pReader->header.FileListSize) goto Done;

	//--- Décompresse la liste des fichiers ---

	pReader->pFileListBuffer = (FILEENTRY13 *)HeapAlloc(App.hHeap,0,sizeof(FILEENTRY13)*pReader->iNumFiles);
	if (!pReader->pFileListBuffer) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }

	pReader->iDecompressed = LZ4_decompress_safe((const char *)pReader->pFileList,(char *)pReader->pFileListBuffer,pReader->header.FileListSize,sizeof(FILEENTRY13)*pReader->iNumFiles);
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
		BOOL		bSkip;
		char*		pszName;
		int		iSize;

		//--- Vérifie les fichiers à charger
		bSkip = TRUE;
		pszName = pReader->pFileListBuffer[pReader->iNumFiles].Name;
		if (dwMode&LS_LOAD_META && !strcmp("meta.lsf",pszName)) bSkip = FALSE;
		else if (dwMode&LS_LOAD_PNG && !strncmp(".png",pszName+strlen(pszName)-4,4)) bSkip = FALSE;
		if (bSkip) continue;

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

		pReader->pFileBuffer = HeapAlloc(App.hHeap,0,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk);
		if (!pReader->pFileBuffer) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }
		if (SetFilePointer(pReader->hFile,pReader->pFileListBuffer[pReader->iNumFiles].OffsetInFile,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER) goto Done;
		if (!ReadFile(pReader->hFile,pReader->pFileBuffer,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk,&pReader->dwBytes,NULL)) goto Done;
		if (pReader->dwBytes != pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk) goto Done;

		//--- Nom du fichier
		iSize = MultiByteToWideChar(CP_ACP,0,pReader->pFileListBuffer[pReader->iNumFiles].Name,-1,NULL,0);
		if (!iSize) goto Done;
		pFile->pszName = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR));
		if (!pFile->pszName) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }
		iSize = MultiByteToWideChar(CP_ACP,0,pReader->pFileListBuffer[pReader->iNumFiles].Name,-1,pFile->pszName,iSize);
		if (!iSize) goto Done;

		//--- CRC32
		crc = crc32(0,NULL,0);
		crc = crc32(crc,pReader->pFileBuffer,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk);
		if (crc != pReader->pFileListBuffer[pReader->iNumFiles].CRC)
			{
			SetLastError(ERROR_SUCCESS);
			pReader->uLastError = TEXT_ERR_LSV_CRC32;
			goto Done;
			}

		//--- Décompression
		pReader->uLastError = lsa_Decompress(pReader->pFileListBuffer[pReader->iNumFiles].Flags,pReader->pFileBuffer,pReader->pFileListBuffer[pReader->iNumFiles].SizeOnDisk,pFile->pData,pFile->uSize,0);
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

		//--- Libère les données du fichier
		HeapFree(App.hHeap,0,pReader->pFileBuffer);
		pReader->pFileBuffer = NULL;
		}

	SetLastError(ERROR_SUCCESS);
	bIsDone = TRUE;

	//--- Terminé ---

Done:	if (GetLastError() != ERROR_SUCCESS && !(dwMode&LS_LOAD_QUIET)) Request_PrintErrorEx(hWnd,Locale_GetText(TEXT_ERR_LSV_LOAD),NULL,MB_ICONHAND,pszArchiveName);

	if (pReader)
		{
		if (pReader->uLastError && !(dwMode&LS_LOAD_QUIET)) Request_MessageBoxEx(hWnd,Locale_GetText(TEXT_ERR_LSV_LOADEX),NULL,MB_ICONHAND,pszArchiveName,Locale_GetText(pReader->uLastError));
		if (pReader->pFileBuffer) HeapFree(App.hHeap,0,pReader->pFileBuffer);
		if (pReader->pFileListBuffer) HeapFree(App.hHeap,0,pReader->pFileListBuffer);
		if (pReader->pFileList) HeapFree(App.hHeap,0,pReader->pFileList);
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


// «»»» Retrouve la structure XML des métadonnées «««««««««««««««««««««««»

NODE* lsv_GetMetaXML(NODE *pRoot)
{
	if (pRoot)
		{
		LSFILE*	pFile = lsv_FindFile(pRoot,szMetaLSF,NULL);
		if (!pFile) return(NULL);
		return(&pFile->nodeXMLRoot);
		}

	return(NULL);
}

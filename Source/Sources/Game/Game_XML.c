
//<<>-<>>---------------------------------------------------------------------()
/*
	Parser XML
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XML.h"
#include "XMLTree.h"
#include "Game.h"
#include "Texts.h"
#include "Requests.h"
#include "LastFiles.h"
#include "Utils.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement d'un fichier LSX (XML)				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initie le chargement d'un fichier XML «««««««««««««««««««««««««««»

int xml_LoadFile(WCHAR *pszFilePath)
{
	XML_PARSER*	pParser;

	Status_SetText(Locale_GetText(TEXT_LOADING),pszFilePath);

	pParser = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_PARSER));
	if (!pParser)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);
		return(0);
		}

	pParser->pszFilePath = pszFilePath;
	pParser->iResult = xml_ReadFile(pParser);
	if (pParser->iResult != XML_ERROR_NONE)
		{
		xml_SendErrorMsg(pParser->iResult,pParser->dwLastErrorMsg);
		xml_ReleaseAll(&App.Game.Save.nodeXMLRoot);
		xml_UpdateProgress(-1,-1);
		xml_FreeParser(pParser);
		return(0);
		}

	xml_UpdateProgress(-1,-1);
	xml_FreeParser(pParser);
	return(1);
}


// «»»» Chargement d'un fichier XML «««««««««««««««««««««««««««««««««««««»

int xml_ReadFile(XML_PARSER *pParser)
{
	DWORD		dwBytes;

	//--- Alloue & initialise le buffer ---

	pParser->hFile = INVALID_HANDLE_VALUE;
	pParser->dwLastErrorType = XML_ERROR_FROM_SYSTEM;

	//--- Chargement du fichier ---

	pParser->hFile = CreateFile(pParser->pszFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (pParser->hFile == INVALID_HANDLE_VALUE) goto Done;

	pParser->dwFileSize = GetFileSize(pParser->hFile,NULL);
	if (pParser->dwFileSize == 0xFFFFFFFF) goto Done;

	pParser->pFileBuffer = HeapAlloc(App.hHeap,0,pParser->dwFileSize);
	if (!pParser->pFileBuffer) { SetLastError(ERROR_NOT_ENOUGH_MEMORY); goto Done; }

	ReadFile(pParser->hFile,pParser->pFileBuffer,pParser->dwFileSize,&dwBytes,NULL);
	if (dwBytes != pParser->dwFileSize) goto Done;

	pParser->dwLastErrorType = XML_ERROR_NONE;

	//--- Vérification basiques ---

	pParser->dwLastErrorType = xml_ReadTag(pParser);
	if (pParser->dwLastErrorType == XML_ERROR_EOF)
		{
		pParser->dwLastErrorMsg = TEXT_ERR_XML_UNEXPECTED_EOF;
		pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
		}
	if (pParser->dwLastErrorType != XML_ERROR_NONE) goto Done;

	if (pParser->dwTagSize < strlen(szXMLHeader))
		{
		pParser->dwLastErrorMsg = TEXT_ERR_XML_HEADER_UNKNOWN;
		pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
		goto Done;
		}

	if (strncmp((char *)&pParser->pFileBuffer[pParser->dwTagBegin],szXMLHeader,strlen(szXMLHeader)))
		{
		pParser->dwLastErrorMsg = TEXT_ERR_XML_HEADER_UNKNOWN;
		pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
		goto Done;
		}

	//--- Lecture ---

	xml_UpdateProgress(0,pParser->dwFileSize);
	pParser->dwLastErrorType = xml_ParseNodes(pParser,NULL,&App.Game.Save.nodeXMLRoot);
	if (pParser->dwLastErrorType == XML_ERROR_EOF) pParser->dwLastErrorType = XML_ERROR_NONE;

	//--- Terminé ! ---

Done:	if (pParser->pFileBuffer) HeapFree(App.hHeap,0,pParser->pFileBuffer);
	if (pParser->hFile != INVALID_HANDLE_VALUE) CloseHandle(pParser->hFile);
	return(pParser->dwLastErrorType);
}


// «»»» Récupère les limites d'un tag «««««««««««««««««««««««««««««««««««»

int xml_ReadTag(XML_PARSER *pParser)
{
	DWORD	dwCursor = pParser->dwCursor;

	pParser->dwTagBegin = 0;
	pParser->dwTagEnd = 0;
	while(pParser->pFileBuffer[dwCursor] != '<')
		if (dwCursor++ >= pParser->dwFileSize)
			return(XML_ERROR_EOF);
	pParser->dwTagBegin = ++dwCursor;
	while(pParser->pFileBuffer[dwCursor] != '>')
		if (dwCursor++ >= pParser->dwFileSize)
			{
			pParser->dwLastErrorMsg = TEXT_ERR_XML_UNEXPECTED_EOF;
			return(XML_ERROR_FROM_LOCALE);
			}
	pParser->dwTagEnd = dwCursor;
	pParser->dwTagSize = pParser->dwTagEnd-pParser->dwTagBegin;

	pParser->dwCursor = dwCursor+1;

	xml_UpdateProgress(pParser->dwCursor,pParser->dwFileSize);
	return(XML_ERROR_NONE);
}


// «»»» Conversion des tags XML en structures «««««««««««««««««««««««««««»

int xml_ParseNodes(XML_PARSER *pParser, XML_NODE *pxnParent, NODE *pRoot)
{
	XML_NODE*	pxnNew;
	DWORD		dwCursor;
	DWORD		dwNameSize;

	pParser->dwLastErrorType = XML_ERROR_NONE;

	while(1)
		{
		pParser->dwLastErrorType = xml_ReadTag(pParser);
		if (pParser->dwLastErrorType != XML_ERROR_NONE) break;

		if (pParser->dwTagSize < 1)
			{
			pParser->dwLastErrorMsg = TEXT_ERR_XML_TAG_EMPTY;
			pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
			break;
			}

		// Récupère le nom du tag (assume qu'il n'y a pas d'espace entre le < et le nom)

		dwCursor = pParser->dwTagBegin;
		dwNameSize = 0;
		while(1)
			{
			if (dwCursor >= pParser->dwTagEnd) break;
			if ((unsigned char)pParser->pFileBuffer[dwCursor] <= ' ') break;
			dwCursor++;
			dwNameSize++;
			}

		if (dwNameSize < 1)
			{
			pParser->dwLastErrorMsg = TEXT_ERR_XML_TAG_EMPTY;
			pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
			break;
			}

		// Alloue la structure

		pxnNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_NODE)+sizeof(WCHAR)*dwNameSize+sizeof(WCHAR));
		if (!pxnNew)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			pParser->dwLastErrorType = XML_ERROR_FROM_SYSTEM;
			break;
			}
		pxnNew->node.type = XML_TYPE_NODE;
		pxnNew->parent = pxnParent;
		MultiByteToWideChar(CP_UTF8,0,(char *)&pParser->pFileBuffer[pParser->dwTagBegin],dwNameSize,pxnNew->name,sizeof(WCHAR)*dwNameSize);

		// Vérifie s'il s'agit d'une fermeture

		if (pxnNew->name[0] == '/')
			{
			if (pxnParent != NULL)
				{
				if (wcscmp(pxnParent->name,&pxnNew->name[1]))
					{
					HeapFree(App.hHeap,0,pxnNew);
					pParser->dwLastErrorMsg = TEXT_ERR_XML_TAG_CLOSE_MISMATCH;
					pParser->dwLastErrorType = XML_ERROR_FROM_LOCALE;
					break;
					}
				}
			HeapFree(App.hHeap,0,pxnNew);
			pParser->dwLastErrorType = XML_ERROR_NONE;
			break;
			}

		// Lecture des valeurs

		pParser->dwLastErrorType = xml_ParseAttributes(pParser,pxnNew);
		if (pParser->dwLastErrorType != XML_ERROR_NONE)
			{
			HeapFree(App.hHeap,0,pxnNew);
			break;
			}

		// Recupère les tags enfants

		List_AddEntry((NODE *)pxnNew,pRoot);
		if (pParser->pFileBuffer[pParser->dwTagEnd-1] != '/')
			{
			pParser->dwLastErrorType = xml_ParseNodes(pParser,pxnNew,&pxnNew->children);
			if (pParser->dwLastErrorType != XML_ERROR_NONE) break;
			}
		}

	return(pParser->dwLastErrorType);
}


// «»»» Conversion des attributs XML en structures ««««««««««««««««««««««»

int xml_ParseAttributes(XML_PARSER *pParser, XML_NODE *pxnOwner)
{
	XML_ATTR*	pxaNew;
	DWORD		dwCursor;
	DWORD		dwAttrBegin,dwAttrLen;
	DWORD		dwValBegin,dwValLen;
	DWORD		dwBufLen;

	//--- Ignore le nom du tag ---

	dwCursor = pParser->dwTagBegin;
	while(1)
		{
		if (dwCursor >= pParser->dwTagEnd) break;
		if ((unsigned char)pParser->pFileBuffer[dwCursor] <= ' ') break;
		dwCursor++;
		}

	//--- Récupère les attributs ---

	while(1)
		{
		//--- Retrouve le début de l'attribut ---
		while(1)
			{
			if (dwCursor >= pParser->dwTagEnd) break;
			if ((unsigned char)pParser->pFileBuffer[dwCursor] > ' ') break;
			dwCursor++;
			}
		dwAttrBegin = dwCursor;
		if (dwAttrBegin >= pParser->dwTagEnd) break;

		//--- Retrouve la fin de l'attribut ---
		while(1)
			{
			if (dwCursor >= pParser->dwTagEnd) break;
			if (pParser->pFileBuffer[dwCursor] == '=') break;
			dwCursor++;
			}
		if (pParser->pFileBuffer[dwCursor] != '=') break;
		dwAttrLen = dwCursor-dwAttrBegin;
		if (dwAttrLen <= 0)
			{
			pParser->dwLastErrorMsg = TEXT_ERR_XML_ATTR_EMPTY;
			return(XML_ERROR_FROM_LOCALE);
			}

		//--- Retrouve le début de la valeur ---
		dwCursor++;
		while(1)
			{
			if (dwCursor >= pParser->dwTagEnd) break;
			if (pParser->pFileBuffer[dwCursor] == '"') break;
			dwCursor++;
			}
		if (pParser->pFileBuffer[dwCursor] != '"') break;
		dwValBegin = ++dwCursor;

		//--- Retrouve la fin de la valeur ---
		while(1)
			{
			if (dwCursor >= pParser->dwTagEnd) break;
			if (pParser->pFileBuffer[dwCursor] == '"') break;
			dwCursor++;
			}
		if (pParser->pFileBuffer[dwCursor] != '"') break;
		dwValLen = dwCursor-dwValBegin;

		//--- Création de la structure ---

		pxaNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_ATTR)+sizeof(WCHAR)*dwAttrLen+sizeof(WCHAR));
		if (!pxaNew)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(XML_ERROR_FROM_SYSTEM);
			}
		pxaNew->node.type = XML_TYPE_ATTR;
		pxaNew->parent = pxnOwner;
		MultiByteToWideChar(CP_UTF8,0,(char *)&pParser->pFileBuffer[dwAttrBegin],dwAttrLen,pxaNew->name,sizeof(WCHAR)*dwAttrLen);

		if (dwValLen > 0)
			{
			dwBufLen = MultiByteToWideChar(CP_UTF8,0,(char *)&pParser->pFileBuffer[dwValBegin],dwValLen,NULL,0);
			if (!dwBufLen)
				{
				HeapFree(App.hHeap,0,pxaNew);
				return(XML_ERROR_FROM_SYSTEM);
				}
			pxaNew->value = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,dwBufLen*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pxaNew->value)
				{
				HeapFree(App.hHeap,0,pxaNew);
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return(XML_ERROR_FROM_SYSTEM);
				}
			MultiByteToWideChar(CP_UTF8,0,(char *)&pParser->pFileBuffer[dwValBegin],dwValLen,pxaNew->value,dwBufLen);
			}

		List_AddEntry((NODE *)pxaNew,&pxnOwner->attributes);

		dwCursor++;
		}

	return(XML_ERROR_NONE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initie la sauvegarde d'un fichier XML «««««««««««««««««««««««««««»

int xml_SaveFile(WCHAR *pszFilePath)
{
	XML_PARSER*	pParser;

	//--- Création de la structure ---

	pParser = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_PARSER));
	if (!pParser)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);
		return(0);
		}

	//--- Sauvegarde ---

	Status_SetText(Locale_GetText(TEXT_SAVING),pszFilePath);
	pParser->pszFilePath = pszFilePath;
	pParser->iResult = xml_WriteFile(pParser);
	xml_UpdateProgress(-1,-1);

	if (pParser->iResult != XML_ERROR_NONE)
		{
		xml_SendErrorMsg(pParser->iResult,pParser->dwLastErrorMsg);
		xml_FreeParser(pParser);
		return(0);
		}

	xml_FreeParser(pParser);
	return(1);
}


// «»»» Sauvegarde d'un fichier XML «««««««««««««««««««««««««««««««««««««»

int xml_WriteFile(XML_PARSER *pParser)
{
	DWORD		dwLastError;
	DWORD		dwWritten;
	static BYTE	szUTF8[4] = { 0xEF, 0xBB, 0xBF, 0x00 };

	pParser->hFile = CreateFile(pParser->pszFilePath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (pParser->hFile == INVALID_HANDLE_VALUE) goto Error;

	if (!WriteFile(pParser->hFile,szUTF8,3,&dwWritten,NULL)) goto Error;
	if (!WriteFile(pParser->hFile,szXMLFullHeader,strlen(szXMLFullHeader),&dwWritten,NULL)) goto Error;

	pParser->uTabsToAllocate = XML_TABS_BUFFER_LEN;
	pParser->uWriteBufferSize = XML_WRITE_BUFFER_LEN;
	pParser->uNodesTotal = xml_TotalNodesCount((XML_NODE *)App.Game.Save.nodeXMLRoot.next);
	pParser->uNodesCount = 0;

	dwLastError = xml_WriteNodes(pParser,(XML_NODE *)App.Game.Save.nodeXMLRoot.next);
	if (pParser->pszTabsBuffer) HeapFree(App.hHeap,0,pParser->pszTabsBuffer);
	if (pParser->pWriteBuffer) HeapFree(App.hHeap,0,pParser->pWriteBuffer);
	if (dwLastError != XML_ERROR_NONE) goto Error;

	CloseHandle(pParser->hFile);
	return(XML_ERROR_NONE);

Error:	dwLastError = GetLastError();
	if (pParser->hFile != INVALID_HANDLE_VALUE) CloseHandle(pParser->hFile);
	DeleteFile(pParser->pszFilePath);
	SetLastError(dwLastError);
	return(XML_ERROR_FROM_SYSTEM);
}

//--- Ecriture des noeuds ---

DWORD xml_WriteNodes(XML_PARSER *pParser, XML_NODE *pxnRoot)
{
	XML_NODE*	pxnCurrent;

	for (pxnCurrent = pxnRoot; pxnCurrent != NULL; pxnCurrent = (XML_NODE *)pxnCurrent->node.next)
		{

		//--- <TAGBEGIN ... ---

		if (!xml_WriteToBuffer(pParser,"\r\n",2,FALSE)) return(XML_ERROR_FROM_SYSTEM);
		if (!xml_WriteTabs(pParser)) return(XML_ERROR_FROM_SYSTEM);
		if (!xml_WriteToBuffer(pParser,"<",1,FALSE)) return(XML_ERROR_FROM_SYSTEM);
		if (!xml_WriteUTF8(pParser,pxnCurrent->name)) return(XML_ERROR_FROM_SYSTEM);

		//--- ... attr="???" ... ---

		if (pxnCurrent->attributes.next)
			{
			XML_ATTR*	pxnAttr;

			for (pxnAttr = (XML_ATTR *)pxnCurrent->attributes.next; pxnAttr != NULL; pxnAttr = (XML_ATTR *)pxnAttr->node.next)
				{
				if (!xml_WriteToBuffer(pParser," ",1,FALSE)) return(XML_ERROR_FROM_SYSTEM);
				if (!xml_WriteUTF8(pParser,pxnAttr->name)) return(XML_ERROR_FROM_SYSTEM);
				if (!xml_WriteToBuffer(pParser,"=\"",2,FALSE)) return(XML_ERROR_FROM_SYSTEM);
				if (pxnAttr->value) { if (!xml_WriteUTF8(pParser,pxnAttr->value)) return(XML_ERROR_FROM_SYSTEM); }
				if (!xml_WriteToBuffer(pParser,"\"",1,FALSE)) return(XML_ERROR_FROM_SYSTEM);
				}
			}

		//--- ... /TAGEND> or /> ---

		if (pxnCurrent->children.next)
			{
			pParser->uTabsCount++;
			if (!xml_WriteToBuffer(pParser,">",1,TRUE)) return(XML_ERROR_FROM_SYSTEM);
			if (xml_WriteNodes(pParser,(XML_NODE *)pxnCurrent->children.next)) return(XML_ERROR_FROM_SYSTEM);
			pParser->uTabsCount--;
			if (!xml_WriteToBuffer(pParser,"\r\n",2,FALSE)) return(XML_ERROR_FROM_SYSTEM);
			if (!xml_WriteTabs(pParser)) return(XML_ERROR_FROM_SYSTEM);
			if (!xml_WriteToBuffer(pParser,"</",2,FALSE)) return(XML_ERROR_FROM_SYSTEM);
			if (!xml_WriteUTF8(pParser,pxnCurrent->name)) return(XML_ERROR_FROM_SYSTEM);
			}
		else if (!xml_WriteToBuffer(pParser," /",2,FALSE)) return(XML_ERROR_FROM_SYSTEM);
		if (!xml_WriteToBuffer(pParser,">",1,TRUE)) return(XML_ERROR_FROM_SYSTEM);

		pParser->uNodesCount++;
		xml_UpdateProgress(pParser->uNodesCount,pParser->uNodesTotal);
		}

	return(XML_ERROR_NONE);
}

//--- Ecriture des tabulations ---

BOOL xml_WriteTabs(XML_PARSER *pParser)
{
	if (!pParser->uTabsCount)
		return(TRUE);

	while(1)
		{
		if (!pParser->pszTabsBuffer)
			{
			pParser->pszTabsBuffer = HeapAlloc(App.hHeap,0,pParser->uTabsToAllocate);
			if (!pParser->pszTabsBuffer)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return(FALSE);
				}
			FillMemory(pParser->pszTabsBuffer,pParser->uTabsToAllocate,0x09);
			}

		if (pParser->uTabsToAllocate <= pParser->uTabsCount)
			{
			HeapFree(App.hHeap,0,pParser->pszTabsBuffer);
			pParser->uTabsToAllocate *= XML_BUFFER_INCREASE_MULT;
			pParser->pszTabsBuffer = NULL;
			continue;
			}

		break;
		}

	return(xml_WriteToBuffer(pParser,pParser->pszTabsBuffer,pParser->uTabsCount,FALSE));
}

//--- Ecriture d'une chaîne WCHAR en UTF-8 ---

BOOL xml_WriteUTF8(XML_PARSER *pParser, WCHAR *pszText)
{
	LPSTR	pszBuffer;
	int	iSize;

	iSize = WideCharToMultiByte(CP_UTF8,0,pszText,-1,NULL,0,NULL,NULL);
	if (!iSize) return(FALSE);

	pszBuffer = HeapAlloc(App.hHeap,0,iSize);
	if (!pszBuffer)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(FALSE);
		}

	if (!WideCharToMultiByte(CP_UTF8,0,pszText,-1,pszBuffer,iSize,NULL,NULL)) return(FALSE);

	if (iSize-1 > 0)
		{
		if (!xml_WriteToBuffer(pParser,pszBuffer,iSize-1,FALSE))
			{
			HeapFree(App.hHeap,0,pszBuffer);
			return(FALSE);
			}
		}

	HeapFree(App.hHeap,0,pszBuffer);
	return(TRUE);
}

//--- Ecriture des données dans un tampon ---

BOOL xml_WriteToBuffer(XML_PARSER *pParser, void *pBuffer, UINT uBufferSize, BOOL bPurge)
{
	DWORD	dwWritten;

	while(1)
		{
		if (!pParser->pWriteBuffer)
			{
			pParser->uWriteBufferCursor = 0;
			pParser->pWriteBuffer = HeapAlloc(App.hHeap,0,pParser->uWriteBufferSize);
			if (!pParser->pWriteBuffer)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return(FALSE);
				}
			}
		if (pParser->uWriteBufferCursor+uBufferSize > pParser->uWriteBufferSize)
			{
			void*	pTemp;

			pParser->uWriteBufferSize *= XML_BUFFER_INCREASE_MULT;
			pTemp = HeapAlloc(App.hHeap,0,pParser->uWriteBufferSize);
			if (!pTemp)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				return(FALSE);
				}
			CopyMemory(pTemp,pParser->pWriteBuffer,pParser->uWriteBufferCursor);
			HeapFree(App.hHeap,0,pParser->pWriteBuffer);
			pParser->pWriteBuffer = pTemp;
			continue;
			}

		CopyMemory(pParser->pWriteBuffer+pParser->uWriteBufferCursor,pBuffer,uBufferSize);
		pParser->uWriteBufferCursor += uBufferSize;

		if (bPurge)
			{
			if (!WriteFile(pParser->hFile,pParser->pWriteBuffer,pParser->uWriteBufferCursor,&dwWritten,NULL)) return(FALSE);
			pParser->uWriteBufferCursor = 0;
			}

		break;
		}

	return(TRUE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Libération des structures					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Libération de la structure de chargement/sauvegarde «««««««««««««»

void xml_FreeParser(XML_PARSER *pParser)
{
	if (!pParser) return;
	HeapFree(App.hHeap,0,pParser);
	return;
}


// «»»» Libération des noeuds XML «««««««««««««««««««««««««««««««««««««««»

void xml_ReleaseAll(NODE *pxnRoot)
{
	XML_NODE*	pxnCurrent;

	while ((pxnCurrent = (XML_NODE *)pxnRoot->next) != NULL) xml_ReleaseNode(pxnCurrent);
	return;
}

//--- Libère un noeud ---

void xml_ReleaseNode(XML_NODE *pxn)
{
	XML_ATTR*	pxa;

	if (!pxn) return;
	List_RemEntry((NODE *)pxn);
	while ((pxa = (XML_ATTR *)pxn->attributes.next) != NULL) xml_ReleaseAttr(pxa);
	if (pxn->children.next) xml_ReleaseAll(&pxn->children);
	HeapFree(App.hHeap,0,pxn);
	return;
}

//--- Libère un attribut ---

void xml_ReleaseAttr(XML_ATTR *pxa)
{
	List_RemEntry((NODE *)pxa);
	if (pxa->value) HeapFree(App.hHeap,0,pxa->value);
	HeapFree(App.hHeap,0,pxa);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Manipulation des structures					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Retrouve une entrée par un chemin «««««««««««««««««««««««««««««««»

// Le chemin est de la forme WCHAR*,WCHAR*,...,NULL
// Le premier WCHAR* est le nom du noeud
// Le second WCHAR* est la valeur de l'attribut id du noeud (peut être nul)

XML_NODE* xml_GetNodeFromPath(XML_NODE *pxnRoot, WCHAR *pszPath[])
{
	XML_NODE*	pxnResult;
	WCHAR*		pszNodeName;
	WCHAR*		pszNodeId;
	int		i;

	if (!pxnRoot) return(NULL);
	if (!pszPath) return(NULL);
	if (!pszPath[0]) return(NULL);

	for (pxnResult = pxnRoot, i = 0;;)
		{
		pszNodeName = pszPath[i++];
		pszNodeId = pszPath[i++];
		if (pszNodeId) pxnResult = xml_GetNode(pxnResult,pszNodeName,szXMLid,pszNodeId);
		else pxnResult = xml_GetNode(pxnResult,pszNodeName,NULL,NULL);
		if (!pxnResult) break;
		if (pszPath[i] == NULL) break;
		pxnResult = (XML_NODE *)pxnResult->children.next;
		}

	return(pxnResult);
}


// «»»» Retrouve le premier enfant d'une entrée à la fin d'un chemin ««««»

XML_NODE* xml_GetNodeFromPathFirstChild(XML_NODE *pxnRoot, WCHAR *pszPath[])
{
	XML_NODE*	pxnResult;

	pxnResult = xml_GetNodeFromPath(pxnRoot,pszPath);
	if (!pxnResult) return(NULL);
	return((XML_NODE *)pxnResult->children.next);
}


// «»»» Retrouve une entrée par son nom et un attribut ««««««««««««««««««»

// La fonction ne recherche que la branche actuelle, elle ne parcourt pas les enfants
// > pszAttrName peut être nul (pas de recherche par attribut)
// > pszAttrValue peut être nulle (l'attribut n'a pas de valeur)

XML_NODE* xml_GetNode(XML_NODE *pxnRoot, WCHAR *pszNodeName, WCHAR *pszAttrName, WCHAR *pszAttrValue)
{
	XML_NODE*	pxn;
	XML_ATTR*	pxa;

	for (pxn = pxnRoot; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
		{
		if (wcscmp(pxn->name,pszNodeName)) continue;
		if (!pszAttrName) break;
		pxa = xml_GetAttr(pxn,pszAttrName);
		if (!pxa) continue;
		if (!pszAttrValue && !pxa->value) break;
		if (!pxa->value) continue;
		if (wcscmp(pxa->value,pszAttrValue)) continue;
		break;
		}

	return(pxn);
}

// «»»» Retrouve une valeur par son nom «««««««««««««««««««««««««««««««««»

XML_ATTR* xml_GetAttr(XML_NODE *pxn, WCHAR *pszAttrName)
{
	XML_ATTR*	pxa;

	if (!pxn) return(NULL);
	if (!pxn->attributes.next) return(NULL);
	if (!pszAttrName) return(NULL);

	for (pxa = (XML_ATTR *)pxn->attributes.next; pxa != NULL; pxa = (XML_ATTR *)pxa->node.next)
		{
		if (wcscmp(pxa->name,pszAttrName)) continue;
		return(pxa);
		}

	return(NULL);
}


// «»»» Retrouve la valeur d'une entrée «««««««««««««««««««««««««««««««««»

XML_ATTR* xml_GetXMLValueAttr(XML_NODE *pxnRoot, WCHAR *pszNodeName, WCHAR *pszAttrName, WCHAR *pszAttrValue)
{
	XML_NODE*	pxn;

	pxn = xml_GetNode(pxnRoot,pszNodeName,pszAttrName,pszAttrValue);
	if (!pxn) return(NULL);
	return(xml_GetAttr(pxn,szXMLvalue));
}


// «»»» Retrouve une valeur par son nom «««««««««««««««««««««««««««««««««»

WCHAR* xml_GetAttrValue(XML_NODE *pxn, WCHAR *pszAttrName)
{
	XML_ATTR*	pxa;

	if (!pxn->attributes.next) return(NULL);
	if (!pszAttrName) return(NULL);

	for (pxa = (XML_ATTR *)pxn->attributes.next; pxa != NULL; pxa = (XML_ATTR *)pxa->node.next)
		{
		if (wcscmp(pxa->name,pszAttrName)) continue;
		return(pxa->value);
		}

	return(NULL);
}


// «»»» Retourne la valeur d'un attribut ««««««««««««««««««««««««««««««««»

// /!\ Returned pointer may be NULL

WCHAR* xml_GetThisAttrValue(XML_ATTR *pxa)
{
	if (!pxa) return(NULL);
	return(pxa->value);
}


// «»»» Détermine si une valeur est "vraie" «««««««««««««««««««««««««««««»

BOOL xml_IsTrue(XML_ATTR *pxa)
{
	if (!pxa) return(FALSE);
	if (!pxa->value) return(FALSE);
	return(wcscmp(pxa->value,L"True")?FALSE:TRUE);
}


// «»»» Calcul le nombre total de noeuds ««««««««««««««««««««««««««««««««»

UINT xml_TotalNodesCount(XML_NODE *pxnRoot)
{
	XML_NODE*	pxn;
	UINT		uTotal;

	for (uTotal = 0, pxn = pxnRoot; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
		{
		uTotal++;
		if (!pxn->children.next) continue;
		uTotal += xml_TotalNodesCount((XML_NODE *)pxn->children.next);
		}

	return(uTotal);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions de création et de conversion				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'une balise «««««««««««««««««««««««««««««««««««««««««««»

//     ... : attribute, value (value may be NULL)
// numAttrs: number of attributes

XML_NODE* xml_CreateNode(WCHAR *pszName, XML_NODE *pxnParent, UINT uNumAttrs, ...)
{
	XML_NODE*	pxnNew;
	XML_ATTR*	pxaNew;
	WCHAR*		pszAttr;
	WCHAR*		pszValue;
	va_list		vl;

	va_start(vl,uNumAttrs);

	pxnNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_NODE)+wcslen(pszName)*sizeof(WCHAR)+sizeof(WCHAR));
	if (pxnNew)
		{
		pxnNew->node.type = XML_TYPE_NODE;
		pxnNew->parent = pxnParent;
		wcscpy(pxnNew->name,pszName);
		while(uNumAttrs)
			{
			pszAttr = va_arg(vl,WCHAR *);
			pxaNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_ATTR)+wcslen(pszAttr)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pxaNew)
				{
				xml_ReleaseNode(pxnNew);
				pxnNew = NULL;
				break;
				}
			pxaNew->node.type = XML_TYPE_ATTR;
			pxaNew->parent = pxnNew;
			wcscpy(pxaNew->name,pszAttr);
			pszValue = va_arg(vl,WCHAR *);
			if (pszValue)
				{
				pxaNew->value = HeapAlloc(App.hHeap,0,wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
				if (!pxaNew->value)
					{
					HeapFree(App.hHeap,0,pxaNew);
					xml_ReleaseNode(pxnNew);
					pxnNew = NULL;
					break;
					}
				wcscpy(pxaNew->value,pszValue);
				}
			List_AddEntry((NODE *)pxaNew,&pxnNew->attributes);
			uNumAttrs--;
			}
		}

	va_end(vl);
	return(pxnNew);
}


// «»»» Création d'une série de balises «««««««««««««««««««««««««««««««««»

// Les balises sont définies comme ceci: WCHAR* pArray[] = { node, (attribute, value, ...), 0|1|-1|-2 }
//        node: nom de la balise
//   attribute: nom de l'attribut (facultatif)
//       value: valeur de l'attribut (facultatif)
//           0: la prochaine balise suit la précédente
//           1: la prochaine balise est enfant de la précédente
//          -1: revient à la balise parente ou fin de la chaîne s'il n'y en a pas
//          -2: fin de la chaîne
// La fonction reçoit un pointeur sur le pointeur de la chaîne (WCHAR** pArrayPtr = pArray)
// xml_CreateNode(&pArrayPtr,pxnParent)

XML_NODE* xml_CreateNodeArray(WCHAR ***pArray, XML_NODE *pxnParent)
{
	NODE		nodeRoot;
	XML_NODE*	pxnNew;
	XML_ATTR*	pxaNew;
	XML_NODE*	pxnChild;

	ZeroMemory(&nodeRoot,sizeof(NODE));

	if (pArray)
		{
		while(**pArray != (WCHAR *)-2)
			{
			pxnNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_NODE)+wcslen(**pArray)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pxnNew)
				{
				xml_ReleaseNode((XML_NODE *)nodeRoot.next);
				return(NULL);
				}
			List_AddEntry((NODE *)pxnNew,&nodeRoot);
			pxnNew->node.type = XML_TYPE_NODE;
			pxnNew->parent = pxnParent;
			wcscpy(pxnNew->name,**pArray);
			(*pArray)++;
			if (**pArray != (WCHAR *)0 && **pArray != (WCHAR *)1 && **pArray != (WCHAR *)-1)
				{
				while(**pArray != (WCHAR *)-2)
					{
					pxaNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(XML_ATTR)+wcslen(**pArray)*sizeof(WCHAR)+sizeof(WCHAR));
					if (!pxaNew)
						{
						xml_ReleaseNode((XML_NODE *)nodeRoot.next);
						return(NULL);
						}
					pxaNew->node.type = XML_TYPE_ATTR;
					pxaNew->parent = pxnNew;
					wcscpy(pxaNew->name,**pArray);
					(*pArray)++;
					List_AddEntry((NODE *)pxaNew,&pxnNew->attributes);
					if (**pArray == (WCHAR *)-2) break;
					if (**pArray == (WCHAR *)-1) break;
					if (**pArray == (WCHAR *)0) break;
					if (**pArray == (WCHAR *)1) break;
					pxaNew->value = HeapAlloc(App.hHeap,0,wcslen(**pArray)*sizeof(WCHAR)+sizeof(WCHAR));
					if (!pxaNew->value)
						{
						xml_ReleaseNode((XML_NODE *)nodeRoot.next);
						return(NULL);
						}
					wcscpy(pxaNew->value,**pArray);
					(*pArray)++;
					if (**pArray == (WCHAR *)-1) break;
					if (**pArray == (WCHAR *)0) break;
					if (**pArray == (WCHAR *)1) break;
					}
				}
			if (**pArray == (WCHAR *)-1)
				{
				(*pArray)++;
				return((XML_NODE *)nodeRoot.next);
				}
			if (**pArray == (WCHAR *)0)
				{
				(*pArray)++;
				continue;
				}
			if (**pArray == (WCHAR *)1)
				{
				(*pArray)++;
				pxnChild = xml_CreateNodeArray(pArray,pxnNew);
				if (!pxnChild)
					{
					xml_ReleaseNode((XML_NODE *)nodeRoot.next);
					return(NULL);
					}
				List_Append((NODE *)pxnChild,&pxnNew->children);
				}
			}
		}

	return((XML_NODE *)nodeRoot.next);
}


// «»»» Modifie la valeur d'un attribut «««««««««««««««««««««««««««««««««»

//--- Valeur textuelle ---

BOOL xml_SetAttrValue(XML_ATTR *pxa, WCHAR *pszValue)
{
	if (pxa)
		{
		WCHAR*	pszBuffer = NULL;

		if (pszValue)
			{
			pszBuffer = HeapAlloc(App.hHeap,0,wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pszBuffer) return(FALSE);
			wcscpy(pszBuffer,pszValue);
			}

		if (pxa->value) HeapFree(App.hHeap,0,pxa->value);
		pxa->value = pszBuffer;
		}

	return(TRUE);
}

//--- Valeur numérique ---

BOOL xml_SetAttrValueNumber(XML_ATTR *pxa, UINT uValue)
{
	BOOL	bResult = TRUE;

	if (pxa)
		{
		WCHAR*	pszBuffer;
		UINT	uLen;

		uLen = Misc_uIntToWCHAR(NULL,uValue);
		if (!uLen) return(xml_SetAttrValue(pxa,NULL));
		pszBuffer = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pszBuffer) return(FALSE);
		Misc_uIntToWCHAR(pszBuffer,uValue);
		bResult = xml_SetAttrValue(pxa,pszBuffer);
		HeapFree(App.hHeap,0,pszBuffer);
		}

	return(bResult);
}


// «»»» Gestion du chemin sous forme de texte «««««««««««««««««««««««««««»

WCHAR* xml_BuildWideCharPath(XML_NODE *pxn)
{
	WCHAR*		pszBuffer;
	WCHAR*		pszText;
	XML_NODE*	pxnCurrent;

	pszBuffer = NULL;
	pszText = NULL;
	pxnCurrent = pxn;

	do {

		pszText = xml_AttrToWideChar(pxnCurrent);
		if (!pszText)
			{
			if (pszBuffer) LocalFree(pszBuffer);
			return(NULL);
			}

		if (pszBuffer)
			{
			WCHAR*	pszTemp = LocalAlloc(LMEM_FIXED,wcslen(pszBuffer)*sizeof(WCHAR)+wcslen(szXMLSeparator)*sizeof(WCHAR)+wcslen(pszText)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pszTemp)
				{
				LocalFree(pszBuffer);
				return(NULL);
				}
			wcscpy(pszTemp,pszText);
			wcscat(pszTemp,szXMLSeparator);
			wcscat(pszTemp,pszBuffer);
			LocalFree(pszText);
			LocalFree(pszBuffer);
			pszText = pszTemp;
			}

		pszBuffer = pszText;
		pxnCurrent = (XML_NODE *)pxnCurrent->parent;

	} while (pxnCurrent);

	return(pszBuffer);
}

//--- Attribut sous forme de texte ---

WCHAR* xml_AttrToWideChar(XML_NODE *pxn)
{
	DWORD_PTR	vl[2];
	WCHAR*		pszText;

	vl[0] = (DWORD_PTR)pxn->name;
	vl[1] = (DWORD_PTR)xml_GetAttrValue(pxn,szXMLid);
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,vl[1]?szXMLAttrId:szXMLAttr,0,0,(WCHAR *)&pszText,1,(va_list *)vl)) return(NULL);
	return(pszText);
}

//--- Valeur sous forme de texte ---

WCHAR* xml_ValueToWideChar(XML_ATTR *pxa)
{
	DWORD_PTR	vl[2];
	WCHAR*		pszText;

	vl[0] = (DWORD_PTR)pxa->name;
	vl[1] = (DWORD_PTR)(pxa->value?pxa->value:szXMLEmpty);
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szXMLValue,0,0,(WCHAR *)&pszText,1,(va_list *)vl)) return(NULL);
	return(pszText);
}

//--- Ajoute une valeur à un chemin ---

WCHAR* xml_AppendWideCharValue(XML_ATTR *pxa, WCHAR *pszText)
{
	WCHAR*		pszValue;
	WCHAR*		pszTemp;

	pszValue = xml_ValueToWideChar(pxa);
	if (!pszValue) return(NULL);
	pszTemp = LocalAlloc(LMEM_FIXED,wcslen(pszText)*sizeof(WCHAR)+wcslen(szXMLSeparator)*sizeof(WCHAR)+wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszTemp)
		{
		LocalFree(pszValue);
		return(NULL);
		}
	wcscpy(pszTemp,pszText);
	wcscat(pszTemp,szXMLSeparator);
	wcscat(pszTemp,pszValue);
	LocalFree(pszValue);
	LocalFree(pszText);
	return(pszTemp);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions diverses						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Envoie un message d'erreur à la fenêtre principale ««««««««««««««»

void xml_SendErrorMsg(UINT uLastErrorType, UINT uLastErrorMsg)
{
	WCHAR*		pszMsg;
	DWORD_PTR	vl;

	pszMsg = NULL;
	vl = (DWORD_PTR)Locale_GetText(uLastErrorMsg);

	if (uLastErrorType == XML_ERROR_FROM_SYSTEM)
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,GetLastError(),LOCALE_USER_DEFAULT,(WCHAR *)&pszMsg,1,NULL);
	else
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,L"%1",0,0,(WCHAR *)&pszMsg,1,(va_list *)&vl);

	if (pszMsg) SendMessage(App.hWnd,WM_MESSAGEBOX,(WPARAM)MB_ICONHAND,(LPARAM)pszMsg);
	return;
}


// «»»» Mise-à-jour de la barre de progression ««««««««««««««««««««««««««»

void xml_UpdateProgress(UINT uCurrent, UINT uMax)
{
	if (uCurrent == -1 && uMax == -1) SendMessage(App.hWnd,WM_UPDATEPROGRESS,-1,-1);
	if ((UINT)(((float)uCurrent/(float)uMax)*100.0f) != App.uProgression) SendMessage(App.hWnd,WM_UPDATEPROGRESS,uCurrent,uMax);
	return;
}

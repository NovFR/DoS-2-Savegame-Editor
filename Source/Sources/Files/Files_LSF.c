
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des fichiers LSF
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
#include "Texts.h"
#include "Utils.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Conversion d'un fichier LSF en XML ««««««««««««««««««««««««««««««»

BOOL lsf_Unpack(HWND hWnd, LSFILE *pFile, DWORD dwMode)
{
	LSFREADER*	pReader;
	BOOL		bIsDone = FALSE;

	if (!pFile) return(FALSE);

	//--- Alloue la structure ---

	pReader = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LSFREADER));
	if (!pReader)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	//--- Vérifie qu'il s'agit bien d'un fichier LSF ---

	if (!(pFile->dwType&LS_TYPE_LSF))
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSF_UNKNOWN;
		goto Done;
		}

	//--- Valeurs de base ---

	pReader->pHeader = (LSFHEADER *)pFile->pData;
	pReader->pBuffer = pFile->pData+sizeof(LSFHEADER);

	//--- Unpack names --

	if (!lsf_UnpackList(pReader,pReader->pHeader->StringsSizeOnDisk,pReader->pHeader->StringsUncompressedSize,LSF_UNPACK_NAMES)) goto Done;
	if (!lsf_UnpackList(pReader,pReader->pHeader->NodesSizeOnDisk,pReader->pHeader->NodesUncompressedSize,LSF_UNPACK_NODES)) goto Done;
	if (!lsf_UnpackList(pReader,pReader->pHeader->AttributesSizeOnDisk,pReader->pHeader->AttributesUncompressedSize,LSF_UNPACK_ATTRIBUTES)) goto Done;
	if (!lsf_UnpackList(pReader,pReader->pHeader->ValuesSizeOnDisk,pReader->pHeader->ValuesUncompressedSize,LSF_UNPACK_VALUES)) goto Done;
	if (!lsf_Create(pReader,pFile)) goto Done;

	SetLastError(ERROR_SUCCESS);
	bIsDone = TRUE;

	//--- Terminé ! ---

Done:	if (GetLastError() != ERROR_SUCCESS && !(dwMode&LS_LOAD_QUIET)) Request_PrintErrorEx(hWnd,Locale_GetText(TEXT_ERR_LSF_LOAD),NULL,MB_ICONHAND,pFile->pszName);

	if (pReader)
		{
		if (pReader->uLastError && !(dwMode&LS_LOAD_QUIET)) Request_MessageBoxEx(hWnd,Locale_GetText(TEXT_ERR_LSF_LOADEX),NULL,MB_ICONHAND,pFile->pszName,Locale_GetText(pReader->uLastError));
		lsf_ReleaseNames(&pReader->nodeNames);
		lsf_ReleaseNodes(&pReader->nodeNodes);
		lsf_ReleaseAttrs(&pReader->nodeAttrs);
		lsf_ReleaseValues(pReader->pValues);
		if (!bIsDone) xml_ReleaseAll(&pFile->nodeXMLRoot);
		HeapFree(App.hHeap,0,pReader);
		}

	return(bIsDone);
}


// «»»» Création de la structure XML ««««««««««««««««««««««««««««««««««««»

BOOL lsf_Create(LSFREADER *pReader, LSFILE *pFile)
{
	LFSNODES*	pNodes;
	XML_NODE*	pxnFirst;
	XML_NODE*	pxnNew;
	static WCHAR*	pszEngineVersions[] = { L"major", L"minor", L"revision", L"build" };
	int		i;

	pNodes = (LFSNODES *)pReader->nodeNodes.next;
	if (pNodes->nodeInfos[0].ParentIndex != -1)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_LSF_NOXMLROOT; // Very unlikely
		return(FALSE);
		}

	//--- Création des balises de base ---

	pReader->uLastError = 0;

	pxnFirst = xml_CreateNode(L"save",NULL,0);
	if (!pxnFirst)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(FALSE);
		}
	List_AddEntry((NODE *)pxnFirst,&pFile->nodeXMLRoot);

	pxnNew = xml_CreateNode(L"header",pxnFirst,2,L"version",L"2",L"time",L"0");
	if (!pxnNew)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(FALSE);
		}
	List_AddEntry((NODE *)pxnNew,&pxnFirst->children);

	pxnNew = xml_CreateNode(L"version",pxnFirst,0);
	if (!pxnNew)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(FALSE);
		}
	List_AddEntry((NODE *)pxnNew,&pxnFirst->children);

	for (i = 0; i != 4; i++)
		{
		XML_ATTR*	pxa;
		UINT		uValue;

		pxa = xml_CreateAttr(pszEngineVersions[i],pxnNew);
		if (!pxa)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(FALSE);
			}

		switch(i)
			{
			case 0:	uValue = (pReader->pHeader->EngineVersion&0xFF000000)>>24;
				break;
			case 1:	uValue = (pReader->pHeader->EngineVersion&0x00FF0000)>>16;
				break;
			case 2:	uValue = (pReader->pHeader->EngineVersion&0x0000FF00)>>8;
				break;
			case 3:	uValue = (pReader->pHeader->EngineVersion&0x000000FF);
				break;
			}
		if (!xml_SetAttrValueNumber(pxa,uValue))
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(FALSE);
			}
		}

	//--- Création des autres balises ---

	pReader->uIndex = 0;

	while (pReader->uIndex < pNodes->uNumNodes)
		{
		if (!lsf_CreateNode(pReader,-1,pxnFirst,&pxnFirst->children)) return(FALSE);
		}

	return(TRUE);
}

//--- Création d'un noeud ---

BOOL lsf_CreateNode(LSFREADER *pReader, UINT uParentIndex, XML_NODE *pxnParent, NODE *pRoot)
{
	LFSNODES*	pNodes;
	XML_NODE*	pxnNew;
	WCHAR*		pszName;
	UINT		uThisIndex;

	pNodes = (LFSNODES *)pReader->nodeNodes.next;
	uThisIndex = pReader->uIndex;

	//--- Get name
	pszName = lsf_GetName(pReader,pNodes->nodeInfos[uThisIndex].NameIndex,pNodes->nodeInfos[uThisIndex].NameOffset);
	if (!pszName)
		{
		SetLastError(ERROR_SUCCESS);
		pReader->uLastError = TEXT_ERR_XML_TAG_EMPTY;
		return(FALSE);
		}

	//--- Create XML region structure
	if (pNodes->nodeInfos[uThisIndex].ParentIndex == -1)
		{
		pxnParent = xml_CreateNode(szXMLregion,pxnParent,1,szXMLid,pszName);
		if (!pxnParent) goto No_Memory;
		List_AddEntry((NODE *)pxnParent,pRoot);
		pRoot = &pxnParent->children;
		}

	//--- Create XML node structure
	pxnNew = xml_CreateNode(szXMLnode,pxnParent,1,szXMLid,pszName);
	if (!pxnNew) goto No_Memory;

	List_AddEntry((NODE *)pxnNew,pRoot);

	//--- Create XML attributes structures
	if (pNodes->nodeInfos[uThisIndex].FirstAttributeIndex != -1)
		{
		XML_NODE*	pxnAttr;
		XML_ATTR*	pxaNew;
		LFSATTRS*	pAttrs;
		UINT		uAttrIndex;

		uAttrIndex = pNodes->nodeInfos[uThisIndex].FirstAttributeIndex;
		pAttrs = (LFSATTRS *)pReader->nodeAttrs.next;
		while (uAttrIndex != -1)
			{
			pszName = lsf_GetName(pReader,pAttrs->attrInfos[uAttrIndex].NameIndex,pAttrs->attrInfos[uAttrIndex].NameOffset);
			if (!pszName)
				{
				SetLastError(ERROR_SUCCESS);
				pReader->uLastError = TEXT_ERR_XML_ATTR_EMPTY;
				return(FALSE);
				}

			pxnAttr = xml_CreateNode(szXMLattribute,pxnNew,1,szXMLid,pszName);
			if (!pxnAttr) goto No_Memory;
			List_AddEntry((NODE *)pxnAttr,&pxnNew->children);

			pxaNew = xml_CreateAttr(szXMLvalue,pxnAttr);
			if (!pxaNew) goto No_Memory;
			pxaNew->value = lsf_ReadAttribute(pAttrs->attrInfos[uAttrIndex].TypeId,pReader->pValues+pAttrs->attrInfos[uAttrIndex].DataOffset,pAttrs->attrInfos[uAttrIndex].Length,&pReader->uLastError);
			if (pReader->uLastError) return(FALSE);

			pxaNew = xml_CreateAttr(szXMLtype,pxnAttr);
			if (!pxaNew) goto No_Memory;
			if (!xml_SetAttrValueNumber(pxaNew,pAttrs->attrInfos[uAttrIndex].TypeId)) goto No_Memory;

			// handle="[handle]" for Translated Strings
			if (pAttrs->attrInfos[uAttrIndex].TypeId == DT_TranslatedString)
				{
				UINT	uLength;
				void*	pValue;

				pxaNew = xml_CreateAttr(L"handle",pxnAttr);
				if (!pxaNew) goto No_Memory;

				pValue = pReader->pValues+pAttrs->attrInfos[uAttrIndex].DataOffset;
				uLength = pAttrs->attrInfos[uAttrIndex].Length;

				uLength = *(DWORD *)pValue;
				pValue += 4;
				pValue += uLength;
				uLength = *(DWORD *)pValue;
				pValue += 4;
				pxaNew->value = Misc_UTF8ToWideCharNZ((char *)pValue,uLength);
				if (!pxaNew->value) goto No_Memory;
				}

			uAttrIndex = pAttrs->attrInfos[uAttrIndex].NextAttributeIndex;
			}
		}

	//--- Stop if all nodes have been parsed
	pReader->uIndex++;
	if (pReader->uIndex >= pNodes->uNumNodes) return(TRUE);

	//--- Add a <children> tag for child nodes
	if (pNodes->nodeInfos[pReader->uIndex].ParentIndex == uThisIndex)
		{
		XML_NODE *pxnTmp;
		pxnTmp = pxnNew;
		pxnNew = xml_CreateNode(L"children",pxnParent,0);
		if (!pxnNew) goto No_Memory;
		List_AddEntry((NODE *)pxnNew,&pxnTmp->children);
		}

	//--- Add child nodes
	while (pReader->uIndex < pNodes->uNumNodes)
		{
		if (pNodes->nodeInfos[pReader->uIndex].ParentIndex != uThisIndex) break;
		if (!lsf_CreateNode(pReader,uThisIndex,pxnNew,&pxnNew->children)) return(FALSE);
		}

	return(TRUE);

No_Memory:
	SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	pReader->uLastError = 0;
	return(FALSE);
}

//--- Retrouve un nom ---

WCHAR* lsf_GetName(LSFREADER *pReader, UINT uNameIndex, UINT uNameOffset)
{
	LFSNAMES*	pNames;
	UINT		uIndex;

	for (pNames = (LFSNAMES *)pReader->nodeNames.next, uIndex = 0; pNames != NULL; pNames = (LFSNAMES *)pNames->node.next, uIndex++)
		{
		if (uIndex == uNameIndex)
			{
			if (uNameOffset >= pNames->uNumStrings) return(NULL);
			return(pNames->pszNames[uNameOffset]);
			}
		}

	return(NULL);
}

//--- Lecture d'un attribut ---

//!\ Partial implementation, should be ok for meta.lsf

WCHAR* lsf_ReadAttribute(UINT uTypeId, void *pValue, UINT uLength, UINT *uError)
{
	WCHAR*	pszResult = NULL;
	WCHAR	szBuffer[41];

	*uError = 0;

	switch(uTypeId)
		{
		case DT_None:
			break;
		case DT_Byte:
			swprintf(szBuffer,40,L"%u",(unsigned int)*(unsigned char *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_Short:
			swprintf(szBuffer,40,L"%hi",*(short int *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_UShort:
			swprintf(szBuffer,40,L"%hu",*(unsigned short int *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_Int:
			swprintf(szBuffer,40,L"%i",*(int *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_Int8:
			swprintf(szBuffer,40,L"%i",(int)*(char *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_UInt:
			swprintf(szBuffer,40,L"%u",*(unsigned int *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_Float:
			swprintf(szBuffer,40,L"%f",*(float *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
		case DT_Double:
			swprintf(szBuffer,40,L"%f",*(double *)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
//		case DT_Vec3:
//			break;
//		case DT_Vec4:
//			break;
		case DT_Bool:
			pszResult = Misc_StrCpyAlloc(*(unsigned char *)pValue?L"True":L"False");
			break;
		case DT_ULongLong:
			swprintf(szBuffer,40,L"%llu",*(unsigned long long int*)pValue);
			pszResult = Misc_StrCpyAlloc(szBuffer);
			break;
//		case DT_ScratchBuffer:
//			break;
		case DT_String:
		case DT_Path:
		case DT_FixedString:
		case DT_LSString:
		case DT_WString:
		case DT_LSWString:
			pszResult = Misc_UTF8ToWideCharNZ((char *)pValue,uLength);
			break;
		case DT_TranslatedString:
			uLength = *(DWORD *)pValue;
			pValue += 4;
			pszResult = Misc_UTF8ToWideCharNZ((char *)pValue,uLength);
			break;
//		case DT_UUID:
//			break;
//		case DT_TranslatedFSString:
//			break;
		default:*uError = TEXT_ERR_LSF_ATTRIBUTE;
			break;
		}

	if (!*uError && !pszResult) *uError = TEXT_ERR_NOMEMORY;
	return(pszResult);
}


// «»»» Récupère les données XML ««««««««««««««««««««««««««««««««««««««««»

BOOL lsf_UnpackList(LSFREADER *pReader, DWORD dwSizeOnDisk, DWORD dwUncompressedSize, DWORD dwType)
{
	BYTE*		pData;
	BYTE*		pWork;
	BOOL		bIsCompressed;
	UINT		dwSize;
	BOOL		bIsDone;

	bIsDone = FALSE;
	bIsCompressed = ((pReader->pHeader->CompressionFlags&COMPRESSION_METHOD) != COMPRESSION_METHOD_NONE);
	dwSize = bIsCompressed?dwUncompressedSize:dwSizeOnDisk;

	if (dwSize == 0) return(TRUE);

	pData = HeapAlloc(App.hHeap,0,dwSize);
	if (!pData)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	//--- Unpack or copy tables --

	if (bIsCompressed)
		{
		pReader->uLastError = lsa_Decompress(pReader->pHeader->CompressionFlags,pReader->pBuffer,dwSizeOnDisk,pData,dwUncompressedSize,dwType == LSF_UNPACK_NAMES?0:pReader->pHeader->Version);
		if (pReader->uLastError)
			{
			SetLastError(ERROR_SUCCESS);
			goto Done;
			}
		}
	else CopyMemory(pData,pReader->pBuffer,dwSize);

	pReader->pBuffer += dwSizeOnDisk;

	//--- Create "hash" tables ---

	switch(dwType)
		{

		//--- NAMES
		case LSF_UNPACK_NAMES: {
			UINT	uNumEntries;
			UINT	uNumStrings;
			UINT	uNameLen;

			pWork = pData;
			uNumEntries = *(DWORD *)pWork;
			pWork += 4;
			while (uNumEntries--)
				{
				LFSNAMES*	pNames;
				UINT		uIndex;

				uNumStrings = *(WORD *)pWork;
				uIndex = 0;
				pWork += 2;

				// [OPTIMIZATION (PROBABLY) NEEDED] Use of a table instead of a list
				pNames = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LFSNAMES)+uNumStrings*sizeof(WCHAR *));
				if (!pNames)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					goto Done;
					}
				List_AddEntry((NODE *)pNames,&pReader->nodeNames);
				pNames->uNumStrings = uNumStrings;

				while (uNumStrings--)
					{
					uNameLen = *(WORD *)pWork;
					pWork += 2;
					if (uNameLen)
						{
						pNames->pszNames[uIndex] = Misc_UTF8ToWideCharNZ((char *)pWork,uNameLen);
						if (!pNames->pszNames[uIndex]) goto Done;
						}
					uIndex++;
					pWork += uNameLen;
					}
				}

			} break;

		//--- NODES
		case LSF_UNPACK_NODES: {
			LFSNODES*	pNodes;
			BYTE*		pBound;
			BOOL		bLongNodes;
			UINT		uNumNodes;

			bLongNodes = pReader->pHeader->Version >= 3 && pReader->pHeader->Extended == 1;

			pWork = pBound = pData;
			pBound += dwSize;
			uNumNodes = 0;
			while (pWork < pBound)
				{
				uNumNodes++;
				if (bLongNodes) pWork += sizeof(NODEENTRYV3);
				else pWork += sizeof(NODEENTRYV2);
				}

			pNodes = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LFSNODES)+sizeof(LFSNODEINFO)*uNumNodes);
			if (!pNodes)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				goto Done;
				}
			pNodes->uNumNodes = uNumNodes;
			List_AddEntry((NODE *)pNodes,&pReader->nodeNodes);

			pWork = pBound = pData;
			pBound += dwSize;
			uNumNodes = 0;
			while (pWork < pBound)
				{
				if (bLongNodes)
					{
					NODEENTRYV3 *pNodeV3 = (NODEENTRYV3 *)pWork;
					pNodes->nodeInfos[uNumNodes].ParentIndex = pNodeV3->ParentIndex;
					pNodes->nodeInfos[uNumNodes].NameIndex = (int)(pNodeV3->NameHashTableIndex >> 16);
					pNodes->nodeInfos[uNumNodes].NameOffset = (int)(pNodeV3->NameHashTableIndex & 0xffff);
					pNodes->nodeInfos[uNumNodes].FirstAttributeIndex = pNodeV3->FirstAttributeIndex;
					pWork += sizeof(NODEENTRYV3);
					}
				else
					{
					NODEENTRYV2 *pNodeV2 = (NODEENTRYV2 *)pWork;
					pNodes->nodeInfos[uNumNodes].ParentIndex = pNodeV2->ParentIndex;
					pNodes->nodeInfos[uNumNodes].NameIndex = (int)(pNodeV2->NameHashTableIndex >> 16);
					pNodes->nodeInfos[uNumNodes].NameOffset = (int)(pNodeV2->NameHashTableIndex & 0xffff);
					pNodes->nodeInfos[uNumNodes].FirstAttributeIndex = pNodeV2->FirstAttributeIndex;
					pWork += sizeof(NODEENTRYV2);
					}
				uNumNodes++;
				}
			pWork = NULL;
			} break;

		//--- ATTRIBUTES
		case LSF_UNPACK_ATTRIBUTES: {
			LFSATTRS*	pAttributes;
			BYTE*		pBound;
			BOOL		bLongAttributes;
			UINT		uNumAttrs;

			bLongAttributes = pReader->pHeader->Version >= 3 && pReader->pHeader->Extended == 1;

			pWork = pBound = pData;
			pBound += dwSize;
			uNumAttrs = 0;
			while (pWork < pBound)
				{
				uNumAttrs++;
				if (bLongAttributes) pWork += sizeof(ATTRIBUTEENTRYV3);
				else pWork += sizeof(ATTRIBUTEENTRYV2);
				}

			pAttributes = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(LFSATTRS)+sizeof(LFSATTRINFO)*uNumAttrs);
			if (!pAttributes)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				goto Done;
				}
			pAttributes->uNumAttrs = uNumAttrs;
			List_AddEntry((NODE *)pAttributes,&pReader->nodeAttrs);

			pWork = pBound = pData;
			pBound += dwSize;
			uNumAttrs = 0;
			if (bLongAttributes)
				{
				while (pWork < pBound)
					{
					ATTRIBUTEENTRYV3 *pAttrV3 = (ATTRIBUTEENTRYV3 *)pWork;
					pAttributes->attrInfos[uNumAttrs].NameIndex = (int)(pAttrV3->NameHashTableIndex >> 16);
					pAttributes->attrInfos[uNumAttrs].NameOffset = (int)(pAttrV3->NameHashTableIndex & 0xffff);
					pAttributes->attrInfos[uNumAttrs].TypeId = (unsigned int)(pAttrV3->TypeAndLength & 0x3f);
					pAttributes->attrInfos[uNumAttrs].Length = (unsigned int)(pAttrV3->TypeAndLength >> 6);
					pAttributes->attrInfos[uNumAttrs].DataOffset = pAttrV3->Offset;
					pAttributes->attrInfos[uNumAttrs].NextAttributeIndex = pAttrV3->NextAttributeIndex;
					pWork += sizeof(ATTRIBUTEENTRYV3);
					}
				}
			else
				{
				UINT	uDataOffset;
				UINT	uNodeIndex;
				UINT	uAttrRefsCnt;
				UINT	uAttrRefsTotal;
				int	iIndex;
				int*	prevAttributeRefs;
				int*	pTmp;

				uAttrRefsTotal = 16;
				prevAttributeRefs = HeapAlloc(App.hHeap,0,uAttrRefsTotal*sizeof(int));
				if (!prevAttributeRefs)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					goto Done;
					}

				uDataOffset = 0;
				iIndex = 0;
				uAttrRefsCnt = 0;

				while (pWork < pBound)
					{
					ATTRIBUTEENTRYV2 *pAttrV2 = (ATTRIBUTEENTRYV2 *)pWork;
					pAttributes->attrInfos[iIndex].NameIndex = (int)(pAttrV2->NameHashTableIndex >> 16);
					pAttributes->attrInfos[iIndex].NameOffset = (int)(pAttrV2->NameHashTableIndex & 0xffff);
					pAttributes->attrInfos[iIndex].TypeId = (unsigned int)(pAttrV2->TypeAndLength & 0x3f);
					pAttributes->attrInfos[iIndex].Length = (unsigned int)(pAttrV2->TypeAndLength >> 6);
					pAttributes->attrInfos[iIndex].DataOffset = uDataOffset;
					pAttributes->attrInfos[iIndex].NextAttributeIndex = -1;

					uNodeIndex = pAttrV2->NodeIndex+1;
					if (uAttrRefsCnt > uNodeIndex)
						{
						if (prevAttributeRefs[uNodeIndex] != -1)
							pAttributes->attrInfos[prevAttributeRefs[uNodeIndex]].NextAttributeIndex = iIndex;
						prevAttributeRefs[uNodeIndex] = iIndex;
						}
					else
						{
						while (uAttrRefsCnt < uNodeIndex)
							{
							if (uAttrRefsCnt >= uAttrRefsTotal)
								{
								uAttrRefsTotal *= 2;
								pTmp = HeapReAlloc(App.hHeap,0,prevAttributeRefs,uAttrRefsTotal*sizeof(int));
								if (!pTmp)
									{
									HeapFree(App.hHeap,0,prevAttributeRefs);
									SetLastError(ERROR_NOT_ENOUGH_MEMORY);
									goto Done;
									}
								prevAttributeRefs = pTmp;
								}
							prevAttributeRefs[uAttrRefsCnt++] = -1;
							}
						if (uAttrRefsCnt >= uAttrRefsTotal)
							{
							uAttrRefsTotal *= 2;
							pTmp = HeapReAlloc(App.hHeap,0,prevAttributeRefs,uAttrRefsTotal*sizeof(int));
							if (!pTmp)
								{
								HeapFree(App.hHeap,0,prevAttributeRefs);
								SetLastError(ERROR_NOT_ENOUGH_MEMORY);
								goto Done;
								}
							prevAttributeRefs = pTmp;
							}
						prevAttributeRefs[uAttrRefsCnt++] = iIndex;
						}

					uDataOffset += pAttributes->attrInfos[iIndex].Length;
					pWork += sizeof(ATTRIBUTEENTRYV2);
					iIndex++;
					}

				HeapFree(App.hHeap,0,prevAttributeRefs);
				}
			} break;

		//--- VALUES
		case LSF_UNPACK_VALUES:
			pReader->pValues = pData;
			pData = NULL;
			break;

		}

	bIsDone = TRUE;

Done:	if (pData) HeapFree(App.hHeap,0,pData);
	return(bIsDone);
}


// «»»» Libère la mémoire utilisée ««««««««««««««««««««««««««««««««««««««»

//--- Noms ---

void lsf_ReleaseNames(NODE *pRoot)
{
	LFSNAMES*	pNames;

	for (pNames = (LFSNAMES *)pRoot->next; pNames != NULL; pNames = (LFSNAMES *)pNames->node.next)
		if (pNames->uNumStrings && pNames->pszNames)
			while (pNames->uNumStrings--)
				if (pNames->pszNames[pNames->uNumStrings]) HeapFree(App.hHeap,0,pNames->pszNames[pNames->uNumStrings]);

	List_ReleaseMemory(pRoot);
	return;
}

//--- Noeuds ---

void lsf_ReleaseNodes(NODE *pRoot)
{
	List_ReleaseMemory(pRoot);
	return;
}

//--- Attributs ---

void lsf_ReleaseAttrs(NODE *pRoot)
{
	List_ReleaseMemory(pRoot);
	return;
}

//--- Valeurs ---

void lsf_ReleaseValues(BYTE *pBuffer)
{
	if (!pBuffer) return;
	HeapFree(App.hHeap,0,pBuffer);
	return;
}

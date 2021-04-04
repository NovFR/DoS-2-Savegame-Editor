
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Fonctions diverses
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION		App;

static XMLVALIDATION		TreeViewValidation[] = {

					{ L"region",	{ L"id" } },
					{ L"node",	{ L"id" } },
					{ L"attribute",	{ L"id", L"type", L"value", L"handle" } },
					{ L"children" },
					{ L"version",	{ L"major", L"minor", L"revision", L"build" } },
					{ NULL }

				};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'une erreur «««««««««««««««««««««««««««««««««««««««««««»

BOOL Tree_DisplayError(HWND hWnd, UINT uMsgId, BOOL *pbAbort, ...)
{
	TASKDIALOGCONFIG	dialog;
	WCHAR*			pszText;
	DWORD_PTR		vldptr[4];
	BOOL			bResult;
	va_list			vl;

	pszText = NULL;
	bResult = FALSE;

	//--- Récupération des arguments

	va_start(vl,pbAbort);

	switch(uMsgId)
		{
		// MSG: L'attribut "%1" existe déjà dans "%2" !
		// MSG: L'attribut "%1" n'est pas autorisé dans "%2" !
		// MSG: "%1" dans "%2" n'est pas un nom d'attribut valide !
		// %1 - (WCHAR *) attr name
		// %2 - (void *) parent node
		case TEXT_ERR_TV_ATTREXISTS:
		case TEXT_ERR_TV_NODEBADATTR:
		case TEXT_ERR_TV_BADATTR:
			vldptr[0] = (DWORD_PTR)va_arg(vl,WCHAR *);
			vldptr[1] = (DWORD_PTR)xml_AttrToWideChar((XML_NODE *)va_arg(vl,void *));
			if (!vldptr[1]) goto Done;
			break;
		// MSG: "%1" ne peut recevoir d'attributs.
		// %1 - (XML_NODE *) node
		case TEXT_ERR_TV_NODEHASNOATTR:
			vldptr[0] = (DWORD_PTR)xml_AttrToWideChar((XML_NODE *)va_arg(vl,void *));
			if (!vldptr[0]) goto Done;
			break;
		// MSG: "%1" n'est pas un nom de node valide !
		// %1 - (WCHAR *) node name
		case TEXT_ERR_TV_BADNODE:
			vldptr[0] = (DWORD_PTR)va_arg(vl,WCHAR *);
			break;
		// MSG: La valeur "%1" de "%2" dans "%3" ne doit pas contenir les caractères: %4
		// %1 - (WCHAR *) value
		// %2 - (WCHAR *) attr name
		// %3 - (void *) parent node
		// %4 - (WCHAR *) characters
		case TEXT_ERR_TV_BADVALUE:
			vldptr[0] = (DWORD_PTR)va_arg(vl,WCHAR *);
			vldptr[1] = (DWORD_PTR)va_arg(vl,WCHAR *);
			vldptr[2] = (DWORD_PTR)xml_AttrToWideChar((XML_NODE *)va_arg(vl,void *));
			vldptr[3] = (DWORD_PTR)Tree_GetHtmlEntities();
			if (!vldptr[2]) goto Done;
			if (!vldptr[3]) goto Done;
			break;
		default:
			#if _DEBUG
			MessageBox(hWnd,L"Unhandled error msg",L"Internal Error",MB_ICONERROR|MB_OK);
			#endif
			return(TRUE);
		}

	va_end(vl);

	//--- Création du message

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,Locale_GetText(uMsgId),0,0,(WCHAR *)&pszText,1,(va_list *)vldptr))
		goto Done;

	//--- Affichage de la boîte de dialogue

	ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
	dialog.cbSize = sizeof(TASKDIALOGCONFIG);
	dialog.hwndParent = hWnd;
	dialog.dwFlags = TDF_USE_HICON_MAIN|TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
	dialog.pszWindowTitle = Locale_GetText(TEXT_TITLE_TREE);
	dialog.hMainIcon = App.hIconError;
	dialog.pszContent = pszText;
	if (pbAbort) dialog.pszVerificationText = Locale_GetText(TEXT_DIALOG_TV_ABORTVERIFICATION);

	TaskDialogIndirect(&dialog,NULL,NULL,pbAbort);
	bResult = TRUE;

	//--- Terminé

Done:	if (pszText) LocalFree(pszText);

	switch(uMsgId)
		{
		case TEXT_ERR_TV_ATTREXISTS:
		case TEXT_ERR_TV_NODEBADATTR:
		case TEXT_ERR_TV_BADATTR:
			if (vldptr[1]) LocalFree((void *)vldptr[1]);
			break;
		case TEXT_ERR_TV_NODEHASNOATTR:
			if (vldptr[0]) LocalFree((void *)vldptr[0]);
			break;
		case TEXT_ERR_TV_BADVALUE:
			if (vldptr[2]) LocalFree((void *)vldptr[2]);
			if (vldptr[3]) HeapFree(App.hHeap,0,(void *)vldptr[3]);
			break;
		}

	return(bResult);
}


// «»»» Vérifie si une valeur est valide ««««««««««««««««««««««««««««««««»

BOOL Tree_IsValueValid(WCHAR *pszValue)
{
	int	i,j;

	if (!pszValue) return(TRUE);

	for (i = 0; htmlEntities[i] != NULL; i += 2)
		for (j = 0; pszValue[j] != 0; j++)
			if (htmlEntities[i+1][0] == pszValue[j])
				{
				if (pszValue[j] == '&')
					{
					int	k,b;
					for (k = 0, b = 0; htmlEntities[k] != NULL; k += 2)
						{
						if (!wcsncmp(htmlEntities[k],&pszValue[j],wcslen(htmlEntities[k])))
							{
							j += wcslen(htmlEntities[k]);
							b = 1;
							break;
							}
						}
					if (!b) return(FALSE);
					}
				else return(FALSE);
				}

	return(TRUE);
}

//--- Récupère la liste des caractères interdits ---

WCHAR* Tree_GetHtmlEntities()
{
	WCHAR*	pszArray;
	UINT	uLen;
	int	i;

	for (i = 0, uLen = 0; htmlEntities[i] != NULL; i += 2)
		uLen += wcslen(htmlEntities[i+1]);

	if (!uLen) return(NULL);

	pszArray = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
	if (pszArray)
		{
		*pszArray = 0;
		for (i = 0; htmlEntities[i] != NULL; i += 2)
			wcscat(pszArray,htmlEntities[i+1]);
		}

	return(pszArray);
}


// «»»» Vérifie si un attribut est valide «««««««««««««««««««««««««««««««»

// pszAttrName: NULL > Check if pszNodeName exists
// pszNodeName: NULL > Check if pszAttrName exists
// Both non NULL > Check if pszAttrName is valid for pszNodeName

BOOL Tree_IsAttrValidForNode(WCHAR *pszAttrName, WCHAR *pszNodeName)
{
	int	i,j;

	for (i = 0; TreeViewValidation[i].pszNode != NULL; i++)
		{
		if (pszNodeName && wcscmp(pszNodeName,TreeViewValidation[i].pszNode)) continue;
		if (!pszAttrName) return(TRUE);
		for (j = 0; j != sizeof(TreeViewValidation[i].pszAttributes)/sizeof(WCHAR *) && TreeViewValidation[i].pszAttributes[j] != NULL; j++)
			if (!wcscmp(pszAttrName,TreeViewValidation[i].pszAttributes[j])) return(TRUE);
		if (pszNodeName) break;
		}

	return(FALSE);
}


// «»»» Vérifie si une node a des attributs «««««««««««««««««««««««««««««»

BOOL Tree_IsNodeHasAttributes(WCHAR *pszNodeName)
{
	int	i;

	for (i = 0; TreeViewValidation[i].pszNode != NULL; i++)
		{
		if (wcscmp(pszNodeName,TreeViewValidation[i].pszNode)) continue;
		if (TreeViewValidation[i].pszAttributes[0] != NULL) return(TRUE);
		break;
		}

	return(FALSE);
}


// «»»» Collecte une liste de noms ««««««««««««««««««««««««««««««««««««««»

XMLNAMES* Tree_CollectNames(NODE *pFirstChild, XMLNAMES *pNames)
{
	NODE*		pNode;
	UINT		uCount;

	uCount = 0;

	for (pNode = pFirstChild; pNode != NULL; pNode = pNode->next)
		{
		switch(pNode->type)
			{
			case XML_TYPE_ATTR:
				if (pNames) pNames->pszNames[uCount] = Misc_StrCpyAlloc(((XML_ATTR *)pNode)->name);
				uCount++;
				break;
			case XML_TYPE_NODE:
				if (!xml_GetAttrValue((XML_NODE *)pNode,szXMLid)) continue;
				if (pNames)
					{
					WCHAR *pszTemp = xml_AttrToWideChar((XML_NODE *)pNode);
					pNames->pszNames[uCount] = NULL;
					if (pszTemp)
						{
						pNames->pszNames[uCount] = Misc_StrCpyAlloc(pszTemp);
						LocalFree(pszTemp);
						}
					}
				uCount++;
				break;
			}
		}

	if (!pNames)
		{
		if (!uCount) return(NULL);
		pNames = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uCount*sizeof(WCHAR *)+sizeof(XMLNAMES));
		if (!pNames) return(NULL);
		pNames->uCount = uCount;
		return(Tree_CollectNames(pFirstChild,pNames));
		}

	qsort(pNames->pszNames,pNames->uCount,sizeof(WCHAR *),Tree_SortNames);
	return(pNames);
}

//--- Libère la mémoire utilisée ---

void Tree_ReleaseNames(XMLNAMES *pNames)
{
	int	i;

	if (!pNames) return;

	for (i = 0; i < pNames->uCount; i++)
		if (pNames->pszNames[i]) HeapFree(App.hHeap,0,pNames->pszNames[i]);

	HeapFree(App.hHeap,0,pNames);
	return;
}

//--- Tri ---

int Tree_SortNames(const void *a, const void *b)
{
	if (!*(WCHAR **)a && *(WCHAR **)b) return(-1);
	if (*(WCHAR **)a && !*(WCHAR **)b) return(1);
	if (!*(WCHAR **)a && !*(WCHAR **)b) return(0);
	return(wcscmp(*(WCHAR **)a,*(WCHAR **)b));
}

//--- Compare les noms ---

void Tree_CompareNames(HWND hDlg, NODE *pRoot, XMLNAMES *pNames, BOOL *pbResult, BOOL *pbAbort)
{
	WCHAR*	pszName;
	WCHAR*	pszPrev;
	int	i;

	if (!pNames) return;
	if (pNames->uCount < 2) return;

	for (pszPrev = NULL, i = 0; i < pNames->uCount; i++)
		{
		pszName = pNames->pszNames[i];
		if (!pszPrev)
			{
			pszPrev = pszName;
			continue;
			}
		if (!pszName) continue;
		if (!wcscmp(pszName,pszPrev)) // Name duplicated ?
			{
			*pbResult = FALSE;
			if (pRoot && pRoot->next) Tree_DisplayError(hDlg,TEXT_ERR_TV_ATTREXISTS,pbAbort,pszName,((XML_ATTR *)pRoot->next)->parent);
			if (*pbAbort) return;
			// Ignore further duplicates of the same name
			for (; i < pNames->uCount; i++) if (wcscmp(pNames->pszNames[i],pszName)) { i--; break; }
			pszPrev = NULL;
			}
		else pszPrev = pszName;
		}

	return;
}


// «»»» Retrouve la première entrée de la liste «««««««««««««««««««««««««»

XML_NODE* Tree_GetRootNode(TVITEMEX *tvItem)
{
	TVITEMEX	tvRoot;

	tvRoot.mask = TVIF_HANDLE|TVIF_PARAM;
	tvRoot.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_ROOT,0);
	tvRoot.lParam = 0;
	SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvRoot);

	if (tvItem)
		{
		tvItem->mask = tvRoot.mask;
		tvItem->hItem = tvRoot.hItem;
		tvItem->lParam = tvRoot.lParam;
		}

	return((XML_NODE *)tvRoot.lParam);
}

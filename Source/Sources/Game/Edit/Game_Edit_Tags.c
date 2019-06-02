
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Mots-clés
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la boîte de dialogue ««««««««««««««««««««««««««««««««»

void Game_Tags()
{
	GAMETAGS	Tags;
	INT_PTR		iResult;

	ZeroMemory(&Tags,sizeof(GAMETAGS));
	if (!Game_LoadDataFile(App.hWnd,szTagsDataPath,DATA_TYPE_TAGS,&Tags.nodeTags)) return;

	if (App.Game.pdcCurrent)
		{
		if (App.Game.pdcCurrent->pxaTags)
			{
			XML_NODE*	pxn;
			GAMEDATA*	pData;
			GAMEDATA*	pDataCopy;
			WCHAR*		pszText;

			SetLastError(ERROR_INVALID_DATA);
			//--- Vérifie que la liste est correct ---
			for (pxn = App.Game.pdcCurrent->pxaTags; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
				{
				if (List_EntryCount(&pxn->attributes) != 1) goto Done; // Too many attributes
				if (List_EntryCount(&pxn->children) != 1) goto Done; // Too many children
				if (List_EntryCount(&((XML_NODE *)pxn->children.next)->attributes) != 3) goto Done; // Too many attributes in child node

				if (wcscmp(pxn->name,szXMLnode)) goto Done; // NOT <node>
				if (wcscmp(((XML_NODE *)pxn->children.next)->name,szXMLattribute)) goto Done; // NOT <attribute>

				pszText = xml_GetAttrValue(pxn,szXMLid);
				if (!pszText || wcscmp(pszText,szXMLTarget)) goto Done; // NOT <node id="Target">
				pszText = xml_GetAttrValue((XML_NODE *)pxn->children.next,szXMLid);
				if (!pszText || wcscmp(pszText,szXMLObject)) goto Done; // NOT <attribute id="Object">
				pszText = xml_GetAttrValue((XML_NODE *)pxn->children.next,szXMLvalue);
				if (!pszText) goto Done; // Empty tag
				pszText = xml_GetAttrValue((XML_NODE *)pxn->children.next,szXMLtype);
				if (!pszText || wcscmp(pszText,L"22")) goto Done; // NOT <attribute type="22">
				}

			//--- Copie les mots-clés ---
			for (pxn = App.Game.pdcCurrent->pxaTags; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
				{
				pszText = xml_GetAttrValue((XML_NODE *)pxn->children.next,szXMLvalue);
				if (!pszText) continue; // Impossible
				pDataCopy = NULL;
				// Ajoute le mot-clé et sa traduction
				for (pData = (GAMEDATA *)Tags.nodeTags.next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
					{
					if (wcscmp(pData->pszId,pszText)) continue;
					pDataCopy = HeapAlloc(App.hHeap,0,sizeof(GAMEDATA));
					if (!pDataCopy)
						{
						SetLastError(ERROR_NOT_ENOUGH_MEMORY);
						goto Done;
						}
					CopyMemory(pDataCopy,pData,sizeof(GAMEDATA));
					ZeroMemory(&pDataCopy->node,sizeof(NODE));
					List_AddEntry((NODE *)pDataCopy,&Tags.nodeUsedTags);
					break;
					}
				// Ajoute le mot-clé directement (non traduit)
				if (!pDataCopy)
					{
					pDataCopy = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEDATA)+wcslen(pszText)*sizeof(WCHAR)+sizeof(WCHAR));
					if (!pDataCopy)
						{
						SetLastError(ERROR_NOT_ENOUGH_MEMORY);
						goto Done;
						}
					pDataCopy->pszId = (WCHAR *)(((BYTE *)pDataCopy)+sizeof(GAMEDATA));
					wcscpy((WCHAR *)(((BYTE *)pDataCopy)+sizeof(GAMEDATA)),pszText);
					pDataCopy->tag.bProtected = TRUE;
					pDataCopy->tag.bHidden = TRUE;
					List_AddEntry((NODE *)pDataCopy,&Tags.nodeUsedTags);
					}
				}
			SetLastError(ERROR_SUCCESS);
			}
		}

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1006),App.hWnd,Game_TagsProc,(LPARAM)&Tags);

	//--- Recréation de la liste des tags dans la structure du personnage ---

	if (iResult == IDOK)
		{
		GAMEDATA*	pData;
		XML_NODE*	pxnNew;
		XML_NODE*	pxnNewChild;
		NODE		nodeNew;

		SetLastError(ERROR_SUCCESS);
		ZeroMemory(&nodeNew,sizeof(NODE));
		for (pData = (GAMEDATA *)Tags.nodeUsedTags.next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
			{
			pxnNew = xml_CreateNode(szXMLnode,App.Game.pdcCurrent->pxaTags->parent,1,szXMLid,szXMLTarget);
			if (!pxnNew)
				{
				xml_ReleaseAll(&nodeNew);
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				break;
				}
			pxnNewChild = xml_CreateNode(szXMLattribute,pxnNew,3,szXMLid,szXMLObject,szXMLvalue,pData->pszId,szXMLtype,L"22");
			if (!pxnNewChild)
				{
				xml_ReleaseAll(&nodeNew);
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				break;
				}
			List_AddEntry((NODE *)pxnNewChild,&pxnNew->children);
			List_AddEntry((NODE *)pxnNew,&nodeNew);
			}
		if (GetLastError() == ERROR_SUCCESS)
			{
			NODE	*pNodeChildren;

			pNodeChildren = &((XML_NODE *)App.Game.pdcCurrent->pxaTags->parent)->children;
			xml_ReleaseAll(&((XML_NODE *)App.Game.pdcCurrent->pxaTags->parent)->children);
			List_Move(&nodeNew,pNodeChildren);
			App.Game.pdcCurrent->pxaTags = (XML_NODE *)pNodeChildren->next;
			}
		}
	else
		{
		if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		SetLastError(ERROR_SUCCESS);
		}

	//--- Terminé ---

Done:	if (GetLastError() != ERROR_SUCCESS) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	List_ReleaseMemory(&Tags.nodeUsedTags);
	Game_UnloadDataFile(DATA_TYPE_TAGS,&Tags.nodeTags);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_TagsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMETAGS*	pTags;

	if (uMsgId == WM_INITDIALOG)
		{
		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pTags = (GAMETAGS *)lParam;

		if (!Game_TagsPopulateList(hDlg,300,&pTags->nodeUsedTags,FALSE,!App.Config.bShowHiddenTags))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		CheckDlgButton(hDlg,110,App.Config.bShowHiddenTags?BST_CHECKED:BST_UNCHECKED);

		SendDlgItemMessage(hDlg,110,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_TAG_SHOWHIDDEN));
		SendDlgItemMessage(hDlg,400,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_ADD));
		SendDlgItemMessage(hDlg,401,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_EDIT));
		SendDlgItemMessage(hDlg,402,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_REMOVE));
		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));

		SetWindowText(hDlg,Locale_GetText(TEXT_BUTTON_TAGS));
		Dialog_CenterWindow(hDlg,App.hWnd);
		return(FALSE);
		}

	pTags = (GAMETAGS *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			switch(wParam)
				{
				case 300:
					Game_TagsMeasureItem(hDlg,(MEASUREITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 300:
					Game_TagsDrawItemBuffered((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 403:
				case 404:
					Dialog_DrawIconButton(wParam == 403?APP_ICON_UP:APP_ICON_DOWN,(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 300:
							Game_TagsEdit(hDlg,pTags);
							return(TRUE);
						}
					break;

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 300:
							Game_TagsSelected(hDlg,pTags);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 110:
							Game_TagsToggle(hDlg,pTags);
							return(TRUE);
						case 400:
							Game_TagsAdd(hDlg,pTags);
							return(TRUE);
						case 401:
							Game_TagsEdit(hDlg,pTags);
							return(TRUE);
						case 402:
							Game_TagsRemove(hDlg,pTags);
							return(TRUE);
						case 403:
							Game_TagsMove(hDlg,pTags,FALSE);
							return(TRUE);
						case 404:
							Game_TagsMove(hDlg,pTags,TRUE);
							return(TRUE);
						case IDOK:
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
							EndDialog(hDlg,IDCANCEL);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_CLOSE:
			EndDialog(hDlg,IDCANCEL);
			return(TRUE);
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modifications							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affiche/Cache les mots-clés cachés ««««««««««««««««««««««««««««««»

void Game_TagsToggle(HWND hDlg, GAMETAGS *pTags)
{
	SendDlgItemMessage(hDlg,300,LB_RESETCONTENT,0,0);
	App.Config.bShowHiddenTags ^= 1;
	CheckDlgButton(hDlg,110,App.Config.bShowHiddenTags?BST_CHECKED:BST_UNCHECKED);
	Game_TagsPopulateList(hDlg,300,&pTags->nodeUsedTags,FALSE,!App.Config.bShowHiddenTags);
	Game_TagsSelected(hDlg,pTags);
	return;
}


// «»»» Modification de la sélection ««««««««««««««««««««««««««««««««««««»

void Game_TagsSelected(HWND hDlg, GAMETAGS *pTags)
{
	GAMEDATA*	pData;
	UINT		uSelected;
	UINT		uCount;

	uSelected = SendDlgItemMessage(hDlg,300,LB_GETCURSEL,0,0);
	if (uSelected != LB_ERR)
		{
		uCount = SendDlgItemMessage(hDlg,300,LB_GETCOUNT,0,0);
		if (uCount != LB_ERR)
			{
			pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uSelected,0);
			if (pData != (GAMEDATA *)LB_ERR)
				{
				EnableWindow(GetDlgItem(hDlg,401),pData->tag.bProtected?FALSE:TRUE);
				EnableWindow(GetDlgItem(hDlg,402),pData->tag.bProtected?FALSE:TRUE);
				EnableWindow(GetDlgItem(hDlg,403),uSelected?TRUE:FALSE);
				EnableWindow(GetDlgItem(hDlg,404),(uCount && uSelected != uCount-1)?TRUE:FALSE);
				return;
				}
			}
		}

	EnableWindow(GetDlgItem(hDlg,401),FALSE);
	EnableWindow(GetDlgItem(hDlg,402),FALSE);
	EnableWindow(GetDlgItem(hDlg,403),FALSE);
	EnableWindow(GetDlgItem(hDlg,404),FALSE);
	return;
}


// «»»» Ajout d'un mot-clé ««««««««««««««««««««««««««««««««««««««««««««««»

void Game_TagsAdd(HWND hDlg, GAMETAGS *pTags)
{
	GAMEDATA*	pDataNew;
	WCHAR*		pszId;

	pszId = Game_EditValue(hDlg,NULL,DATA_TYPE_TAGS);
	if (pszId)
		{
		GAMEDATA*	pDataCopyFrom;
		LRESULT		lResult;

		//--- Ne pas ajouter le mot-clé si l'identifiant existe déjà dans la liste ---

		if (Game_TagsExists(pszId,&pTags->nodeUsedTags,NULL))
			{
			MessageBox(hDlg,Locale_GetText(TEXT_ERR_TAG_EXISTS),NULL,MB_ICONERROR);
			HeapFree(App.hHeap,0,pszId);
			return;
			}

		//--- Ajout du nouveau mot-clé ---

		for (pDataCopyFrom = (GAMEDATA *)pTags->nodeTags.next; pDataCopyFrom != NULL; pDataCopyFrom = (GAMEDATA *)pDataCopyFrom->node.next)
			{
			if (wcscmp(pDataCopyFrom->pszId,pszId)) continue;
			pDataNew = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEDATA));
			if (!pDataNew)
				{
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_TAG_ADD),NULL,MB_ICONERROR);
				break;
				}
			CopyMemory(pDataNew,pDataCopyFrom,sizeof(GAMEDATA));
			List_AddEntry((NODE *)pDataNew,&pTags->nodeUsedTags);
			lResult = SendDlgItemMessage(hDlg,300,LB_ADDSTRING,0,(LPARAM)pDataNew);
			if (lResult ==  LB_ERR || lResult == LB_ERRSPACE)
				{
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_TAG_ADD),NULL,MB_ICONERROR);
				List_RemEntry((NODE *)pDataNew);
				HeapFree(App.hHeap,0,pDataNew);
				break;
				}
			SendDlgItemMessage(hDlg,300,LB_SETCURSEL,(WPARAM)lResult,0);
			Game_TagsSelected(hDlg,pTags);
			break;
			}

		HeapFree(App.hHeap,0,pszId);
		}

	return;
}


// «»»» Modification du mot-clé sélectionné «««««««««««««««««««««««««««««»

void Game_TagsEdit(HWND hDlg, GAMETAGS *pTags)
{
	GAMEDATA*	pData;
	WCHAR*		pszId;
	UINT		uSelected;

	uSelected = SendDlgItemMessage(hDlg,300,LB_GETCURSEL,0,0);
	if (uSelected == LB_ERR) return;

	pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uSelected,0);
	if (pData == (GAMEDATA *)LB_ERR) return;
	if (pData->tag.bProtected) return;

	pszId = Game_EditValue(hDlg,pData->pszId,DATA_TYPE_TAGS);
	if (pszId)
		{
		NODE		nodeCopy;
		GAMEDATA*	pDataCopy;

		//--- Inutile de modifier le mot-clé si le même identifiant a été sélectionné ---

		if (!wcscmp(pData->pszId,pszId))
			{
			HeapFree(App.hHeap,0,pszId);
			return;
			}

		//--- Ne pas modifier le mot-clé si l'identifiant existe déjà dans la liste ---

		if (Game_TagsExists(pszId,&pTags->nodeUsedTags,pData))
			{
			MessageBox(hDlg,Locale_GetText(TEXT_ERR_TAG_EXISTS),NULL,MB_ICONERROR);
			HeapFree(App.hHeap,0,pszId);
			return;
			}

		//--- Copie de la structure ---

		for (pDataCopy = (GAMEDATA *)pTags->nodeTags.next; pDataCopy != NULL; pDataCopy = (GAMEDATA *)pDataCopy->node.next)
			{
			if (wcscmp(pDataCopy->pszId,pszId)) continue;
			CopyMemory(&nodeCopy,&pData->node,sizeof(NODE));
			CopyMemory(pData,pDataCopy,sizeof(GAMEDATA));
			CopyMemory(&pData->node,&nodeCopy,sizeof(NODE));
			// Not ideal, but there no way to force the LB to update itself
			// There is no error check, but in case of error, the list is fucked up anyway
			SendDlgItemMessage(hDlg,300,LB_DELETESTRING,(WPARAM)uSelected,0);
			SendDlgItemMessage(hDlg,300,LB_INSERTSTRING,(WPARAM)uSelected,(LPARAM)pData);
			SendDlgItemMessage(hDlg,300,LB_SETCURSEL,(WPARAM)uSelected,0);
			Game_TagsSelected(hDlg,pTags);
			break;
			}

		HeapFree(App.hHeap,0,pszId);
		}

	return;
}


// «»»» Suppression du mot-clé sélectionné ««««««««««««««««««««««««««««««»

void Game_TagsRemove(HWND hDlg, GAMETAGS *pTags)
{
	GAMEDATA*	pData;
	UINT		uSelected;
	LRESULT		lResult;

	uSelected = SendDlgItemMessage(hDlg,300,LB_GETCURSEL,0,0);
	if (uSelected == LB_ERR) return;
	pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uSelected,0);
	if (pData == (GAMEDATA *)LB_ERR) return;

	lResult = SendDlgItemMessage(hDlg,300,LB_DELETESTRING,(WPARAM)uSelected,0);
	if (lResult != LB_ERR)
		{
		List_RemEntry((NODE *)pData);
		HeapFree(App.hHeap,0,pData);
		Game_TagsSelected(hDlg,pTags);
		}

	return;
}


// «»»» Déplacement d'un mot-clé ««««««««««««««««««««««««««««««««««««««««»

void Game_TagsMove(HWND hDlg, GAMETAGS *pTags, BOOL bDown)
{
	GAMEDATA*	pData;
	NODE*		pNode;
	UINT		uSelected;
	UINT		uCount;
	LRESULT		lResult;

	uSelected = SendDlgItemMessage(hDlg,300,LB_GETCURSEL,0,0);
	if (uSelected == LB_ERR) return;
	pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uSelected,0);
	if (pData == (GAMEDATA *)LB_ERR) return;

	if (bDown)
		{
		uCount = SendDlgItemMessage(hDlg,300,LB_GETCOUNT,0,0);
		if (uCount == LB_ERR) return;
		if (uSelected == uCount-1) return;
		lResult = SendDlgItemMessage(hDlg,300,LB_INSERTSTRING,(WPARAM)uSelected+2,(LPARAM)pData);
		if (lResult == LB_ERR) return;
		if (lResult == LB_ERRSPACE) return;
		SendDlgItemMessage(hDlg,300,LB_DELETESTRING,(WPARAM)uSelected,0);
		SendDlgItemMessage(hDlg,300,LB_SETCURSEL,(WPARAM)uSelected+1,0);
		if (App.Config.bShowHiddenTags)
			pNode = pData->node.next;
		else
			{
			pNode = (NODE *)pData;
			while ((pNode = pNode->next) != NULL) if (!((GAMEDATA *)pNode)->tag.bHidden) break;
			if (!pNode) for (pNode = (NODE *)pData; pNode->next != NULL; pNode = pNode->next);
			}
		List_RemEntry((NODE *)pData);
		List_InsertEntry((NODE *)pData,pNode,TRUE);
		}
	else
		{
		if (!uSelected) return;
		lResult = SendDlgItemMessage(hDlg,300,LB_INSERTSTRING,(WPARAM)uSelected-1,(LPARAM)pData);
		if (lResult == LB_ERR) return;
		if (lResult == LB_ERRSPACE) return;
		SendDlgItemMessage(hDlg,300,LB_DELETESTRING,(WPARAM)uSelected+1,0);
		SendDlgItemMessage(hDlg,300,LB_SETCURSEL,(WPARAM)uSelected-1,0);
		if (App.Config.bShowHiddenTags)
			pNode = pData->node.prev;
		else
			{
			pNode = (NODE *)pData;
			while ((pNode = pNode->prev) != NULL) if (!((GAMEDATA *)pNode)->tag.bHidden) break;
			if (!pNode) pNode = pTags->nodeUsedTags.next;
			}
		List_RemEntry((NODE *)pData);
		List_InsertEntry((NODE *)pData,pNode,FALSE);
		}

	Game_TagsSelected(hDlg,pTags);
	return;
}


// «»»» Vérifie si un mot-clé existe déjà «««««««««««««««««««««««««««««««»

BOOL Game_TagsExists(WCHAR *pszId, NODE *pList, GAMEDATA *pIgnoreThis)
{
	GAMEDATA*	pData;

	for (pData = (GAMEDATA *)pList->next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
		{
		if (pIgnoreThis) { if (pData == pIgnoreThis) continue; }
		if (wcscmp(pData->pszId,pszId)) continue;
		return(TRUE);
		}

	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ajoute les entrées dans la liste ««««««««««««««««««««««««««««««««»

int Game_TagsPopulateList(HWND hDlg, UINT uCtrlId, NODE *pTags, BOOL bIgnoreProtected, BOOL bIgnoreHidden)
{
	GAMEDATA*	pData;
	LRESULT		lResult;

	for (pData = (GAMEDATA *)pTags->next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
		{
		if (bIgnoreHidden && pData->tag.bHidden) continue;
		if (bIgnoreProtected && pData->tag.bProtected) continue;
		lResult = SendDlgItemMessage(hDlg,uCtrlId,LB_ADDSTRING,0,(LPARAM)pData);
		if (lResult == LB_ERR) return(0);
		if (lResult == LB_ERRSPACE) return(0);
		}

	return(1);
}


// «»»» Affichage des entrées de la liste «««««««««««««««««««««««««««««««»

//--- Mesure la hauteur d'une entrée ---

void Game_TagsMeasureItem(HWND hDlg, MEASUREITEMSTRUCT *pMeasure)
{
	GAMEDATA*	pData;
	HDC		hDC;

	pData = (GAMEDATA *)pMeasure->itemData;
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = 0;
	hDC = GetDC(hDlg);
	if (hDC)
		{
		// Base height
		pMeasure->itemHeight = 28+App.Font.uFontHeight;
		if (pData->tag.pszDescription)
			{
			HFONT		hDefFont;
			RECT		rcText;

			GetClientRect(GetDlgItem(hDlg,300),&rcText);
			rcText.left += 16;
			rcText.right -= 16;
			hDefFont = SelectObject(hDC,App.Font.hFont);
			pMeasure->itemHeight += DrawText(hDC,pData->tag.pszDescription,-1,&rcText,DT_CALCRECT|DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_WORDBREAK);
			pMeasure->itemHeight += 20;
			SelectObject(hDC,hDefFont);
			}
		ReleaseDC(hDlg,hDC);
		}
	return;
}

//--- Affichage de l'entrée (DoubleBuffering) ---

void Game_TagsDrawItemBuffered(DRAWITEMSTRUCT *pDraw)
{
	DRAWITEMSTRUCT	DrawItem;
	BOOL		bSimple;

	CopyMemory(&DrawItem,pDraw,sizeof(DRAWITEMSTRUCT));
	DrawItem.rcItem.right -= DrawItem.rcItem.left;
	DrawItem.rcItem.bottom -= DrawItem.rcItem.top;
	DrawItem.rcItem.left = 0;
	DrawItem.rcItem.top = 0;
	bSimple = TRUE;

	DrawItem.hDC = CreateCompatibleDC(pDraw->hDC);
	if (DrawItem.hDC)
		{
		HBITMAP hBitmap = CreateCompatibleBitmap(pDraw->hDC,DrawItem.rcItem.right,DrawItem.rcItem.bottom);
		if (hBitmap)
			{
			HBITMAP hbitmapDefault = SelectObject(DrawItem.hDC,hBitmap);
			Game_TagsDrawItem(&DrawItem);
			BitBlt(pDraw->hDC,pDraw->rcItem.left,pDraw->rcItem.top,DrawItem.rcItem.right,DrawItem.rcItem.bottom,DrawItem.hDC,0,0,SRCCOPY);
			SelectObject(DrawItem.hDC,hbitmapDefault);
			DeleteObject(hBitmap);
			bSimple = FALSE;
			}
		DeleteDC(DrawItem.hDC);
		}

	if (bSimple) Game_TagsDrawItem(pDraw);
	return;
}

//--- Affichage de l'entrée ---

void Game_TagsDrawItem(DRAWITEMSTRUCT *pDraw)
{
	GAMEDATA*	pData;
	WCHAR*		pszText;
	HPEN		hPen;
	HPEN		hDefPen;
	HFONT		hDefFont;
	RECT		rcClient;
	COLORREF	crText;
	int		iBkMode;

	pData = (GAMEDATA *)pDraw->itemData;
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_WINDOW));
	if (!pData) return;

	CopyRect(&rcClient,&pDraw->rcItem);
	rcClient.left += 2;
	rcClient.top += 2;
	rcClient.right -= 2;
	rcClient.bottom -= 2;

	hDefFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);

	//--- Habillage ---

	hPen = CreatePen(PS_SOLID,1,GetSysColor(COLOR_WINDOWTEXT));
	if (hPen)
		{
		POINT	ptLine[18];

		hDefPen = SelectObject(pDraw->hDC,hPen);
		ptLine[0].x = rcClient.left+4;				// Start point
		ptLine[0].y = rcClient.top+2+(App.Font.uFontHeight/2);
		ptLine[1].x = rcClient.right-8;				// Upper line
		ptLine[1].y = ptLine[0].y;
		ptLine[2].x = ptLine[1].x;				// Upper right corner
		ptLine[2].y = ptLine[1].y+8;
		ptLine[3].x = ptLine[2].x-6;
		ptLine[3].y = ptLine[2].y;
		ptLine[4].x = ptLine[3].x;
		ptLine[4].y = ptLine[3].y-4;
		ptLine[5].x = ptLine[4].x+10;
		ptLine[5].y = ptLine[4].y;
		ptLine[6].x = ptLine[5].x;				// Right line
		ptLine[6].y = rcClient.bottom-8;
		ptLine[7].x = ptLine[6].x-10;				// Right corner
		ptLine[7].y = ptLine[6].y;
		ptLine[8].x = ptLine[7].x;
		ptLine[8].y = ptLine[7].y-4;
		ptLine[9].x = ptLine[8].x+6;
		ptLine[9].y = ptLine[8].y;
		ptLine[10].x = ptLine[9].x;
		ptLine[10].y = ptLine[9].y+8;
		ptLine[11].x = ptLine[0].x+4;				// Bottom line
		ptLine[11].y = ptLine[10].y;
		ptLine[12].x = ptLine[11].x;				// Bottom corner
		ptLine[12].y = ptLine[11].y-8;
		ptLine[13].x = ptLine[12].x+6;
		ptLine[13].y = ptLine[12].y;
		ptLine[14].x = ptLine[13].x;
		ptLine[14].y = ptLine[13].y+4;
		ptLine[15].x = ptLine[14].x-10;
		ptLine[15].y = ptLine[14].y;
		ptLine[16].x = ptLine[15].x;				// Left line
		ptLine[16].y = ptLine[0].y;
		Polyline(pDraw->hDC,ptLine,17);
		SelectObject(pDraw->hDC,hDefPen);
		DeleteObject(hPen);
		}

	//--- Affichage de l'identifiant ---

	if (pData->pszId)
		{
		RECT	rcText;
		SIZE	Size;

		GetTextExtentPoint32(pDraw->hDC,pData->pszId,wcslen(pData->pszId),&Size);
		Size.cx += 4;
		if (pData->tag.bProtected || pData->tag.bHidden) Size.cx += 20;
		rcText.top = rcClient.top+2;
		rcText.bottom = rcText.top+App.Font.uFontHeight+2;
		rcText.left = rcClient.left+4;
		rcText.right = rcText.left+Size.cx;
		FillRect(pDraw->hDC,&rcText,GetSysColorBrush(COLOR_WINDOW));
		CopyRect(&rcText,&rcClient);
		rcText.left += 4;
		rcText.top += 2;
		rcText.bottom = rcText.top+App.Font.uFontHeight+2;
		if (pData->tag.bProtected || pData->tag.bHidden)
			{
			DrawIconEx(pDraw->hDC,rcText.left,rcText.top+(rcText.bottom-rcText.top-16)/2,App.hIcons[pData->tag.bHidden?APP_ICON_HIDDEN:APP_ICON_PROTECTED],16,16,0,NULL,DI_NORMAL);
			rcText.left += 20;
			}
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
		DrawText(pDraw->hDC,pData->pszId,-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		}

	//--- Affichage du nom ---

	pszText = pData->pszText?pData->pszText:Locale_GetText(TEXT_UNKNOWN);
	if (pszText)
		{
		RECT	rcText;
		SIZE	Size;

		GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
		Size.cx += 20;
		rcText.top = rcClient.top+2;
		rcText.bottom = rcText.top+App.Font.uFontHeight+2;
		rcText.left = rcClient.left+(rcClient.right-rcClient.left-Size.cx)/2;
		rcText.right = rcText.left+Size.cx;
		FillRect(pDraw->hDC,&rcText,GetSysColorBrush(COLOR_WINDOW));
		Size.cx -= 10;
		rcText.left = rcClient.left+(rcClient.right-rcClient.left-Size.cx)/2;
		rcText.right = rcText.left+Size.cx;
		hPen = CreatePen(PS_SOLID,1,GetSysColor(COLOR_WINDOWTEXT));
		if (hPen)
			{
			HBRUSH	hDefBrush;

			hDefPen = SelectObject(pDraw->hDC,hPen);
			hDefBrush = SelectObject(pDraw->hDC,GetSysColorBrush(COLOR_WINDOWTEXT));
			RoundRect(pDraw->hDC,rcText.left,rcText.top,rcText.right,rcText.bottom,8,8);
			SelectObject(pDraw->hDC,hDefBrush);
			SelectObject(pDraw->hDC,hDefPen);
			DeleteObject(hPen);
			}
		else FillRect(pDraw->hDC,&rcText,GetSysColorBrush(COLOR_WINDOWTEXT));
		CopyRect(&rcText,&rcClient);
		rcText.top += 2;
		rcText.bottom = rcText.top+App.Font.uFontHeight+2;
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOW));
		DrawText(pDraw->hDC,pszText,-1,&rcText,DT_CENTER|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		}

	//--- Affichage de la description ---

	if (pData->tag.pszDescription)
		{
		RECT	rcText;

		CopyRect(&rcText,&rcClient);
		rcText.top += 2+App.Font.uFontHeight+16;
		rcText.bottom -= 16;
		rcText.left += 16;
		rcText.right -= 16;
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
		DrawText(pDraw->hDC,pData->tag.pszDescription,-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_WORDBREAK);
		}

	//--- Affichage du rectangle de focus ---

	if (pDraw->itemState&ODS_FOCUS)
		{
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
		DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
		}

	//--- Surlignage si sélectionné ---

	if (pDraw->itemState&ODS_SELECTED)
		{
		HDC		hDC;
		HBITMAP		hBitmap;
		HBITMAP		hDefBitmap;
		RECT		rcRect;
		BLENDFUNCTION	blend;

		hDC = CreateCompatibleDC(pDraw->hDC);
		if (hDC)
			{
			hBitmap = CreateCompatibleBitmap(pDraw->hDC,16,16);
			if (hBitmap)
				{
				hDefBitmap = SelectObject(hDC,hBitmap);
				rcRect.left = 0;
				rcRect.top = 0;
				rcRect.right = 16;
				rcRect.bottom = 16;
				FillRect(hDC,&rcRect,GetSysColorBrush(COLOR_HIGHLIGHT));
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 50;
				blend.AlphaFormat = 0;
				AlphaBlend(pDraw->hDC,rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top,hDC,0,0,8,8,blend);
				SelectObject(hDC,hDefBitmap);
				DeleteObject(hBitmap);
				}
			DeleteDC(hDC);
			}
		}

	SetTextColor(pDraw->hDC,crText);
	SetBkMode(pDraw->hDC,iBkMode);
	SelectObject(pDraw->hDC,hDefFont);
	return;
}

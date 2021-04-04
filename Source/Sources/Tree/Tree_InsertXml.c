
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Insertion de LSX/XML
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Game.h"
#include "Texts.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	TreeViewInsertMenu[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_InsertXML(HWND hWnd, HTREEITEM hItem)
{
	XMLINSERT	ins;
	TVITEMEX	tvItem;

	ZeroMemory(&ins,sizeof(XMLINSERT));
	ins.hItem = hItem;

	//--- Retrouve l'entrée réelle ---

	tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
	tvItem.hItem = hItem;
	tvItem.lParam = 0;
	SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
	if (tvItem.lParam)
		{
		switch(((NODE *)tvItem.lParam)->type)
			{
			case XML_TYPE_NODE:
				ins.hItem = tvItem.hItem;
				break;
			case XML_TYPE_ATTR:
				// ATTR node can't contain other nodes so we pick up the parent node instead
				ins.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)tvItem.hItem);
				break;
			}
		if (ins.hItem != hItem) SendMessage(App.xmlTree.hwndTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)ins.hItem);
		}

	//--- Charge les insertions ---

	Game_LoadDataFile(hWnd,szInsertsDataPath,DATA_TYPE_INSERTIONS,&ins.inserts);

	//--- Affichage de la boîte de dialogue ---

	ins.dialogStruct.hwndParent = hWnd;
	ins.dialogStruct.pszTitle = Locale_GetText(IDM_TV_INSERTXML);
	ins.dialogStruct.pszText = Locale_GetText(TEXT_DIALOG_TV_INFO);
	ins.dialogStruct.uResID = 1013;
	ins.dialogStruct.fnInit = Tree_InsertXML_Init;
	ins.dialogStruct.fnClick = Tree_InsertXML_Click;
	ins.dialogStruct.fnOk = Tree_InsertXML_Ok;
	Dialog_Open((DIALOG *)&ins);

	Game_UnloadDataFile(DATA_TYPE_INSERTIONS,&ins.inserts);
	return;
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

int Tree_InsertXML_Init(HWND hDlg, RECT *rcDialog, int iHeight, void *pDialog)
{
	Dialog_OffsetY(hDlg,200,iHeight);
	Dialog_OffsetY(hDlg,210,iHeight);
	Dialog_OffsetY(hDlg,211,iHeight);
	Dialog_OffsetY(hDlg,212,iHeight);
	Dialog_OffsetY(hDlg,300,iHeight);
	Dialog_OffsetY(hDlg,301,iHeight);

	SetDlgItemText(hDlg,210,Locale_GetText(TEXT_DIALOG_TV_INSERT_BEFORE));
	SetDlgItemText(hDlg,211,Locale_GetText(TEXT_DIALOG_TV_INSERT_AFTER));
	SetDlgItemText(hDlg,212,Locale_GetText(TEXT_DIALOG_TV_INSERT_ASFIRSTCHILD));
	SetDlgItemText(hDlg,300,Locale_GetText(TEXT_DIALOG_TV_VERIFY));
	SetDlgItemText(hDlg,301,Locale_GetText(MENU_TVINSERT));

	CheckRadioButton(hDlg,210,212,210);

	if (!List_EntryCount(&((XMLINSERT *)pDialog)->inserts)) EnableWindow(GetDlgItem(hDlg,301),FALSE);

	SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
	SetFocus(GetDlgItem(hDlg,200));
	return(1);
}


// «»»» Sélection d'un bouton «««««««««««««««««««««««««««««««««««««««««««»

void Tree_InsertXML_Click(HWND hDlg, UINT uCtlID, void *pDialog)
{
	switch(uCtlID)
		{
		case 300:
			Tree_InsertXML_Verify(hDlg,(XMLINSERT *)pDialog);
			break;
		case 301:
			Tree_InsertXML_Menu(hDlg,GetDlgItem(hDlg,uCtlID),(XMLINSERT *)pDialog);
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Insertion de LSX							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Insertion «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Tree_InsertXML_Ok(HWND hDlg, void *pDialog)
{
	WCHAR*	pszXML;
	NODE	root;
	UINT	uLen;
	UINT	uMode;
	int	iResult;

	ZeroMemory(&root,sizeof(NODE));
	uMode = Tree_InsertXML_GetMode(hDlg);
	pszXML = NULL;

	//--- Récupération du contenu
	uLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
	if (!uLen) goto Error;

	pszXML = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
	if (!pszXML) goto Error;
	SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)uLen,(LPARAM)pszXML);

	//--- Conversion
	iResult = xml_LoadFile(hDlg,pszXML,&root,XML_FLAG_FROMMEMORY|XML_FLAG_DISPLAYMSG|XML_FLAG_NOSTATUSMSG|XML_FLAG_NOPROGRESS);
	if (!iResult) goto Error;
	else if (!Tree_InsertXML_Check(hDlg,&root,((XMLINSERT *)pDialog)->hItem,uMode)) goto Error;
	else
		{
		TV_INSERTSTRUCT	tvIns;
		TVITEMEX	tvItem;
		HTREEITEM	hItem;
		HTREEITEM	hParentItem;
		HTREEITEM	hPrevItem;
		XML_NODE*	pCurrentNode;
		XML_NODE*	pFirstNode;
		XML_NODE*	pWalkNode;
		XML_NODE*	pLastNode;

		hItem = NULL;
		pCurrentNode = NULL;
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.hItem = ((XMLINSERT *)pDialog)->hItem;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (tvItem.lParam && (pCurrentNode = (XML_NODE *)tvItem.lParam) && (hItem = tvItem.hItem))
			{
			switch(uMode)
				{
				case TV_INSERT_BEFORE:
					//--- Retrieve parent items
					hParentItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hItem);
					hPrevItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_PREVIOUS,(LPARAM)hItem);
					//--- Insert into treeview
					for (pLastNode = NULL, pFirstNode = pWalkNode = (XML_NODE *)root.next; pWalkNode != NULL; pWalkNode = (XML_NODE *)pWalkNode->node.next)
						{
						pWalkNode->parent = pCurrentNode->parent;
						if (pWalkNode->node.next == NULL) pLastNode = pWalkNode;
						tvIns.hParent = hParentItem?hParentItem:TVI_ROOT;
						tvIns.hInsertAfter = hPrevItem?hPrevItem:TVI_FIRST;
						tvIns.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
						tvIns.itemex.pszText = LPSTR_TEXTCALLBACK;
						tvIns.itemex.cChildren = (pWalkNode->children.next || pWalkNode->attributes.next)?I_CHILDRENCALLBACK:0;
						tvIns.itemex.lParam = (LPARAM)pWalkNode;
						hPrevItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvIns);
						}
					//--- Insert into struct
					pCurrentNode->node.prev->next = (NODE *)pFirstNode;
					pFirstNode->node.prev = pCurrentNode->node.prev;
					pLastNode->node.next = (NODE *)pCurrentNode;
					pCurrentNode->node.prev = (NODE *)pLastNode;
					root.next = NULL;
					break;
				case TV_INSERT_AFTER:
					//--- Retrieve parent items
					hParentItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)hItem);
					//--- Insert into treeview
					for (pLastNode = NULL, pFirstNode = pWalkNode = (XML_NODE *)root.next; pWalkNode != NULL; pWalkNode = (XML_NODE *)pWalkNode->node.next)
						{
						pWalkNode->parent = pCurrentNode->parent;
						if (pWalkNode->node.next == NULL) pLastNode = pWalkNode;
						tvIns.hParent = hParentItem?hParentItem:TVI_ROOT;
						tvIns.hInsertAfter = hItem;
						tvIns.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
						tvIns.itemex.pszText = LPSTR_TEXTCALLBACK;
						tvIns.itemex.cChildren = (pWalkNode->children.next || pWalkNode->attributes.next)?I_CHILDRENCALLBACK:0;
						tvIns.itemex.lParam = (LPARAM)pWalkNode;
						hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvIns);
						}
					//--- Insert into struct
					pFirstNode->node.prev = (NODE *)pCurrentNode;
					pLastNode->node.next = pCurrentNode->node.next;
					if (pCurrentNode->node.next) pCurrentNode->node.next->prev = (NODE *)pLastNode;
					pCurrentNode->node.next = (NODE *)pFirstNode;
					root.next = NULL;
					break;
				case TV_INSERT_ASCHILD:
					//--- Retrieve parent items
					hParentItem = hItem;
					hItem = NULL;
					//--- Déplie l'arborescence pour obtenir le contenu
					SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)hParentItem);
					//--- Retrieve the first non attr item in the treeview
					hPrevItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)hParentItem);
					while (hPrevItem)
						{
						tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
						tvItem.hItem = hPrevItem;
						tvItem.lParam = 0;
						SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
						if (tvItem.lParam && ((NODE *)tvItem.lParam)->type == XML_TYPE_NODE) break;
						hItem = hPrevItem;
						hPrevItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)hPrevItem);
						}
					//--- Insert into treeview
					for (pLastNode = NULL, pFirstNode = pWalkNode = (XML_NODE *)root.next; pWalkNode != NULL; pWalkNode = (XML_NODE *)pWalkNode->node.next)
						{
						pWalkNode->parent = pCurrentNode;
						if (pWalkNode->node.next == NULL) pLastNode = pWalkNode;
						tvIns.hParent = hParentItem;
						tvIns.hInsertAfter = hItem?hItem:TVI_FIRST;
						tvIns.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
						tvIns.itemex.pszText = LPSTR_TEXTCALLBACK;
						tvIns.itemex.cChildren = (pWalkNode->children.next || pWalkNode->attributes.next)?I_CHILDRENCALLBACK:0;
						tvIns.itemex.lParam = (LPARAM)pWalkNode;
						hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvIns);
						}
					//--- Insert into struct
					pFirstNode->node.prev = &pCurrentNode->children;
					pLastNode->node.next = pCurrentNode->children.next;
					if (pCurrentNode->children.next) pCurrentNode->children.next->prev = (NODE *)pLastNode;
					pCurrentNode->children.next = (NODE *)pFirstNode;
					root.next = NULL;
					//--- Update parent item
					tvItem.mask = TVIF_HANDLE|TVIF_CHILDREN;
					tvItem.cChildren = I_CHILDRENCALLBACK;
					tvItem.hItem = hParentItem;
					SendMessage(App.xmlTree.hwndTree,TVM_SETITEM,0,(LPARAM)&tvItem);
					SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)hParentItem);
					break;
				}
			}
		}

	//--- Fin
	HeapFree(App.hHeap,0,pszXML);
	xml_ReleaseAll(&root);
	return(1);

	//--- Erreur
Error:	if (pszXML) HeapFree(App.hHeap,0,pszXML);
	xml_ReleaseAll(&root);
	SetFocus(GetDlgItem(hDlg,200));
	return(0);
}


// «»»» Vérification (basique) ««««««««««««««««««««««««««««««««««««««««««»

void Tree_InsertXML_Verify(HWND hDlg, XMLINSERT *pInsert)
{
	WCHAR*	pszXML;
	NODE	root;
	UINT	uLen;
	int	iResult;

	ZeroMemory(&root,sizeof(NODE));

	//--- Récupération du contenu
	uLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
	if (!uLen)
		{
		SetFocus(GetDlgItem(hDlg,200));
		return;
		}
	pszXML = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
	if (!pszXML)
		{
		SetFocus(GetDlgItem(hDlg,200));
		return;
		}
	SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)uLen,(LPARAM)pszXML);

	//--- Conversion
	iResult = xml_LoadFile(hDlg,pszXML,&root,XML_FLAG_FROMMEMORY|XML_FLAG_DISPLAYMSG|XML_FLAG_NOSTATUSMSG|XML_FLAG_NOPROGRESS);
	if (iResult && Tree_InsertXML_Check(hDlg,&root,pInsert->hItem,Tree_InsertXML_GetMode(hDlg))) MessageBox(hDlg,Locale_GetText(TEXT_DIALOG_TV_NOERROR),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);

	//--- Libèration de la mémoire
	HeapFree(App.hHeap,0,pszXML);
	xml_ReleaseAll(&root);
	SetFocus(GetDlgItem(hDlg,200));
	return;
}


// «»»» Vérifications supplémentaires «««««««««««««««««««««««««««««««««««»

BOOL Tree_InsertXML_Check(HWND hDlg, NODE *pRoot, HTREEITEM hItem, UINT uMode)
{
	BOOL	bAbort;
	BOOL	bResult;

	if (pRoot->next == NULL)
		{
		MessageBox(hDlg,Locale_GetText(TEXT_DIALOG_TV_EMPTY),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
		return(FALSE);
		}

	bAbort = FALSE;
	bResult = TRUE;
	return(Tree_InsertXML_CheckNode(hDlg,pRoot,&bResult,&bAbort));
}

//--- Vérification de XML_NODE ---

BOOL Tree_InsertXML_CheckNode(HWND hDlg, NODE *pRoot, BOOL *pbResult, BOOL *pbAbort)
{
	XML_NODE*	pxn;

	for (pxn = (XML_NODE *)pRoot->next; pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
		{
		// Node allowed ?
		if (!Tree_IsNodeKnown(pxn->name))
			{
			*pbResult = FALSE;
			Tree_DisplayError(hDlg,TEXT_ERR_TV_BADNODE,pbAbort,pxn->name);
			if (*pbAbort) return(FALSE);
			}
		else if (pxn->attributes.next) // Check attributes for known nodes
			{
			// Node can have attributes ?
			if (!Tree_IsNodeHasAttributes(pxn->name))
				{
				Tree_DisplayError(hDlg,TEXT_ERR_TV_NODEHASNOATTR,pbAbort,pxn);
				if (*pbAbort) return(FALSE);
				}
			else // Check attributes
				{
				Tree_InsertXML_CheckAttr(hDlg,&pxn->attributes,pbResult,pbAbort);
				if (*pbAbort) return(*pbResult);
				}
			}
		if (pxn->children.next) // Check children
			{
			Tree_InsertXML_CheckNode(hDlg,&pxn->children,pbResult,pbAbort);
			if (*pbAbort) return(*pbResult);
			}
		}

	return(*pbResult);
}

//--- Vérification de XML_ATTR ---

BOOL Tree_InsertXML_CheckAttr(HWND hDlg, NODE *pRoot, BOOL *pbResult, BOOL *pbAbort)
{
	XML_ATTR*	pxa;

	for (pxa = (XML_ATTR *)pRoot->next; pxa != NULL; pxa = (XML_ATTR *)pxa->node.next)
		{
		if (!Tree_IsAttrKnown(pxa->name)) // Is attribute known ?
			{
			*pbResult = FALSE;
			Tree_DisplayError(hDlg,TEXT_ERR_TV_BADATTR,pbAbort,pxa->name,pxa->parent);
			if (*pbAbort) return(FALSE);
			}
		else if (!Tree_IsAttrValidForNode(pxa->name,((XML_NODE *)pxa->parent)->name)) // Is attribute valid for node ?
			{
			*pbResult = FALSE;
			Tree_DisplayError(hDlg,TEXT_ERR_TV_NODEBADATTR,pbAbort,pxa->name,pxa->parent);
			if (*pbAbort) return(FALSE);
			}
		else if (!Tree_IsValueValid(pxa->value)) // Is value valid ?
			{
			*pbResult = FALSE;
			Tree_DisplayError(hDlg,TEXT_ERR_TV_BADVALUE,pbAbort,pxa->value,pxa->name,pxa->parent);
			if (*pbAbort) return(FALSE);
			}
		}

	if (*pbResult != FALSE)
		{
		XMLNAMES *pNames = Tree_CollectNames(pRoot->next,NULL);
		Tree_CompareNames(hDlg,pRoot,pNames,pbResult,pbAbort);
		Tree_ReleaseNames(pNames);
		}

	return(*pbResult);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Méthode d'insertion «««««««««««««««««««««««««««««««««««««««««««««»

UINT Tree_InsertXML_GetMode(HWND hDlg)
{
	UINT	uMode;

	uMode = TV_INSERT_BEFORE;
	if (IsDlgButtonChecked(hDlg,211) == BST_CHECKED) uMode = TV_INSERT_AFTER;
	else if (IsDlgButtonChecked(hDlg,212) == BST_CHECKED) uMode = TV_INSERT_ASCHILD;
	return(uMode);
}


// «»»» Menu d'insertions «««««««««««««««««««««««««««««««««««««««««««««««»

void Tree_InsertXML_Menu(HWND hDlg, HWND hwndCtrl, XMLINSERT *pInsert)
{
	GAMEDATAINSERT*	pData;
	MENUITEMINFO	MenuItem;
	HMENU		hMenu;
	HMENU		hSubMenu;
	UINT		uMenuId;
	UINT		uFlags;
	POINT		ptCursor;
	int		i;

	hMenu = Menu_Create(TreeViewInsertMenu);
	if (!hMenu) return;

	//--- Positionnement du menu ---

	if (hwndCtrl)
		{
		RECT	rcWindow;

		GetWindowRect(hwndCtrl,&rcWindow);
		ptCursor.x = rcWindow.left;
		ptCursor.y = rcWindow.top;
		uFlags = TPM_LEFTALIGN|TPM_BOTTOMALIGN;
		}
	else
		{
		GetCursorPos(&ptCursor);
		uFlags = TPM_LEFTALIGN|TPM_TOPALIGN;
		}

	//--- Ajout des entrées ---

	hSubMenu = GetSubMenu(hMenu,0);

	for (i = IDM_CANCEL+1, pData = (GAMEDATAINSERT *)pInsert->inserts.next; pData != NULL; pData = (GAMEDATAINSERT *)pData->node.next, i++)
		{
		if (!pData->pszContent)
			{
			if (!wcscmp(pData->pszName,L"-"))
				{
				pData->cMenu.uType = MENU_SEPARATOR;
				MenuItem.fType = MFT_SEPARATOR;
				}
			else if (!wcscmp(pData->pszName,L"*"))
				{
				pData->cMenu.uType = MENU_BLANK;
				MenuItem.fType = MFT_OWNERDRAW|MFT_SEPARATOR;
				}
			else
				{
				pData->cMenu.pszText = pData->pszName;
				pData->cMenu.uTextLen = wcslen(pData->pszName);
				pData->cMenu.uSpecialFlags = MENU_FLAG_SEPARATOR;
				pData->cMenu.uType = MENU_ITEM;
				MenuItem.fType = MFT_OWNERDRAW|MFT_SEPARATOR;
				}
			}
		else
			{
			pData->cMenu.pszText = pData->pszName;
			pData->cMenu.uTextLen = wcslen(pData->pszName);
			pData->cMenu.uType = MENU_ITEM;
			MenuItem.fType = MFT_OWNERDRAW;
			}
		pData->cMenu.uId = i;
		MenuItem.cbSize = sizeof(MENUITEMINFO);
		MenuItem.fMask = MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_DATA;
		MenuItem.fState = 0;
		MenuItem.wID = i;
		MenuItem.dwItemData = (ULONG_PTR)&pData->cMenu;
		MenuItem.dwTypeData = NULL;
		InsertMenuItem(hSubMenu,0,FALSE,&MenuItem);
		}

	//--- Affichage du menu ---

	uMenuId = TrackPopupMenu(hSubMenu,uFlags|TPM_NOANIMATION|TPM_NONOTIFY|TPM_RETURNCMD,ptCursor.x,ptCursor.y,0,App.hWnd,NULL);
	Menu_Release(hMenu,TreeViewInsertMenu);

	if (uMenuId > IDM_CANCEL)
		{
		for (i = 1, uMenuId -= IDM_CANCEL, pData = (GAMEDATAINSERT *)pInsert->inserts.next; pData != NULL && i != uMenuId; pData = (GAMEDATAINSERT *)pData->node.next, i++);
		if (pData && pData->pszContent)
			{
			WCHAR*	pszBuffer;
			WCHAR*	pszEdit;
			UINT	uSelStart;
			UINT	uSelEnd;
			UINT	uLen;

			SendDlgItemMessage(hDlg,200,EM_GETSEL,(WPARAM)&uSelStart,(LPARAM)&uSelEnd);
			uLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
			if (!uLen)
				{
				SendDlgItemMessage(hDlg,200,WM_SETTEXT,0,(LPARAM)pData->pszContent);
				SendDlgItemMessage(hDlg,200,EM_SETSEL,0,wcslen(pData->pszContent));
				}
			else
				{
				pszEdit = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
				if (pszEdit)
					{
					pszBuffer = HeapAlloc(App.hHeap,0,(uLen-(uSelEnd-uSelStart)+wcslen(pData->pszContent))*sizeof(WCHAR));
					if (pszBuffer)
						{
						SendDlgItemMessage(hDlg,200,WM_GETTEXT,uLen,(LPARAM)pszEdit);
						if (uSelStart) CopyMemory(pszBuffer,pszEdit,uSelStart*sizeof(WCHAR));
						pszBuffer[uSelStart] = 0;
						wcscat(pszBuffer,pData->pszContent);
						wcscat(pszBuffer,&pszEdit[uSelEnd]);
						SendDlgItemMessage(hDlg,200,WM_SETTEXT,0,(LPARAM)pszBuffer);
						SendDlgItemMessage(hDlg,200,EM_SETSEL,(WPARAM)uSelStart,(LPARAM)uSelStart+wcslen(pData->pszContent));
						HeapFree(App.hHeap,0,pszBuffer);
						}
					HeapFree(App.hHeap,0,pszEdit);
					}
				}
			}
		}

	SetFocus(GetDlgItem(hDlg,200));
	return;
}


//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Insertion d'attributs
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

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_InsertAttribute(HWND hWnd, HTREEITEM hItem)
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
				if (!Tree_IsNodeHasAttributes(((XML_NODE *)tvItem.lParam)->name))
					{
					Tree_DisplayError(hWnd,TEXT_ERR_TV_NODEHASNOATTR,NULL,(XML_NODE *)tvItem.lParam);
					return;
					}
				break;
			case XML_TYPE_ATTR:
				ins.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_PARENT,(LPARAM)tvItem.hItem);
				break;
			}
		if (ins.hItem != hItem) SendMessage(App.xmlTree.hwndTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)ins.hItem);
		}

	//--- Affichage de la boîte de dialogue ---

	ins.dialogStruct.hwndParent = hWnd;
	ins.dialogStruct.pszTitle = Locale_GetText(IDM_TV_INSERTATTR);
	ins.dialogStruct.pszText = Locale_GetText(TEXT_DIALOG_TV_ATTR_INFO);
	ins.dialogStruct.uResID = 1014;
	ins.dialogStruct.fnInit = Tree_InsertAttr_Init;
	ins.dialogStruct.fnDraw = Tree_InsertAttr_Draw;
	ins.dialogStruct.fnOk = Tree_InsertAttr_Ok;
	Dialog_Open((DIALOG *)&ins);
	return;
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

int Tree_InsertAttr_Init(HWND hDlg, RECT *rcDialog, int iHeight, void *pInsert)
{
	Dialog_OffsetY(hDlg,200,iHeight);
	Dialog_OffsetY(hDlg,201,iHeight);
	Dialog_OffsetY(hDlg,202,iHeight);
	Dialog_OffsetY(hDlg,203,iHeight);

	SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
	SetFocus(GetDlgItem(hDlg,200));
	return(1);
}


// «»»» Affichage de la boîte de dialogue «««««««««««««««««««««««««««««««»

BOOL Tree_InsertAttr_Draw(UINT uCtlId, DRAWITEMSTRUCT *pDraw, void *pInsert)
{
	switch(uCtlId)
		{
		case 201:
			Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_TV_ATTR_NAME),pDraw,NULL,DT_RIGHT);
			return(TRUE);
		case 203:
			Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_TV_ATTR_VALUE),pDraw,NULL,DT_RIGHT);
			return(TRUE);
		}
	return(FALSE);
}


// «»»» Insertion «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Tree_InsertAttr_Ok(HWND hDlg, void *pInsert)
{
	TV_INSERTSTRUCT	tvInsItem;
	TVITEMEX	tvItem;
	XML_NODE*	pxn;
	XML_ATTR*	pxa;
	WCHAR*		pszName;
	WCHAR*		pszValue;
	HTREEITEM	hParentItem;
	HTREEITEM	hItem;
	HTREEITEM	hNextItem;
	UINT		uNameLen;
	UINT		uValueLen;

	pszName = NULL;
	pszValue = NULL;

	//--- Taille des textes

	uNameLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
	if (!uNameLen) goto Error;
	uValueLen = SendDlgItemMessage(hDlg,202,WM_GETTEXTLENGTH,0,0);

	//--- Retrouve le dernier attribut

	hParentItem = ((XMLINSERT *)pInsert)->hItem;
	hItem = NULL;

	tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
	tvItem.hItem = hParentItem;
	tvItem.lParam = 0;
	SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
	if (!(pxn = (XML_NODE *)tvItem.lParam)) goto Error;

	//--- Création des valeurs ---

	pszName = HeapAlloc(App.hHeap,0,++uNameLen*sizeof(WCHAR));
	if (!pszName) goto Error;
	SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)uNameLen,(LPARAM)pszName);
	uNameLen = Misc_HtmlSpecialChars(NULL,pszName);
	if (uNameLen)
		{
		WCHAR *pszCopy = HeapAlloc(App.hHeap,0,++uNameLen*sizeof(WCHAR));
		if (!pszCopy) goto Error;
		Misc_HtmlSpecialChars(pszCopy,pszName);
		HeapFree(App.hHeap,0,pszName);
		pszName = pszCopy;
		}

	if (uValueLen)
		{
		pszValue = HeapAlloc(App.hHeap,0,++uValueLen*sizeof(WCHAR));
		if (!pszValue) goto Error;
		SendDlgItemMessage(hDlg,202,WM_GETTEXT,(WPARAM)uValueLen,(LPARAM)pszValue);
		uValueLen = Misc_HtmlSpecialChars(NULL,pszValue);
		if (uValueLen)
			{
			WCHAR *pszCopy = HeapAlloc(App.hHeap,0,++uValueLen*sizeof(WCHAR));
			if (!pszCopy) goto Error;
			Misc_HtmlSpecialChars(pszCopy,pszValue);
			HeapFree(App.hHeap,0,pszValue);
			pszValue = pszCopy;
			}
		}

	//--- Vérification des attributs ---

	SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)hParentItem);

	hNextItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)hParentItem);
	while (hNextItem)
		{
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.hItem = hNextItem;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (tvItem.lParam && ((NODE *)tvItem.lParam)->type == XML_TYPE_NODE) break;
		if (!wcscmp(((XML_ATTR *)tvItem.lParam)->name,pszName))
			{
			Tree_DisplayError(hDlg,TEXT_ERR_TV_ATTREXISTS,NULL,pszName,((XML_ATTR *)tvItem.lParam)->parent);
			goto Error;
			}
		hItem = hNextItem;
		hNextItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)hNextItem);
		}
	if (!Tree_IsAttrValidForNode(pszName,pxn->name))
		{
		Tree_DisplayError(hDlg,TEXT_ERR_TV_NODEBADATTR,NULL,pszName,pxn);
		goto Error;
		}

	//--- Création de l'attribut XML ---

	pxa = xml_CreateAttr(pszName,pxn);
	if (!pxa) goto Error;
	xml_SetAttrValue(pxa,pszValue);

	//--- Création de l'attribut HITEM ---

	tvInsItem.hParent = hParentItem;
	tvInsItem.hInsertAfter = hItem?hItem:TVI_FIRST;
	tvInsItem.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
	tvInsItem.itemex.pszText = LPSTR_TEXTCALLBACK;
	tvInsItem.itemex.cChildren = 0;
	tvInsItem.itemex.lParam = (LPARAM)pxa;
	SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvInsItem);

	tvItem.mask = TVIF_HANDLE|TVIF_CHILDREN;
	tvItem.cChildren = I_CHILDRENCALLBACK;
	tvItem.hItem = hParentItem;
	SendMessage(App.xmlTree.hwndTree,TVM_SETITEM,0,(LPARAM)&tvItem);
	SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)hParentItem);

	if (pszValue) HeapFree(App.hHeap,0,pszValue);
	HeapFree(App.hHeap,0,pszName);
	return(1);

	//--- Erreur ---

Error:	if (pszValue) HeapFree(App.hHeap,0,pszValue);
	if (pszName) HeapFree(App.hHeap,0,pszName);
	SetFocus(GetDlgItem(hDlg,200));
	return(0);
}

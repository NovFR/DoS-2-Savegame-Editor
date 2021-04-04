
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Recherche
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Dialogs.h"
#include "Requests.h"
#include "Texts.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_SearchDialog(HWND hWnd)
{
	if (App.xmlTree.search.hDlg)
		{
		if (!Tree_SearchIsEnabled())
			{
			SetActiveWindow(App.xmlTree.search.hDlg);
			return;
			}
		Tree_Search(hWnd);
		return;
		}

	App.xmlTree.search.hDlg = CreateDialog(App.hInstance,MAKEINTRESOURCE(1015),hWnd,Tree_SearchDialogProc);
	if (!App.xmlTree.search.hDlg) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR|MB_OK);
	return;
}


// «»»» Messages de la boîte de dialogue ««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Tree_SearchDialogProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	if (uMsgId == WM_INITDIALOG)
		{
		RECT		rcDialog;
		int		i;
		int		iHeight = 0;
		static UINT	uCtlIds[] = { 150, 200, 201, 202, 203, 204, 205, 206, 207, 210, 250, 300, 301, 310, 0 };

		iHeight = Dialog_GetInfoHeight(hDlg,100,Locale_GetText(TEXT_DIALOG_TV_SEARCH_INFO));
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+iHeight,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,iHeight,SWP_NOZORDER|SWP_NOMOVE);

		if (App.xmlTree.search.bMoved)
			{
			SetWindowPos(hDlg,NULL,App.xmlTree.search.lLeft,App.xmlTree.search.lTop,0,0,SWP_NOZORDER|SWP_NOSIZE);
			}
		else
			{
			HMONITOR	hMonitor;
			MONITORINFO	mi;
			RECT		rcParent;
			int		iDialogWidth;

			mi.cbSize = sizeof(MONITORINFO);
			hMonitor = MonitorFromWindow(App.xmlTree.hWnd,MONITOR_DEFAULTTOPRIMARY);
			GetWindowRect(App.xmlTree.hWnd,&rcParent);
			GetWindowRect(hDlg,&rcDialog);
			GetMonitorInfo(hMonitor,&mi);
			iDialogWidth = rcDialog.right-rcDialog.left;
			rcDialog.left = rcParent.right-iDialogWidth/2;
			rcDialog.right = rcDialog.left+iDialogWidth;
			rcDialog.top = rcParent.top+GetSystemMetrics(SM_CYCAPTION)+40;
			if (rcDialog.right > mi.rcMonitor.right) rcDialog.left = mi.rcMonitor.right-iDialogWidth;
			SetWindowPos(hDlg,NULL,rcDialog.left,rcDialog.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			}

		for (i = 0; uCtlIds[i] != 0; i++) Dialog_OffsetY(hDlg,uCtlIds[i],iHeight);
		Dialog_OffsetY(hDlg,IDOK,iHeight);
		Dialog_OffsetY(hDlg,IDCANCEL,iHeight);

		SetDlgItemText(hDlg,200,App.xmlTree.search.pszId);
		SetDlgItemText(hDlg,202,App.xmlTree.search.pszType);
		SetDlgItemText(hDlg,204,App.xmlTree.search.pszValue);
		SetDlgItemText(hDlg,206,App.xmlTree.search.pszHandle);
		SetDlgItemText(hDlg,300,App.xmlTree.search.pszAll);
		CheckRadioButton(hDlg,150,250,App.xmlTree.search.uMode == TV_SEARCH_NODE?150:250);
		CheckDlgButton(hDlg,210,App.xmlTree.search.bMatchAny?BST_CHECKED:BST_UNCHECKED);

		SetDlgItemText(hDlg,150,Locale_GetText(TEXT_DIALOG_TV_SEARCH_NODE));
		SetDlgItemText(hDlg,250,Locale_GetText(TEXT_DIALOG_TV_SEARCH_ATTR));
		SetDlgItemText(hDlg,210,Locale_GetText(TEXT_DIALOG_TV_SEARCH_ANY));
		SetDlgItemText(hDlg,310,Locale_GetText(TEXT_DIALOG_TV_SEARCH_CASE));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_DIALOG_TV_SEARCH_NEXT));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CLOSE));

		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TV_SEARCH_TITLE));
		return(FALSE);
		}

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(Locale_GetText(TEXT_DIALOG_TV_SEARCH_INFO),(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				case 201:
					Dialog_DrawLabel(szXMLid,(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
					return(TRUE);
				case 203:
					Dialog_DrawLabel(szXMLtype,(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
					return(TRUE);
				case 205:
					Dialog_DrawLabel(szXMLvalue,(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
					return(TRUE);
				case 207:
					Dialog_DrawLabel(szXMLhandle,(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
					return(TRUE);
				case 301:
					Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_TV_SEARCH_STRING),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
					return(TRUE);
				}

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
						case 202:
						case 204:
						case 206:
						case 300:
							EnableWindow(GetDlgItem(hDlg,IDOK),Tree_SearchGetString(hDlg,LOWORD(wParam)));
							break;
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 150:
							App.xmlTree.search.uMode = TV_SEARCH_NODE;
							EnableWindow(GetDlgItem(hDlg,IDOK),Tree_SearchIsEnabled()?TRUE:FALSE);
							break;
						case 210:
							App.xmlTree.search.bMatchAny = IsDlgButtonChecked(hDlg,210) == BST_CHECKED?TRUE:FALSE;
							break;
						case 250:
							App.xmlTree.search.uMode = TV_SEARCH_ATTR;
							EnableWindow(GetDlgItem(hDlg,IDOK),Tree_SearchIsEnabled()?TRUE:FALSE);
							break;
						case 310:
							App.xmlTree.search.bCaseSensitive = IsDlgButtonChecked(hDlg,310) == BST_CHECKED?TRUE:FALSE;
							break;
						case IDOK:
							Tree_Search(hDlg);
							return(TRUE);

						case IDCANCEL:
							DestroyWindow(hDlg);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_MOVING:
			App.xmlTree.search.bMoved = TRUE;
			App.xmlTree.search.lLeft = ((RECT *)lParam)->left;
			App.xmlTree.search.lTop = ((RECT *)lParam)->top;
			return(FALSE);

		case WM_CLOSE:
			DestroyWindow(hDlg);
			return(TRUE);

		case WM_DESTROY:
			App.xmlTree.search.hDlg = NULL;
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Stockage des chaînes ««««««««««««««««««««««««««««««««««««««««««««»

BOOL Tree_SearchGetString(HWND hDlg, UINT uCtlId)
{
	WCHAR**	pszBuffer;
	UINT	uLen;

	switch(uCtlId)
		{
		case 200:
			pszBuffer = &App.xmlTree.search.pszId;
			break;
		case 202:
			pszBuffer = &App.xmlTree.search.pszType;
			break;
		case 204:
			pszBuffer = &App.xmlTree.search.pszValue;
			break;
		case 206:
			pszBuffer = &App.xmlTree.search.pszHandle;
			break;
		case 300:
			pszBuffer = &App.xmlTree.search.pszAll;
			break;
		default:return(FALSE);
		}

	if (*pszBuffer)
		{
		HeapFree(App.hHeap,0,*pszBuffer);
		*pszBuffer = NULL;
		}
	uLen = SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXTLENGTH,0,0);
	if (uLen)
		{
		*pszBuffer = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
		if (*pszBuffer) SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXT,(WPARAM)uLen,(LPARAM)*pszBuffer);
		}

	return(Tree_SearchIsEnabled()?TRUE:FALSE);
}


// «»»» Vérifie qu'il y a quelque chose à rechercher ««««««««««««««««««««»

DWORD Tree_SearchIsEnabled()
{
	DWORD	dwWatch = 0;

	switch (App.xmlTree.search.uMode)
		{
		case TV_SEARCH_NODE:
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszId,TREEVIEW_WATCH_ID);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszType,TREEVIEW_WATCH_TYPE);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszValue,TREEVIEW_WATCH_VALUE);
			dwWatch |= Tree_SearchStringIsOk(App.xmlTree.search.pszHandle,TREEVIEW_WATCH_HANDLE);
			break;
		case TV_SEARCH_ATTR:
			dwWatch = Tree_SearchStringIsOk(App.xmlTree.search.pszAll,TREEVIEW_WATCH_VALUE);
			break;
		}

	return(dwWatch);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Recherche							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Tree_Search(HWND hWnd)
{
	TVITEMEX	tvItem;
	HTREEITEM	hItem;
	NODE*		pCursor;
	NODE*		pNode;
	BOOL		bMatch;
	DWORD		dwMatch;
	DWORD		dwWatch;
	BOOL		bCaseSensitive;

	hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
	if (!hItem) return;

	tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
	tvItem.hItem = hItem;
	tvItem.lParam = 0;
	SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
	if (!(pNode = pCursor = (NODE *)tvItem.lParam)) return;

	dwWatch = Tree_SearchIsEnabled();
	if (!dwWatch) return;

	for (bMatch = FALSE, bCaseSensitive = App.xmlTree.search.bCaseSensitive;;)
		{
		pNode = Tree_SearchNextItem(pNode);
		if (pNode == pCursor) break;

		switch(pNode->type)
			{
			case XML_TYPE_NODE:
				if (App.xmlTree.search.uMode != TV_SEARCH_NODE) break;
				if (!((XML_NODE *)pNode)->attributes.next) break;
				dwMatch  = 0;
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLid),App.xmlTree.search.pszId,TREEVIEW_WATCH_ID,FALSE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLtype),App.xmlTree.search.pszType,TREEVIEW_WATCH_TYPE,TRUE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLvalue),App.xmlTree.search.pszValue,TREEVIEW_WATCH_VALUE,FALSE,bCaseSensitive);
				dwMatch |= Tree_SearchMatch(xml_GetAttrValue((XML_NODE *)pNode,szXMLhandle),App.xmlTree.search.pszHandle,TREEVIEW_WATCH_HANDLE,FALSE,bCaseSensitive);
				if (App.xmlTree.search.bMatchAny && dwMatch) bMatch = TRUE;
				else if (dwMatch == dwWatch) bMatch = TRUE;
				break;
			case XML_TYPE_ATTR:
				if (App.xmlTree.search.uMode != TV_SEARCH_ATTR) break;
				bMatch = Tree_SearchMatch(xml_GetThisAttrValue((XML_ATTR *)pNode),App.xmlTree.search.pszAll,TREEVIEW_WATCH_VALUE,FALSE,bCaseSensitive)?TRUE:FALSE;
				break;
			}

		if (bMatch)
			{
			Tree_SearchSelectItem(pNode);
			break;
			}
		}

	if (!bMatch) MessageBox(hWnd,Locale_GetText(TEXT_DIALOG_TV_SEARCH_NOTFOUND),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
	return;
}

//--- Vérifie si une chaîne de recherche est valide ---

DWORD Tree_SearchStringIsOk(WCHAR *pszSearch, DWORD dwFlag)
{
	if (!pszSearch) return(0);
	if (!*pszSearch) return(0);
	return(dwFlag);
}

//--- Vérifie si une chaîne correspond ---

DWORD Tree_SearchMatch(WCHAR *pszValue, WCHAR *pszSearch, DWORD dwFlag, BOOL bStrict, BOOL bCaseSensitive)
{
	if (!pszValue) return(0);
	if (!pszSearch) return(0);
	if (!*pszSearch) return(0);

	if (bStrict) return(wcscmp(pszValue,pszSearch) != 0?0:dwFlag);
	else if (!bCaseSensitive) return(StrStrI(pszValue,pszSearch) == NULL?0:dwFlag);
	else return(wcsstr(pszValue,pszSearch) == NULL?0:dwFlag);
}


// «»»» Recherche la prochaine entrée «««««««««««««««««««««««««««««««««««»

NODE* Tree_SearchNextItem(NODE *pEntry)
{
	XML_NODE*	pxnRoot;
	NODE*		pParent;

	if (!pEntry) return(NULL);

	switch(pEntry->type)
		{
		case XML_TYPE_NODE:
			if (((XML_NODE *)pEntry)->attributes.next) return(((XML_NODE *)pEntry)->attributes.next); // first attribute
			if (((XML_NODE *)pEntry)->children.next) return(((XML_NODE *)pEntry)->children.next); // first child
			if (pEntry->next) return(pEntry->next); // next entry
			pParent = ((XML_NODE *)pEntry)->parent; // parent node
			pxnRoot = Tree_GetRootNode(NULL); // root node
			if (!pxnRoot) return(NULL); // Should not happen !
			while (1) // Search for next parent
				{
				if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
				if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
				if (pParent->next) return(pParent->next); // Returns next parent
				pParent = ((XML_NODE *)pParent)->parent;
				}
			break;
		case XML_TYPE_ATTR:
			if (pEntry->next) return(pEntry->next); // next entry
			pParent = ((XML_ATTR *)pEntry)->parent; // parent node
			pxnRoot = Tree_GetRootNode(NULL); // root node
			if (!pxnRoot) return(NULL); // Should not happen !
			if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
			if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
			if (((XML_NODE *)pParent)->children.next) return(((XML_NODE *)pParent)->children.next); // Returns parent child
			while (1) // Search for next parent
				{
				if (!pParent) return((NODE *)pxnRoot); // Returns root entry (no parent)
				if (pParent == pxnRoot->parent) return((NODE *)pxnRoot); // Returns root entry (parent is outside the tree view)
				if (pParent->next) return(pParent->next); // Returns next parent
				pParent = ((XML_NODE *)pParent)->parent;
				}
			break;
		}

	return(NULL);
}


// «»»» Sélectionne une entrée ««««««««««««««««««««««««««««««««««««««««««»

void Tree_SearchSelectItem(NODE *pEntry)
{
	TVITEMEX	tvItem;
	XML_NODE*	pxnRoot;
	NODE*		pParent;
	NODE**		pParents;
	UINT		uCount;
	int		i;

	if (!pEntry) return;

	uCount = 0;
	pxnRoot = Tree_GetRootNode(&tvItem);
	if (!pxnRoot) return;

	// --- Builds the parents list ---
	//
	// pParents[0] = entry parent
	// pParents[x] = root

	for (pParent = pEntry;;)
		{
		uCount++;
		if (pParent->type == XML_TYPE_NODE) pParent = ((XML_NODE *)pParent)->parent;
		else if (pParent->type == XML_TYPE_ATTR) pParent = ((XML_ATTR *)pParent)->parent;
		if (pParent == pxnRoot->parent) break;
		if (!pParent) break;
		}

	pParents = HeapAlloc(App.hHeap,0,sizeof(NODE *)*uCount);
	if (!pParents) return;

	for (pParent = pEntry, i = 0; i < uCount; i++)
		{
		if (pParent->type == XML_TYPE_NODE) pParents[i] = pParent = ((XML_NODE *)pParent)->parent;
		else if (pParent->type == XML_TYPE_ATTR) pParents[i] = pParent = ((XML_ATTR *)pParent)->parent;
		if (pParent == pxnRoot->parent) break;
		if (!pParent) break;
		}

	// --- Selects the entry ---
	//
	// uCount = 1, entry is in root list
	// uCount > 1, entry is somewhere else. We need to expand parents to get the TreeView informations.

	if (uCount > 1) tvItem.hItem = Tree_SearchSelectedItemParent(tvItem.hItem,pParents,uCount-1);
	while (tvItem.hItem)
		{
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (!tvItem.lParam) break;
		if ((NODE *)tvItem.lParam != pEntry)
			{
			tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)tvItem.hItem);
			continue;
			}
		SendMessage(App.xmlTree.hwndTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)tvItem.hItem);
		if (pEntry->type == XML_TYPE_NODE)
			{
			SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_COLLAPSE,(LPARAM)tvItem.hItem);
			SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)tvItem.hItem);
			SendMessage(App.xmlTree.hwndTree,TVM_ENSUREVISIBLE,0,(LPARAM)tvItem.hItem);
			}
		break;
		}

	HeapFree(App.hHeap,0,pParents);
	return;
}

//--- Retrouve la liste de l'entrée et déplie les parents ---

HTREEITEM Tree_SearchSelectedItemParent(HTREEITEM hItem, NODE **pParents, UINT uCursor)
{
	TVITEMEX	tvItem;

	tvItem.hItem = hItem;

	while (tvItem.hItem)
		{
		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		if (!tvItem.lParam) return(NULL);
		if (
			(((NODE *)tvItem.lParam)->type == XML_TYPE_NODE)
			&& (
				(((XML_NODE *)tvItem.lParam)->attributes.next && ((XML_ATTR *)((XML_NODE *)tvItem.lParam)->attributes.next)->parent == pParents[uCursor-1])
				||
				(((XML_NODE *)tvItem.lParam)->children.next && ((XML_NODE *)((XML_NODE *)tvItem.lParam)->children.next)->parent == pParents[uCursor-1])
			)
		)
			{
			SendMessage(App.xmlTree.hwndTree,TVM_EXPAND,(WPARAM)TVE_EXPAND,(LPARAM)tvItem.hItem);
			tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)tvItem.hItem);
			if (--uCursor < 1) return(tvItem.hItem);
			}

		else tvItem.hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_NEXT,(LPARAM)tvItem.hItem);
		}

	return(NULL);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Libère la mémoire utilisée ««««««««««««««««««««««««««««««««««««««»

void Tree_SearchRelease()
{
	if (App.xmlTree.search.pszId) HeapFree(App.hHeap,0,App.xmlTree.search.pszId);
	if (App.xmlTree.search.pszType) HeapFree(App.hHeap,0,App.xmlTree.search.pszType);
	if (App.xmlTree.search.pszValue) HeapFree(App.hHeap,0,App.xmlTree.search.pszValue);
	if (App.xmlTree.search.pszHandle) HeapFree(App.hHeap,0,App.xmlTree.search.pszHandle);
	if (App.xmlTree.search.pszAll) HeapFree(App.hHeap,0,App.xmlTree.search.pszAll);

	App.xmlTree.search.pszId = NULL;
	App.xmlTree.search.pszType = NULL;
	App.xmlTree.search.pszValue = NULL;
	App.xmlTree.search.pszHandle = NULL;
	App.xmlTree.search.pszAll = NULL;
	return;
}
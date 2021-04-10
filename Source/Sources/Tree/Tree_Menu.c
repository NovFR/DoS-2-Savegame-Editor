
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Menu de la fenêtre principale
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Menus.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	TreeViewMenu[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions du menu						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Menu ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Tree_Menu(HWND hWnd, HWND hwndCtrl)
{
	HMENU		hMenu;
	HTREEITEM	hItem;
	UINT		uMenuId;
	NODE*		pSelected;

	//--- Menu contextuel ---

	if (!hwndCtrl)
		{
		TVHITTESTINFO	tvHitTest;

		GetCursorPos(&tvHitTest.pt);
		MapWindowPoints(NULL,App.xmlTree.hwndTree,&tvHitTest.pt,1);
		tvHitTest.flags = TVHT_ONITEMLABEL;
		tvHitTest.hItem = NULL;
		hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_HITTEST,0,(LPARAM)&tvHitTest);
		if (hItem) SendMessage(App.xmlTree.hwndTree,TVM_SELECTITEM,(WPARAM)TVGN_CARET,(LPARAM)hItem);
		}
	else

	//--- Entrée sélectionnée ---

	pSelected = NULL;
	hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,0);
	if (hItem)
		{
		TVITEMEX	tvItem;

		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.hItem = hItem;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		pSelected = (NODE *)tvItem.lParam;
		}

	//--- Menu ---

	uMenuId = 0;
	hMenu = Menu_Create(TreeViewMenu);
	if (hMenu)
		{
		XML_NODE*	pRoot;
		HMENU		hSubMenu;
		POINT		ptCursor;
		UINT		uFlags;

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

		pRoot = Tree_GetRootNode(NULL);
		if (pRoot) EnableMenuItem(hMenu,IDM_TV_SEARCH,MF_BYCOMMAND);

		if (pSelected)
			{
			EnableMenuItem(hMenu,IDM_TV_INSERTXML,MF_BYCOMMAND);
			EnableMenuItem(hMenu,IDM_TV_INSERTATTR,MF_BYCOMMAND);
			EnableMenuItem(hMenu,IDM_TV_REMOVE,MF_BYCOMMAND);
			if (pSelected->type == XML_TYPE_ATTR) EnableMenuItem(hMenu,IDM_TV_MODIFY,MF_BYCOMMAND);
			}

		hSubMenu = GetSubMenu(hMenu,0);
		uMenuId = TrackPopupMenu(hSubMenu,uFlags|TPM_NOANIMATION|TPM_NONOTIFY|TPM_RETURNCMD,ptCursor.x,ptCursor.y,0,App.hWnd,NULL);
		Menu_Release(hMenu,TreeViewMenu);
		}

	//--- Execute command ---

	switch(uMenuId)
		{
		case IDM_TV_INSERTXML:
			Tree_Command(hWnd,App.xmlTree.hwndTree,VK_INSERT,FALSE);
			break;
		case IDM_TV_INSERTATTR:
			Tree_Command(hWnd,App.xmlTree.hwndTree,VK_INSERT,TRUE);
			break;
		case IDM_TV_REMOVE:
			Tree_Command(hWnd,App.xmlTree.hwndTree,VK_DELETE,FALSE);
			break;
		case IDM_TV_MODIFY:
			Tree_Command(hWnd,App.xmlTree.hwndTree,VK_F2,FALSE);
			break;
		case IDM_TV_SEARCH:
			Tree_Command(hWnd,App.xmlTree.hwndTree,VK_F3,FALSE);
			break;
		}

	return;
}


// «»»» Exécution d'une commande ««««««««««««««««««««««««««««««««««««««««»

void Tree_Command(HWND hWnd, HWND hwndTree, WORD wVKey, BOOL bCtrl)
{
	HTREEITEM	hTreeItem;

	switch(wVKey)
		{
		case VK_INSERT:
			if (App.hThread) break;
			hTreeItem = (HTREEITEM)SendMessage(hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
			if (hTreeItem)
				{
				if (bCtrl) Tree_InsertAttribute(hWnd,hTreeItem);
				else Tree_InsertXML(hWnd,hTreeItem);
				}
			#if _DEBUG
			else
				{
				if (bCtrl) Tree_InsertAttribute(hWnd,NULL);
				else Tree_InsertXML(hWnd,NULL);
				}
			#endif
			break;

		case VK_RETURN:
			hTreeItem = (HTREEITEM)SendMessage(hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
			if (hTreeItem)
				{
				TVITEMEX	tvItem;

				tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
				tvItem.hItem = hTreeItem;
				tvItem.lParam = 0;
				SendMessage(hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
				if (tvItem.lParam)
					{
					switch(((NODE *)tvItem.lParam)->type)
						{
						case XML_TYPE_NODE:
							SendMessage(hwndTree,TVM_EXPAND,TVE_TOGGLE,(LPARAM)hTreeItem);
							break;
						case XML_TYPE_ATTR:
							if (App.hThread) break;
							SendMessage(hwndTree,TVM_EDITLABEL,0,(LPARAM)hTreeItem);
							break;
						}
					}
				}
			break;

		case VK_DELETE:
			if (App.hThread) break;
			if (Tree_Confirm(hWnd,Locale_GetText(TEXT_NODES_REMOVE),Locale_GetText(TEXT_TITLE_REQUEST),&App.xmlTree.bDontAskAgain))
				{
				hTreeItem = (HTREEITEM)SendMessage(hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
				if (hTreeItem)
					{
					TVITEMEX	tvItem;

					tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
					tvItem.hItem = hTreeItem;
					tvItem.lParam = 0;
					SendMessage(hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
					if (tvItem.lParam)
						{
						switch(((NODE *)tvItem.lParam)->type)
							{
							case XML_TYPE_NODE:
								xml_ReleaseNode((XML_NODE *)tvItem.lParam);
								break;
							case XML_TYPE_ATTR:
								xml_ReleaseAttr((XML_ATTR *)tvItem.lParam);
								break;
							}
						SendMessage(hwndTree,TVM_DELETEITEM,0,(LPARAM)hTreeItem);
						}
					}
				}
			SetFocus(App.xmlTree.hwndTree);
			break;

		case VK_F2:
			if (App.hThread) break;
			hTreeItem = (HTREEITEM)SendMessage(hwndTree,TVM_GETNEXTITEM,TVGN_CARET,0);
			if (hTreeItem) SendMessage(hwndTree,TVM_EDITLABEL,0,(LPARAM)hTreeItem);
			break;

		case VK_F3:
			Tree_SearchDialog(hWnd);
			break;
		}

	return;
}

//--- Confirmation de la suppression ---

BOOL Tree_Confirm(HWND hWnd, WCHAR *pszText, WCHAR *pszTitle, BOOL *bDontAskAgain)
{
	TASKDIALOGCONFIG	dialog;
	int			iResult;

	if (*bDontAskAgain) return(TRUE);

	ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
	dialog.cbSize = sizeof(TASKDIALOGCONFIG);
	dialog.hwndParent = hWnd;
	dialog.dwFlags = TDF_USE_HICON_MAIN|TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
	dialog.dwCommonButtons = TDCBF_YES_BUTTON|TDCBF_NO_BUTTON;
	dialog.pszWindowTitle = pszTitle;
	dialog.hMainIcon = App.hShellIcons[APP_SHELLICON_QUESTION];
	dialog.pszContent = pszText;
	dialog.nDefaultButton = IDNO;
	dialog.pszVerificationText = Locale_GetText(TEXT_DONTASKAGAIN);
	TaskDialogIndirect(&dialog,&iResult,NULL,bDontAskAgain);

	if (iResult == IDYES) return(TRUE);

	*bDontAskAgain = FALSE;
	return(FALSE);
}

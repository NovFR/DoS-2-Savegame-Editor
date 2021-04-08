
//<<>-<>>---------------------------------------------------------------------()
/*
	Liste XML sous forme de TreeView (Debug principalement)
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "XMLTree.h"
#include "Texts.h"
#include "Requests.h"
#include "Utils.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initalisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

void Tree_Open(XML_NODE *pxn)
{
	HWND		hWnd;
	int		X,Y,W,H;

	Tree_Destroy();
	App.xmlTree.pxn = pxn;

	if (App.Config.windowTreeView.usedefault.bSize)
		{
		W = TREEVIEW_WIDTH;
		H = TREEVIEW_HEIGHT;
		}
	else
		{
		W = App.Config.windowTreeView.position.iWidth;
		H = App.Config.windowTreeView.position.iHeight;
		}

	if (App.Config.windowTreeView.usedefault.bCoords)
		{
		RECT	rcWindow;

		GetWindowRect(App.hWnd,&rcWindow);
		X = rcWindow.left+((rcWindow.right-rcWindow.left)-W)/2;
		Y = rcWindow.top+((rcWindow.bottom-rcWindow.top)-H)/2;
		}
	else
		{
		X = App.Config.windowTreeView.position.iLeft;
		Y = App.Config.windowTreeView.position.iTop;
		}

	hWnd = CreateWindowEx(0,szWindowClass,szTitle,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_SIZEBOX,X,Y,W,H,NULL,NULL,App.hInstance,(void *)WINDOW_TREE);
	if (!hWnd)
		{
		App.xmlTree.pxn = NULL;
		App.xmlTree.hWnd = NULL;
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_WINDOW_CREATE),NULL,MB_ICONERROR);
		return;
		}

	ShowWindow(hWnd,SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	if (App.Config.bTVDebug) Tree_Debug(FALSE);
	return;
}

//--- WM_CREATE ---

LRESULT Tree_Create(HWND hWnd)
{
	RECT	rcClient;
	int	iButtonWidth,iButtonHeight;

	App.xmlTree.hWnd = hWnd;

	iButtonWidth = TREEVIEW_BUTTON_WIDTH;
	iButtonHeight = App.Font.uFontHeight+TREEVIEW_BUTTON_PADDING;

	GetClientRect(hWnd,&rcClient);
	App.xmlTree.hwndTree = CreateWindowEx(WS_EX_STATICEDGE,WC_TREEVIEW,NULL,WS_CHILD|WS_GROUP|WS_TABSTOP|TVS_DISABLEDRAGDROP|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS|TVS_EDITLABELS|TVS_TRACKSELECT,rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top,hWnd,(HMENU)CTLID_TREEVIEW,App.hInstance,NULL);
	if (!App.xmlTree.hwndTree) return(0);
	if (!Tree_CreateTreeView(App.xmlTree.pxn)) return(0);

	App.xmlTree.hwndMenu = CreateWindowEx(0,szButtonClass,NULL,WS_CHILD|WS_VISIBLE|WS_GROUP|WS_TABSTOP|BS_PUSHBUTTON|BS_TEXT|BS_CENTER|BS_VCENTER,0,0,iButtonWidth,iButtonHeight,hWnd,(HMENU)(UINT_PTR)CTLID_TREEMENU,App.hInstance,NULL);
	if (!App.xmlTree.hwndMenu) return(0);
	App.xmlTree.hwndDebug = CreateWindowEx(0,szButtonClass,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_CHECKBOX|BS_TEXT|BS_LEFT|BS_VCENTER,0,0,iButtonWidth,iButtonHeight,hWnd,(HMENU)(UINT_PTR)CTLID_TREEDEBUG,App.hInstance,NULL);
	if (!App.xmlTree.hwndDebug) return(0);
	App.xmlTree.hwndClose = CreateWindowEx(0,szButtonClass,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_TEXT|BS_CENTER|BS_VCENTER,0,0,iButtonWidth,iButtonHeight,hWnd,(HMENU)(UINT_PTR)CTLID_TREECLOSE,App.hInstance,NULL);
	if (!App.xmlTree.hwndClose) return(0);

	SendMessage(App.xmlTree.hwndClose,WM_SETFONT,(WPARAM)App.Font.hFont,0);
	SendMessage(App.xmlTree.hwndClose,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CLOSE));
	SendMessage(App.xmlTree.hwndMenu,WM_SETFONT,(WPARAM)App.Font.hFont,0);
	SendMessage(App.xmlTree.hwndMenu,WM_SETTEXT,0,(LPARAM)Locale_GetText(MENU_TVEDIT));
	SendMessage(App.xmlTree.hwndDebug,WM_SETFONT,(WPARAM)App.Font.hFont,0);
	SendMessage(App.xmlTree.hwndDebug,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DEBUG_TREE));

	ShowWindow(App.xmlTree.hwndTree,SW_SHOWNORMAL);
	return(1);
}

//--- Destruction de la fenêtre ---

void Tree_Destroy()
{
	if (!IsWindow(App.xmlTree.hWnd)) return;
	SendMessage(App.xmlTree.hWnd,WM_CLOSE,0,0);
	return;
}


// «»»» Création de la liste ««««««««««««««««««««««««««««««««««««««««««««»

int Tree_CreateTreeView(XML_NODE *pxn)
{
	if (!pxn) return(1);

	if (!Tree_CreateNodeTree(pxn,NULL))
		{
		Tree_DestroyTreeView();
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_TREE_CREATE),NULL,MB_ICONERROR);
		return(0);
		}

	if (pxn->parent)
		{
		WCHAR *Text = xml_AttrToWideChar((XML_NODE *)pxn->parent);
		if (Text)
			{
			Misc_SetWindowText(App.xmlTree.hWnd,&App.xmlTree.pszWindowTitle,szTitle,szTitleFmtLong,szTitle,Locale_GetText(TEXT_TITLE_TREE),Text);
			LocalFree(Text);
			}
		}
	else Misc_SetWindowText(App.xmlTree.hWnd,&App.xmlTree.pszWindowTitle,szTitle,szTitleFmt,szTitle,Locale_GetText(TEXT_TITLE_TREE));

	return(1);
}

//--- Création d'un noeud ---

int Tree_CreateNodeTree(XML_NODE *pxn, HTREEITEM hParent)
{
	XML_NODE*	pxnCurrent;
	TV_INSERTSTRUCT	tvItem;
	HTREEITEM	hItem;

	for (pxnCurrent = pxn; pxnCurrent != NULL; pxnCurrent = (XML_NODE *)pxnCurrent->node.next)
		{
		tvItem.hParent = hParent?hParent:TVI_ROOT;
		tvItem.hInsertAfter = TVI_LAST;
		tvItem.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
		tvItem.itemex.pszText = LPSTR_TEXTCALLBACK;
		tvItem.itemex.cChildren = (pxnCurrent->children.next || pxnCurrent->attributes.next)?I_CHILDRENCALLBACK:0;
		tvItem.itemex.lParam = (LPARAM)pxnCurrent;
		hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvItem);
		if (!hItem) return(0);
		}

	return(1);
}

//--- Création des valeurs ---

int Tree_CreateAttrTree(XML_NODE *pxn, HTREEITEM hParent)
{
	XML_ATTR*	pxa;
	TV_INSERTSTRUCT	tvItem;
	HTREEITEM	hItem;

	for (pxa = (XML_ATTR *)pxn->attributes.next; pxa != NULL; pxa = (XML_ATTR *)pxa->node.next)
		{
		tvItem.hParent = hParent;
		tvItem.hInsertAfter = TVI_LAST;
		tvItem.itemex.mask = TVIF_CHILDREN|TVIF_PARAM|TVIF_TEXT;
		tvItem.itemex.pszText = LPSTR_TEXTCALLBACK;
		tvItem.itemex.cChildren = 0;
		tvItem.itemex.lParam = (LPARAM)pxa;
		hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_INSERTITEM,0,(LPARAM)&tvItem);
		if (!hItem) return(0);
		}

	return(1);
}


// «»»» Destruction de la liste «««««««««««««««««««««««««««««««««««««««««»

void Tree_DestroyTreeView()
{
	Misc_SetWindowText(App.xmlTree.hWnd,&App.xmlTree.pszWindowTitle,szTitle,NULL);
	SendMessage(App.xmlTree.hwndTree,TVM_DELETEITEM,0,(LPARAM)TVI_ROOT);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des messages						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LRESULT Tree_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		case WM_PAINT:
			if (GetUpdateRect(hWnd,NULL,FALSE))
				{
				HDC		hDC;
				PAINTSTRUCT	paintStruct;
				RECT		rcWindow;

				hDC = BeginPaint(hWnd,&paintStruct);
				GetClientRect(hWnd,&rcWindow);
				FillRect(hDC,&rcWindow,GetSysColorBrush(COLOR_BTNFACE));
				EndPaint(hWnd,&paintStruct);
				}
			break;

		case WM_MOVING:
			Tree_DebugAttach((RECT *)lParam,NULL);
			break;

		case WM_MOVE:
			Tree_Move(hWnd);
			break;

		case WM_SIZING:
			if (((RECT *)lParam)->right-((RECT *)lParam)->left < TREEVIEW_MIN_WIDTH) ((RECT *)lParam)->right = ((RECT *)lParam)->left+TREEVIEW_MIN_WIDTH;
			if (((RECT *)lParam)->bottom-((RECT *)lParam)->top < TREEVIEW_MIN_HEIGHT) ((RECT *)lParam)->bottom = ((RECT *)lParam)->top+TREEVIEW_MIN_HEIGHT;
			break;

		case WM_SIZE:
			Tree_Resize(hWnd);
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case NM_RCLICK:
					if (((NMHDR *)lParam)->idFrom != CTLID_TREEVIEW) break;
					Tree_Menu(hWnd,NULL);
					break;

				case TVN_KEYDOWN:
					if (((NMHDR *)lParam)->idFrom != CTLID_TREEVIEW) break;
					Tree_Command(hWnd,((NMHDR *)lParam)->hwndFrom,((TV_KEYDOWN *)lParam)->wVKey,(GetAsyncKeyState(VK_CONTROL)));
					break;

				case TVN_SELCHANGED:
					if (((NMHDR *)lParam)->idFrom != CTLID_TREEVIEW) break;
					Tree_DebugUpdate(((NMTREEVIEW *)lParam)->itemNew.hItem);
					break;

				case TVN_BEGINLABELEDIT:
					if (((NMHDR *)lParam)->idFrom == CTLID_TREEVIEW)
						{
						NMTVDISPINFO*	pDispInfo;
						XML_ATTR*	pxa;
						HWND		hwndEdit;

						pDispInfo = (NMTVDISPINFO *)lParam;
						if (!pDispInfo->item.lParam) break;
						switch(((NODE *)pDispInfo->item.lParam)->type)
							{
							case XML_TYPE_NODE:
								return(1);
							case XML_TYPE_ATTR:
								pxa = (XML_ATTR *)pDispInfo->item.lParam;
								hwndEdit = (HWND)SendMessage(((NMHDR *)lParam)->hwndFrom,TVM_GETEDITCONTROL,0,0);
								SendMessage(hwndEdit,WM_SETTEXT,0,(LPARAM)pxa->value);
								App.xmlTree.bEditing = TRUE;
								App.xmlTree.bEditAborted = FALSE;
								break;
							}
						}
					break;

				case TVN_ENDLABELEDIT:
					if (((NMHDR *)lParam)->idFrom == CTLID_TREEVIEW)
						{
						NMTVDISPINFO*	pDispInfo;
						XML_ATTR*	pxa;
						WCHAR*		pszValue;

						App.xmlTree.bEditing = FALSE;
						if (App.xmlTree.bEditAborted)
							{
							App.xmlTree.bEditAborted = FALSE;
							break;
							}
						pDispInfo = (NMTVDISPINFO *)lParam;
						pxa = (XML_ATTR *)pDispInfo->item.lParam;
						if (!pDispInfo->item.lParam) break;
						if (pDispInfo->item.pszText)
							{
							if (wcslen(pDispInfo->item.pszText))
								{
								pszValue = HeapAlloc(App.hHeap,0,wcslen(pDispInfo->item.pszText)*sizeof(WCHAR)+sizeof(WCHAR));
								if (pszValue)
									{
									if (pxa->value) HeapFree(App.hHeap,0,pxa->value);
									pxa->value = pszValue;
									wcscpy(pxa->value,pDispInfo->item.pszText);
									return(1);
									}
								}
							else
								{
								if (pxa->value) HeapFree(App.hHeap,0,pxa->value);
								pxa->value = NULL;
								return(1);
								}
							}
						}
					break;

				case TVN_ITEMEXPANDING:
					if (((NMHDR *)lParam)->idFrom == CTLID_TREEVIEW)
						{
						NMTREEVIEW*	pTreeView;
						XML_NODE*	pxn;

						pTreeView = (NMTREEVIEW *)lParam;
						if (pTreeView->action&TVE_EXPAND && !SendMessage(pTreeView->hdr.hwndFrom,TVM_GETNEXTITEM,(WPARAM)TVGN_CHILD,(LPARAM)pTreeView->itemNew.hItem))
							{
							pxn = (XML_NODE *)pTreeView->itemNew.lParam;
							if (pxn && pxn->node.type == XML_TYPE_NODE)
								{
								if (pxn->attributes.next) Tree_CreateAttrTree(pxn,pTreeView->itemNew.hItem);
								if (pxn->children.next) Tree_CreateNodeTree((XML_NODE *)pxn->children.next,pTreeView->itemNew.hItem);
								}
							}
						}
					break;

				case TVN_GETDISPINFO:
					if (((NMHDR *)lParam)->idFrom == CTLID_TREEVIEW)
						{
						NMTVDISPINFO*	pDispInfo;
						XML_NODE*	pxn;
						XML_ATTR*	pxa;
						WCHAR*		pszText;
						int		iMax;

						pDispInfo = (NMTVDISPINFO *)lParam;
						iMax = pDispInfo->item.cchTextMax;
						switch(((NODE *)pDispInfo->item.lParam)->type)
							{
							case XML_TYPE_NODE:
								pxn = (XML_NODE *)pDispInfo->item.lParam;
								if (pDispInfo->item.mask&TVIF_CHILDREN)
									{
									pDispInfo->item.cChildren = (pxn->children.next || pxn->attributes.next)?1:0;
									}
								if (pDispInfo->item.mask&TVIF_TEXT)
									{
									Misc_StrCpy(pDispInfo->item.pszText,iMax,pxn->name);
									pszText = xml_GetAttrValue(pxn,szXMLid);
									if (!pszText) break;
									Misc_StrCat(pDispInfo->item.pszText,iMax,L"[");
									Misc_StrCat(pDispInfo->item.pszText,iMax,szXMLid);
									Misc_StrCat(pDispInfo->item.pszText,iMax,L"=");
									Misc_StrCat(pDispInfo->item.pszText,iMax,pszText);
									Misc_StrCat(pDispInfo->item.pszText,iMax,L"]");
									}
								break;
							case XML_TYPE_ATTR:
								pxa = (XML_ATTR *)pDispInfo->item.lParam;
								if (pDispInfo->item.mask&TVIF_CHILDREN)
									{
									pDispInfo->item.cChildren = 0;
									}
								if (pDispInfo->item.mask&TVIF_TEXT)
									{
									Misc_StrCpy(pDispInfo->item.pszText,iMax,pxa->name);
									Misc_StrCat(pDispInfo->item.pszText,iMax,L"=\"");
									if (pxa->value) Misc_StrCat(pDispInfo->item.pszText,iMax,pxa->value);
									Misc_StrCat(pDispInfo->item.pszText,iMax,L"\"");
									}
								break;
							}
						}
					break;
				}
			break;

		case WM_ACTIVATE:
			switch(LOWORD(wParam))
				{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					SetFocus(App.xmlTree.hwndTree);
					break;
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case CTLID_TREEMENU:
							Tree_Menu(hWnd,(HWND)lParam);
							break;
						case CTLID_TREECLOSE:
							PostMessage(App.xmlTree.hWnd,WM_CLOSE,0,0);
							break;
						case CTLID_TREEDEBUG:
							App.Config.bTVDebug = SendMessage(App.xmlTree.hwndDebug,BM_GETCHECK,0,0) == BST_CHECKED?FALSE:TRUE;
							Tree_Debug(FALSE);
							break;
						case IDOK:
							if (App.xmlTree.bEditing) SendMessage(App.xmlTree.hwndTree,TVM_ENDEDITLABELNOW,0,(LPARAM)FALSE);
							else Tree_Command(hWnd,App.xmlTree.hwndTree,VK_RETURN,FALSE);
							break;
						case IDCANCEL:
							if (App.xmlTree.bEditing)
								{
								App.xmlTree.bEditAborted = TRUE;
								SendMessage(App.xmlTree.hwndTree,TVM_ENDEDITLABELNOW,0,(LPARAM)TRUE);
								}
							break;
						}
					break;
				}
			break;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			Tree_Debug(TRUE);
			Tree_SearchRelease();
			Misc_SetWindowText(App.xmlTree.hWnd,&App.xmlTree.pszWindowTitle,szTitle,NULL);
			App.xmlTree.hWnd = NULL;
			App.xmlTree.bDontAskAgain = FALSE;
			App.xmlTree.bEditing = FALSE;
			App.xmlTree.bEditAborted = FALSE;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Déplacement de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Tree_Move(HWND hWnd)
{
	RECT	rcWindow;

	GetWindowRect(hWnd,&rcWindow);
	App.Config.windowTreeView.position.iLeft = rcWindow.left;
	App.Config.windowTreeView.position.iTop = rcWindow.top;
	return;
}


// «»»» Redimension de la fenêtre «««««««««««««««««««««««««««««««««««««««»

void Tree_Resize(HWND hWnd)
{
	RECT	rcWindow;
	RECT	rcClient;
	int	iButtonWidth,iButtonHeight;
	int	Y;

	GetWindowRect(hWnd,&rcWindow);
	Tree_DebugAttach(&rcWindow,NULL);
	App.Config.windowTreeView.position.iWidth = rcWindow.right-rcWindow.left;
	App.Config.windowTreeView.position.iHeight = rcWindow.bottom-rcWindow.top;

	GetClientRect(hWnd,&rcWindow);
	iButtonWidth = TREEVIEW_BUTTON_WIDTH;
	iButtonHeight = App.Font.uFontHeight+TREEVIEW_BUTTON_PADDING;

	rcClient.left = TREEVIEW_MARGIN_LEFT;
	rcClient.top = TREEVIEW_MARGIN_TOP;
	rcClient.right = rcWindow.right-TREEVIEW_MARGIN_RIGHT;
	rcClient.bottom = rcWindow.bottom-(TREEVIEW_MARGIN_BOTTOM+iButtonHeight+TREEVIEW_BUTTON_MARGIN_BOTTOM);
	SetWindowPos(App.xmlTree.hwndTree,NULL,rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	GetClientRect(hWnd,&rcClient);
	Y = rcClient.bottom-iButtonHeight-TREEVIEW_BUTTON_MARGIN_BOTTOM;
	SetWindowPos(App.xmlTree.hwndClose,NULL,rcClient.right-iButtonWidth-TREEVIEW_BUTTON_MARGIN_RIGHT,Y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	SetWindowPos(App.xmlTree.hwndMenu,NULL,TREEVIEW_BUTTON_MARGIN_LEFT,Y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	GetWindowRect(App.xmlTree.hwndDebug,&rcWindow);
	rcWindow.left = TREEVIEW_BUTTON_MARGIN_LEFT+iButtonWidth+TREEVIEW_BUTTON_SPACING;
	rcWindow.right = rcClient.right-iButtonWidth-TREEVIEW_BUTTON_MARGIN_RIGHT-TREEVIEW_BUTTON_SPACING;
	SetWindowPos(App.xmlTree.hwndDebug,NULL,rcWindow.left,Y,rcWindow.right-rcWindow.left,rcWindow.bottom-rcWindow.top,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	InvalidateRect(hWnd,NULL,FALSE);
	return;
}

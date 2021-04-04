
//<<>-<>>---------------------------------------------------------------------()
/*
	TreeView: Fenêtre de déboguage
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fenêtre de déboguage						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage/Fermeture de la fenêtre «««««««««««««««««««««««««««««««»

void Tree_Debug(BOOL bDestroy)
{
	HWND	hWnd;

	if (App.xmlTree.debug.hWnd)
		{
		DestroyWindow(App.xmlTree.debug.hWnd);
		return;
		}
	if (bDestroy) return;

	hWnd = CreateWindowEx(WS_EX_NOACTIVATE|WS_EX_TOOLWINDOW,szWindowClass,Locale_GetText(TEXT_DEBUG_TREE),WS_CAPTION,0,0,300,400,App.xmlTree.hWnd,NULL,App.hInstance,(void *)WINDOW_TREE_DEBUG);
	if (hWnd)
		{
		ShowWindow(hWnd,SW_SHOWNOACTIVATE);
		UpdateWindow(hWnd);
		}

	return;
}


// «»»» Création de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

int Tree_DebugCreate(HWND hWnd)
{
	SendMessage(App.xmlTree.hwndDebug,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
	App.xmlTree.debug.hWnd = hWnd;
	Tree_DebugAttach(NULL,NULL);
	Tree_DebugUpdate(NULL);
	return(0);
}


// «»»» Messages de la fenêtre ««««««««««««««««««««««««««««««««««««««««««»

LRESULT Tree_DebugMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
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
				Tree_DebugPaint(hDC,&rcWindow,TRUE);
				EndPaint(hWnd,&paintStruct);
				}
			break;

		case WM_MOVING:
			Tree_DebugAttach(NULL,(RECT *)lParam);
			break;

		case WM_DESTROY:
			SendMessage(App.xmlTree.hwndDebug,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
			App.xmlTree.debug.hWnd = NULL;
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Positionnement forcé de la fenêtre ««««««««««««««««««««««««««««««»

void Tree_DebugAttach(RECT *rcParent, RECT *rcWindow)
{
	RECT	rcNew;

	if (!App.xmlTree.debug.hWnd) return;

	if (!rcParent)
		GetWindowRect(App.xmlTree.hWnd,&rcNew);
	else
		CopyRect(&rcNew,rcParent);

	if (rcWindow)
		{
		rcWindow->right -= rcWindow->left;
		rcWindow->bottom -= rcWindow->top;
		rcWindow->left = rcNew.right;
		rcWindow->top = rcNew.top;
		rcWindow->right += rcWindow->left;
		rcWindow->bottom += rcWindow->top;
		}

	SetWindowPos(App.xmlTree.debug.hWnd,NULL,rcNew.right,rcNew.top,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOSIZE);
	return;
}


// «»»» Mise-à-jour de l'affichage ««««««««««««««««««««««««««««««««««««««»

void Tree_DebugUpdate(HTREEITEM hItem)
{
	HDC	hDC;
	RECT	rcWindow;
	RECT	rcClient;

	if (!App.xmlTree.debug.hWnd)
		return;

	if (!hItem)
		hItem = (HTREEITEM)SendMessage(App.xmlTree.hwndTree,TVM_GETNEXTITEM,(WPARAM)TVGN_CARET,0);

	if (hItem)
		{
		TVITEMEX	tvItem;

		tvItem.mask = TVIF_HANDLE|TVIF_PARAM;
		tvItem.hItem = hItem;
		tvItem.lParam = 0;
		SendMessage(App.xmlTree.hwndTree,TVM_GETITEM,0,(LPARAM)&tvItem);
		App.xmlTree.debug.node = (NODE *)tvItem.lParam;
		}
	else App.xmlTree.debug.node = NULL;

	GetWindowRect(App.xmlTree.debug.hWnd,&rcWindow);
	GetClientRect(App.xmlTree.debug.hWnd,&rcClient);
	hDC = GetDC(App.xmlTree.debug.hWnd);
	if (hDC)
		{
		rcWindow.bottom -= rcWindow.top+rcClient.bottom;
		rcWindow.bottom += Tree_DebugPaint(hDC,&rcClient,FALSE);
		ReleaseDC(App.xmlTree.debug.hWnd,hDC);
		SetWindowPos(App.xmlTree.debug.hWnd,NULL,0,0,rcWindow.right-rcWindow.left,rcWindow.bottom,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
		}
	InvalidateRect(App.xmlTree.debug.hWnd,NULL,FALSE);
	return;
}


// «»»» Affichage «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Tree_DebugPaint(HDC hDC, RECT *rcWindow, BOOL bRender)
{
	NODE*		pNode;
	XML_NODE*	pxn;
	XML_ATTR*	pxa;
	HFONT		hFont;
	RECT		rcArea;
	COLORREF	crColor;
	int		iBkMode;
	int		iHeight;

	hFont = SelectFont(hDC,GetStockObject(SYSTEM_FIXED_FONT));
	crColor = SetTextColor(hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(hDC,TRANSPARENT);
	iHeight = TREEVIEW_DEBUG_PADDING*2;

	CopyRect(&rcArea,rcWindow);
	rcArea.top += TREEVIEW_DEBUG_PADDING;
	rcArea.left += TREEVIEW_DEBUG_PADDING;
	rcArea.right -= TREEVIEW_DEBUG_PADDING;

	if (App.xmlTree.debug.node)
		{
		iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"ROOT",Tree_GetRootNode(NULL),0,bRender);
		switch(App.xmlTree.debug.node->type)
			{
			case XML_TYPE_NODE:
				pxn = (XML_NODE *)App.xmlTree.debug.node;
				iHeight += Tree_DebugPaintLF(16,&rcArea,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"XML_NODE",pxn,0,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"next",pxn->node.next,1,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"prev",pxn->node.prev,1,bRender);
				iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"parent",pxn->parent,1,bRender);
				if (pxn->attributes.next)
					{
					iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"attributes",&pxn->attributes,1,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"next",pxn->attributes.next,2,bRender);
					}
				if (pxn->children.next)
					{
					iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"children",&pxn->children,1,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"next",pxn->children.next,2,bRender);
					}
				if (pxn->content)
					{
					iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"content",pxn->content,1,bRender);
					}
				break;
			case XML_TYPE_ATTR:
				pxa = (XML_ATTR *)App.xmlTree.debug.node;
				iHeight += Tree_DebugPaintLF(16,&rcArea,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"XML_ATTR",pxa,0,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"next",pxa->node.next,1,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"prev",pxa->node.prev,1,bRender);
				iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"parent",pxa->parent,1,bRender);
				iHeight += Tree_DebugPaintLF(8,&rcArea,bRender);
				iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"value",pxa->value,1,bRender);
				break;
			}
			pNode = Tree_SearchNextItem(App.xmlTree.debug.node);
			switch(pNode->type)
				{
				case XML_TYPE_NODE:
					iHeight += Tree_DebugPaintLF(16,&rcArea,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"> XML_NODE",pNode,0,bRender);
					break;
				case XML_TYPE_ATTR:
					iHeight += Tree_DebugPaintLF(16,&rcArea,bRender);
					iHeight += Tree_DebugPaintDrawAddr(hDC,&rcArea,L"> XML_ATTR",pNode,0,bRender);
					break;
				}
		}

	SetBkMode(hDC,iBkMode);
	SetTextColor(hDC,crColor);
	SelectFont(hDC,hFont);
	return(iHeight);
}

//--- Saut à la la ligne ---

int Tree_DebugPaintLF(int iSize, RECT *rcArea, BOOL bRender)
{
	rcArea->top += iSize;
	return(iSize);
}

//--- Affichage d'une adresse ---

int Tree_DebugPaintDrawAddr(HDC hDC, RECT *rcArea, WCHAR *pszLabel, void *pAddr, int iTabs, BOOL bRender)
{
	RECT	rcText;
	SIZE	sizeLabel;

	CopyRect(&rcText,rcArea);
	GetTextExtentPoint32(hDC,pszLabel,wcslen(pszLabel),&sizeLabel);

	if (bRender)
		{
		WCHAR	szBuffer[20];
		RECT	rcEdge;
		SIZE	sizeAddr;

		if (iTabs)
			{
			CopyRect(&rcEdge,&rcText);

			rcEdge.left += iTabs*22;
			MoveToEx(hDC,rcEdge.left,rcEdge.top,NULL);
			rcEdge.top += sizeLabel.cy/2;
			LineTo(hDC,rcEdge.left,rcEdge.top);

			rcEdge.right = rcEdge.left+16;
			rcEdge.bottom = rcEdge.top+2;
			MoveToEx(hDC,rcEdge.left,rcEdge.top,NULL);
			LineTo(hDC,rcEdge.right,rcEdge.top);
			rcText.left = rcEdge.right+4;
			}

		rcText.bottom = rcText.top+sizeLabel.cy;
		DrawTextEx(hDC,pszLabel,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER,NULL);
		rcText.left += sizeLabel.cx;

		Misc_Printf(szBuffer,20,L"0x%llX",pAddr);
		DrawTextEx(hDC,szBuffer,-1,&rcText,DT_END_ELLIPSIS|DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER,NULL);

		GetTextExtentPoint32(hDC,szBuffer,wcslen(szBuffer),&sizeAddr);
		rcEdge.left = rcText.left+4;
		rcEdge.top = rcText.top+sizeLabel.cy/2;
		rcEdge.right = rcArea->right-sizeAddr.cx-4;
		if (rcEdge.right-rcEdge.left > 0)
			{
			MoveToEx(hDC,rcEdge.left,rcEdge.top,NULL);
			LineTo(hDC,rcEdge.right,rcEdge.top);
			}
		}

	rcArea->top += sizeLabel.cy+2;
	return(sizeLabel.cy+2);
}

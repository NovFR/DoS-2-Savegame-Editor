
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des messages de la fenêtre des attributs
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Game.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Conteneur							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation de la fenêtre ««««««««««««««««««««««««««««««««««««»

LRESULT Game_ContainerCreate(HWND hWnd)
{
	RECT	rcWindow;

	GetClientRect(hWnd,&rcWindow);

	App.Game.Layout.hwndDecoTop = CreateWindowEx(WS_EX_TRANSPARENT|WS_EX_TOPMOST,szStaticClass,NULL,WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,0,0,rcWindow.right-rcWindow.left,WINDOW_DECO_HEIGHT,hWnd,(HMENU)CTLID_ATTRS_DECOTOP,App.hInstance,NULL);
	if (!App.Game.Layout.hwndDecoTop) return(-1);
	App.Game.Layout.hwndDecoBottom = CreateWindowEx(WS_EX_TRANSPARENT|WS_EX_TOPMOST,szStaticClass,NULL,WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,0,0,rcWindow.right-rcWindow.left,WINDOW_DECO_HEIGHT,hWnd,(HMENU)CTLID_ATTRS_DECOBOTTOM,App.hInstance,NULL);
	if (!App.Game.Layout.hwndDecoBottom) return(-1);

	App.Game.Layout.hwndScrollbar = CreateWindowEx(WS_EX_TOPMOST,szScrollClass,NULL,WS_CHILD|SBS_VERT,0,0,MAIN_ATTR_SCROLLBAR_WIDTH,100,hWnd,(HMENU)CTLID_ATTRS_SCROLLBAR,App.hInstance,NULL);
	if (!App.Game.Layout.hwndScrollbar) return(-1);
	App.Game.Layout.hwndAttributes = CreateWindowEx(WS_EX_CONTROLPARENT,szWindowClass,NULL,WS_CHILD|WS_VISIBLE,0,0,rcWindow.right-rcWindow.left,100,hWnd,NULL,App.hInstance,(void *)WINDOW_GAME_ATTRIBUTES);
	if (!App.Game.Layout.hwndAttributes) return(-1);

	return(0);
}


// «»»» Messages provenant de la fenêtre ««««««««««««««««««««««««««««««««»

LRESULT Game_ProcessContainerMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(((DRAWITEMSTRUCT *)lParam)->CtlID)
				{
				case CTLID_ATTRS_DECOTOP:
					Game_PaintDecorationBorder((DRAWITEMSTRUCT *)lParam,TRUE);
					Game_PaintDecoration(((DRAWITEMSTRUCT *)lParam)->hDC,-8,0,App.Game.Layout.hDecoTop);
					break;
				case CTLID_ATTRS_DECOBOTTOM:
					Game_PaintDecorationBorder((DRAWITEMSTRUCT *)lParam,FALSE);
					Game_PaintDecoration(((DRAWITEMSTRUCT *)lParam)->hDC,-8,0,App.Game.Layout.hDecoBottom);
					break;
				}
			break;

		case WM_PAINT:
			if (GetUpdateRect(hWnd,NULL,FALSE))
				{
				HDC		hDC;
				PAINTSTRUCT	paintStruct;
				RECT		rcWindow;

				hDC = BeginPaint(hWnd,&paintStruct);
				GetClientRect(hWnd,&rcWindow);
				FillRect(hDC,&rcWindow,GetSysColorBrush(COLOR_WINDOW));
				EndPaint(hWnd,&paintStruct);
				}
			break;

		case WM_VSCROLL:
			Game_ContainerScroll(LOWORD(wParam));
			break;

		case WM_SIZE:
			Game_ContainerResize(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_MOUSEWHEEL:
			Game_ContainerMouseWheel(hWnd,(int)GET_WHEEL_DELTA_WPARAM(wParam));
			break;

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Redimension du conteneur ««««««««««««««««««««««««««««««««««««««««»

void Game_ContainerResize(HWND hWnd, int iWidth, int iHeight)
{
	RECT	rcAttributes;

	GetWindowRect(App.Game.Layout.hwndAttributes,&rcAttributes);
	MapWindowPoints(NULL,hWnd,(POINT *)&rcAttributes,2);

	rcAttributes.right = rcAttributes.left+iWidth;
	if (rcAttributes.bottom-rcAttributes.top > iHeight) rcAttributes.right -= MAIN_ATTR_SCROLLBAR_TOTALWIDTH-MAIN_ATTR_RIGHTMARGIN;
	SetWindowPos(App.Game.Layout.hwndAttributes,NULL,rcAttributes.left,Game_ContainerTopOffset(rcAttributes.top,&rcAttributes),rcAttributes.right-rcAttributes.left,rcAttributes.bottom-rcAttributes.top,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	SetWindowPos(App.Game.Layout.hwndScrollbar,NULL,iWidth-MAIN_ATTR_SCROLLBAR_TOTALWIDTH+(MAIN_ATTR_SCROLLBAR_TOTALWIDTH-MAIN_ATTR_SCROLLBAR_WIDTH)/2,MAIN_ATTR_SCROLLBAR_VERTMARGIN,MAIN_ATTR_SCROLLBAR_WIDTH,iHeight-MAIN_ATTR_SCROLLBAR_VERTMARGIN*2,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	SetWindowPos(App.Game.Layout.hwndDecoTop,NULL,0,0,iWidth,WINDOW_DECO_HEIGHT,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	SetWindowPos(App.Game.Layout.hwndDecoBottom,NULL,0,iHeight-WINDOW_DECO_HEIGHT,iWidth,WINDOW_DECO_HEIGHT,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	Game_ContainerScrollInfo(SIF_PAGE|SIF_RANGE);
	return;
}


// «»»» Déplacement des attributs «««««««««««««««««««««««««««««««««««««««»

//--- WM_MOUSEWHEEL ---

void Game_ContainerMouseWheel(HWND hWnd, int iDelta)
{
	RECT	rcWindow;
	LONG	lTop;

	GetWindowRect(App.Game.Layout.hwndAttributes,&rcWindow);
	MapWindowPoints(NULL,hWnd,(POINT *)&rcWindow,2);
	lTop = rcWindow.top;

	if (iDelta < 0) lTop -= App.Font.uFontHeight;
	else lTop += App.Font.uFontHeight;

	rcWindow.bottom = rcWindow.bottom-rcWindow.top;
	rcWindow.top = lTop;
	rcWindow.bottom += rcWindow.top;
	lTop = Game_ContainerTopOffset(lTop,&rcWindow);

	SetWindowPos(App.Game.Layout.hwndAttributes,NULL,0,lTop,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	return;
}

//--- WM_VSCROLL ---

void Game_ContainerScroll(UINT uRequest)
{
	SCROLLINFO	scrollInfo;
	RECT		rcAttributes;

	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(App.Game.Layout.hwndScrollbar,SB_CTL,&scrollInfo);

	switch(uRequest)
		{
		case SB_TOP:
			scrollInfo.nPos = scrollInfo.nMin;
			break;
		case SB_BOTTOM:
			scrollInfo.nPos = scrollInfo.nMax;
			break;
		case SB_LINEUP:
			scrollInfo.nPos -= 1;
			break;
		case SB_LINEDOWN:
			scrollInfo.nPos += 1;
			break;
		case SB_PAGEUP:
			scrollInfo.nPos -= scrollInfo.nPage;
			break;
		case SB_PAGEDOWN:
			scrollInfo.nPos += scrollInfo.nPage;
			break;
		case SB_THUMBTRACK:
			scrollInfo.nPos = scrollInfo.nTrackPos;
			break;
		}

	scrollInfo.fMask = SIF_POS;
	SetScrollInfo(App.Game.Layout.hwndScrollbar,SB_CTL,&scrollInfo,TRUE);
	GetScrollInfo(App.Game.Layout.hwndScrollbar,SB_CTL,&scrollInfo);

	GetWindowRect(App.Game.Layout.hwndAttributes,&rcAttributes);
	MapWindowPoints(NULL,App.Game.Layout.hwndContainer,(POINT *)&rcAttributes,2);

	if (rcAttributes.top != -scrollInfo.nPos)
		SetWindowPos(App.Game.Layout.hwndAttributes,NULL,0,-scrollInfo.nPos,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);

	return;
}


// «»»» Corrige la position des attributs si nécessaire «««««««««««««««««»

// Parameters:
// 	lTop - Top offset of hwndAttributes
// 	rcChild - RECT of hwndAttributes relative to hwndContainer
//
// Return value:
// 	Corrected top offset of hwndAttributes

long Game_ContainerTopOffset(LONG lTop, RECT *rcChild)
{
	RECT	rcWindow;
	RECT	rcContainer;

	if (!rcChild)
		{
		GetWindowRect(App.Game.Layout.hwndAttributes,&rcWindow);
		MapWindowPoints(NULL,App.Game.Layout.hwndContainer,(POINT *)&rcWindow,2);
		}

	GetClientRect(App.Game.Layout.hwndContainer,&rcContainer);
	if (rcContainer.bottom > rcChild->bottom-rcChild->top) return(0); // Fits entirely

	CopyRect(&rcWindow,rcChild);
	if (rcWindow.top > 0) return(0);
	if (rcWindow.bottom < rcContainer.bottom) return(-(rcChild->bottom-rcChild->top-rcContainer.bottom));
	return(lTop);
}


// «»»» Taille et position de la scrollbar ««««««««««««««««««««««««««««««»

void Game_ContainerScrollInfo(UINT fMask)
{
	SCROLLINFO	scrollInfo;
	RECT		rcContainer;
	RECT		rcAttributes;

	GetClientRect(App.Game.Layout.hwndContainer,&rcContainer);
	GetWindowRect(App.Game.Layout.hwndAttributes,&rcAttributes);
	MapWindowPoints(NULL,App.Game.Layout.hwndContainer,(POINT *)&rcAttributes,2);

	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = fMask;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = (rcAttributes.bottom-rcAttributes.top)-1;
	scrollInfo.nPage = rcContainer.bottom;
	scrollInfo.nPos = -rcAttributes.top;
	SetScrollInfo(App.Game.Layout.hwndScrollbar,SB_CTL,&scrollInfo,TRUE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Attributs							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation de la fenêtre ««««««««««««««««««««««««««««««««««««»

LRESULT Game_AttributesCreate(HWND hWnd)
{
	int		W,H;
	int		i;

	// NOTE: Positions are set in Game_PaintAttributes()

	W = MAIN_ATTR_BTN_WIDTH;
	H = App.Font.uFontHeight+12;

	// Attributs & points
	for (i = 0; i != 6; i++) if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_ATTRIBUTES+i,&App.Game.Layout.hwndAttrBtn[i],BS_OWNERDRAW)) return(-1);
	for (i = 0; i != 4; i++) if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_POINTS+i,&App.Game.Layout.hwndPointsBtn[i],BS_OWNERDRAW)) return(-1);

	// Vie & Armures
	if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_LIFE,&App.Game.Layout.hwndLifeBtn,BS_OWNERDRAW)) return(-1);
	if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_PHYSICAL,&App.Game.Layout.hwndPhysicalBtn,BS_OWNERDRAW)) return(-1);
	if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_MAGICAL,&App.Game.Layout.hwndMagicalBtn,BS_OWNERDRAW)) return(-1);

	// Expérience
	if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_EXPERIENCE,&App.Game.Layout.hwndExperienceBtn,BS_OWNERDRAW)) return(-1);
	if (!Game_CreateButton(hWnd,0,0,W,H,NULL,CTLID_NEXTLEVEL,&App.Game.Layout.hwndNextLevelBtn,BS_OWNERDRAW)) return(-1);

	return(0);
}


// «»»» Messages provenant de la fenêtre ««««««««««««««««««««««««««««««««»

LRESULT Game_ProcessAttributesMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		case WM_DRAWITEM:
			return(SendMessage(App.hWnd,WM_DRAWITEM,wParam,lParam));

		case WM_PAINT:
			if (GetUpdateRect(hWnd,NULL,FALSE))
				{
				HDC		hDC;
				PAINTSTRUCT	paintStruct;
				RECT		rcWindow;

				hDC = BeginPaint(hWnd,&paintStruct);
				GetClientRect(hWnd,&rcWindow);
				FillRect(hDC,&rcWindow,GetSysColorBrush(COLOR_WINDOW));
				Game_PaintAttributes(hWnd,hDC,&rcWindow);
				EndPaint(hWnd,&paintStruct);
				}
			break;

 		case WM_MOVE:
			Game_ContainerScrollInfo(SIF_POS);
			break;

		case WM_SIZE:
			Game_AttributesResize(hWnd,LOWORD(lParam),HIWORD(lParam));
			break;

		case WM_COMMAND:
			return(SendMessage(App.hWnd,WM_COMMAND,wParam,lParam));

		default:
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}


// «»»» Redimension des attributs «««««««««««««««««««««««««««««««««««««««»

void Game_AttributesResize(HWND hWnd, int iWidth, int iHeight)
{
	RECT	rcContainer;

	GetClientRect(App.Game.Layout.hwndContainer,&rcContainer);
	if (rcContainer.bottom < iHeight)
		{
		ShowWindow(App.Game.Layout.hwndScrollbar,SW_SHOW);
		Game_ContainerScrollInfo(SIF_PAGE|SIF_RANGE);
		}
	else ShowWindow(App.Game.Layout.hwndScrollbar,SW_HIDE);
	return;
}


// «»»» Fait en sorte que le bouton en focus soit visible «««««««««««««««»

void Game_AttributesMakeFocusVisible()
{
	if (IsWindow(App.Game.Layout.hwndAttributes))
		{
		HWND	hwndFocus;
		RECT	rcControl;
		RECT	rcContainer;
		RECT	rcAttributes;
		LONG	lTop;

		hwndFocus = GetFocus();
		if (!hwndFocus) return;

		if (GetParent(hwndFocus) != App.Game.Layout.hwndAttributes) return;

		GetWindowRect(hwndFocus,&rcControl);
		GetClientRect(App.Game.Layout.hwndContainer,&rcContainer);
		GetWindowRect(App.Game.Layout.hwndAttributes,&rcAttributes);
		MapWindowPoints(NULL,App.Game.Layout.hwndContainer,(POINT *)&rcControl,2);
		MapWindowPoints(NULL,App.Game.Layout.hwndContainer,(POINT *)&rcAttributes,2);

		rcContainer.top += rcControl.bottom-rcControl.top;
		rcContainer.bottom -= rcControl.bottom-rcControl.top;
		lTop = rcAttributes.top;

		if (rcControl.top < rcContainer.top) // Above the visible area
			{
			lTop += rcContainer.top-rcControl.top;
			}
		else if (rcControl.bottom > rcContainer.bottom) // Below the visible area
			{
			lTop -= rcControl.bottom-rcContainer.bottom;
			}
		else return;

		rcAttributes.bottom = rcAttributes.bottom-rcAttributes.top;
		rcAttributes.top = lTop;
		rcAttributes.bottom += rcAttributes.top;

		lTop = Game_ContainerTopOffset(lTop,&rcAttributes);
		SetWindowPos(App.Game.Layout.hwndAttributes,NULL,0,lTop,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
		}

	return;
}

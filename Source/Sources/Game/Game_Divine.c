
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des archives du jeu
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Locale.h"
#include "Texts.h"
#include "Divine.h"
#include "Game.h"
#include "Files.h"
#include "Infos.h"
#include "LastFiles.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Requests.h"
#include "Taskbar.h"

extern APPLICATION	App;

static DIVINELOGHEADER	LogHeaders[] = {
					{"[DEBUG]", FALSE, IDI_INFORMATION},
					{"[TRACE]", FALSE, IDI_INFORMATION},
					{"[INFO]",  FALSE, IDI_INFORMATION},
					{"[FATAL]",  TRUE, IDI_ERROR},
					{"[ERROR]",  TRUE, IDI_ERROR},
					{NULL,0}
				};

static GAMEEDITPAGE	SaveGamePages[] = {
					{ GAME_PAGE_SAVEGAME_PROFILE, TEXT_SAVEGAME_TITLE_PROFILE, TEXT_SAVEGAME_INFO_PROFILE, 8000 },
					{ GAME_PAGE_SAVEGAME_LIST, TEXT_SAVEGAME_TITLE_LIST, TEXT_SAVEGAME_INFO_LIST, 8001 },
					{ 0 }
				};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection de la sauvegarde					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Divine_Select()
{
	DIVINESGCONTEXT*	pContext;
	PROPSHEETHEADER*	psh;
	PROPSHEETPAGE*		psp;
	int			iNumPages;
	int			iResult;
	int			i;

	psh = NULL;
	psp = NULL;
	iResult = -1;

	//--- Alloue les structures ---

	pContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DIVINESGCONTEXT));
	if (!pContext)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	psh = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETHEADER));
	if (!psh)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	for (i = 0, iNumPages = 0; SaveGamePages[i].uPageID != 0; i++, iNumPages++);

	psp = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETPAGE)*iNumPages);
	if (!psp)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	//--- Valeurs par défaut ---

	if (App.Config.uGame) pContext->uGame = App.Config.uGame;
	else pContext->uGame = DIVINE_DOS_2EE;

	pContext->pszProfile = Misc_StrCpyAlloc(App.Config.pszProfile);
	pContext->pszSaveName = Misc_StrCpyAlloc(App.Game.Save.pszSaveName);

	//--- Alloue les pages ---

	for (i = 0; SaveGamePages[i].uPageID != 0; i++)
		{
		GAMEEDITPAGECONTEXT*	ctx;

		ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITPAGECONTEXT));
		if (!ctx)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}

		ctx->savegame.pContext = pContext;
		ctx->uPageID = SaveGamePages[i].uPageID;
		ctx->pszInfo = Locale_GetText(SaveGamePages[i].uInfoID);

		psp[i].dwSize = sizeof(PROPSHEETPAGE);
		psp[i].dwFlags = PSP_USETITLE;
		psp[i].hInstance = App.hInstance;
		psp[i].pszTemplate = MAKEINTRESOURCE(SaveGamePages[i].uResID);
		psp[i].pszTitle = Locale_GetText(SaveGamePages[i].uTitleID);
		psp[i].pfnDlgProc = (DLGPROC)Divine_SelectProc;
		psp[i].lParam = (LPARAM)ctx;
		}

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_NOAPPLYNOW|PSH_WIZARD;
	psh->hwndParent = App.hWnd;
	psh->hInstance = App.hInstance;
	psh->pszIcon = MAKEINTRESOURCE(1);
	psh->nPages = iNumPages;
	psh->ppsp = psp;

	iResult = PropertySheet(psh);
	if (iResult) Divine_Open(pContext->uGame,pContext->pszProfile,pContext->pszSaveName,NULL);

	//--- Terminé ---

Done:	if (psh) HeapFree(App.hHeap,0,psh);
	if (psp)
		{
		for (i = 0; SaveGamePages[i].uPageID != 0; i++) if (psp[i].lParam) HeapFree(App.hHeap,0,(void *)psp[i].lParam);
		HeapFree(App.hHeap,0,psp);
		}
	if (pContext)
		{
		if (pContext->pszProfile) HeapFree(App.hHeap,0,pContext->pszProfile);
		if (pContext->pszSaveName) HeapFree(App.hHeap,0,pContext->pszSaveName);
		if (pContext->hBitmap) DeleteObject(pContext->hBitmap);
		lsv_Release(&pContext->GameFiles);
		Divine_SelectReleaseList(&pContext->Profiles);
		Divine_SelectReleaseList(&pContext->Savegames);
		HeapFree(App.hHeap,0,pContext);
		}

	if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	return;
}


// «»»» Boîte de dialogue «««««««««««««««««««««««««««««««««««««««««««««««»

BOOL CALLBACK Divine_SelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	PROPSHEETPAGE*	psp;

	if (uMsgId == WM_MEASUREITEM)
		{
		if (Dialog_ViewComboMeasureItem(190,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
		((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
		if (((MEASUREITEMSTRUCT *)lParam)->itemHeight < 24+4) ((MEASUREITEMSTRUCT *)lParam)->itemHeight = 24+4;
		return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		GAMEEDITPAGECONTEXT*	ctx;
		RECT			rcDialog;
		int			Height;

		ctx = (GAMEEDITPAGECONTEXT *)((PROPSHEETPAGE *)lParam)->lParam;
		Height = Dialog_GetInfoHeight(hDlg,100,ctx->pszInfo);
		GetWindowRect(hDlg,&rcDialog);
		// PropSheet dialogs need more work to be resized...
		//SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);
		SetWindowLongPtr(hDlg,DWLP_USER,(LONG_PTR)lParam);
		SendMessage(hDlg,PSM_SETWIZBUTTONS,(WPARAM)PSWIZB_NEXT,0);

		switch(ctx->uPageID)
			{
			case GAME_PAGE_SAVEGAME_PROFILE:
				Dialog_OffsetY(hDlg,190,Height);
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;

			case GAME_PAGE_SAVEGAME_LIST:
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;
			}

		return(TRUE);
		}

	psp = (PROPSHEETPAGE *)GetWindowLongPtr(hDlg,DWLP_USER);
	if (!psp) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(((GAMEEDITPAGECONTEXT *)psp->lParam)->pszInfo,(DRAWITEMSTRUCT *)lParam,BF_RECT);
					return(TRUE);
				case 190:
					Divine_SelectDrawGame((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 200:
					Divine_SelectDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 300:
					Divine_SelectDrawSeparator((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 301:
					Divine_SelectDrawGameImage((DRAWITEMSTRUCT *)lParam,((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext);
					return(TRUE);
				}
			return(Infos_Draw(hDlg,wParam,(DRAWITEMSTRUCT *)lParam,&((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext->GameFiles));

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_DBLCLK:
					switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
						{
						case GAME_PAGE_SAVEGAME_PROFILE:
							SendMessage(GetParent(hDlg),PSM_PRESSBUTTON,(WPARAM)PSBTN_NEXT,0);
							return(TRUE);
						case GAME_PAGE_SAVEGAME_LIST:
							SendMessage(GetParent(hDlg),PSM_PRESSBUTTON,(WPARAM)PSBTN_FINISH,0);
							return(TRUE);
						}
					break;

				case LBN_SELCHANGE:
					switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
						{
						case GAME_PAGE_SAVEGAME_PROFILE:
							switch(LOWORD(wParam))
								{
								case 190:
									Divine_SelectReleaseList(&((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext->Profiles);
									Divine_SelectReleaseList(&((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext->Savegames);
									Divine_SelectCreateList(hDlg,DIVINE_PROFILE_LIST,((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext);
									Divine_SelectLoadImage(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext);
									return(TRUE);
								case 200:
									PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)Divine_SelectSetNextPage(hDlg,FALSE,(GAMEEDITPAGECONTEXT *)psp->lParam) != -1?PSWIZB_NEXT:0);
									return(TRUE);
								}
							break;
						case GAME_PAGE_SAVEGAME_LIST:
							switch(LOWORD(wParam))
								{
								case 200:
									Divine_SelectLoadGameInfos(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->savegame.pContext);
									PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Divine_SelectSetNextPage(hDlg,FALSE,(GAMEEDITPAGECONTEXT *)psp->lParam) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
									return(TRUE);
								}
							break;
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case PSN_SETACTIVE:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Divine_SelectActivate(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam)?0:-1);
					return(TRUE);
				case PSN_WIZBACK:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Divine_SelectSetPrevPage(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam));
					return(TRUE);
				case PSN_WIZNEXT:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Divine_SelectSetNextPage(hDlg,TRUE,(GAMEEDITPAGECONTEXT *)psp->lParam));
					return(TRUE);
				case PSN_WIZFINISH:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,(Divine_SelectSetNextPage(hDlg,TRUE,(GAMEEDITPAGECONTEXT *)psp->lParam) == -1)?TRUE:FALSE);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Navigation ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

//--- Activation d'une page ---

int Divine_SelectActivate(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_SAVEGAME_PROFILE:
			if (!ctx->bPageSet)
				{
				LRESULT	lResult;

				lResult = SendDlgItemMessage(hDlg,190,CB_ADDSTRING,0,(LPARAM)DIVINE_DOS_2);
				if (lResult == CB_ERR || lResult == CB_ERRSPACE) return(0);
				lResult = SendDlgItemMessage(hDlg,190,CB_ADDSTRING,0,(LPARAM)DIVINE_DOS_2EE);
				if (lResult == CB_ERR || lResult == CB_ERRSPACE) return(0);
				SendDlgItemMessage(hDlg,190,CB_SETCURSEL,(WPARAM)ctx->savegame.pContext->uGame == DIVINE_DOS_2?0:1,0);
				ctx->bPageSet = TRUE;
				}
			if (!Divine_SelectCreateList(hDlg,DIVINE_PROFILE_LIST,ctx->savegame.pContext)) return(0);
			Divine_SelectLoadImage(hDlg,ctx->savegame.pContext);
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)Divine_SelectSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_NEXT:0);
			break;

		case GAME_PAGE_SAVEGAME_LIST:
			if (!ctx->bPageSet)
				{
				if (!Infos_Initialise(hDlg)) return(0);
				ctx->bPageSet = TRUE;
				}
			if (!Divine_SelectCreateList(hDlg,DIVINE_SAVEGAMES_LIST,ctx->savegame.pContext)) return(0);
			PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)PSWIZB_BACK|(Divine_SelectSetNextPage(hDlg,FALSE,ctx) != -1?PSWIZB_FINISH:PSWIZB_DISABLEDFINISH));
			break;
		}

	return(1);
}

//--- Activation de la page précédente ---

LONG_PTR Divine_SelectSetPrevPage(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_SAVEGAME_PROFILE:
			break;

		case GAME_PAGE_SAVEGAME_LIST:
			return((LONG_PTR)SaveGamePages[0].uResID);
		}

	return(-1);
}

//--- Activation de la page suivante ---

LONG_PTR Divine_SelectSetNextPage(HWND hDlg, BOOL bStore, GAMEEDITPAGECONTEXT *ctx)
{
	DIVINEITEM*	pItem;
	UINT		uSelected;
	UINT		uGame;

	switch(ctx->uPageID)
		{
		case GAME_PAGE_SAVEGAME_PROFILE:
			uGame = SendDlgItemMessage(hDlg,190,CB_GETCURSEL,0,0);
			if (uGame == LB_ERR) break;
			uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
			if (uSelected == LB_ERR) break;
			pItem = (DIVINEITEM *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
			if (pItem == (DIVINEITEM *)LB_ERR) break;
			if (!pItem) break;
			if (bStore)
				{
				if (ctx->savegame.pContext->pszProfile) HeapFree(App.hHeap,0,ctx->savegame.pContext->pszProfile);
				ctx->savegame.pContext->pszProfile = HeapAlloc(App.hHeap,0,wcslen(pItem->name)*sizeof(WCHAR)+sizeof(WCHAR));
				if (!ctx->savegame.pContext->pszProfile)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					break;
					}
				wcscpy(ctx->savegame.pContext->pszProfile,pItem->name);
				ctx->savegame.pContext->uGame = ++uGame;
				}
			return(0);

		case GAME_PAGE_SAVEGAME_LIST:
			uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
			if (uSelected == LB_ERR) break;
			pItem = (DIVINEITEM *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
			if (pItem == (DIVINEITEM *)LB_ERR) break;
			if (!pItem) break;
			if (bStore)
				{
				if (ctx->savegame.pContext->pszSaveName) HeapFree(App.hHeap,0,ctx->savegame.pContext->pszSaveName);
				ctx->savegame.pContext->pszSaveName = HeapAlloc(App.hHeap,0,wcslen(pItem->name)*sizeof(WCHAR)+sizeof(WCHAR));
				if (!ctx->savegame.pContext->pszSaveName)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					break;
					}
				wcscpy(ctx->savegame.pContext->pszSaveName,pItem->name);
				}
			return(0);
		}

	return(-1);
}


// «»»» Affichage du jeu ««««««««««««««««««««««««««««««««««««««««««««««««»

void Divine_SelectDrawGame(DRAWITEMSTRUCT *pDraw)
{
	WCHAR*	pszText;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	pszText = NULL;
	switch(pDraw->itemData)
		{
		case DIVINE_DOS_2:
			pszText = szGameName;
			break;
		case DIVINE_DOS_2EE:
			pszText = szGameNameEE;
			break;
		}

	if (pszText)
		{
		RECT		rcText;
		HFONT		hFont;
		COLORREF	crText;
		int		iBkMode;

		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 2;
		rcText.right -= 2;
		DrawText(pDraw->hDC,pszText,-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crText);
		SelectObject(pDraw->hDC,hFont);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return;
}


// «»»» Affichage d'un répertoire «««««««««««««««««««««««««««««««««««««««»

void Divine_SelectDrawItem(DRAWITEMSTRUCT *pDraw)
{
	DIVINEITEM*	pItem;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	pItem = (DIVINEITEM *)pDraw->itemData;
	if (pItem != 0 && pItem != (DIVINEITEM *)-1)
		{
		RECT		rcText;
		HFONT		hFont;
		COLORREF	crText;
		int		iBkMode;

		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 2;
		rcText.right -= 2;
		DrawIconEx(pDraw->hDC,rcText.left,rcText.top+(rcText.bottom-rcText.top-16)/2,App.hIcons[pItem->node.type == DIVINE_PROFILE_LIST?APP_ICON_FOLDER:APP_ICON_SAVEGAME],16,16,0,NULL,DI_NORMAL);
		rcText.left += 20;
		DrawText(pDraw->hDC,pItem->name,-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crText);
		SelectObject(pDraw->hDC,hFont);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return;
}


// «»»» Affichage de la ligne de séparation «««««««««««««««««««««««««««««»

void Divine_SelectDrawSeparator(DRAWITEMSTRUCT *pDraw)
{
	RECT	rcItem;

	CopyRect(&rcItem,&pDraw->rcItem);
	rcItem.left = (rcItem.right-rcItem.left-2)/2;
	rcItem.right = rcItem.left+2;
	DrawEdge(pDraw->hDC,&rcItem,BDR_SUNKENOUTER,BF_LEFT|BF_RIGHT);
	return;
}


// «»»» Affichage de l'image du jeu «««««««««««««««««««««««««««««««««««««»

void Divine_SelectDrawGameImage(DRAWITEMSTRUCT *pDraw, DIVINESGCONTEXT *pContext)
{
	HDC	hDC;
	BITMAP	bm;

	if (pContext->hBitmap)
		{
		GetObject(pContext->hBitmap,sizeof(BITMAP),&bm);
		hDC = CreateCompatibleDC(pDraw->hDC);
		if (hDC)
			{
			HBITMAP hDefBitmap = SelectObject(hDC,pContext->hBitmap);
			BitBlt(pDraw->hDC,pDraw->rcItem.left,pDraw->rcItem.top,pDraw->rcItem.right-pDraw->rcItem.left,pDraw->rcItem.bottom-pDraw->rcItem.top,hDC,bm.bmWidth/2-(pDraw->rcItem.right-pDraw->rcItem.left)/2,bm.bmHeight-(pDraw->rcItem.bottom-pDraw->rcItem.top),SRCCOPY);
			SelectObject(hDC,hDefBitmap);
			DeleteDC(hDC);
			}
		}
	else FillRect(pDraw->hDC,&pDraw->rcItem,GetStockObject(BLACK_BRUSH));

	return;
}


// «»»» Chargement de l'image du jeu ««««««««««««««««««««««««««««««««««««»

void Divine_SelectLoadImage(HWND hDlg, DIVINESGCONTEXT *pContext)
{
	UINT	uGame;

	uGame = SendDlgItemMessage(hDlg,190,CB_GETCURSEL,0,0);
	if (uGame == CB_ERR) return;

	if (pContext->uGameImg != ++uGame)
		{
		WCHAR*	pszImage;

		switch(uGame)
			{
			case DIVINE_DOS_2:
				pszImage = szDOS2PNGPath;
				break;
			case DIVINE_DOS_2EE:
				pszImage = szDOS2EEPNGPath;
				break;
			default:return;
			}

		if (pContext->hBitmap) DeleteObject(pContext->hBitmap);
		pContext->uGameImg = uGame;
		pContext->hBitmap = png_LoadImage(pszImage);
		InvalidateRect(GetDlgItem(hDlg,301),NULL,FALSE);
		}

	return;
}


// «»»» Chargement des informations sur la sauvegarde sélectionnée ««««««»

void Divine_SelectLoadGameInfos(HWND hDlg, DIVINESGCONTEXT *pContext)
{
	DIVINEITEM*	pItem;
	WCHAR*		pszPath;
	LRESULT		lResult;

	pszPath = NULL;
	lResult = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (lResult == LB_ERR) goto Error;

	pItem = (DIVINEITEM *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)lResult,0);
	if (pItem == (DIVINEITEM *)LB_ERR) goto Error;
	pszPath = Divine_GetSaveGamePath(pContext->uGame,pContext->pszProfile,pItem->name,pContext->pszCustomSavePath);
	if (!pszPath) goto Error;

	lsv_Release(&pContext->GameFiles);
	lResult = lsv_Load(hDlg,pszPath,&pContext->GameFiles,LS_LOAD_QUIET|LS_LOAD_META|LS_LOAD_PNG);
	if (!lResult) goto Error;

	Infos_PrepareAndUpdate(hDlg,pItem->name,&pContext->GameFiles);
	HeapFree(App.hHeap,0,pszPath);
	return;

Error:	if (pszPath) HeapFree(App.hHeap,0,pszPath);
	lsv_Release(&pContext->GameFiles);
	SendDlgItemMessage(hDlg,801,LVM_DELETEALLITEMS,0,0);
	InvalidateRect(GetDlgItem(hDlg,800),NULL,FALSE);
	return;
}


// «»»» Création d'une liste ««««««««««««««««««««««««««««««««««««««««««««»

int Divine_SelectCreateList(HWND hDlg, UINT uType, DIVINESGCONTEXT *pContext)
{
	WIN32_FIND_DATA		Find;
	HANDLE			hFile;
	DIVINEITEM*		pItem;
	DIVINEITEM*		pSelected;
	WCHAR*			pszPath;
	WCHAR*			pszSearch;
	NODE*			pRoot;
	UINT			uGame;
	UINT			uLen;
	LRESULT			lResult;
	int			iResult;
	int			iIndex;

	//--- Sélection du répertoire ---

	switch(uType)
		{
		case DIVINE_PROFILE_LIST:
			uGame = SendDlgItemMessage(hDlg,190,CB_GETCURSEL,0,0);
			if (uGame == CB_ERR) uGame = 0;
			if (pContext->Profiles.type == ++uGame) return(1);
			SendDlgItemMessage(hDlg,200,LB_RESETCONTENT,0,0);
			uLen  = App.Config.pszLarianPath?(wcslen(App.Config.pszLarianPath)*sizeof(WCHAR)):0;
			uLen += sizeof(WCHAR)+wcslen(Divine_GetGameName(uGame))*sizeof(WCHAR);
			uLen += sizeof(WCHAR)+wcslen(szPlayerProfiles)*sizeof(WCHAR);
			pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen+sizeof(WCHAR));
			if (!pszPath)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				return(0);
				}
			if (App.Config.pszLarianPath) wcscpy(pszPath,App.Config.pszLarianPath);
			PathAppend(pszPath,Divine_GetGameName(uGame));
			PathAppend(pszPath,szPlayerProfiles);
			pRoot = &pContext->Profiles;
			break;

		case DIVINE_SAVEGAMES_LIST:
			if (pContext->Savegames.type == pContext->uGame) return(1);
			SendDlgItemMessage(hDlg,200,LB_RESETCONTENT,0,0);
			uGame = pContext->uGame;
			uLen  = App.Config.pszLarianPath?(wcslen(App.Config.pszLarianPath)*sizeof(WCHAR)):0;
			uLen += sizeof(WCHAR)+wcslen(Divine_GetGameName(uGame))*sizeof(WCHAR);
			uLen += sizeof(WCHAR)+wcslen(szPlayerProfiles)*sizeof(WCHAR);
			uLen += sizeof(WCHAR)+wcslen(szSavegames)*sizeof(WCHAR);
			uLen += sizeof(WCHAR)+wcslen(szStory)*sizeof(WCHAR);
			uLen += sizeof(WCHAR)+wcslen(pContext->pszProfile)*sizeof(WCHAR);
			pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen+sizeof(WCHAR));
			if (!pszPath)
				{
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				return(0);
				}
			if (App.Config.pszLarianPath) wcscpy(pszPath,App.Config.pszLarianPath);
			PathAppend(pszPath,Divine_GetGameName(uGame));
			PathAppend(pszPath,szPlayerProfiles);
			PathAppend(pszPath,pContext->pszProfile);
			PathAppend(pszPath,szSavegames);
			PathAppend(pszPath,szStory);
			pRoot = &pContext->Savegames;
			break;

		default:return(0);
		}

	Divine_SelectReleaseList(pRoot);

	//--- Vérifie que le répertoire existe ---

	if (!PathFileExists(pszPath))
		{
		HeapFree(App.hHeap,0,pszPath);
		return(1);
		}

	//--- Récupère la liste des dossiers dans le répertoire ---

	pszSearch = HeapAlloc(App.hHeap,0,wcslen(pszPath)*sizeof(WCHAR)+sizeof(WCHAR)+wcslen(szWild)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszSearch)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		HeapFree(App.hHeap,0,pszPath);
		return(0);
		}
	wcscpy(pszSearch,pszPath);
	PathAppend(pszSearch,szWild);
	hFile = FindFirstFile(pszSearch,&Find);
	HeapFree(App.hHeap,0,pszSearch);
	if (hFile == INVALID_HANDLE_VALUE)
		{
		Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		HeapFree(App.hHeap,0,pszPath);
		return(0);
		}

	iResult = 0;
	pSelected = NULL;

	do {

		if (!wcscmp(Find.cFileName,szRootPath)) continue;
		if (!wcscmp(Find.cFileName,szParentPath)) continue;

		if (Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			//--- Vérifie que le profil contient bien le répertoire des sauvegardes
			if (uType == DIVINE_PROFILE_LIST)
				{
				WCHAR*	pszTemp;
				UINT	uLen;

				uLen  = wcslen(pszPath);
				uLen += 1+wcslen(Find.cFileName);
				uLen += 1+wcslen(szSavegames);
				uLen += 1+wcslen(szStory);
				pszTemp = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
				if (!pszTemp)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					goto Done;
					}
				wcscpy(pszTemp,pszPath);
				PathAppend(pszTemp,Find.cFileName);
				PathAppend(pszTemp,szSavegames);
				PathAppend(pszTemp,szStory);
				if (!PathFileExists(pszTemp))
					{
					HeapFree(App.hHeap,0,pszTemp);
					continue;
					}
				HeapFree(App.hHeap,0,pszTemp);
				}

			//--- Ajoute le dossier à la liste interne
			pItem = HeapAlloc(App.hHeap,0,sizeof(DIVINEITEM));
			if (!pItem)
				{
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				goto Done;
				}
			List_AddEntry((NODE *)pItem,pRoot);
			pItem->node.type = uType;
			pItem->time.dwLowDateTime = Find.ftCreationTime.dwLowDateTime;
			pItem->time.dwHighDateTime = Find.ftCreationTime.dwHighDateTime;
			pItem->name = Misc_StrCpyAlloc(Find.cFileName);
			if (!pItem->name)
				{
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				goto Done;
				}

			//--- Tente d'insérer le dossier en fonction de la date de création
			if (SendDlgItemMessage(hDlg,200,LB_GETCOUNT,0,0) > 1)
				{
				DIVINEITEM*	pListItem;
				long		lResult;

				iIndex = 0;
				while (1)
					{
					pListItem = (DIVINEITEM *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)iIndex,0);
					if (pListItem == (DIVINEITEM *)LB_ERR) { iIndex = -1; break; }
					lResult = CompareFileTime(&pListItem->time,&pItem->time);
					if (lResult == -1) break;
					iIndex++;
					}
				}
			else iIndex = -1;

			//--- Ajoute le dossier à la liste visuelle
			lResult = SendDlgItemMessage(hDlg,200,LB_INSERTSTRING,(WPARAM)iIndex,(LPARAM)pItem);
			if (lResult == LB_ERR || lResult == LB_ERRSPACE)
				{
				Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				goto Done;
				}

			switch(uType)
				{
				case DIVINE_PROFILE_LIST:
					if (!pContext->pszProfile) break;
					if (wcscmp(pItem->name,pContext->pszProfile)) break;
					pSelected = pItem;
					break;
				case DIVINE_SAVEGAMES_LIST:
					if (!pContext->pszSaveName) break;
					if (wcscmp(pItem->name,pContext->pszSaveName)) break;
					pSelected = pItem;
					break;
				}
			}

	} while (FindNextFile(hFile,&Find));

	if (pSelected)
		{
		SendDlgItemMessage(hDlg,200,LB_SELECTSTRING,(WPARAM)-1,(LPARAM)pSelected);
		PostMessage(GetParent(hDlg),PSM_SETWIZBUTTONS,0,(LPARAM)uType == DIVINE_PROFILE_LIST?PSWIZB_NEXT:PSWIZB_FINISH);
		if (uType == DIVINE_SAVEGAMES_LIST) Divine_SelectLoadGameInfos(hDlg,pContext);
		}

	iResult = 1;

Done:	FindClose(hFile);
	HeapFree(App.hHeap,0,pszPath);

	if (iResult) pRoot->type = uGame;
	else Divine_SelectReleaseList(pRoot);

	return(iResult);
}


// «»»» Libération de la mémoire utilisée par une liste «««««««««««««««««»

void Divine_SelectReleaseList(NODE *pRoot)
{
	DIVINEITEM*	pItem;

	for (pItem = (DIVINEITEM *)pRoot->next; pItem != NULL; pItem = (DIVINEITEM *)pItem->node.next)
		{
		if (pItem->name) HeapFree(App.hHeap,0,pItem->name);
		}

	pRoot->type = 0;
	List_ReleaseMemory(pRoot);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Chargement							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Ouverture d'une sauvegarde particulière «««««««««««««««««««««««««»

void Divine_OpenAs()
{
	OPENFILENAME	ofn;
	DIVINECONTEXT*	ctx;
	WCHAR*		lpstrFilter;
	WCHAR*		lpstrFile;
	WCHAR*		pszFilter;
	UINT		uLen,uStrLen;
	BOOL		bResult;

	//--- Initialisations ---

	ctx = Divine_PrepareContext(TEXT_ERR_LOADING);
	if (!ctx) return;

	uLen  = wcslen(Locale_GetText(TEXT_DIALOG_SAVE_DOS))+1;
	uLen += wcslen(szLSVwext)+1;
	uLen += wcslen(Locale_GetText(TEXT_DIALOG_SAVE_DOSDE))+1;
	uLen += wcslen(szLSVwext)+1;
	uLen += 1;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = App.hWnd;
	ofn.hInstance = App.hInstance;
	ofn.lpstrFilter = lpstrFilter = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR));
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 2;
	ofn.lpstrFile = lpstrFile = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,2048*sizeof(WCHAR));
	ofn.nMaxFile = 2048;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = App.Config.pszLarianPath;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR|OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	ofn.pvReserved = NULL;
	ofn.dwReserved = 0;
	ofn.FlagsEx = 0;

	// Abort if buffer allocation failed
	if (!lpstrFile || !lpstrFilter)
		{
		if (lpstrFile) HeapFree(App.hHeap,0,lpstrFile);
		if (lpstrFilter) HeapFree(App.hHeap,0,lpstrFilter);
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	// Set filters and extensions
	wcscpy(lpstrFilter,Locale_GetText(TEXT_DIALOG_SAVE_DOS));
	pszFilter = wcschr(lpstrFilter,L'\0');
	wcscpy(++pszFilter,szLSVwext);
	pszFilter = wcschr(pszFilter,L'\0');
	wcscpy(++pszFilter,Locale_GetText(TEXT_DIALOG_SAVE_DOSDE));
	pszFilter = wcschr(pszFilter,L'\0');
	wcscpy(++pszFilter,szLSVwext);

	//--- Sélection du fichier ---

	bResult = GetOpenFileName(&ofn);
	if (!bResult)
		{
		HeapFree(App.hHeap,0,lpstrFile);
		HeapFree(App.hHeap,0,lpstrFilter);
		if (GetLastError() == ERROR_SUCCESS) return;
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	HeapFree(App.hHeap,0,lpstrFilter);

	//--- Copie des informations ---

	uLen  = uStrLen = wcslen(lpstrFile);
	uLen -= ofn.nFileOffset;
	uLen -= uStrLen-ofn.nFileExtension+1;
	ctx->pszSaveName = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!ctx->pszSaveName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}
	CopyMemory(ctx->pszSaveName,lpstrFile+ofn.nFileOffset,uLen*sizeof(WCHAR));

	ctx->uGame = ofn.nFilterIndex;
	ctx->pszCustomSavePath = lpstrFile;

	//--- Chargement ---

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Divine_LoadThread,ctx,0,NULL);
	if (!App.hThread)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	return;
}


// «»»» Préparation du chargement «««««««««««««««««««««««««««««««««««««««»

void Divine_Open(UINT uGame, WCHAR *pszProfile, WCHAR *pszSaveName, WCHAR *pszCustomSavePath)
{
	DIVINECONTEXT*	ctx;

	ctx = Divine_PrepareContext(TEXT_ERR_LOADING);
	if (!ctx) return;

	ctx->uGame = uGame;

	if (pszProfile)
		{
		ctx->pszProfile = Misc_StrCpyAlloc(pszProfile);
		if (!ctx->pszProfile)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
			Divine_ReleaseContext(ctx);
			return;
			}
		}

	ctx->pszSaveName = Misc_StrCpyAlloc(pszSaveName);
	if (!ctx->pszSaveName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	if (pszCustomSavePath)
		{
		ctx->pszCustomSavePath = Misc_StrCpyAlloc(pszCustomSavePath);
		if (!ctx->pszCustomSavePath)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
			Divine_ReleaseContext(ctx);
			return;
			}
		}

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Divine_LoadThread,ctx,0,NULL);
	if (!App.hThread)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_LOADING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	return;
}


// «»»» Tâche de chargement «««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Divine_LoadThread(DIVINECONTEXT *ctx)
{
	Divine_Close();
	Game_Lock(GAME_LOCK_DISABLED|GAME_LOCK_APP);
	Taskbar_SetProgressState(TBPF_INDETERMINATE);

	#if _DEBUG
	ctx->uDebugLevel = DIVINE_DEBUG_ALL;
	#else
	ctx->uDebugLevel = DIVINE_DEBUG_ERROR;
	#endif

	ctx->dwResult = Divine_Execute(DIVINE_EXTRACTARCHIVE,ctx);
	if (ctx->dwResult != ERROR_SUCCESS) goto Done;

	ctx->dwResult = Divine_Execute(DIVINE_GLOBALSLSFTOLSX,ctx);
	if (ctx->dwResult != ERROR_SUCCESS) goto Done;

	ctx->pszPath = Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSX);
	if (ctx->pszPath)
		{
		if (xml_LoadFile(ctx->pszPath))
			{
			Game_BuildPlayers();
			Game_UpdateButtons();
			Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_FILE);
			Misc_SetWindowText(App.hWnd,&App.pszWindowTitle,szTitle,szTitleFmt,szTitle,ctx->pszSaveName);

			LastFiles_Add(ctx->uGame,ctx->pszProfile,ctx->pszSaveName,ctx->pszCustomSavePath);

			CopyMemory(&App.Game.Save.ftLastWrite,&ctx->ftLastWrite,sizeof(FILETIME));
			if (ctx->pszProfile)
				{
				if (App.Config.pszProfile) HeapFree(App.hHeap,0,App.Config.pszProfile);
				App.Config.pszProfile = ctx->pszProfile;
				ctx->pszProfile = NULL;
				}

			App.Config.uGame = ctx->uGame;

			if (App.Game.Save.pszCustomSavePath) HeapFree(App.hHeap,0,App.Game.Save.pszCustomSavePath);
			if (App.Game.Save.pszSaveName)  HeapFree(App.hHeap,0,App.Game.Save.pszSaveName);
			App.Game.Save.pszCustomSavePath = ctx->pszCustomSavePath;
			App.Game.Save.pszSaveName = ctx->pszSaveName;
			ctx->pszSaveName = NULL;
			ctx->pszCustomSavePath = NULL;
			}
		}
	else xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);

Done:	if (ctx->dwResult != ERROR_SUCCESS)
		{
		SetLastError(ctx->dwResult);
		if (!ctx->bNoErrorMsg) xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);
		}

	Taskbar_SetProgressState(TBPF_NOPROGRESS);
	Status_SetText(Locale_GetText(TEXT_DONE));
	Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_APP);
	Divine_ReleaseContext(ctx);
	App.hThread = NULL;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sauvegarde							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création d'une sauvegarde particulière ««««««««««««««««««««««««««»

void Divine_WriteAs()
{
	DIVINECONTEXT*	ctx;
	IFileDialog*	pFileDialog;
	IShellItem*	pItem;
	WCHAR*		pszPath;
	HRESULT		hr;

	ctx = Divine_PrepareContext(TEXT_ERR_SAVING);
	if (!ctx) return;

	//--- Copie le jeu ciblé ---

	ctx->uGame = App.Config.uGame;

	//--- Copie du nom ---

	ctx->pszSaveName = Misc_StrCpyAlloc(App.Game.Save.pszSaveName);
	if (!ctx->pszSaveName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	//--- Sélection du répertoire cible ---

	hr = CoCreateInstance(&CLSID_FileOpenDialog,NULL,CLSCTX_INPROC,&IID_IFileDialog,(void *)&pFileDialog);
	if (SUCCEEDED(hr))
		{
		pFileDialog->lpVtbl->SetTitle(pFileDialog,Locale_GetText(TEXT_DIALOG_SAVE_LOCATION));
		pFileDialog->lpVtbl->SetOptions(pFileDialog,FOS_PICKFOLDERS|FOS_FORCEFILESYSTEM);

		if (App.Game.Save.pszCustomSavePath)
			{
			pszPath = Misc_StrCpyAlloc(App.Game.Save.pszCustomSavePath);
			if (pszPath)
				{
				*(PathFindFileName(pszPath)) = 0;
				hr = SHCreateItemFromParsingName(pszPath,NULL,&IID_IShellItem,(void *)&pItem);
				if (SUCCEEDED(hr))
					{
					pFileDialog->lpVtbl->SetFolder(pFileDialog,pItem);
					pItem->lpVtbl->Release(pItem);
					}
				HeapFree(App.hHeap,0,pszPath);
				}
			}

		hr = pFileDialog->lpVtbl->Show(pFileDialog,App.hWnd);
		if (SUCCEEDED(hr))
			{
			hr = pFileDialog->lpVtbl->GetResult(pFileDialog,&pItem);
			if (SUCCEEDED(hr))
				{
				hr = pItem->lpVtbl->GetDisplayName(pItem,SIGDN_FILESYSPATH,&pszPath);
				if (SUCCEEDED(hr))
					{
					UINT uLen = wcslen(pszPath);
					uLen += 1;
					uLen += wcslen(ctx->pszSaveName);
					uLen += wcslen(szLSVext);
					uLen += 1;
					ctx->pszCustomSavePath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR));
					if (ctx->pszCustomSavePath)
						{
						wcscpy(ctx->pszCustomSavePath,pszPath);
						PathAppend(ctx->pszCustomSavePath,ctx->pszSaveName);
						wcscat(ctx->pszCustomSavePath,szLSVext);
						}
					else hr = E_OUTOFMEMORY;
					CoTaskMemFree(pszPath);
					}
				pItem->lpVtbl->Release(pItem);
				}
			}
		pFileDialog->lpVtbl->Release(pFileDialog);
		}

	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) return;

	if (FAILED(hr))
		{
		MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	//--- Sauvegarde ---

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Divine_SaveThread,ctx,0,NULL);
	if (!App.hThread)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	return;
}


// «»»» Préparation de la sauvegarde ««««««««««««««««««««««««««««««««««««»

void Divine_Write()
{
	DIVINECONTEXT*	ctx;

	if (!App.Game.Save.pszSaveName) return;
	if (!App.Game.Save.pszCustomSavePath && !App.Config.pszProfile) return;

	ctx = Divine_PrepareContext(TEXT_ERR_SAVING);
	if (!ctx) return;

	//--- Copie le jeu ciblé ---

	ctx->uGame = App.Config.uGame;

	//--- Copie du nom de profil (optionnel) ---

	if (App.Config.pszProfile)
		{
		ctx->pszProfile = Misc_StrCpyAlloc(App.Config.pszProfile);
		if (!ctx->pszProfile)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
			Divine_ReleaseContext(ctx);
			return;
			}
		}

	//--- Copie du nom ---

	ctx->pszSaveName = Misc_StrCpyAlloc(App.Game.Save.pszSaveName);
	if (!ctx->pszSaveName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	//--- Copie de l'emplacement (optionnel) ---

	if (App.Game.Save.pszCustomSavePath)
		{
		ctx->pszCustomSavePath = Misc_StrCpyAlloc(App.Game.Save.pszCustomSavePath);
		if (!ctx->pszCustomSavePath)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
			Divine_ReleaseContext(ctx);
			return;
			}
		}

	//--- Vérifie si la sauvegarde a été modifiée ---

	if (Divine_IsSaveGameChanged(App.hWnd,Locale_GetText(TEXT_MODIFIED_SAVEGAME),App.Config.uGame,App.Config.pszProfile,App.Game.Save.pszSaveName,App.Game.Save.pszCustomSavePath,&App.Game.Save.ftLastWrite))
		{
		Divine_ReleaseContext(ctx);
		return;
		}

	//--- Sauvegarde ---

	App.hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Divine_SaveThread,ctx,0,NULL);
	if (!App.hThread)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_SAVING),NULL,MB_ICONERROR);
		Divine_ReleaseContext(ctx);
		return;
		}

	return;
}


// «»»» Tâche de sauvegarde «««««««««««««««««««««««««««««««««««««««««««««»

DWORD WINAPI Divine_SaveThread(DIVINECONTEXT *ctx)
{
	Game_Lock(GAME_LOCK_DISABLED|GAME_LOCK_ALL);
	Taskbar_SetProgressState(TBPF_INDETERMINATE);

	#if _DEBUG
	ctx->uDebugLevel = DIVINE_DEBUG_ALL;
	#else
	ctx->uDebugLevel = DIVINE_DEBUG_ERROR;
	#endif

	//--- globals.lsx > globals.lsf ---

	ctx->pszPath = Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSX);
	if (ctx->pszPath)
		{
		if (!xml_SaveFile(ctx->pszPath,XML_TARGET_GLOBALS,NULL)) goto Done;
		}
	else
		{
		xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);
		goto Done;
		}
	ctx->dwResult = Divine_Execute(DIVINE_GLOBALSLSXTOLSF,ctx);
	if (ctx->dwResult != ERROR_SUCCESS) goto Done;

	//--- meta.lsx > meta.lsf ---

	if (lsv_GetMetaXML(&App.Game.Save.nodeFiles))
		{
		HeapFree(App.hHeap,0,ctx->pszPath);
		ctx->pszPath = Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szMetaLSX);
		if (ctx->pszPath)
			{
			if (!xml_SaveFile(ctx->pszPath,XML_TARGET_META,NULL)) goto Done;
			}
		else
			{
			xml_SendErrorMsg(XML_ERROR_FROM_SYSTEM,0);
			goto Done;
			}
		ctx->dwResult = Divine_Execute(DIVINE_METALSXTOLSF,ctx);
		if (ctx->dwResult != ERROR_SUCCESS) goto Done;
		}

	//--- Create archive.lsv and replace savegame ---

	#if !_DEBUG || !DIVINE_SIMULATION
	ctx->dwResult = Divine_Execute(DIVINE_CREATEARCHIVE,ctx);
	if (ctx->dwResult == ERROR_SUCCESS)
		{
		LastFiles_Add(ctx->uGame,ctx->pszProfile,ctx->pszSaveName,ctx->pszCustomSavePath);
		CopyMemory(&App.Game.Save.ftLastWrite,&ctx->ftLastWrite,sizeof(FILETIME));
		if (App.Game.Save.pszCustomSavePath) HeapFree(App.hHeap,0,App.Game.Save.pszCustomSavePath);
		App.Game.Save.pszCustomSavePath = ctx->pszCustomSavePath;
		ctx->pszCustomSavePath = NULL;
		}
	#else
	#warning Save routine in simulation mode !
	#endif

	//--- Done ---

Done:	Taskbar_SetProgressState(TBPF_NOPROGRESS);
	Status_SetText(Locale_GetText(TEXT_DONE));
	Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_ALL);
	Divine_ReleaseContext(ctx);
	App.hThread = NULL;
	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Vérifications et initialisations de base ««««««««««««««««««««««««»

DIVINECONTEXT* Divine_PrepareContext(UINT uErr)
{
	DIVINECONTEXT*	ctx;

	if (App.hThread)
		{
		MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_RUNNING),NULL,MB_ICONERROR);
		return(NULL);
		}

	if (!PathFileExists(szDivineEXE))
		{
		MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_MISSINGCONVERTER),NULL,MB_ICONERROR);
		return(NULL);
		}

	ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DIVINECONTEXT));
	if (!ctx)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(uErr),NULL,MB_ICONERROR);
		return(NULL);
		}

	return(ctx);
}


// «»»» Exécution d'une opération (divine.exe) ««««««««««««««««««««««««««»

DWORD Divine_Execute(UINT uType, DIVINECONTEXT *ctx)
{
	SECURITY_ATTRIBUTES	Security;
	PROCESS_INFORMATION	ProcInfo;
	STARTUPINFO		Startup;
	HANDLE			hChildStdOutRead,hChildStdOutWrite;
	HANDLE			hChildStdInRead,hChildStdInWrite;
	WCHAR*			pszFmt;
	WCHAR*			pszParameters;
	WCHAR*			pszTemp;
	DWORD			dwLastError;
	DWORD			dwRead;
	DWORD_PTR		vl[4];

	dwLastError = ERROR_SUCCESS;
	hChildStdOutRead = hChildStdOutWrite = INVALID_HANDLE_VALUE;
	hChildStdInRead = hChildStdInWrite = INVALID_HANDLE_VALUE;
	vl[1] = vl[2] = 0;
	vl[3] = (DWORD_PTR)(ctx->uGame == DIVINE_DOS_2?L"dos2":L"dos2de");

	switch(ctx->uDebugLevel)
		{
		case DIVINE_DEBUG_ALL:
			vl[0] = (DWORD_PTR)L"all";
			break;
		default:vl[0] = (DWORD_PTR)L"error";
			break;
		}

	//--- Préparation des paramètres ---

	switch(uType)
		{
		case DIVINE_EXTRACTARCHIVE:
			pszFmt = szDivineExtract;

			vl[1] = (DWORD_PTR)Divine_GetSaveGamePath(ctx->uGame,ctx->pszProfile,ctx->pszSaveName,ctx->pszCustomSavePath);
			if (!vl[1])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			if (!PathFileExists((WCHAR *)vl[1]))
				{
				dwLastError = GetLastError();
				goto Done;
				}

			vl[2] = (DWORD_PTR)Divine_CreateTempPath(3,szTempPath,szSavegames,ctx->pszSaveName);
			if (!vl[2])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			if (!Misc_GetFileTime((WCHAR *)vl[1],NULL,NULL,&ctx->ftLastWrite))
				{
				dwLastError = GetLastError();
				goto Done;
				}

			Status_SetText(Locale_GetText(TEXT_LOADING_EXTRACT),ctx->pszSaveName);
			break;

		case DIVINE_CREATEARCHIVE:
			pszFmt = szDivineCreate;

			vl[1] = (DWORD_PTR)Divine_GetTempPath(3,szTempPath,szSavegames,ctx->pszSaveName);
			if (!vl[1])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			vl[2] = (DWORD_PTR)Divine_GetTempPath(3,szTempPath,szSavegames,ctx->pszSaveName);
			if (!vl[2])
				{
				dwLastError = GetLastError();
				goto Done;
				}
			pszTemp = HeapAlloc(App.hHeap,0,wcslen((WCHAR *)vl[2])*sizeof(WCHAR)+wcslen(szLSVext)*sizeof(WCHAR)+sizeof(WCHAR));
			if (!pszTemp)
				{
				dwLastError = GetLastError();
				goto Done;
				}
			wcscpy(pszTemp,(WCHAR *)vl[2]);
			wcscat(pszTemp,szLSVext);
			HeapFree(App.hHeap,0,(WCHAR *)vl[2]);
			vl[2] = (DWORD_PTR)pszTemp;

			Status_SetText(Locale_GetText(TEXT_SAVING_CREATE),ctx->pszSaveName);
			break;

		case DIVINE_GLOBALSLSFTOLSX:
			pszFmt = szDivineConvertLSF;

			vl[1] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSF);
			if (!vl[1])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			vl[2] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSX);
			if (!vl[2])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			Status_SetText(Locale_GetText(TEXT_LOADING_CONVERTLSF),ctx->pszSaveName,szGlobalsLSF);
			break;

		case DIVINE_GLOBALSLSXTOLSF:
			pszFmt = szDivineConvertLSX;

			vl[1] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSX);
			if (!vl[1])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			vl[2] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szGlobalsLSF);
			if (!vl[2])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			Status_SetText(Locale_GetText(TEXT_SAVING_CONVERTLSX),ctx->pszSaveName,szGlobalsLSX);
			break;

		case DIVINE_METALSXTOLSF:
			pszFmt = szDivineConvertLSX;

			vl[1] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szMetaLSX);
			if (!vl[1])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			vl[2] = (DWORD_PTR)Divine_GetTempPath(4,szTempPath,szSavegames,ctx->pszSaveName,szMetaLSF);
			if (!vl[2])
				{
				dwLastError = GetLastError();
				goto Done;
				}

			Status_SetText(Locale_GetText(TEXT_SAVING_CONVERTLSX),ctx->pszSaveName,szGlobalsLSX);
			break;
		}

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszFmt,0,0,(WCHAR *)&pszParameters,1,(va_list *)&vl))
		{
		dwLastError = GetLastError();
		goto Done;
		}

	//--- Exécution de l'opération ---

	Security.nLength = sizeof(SECURITY_ATTRIBUTES);
	Security.bInheritHandle = TRUE;
	Security.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&hChildStdOutRead,&hChildStdOutWrite,&Security,0))
		{
		dwLastError = GetLastError();
		goto Done;
		}
	if (!SetHandleInformation(hChildStdOutRead,HANDLE_FLAG_INHERIT,0))
		{
		dwLastError = GetLastError();
		goto Done;
		}
	if (!CreatePipe(&hChildStdInRead,&hChildStdInWrite,&Security,0))
		{
		dwLastError = GetLastError();
		goto Done;
		}
	if (!SetHandleInformation(hChildStdInWrite,HANDLE_FLAG_INHERIT,0))
		{
		dwLastError = GetLastError();
		goto Done;
		}

	ZeroMemory(&Startup,sizeof(STARTUPINFO));
	ZeroMemory(&ProcInfo,sizeof(PROCESS_INFORMATION));
	Startup.cb = sizeof(STARTUPINFO);
	Startup.dwFlags = STARTF_USESTDHANDLES;
	Startup.hStdInput = hChildStdInRead;
	Startup.hStdOutput = hChildStdOutWrite;
	Startup.hStdError = hChildStdOutWrite;
	if (CreateProcess(szDivineEXE,pszParameters,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&Startup,&ProcInfo))
		{
		WaitForSingleObject(ProcInfo.hProcess,INFINITE);
		CloseHandle(ProcInfo.hProcess);
		CloseHandle(ProcInfo.hThread);
		if (PeekNamedPipe(hChildStdOutRead,NULL,0,NULL,&dwRead,NULL) && dwRead)
			{
			ctx->pszLog = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,dwRead+1);
			if (!ctx->pszLog)
				{
				dwLastError = ERROR_NOT_ENOUGH_MEMORY;
				goto Done;
				}
			PeekNamedPipe(hChildStdOutRead,ctx->pszLog,dwRead,NULL,NULL,NULL);
			dwLastError = Divine_ParseLog(ctx);
			HeapFree(App.hHeap,0,ctx->pszLog);
			ctx->pszLog = NULL;
			}
		}
	else dwLastError = GetLastError();
	LocalFree(pszParameters);

	//--- Terminé ! ---

Done:	if (dwLastError == ERROR_SUCCESS)
		{
		switch (uType)
			{
			case DIVINE_METALSXTOLSF:
			case DIVINE_GLOBALSLSXTOLSF:
				#if !_DEBUG || !DIVINE_SIMULATION
				if (!DeleteFile((WCHAR *)vl[1])) dwLastError = GetLastError();
				#endif
				break;

			case DIVINE_CREATEARCHIVE:
				pszTemp = Divine_GetSaveGamePath(ctx->uGame,ctx->pszProfile,ctx->pszSaveName,ctx->pszCustomSavePath);
				if (pszTemp)
					{
					if (!MoveFileEx((WCHAR *)vl[2],pszTemp,MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING)) dwLastError = GetLastError();
					else if (!Misc_GetFileTime(pszTemp,NULL,NULL,&ctx->ftLastWrite)) dwLastError = GetLastError();
					HeapFree(App.hHeap,0,pszTemp);
					}
				else dwLastError = GetLastError();
				break;
			}
		}

	if (hChildStdInRead != INVALID_HANDLE_VALUE) CloseHandle(hChildStdInRead);
	if (hChildStdInWrite != INVALID_HANDLE_VALUE) CloseHandle(hChildStdInWrite);
	if (hChildStdOutWrite != INVALID_HANDLE_VALUE) CloseHandle(hChildStdOutWrite);
	if (hChildStdOutRead != INVALID_HANDLE_VALUE) CloseHandle(hChildStdOutRead);
	if (vl[2]) HeapFree(App.hHeap,0,(WCHAR *)vl[2]);
	if (vl[1]) HeapFree(App.hHeap,0,(WCHAR *)vl[1]);
	return(dwLastError);
}


// «»»» Libération des données de gestion «««««««««««««««««««««««««««««««»

void Divine_ReleaseContext(DIVINECONTEXT *ctx)
{
	if (!ctx) return;
	if (ctx->pszCustomSavePath) HeapFree(App.hHeap,0,ctx->pszCustomSavePath);
	if (ctx->pszSaveName) HeapFree(App.hHeap,0,ctx->pszSaveName);
	if (ctx->pszProfile) HeapFree(App.hHeap,0,ctx->pszProfile);
	if (ctx->pszPath) HeapFree(App.hHeap,0,ctx->pszPath);
	HeapFree(App.hHeap,0,ctx);
	return;
}


// «»»» Fermeture de la sauvegarde ««««««««««««««««««««««««««««««««««««««»

void Divine_Close()
{
	Misc_SetWindowText(App.hWnd,&App.pszWindowTitle,szTitle,NULL);

	Game_ReleasePlayers();
	xml_ReleaseAll(&App.Game.Save.nodeXMLRoot);
	lsv_Release(&App.Game.Save.nodeFiles);
	Divine_Cleanup();

	if (App.Game.Save.pszCustomSavePath) HeapFree(App.hHeap,0,App.Game.Save.pszCustomSavePath);
	if (App.Game.Save.pszSaveName) HeapFree(App.hHeap,0,App.Game.Save.pszSaveName);
	App.Game.Save.pszCustomSavePath = NULL;
	App.Game.Save.pszSaveName = NULL;

	Game_Lock(GAME_LOCK_DISABLED|GAME_LOCK_FILE|GAME_LOCK_TREE);
	return;
}


// «»»» Nettoyage du répertoire temporaire ««««««««««««««««««««««««««««««»

void Divine_Cleanup()
{
	WCHAR*	pszPath;

	pszPath = Divine_GetTempPath(1,szTempPath);
	if (!pszPath) return;
	Divine_CleanupLoop(pszPath);
	RemoveDirectory(pszPath);
	HeapFree(App.hHeap,0,pszPath);
	return;
}

//--- Boucle de suppression des fichiers ---

int Divine_CleanupLoop(WCHAR *pszPath)
{
	WIN32_FIND_DATA		Find;
	HANDLE			hFile;
	WCHAR*			pszCurrent;
	DWORD			dwLen;

	//--- Copie le répertoire actuel
	dwLen = GetCurrentDirectory(0,NULL);
	pszCurrent = HeapAlloc(App.hHeap,0,dwLen*sizeof(WCHAR));
	if (!pszCurrent)
		{
		HeapFree(App.hHeap,0,pszPath);
		return(0);
		}
	GetCurrentDirectory(dwLen,pszCurrent);

	//--- Modifie le répertoire
	if (!SetCurrentDirectory(pszPath))
		{
		HeapFree(App.hHeap,0,pszCurrent);
		return(0);
		}

	//--- Retrouve les fichiers dans le répertoire
	hFile = FindFirstFile(szWild,&Find);
	if (hFile == INVALID_HANDLE_VALUE)
		{
		HeapFree(App.hHeap,0,pszCurrent);
		return(0);
		}

	do {

		if (!wcscmp(Find.cFileName,szRootPath)) continue;
		if (!wcscmp(Find.cFileName,szParentPath)) continue;

		if (Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			//--- Parcours le répertoire
			if (!Divine_CleanupLoop(Find.cFileName))
				{
				FindClose(hFile);
				HeapFree(App.hHeap,0,pszCurrent);
				return(0);
				}
			//--- Supprime le répertoire
			RemoveDirectory(Find.cFileName);
			continue;
			}

		//--- Supprime le fichier
		DeleteFile(Find.cFileName);

	} while (FindNextFile(hFile,&Find));

	FindClose(hFile);

	//--- Restaure le répertoire actuel
	if (!SetCurrentDirectory(pszCurrent))
		{
		HeapFree(App.hHeap,0,pszCurrent);
		return(0);
		}

	HeapFree(App.hHeap,0,pszCurrent);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Logs								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Analyse du journal ««««««««««««««««««««««««««««««««««««««««««««««»

DWORD Divine_ParseLog(DIVINECONTEXT *ctx)
{
	DIVINELOGHEADER*	pHeader;
	int			i;

	for (i = 0; ctx->pszLog[i] != 0; i++)
		{
		if ((pHeader = Divine_GetHeader(&ctx->pszLog[i])) != NULL)
			{
			if (pHeader->bIsError)
				{
				if (MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_DIVINE),NULL,MB_ICONERROR|MB_YESNO) == IDYES) Divine_ShowLog(ctx);
				ctx->bNoErrorMsg = TRUE;
				return(ERROR_INVALID_DATA);
				}
			}
		while (ctx->pszLog[i] != '\n')
			{
			if (ctx->pszLog[i] == 0)
				{
				i--;
				break;
				}
			i++;
			}
		}

	return(ERROR_SUCCESS);
}


// «»»» Retrouve les informations d'une ligne du journal ««««««««««««««««»

DIVINELOGHEADER* Divine_GetHeader(char *pszLogLine)
{
	int	i;

	for (i = 0; LogHeaders[i].pszHeader != NULL; i++)
		{
		if (strncmp(pszLogLine,LogHeaders[i].pszHeader,strlen(LogHeaders[i].pszHeader))) continue;
		return(&LogHeaders[i]);
		}

	return(NULL);
}


// «»»» Affichage du journal ««««««««««««««««««««««««««««««««««««««««««««»

void Divine_ShowLog(DIVINECONTEXT *ctx)
{
	DIVINELOG*		pLog;
	DIVINELOGHEADER*	pHeader;
	WCHAR*			pszResIcon;
	char*			pszHeader;
	char*			pszStart;
	char*			pszEnd;
	UINT			uLen;
	UINT			uWideLen;

	pszStart = ctx->pszLog;
	while (*pszStart != 0)
		{
		pszEnd = strchr(pszStart,'\r');
		if (!pszEnd) pszEnd = strchr(pszStart,'\n');
		if (!pszEnd) pszEnd = strchr(pszStart,'\0');
		if (!pszEnd) break;
		pHeader = Divine_GetHeader(pszStart);
		if (pHeader) pszResIcon = pHeader->pszResIcon;
		else pszResIcon = NULL;
		pszHeader = strchr(pszStart,']');
		if (pszHeader) pszStart = ++pszHeader;
		uLen = pszEnd-pszStart;
		uWideLen = MultiByteToWideChar(CP_ACP,0,pszStart,uLen,NULL,0);
		pLog = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DIVINELOG)+uWideLen*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pLog) break;
		pLog->icon = pszResIcon;
		MultiByteToWideChar(CP_ACP,0,pszStart,uLen,pLog->line,uWideLen);
		List_AddEntry((NODE *)pLog,&ctx->Log);
		pszStart = pszEnd;
		if (*pszStart == '\r') pszStart++;
		if (*pszStart == 0) break;
		if (*pszStart == '\n') pszStart++;
		}

	DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1008),App.hWnd,Divine_LogProc,(LPARAM)&ctx->Log);
	List_ReleaseMemory(&ctx->Log);
	return;
}


// «»»» Messages de la boîte de dialogue ««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Divine_LogProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	if (uMsgId == WM_MEASUREITEM)
		{
		((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
		((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
		if (((MEASUREITEMSTRUCT *)lParam)->itemHeight < DIVINE_ICON_SIZE+4) ((MEASUREITEMSTRUCT *)lParam)->itemHeight = DIVINE_ICON_SIZE+4;
		return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		DIVINELOG*	pLog;
		LRESULT		lResult;

		for (pLog = (DIVINELOG *)((NODE *)lParam)->next; pLog != NULL; pLog = (DIVINELOG *)pLog->node.next)
			{
			lResult = SendDlgItemMessage(hDlg,200,LB_ADDSTRING,0,(LPARAM)pLog);
			if (lResult == LB_ERR || lResult == LB_ERRSPACE)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			}

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TITLE_LOG));
		Dialog_CenterWindow(hDlg,App.hWnd);
		return(FALSE);
		}

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 200:
					Divine_DrawLogLine((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
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

//--- Affichage d'une ligne ---

void Divine_DrawLogLine(DRAWITEMSTRUCT *pDraw)
{
	DIVINELOG*	pLog;
	HFONT		hFont;
	RECT		rcDraw;
	COLORREF	crText;
	int		iBack;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	pLog = (DIVINELOG *)pDraw->itemData;
	if (!pLog) return;
	if (pLog == (DIVINELOG *)-1) return;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	CopyRect(&rcDraw,&pDraw->rcItem);
	rcDraw.left += 4;
	rcDraw.right -= 14;
	rcDraw.top += 1;
	rcDraw.bottom -= 1;
	if (pLog->icon) DrawIconEx(pDraw->hDC,rcDraw.left,rcDraw.top+(rcDraw.bottom-rcDraw.top-DIVINE_ICON_SIZE)/2,LoadImage(NULL,MAKEINTRESOURCE(pLog->icon),IMAGE_ICON,DIVINE_ICON_SIZE,DIVINE_ICON_SIZE,LR_DEFAULTCOLOR|LR_SHARED),DIVINE_ICON_SIZE,DIVINE_ICON_SIZE,0,NULL,DI_NORMAL);
	rcDraw.left += DIVINE_ICON_SIZE+4;
	DrawText(pDraw->hDC,pLog->line,-1,&rcDraw,DT_LEFT|DT_NOPREFIX|DT_PATH_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Vérifie que la sauvegarde n'a pas été modifiée ««««««««««««««««««»

int Divine_IsSaveGameChanged(HWND hWnd, WCHAR *pszText, UINT uGame, WCHAR *pszProfile, WCHAR *pszSaveName, WCHAR *pszCustomSavePath, FILETIME *pLastWrite)
{
	FILETIME	ftLastWrite;
	WCHAR*		pszPath;
	int		iResult;

	iResult = 1;
	pszPath = Divine_GetSaveGamePath(uGame,pszProfile,pszSaveName,pszCustomSavePath);
	if (pszPath)
		{
		if (!PathFileExists(pszPath))
			{
			HeapFree(App.hHeap,0,pszPath);
			return(0);
			}
		if (Misc_GetFileTime(pszPath,NULL,NULL,&ftLastWrite))
			{
			if (CompareFileTime(&ftLastWrite,pLastWrite))
				{
				if (MessageBox(hWnd,pszText,Locale_GetText(TEXT_TITLE_REQUEST),MB_ICONQUESTION|MB_YESNO) == IDYES)
					iResult = 0;
				}
			else iResult = 0;
			}
		HeapFree(App.hHeap,0,pszPath);
		}

	return(iResult);
}


// «»»» Création du répertoire temporaire «««««««««««««««««««««««««««««««»

WCHAR* Divine_CreateTempPath(UINT uNumPaths, ...)
{
	WCHAR*		pszTempPath;
	WCHAR*		pszPath;
	BOOL		bResult;
	UINT		uLen;
	UINT		uNum;
	va_list		vl;

	//--- Récupère le répertoire temporaire du système
	pszTempPath = Divine_GetTempPath(0);
	if (!pszTempPath) return(NULL);

	//--- Calcul la taille finale du répertoire temporaire
	va_start(vl,uNumPaths);
	uLen = wcslen(pszTempPath);
	uNum = uNumPaths;
	while (uNum)
		{
		uLen += 1+wcslen(va_arg(vl,WCHAR *));
		uNum--;
		}
	va_end(vl);

	//--- Alloue la mémoire pour le répertoire temporaire
	pszPath = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszPath)
		{
		HeapFree(App.hHeap,0,pszTempPath);
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(NULL);
		}

	//--- Boucle de création des répertoires
	va_start(vl,uNumPaths);
	wcscpy(pszPath,pszTempPath);
	bResult = 1;
	while(uNumPaths)
		{
		PathAppend(pszPath,va_arg(vl,WCHAR *));
		if (!PathFileExists(pszPath))
			{
			bResult = CreateDirectory(pszPath,NULL);
			if (!bResult)
				{
				HeapFree(App.hHeap,0,pszPath);
				pszPath = NULL;
				break;
				}
			}
		uNumPaths--;
		}
	va_end(vl);

	HeapFree(App.hHeap,0,pszTempPath);
	return(pszPath);
}


// «»»» Détermine un chemin pour un fichier/dossier temporaire ««««««««««»

WCHAR*	Divine_GetTempPath(UINT uNumPaths, ...)
{
	WCHAR*	pszPath;
	UINT	uNum;
	UINT	uLen;
	va_list	vl;

	//--- Calcul la taille totale du répertoire temporaire
	va_start(vl,uNumPaths);
	uLen = App.Config.pszTempPath?wcslen(App.Config.pszTempPath):1;
	uNum = uNumPaths;
	while (uNum)
		{
		uLen += 1+wcslen(va_arg(vl,WCHAR *));
		uNum--;
		}
	va_end(vl);

	//--- Alloue la mémoire pour le répertoire temporaire
	pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszPath)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(NULL);
		}

	//--- Copie du répertoire temporaire
	va_start(vl,uNumPaths);
	if (App.Config.pszTempPath) wcscpy(pszPath,App.Config.pszTempPath);
	while (uNumPaths)
		{
		PathAppend(pszPath,va_arg(vl,WCHAR *));
		uNumPaths--;
		}
	return(pszPath);
}


// «»»» Recupère le chemin du fichier de sauvegarde «««««««««««««««««««««»

WCHAR* Divine_GetSaveGamePath(UINT uGame, WCHAR *pszProfile, WCHAR *pszSaveName, WCHAR *pszCustomSavePath)
{
	WCHAR*	pszPath;
	UINT	uLen;

	if (pszCustomSavePath)
		{
		uLen = wcslen(pszCustomSavePath);
		pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pszPath)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(NULL);
			}
		wcscpy(pszPath,pszCustomSavePath);
		}
	else
		{
		uLen  = App.Config.pszLarianPath?(wcslen(App.Config.pszLarianPath)*sizeof(WCHAR)):0;
		uLen += sizeof(WCHAR)+wcslen(Divine_GetGameName(uGame))*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(szPlayerProfiles)*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(pszProfile)*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(szSavegames)*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(szStory)*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(pszSaveName)*sizeof(WCHAR);
		uLen += sizeof(WCHAR)+wcslen(pszSaveName)*sizeof(WCHAR);
		uLen += wcslen(szLSVext)*sizeof(WCHAR);
		pszPath = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,uLen+sizeof(WCHAR));
		if (!pszPath)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(NULL);
			}
		if (App.Config.pszLarianPath) wcscpy(pszPath,App.Config.pszLarianPath);
		PathAppend(pszPath,Divine_GetGameName(uGame));
		PathAppend(pszPath,szPlayerProfiles);
		PathAppend(pszPath,pszProfile);
		PathAppend(pszPath,szSavegames);
		PathAppend(pszPath,szStory);
		PathAppend(pszPath,pszSaveName);
		PathAppend(pszPath,pszSaveName);
		wcscat(pszPath,szLSVext);
		}

	return(pszPath);
}


// «»»» Recupère le nom du jeu ««««««««««««««««««««««««««««««««««««««««««»

//--- A partir d'un identifiant ---

WCHAR* Divine_GetGameName(UINT uGame)
{
	return(uGame == DIVINE_DOS_2?szGameName:szGameNameEE);
}

//--- A partir d'un nom ---

UINT Divine_GetGameFromName(WCHAR *pszName, UINT uLen)
{
	if (uLen == wcslen(szGameNameEE) && !wcsncmp(szGameNameEE,pszName,uLen)) return(DIVINE_DOS_2EE);
	return(DIVINE_DOS_2);
}


// «»»» Exécute le programme de conversion ««««««««««««««««««««««««««««««»

void Divine_RunConverter()
{
	if (!PathFileExists(szConverterEXE))
		{
		MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_MISSINGCONVERTER),NULL,MB_ICONERROR);
		return;
		}

	ShellExecute(App.hWnd,L"open",szConverterEXE,NULL,NULL,SW_SHOWNORMAL);
	return;
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Edition
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Locale.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION	App;
extern GAMEQUALITY	Qualities[];

static GAMEEDITPAGE	Pages[] = {
					{ GAME_PAGE_NAME, TEXT_DIALOG_TITLE_NAME, TEXT_DIALOG_INFO_NAME, 6000 },
					{ GAME_PAGE_AMOUNT, TEXT_DIALOG_TITLE_AMOUNT, TEXT_DIALOG_INFO_AMOUNT, 6001 },
					{ GAME_PAGE_BOOSTERS, TEXT_DIALOG_TITLE_BOOSTERS, TEXT_DIALOG_INFO_BOOSTERS, 6002 },
					{ GAME_PAGE_BONUSES, TEXT_DIALOG_TITLE_BONUSES, TEXT_DIALOG_INFO_BONUSES, 6003 },
					{ GAME_PAGE_RUNES, TEXT_DIALOG_TITLE_RUNES, TEXT_DIALOG_INFO_RUNES, 6004 },
					{ GAME_PAGE_SYNCHRONIZE, TEXT_DIALOG_TITLE_SYNCHRONIZE, TEXT_DIALOG_INFO_SYNCHRONIZE, 6005 },
					{ 0 }
				};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Création de la boîte de dialogue					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Game_Edit(DOS2ITEM *pItem, UINT uPageID)
{
	PROPSHEETHEADER*	psh;
	PROPSHEETPAGE*		psp;
	GAMEEDITITEMCONTEXT*	pItemContext;
	const WCHAR*		pszStartPage;
	int			iResult;
	int			iNumPages;
	int			i;

	pszStartPage = NULL;
	psh = NULL;
	psp = NULL;
	pItemContext = NULL;
	iResult = -1;

	//--- Alloue les structures ---

	psh = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETHEADER));
	if (!psh)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	for (i = 0, iNumPages = 0; Pages[i].uPageID != 0; i++, iNumPages++); // Allocate all pages even if that's not really needed
	psp = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(PROPSHEETPAGE)*iNumPages);
	if (!psp)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	pItemContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITITEMCONTEXT));
	if (!pItemContext)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		goto Done;
		}

	if (!Locale_Load(App.hWnd,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_MISC,(void **)&pItemContext->pLocale,NULL))
		{
		iResult = 0;
		goto Done;
		}

	//--- Copie les propriétés de l'objet ---

	pItemContext->pItem = pItem;
	pItemContext->pszStats = xml_GetThisAttrValue(pItem->pxaStats);
	pItemContext->bIsGenerated = xml_IsTrue(pItem->pxaIsGenerated);
	pItemContext->uLevel = wcstol(xml_GetThisAttrValue(pItem->pxaLevel),NULL,10);
	pItemContext->uSlot = wcstol(xml_GetThisAttrValue(pItem->pxaSlot),NULL,10);
	pItemContext->bBonuses = pItem->pxnPermanentBoost?TRUE:FALSE;
	pItemContext->bHasRunes = (pItem->pxaRunes[0] && pItem->pxaRunes[1] && pItem->pxaRunes[2])?TRUE:FALSE;
	pItemContext->uNewLevel = 1;
	pItemContext->iAmount = pItemContext->iAmountOld = pItem->pxaAmount?wcstol(xml_GetThisAttrValue(pItem->pxaAmount),NULL,10):1;
	pItemContext->uFilter = Game_GetItemFlags(pItemContext->pszStats);
	if (!pItemContext->uFilter) pItemContext->uFilter = FILTER_ALL;
	else pItemContext->uFilter |= (FILTER_ALL_TYPES);

	//--- Retire la quantité pour les objets qui ne s'empilent pas ---

	if (pItemContext->pszStats && pItemContext->iAmount == 1)
		{
		static WCHAR*	pszAllowed[] = { L"WPN_Arrow_", L"WPN_ArrowHead_", L"WPN_ArrowShaft_", NULL };
		BOOL		bAllowed = FALSE;
		UINT		uLen = wcslen(pItemContext->pszStats);

		for (i = 0; pszAllowed[i] != NULL; i++)
			{
			if (Game_CompareStrings(pszAllowed[i],pItemContext->pszStats,uLen,CMP_TYPE_BEGIN))
				{
				bAllowed = TRUE;
				break;
				}
			}

		if (!bAllowed)
			{
			if (Game_CompareStrings(L"ARM_",pItemContext->pszStats,uLen,CMP_TYPE_BEGIN)) pItemContext->iAmount = pItemContext->iAmountOld = -1;
			else if (Game_CompareStrings(L"WPN_",pItemContext->pszStats,uLen,CMP_TYPE_BEGIN)) pItemContext->iAmount = pItemContext->iAmountOld = -1;
			else if (Game_CompareStrings(L"CONT_",pItemContext->pszStats,uLen,CMP_TYPE_BEGIN)) pItemContext->iAmount = pItemContext->iAmountOld = -1;
			}
		}

	//--- Création des pages ---

	if (uPageID == 0) // Sélection automatique de la page la plus appropriée
		{
		if (pItemContext->iAmount > 0) uPageID = GAME_PAGE_AMOUNT;
		else uPageID = GAME_PAGE_NAME;
		}

	for (i = 0, iNumPages = 0; Pages[i].uPageID != 0; i++) // Création des pages
		{
		GAMEEDITPAGECONTEXT*	ctx;

		if (Pages[i].uPageID == GAME_PAGE_AMOUNT && pItemContext->iAmount == -1) continue;
		else if (Pages[i].uPageID == GAME_PAGE_BOOSTERS && !pItemContext->bIsGenerated) continue;
		else if (Pages[i].uPageID == GAME_PAGE_BONUSES && !pItemContext->bBonuses) continue;
		else if (Pages[i].uPageID == GAME_PAGE_RUNES && !pItemContext->bHasRunes) continue;
		else if (Pages[i].uPageID == GAME_PAGE_SYNCHRONIZE && !pItemContext->uLevel) continue;

		ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITPAGECONTEXT));
		if (!ctx)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			goto Done;
			}

		ctx->item.pContext = pItemContext;
		ctx->uPageID = Pages[i].uPageID;
		ctx->pszInfo = Locale_GetText(Pages[i].uInfoID);

		psp[iNumPages].dwSize = sizeof(PROPSHEETPAGE);
		psp[iNumPages].dwFlags = PSP_USETITLE;
		psp[iNumPages].hInstance = App.hInstance;
		psp[iNumPages].pszTemplate = MAKEINTRESOURCE(Pages[i].uResID);
		psp[iNumPages].pszTitle = Locale_GetText(Pages[i].uTitleID);
		psp[iNumPages].pfnDlgProc = (DLGPROC)Game_EditProc;
		psp[iNumPages].lParam = (LPARAM)ctx;

		if (uPageID == Pages[i].uPageID) pszStartPage = psp[iNumPages].pszTitle;
		iNumPages++;
		}

	//--- Initialisations ---

	for (i = 0; i != iNumPages; i++)
		{
		GAMEEDITPAGECONTEXT *ctx = (GAMEEDITPAGECONTEXT *)psp[i].lParam;
		switch(ctx->uPageID)
			{
			case GAME_PAGE_NAME:
				if (pItem->pxaDisplayName)
					if (!Game_EditNameCopy(pItem->pxaDisplayName,&pItemContext->pszDisplayName)) goto Done;
				if (pItem->pxaDescription)
					if (!Game_EditNameCopy(pItem->pxaDescription,&pItemContext->pszDescription)) goto Done;
				break;
			case GAME_PAGE_AMOUNT:
				break;
			case GAME_PAGE_BOOSTERS:
				iResult = Game_EditBoostersCopy(pItemContext,pItem->pxnGeneration);
				if (iResult == 0 || iResult == -1) goto Done;
				break;
			case GAME_PAGE_BONUSES:
				iResult = Game_EditBonusCopy(pItemContext,pItem->pxnPermanentBoost);
				if (iResult == 0 || iResult == -1) goto Done;
				break;
			case GAME_PAGE_RUNES:
				if (!Game_EditRunesCopy(pItem,pItemContext)) goto Done;
				break;
			case GAME_PAGE_SYNCHRONIZE:
				if (App.Game.pdcCurrent)
					{
					WCHAR*		szLevel;

					szLevel = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
					if (szLevel) pItemContext->uNewLevel = Game_GetLevelFromExp(wcstol(szLevel,NULL,10));
					}
				break;
			}
		}

	//--- Affichage de la boîte de dialogue ---

	psh->dwSize = sizeof(PROPSHEETHEADER);
	psh->dwFlags = PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_NOAPPLYNOW|PSH_USEPSTARTPAGE|PSH_NOCONTEXTHELP;
	psh->hwndParent = App.hWnd;
	psh->hInstance = App.hInstance;
	psh->pszIcon = MAKEINTRESOURCE(1);
	psh->pszCaption = Locale_GetText(TEXT_DIALOG_EDIT);
	psh->nPages = iNumPages;
	psh->pStartPage = pszStartPage?pszStartPage:psp[0].pszTitle;
	psh->ppsp = psp;

	iResult = PropertySheet(psh);

	//--- Application des résultats ---

	if (iResult)
		{
		for (i = 0; i != iNumPages; i++)
			{
			GAMEEDITPAGECONTEXT *ctx = (GAMEEDITPAGECONTEXT *)psp[i].lParam;
			if (!ctx->bPageSet) continue;
			switch(ctx->uPageID)
				{
				case GAME_PAGE_NAME:
					Game_EditNameSet(pItem,&pItem->pxaDescription,pItemContext->pszDescription,L"CustomDescription");
					Game_EditNameSet(pItem,&pItem->pxaDisplayName,pItemContext->pszDisplayName,L"CustomDisplayName");
					Game_ItemDisplayNameRelease(pItem);
					break;
				case GAME_PAGE_AMOUNT:
					if (pItemContext->iAmount == 0) pItemContext->iAmount = 1;
					if (pItemContext->iAmount != pItemContext->iAmountOld)
						{
						if (!pItem->pxaAmount)
							{
							if (pItemContext->iAmount > 1)
								{
								XML_NODE *pxnAmount = xml_CreateNode(szXMLattribute,pItem->pxnRoot,3,szXMLid,L"Amount",szXMLvalue,L"1",szXMLtype,L"4");
								if (xml_InsertChildNode(pxnAmount,pItem->pxnRoot,szXMLattribute,szXMLid,L"Slot",TRUE))
									pItem->pxaAmount = xml_GetXMLValueAttr(pxnAmount,szXMLattribute,szXMLid,L"Amount");
								else xml_ReleaseNode(pxnAmount); // Bad
								}
							}
						if (pItem->pxaAmount && pItemContext->iAmount == 1 && App.Config.uGame == 2)
							{
							xml_ReleaseNode(pItem->pxaAmount->parent);
							pItem->pxaAmount = NULL;
							}
						else xml_SetAttrValueNumber(pItem->pxaAmount,pItemContext->iAmount);
						}
					break;
				case GAME_PAGE_BOOSTERS:
					Game_EditBoostersRebuild(pItemContext,pItem->pxaIsGenerated,pItem->pxnGeneration);
					break;
				case GAME_PAGE_BONUSES:
					Game_EditBonusRebuild(pItemContext,pItem->pxnPermanentBoost);
					break;
				case GAME_PAGE_RUNES:
					xml_SetAttrValue(pItem->pxaRunes[0],pItemContext->runes[0].pszId);
					xml_SetAttrValue(pItem->pxaRunes[1],pItemContext->runes[1].pszId);
					xml_SetAttrValue(pItem->pxaRunes[2],pItemContext->runes[2].pszId);
					break;
				case GAME_PAGE_SYNCHRONIZE:
					if (pItemContext->bSetLevel) Game_Synchronize_Level(pItemContext->uNewLevel,FALSE,pItem->pxaLevel,pItem->pxaIsGenerated,pItem->pxnGeneration,&pItem->pxnLevelOverride);
					break;
				}
			}
		SendMessage(App.Game.Layout.hwndInventory,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_ItemsListSort);
		InvalidateRect(App.Game.Layout.hwndInventory,NULL,FALSE);
		}

	//--- Terminé ! ---

Done:	if (pItemContext)
		{
		if (pItemContext->pszDisplayName) HeapFree(App.hHeap,0,pItemContext->pszDisplayName);
		if (pItemContext->pszDescription) HeapFree(App.hHeap,0,pItemContext->pszDescription);
		Locale_Unload(LOCALE_TYPE_MISC,(void **)&pItemContext->pLocale,NULL);
		Game_BonusReleaseAll(&pItemContext->nodeBonuses);
		Game_EditBoostersRelease(&pItemContext->nodeBoosters);
		Game_EditRunesRelease(&pItemContext->runes[0]);
		Game_EditRunesRelease(&pItemContext->runes[1]);
		Game_EditRunesRelease(&pItemContext->runes[2]);
		HeapFree(App.hHeap,0,pItemContext);
		}
	if (psh) HeapFree(App.hHeap,0,psh);
	if (psp)
		{
		for (i = 0; Pages[i].uResID != 0; i++) if (psp[i].lParam) HeapFree(App.hHeap,0,(void *)psp[i].lParam); // That's ok, extra pages lParam is 0
		HeapFree(App.hHeap,0,psp);
		}

	if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Pages								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

BOOL CALLBACK Game_EditProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	PROPSHEETPAGE*	psp;

	if (uMsgId == WM_MEASUREITEM)
		{
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
		int			i;

		ctx = (GAMEEDITPAGECONTEXT *)((PROPSHEETPAGE *)lParam)->lParam;
		Height = Dialog_GetInfoHeight(hDlg,100,ctx->pszInfo);
		GetWindowRect(hDlg,&rcDialog);
		// PropSheet dialogs need more work to be resized...
		//SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);

		switch(ctx->uPageID)
			{
			case GAME_PAGE_NAME:
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				GetWindowRect(GetDlgItem(hDlg,201),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,201),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				break;
			case GAME_PAGE_AMOUNT:
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				break;
			case GAME_PAGE_BOOSTERS:
			case GAME_PAGE_BONUSES:
				Dialog_OffsetY(hDlg,101,Height);
				Dialog_OffsetY(hDlg,200,Height);
				GetWindowRect(GetDlgItem(hDlg,200),&rcDialog);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcDialog,2);
				rcDialog.bottom -= Height;
				SetWindowPos(GetDlgItem(hDlg,101),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				SetWindowPos(GetDlgItem(hDlg,200),NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top,SWP_NOZORDER|SWP_NOMOVE);
				SendDlgItemMessage(hDlg,201,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_ADD));
				SendDlgItemMessage(hDlg,202,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_EDIT));
				SendDlgItemMessage(hDlg,203,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_BUTTON_REMOVE));
				break;
			case GAME_PAGE_RUNES:
				for (i = 0; i != 3; i++)
					{
					Dialog_OffsetY(hDlg,200+i*10,Height);
					Dialog_OffsetY(hDlg,201+i*10,Height);
					Dialog_OffsetY(hDlg,202+i*10,Height);
					Dialog_OffsetY(hDlg,203+i*10,Height);
					Dialog_OffsetY(hDlg,204+i*10,Height);
					Dialog_OffsetY(hDlg,205+i*10,Height);
					Dialog_OffsetY(hDlg,206+i*10,Height);
					}
				break;
			case GAME_PAGE_SYNCHRONIZE:
				Dialog_OffsetY(hDlg,200,Height);
				Dialog_OffsetY(hDlg,201,Height);
				Dialog_OffsetY(hDlg,202,Height);
				Dialog_OffsetY(hDlg,203,Height);
				Dialog_OffsetY(hDlg,204,Height);
				SendDlgItemMessage(hDlg,204,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_DIALOG_SYNCHRO_ENABLE));
				break;
			}

		Game_EditInit(hDlg,ctx);
		SetWindowLongPtr(hDlg,DWLP_USER,(LONG_PTR)lParam);
		Dialog_CenterWindow(GetParent(hDlg),App.hWnd);
		return(TRUE);
		}

	psp = (PROPSHEETPAGE *)GetWindowLongPtr(hDlg,DWLP_USER);
	if (!psp) return(FALSE);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			if (wParam == 100)
				{
				Dialog_DrawInfo(((GAMEEDITPAGECONTEXT *)psp->lParam)->pszInfo,(DRAWITEMSTRUCT *)lParam,BF_RECT);
				return(TRUE);
				}
			switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
				{
				case GAME_PAGE_AMOUNT:
					switch(wParam)
						{
						case 201:
							Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_AMOUNT),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
							return(TRUE);
						}
					break;
				case GAME_PAGE_BOOSTERS:
					switch(wParam)
						{
						case 101:
							Dialog_DrawInfo(Locale_GetText(TEXT_DIALOG_INFO_EMPTYBOOSTERS),(DRAWITEMSTRUCT *)lParam,BF_TOP|BF_BOTTOM);
							return(TRUE);
						case 200:
							Game_EditBoostersDraw((DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				case GAME_PAGE_BONUSES:
					switch(wParam)
						{
						case 101:
							Dialog_DrawInfo(Locale_GetText(TEXT_DIALOG_INFO_EMPTYBONUSES),(DRAWITEMSTRUCT *)lParam,BF_TOP|BF_BOTTOM);
							return(TRUE);
						case 200:
							Game_EditBonusDraw((DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				case GAME_PAGE_RUNES:
					switch(wParam)
						{
						case 200:
						case 210:
						case 220:
							Game_EditRunesDrawName(((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext,(wParam-200)/10,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 204:
						case 205:
						case 206:
						case 214:
						case 215:
						case 216:
						case 224:
						case 225:
						case 226:
							Game_EditRunesDrawBonus(((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext,wParam,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 202:
						case 203:
						case 212:
						case 213:
						case 222:
						case 223:
							if ((wParam-202)%10) Dialog_DrawIconButton(APP_ICON_REMOVE,(DRAWITEMSTRUCT *)lParam);
							else Dialog_DrawTextButton(szEdit,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				case GAME_PAGE_SYNCHRONIZE:
					switch(wParam)
						{
						case 201:
							Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_OBJECTLEVEL),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
							return(TRUE);
						case 203:
							Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_SYNCHROLEVEL),(DRAWITEMSTRUCT *)lParam,DT_RIGHT);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_COMMAND:
			switch(((GAMEEDITPAGECONTEXT *)psp->lParam)->uPageID)
				{
				case GAME_PAGE_BOOSTERS:
					switch(HIWORD(wParam))
						{
						case LBN_SELCHANGE:
							Game_EditBoostersSelected(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
							return(TRUE);
						case LBN_DBLCLK:
							Game_EditBoostersChange(hDlg,FALSE,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
							return(TRUE);
						case BN_CLICKED:
							switch(LOWORD(wParam))
								{
								case 201:
								case 202:
									Game_EditBoostersChange(hDlg,wParam == 201,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								case 203:
									Game_EditBoostersRemove(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								}
							break;
						}
					break;
				case GAME_PAGE_BONUSES:
					switch(HIWORD(wParam))
						{
						case LBN_SELCHANGE:
							Game_EditBonusSelected(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
							return(TRUE);
						case LBN_DBLCLK:
							Game_EditBonusChange(hDlg,FALSE,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
							return(TRUE);
						case BN_CLICKED:
							switch(LOWORD(wParam))
								{
								case 201:
								case 202:
									Game_EditBonusChange(hDlg,wParam == 201,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								case 203:
									Game_EditBonusRemove(hDlg,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								}
							break;
						}
					break;
				case GAME_PAGE_RUNES:
					switch(HIWORD(wParam))
						{
						case BN_CLICKED:
							switch(LOWORD(wParam))
								{
								case 202:
								case 212:
								case 222:
									Game_EditRunesChange(hDlg,(LOWORD(wParam)-202)/10,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								case 203:
								case 213:
								case 223:
									Game_EditRunesRemove(hDlg,(LOWORD(wParam)-203)/10,((GAMEEDITPAGECONTEXT *)psp->lParam)->item.pContext);
									return(TRUE);
								}
							break;
						}
					break;
				case GAME_PAGE_SYNCHRONIZE:
					switch(HIWORD(wParam))
						{
						case BN_CLICKED:
							switch(LOWORD(wParam))
								{
								case 204: {
									BOOL bEnable = (IsDlgButtonChecked(hDlg,204) == BST_CHECKED);
									EnableWindow(((GAMEEDITPAGECONTEXT *)psp->lParam)->item.hwndCtrl[1],bEnable);
									EnableWindow(GetDlgItem(hDlg,202),bEnable);
									} return(TRUE);
								}
							break;
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case PSN_KILLACTIVE:
					//SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_EditApply(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam)?FALSE:TRUE);
					return(TRUE);

				case PSN_APPLY:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_EditApply(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam)?PSNRET_NOERROR:PSNRET_INVALID);
					return(TRUE);

				case PSN_SETACTIVE:
					SetWindowLongPtr(hDlg,DWLP_MSGRESULT,Game_EditActivate(hDlg,(GAMEEDITPAGECONTEXT *)psp->lParam)?0:-1);
					return(TRUE);
				}
			break;
		}

	return(FALSE);
}


// «»»» Initialisation d'une page «««««««««««««««««««««««««««««««««««««««»

BOOL Game_EditInit(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	if (ctx->bPageSet) return(TRUE);

	ctx->bPageSet = TRUE;

	switch(ctx->uPageID)
		{
		case GAME_PAGE_NAME:
			SendDlgItemMessage(hDlg,200,WM_SETTEXT,0,(LPARAM)ctx->item.pContext->pszDisplayName);
			SendDlgItemMessage(hDlg,201,WM_SETTEXT,0,(LPARAM)ctx->item.pContext->pszDescription);
			break;

		case GAME_PAGE_AMOUNT: {
			HWND		hwndCtrl;

			SetDlgItemInt(hDlg,200,ctx->item.pContext->iAmount,FALSE);
			SendDlgItemMessage(hDlg,200,EM_LIMITTEXT,(WPARAM)5,0);
			hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,NULL,App.hInstance,0);
			if (hwndCtrl)
				{
				SendMessage(hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
				SendMessage(hwndCtrl,UDM_SETRANGE32,1,99999);
				SendMessage(hwndCtrl,UDM_SETPOS32,0,ctx->item.pContext->iAmount);
				}
			if (ctx->item.pContext->iAmount < 0)
				{
				EnableWindow(GetDlgItem(hDlg,200),FALSE);
				if (hwndCtrl) EnableWindow(hwndCtrl,FALSE);
				}
			} break;

		case GAME_PAGE_BOOSTERS: {
			GAMEEDITBOOSTER*	pEditBooster;
			LRESULT			lResult;

			for (pEditBooster = (GAMEEDITBOOSTER *)ctx->item.pContext->nodeBoosters.next; pEditBooster != NULL; pEditBooster = (GAMEEDITBOOSTER *)pEditBooster->node.next)
				{
				lResult = SendDlgItemMessage(hDlg,200,LB_ADDSTRING,0,(LPARAM)pEditBooster);
				if (lResult == LB_ERR || lResult == LB_ERRSPACE)
					{
					Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					ctx->bPageSet = FALSE;
					break;
					}
				}

			Game_EditShowList(hDlg,&ctx->item.pContext->nodeBoosters);
			EnableWindow(GetDlgItem(hDlg,200),ctx->item.pContext->bIsGenerated);
			EnableWindow(GetDlgItem(hDlg,201),ctx->item.pContext->bIsGenerated);
			} break;

		case GAME_PAGE_BONUSES: {
			GAMEEDITBONUS*		pEditBonus;
			LRESULT			lResult;

			for (pEditBonus = (GAMEEDITBONUS *)ctx->item.pContext->nodeBonuses.next; pEditBonus != NULL; pEditBonus = (GAMEEDITBONUS *)pEditBonus->node.next)
				{
				lResult = SendDlgItemMessage(hDlg,200,LB_ADDSTRING,0,(LPARAM)pEditBonus);
				if (lResult == LB_ERR || lResult == LB_ERRSPACE)
					{
					Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					ctx->bPageSet = FALSE;
					break;
					}
				}

			Game_EditShowList(hDlg,&ctx->item.pContext->nodeBonuses);
			EnableWindow(GetDlgItem(hDlg,200),ctx->item.pContext->bBonuses);
			EnableWindow(GetDlgItem(hDlg,201),ctx->item.pContext->bBonuses);
			} break;

		case GAME_PAGE_RUNES: {
			int			i;

			for (i = 0; i != 3; i++)
				{
				if (!ctx->item.pContext->bHasRunes) continue;
				EnableWindow(GetDlgItem(hDlg,200+i*10),TRUE);
				EnableWindow(GetDlgItem(hDlg,202+i*10),TRUE);
				if (ctx->item.pContext->runes[i].pszId) EnableWindow(GetDlgItem(hDlg,203+i*10),TRUE);
				}
			} break;

		case GAME_PAGE_SYNCHRONIZE: {
			SetDlgItemInt(hDlg,200,ctx->item.pContext->uLevel,FALSE);
			SetDlgItemInt(hDlg,202,ctx->item.pContext->uNewLevel,FALSE);
			ctx->item.hwndCtrl[0] = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,NULL,App.hInstance,0);
			if (ctx->item.hwndCtrl[0])
				{
				SendMessage(ctx->item.hwndCtrl[0],UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
				SendMessage(ctx->item.hwndCtrl[0],UDM_SETRANGE32,GAME_LEVEL_MIN,GAME_LEVEL_MAX);
				SendMessage(ctx->item.hwndCtrl[0],UDM_SETPOS32,0,ctx->item.pContext->uLevel);
				}
			ctx->item.hwndCtrl[1] = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,NULL,App.hInstance,0);
			if (ctx->item.hwndCtrl[1])
				{
				SendMessage(ctx->item.hwndCtrl[1],UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,202),0);
				SendMessage(ctx->item.hwndCtrl[1],UDM_SETRANGE32,GAME_LEVEL_MIN,GAME_LEVEL_MAX);
				SendMessage(ctx->item.hwndCtrl[1],UDM_SETPOS32,0,ctx->item.pContext->uNewLevel);
				}
			if (!ctx->item.pContext->uLevel)
				{
				EnableWindow(ctx->item.hwndCtrl[1],FALSE);
				EnableWindow(GetDlgItem(hDlg,202),FALSE);
				EnableWindow(GetDlgItem(hDlg,204),FALSE);
				}
			else if (!ctx->item.pContext->bSetLevel)
				{
				EnableWindow(ctx->item.hwndCtrl[1],FALSE);
				EnableWindow(GetDlgItem(hDlg,202),FALSE);
				}
			CheckDlgButton(hDlg,204,ctx->item.pContext->bSetLevel?BST_CHECKED:BST_UNCHECKED);
			} break;
		}

	return(ctx->bPageSet);
}


// «»»» Activation d'une page «««««««««««««««««««««««««««««««««««««««««««»

int Game_EditActivate(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	if (!ctx->bPageSet && !Game_EditInit(hDlg,ctx)) return(0);
	return(1);
}


// «»»» Application des changements «««««««««««««««««««««««««««««««««««««»

int Game_EditApply(HWND hDlg, GAMEEDITPAGECONTEXT *ctx)
{
	switch(ctx->uPageID)
		{
		case GAME_PAGE_NAME:
			if (ctx->bPageSet)
				{
				UINT	uLen;

				if (ctx->item.pContext->pszDisplayName) HeapFree(App.hHeap,0,ctx->item.pContext->pszDisplayName);
				if (ctx->item.pContext->pszDescription) HeapFree(App.hHeap,0,ctx->item.pContext->pszDescription);
				ctx->item.pContext->pszDisplayName = NULL;
				ctx->item.pContext->pszDescription = NULL;
				uLen = SendDlgItemMessage(hDlg,200,WM_GETTEXTLENGTH,0,0);
				if (uLen)
					{
					ctx->item.pContext->pszDisplayName = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
					if (ctx->item.pContext->pszDisplayName) SendDlgItemMessage(hDlg,200,WM_GETTEXT,(WPARAM)uLen,(LPARAM)ctx->item.pContext->pszDisplayName);
					}
				uLen = SendDlgItemMessage(hDlg,201,WM_GETTEXTLENGTH,0,0);
				if (uLen)
					{
					ctx->item.pContext->pszDescription = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
					if (ctx->item.pContext->pszDescription) SendDlgItemMessage(hDlg,201,WM_GETTEXT,(WPARAM)uLen,(LPARAM)ctx->item.pContext->pszDescription);
					}
				}
			break;

		case GAME_PAGE_AMOUNT:
			if (!ctx->bPageSet) break;
			ctx->item.pContext->iAmount = GetDlgItemInt(hDlg,200,NULL,FALSE);
			break;

		case GAME_PAGE_BOOSTERS:
			if (!ctx->bPageSet) break;
			break;

		case GAME_PAGE_BONUSES:
			if (!ctx->bPageSet) break;
			break;

		case GAME_PAGE_RUNES:
			if (!ctx->bPageSet) break;
			break;

		case GAME_PAGE_SYNCHRONIZE:
			if (!ctx->bPageSet) break;
			ctx->item.pContext->uNewLevel = GetDlgItemInt(hDlg,202,NULL,FALSE);
			ctx->item.pContext->bSetLevel = (IsDlgButtonChecked(hDlg,204) == BST_CHECKED);
			break;
		}

	return(1);
}


// «»»» Affichage en cas de liste vide ««««««««««««««««««««««««««««««««««»

void Game_EditShowList(HWND hDlg, NODE *pRoot)
{
	if (!List_EntryCount(pRoot))
		{
		ShowWindow(GetDlgItem(hDlg,200),SW_HIDE);
		ShowWindow(GetDlgItem(hDlg,101),SW_SHOW);
		}
	else
		{
		ShowWindow(GetDlgItem(hDlg,101),SW_HIDE);
		ShowWindow(GetDlgItem(hDlg,200),SW_SHOW);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Nom et Description						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie d'un texte ««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL Game_EditNameCopy(XML_ATTR *pxaName, WCHAR **pszName)
{
	WCHAR*	pszRef;
	UINT	uLen;

	if ((pszRef = xml_GetThisAttrValue(pxaName)) == NULL) return(TRUE);

	uLen = Misc_HtmlSpecialCharsDecode(NULL,pszRef);
	if (uLen)
		{
		*pszName = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
		if (!*pszName) return(FALSE);
		Misc_HtmlSpecialCharsDecode(*pszName,pszRef);
		}

	return(TRUE);
}


// «»»» Application d'un texte ««««««««««««««««««««««««««««««««««««««««««»

BOOL Game_EditNameSet(DOS2ITEM *pItem, XML_ATTR **pxaNamePtr, WCHAR *pszName, WCHAR *pszAttrName)
{
	XML_NODE*	pxnParent;
	XML_NODE*	pxn;
	XML_NODE*	pxnChild;
	WCHAR*		pszCopy;
	UINT		uLen;

	//--- Supprime l'ancien texte ---

	pxn = xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,L"children",NULL,NULL);
	if (pxn)
		{
		pxn = xml_GetNode((XML_NODE *)pxn->children.next,szXMLnode,szXMLid,pszAttrName);
		if (pxn) xml_ReleaseNode(pxn);
		*pxaNamePtr = NULL;
		}

	if (!pszName) return(TRUE);

	//--- Copie le nom ---

	uLen = Misc_HtmlSpecialChars(NULL,pszName);
	if (uLen)
		{
		pszCopy = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
		if (!pszCopy) return(FALSE);
		Misc_HtmlSpecialChars(pszCopy,pszName);
		}
	else return(TRUE);

	//--- Recréation des structures ---

	pxnParent = xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,L"children",NULL,NULL);
	if (pxnParent)
		{
		pxn = xml_CreateNode(szXMLnode,pxnParent,1,szXMLid,pszAttrName);
		if (pxn)
			{
			pxnChild = xml_CreateNode(szXMLattribute,pxn,3,szXMLid,pszAttrName,szXMLvalue,pszCopy,szXMLtype,L"30");
			if (pxnChild)
				{
				List_AddEntry((NODE *)pxnChild,&pxn->children);
				List_InsertEntry((NODE *)pxn,&pxnParent->children,TRUE);
				HeapFree(App.hHeap,0,pszCopy);
				*pxaNamePtr = xml_GetAttr(pxnChild,szXMLvalue);
				return(TRUE);
				}
			else
				{
				xml_ReleaseNode(pxn);
				HeapFree(App.hHeap,0,pszCopy);
				return(FALSE);
				}
			}
		}

	HeapFree(App.hHeap,0,pszCopy);
	return(FALSE);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Augmentations (Boosters)						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélection d'une augmentation ««««««««««««««««««««««««««««««««««««»

void Game_EditBoostersSelected(HWND hDlg, GAMEEDITITEMCONTEXT *pItemContext)
{
	BOOL			bEnable;
	int			iSel;

	iSel = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (iSel == LB_ERR) bEnable = FALSE;
	else bEnable = TRUE;

	EnableWindow(GetDlgItem(hDlg,202),bEnable);
	EnableWindow(GetDlgItem(hDlg,203),bEnable);
	return;
}


// «»»» Affichage des augmentations «««««««««««««««««««««««««««««««««««««»

void Game_EditBoostersDraw(DRAWITEMSTRUCT *pDraw)
{
	GAMEEDITBOOSTER*	pEditBooster;
	HFONT			hFont;
	RECT			rcText;
	COLORREF		crText;
	WCHAR*			pszText;
	int			iBack;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	pEditBooster = (GAMEEDITBOOSTER *)pDraw->itemData;
	if (!pEditBooster) return;
	if (pEditBooster == (GAMEEDITBOOSTER *)-1) return;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	if (pEditBooster->pszName) pszText = pEditBooster->pszName;
	else
		{
		pszText = pEditBooster->pszId;
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_GRAYTEXT));
		}

	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 4;
	rcText.right -= 4;
	DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Copie les augmentations de l'objet ««««««««««««««««««««««««««««««»

int Game_EditBoostersCopy(GAMEEDITITEMCONTEXT *pItemContext, XML_NODE *pxnGeneration)
{
	GAMEEDITBOOSTER*	pEditBooster;
	XML_NODE*		pxnChildren;
	XML_NODE*		pxnBooster;
	XML_NODE*		pxnAttribute;
	XML_ATTR*		pxa;
	UINT			uLastError;

	if (!pxnGeneration) return(1);
	if (!pxnGeneration->children.next) return(1);

	uLastError = 0;
	pxnChildren = xml_GetNode((XML_NODE *)pxnGeneration->children.next,L"children",NULL,NULL);
	if (!pxnChildren) return(1); // N'a aucune augmentation particulière (children/node[id="Booster"]/etc. manquant - Cas possible)

	for (pxnBooster = (XML_NODE *)pxnChildren->children.next; pxnBooster != NULL; pxnBooster = (XML_NODE *)pxnBooster->node.next)
		{
		//--- Vérifie qu'il s'agit bien de <node id="Booster> ---
		pxa = xml_GetAttr(pxnBooster,szXMLid);
		if (!pxa || !pxa->value || wcscmp(szXMLBoost,pxa->value))
			{
			uLastError = TEXT_ERR_GENBOOSTUNKNOWNNODE;
			goto Done;
			}
		//--- Vérifie qu'il y a bien un attribut et un seul ---
		if (!pxnBooster->children.next)
			{
			uLastError = TEXT_ERR_GENBOOSTEMPTY;
			goto Done;
			}
		if (List_EntryCount(pxnBooster->children.next) > 1)
			{
			uLastError = TEXT_ERR_GENBOOSTTOOMANYATTR;
			goto Done;
			}
		//--- Vérifie qu'il s'agit bien de <attribute id="Object" type="22"> ---
		pxnAttribute = (XML_NODE *)pxnBooster->children.next;
		if (wcscmp(szXMLattribute,pxnAttribute->name))
			{
			uLastError = TEXT_ERR_GENBOOSTUNKNOWNATTR;
			goto Done;
			}
		pxa = xml_GetAttr(pxnAttribute,szXMLid);
		if (!pxa || !pxa->value || wcscmp(szXMLObject,pxa->value))
			{
			uLastError = TEXT_ERR_GENBOOSTUNKNOWNATTR;
			goto Done;
			}
		pxa = xml_GetAttr(pxnAttribute,szXMLtype);
		if (!pxa || !pxa->value || wcscmp(L"22",pxa->value))
			{
			uLastError = TEXT_ERR_GENBOOSTUNKNOWNATTR;
			goto Done;
			}
		//--- Copie l'augmentation ---
		pxa = xml_GetAttr(pxnAttribute,szXMLvalue);
		if (!pxa || !pxa->value)
			{
			uLastError = TEXT_ERR_GENBOOSTATTREMPTY;
			goto Done;
			}
		// Copie l'identifiant
		pEditBooster = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITBOOSTER));
		if (!pEditBooster)
			{
			Game_EditBoostersRelease(&pItemContext->nodeBoosters);
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(-1);
			}
		pEditBooster->pszId = HeapAlloc(App.hHeap,0,wcslen(pxa->value)*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pEditBooster->pszId)
			{
			HeapFree(App.hHeap,0,pEditBooster);
			Game_EditBoostersRelease(&pItemContext->nodeBoosters);
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(-1);
			}
		wcscpy(pEditBooster->pszId,pxa->value);
		// Copie le nom
		Locale_QueryID(pItemContext->pLocale->db,szDataBaseBoosters,pEditBooster->pszId,NULL,&pEditBooster->pszName,1);
		// Ajoute à la liste
		List_AddEntry((NODE *)pEditBooster,&pItemContext->nodeBoosters);
		}

Done:	if (uLastError)
		{
		Game_EditBoostersRelease(&pItemContext->nodeBoosters);
		MessageBox(App.hWnd,Locale_GetText(uLastError),NULL,MB_ICONERROR);
		return(0);
		}

	return(1);
}


// «»»» Recréation des augmentations de l'objet «««««««««««««««««««««««««»

int Game_EditBoostersRebuild(GAMEEDITITEMCONTEXT *pItemContext, XML_ATTR *pxaIsGenerated, XML_NODE *pxnGeneration)
{
	GAMEEDITBOOSTER*	pEditBooster;
	XML_NODE*		pxnChildren;
	XML_NODE*		pxnRoot;
	XML_NODE*		pxnNew;
	XML_NODE*		pxnNewChild;

	if (!xml_IsTrue(pxaIsGenerated)) return(1);
	if (!pxnGeneration) return(1);
	if (!pxnGeneration->children.next) return(1);

	pxnRoot = xml_CreateNode(L"children",(XML_NODE *)pxnGeneration->children.next,0);
	if (pxnRoot)
		{
		for (pEditBooster = (GAMEEDITBOOSTER *)pItemContext->nodeBoosters.next; pEditBooster != NULL; pEditBooster = (GAMEEDITBOOSTER *)pEditBooster->node.next)
			{
			pxnNew = xml_CreateNode(L"node",pxnRoot,1,L"id",L"Boost");
			if (!pxnNew)
				{
				xml_ReleaseNode(pxnRoot);
				return(0);
				}
			List_AddEntry((NODE *)pxnNew,&pxnRoot->children);
			pxnNewChild = xml_CreateNode(L"attribute",pxnNew,3,L"id",L"Object",L"value",pEditBooster->pszId,L"type",L"22");
			if (!pxnNewChild)
				{
				xml_ReleaseNode(pxnRoot);
				return(0);
				}
			List_AddEntry((NODE *)pxnNewChild,&pxnNew->children);
			}
		pxnChildren = xml_GetNode((XML_NODE *)pxnGeneration->children.next,L"children",NULL,NULL);
		if (pxnChildren) xml_ReleaseNode(pxnChildren);
		if (List_EntryCount(&pxnRoot->children)) List_Append((NODE *)pxnRoot,&pxnGeneration->children);
		else xml_ReleaseNode(pxnRoot);
		return(1);
		}

	return(0);
}


// «»»» Libère les structures utilisées par les augmentations «««««««««««»

void Game_EditBoostersRelease(NODE *pRoot)
{
	GAMEEDITBOOSTER*	pEditBooster;

	for (pEditBooster = (GAMEEDITBOOSTER *)pRoot->next; pEditBooster != NULL; pEditBooster = (GAMEEDITBOOSTER *)pEditBooster->node.next)
		{
		if (pEditBooster->pszId) HeapFree(App.hHeap,0,pEditBooster->pszId);
		if (pEditBooster->pszName) HeapFree(App.hHeap,0,pEditBooster->pszName);
		}

	List_ReleaseMemory(pRoot);
	return;
}


// «»»» Ajout / Modification d'une augmentation «««««««««««««««««««««««««»

void Game_EditBoostersChange(HWND hDlg, BOOL bAdd, GAMEEDITITEMCONTEXT *pItemContext)
{
	GAMEEDITBOOSTER*	pEditBooster;
	WCHAR*			pszResult;
	WCHAR*			pszBooster;
	UINT			uSelected;

	pEditBooster = NULL;
	pszBooster = NULL;
	uSelected = -1;

	//--- [Modification] Retrouve la sélection ---

	if (!bAdd)
		{
		uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
		if (uSelected == LB_ERR) return;
		pEditBooster = (GAMEEDITBOOSTER *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
		if (pEditBooster == (GAMEEDITBOOSTER *)LB_ERR) return;
		pszBooster = pEditBooster->pszId;
		}

	//--- Sélectionne l'augmentation ---

	pszResult = Game_EditValue(hDlg,pszBooster,DATA_TYPE_BOOSTERS,pItemContext->pItem,&pItemContext->uFilter);
	if (!pszResult) return;

	//--- [Ajout] Création d'une nouvelle augmentation ---

	if (!pEditBooster)
		{
		pEditBooster = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITBOOSTER));
		if (!pEditBooster)
			{
			HeapFree(App.hHeap,0,pszResult);
			return;
			}
		pEditBooster->pszId = pszResult;
		List_AddEntry((NODE *)pEditBooster,&pItemContext->nodeBoosters);
		}

	//--- [Modification] Mise à jour de l'entrée ---

	if (!bAdd)
		{
		SendDlgItemMessage(hDlg,200,LB_SETITEMDATA,(WPARAM)uSelected,(LPARAM)NULL);
		HeapFree(App.hHeap,0,pEditBooster->pszId);
		pEditBooster->pszId = pszResult;
		}

	//--- Mise à jour de la localisation ---

	if (pEditBooster->pszName) HeapFree(App.hHeap,0,pEditBooster->pszName);
	Locale_QueryID(pItemContext->pLocale->db,szDataBaseBoosters,pEditBooster->pszId,NULL,&pEditBooster->pszName,1);

	//--- [Modification] Réinitialise les données de l'entrée ---

	if (!bAdd)
		{
		SendDlgItemMessage(hDlg,200,LB_SETITEMDATA,(WPARAM)uSelected,(LPARAM)pEditBooster);
		InvalidateRect(GetDlgItem(hDlg,200),NULL,FALSE);
		return;
		}

	//--- [Ajout] Ajoute une nouvelle entrée ---

	uSelected = SendDlgItemMessage(hDlg,200,LB_INSERTSTRING,(WPARAM)uSelected,(LPARAM)pEditBooster);
	if (uSelected == LB_ERR || uSelected == LB_ERRSPACE)
		{
		List_RemEntry((NODE *)pEditBooster);
		HeapFree(App.hHeap,0,pEditBooster->pszId);
		if (pEditBooster->pszName) HeapFree(App.hHeap,0,pEditBooster->pszName);
		HeapFree(App.hHeap,0,pEditBooster);
		return;
		}
	Game_EditShowList(hDlg,&pItemContext->nodeBoosters);
	SendDlgItemMessage(hDlg,200,LB_SETCURSEL,(WPARAM)uSelected,0);
	Game_EditBoostersSelected(hDlg,pItemContext);
	return;
}


// «»»» Retrait d'une augmentation ««««««««««««««««««««««««««««««««««««««»

void Game_EditBoostersRemove(HWND hDlg, GAMEEDITITEMCONTEXT *pItemContext)
{
	GAMEEDITBOOSTER*	pEditBooster;
	UINT			uSelected;

	uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (uSelected == LB_ERR) return;
	pEditBooster = (GAMEEDITBOOSTER *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
	if (pEditBooster == (GAMEEDITBOOSTER *)LB_ERR) return;

	SendDlgItemMessage(hDlg,200,LB_DELETESTRING,(WPARAM)uSelected,0);
	List_RemEntry((NODE *)pEditBooster);
	if (pEditBooster->pszId) HeapFree(App.hHeap,0,pEditBooster->pszId);
	if (pEditBooster->pszName) HeapFree(App.hHeap,0,pEditBooster->pszName);
	HeapFree(App.hHeap,0,pEditBooster);
	Game_EditShowList(hDlg,&pItemContext->nodeBoosters);
	Game_EditBoostersSelected(hDlg,pItemContext);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Bonus permanent							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Sélection d'un bonus ««««««««««««««««««««««««««««««««««««««««««««»

void Game_EditBonusSelected(HWND hDlg, GAMEEDITITEMCONTEXT *pItemContext)
{
	BOOL		bEnable;
	int		iSel;

	iSel = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (iSel == LB_ERR) bEnable = FALSE;
	else bEnable = TRUE;

	EnableWindow(GetDlgItem(hDlg,202),bEnable);
	EnableWindow(GetDlgItem(hDlg,203),bEnable);
	return;
}


// «»»» Affichage d'un bonus ««««««««««««««««««««««««««««««««««««««««««««»

void Game_EditBonusDraw(DRAWITEMSTRUCT *pDraw)
{
	GAMEEDITBONUS*	pEditBonus;
	WCHAR*		pszText;
	HFONT		hFont;
	RECT		rcText;
	COLORREF	crText;
	int		iBack;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	pEditBonus = (GAMEEDITBONUS *)pDraw->itemData;
	if (!pEditBonus) return;
	if (pEditBonus == (GAMEEDITBONUS *)-1) return;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 4;
	rcText.right -= 4;
	pszText = Game_BonusGetDisplayText(pEditBonus);
	DrawText(pDraw->hDC,pszText?pszText:pEditBonus->pszId,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	if (pszText) LocalFree(pszText);

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Copie les bonus d'un objet ««««««««««««««««««««««««««««««««««««««»

int Game_EditBonusCopy(GAMEEDITITEMCONTEXT *pItemContext, XML_NODE *pxnPermBonus)
{
	GAMEBONUS*	pBonus;
	LOCALE_MISC*	pLocale;
	XML_NODE*	pxnBonus;
	WCHAR*		pszId;
	WCHAR*		pszValue;
	WCHAR*		pszType;
	UINT		uLastError;
	int		iResult;

	if (!pxnPermBonus) return(1);
	if (!pxnPermBonus->children.next) return(1);

	iResult = 1;
	uLastError = 0;
	pLocale = NULL;

	Locale_Load(App.hWnd,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_MISC,(void **)&pLocale,NULL);

	for (pxnBonus = (XML_NODE *)pxnPermBonus->children.next; pxnBonus != NULL; pxnBonus = (XML_NODE *)pxnBonus->node.next)
		{
		//--- Analyse le noeud enfant pour trouver les compétences (Art de la guerre, etc.) ---

		if (!wcscmp(pxnBonus->name,L"children"))
			{
			XML_NODE *pxn = xml_GetNode((XML_NODE *)pxnBonus->children.next,szXMLnode,szXMLid,L"Abilities");
			if (!pxn) continue;
			iResult = Game_EditBonusCopy(pItemContext,pxn);
			if (iResult == 0 || iResult == -1) return(iResult);
			continue;
			}

		//--- Ignore les balises inconnues ---

		if (wcscmp(pxnBonus->name,szXMLattribute)) continue;

		//--- Récupère les informations de la balise <attribute> ---

		pszId = xml_GetAttrValue(pxnBonus,szXMLid);
		pszValue = xml_GetAttrValue(pxnBonus,szXMLvalue);
		pszType = xml_GetAttrValue(pxnBonus,szXMLtype);
		if (!pszId || !pszValue || !pszType)
			{
			SetLastError(ERROR_INVALID_DATA);
			iResult = -1;
			break;
			}

		//--- Retrouve le bonus associé, ignore la balise si le bonus est inconnu ---

		pBonus = Game_BonusGetFromID(pszId);
		if (!pBonus) continue;

		//--- Sépare les compétences en plusieurs structures ---

		if (pBonus->uFlags&BONUS_FLAG_SKILL)
			{
			WCHAR*	pszNextSkill;
			WCHAR*	pszTempValue;
			UINT	uLen;

			pszNextSkill = pszValue;
			while ((pszNextSkill = wcschr(pszNextSkill,L';')) != NULL)
				{
				uLen = pszNextSkill-pszValue;
				pszTempValue = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
				if (!pszTempValue)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					iResult = -1;
					break;
					}
				CopyMemory(pszTempValue,pszValue,uLen*sizeof(WCHAR));
				pszTempValue[uLen] = 0;

				iResult = Game_EditBonusCopyCreate(pItemContext,pxnPermBonus,pszId,pszTempValue,pBonus,pLocale);
				if (iResult == 0 || iResult == -1) break;

				pszValue = ++pszNextSkill;
				}
			if (iResult == 0 || iResult == -1) break;
			if (!wcslen(pszValue)) continue;
			}

		//--- Création de la structure ---

		iResult = Game_EditBonusCopyCreate(pItemContext,pxnPermBonus,pszId,pszValue,pBonus,pLocale);
		if (iResult == 0 || iResult == -1) break;
		}

	Locale_Unload(LOCALE_TYPE_MISC,(void **)&pLocale,NULL);

	if (uLastError)
		{
		MessageBox(App.hWnd,Locale_GetText(uLastError),NULL,MB_ICONERROR);
		iResult = 0;
		}

	return(iResult);
}

//--- Création de la structure ---

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
int Game_EditBonusCopyCreate(GAMEEDITITEMCONTEXT *pItemContext, XML_NODE *pxnPermBonus, WCHAR *pszId, WCHAR *pszValue, GAMEBONUS *pBonus, LOCALE_MISC *pLocale)
{
	GAMEEDITBONUS*	pEditBonus;
	WCHAR*		pszExtra1;
	WCHAR*		pszExtra2;
	WCHAR*		pszLocale;

	//--- Alloue la structure
	pEditBonus = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITBONUS));
	if (!pEditBonus)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(-1);
		}
	pEditBonus->uLocaleID = pBonus->uLocaleID;
	pEditBonus->uFlags = pBonus->uFlags;
	List_AddEntry((NODE *)pEditBonus,&pItemContext->nodeBonuses);

	//--- Copie l'identifiant
	pEditBonus->pszId = HeapAlloc(App.hHeap,0,wcslen(pszId)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pEditBonus->pszId)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(-1);
		}
	wcscpy(pEditBonus->pszId,pszId);

	//--- Copie les paramètres de reflexion
	if (pBonus->uFlags&BONUS_FLAG_REFLECTION)
		{
		XML_NODE*	pxn;
		WCHAR*		pszAttrValue;

		pszExtra1 = NULL;
		pszExtra2 = NULL;
		pxn = xml_GetNodeFromPath((XML_NODE *)pxnPermBonus->children.next,szItemReflectionMapPath);
		if (pxn)
			{
			pszAttrValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxn->children.next,szXMLattribute,szXMLid,L"Percentage"));
			if (pszAttrValue)
				{
				pszExtra1 = HeapAlloc(App.hHeap,0,wcslen(pszAttrValue)*sizeof(WCHAR)+sizeof(WCHAR));
				if (pszExtra1) wcscpy(pszExtra1,pszAttrValue);
				}
			pszAttrValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxn->children.next,szXMLattribute,szXMLid,L"DamageType"));
			if (pszAttrValue)
				{
				pszExtra2 = HeapAlloc(App.hHeap,0,wcslen(pszAttrValue)*sizeof(WCHAR)+sizeof(WCHAR));
				if (pszExtra2) wcscpy(pszExtra2,pszAttrValue);
				}
			}
		pEditBonus->reflection.bIsSet = wcscmp(pszValue,szXMLTrue)?FALSE:TRUE;
		}
	//--- Copie le nom de la compétence
	else if (pBonus->uFlags&BONUS_FLAG_SKILL)
		{
		pszExtra1 = HeapAlloc(App.hHeap,0,wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pszExtra1)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(-1);
			}
		wcscpy(pszExtra1,pszValue);
		if (pLocale) Locale_QueryID(pLocale->db,szDataBaseSkills,pszExtra1,NULL,&pszLocale,1);
		}
	//--- Copie le nom de la couleur
	else if (pBonus->uFlags&BONUS_FLAG_COLORS)
		{
		pszExtra1 = HeapAlloc(App.hHeap,0,wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pszExtra1)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(-1);
			}
		wcscpy(pszExtra1,pszValue);
		if (pLocale) Locale_QueryID(pLocale->db,szDataBaseColors,pszExtra1,NULL,&pszLocale,1);
		}

	//--- Attribue les paramètres
	if (pBonus->uFlags&BONUS_FLAG_INT) Game_BonusSetVal(pEditBonus,BONUS_FLAG_INT,pszValue);
	else if (pBonus->uFlags&BONUS_FLAG_UINT) Game_BonusSetVal(pEditBonus,BONUS_FLAG_UINT,pszValue);
	else if (pBonus->uFlags&BONUS_FLAG_FLOAT) Game_BonusSetVal(pEditBonus,BONUS_FLAG_FLOAT,pszValue);
	else if (pBonus->uFlags&BONUS_FLAG_DAMAGETYPE) Game_BonusSetVal(pEditBonus,BONUS_FLAG_UINT,pszValue);
	else if (pBonus->uFlags&BONUS_FLAG_REFLECTION) Game_BonusSetVal(pEditBonus,BONUS_FLAG_REFLECTION,pEditBonus->reflection.bIsSet,pszExtra1,pszExtra2);
	else if (pBonus->uFlags&BONUS_FLAG_SKILL) Game_BonusSetVal(pEditBonus,BONUS_FLAG_SKILL,pszExtra1,pszLocale);
	else if (pBonus->uFlags&BONUS_FLAG_COLORS) Game_BonusSetVal(pEditBonus,BONUS_FLAG_COLORS,pszExtra1,pszLocale);
	return(1);
}
#pragma GCC diagnostic pop


// «»»» Recréation des bonus d'un objet «««««««««««««««««««««««««««««««««»

int Game_EditBonusRebuild(GAMEEDITITEMCONTEXT *pItemContext, XML_NODE *pxnPermBonus)
{
	GAMEEDITBONUS*	pBonus;
	GAMEEDITBONUS*	pReflection;
	XML_NODE*	pxnBonus;
	XML_NODE*	pxnParent;
	XML_NODE*	pxnAttributes;
	NODE*		pLastInserted;
	WCHAR*		pszSkills;
	WCHAR*		pszBool;
	BOOL		bIgnoreSkills;
	UINT		uSkillsTotalLen;

	if (!pxnPermBonus) return(1);

	//--- Récupère le pointeur sur les compétences (abilities) ---

	pxnAttributes = xml_GetNode((XML_NODE *)pxnPermBonus->children.next,L"children",NULL,NULL);
	if (!pxnAttributes)
		{
		SetLastError(ERROR_INVALID_DATA);
		return(0);
		}
	pxnAttributes = xml_GetNode((XML_NODE *)pxnAttributes->children.next,szXMLnode,szXMLid,L"Abilities");
	if (!pxnAttributes)
		{
		SetLastError(ERROR_INVALID_DATA);
		return(0);
		}

	//--- Supprime toutes les balises connues ---

	Game_EditBonusRebuildCleanup(pItemContext,pxnPermBonus);

	//--- Ajoute les bonus sélectionnés dans la structure XML ---

	pReflection = NULL;
	pLastInserted = &pxnPermBonus->children;
	bIgnoreSkills = FALSE;

	for (pBonus = (GAMEEDITBONUS *)pItemContext->nodeBonuses.next; pBonus != NULL; pBonus = (GAMEEDITBONUS *)pBonus->node.next)
		{
		if (pBonus->uType == BONUS_FLAG_SKILL && bIgnoreSkills) continue;
		if (pBonus->uType == BONUS_FLAG_REFLECTION)
			{
			pReflection = pBonus;
			continue;
			}

		if (pBonus->uFlags&BONUS_FLAG_ATTRIBUTE) pxnParent = pxnAttributes;
		else pxnParent = pxnPermBonus;
		pxnBonus = NULL;

		switch(pBonus->uType)
			{
			case BONUS_FLAG_INT:
			case BONUS_FLAG_UINT:
				pxnBonus = xml_CreateNode(szXMLattribute,pxnParent,3,szXMLid,pBonus->pszId,szXMLvalue,pBonus->value.szVal,szXMLtype,L"4");
				break;
			case BONUS_FLAG_FLOAT:
				pxnBonus = xml_CreateNode(szXMLattribute,pxnParent,3,szXMLid,pBonus->pszId,szXMLvalue,pBonus->value.szVal,szXMLtype,L"6");
				break;
			case BONUS_FLAG_REFLECTION:
				// Magically reached !
				break;
			case BONUS_FLAG_SKILL:
				uSkillsTotalLen = Game_EditBonusRebuildGetSkills(pItemContext,NULL);
				pszSkills = HeapAlloc(App.hHeap,0,uSkillsTotalLen+sizeof(WCHAR));
				if (!pszSkills)
					{
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					return(0);
					}
				Game_EditBonusRebuildGetSkills(pItemContext,pszSkills);
				pxnBonus = xml_CreateNode(szXMLattribute,pxnParent,3,szXMLid,pBonus->pszId,szXMLvalue,pszSkills,szXMLtype,L"22");
				if (pszSkills) HeapFree(App.hHeap,0,pszSkills);
				bIgnoreSkills = TRUE;
				break;
			case BONUS_FLAG_COLORS:
				pxnBonus = xml_CreateNode(szXMLattribute,pxnParent,3,szXMLid,pBonus->pszId,szXMLvalue,pBonus->colors.pszId,szXMLtype,L"22");
				break;
			}
		if (!pxnBonus)
			{
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return(0);
			}

		if (pBonus->uFlags&BONUS_FLAG_ATTRIBUTE) List_AddEntry((NODE *)pxnBonus,&pxnAttributes->children);
		else
			{
			List_InsertEntry((NODE *)pxnBonus,pLastInserted,TRUE);
			pLastInserted = (NODE *)pxnBonus;
			}
		}

	//--- Ajout de la reflexion ---

	if (pReflection) pszBool = pReflection->reflection.bIsSet?szXMLTrue:szXMLFalse;
	else pszBool = szXMLFalse;
	pxnBonus = xml_CreateNode(szXMLattribute,pxnPermBonus,3,szXMLid,L"HasReflection",szXMLvalue,pszBool,szXMLtype,L"19");
	if (!pxnBonus) return(0);
	List_InsertEntry((NODE *)pxnBonus,pLastInserted,TRUE);

	if (pReflection)
		{
		if (pReflection->reflection.bIsSet)
			{
			WCHAR*	pArray[] = {	szXMLnode, szXMLid, L"Reflection", (WCHAR *)1,
							szXMLattribute, szXMLid, L"DamageType", szXMLvalue, L"0", szXMLtype, L"1", (WCHAR *)0,
							szXMLattribute, szXMLid, L"MeleeOnly", szXMLvalue, L"True", szXMLtype, L"19", (WCHAR *)0,
							L"children", (WCHAR *)1,
								szXMLnode, szXMLid, L"MapValue", (WCHAR *)1,
									szXMLattribute, szXMLid, L"Percentage", szXMLvalue, pReflection->reflection.szPercentage, szXMLtype, L"4", (WCHAR *)0,
									szXMLattribute, szXMLid, L"DamageType", szXMLvalue, pReflection->reflection.szDamageType, szXMLtype, L"1", (WCHAR *)0,
									(WCHAR *)-2
						};
			WCHAR**	pArrayPtr = pArray;

			pxnBonus = xml_CreateNodeArray(&pArrayPtr,(XML_NODE *)pxnAttributes->parent);
			if (pxnBonus) List_InsertEntry((NODE *)pxnBonus,(NODE *)pxnAttributes,TRUE);
			}
		}

	return(1);
}

//--- Retrouve toutes les compétences ---

//!\ Returns the length (in bytes) of the buffer NOT including the null terminating character

UINT Game_EditBonusRebuildGetSkills(GAMEEDITITEMCONTEXT *pItemContext, WCHAR *pszBuffer)
{
	GAMEEDITBONUS*	pBonus;
	UINT		uTotalLen;

	if (pszBuffer) *pszBuffer = 0;

	for (uTotalLen = 0, pBonus = (GAMEEDITBONUS *)pItemContext->nodeBonuses.next; pBonus != NULL; pBonus = (GAMEEDITBONUS *)pBonus->node.next)
		{
		if (pBonus->uType != BONUS_FLAG_SKILL) continue;
		if (uTotalLen)
			{
			if (pszBuffer) wcscat(pszBuffer,L";");
			uTotalLen += sizeof(WCHAR);
			}
		uTotalLen += wcslen(pBonus->skill.pszId)*sizeof(WCHAR);
		if (pszBuffer) wcscat(pszBuffer,pBonus->skill.pszId);
		}

	return(uTotalLen);
}


//--- Supprime toutes les balises connues ---

void Game_EditBonusRebuildCleanup(GAMEEDITITEMCONTEXT *pItemContext, XML_NODE *pxnPermBonus)
{
	XML_NODE*	pxnBonus;
	XML_NODE*	pxnNextBonus;

	if (!pxnPermBonus) return;
	if (!pxnPermBonus->children.next) return;

	for (pxnBonus = (XML_NODE *)pxnPermBonus->children.next; pxnBonus != NULL; pxnBonus = pxnNextBonus)
		{
		pxnNextBonus = (XML_NODE *)pxnBonus->node.next;
		// Remove known <attribute>'s in children/node[id=Abilities]
		if (!wcscmp(pxnBonus->name,L"children"))
			{
			XML_NODE *pxn = xml_GetNode((XML_NODE *)pxnBonus->children.next,szXMLnode,szXMLid,L"Abilities");
			if (!pxn) continue;
			Game_EditBonusRebuildCleanup(pItemContext,pxn);
			continue;
			}

		// Remove known <attribute>
		if (!wcscmp(pxnBonus->name,szXMLattribute))
			{
			if (!Game_BonusGetFromID(xml_GetAttrValue(pxnBonus,szXMLid))) continue;
			xml_ReleaseNode(pxnBonus);
			continue;
			}
		}

	// Remove <node id="Reflection"> in <children>
	pxnBonus = xml_GetNodeFromPath((XML_NODE *)pxnPermBonus->children.next,szItemReflectionPath);
	if (pxnBonus) xml_ReleaseNode(pxnBonus);

	return;
}


// «»»» Ajout / Modification d'un bonus «««««««««««««««««««««««««««««««««»

void Game_EditBonusChange(HWND hDlg, BOOL bAdd, GAMEEDITITEMCONTEXT *pItemContext)
{
	GAMEEDITBONUS*	pEditBonus;
	GAMEEDITBONUS*	pEditBonusNew;
	UINT		uSelected;

	pEditBonus = NULL;
	uSelected = -1;

	//--- [Modification] Retrouve la sélection
	if (!bAdd)
		{
		uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
		if (uSelected == LB_ERR) return;
		pEditBonus = (GAMEEDITBONUS *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
		if (pEditBonus == (GAMEEDITBONUS *)LB_ERR) return;
		}

	//--- Sélectionne le nouveau bonus ---

	pEditBonusNew = Game_Bonus(hDlg,pEditBonus,pItemContext->pItem);
	if (!pEditBonusNew) return;

	//--- [Ajout] Ajoute le nouveau bonus dans la liste ---

	if (!pEditBonus) List_AddEntry((NODE *)pEditBonusNew,&pItemContext->nodeBonuses);

	//--- [Modification] Insère le nouveau bonus dans la liste ---

	if (!bAdd)
		{
		SendDlgItemMessage(hDlg,200,LB_SETITEMDATA,(WPARAM)uSelected,(LPARAM)NULL);
		List_InsertEntry((NODE *)pEditBonusNew,(NODE *)pEditBonus,FALSE);
		}

	//--- [Modification] Supprime l'ancien bonus et met à jour la liste ---

	if (!bAdd)
		{
		List_RemEntry((NODE *)pEditBonus);
		Game_BonusRelease(pEditBonus,FALSE);
		SendDlgItemMessage(hDlg,200,LB_SETITEMDATA,(WPARAM)uSelected,(LPARAM)pEditBonusNew);
		InvalidateRect(GetDlgItem(hDlg,200),NULL,FALSE);
		return;
		}

	//--- [Ajout] Ajoute une nouvelle entrée dans la liste ---

	uSelected = SendDlgItemMessage(hDlg,200,LB_INSERTSTRING,(WPARAM)uSelected,(LPARAM)pEditBonusNew);
	if (uSelected == LB_ERR || uSelected == LB_ERRSPACE)
		{
		List_RemEntry((NODE *)pEditBonusNew);
		Game_BonusRelease(pEditBonusNew,FALSE);
		return;
		}
	Game_EditShowList(hDlg,&pItemContext->nodeBonuses);
	SendDlgItemMessage(hDlg,200,LB_SETCURSEL,(WPARAM)uSelected,0);
	Game_EditBonusSelected(hDlg,pItemContext);
	return;
}


// «»»» Retrait d'un bonus ««««««««««««««««««««««««««««««««««««««««««««««»

void Game_EditBonusRemove(HWND hDlg, GAMEEDITITEMCONTEXT *pItemContext)
{
	GAMEEDITBONUS*	pEditBonus;
	UINT		uSelected;

	uSelected = SendDlgItemMessage(hDlg,200,LB_GETCURSEL,0,0);
	if (uSelected == LB_ERR) return;
	pEditBonus = (GAMEEDITBONUS *)SendDlgItemMessage(hDlg,200,LB_GETITEMDATA,(WPARAM)uSelected,0);
	if (pEditBonus == (GAMEEDITBONUS *)LB_ERR) return;

	SendDlgItemMessage(hDlg,200,LB_DELETESTRING,(WPARAM)uSelected,0);
	List_RemEntry((NODE *)pEditBonus);
	Game_BonusRelease(pEditBonus,FALSE);
	Game_EditShowList(hDlg,&pItemContext->nodeBonuses);
	Game_EditBonusSelected(hDlg,pItemContext);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Runes								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie les runes «««««««««««««««««««««««««««««««««««««««««««««««««»

int Game_EditRunesCopy(DOS2ITEM *dosItem, GAMEEDITITEMCONTEXT *pItemContext)
{
	WCHAR*	pszText;
	int	i;

	for (i = 0; i != 3; i++)
		{
		pszText = xml_GetThisAttrValue(dosItem->pxaRunes[i]);
		if (!pszText) continue;
		pItemContext->runes[i].pszId = HeapAlloc(App.hHeap,0,wcslen(pszText)*sizeof(WCHAR)+sizeof(WCHAR));
		if (!pItemContext->runes[i].pszId) return(0);
		wcscpy(pItemContext->runes[i].pszId,pszText);
		Locale_QueryID(pItemContext->pLocale->db,szDataBaseRunes,pItemContext->runes[i].pszId,NULL,&pItemContext->runes[i].pszName,1);
		}

	return(1);
}


// «»»» Libère les données utilisées par une rune «««««««««««««««««««««««»

void Game_EditRunesRelease(GAMEEDITRUNE *pEditRune)
{
	int	i;

	if (pEditRune->pszId) HeapFree(App.hHeap,0,pEditRune->pszId);
	if (pEditRune->pszName) HeapFree(App.hHeap,0,pEditRune->pszName);

	pEditRune->pszId = NULL;
	pEditRune->pszName = NULL;

	for (i = 0; i != 3; i++)
		{
		if (pEditRune->pszBonuses[i]) HeapFree(App.hHeap,0,pEditRune->pszBonuses[i]);
		pEditRune->pszBonuses[i] = NULL;
		}

	return;
}


// «»»» Affichage du nom d'une rune «««««««««««««««««««««««««««««««««««««»

void Game_EditRunesDrawName(GAMEEDITITEMCONTEXT *pItemContext, UINT uRuneId, DRAWITEMSTRUCT *pDraw)
{
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	if (pItemContext->runes[uRuneId].pszId)
		{
		WCHAR*		pszText;
		RECT		rcDraw;
		HFONT		hFont;
		COLORREF	crColor;
		int		iBkMode;

		CopyRect(&rcDraw,&pDraw->rcItem);
		Game_PaintIcon(pDraw->hDC,pItemContext->runes[uRuneId].pszId,APP_ICON_EMPTY,&rcDraw,GAME_ICON_SIZE,FALSE,FALSE);
		CopyRect(&rcDraw,&pDraw->rcItem);
		rcDraw.left += GAME_ICON_SIZE;
		DrawEdge(pDraw->hDC,&rcDraw,BDR_SUNKENOUTER,BF_LEFT);
		rcDraw.left += 4;
		rcDraw.right -= 4;
		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crColor = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		pszText = pItemContext->runes[uRuneId].pszName?pItemContext->runes[uRuneId].pszName:pItemContext->runes[uRuneId].pszId;
		DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crColor);
		SelectObject(pDraw->hDC,hFont);
		}

	DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKEN,BF_RECT);
	return;
}


// «»»» Affichage des bonus d'une rune ««««««««««««««««««««««««««««««««««»

void Game_EditRunesDrawBonus(GAMEEDITITEMCONTEXT *pItemContext, UINT uCtrlId, DRAWITEMSTRUCT *pDraw)
{
	UINT		uRuneId;
	UINT		uBonusId;
	HFONT		hFont;
	COLORREF	crColor;
	RECT		rcArea;
	int		iBkMode;
	static UINT	uIcons[3] = { APP_ICON_WEAPON, APP_ICON_ARMOR, APP_ICON_ACCESSORY };

	if (uCtrlId < 204) return;
	uRuneId = (uCtrlId-204)/10;
	uBonusId = (uCtrlId-204)%10;
	if (uRuneId > 2) return;
	if (uBonusId > 2) return;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	if (!pItemContext->runes[uRuneId].pszId) return;

	if (!pItemContext->runes[uRuneId].pszBonuses[0] || !pItemContext->runes[uRuneId].pszBonuses[1] || !pItemContext->runes[uRuneId].pszBonuses[2])
		Locale_QueryBonuses(pItemContext->pLocale->db,pItemContext->runes[uRuneId].pszId,pItemContext->runes[uRuneId].pszBonuses);

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crColor = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);

	if (pItemContext->runes[uRuneId].pszBonuses[uBonusId])
		{
		UINT	uType;

		CopyRect(&rcArea,&pDraw->rcItem);
		rcArea.left += 4;
		rcArea.right -= 4;
		DrawIconEx(pDraw->hDC,rcArea.left,rcArea.top+(rcArea.bottom-rcArea.top-16)/2,App.hIcons[uIcons[uBonusId]],15,16,0,NULL,DI_NORMAL);
		rcArea.left += 20;
		DrawText(pDraw->hDC,pItemContext->runes[uRuneId].pszBonuses[uBonusId],-1,&rcArea,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		uType = Game_GetItemType(pItemContext->pszStats);
		if ((uType == DATA_TYPE_IS_WEAPON && uBonusId != 0) || (uType == DATA_TYPE_IS_ARMOR && uBonusId != 1) || (uType == DATA_TYPE_IS_ACCESSORY && uBonusId != 2))
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
					FillRect(hDC,&rcRect,GetSysColorBrush(COLOR_BTNFACE));
					blend.BlendOp = AC_SRC_OVER;
					blend.BlendFlags = 0;
					blend.SourceConstantAlpha = 200;
					blend.AlphaFormat = 0;
					AlphaBlend(pDraw->hDC,pDraw->rcItem.left,pDraw->rcItem.top,pDraw->rcItem.right-pDraw->rcItem.left,pDraw->rcItem.bottom-pDraw->rcItem.top,hDC,0,0,8,8,blend);
					SelectObject(hDC,hDefBitmap);
					DeleteObject(hBitmap);
					}
				DeleteDC(hDC);
				}
			}
		}

	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crColor);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Modification d'une rune «««««««««««««««««««««««««««««««««««««««««»

void Game_EditRunesChange(HWND hDlg, UINT uIndex, GAMEEDITITEMCONTEXT *pItemContext)
{
	WCHAR*	pszResult;

	pszResult = Game_EditValue(hDlg,pItemContext->runes[uIndex].pszId,DATA_TYPE_RUNES,pItemContext->pItem);
	if (!pszResult) return;

	Game_EditRunesRelease(&pItemContext->runes[uIndex]);
	pItemContext->runes[uIndex].pszId = pszResult;
	Locale_QueryID(pItemContext->pLocale->db,szDataBaseRunes,pItemContext->runes[uIndex].pszId,NULL,&pItemContext->runes[uIndex].pszName,1);

	InvalidateRect(GetDlgItem(hDlg,200+uIndex*10),NULL,FALSE);
	EnableWindow(GetDlgItem(hDlg,203+uIndex*10),TRUE);
	InvalidateRect(GetDlgItem(hDlg,204+uIndex*10),NULL,FALSE);
	InvalidateRect(GetDlgItem(hDlg,205+uIndex*10),NULL,FALSE);
	InvalidateRect(GetDlgItem(hDlg,206+uIndex*10),NULL,FALSE);
	return;
}


// «»»» Retrait d'une rune ««««««««««««««««««««««««««««««««««««««««««««««»

void Game_EditRunesRemove(HWND hDlg, UINT uIndex, GAMEEDITITEMCONTEXT *pItemContext)
{
	Game_EditRunesRelease(&pItemContext->runes[uIndex]);
	InvalidateRect(GetDlgItem(hDlg,200+uIndex*10),NULL,FALSE);
	EnableWindow(GetDlgItem(hDlg,203+uIndex*10),FALSE);
	InvalidateRect(GetDlgItem(hDlg,204+uIndex*10),NULL,FALSE);
	InvalidateRect(GetDlgItem(hDlg,205+uIndex*10),NULL,FALSE);
	InvalidateRect(GetDlgItem(hDlg,206+uIndex*10),NULL,FALSE);
	return;
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration - Modification
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Locale.h"
#include "GameLocale.h"
#include "Texts.h"
#include "Menus.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Game.h"

extern APPLICATION		App;

static UINT			cfgCatPaths[] = { 100, 101, 102, 103, 104, 105, 106, 111, 112, 113, 114, 115, 116, 199, 0 };
static UINT			cfgCatLangs[] = { 200, 201, 202, 211, 212, 0 };
static UINT			cfgCatEdits[] = { 300, 301, 302, 310, 311, 312, 320, 321, 322, 330, 331, 332, 0 };
static UINT			cfgCatWinds[] = { 400, 401, 402, 410, 411, 412, 0 };
static UINT			cfgCatTView[] = { 500, 501, 502, 503, 0 };
static UINT			cfgCatDispl[] = { 600, 601, 602, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 0 };

static UINT*			ConfigCategories[] = { cfgCatPaths, cfgCatLangs, cfgCatEdits, cfgCatWinds, cfgCatTView, cfgCatDispl, NULL };
static UINT			ConfigCategoriesNames[] = { TEXT_CONFIG_PATHS, TEXT_CONFIG_LANG, TEXT_CONFIG_EDITION, TEXT_CONFIG_WINDOWS, TEXT_TITLE_TREE, TEXT_CONFIG_DISPLAY, 0 };


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue					  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Modification des paramètres «««««««««««««««««««««««««««««««««««««»

void Config_Modify()
{
	CONFIGCONTEXT*	pContext;
	INT_PTR		iResult;
	DWORD		dwRedraw;

	//--- Initialisations ---

	pContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(CONFIGCONTEXT));
	if (!pContext)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),Locale_GetText(IDM_CONFIGMODIFY),MB_ICONERROR);
		goto Done;
		}

	pContext->bIsLimited = (App.Game.Save.pszSaveName)?TRUE:FALSE;

	pContext->pConfig = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(CONFIG));
	if (!pContext->pConfig)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),Locale_GetText(IDM_CONFIGMODIFY),MB_ICONERROR);
		goto Done;
		}

	if (!Locale_Enum(App.hWnd,szLangPath,&pContext->nRoot,LOCALE_TYPE_APPLICATION))
		goto Done;
	if (!Locale_Enum(App.hWnd,szLSLPath,&pContext->nRootLS,LOCALE_TYPE_GAME))
		goto Done;

	//--- Copie la configuration ---

	CopyMemory(pContext->pConfig,&App.Config,sizeof(CONFIG));

	pContext->pConfig->pszLocaleName = Misc_StrCpyAlloc(App.Config.pszLocaleName);
	pContext->pConfig->pszLocaleNameLS = Misc_StrCpyAlloc(App.Config.pszLocaleNameLS);
	pContext->pConfig->pszProfile = Misc_StrCpyAlloc(App.Config.pszProfile); // Unused but needed when saving config
	pContext->pConfig->pszLarianPath = Misc_StrCpyAlloc(App.Config.pszLarianPath);
	pContext->pConfig->pszTempPath = Misc_StrCpyAlloc(App.Config.pszTempPath);

	// NOTE: No error check here because NULL ptr is a possibility.

	//--- Boîte de dialogue ---

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1101),App.hWnd,Config_ModifyProc,(LPARAM)pContext);
	if (!iResult || iResult == -1)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		goto Done;
		}
	if (iResult != IDOK) goto Done;

	//--- Application des changements ---

	dwRedraw = 0;
	App.Config.bSaveOnExit = pContext->pConfig->bSaveOnExit;
	Menu_SetFlag(App.hMenu,IDM_CONFIGSAVEONEXIT,App.Config.bSaveOnExit);

	// Paths
	Config_ModifyMovePath(&App.Config.pszLarianPath,&pContext->pConfig->pszLarianPath);
	Config_ModifyMovePath(&App.Config.pszTempPath,&pContext->pConfig->pszTempPath);

	// Edition
	App.Config.bShowHiddenTags = pContext->pConfig->bShowHiddenTags;
	App.Config.bRunesGroups = pContext->pConfig->bRunesGroups;
	App.Config.uRunesView = pContext->pConfig->uRunesView;
	App.Config.bSkillsGroups = pContext->pConfig->bSkillsGroups;
	App.Config.uSkillsView = pContext->pConfig->uSkillsView;
	App.Config.bBoostersGroups = pContext->pConfig->bBoostersGroups;
	App.Config.bCapOverride = pContext->pConfig->bCapOverride;

	// Windows
	App.Config.windowMain.usedefault.bCoords = pContext->pConfig->windowMain.usedefault.bCoords;
	App.Config.windowMain.usedefault.bSize = pContext->pConfig->windowMain.usedefault.bSize;
	App.Config.windowTreeView.usedefault.bCoords = pContext->pConfig->windowTreeView.usedefault.bCoords;
	App.Config.windowTreeView.usedefault.bSize = pContext->pConfig->windowTreeView.usedefault.bSize;

	// TreeView
	App.Config.bTVSearchOpacity = pContext->pConfig->bTVSearchOpacity;
	App.Config.bTVSearchHistory = pContext->pConfig->bTVSearchHistory;

	if (App.xmlTree.search.hDlg)
		{
		CheckDlgButton(App.xmlTree.search.hDlg,400,App.Config.bTVSearchOpacity?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(App.xmlTree.search.hDlg,402,App.Config.bTVSearchHistory?BST_CHECKED:BST_UNCHECKED);
		}

	// Locale
	if (wcscmp(App.Config.pszLocaleName,pContext->pConfig->pszLocaleName)) Config_SetLanguage(App.hWnd,pContext->pConfig->pszLocaleName);

	if (wcscmp(App.Config.pszLocaleNameLS,pContext->pConfig->pszLocaleNameLS))
		{
		Locale_Unload(LOCALE_TYPE_GAME,(void **)&App.pLanguage,&App.Config.pszLocaleNameLS);
		Locale_Load(App.hWnd,szLSLPath,pContext->pConfig->pszLocaleNameLS,LOCALE_TYPE_GAME,(void **)&App.pLanguage,NULL);
		App.Config.pszLocaleNameLS = pContext->pConfig->pszLocaleNameLS;
		pContext->pConfig->pszLocaleNameLS = NULL;
		Game_ReleaseDisplayNames();
		dwRedraw |= CONFIG_REDRAW_WINDOW;
		dwRedraw |= CONFIG_REDRAW_ITEMS;
		}

	// Display
	if (App.Config.uListDisplayMode != pContext->pConfig->uListDisplayMode) dwRedraw |= CONFIG_REDRAW_ITEMS;
	if (App.Config.crListStats != pContext->pConfig->crListStats) dwRedraw |= CONFIG_REDRAW_ITEMS;
	if (App.Config.lListTopMargin != pContext->pConfig->lListTopMargin) dwRedraw |= CONFIG_REDRAW_ITEMS;
	if (App.Config.lListSpacing != pContext->pConfig->lListSpacing) dwRedraw |= CONFIG_REDRAW_ITEMS;
	App.Config.uListDisplayMode = pContext->pConfig->uListDisplayMode;
	App.Config.crListStats = pContext->pConfig->crListStats;
	App.Config.lListTopMargin = pContext->pConfig->lListTopMargin;
	App.Config.lListSpacing = pContext->pConfig->lListSpacing;

	if (App.Config.bItemsDisplayName != pContext->pConfig->bItemsDisplayName || App.Config.bItemsResolve != pContext->pConfig->bItemsResolve)
		{
		App.Config.bItemsDisplayName = pContext->pConfig->bItemsDisplayName;
		App.Config.bItemsResolve = pContext->pConfig->bItemsResolve;
		Game_ReleaseDisplayNames();
		dwRedraw |= CONFIG_REDRAW_ITEMS;
		}

	//--- Mise à jour des affichages ---

	if (dwRedraw&CONFIG_REDRAW_ITEMS)
		{
		SendMessage(App.Game.Layout.hwndInventory,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_ItemsListSort);
		InvalidateRect(App.Game.Layout.hwndInventory,NULL,FALSE);
		InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
		}
	if (dwRedraw&CONFIG_REDRAW_WINDOW)
		{
		InvalidateRect(App.hWnd,NULL,FALSE);
		}

	//--- Terminé ---

Done:	if (pContext)
		{
		if (pContext->pConfig)
			{
			if (pContext->pszTempPath) HeapFree(App.hHeap,0,pContext->pszTempPath);
			if (pContext->pszLarianPath) HeapFree(App.hHeap,0,pContext->pszLarianPath);
			Config_Release(pContext->pConfig);
			Locale_EnumRelease(&pContext->nRoot);
			Locale_EnumRelease(&pContext->nRootLS);
			HeapFree(App.hHeap,0,pContext->pConfig);
			}
		HeapFree(App.hHeap,0,pContext);
		}

	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Config_ModifyProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	CONFIGCONTEXT*	pContext;

	if (uMsgId == WM_INITDIALOG)
		{
		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pContext = (CONFIGCONTEXT *)lParam;

		//--- Liste des langues
		if (!Config_SelectLanguageInit(hDlg,201,&pContext->nRoot,TRUE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		if (!Config_SelectLanguageInit(hDlg,202,&pContext->nRootLS,TRUE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		//--- Catégories
		if (!Config_ModifyCategories(hDlg,CONFIG_CATEGORY_INIT,pContext))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		//--- Répertoires par défaut
		Config_DefaultTempLocation(&pContext->pszTempPath,TRUE);
		Config_DefaultSaveLocation(&pContext->pszLarianPath,TRUE);

		//--- Fichiers temporaires
		SetDlgItemText(hDlg,102,pContext->pConfig->pszTempPath);
		SetDlgItemText(hDlg,103,Locale_GetText(TEXT_CONFIG_MODIFY));
		SetDlgItemText(hDlg,104,Locale_GetText(TEXT_CONFIG_DEFAULT));
		SetDlgItemText(hDlg,105,pContext->pszTempPath);

		//--- Sauvegardes
		SetDlgItemText(hDlg,112,pContext->pConfig->pszLarianPath);
		SetDlgItemText(hDlg,113,Locale_GetText(TEXT_CONFIG_MODIFY));
		SetDlgItemText(hDlg,114,Locale_GetText(TEXT_CONFIG_DEFAULT));
		SetDlgItemText(hDlg,115,pContext->pszLarianPath);

		//--- Taille des zones d'édition
		SendDlgItemMessage(hDlg,102,EM_LIMITTEXT,(WPARAM)MAX_PATH,0);
		SendDlgItemMessage(hDlg,105,EM_LIMITTEXT,(WPARAM)MAX_PATH,0);
		SendDlgItemMessage(hDlg,112,EM_LIMITTEXT,(WPARAM)MAX_PATH,0);
		SendDlgItemMessage(hDlg,115,EM_LIMITTEXT,(WPARAM)MAX_PATH,0);

		//--- Textes
		SetDlgItemText(hDlg,301,Locale_GetText(TEXT_CONFIG_OVERRIDE));
		SetDlgItemText(hDlg,302,Locale_GetText(TEXT_DIALOG_TAG_SHOWHIDDEN));
		SetDlgItemText(hDlg,310,Locale_GetText(TEXT_VIEW_GROUPS));
		SetDlgItemText(hDlg,320,Locale_GetText(TEXT_VIEW_GROUPS));
		SetDlgItemText(hDlg,330,Locale_GetText(TEXT_VIEW_GROUPS));
		SetDlgItemText(hDlg,401,Locale_GetText(TEXT_CONFIG_WINDOWS_USEDEFAULTPOS));
		SetDlgItemText(hDlg,402,Locale_GetText(TEXT_CONFIG_WINDOWS_USEDEFAULTSIZE));
		SetDlgItemText(hDlg,411,Locale_GetText(TEXT_CONFIG_WINDOWS_USEDEFAULTPOS));
		SetDlgItemText(hDlg,412,Locale_GetText(TEXT_CONFIG_WINDOWS_USEDEFAULTSIZE));
		SetDlgItemText(hDlg,501,Locale_GetText(TEXT_DIALOG_TV_SEARCH_OPACITY));
		SetDlgItemText(hDlg,502,Locale_GetText(TEXT_DIALOG_TV_SEARCH_HISTORY));
		SetDlgItemText(hDlg,503,Locale_GetText(TEXT_DIALOG_TV_SEARCH_HISTORYCLEAR));
		SetDlgItemText(hDlg,601,Locale_GetText(TEXT_CONFIG_ITEMSDISPLAYNAME));
		SetDlgItemText(hDlg,602,Locale_GetText(TEXT_CONFIG_ITEMSRESOLVE));
		SetDlgItemText(hDlg,777,Locale_GetText(TEXT_SAVE));
		SetDlgItemText(hDlg,778,Locale_GetText(IDM_CONFIGSAVEONEXIT));

		//--- Drapeaux
		CheckDlgButton(hDlg,301,pContext->pConfig->bCapOverride?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,302,pContext->pConfig->bShowHiddenTags?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,310,pContext->pConfig->bRunesGroups?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,320,pContext->pConfig->bSkillsGroups?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,330,pContext->pConfig->bBoostersGroups?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,401,pContext->pConfig->windowMain.usedefault.bCoords?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,402,pContext->pConfig->windowMain.usedefault.bSize?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,411,pContext->pConfig->windowTreeView.usedefault.bCoords?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,412,pContext->pConfig->windowTreeView.usedefault.bSize?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,501,pContext->pConfig->bTVSearchOpacity?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,502,pContext->pConfig->bTVSearchHistory?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,601,pContext->pConfig->bItemsDisplayName?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,602,pContext->pConfig->bItemsResolve?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,778,pContext->pConfig->bSaveOnExit?BST_CHECKED:BST_UNCHECKED);

		//--- Affichage
		if (!Dialog_ViewComboInit(hDlg,311,pContext->pConfig->uRunesView,3,LV_VIEW_DETAILS,LV_VIEW_ICON,LV_VIEW_TILE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		if (!Dialog_ViewComboInit(hDlg,321,pContext->pConfig->uSkillsView,2,LV_VIEW_ICON,LV_VIEW_TILE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		if (!Dialog_ViewComboInit(hDlg,331,1,1,LV_VIEW_DETAILS))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		SendDlgItemMessage(hDlg,611,CB_ADDSTRING,0,(LPARAM)Locale_GetText(TEXT_CONFIG_LDISPLAY_NAMEONLY));
		SendDlgItemMessage(hDlg,611,CB_ADDSTRING,0,(LPARAM)Locale_GetText(TEXT_CONFIG_LDISPLAY_STATSONLY));
		SendDlgItemMessage(hDlg,611,CB_ADDSTRING,0,(LPARAM)Locale_GetText(TEXT_CONFIG_LDISPLAY_ALL));
		SendDlgItemMessage(hDlg,611,CB_SETCURSEL,(WPARAM)pContext->pConfig->uListDisplayMode,0);
		SendDlgItemMessage(hDlg,616,UDM_SETRANGE32,0,99);
		SendDlgItemMessage(hDlg,616,UDM_SETPOS32,0,pContext->pConfig->lListTopMargin);
		SendDlgItemMessage(hDlg,619,UDM_SETRANGE32,0,99);
		SendDlgItemMessage(hDlg,619,UDM_SETPOS32,0,pContext->pConfig->lListSpacing);

		//--- Désactive les répertoires si une sauvegarde est en cours
		ShowWindow(GetDlgItem(hDlg,199),pContext->bIsLimited?SW_SHOW:SW_HIDE);
		if (pContext->bIsLimited)
			{
			EnableWindow(GetDlgItem(hDlg,102),FALSE);
			EnableWindow(GetDlgItem(hDlg,103),FALSE);
			EnableWindow(GetDlgItem(hDlg,104),FALSE);
			EnableWindow(GetDlgItem(hDlg,105),FALSE);

			EnableWindow(GetDlgItem(hDlg,112),FALSE);
			EnableWindow(GetDlgItem(hDlg,113),FALSE);
			EnableWindow(GetDlgItem(hDlg,114),FALSE);
			EnableWindow(GetDlgItem(hDlg,115),FALSE);
			}

		//--- Désactive le bouton de réinitialisation si aucun historique n'existe
		EnableWindow(GetDlgItem(hDlg,503),PathFileExists(szTVSearchHistoryPath));

		//--- Dernières initialisations
		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
		SetWindowText(hDlg,Locale_GetText(TEXT_CONFIG_TITLE));
		Dialog_CenterWindow(hDlg,App.hWnd);
		SetFocus(GetDlgItem(hDlg,999));
		return(FALSE);
		}

	pContext = (CONFIGCONTEXT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			switch(wParam)
				{
				case 999:
				case 201:
				case 202:
					((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
					((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
					if (((MEASUREITEMSTRUCT *)lParam)->itemHeight < 24) ((MEASUREITEMSTRUCT *)lParam)->itemHeight = 24;
					return(TRUE);
				}
			break;

		case WM_CTLCOLORSTATIC:
			if ((HWND)lParam == GetDlgItem(hDlg,778)) return((INT_PTR)GetSysColorBrush(COLOR_WINDOW));
			return((INT_PTR)GetSysColorBrush(COLOR_BTNFACE));

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 999:
					Config_ModifyCategoriesDraw((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 100:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_PATHS),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 101:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_TEMP),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 106:
				case 116:
					Config_ModifyDrawArrow(hDlg,wParam-2,(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 111:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_SAVEGAMES),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 199:
					Config_ModifyDrawWarning(Locale_GetText(TEXT_CONFIG_OPENEDSAVEGAME),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 200:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_LANG),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 201:
				case 202:
					Config_SelectLanguageDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 211:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_LANG_INTERFACE),(DRAWITEMSTRUCT *)lParam,NULL,DT_LEFT|DT_VCENTER);
					return(TRUE);
				case 212:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_LANG_GAME),(DRAWITEMSTRUCT *)lParam,NULL,DT_LEFT|DT_VCENTER);
					return(TRUE);
				case 300:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_EDITION),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 311:
				case 321:
				case 331:
					Dialog_ViewComboDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 312:
					Config_ModifyDrawLabel((DRAWITEMSTRUCT *)lParam,TEXT_CONFIG_RUNES);
					return(TRUE);
				case 322:
					Config_ModifyDrawLabel((DRAWITEMSTRUCT *)lParam,TEXT_CONFIG_SKILLS);
					return(TRUE);
				case 332:
					Config_ModifyDrawLabel((DRAWITEMSTRUCT *)lParam,TEXT_CONFIG_BOOSTERS);
					return(TRUE);
				case 400:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_WINDOWS_MAIN),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 410:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_WINDOWS_TREE),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 500:
					Dialog_DrawTitle(Locale_GetText(TEXT_TITLE_TREE),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 600:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_DISPLAY),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 610:
					Dialog_DrawTitle(Locale_GetText(TEXT_CONFIG_INVENTORY),(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 612:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_LDISPLAY),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 613:
					Dialog_DrawColorButton(Locale_GetText(TEXT_CONFIG_LSTATSCOLOR),pContext->pConfig->crListStats,(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 614:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_LTOPMARGIN),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 617:
					Dialog_DrawLabel(Locale_GetText(TEXT_CONFIG_LSPACING),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				}
			break;

		case WM_PAINT:
			if (GetUpdateRect(hDlg,NULL,FALSE))
				{
				HDC		hDC;
				PAINTSTRUCT	paintStruct;
				RECT		rcWindow;
				RECT		rcEdge;

				hDC = BeginPaint(hDlg,&paintStruct);
				GetClientRect(hDlg,&rcWindow);
				GetWindowRect(GetDlgItem(hDlg,IDOK),&rcEdge);
				MapWindowPoints(NULL,hDlg,(POINT *)&rcEdge,2);
				rcEdge.top -= 20;
				rcEdge.left = rcWindow.left;
				rcEdge.right = rcWindow.right;
				rcEdge.bottom = rcWindow.bottom;
				FillRect(hDC,&rcEdge,GetSysColorBrush(COLOR_WINDOW));
				DrawEdge(hDC,&rcEdge,BDR_SUNKENOUTER,BF_TOP);
				EndPaint(hDlg,&paintStruct);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 999:
							Config_ModifyCategories(hDlg,CONFIG_CATEGORY_HIDE,pContext);
							Config_ModifyCategories(hDlg,CONFIG_CATEGORY_SHOW,pContext);
							break;
						}
					break;
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 103:
							Config_ModifySelectLocation(hDlg,102,Locale_GetText(TEXT_CONFIG_TEMP));
							return(TRUE);
						case 104:
							Config_ModifyCopyDlgText(hDlg,102,105);
							return(TRUE);
						case 113:
							Config_ModifySelectLocation(hDlg,112,Locale_GetText(TEXT_CONFIG_SAVEGAMES));
							return(TRUE);
						case 114:
							Config_ModifyCopyDlgText(hDlg,112,115);
							return(TRUE);
						case 503:
							Tree_SearchHistoryClear(App.xmlTree.search.hDlg);
							EnableWindow(GetDlgItem(hDlg,503),PathFileExists(szTVSearchHistoryPath));
							return(TRUE);
						case 613:
							Config_ModifyPickColor(hDlg,613,&pContext->pConfig->crListStats,NULL);
							return(TRUE);
						case 777:
							if (!Config_ModifyApplyChanges(hDlg,pContext)) return(TRUE);
							if (!Config_Save(FALSE,pContext->pConfig)) return(TRUE);
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDOK:
							if (!Config_ModifyApplyChanges(hDlg,pContext)) return(TRUE);
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


// «»»» Affichage du message d'avertissement ««««««««««««««««««««««««««««»

void Config_ModifyDrawWarning(WCHAR *pszText, DRAWITEMSTRUCT *pDraw)
{
	HTHEME	hTheme;
	RECT	rcText;

	hTheme = OpenThemeData(App.hWnd,L"TOOLTIP");
	if (hTheme)
		{
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 52;
		DrawThemeBackground(hTheme,pDraw->hDC,TTP_STANDARD,0,&pDraw->rcItem,NULL);
		DrawIconEx(pDraw->hDC,pDraw->rcItem.left+12,pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-32)/2,App.hShellIcons[APP_SHELLICON_WARNING],32,32,0,NULL,DI_NORMAL);
		DrawThemeText(hTheme,pDraw->hDC,TTP_STANDARD,TTSS_NORMAL,pszText,-1,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER,0,&rcText);
		CloseThemeData(hTheme);
		}
	else
		{
		COLORREF	crColor;
		int		iBkMode;

		FillRect(pDraw->hDC,&pDraw->rcItem,GetStockObject(WHITE_BRUSH));
		DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKENOUTER,BF_RECT|BF_FLAT);
		DrawIconEx(pDraw->hDC,pDraw->rcItem.left+12,pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-32)/2,App.hShellIcons[APP_SHELLICON_WARNING],32,32,0,NULL,DI_NORMAL);

		crColor = SetTextColor(pDraw->hDC,RGB(0,0,0));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 52;
		DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetTextColor(pDraw->hDC,crColor);
		SetBkMode(pDraw->hDC,iBkMode);
		}

	return;
}


// «»»» Affichage de la flèche "par défaut" «««««««««««««««««««««««««««««»

void Config_ModifyDrawArrow(HWND hDlg, UINT uCtlId, DRAWITEMSTRUCT *pDraw)
{
	HPEN	hPen,hDefPen;
	HBRUSH	hBrush,hDefBrush;
	RECT	rcButton;
	POINT	ptPoly[3];
	int	X,Y;

	hPen = CreatePen(PS_SOLID,1,RGB(70,70,70));
	if (hPen)
		{
		hBrush = CreateSolidBrush(RGB(240,240,240));
		if (hBrush)
			{
			hDefPen = SelectObject(pDraw->hDC,hPen);
			hDefBrush = SelectObject(pDraw->hDC,hBrush);

			GetWindowRect(GetDlgItem(hDlg,uCtlId),&rcButton);
			X = (rcButton.right-rcButton.left)/2;
			Y = (pDraw->rcItem.bottom-pDraw->rcItem.top)/2;

			MoveToEx(pDraw->hDC,pDraw->rcItem.left,Y,NULL);
			LineTo(pDraw->hDC,pDraw->rcItem.right-X,Y);
			LineTo(pDraw->hDC,pDraw->rcItem.right-X,Y-4);

			ptPoly[0].x = pDraw->rcItem.right-X-4;
			ptPoly[0].y = Y-4;
			ptPoly[1].x = pDraw->rcItem.right-X+4;
			ptPoly[1].y = Y-4;
			ptPoly[2].x = pDraw->rcItem.right-X;
			ptPoly[2].y = pDraw->rcItem.top;
			Polygon(pDraw->hDC,ptPoly,3);

			SelectObject(pDraw->hDC,hDefBrush);
			SelectObject(pDraw->hDC,hDefPen);
			DeleteObject(hBrush);
			}
		DeleteObject(hPen);
		}

	return;
}


// «»»» Affichage des étiquettes «««««««««««««««««««««««««««««««««««««««»

void Config_ModifyDrawLabel(DRAWITEMSTRUCT *pDraw, UINT uLocaleId)
{
	RECT		rcDraw;
	COLORREF	crColor;
	int		iBkMode;

	CopyRect(&rcDraw,&pDraw->rcItem);
	rcDraw.left = rcDraw.right-2;
	FillRect(pDraw->hDC,&rcDraw,GetSysColorBrush(COLOR_WINDOWTEXT));

	CopyRect(&rcDraw,&pDraw->rcItem);
	rcDraw.top += 4;
	rcDraw.right -= 8;
	crColor = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	DrawText(pDraw->hDC,Locale_GetText(uLocaleId),-1,&rcDraw,DT_NOPREFIX|DT_RIGHT|DT_TOP);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crColor);

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Catégories						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Gestion des catégories ««««««««««««««««««««««««««««««««««««««««««»

int Config_ModifyCategories(HWND hDlg, UINT uMode, CONFIGCONTEXT *pContext)
{
	int	i,j;

	switch(uMode)
		{
		case CONFIG_CATEGORY_INIT:
			for (i = 0; ConfigCategoriesNames[i] != 0; i++)
				{
				j = SendDlgItemMessage(hDlg,999,LB_ADDSTRING,0,(LPARAM)ConfigCategoriesNames[i]);
				if (j == LB_ERR) return(0);
				if (j == LB_ERRSPACE) return(0);
				}
			for (i = 0; ConfigCategories[i] != NULL; i++)
				for (j = 0; ConfigCategories[i][j] != 0; j++)
					ShowWindow(GetDlgItem(hDlg,ConfigCategories[i][j]),FALSE);

			if (SendDlgItemMessage(hDlg,999,LB_SETCURSEL,0,0) == LB_ERR) return(0);
			Config_ModifyCategories(hDlg,CONFIG_CATEGORY_SHOW,pContext);
			return(1);

		case CONFIG_CATEGORY_HIDE:
			i = pContext->uCategory;
			if (i >= sizeof(ConfigCategories)/sizeof(UINT *)-1) break;
			for (j = 0; ConfigCategories[i][j] != 0; j++)
				ShowWindow(GetDlgItem(hDlg,ConfigCategories[i][j]),FALSE);
			break;

		case CONFIG_CATEGORY_SHOW:
			i = pContext->uCategory = SendDlgItemMessage(hDlg,999,LB_GETCURSEL,0,0);
			if (i == LB_ERR) break;
			if (i >= sizeof(ConfigCategories)/sizeof(UINT *)-1) break;
			for (j = 0; ConfigCategories[i][j] != 0; j++)
				ShowWindow(GetDlgItem(hDlg,ConfigCategories[i][j]),TRUE);
			if (i == 0) ShowWindow(GetDlgItem(hDlg,199),pContext->bIsLimited?SW_SHOW:SW_HIDE);
			break;
		}

	return(0);
}


// «»»» Affichage des catégories ««««««««««««««««««««««««««««««««««««««««»

void Config_ModifyCategoriesDraw(DRAWITEMSTRUCT *pDraw)
{
	RECT		rcText;
	HFONT		hFont;
	COLORREF	crText;
	int		iBkMode;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:(pDraw->itemState&ODS_DISABLED?COLOR_GRAYTEXT:COLOR_WINDOWTEXT)));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 2;
	rcText.right -= 2;
	DrawText(pDraw->hDC,Locale_GetText(pDraw->itemData),-1,&rcText,DT_RIGHT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	SetBkMode(pDraw->hDC,iBkMode);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Application des changements «««««««««««««««««««««««««««««««««««««»

int Config_ModifyApplyChanges(HWND hDlg, CONFIGCONTEXT *pContext)
{
	LOCALE_TEXT*	pLocale;
	WCHAR*		pszLocaleName;

	if (!Config_ModifyGetLanguage(hDlg,201,LOCALE_TYPE_APPLICATION,pContext)) goto Error;
	Config_ModifyGetLanguage(hDlg,202,LOCALE_TYPE_GAME,pContext);

	//--- Ensure the language db can be loaded
	if (!Locale_Load(hDlg,szLangPath,pContext->pConfig->pszLocaleName,LOCALE_TYPE_APPLICATION,(void **)&pLocale,&pszLocaleName)) goto Error;
	Locale_Unload(LOCALE_TYPE_APPLICATION,(void **)&pLocale,&pszLocaleName);

	//--- Apply flags
	pContext->pConfig->bSaveOnExit = IsDlgButtonChecked(hDlg,778) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bCapOverride = IsDlgButtonChecked(hDlg,301) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bShowHiddenTags = IsDlgButtonChecked(hDlg,302) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bRunesGroups = IsDlgButtonChecked(hDlg,310) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bSkillsGroups = IsDlgButtonChecked(hDlg,320) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bBoostersGroups = IsDlgButtonChecked(hDlg,330) == BST_CHECKED?TRUE:FALSE;

	//--- Apply windows settings
	pContext->pConfig->windowMain.usedefault.bCoords = IsDlgButtonChecked(hDlg,401) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->windowMain.usedefault.bSize = IsDlgButtonChecked(hDlg,402) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->windowTreeView.usedefault.bCoords = IsDlgButtonChecked(hDlg,411) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->windowTreeView.usedefault.bSize = IsDlgButtonChecked(hDlg,412) == BST_CHECKED?TRUE:FALSE;

	//--- Apply treeview settings
	pContext->pConfig->bTVSearchOpacity = IsDlgButtonChecked(hDlg,501) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bTVSearchHistory = IsDlgButtonChecked(hDlg,502) == BST_CHECKED?TRUE:FALSE;

	//--- Apply display settings
	pContext->pConfig->bItemsDisplayName = IsDlgButtonChecked(hDlg,601) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->bItemsResolve = IsDlgButtonChecked(hDlg,602) == BST_CHECKED?TRUE:FALSE;
	pContext->pConfig->uListDisplayMode = SendDlgItemMessage(hDlg,611,CB_GETCURSEL,0,0);
	pContext->pConfig->lListTopMargin = GetDlgItemInt(hDlg,615,NULL,FALSE);
	pContext->pConfig->lListSpacing = GetDlgItemInt(hDlg,618,NULL,FALSE);

	//--- Apply view modes
	if (!Dialog_ViewComboChanged(hDlg,311,0,&pContext->pConfig->uRunesView)) goto Error;
	if (!Dialog_ViewComboChanged(hDlg,321,0,&pContext->pConfig->uSkillsView)) goto Error;

	//--- Apply paths
	if (!Config_ModifyGetPath(hDlg,102,&pContext->pConfig->pszTempPath)) goto Error;
	if (!Config_ModifyGetPath(hDlg,112,&pContext->pConfig->pszLarianPath)) goto Error;

	//--- Copy customcolors for save
	CopyMemory(&pContext->pConfig->crCustColors,&App.Config.crCustColors,sizeof(COLORREF)*16);
	return(1);

Error:	Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_CONFIGCHANGE),NULL,MB_ICONERROR);
	return(0);
}


// «»»» Copie la chaîne d'une boîte d'édition dans une autre ««««««««««««»

void Config_ModifyCopyDlgText(HWND hDlg, UINT uDestId, UINT uSrcId)
{
	WCHAR*	pszText;
	UINT	uLen;

	uLen = SendDlgItemMessage(hDlg,uSrcId,WM_GETTEXTLENGTH,0,0);
	if (!uLen)
		{
		SetDlgItemText(hDlg,uDestId,NULL);
		return;
		}

	pszText = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
	if (pszText)
		{
		if (GetDlgItemText(hDlg,uSrcId,pszText,uLen)) SetDlgItemText(hDlg,uDestId,pszText);
		HeapFree(App.hHeap,0,pszText);
		}

	return;
}


// «»»» Modification de la langue «««««««««««««««««««««««««««««««««««««««»

int Config_ModifyGetLanguage(HWND hDlg, UINT uCtlId, UINT uType, CONFIGCONTEXT *pContext)
{
	LOCALE_ENUM*	pEnum;
	WCHAR**		pszLocaleName;
	int		iSelected;

	iSelected = SendDlgItemMessage(hDlg,uCtlId,LB_GETCURSEL,0,0);
	if (iSelected == LB_ERR) return(0);

	pEnum = (LOCALE_ENUM *)SendDlgItemMessage(hDlg,uCtlId,LB_GETITEMDATA,(WPARAM)iSelected,0);
	if (pEnum == (LOCALE_ENUM *)LB_ERR) return(0);

	switch (uType)
		{
		case LOCALE_TYPE_GAME:
			pszLocaleName = &pContext->pConfig->pszLocaleNameLS;
			break;
		default:pszLocaleName = &pContext->pConfig->pszLocaleName;
			break;
		}

	// Language not changed?
	if (*pszLocaleName)
		{
		if (!wcscmp(pEnum->szLang,*pszLocaleName)) return(1);
		HeapFree(App.hHeap,0,*pszLocaleName);
		}

	*pszLocaleName = Misc_StrCpyAlloc(pEnum->szLang);
	if (!*pszLocaleName)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(0);
		}

	return(1);
}


// «»»» Obtention d'une chaîne de caractères ««««««««««««««««««««««««««««»

int Config_ModifyGetPath(HWND hDlg, UINT uCtlId, WCHAR **pszPath)
{
	WCHAR*	pszNewPath;
	UINT	uLen;

	uLen = SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXTLENGTH,0,0);
	if (!uLen)
		{
		SetLastError(ERROR_INVALID_DATA);
		return(0);
		}

	pszNewPath = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
	if (!pszNewPath)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return(0);
		}

	GetDlgItemText(hDlg,uCtlId,pszNewPath,uLen);

	if (!PathFileExists(pszNewPath))
		{
		HeapFree(App.hHeap,0,pszNewPath);
		return(0);
		}

	if (*pszPath) HeapFree(App.hHeap,0,*pszPath);
	*pszPath = pszNewPath;
	return(1);
}


// «»»» Remplacement d'une chaîne de caractères «««««««««««««««««««««««««»

void Config_ModifyMovePath(WCHAR **pDest, WCHAR **pSrc)
{
	if (*pDest) HeapFree(App.hHeap,0,*pDest);
	*pDest = *pSrc;
	*pSrc = NULL;
	return;
}


// «»»» Sélection d'un répertoire «««««««««««««««««««««««««««««««««««««««»

void Config_ModifySelectLocation(HWND hDlg, UINT uCtlId, WCHAR *pszTitle)
{
	WCHAR*			pszPath;
	IFileDialog*		pFileDialog;
	IShellItem*		pItem;
	HRESULT			hr;
	int			iLen;

	//--- Copie le répertoire par défaut ---

	iLen = SendDlgItemMessage(hDlg,uCtlId,WM_GETTEXTLENGTH,0,0);
	pszPath = HeapAlloc(App.hHeap,0,++iLen*sizeof(WCHAR));
	if (pszPath) GetDlgItemText(hDlg,uCtlId,pszPath,iLen);

	//--- Sélectionne le répertoire ---

	hr = CoCreateInstance(&CLSID_FileOpenDialog,NULL,CLSCTX_INPROC,&IID_IFileDialog,(void *)&pFileDialog);
	if (SUCCEEDED(hr))
		{
		pFileDialog->lpVtbl->SetTitle(pFileDialog,pszTitle);
		pFileDialog->lpVtbl->SetOptions(pFileDialog,FOS_PICKFOLDERS|FOS_FORCEFILESYSTEM);
		if (pszPath)
			{
			hr = SHCreateItemFromParsingName(pszPath,NULL,&IID_IShellItem,(void *)&pItem);
			if (SUCCEEDED(hr))
				{
				pFileDialog->lpVtbl->SetFolder(pFileDialog,pItem);
				pItem->lpVtbl->Release(pItem);
				}
			}

		hr = pFileDialog->lpVtbl->Show(pFileDialog,hDlg);
		if (SUCCEEDED(hr))
			{
			WCHAR*		pszFilePath;

			hr = pFileDialog->lpVtbl->GetResult(pFileDialog,&pItem);
			if (SUCCEEDED(hr))
				{
				hr = pItem->lpVtbl->GetDisplayName(pItem,SIGDN_FILESYSPATH,&pszFilePath);
				if (SUCCEEDED(hr))
					{
					SetDlgItemText(hDlg,uCtlId,pszFilePath);
					CoTaskMemFree(pszFilePath);
					}
				pItem->lpVtbl->Release(pItem);
				}
			}
		pFileDialog->lpVtbl->Release(pFileDialog);
		}

	if (pszPath) HeapFree(App.hHeap,0,pszPath);
	return;
}


// «»»» Sélection d'une couleur «««««««««««««««««««««««««««««««««««««««««»

void Config_ModifyPickColor(HWND hDlg, UINT uCtlId, COLORREF *pcrColor, LPCCHOOKPROC pfnHook)
{
	CHOOSECOLOR	ccColor;

	ZeroMemory(&ccColor,sizeof(CHOOSECOLOR));
	App.Config.crCustColors[0] = CONFIG_LDS_COLOR;
	App.Config.crCustColors[1] = *pcrColor;
	ccColor.lStructSize = sizeof(CHOOSECOLOR);
	ccColor.hwndOwner = hDlg;
	ccColor.rgbResult = *pcrColor;
	ccColor.Flags = CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
	ccColor.lpCustColors = App.Config.crCustColors;
	ccColor.lCustData = 0;
	ccColor.lpfnHook = pfnHook;
	if (pfnHook) ccColor.Flags |= CC_ENABLEHOOK;
	if (!ChooseColor(&ccColor)) return;

	*pcrColor = ccColor.rgbResult;
	InvalidateRect(GetDlgItem(hDlg,uCtlId),NULL,FALSE);
	return;
}

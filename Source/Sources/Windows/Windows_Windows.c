
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des messages des fenêtres
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données							  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "About.h"
#include "Texts.h"
#include "Menus.h"
#include "Mods.h"
#include "Files.h"
#include "Infos.h"
#include "LastFiles.h"
#include "Requests.h"
#include "Divine.h"
#include "GameEdit.h"
#include "GameLocale.h"
#include "Utils.h"
#include "Dialogs.h"
#include "Taskbar.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	MainMenu;
extern UINT			TextsAttr[];
extern UINT			TextsPts[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Processus principal						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LRESULT CALLBACK Window_Proc(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	LONG_PTR	lptrWindowID;

	if (uMsgId == WM_CREATE)
		{
		SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)(((CREATESTRUCT *)lParam)->lpCreateParams));
		switch((LONG_PTR)(((CREATESTRUCT *)lParam)->lpCreateParams))
			{
			case WINDOW_MAIN:
				return(Window_Create(hWnd));
			case WINDOW_TREE:
				return(Tree_Create(hWnd));
			case WINDOW_GAME_CONTAINER:
				return(Game_ContainerCreate(hWnd));
			case WINDOW_GAME_ATTRIBUTES:
				return(Game_AttributesCreate(hWnd));
			case WINDOW_TREE_DEBUG:
				return(Tree_DebugCreate(hWnd));
			}
		return(-1);
		}

	lptrWindowID = GetWindowLongPtr(hWnd,GWLP_USERDATA);
	switch(lptrWindowID)
		{
		case WINDOW_MAIN:
			return(Window_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_TREE:
			return(Tree_ProcessMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_GAME_CONTAINER:
			return(Game_ProcessContainerMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_GAME_ATTRIBUTES:
			return(Game_ProcessAttributesMessages(hWnd,uMsgId,wParam,lParam));
		case WINDOW_TREE_DEBUG:
			return(Tree_DebugMessages(hWnd,uMsgId,wParam,lParam));
		}

	return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
}


// «»»» Lecture des messages provenant de la fenêtre principale «««««««««»

LRESULT Window_ProcessMessages(HWND hWnd, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	switch(uMsgId)
		{
		#if _DEBUG
		case WM_KEYDOWN:
			if (wParam != VK_F1) break;
			//Game_Tags();
			//Game_Talents();
			//Game_Bonus(hWnd,NULL,NULL);
			//Game_EditValue(hWnd,NULL,DATA_TYPE_BOOSTERS,NULL,NULL);
			//Mods_Dialog();
			//Game_EditXP(hWnd,NULL);
			//Game_Stats(hWnd);
			//Game_Skills(hWnd);
			//Game_SynchronizeAll();
			//Tree_Open(NULL);
			break;
		#endif

		case WM_PAINT:
			if (GetUpdateRect(hWnd,NULL,FALSE))
				{
				HDC		hDC;
				PAINTSTRUCT	paintStruct;
				RECT		rcWindow;
				HRGN		hrgnWindow;

				hDC = BeginPaint(hWnd,&paintStruct);
				GetClientRect(hWnd,&rcWindow);

				if (App.hwndStatus)
					{
					RECT	rcStatus;
					RECT	rcTemp;

					CopyRect(&rcTemp,&rcWindow);
					GetWindowRect(App.hwndStatus,&rcStatus);
					MapWindowPoints(NULL,hWnd,(POINT *)&rcStatus,2);
					SubtractRect(&rcWindow,&rcTemp,&rcStatus);
					}

				hrgnWindow = CreateRectRgnIndirect(&rcWindow);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndContainer,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndAbilitiesBtn,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndTagsBtn,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndTalentsBtn,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndSkillsBtn,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndInfosBtn,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndInventory,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndInventoryName,hWnd);
				Window_CombineRegions(hrgnWindow,App.Game.Layout.hwndMenuBtn,hWnd);
				if (hrgnWindow) SelectClipRgn(hDC,hrgnWindow);
				FillRect(hDC,&rcWindow,GetSysColorBrush(COLOR_BTNFACE));
				if (hrgnWindow)
					{
					SelectClipRgn(hDC,NULL);
					DeleteObject(hrgnWindow);
					}

				Game_Paint(hWnd,hDC,&rcWindow);
				DrawEdge(hDC,&rcWindow,EDGE_SUNKEN,BF_BOTTOM);
				EndPaint(hWnd,&paintStruct);
				}
			break;

		case WM_MEASUREITEM:
			Window_MeasureItems(hWnd,(UINT)wParam,(MEASUREITEMSTRUCT *)lParam);
			break;

		case WM_DRAWITEM:
			Window_DrawItems((DRAWITEMSTRUCT *)lParam);
			break;

		case WM_MOVE:
			Window_Moved(hWnd);
			break;

		case WM_SIZING:
			if (((RECT *)lParam)->right-((RECT *)lParam)->left < MAIN_WINDOW_MINWIDTH) ((RECT *)lParam)->right = ((RECT *)lParam)->left+MAIN_WINDOW_MINWIDTH;
			if (((RECT *)lParam)->bottom-((RECT *)lParam)->top < MAIN_WINDOW_MINHEIGHT) ((RECT *)lParam)->bottom = ((RECT *)lParam)->top+MAIN_WINDOW_MINHEIGHT;
			if (((RECT *)lParam)->right-((RECT *)lParam)->left > MAIN_WINDOW_MAXWIDTH) ((RECT *)lParam)->right = ((RECT *)lParam)->left+MAIN_WINDOW_MAXWIDTH;
			if (((RECT *)lParam)->bottom-((RECT *)lParam)->top > MAIN_WINDOW_MAXHEIGHT) ((RECT *)lParam)->bottom = ((RECT *)lParam)->top+MAIN_WINDOW_MAXHEIGHT;
			break;

		case WM_SIZE:
			Window_Resized(hWnd);
			break;

		case WM_NOTIFY:
			Window_Notify(hWnd,(UINT)wParam,(NMHDR *)lParam);
			break;

		case WM_COMMAND:
			Window_Command(hWnd,(UINT)HIWORD(wParam),(UINT)LOWORD(wParam),(HWND)lParam);
			break;

		case WM_MESSAGEBOX:
			MessageBox(hWnd,(WCHAR *)lParam,NULL,wParam);
			LocalFree((WCHAR *)lParam);
			break;

		case WM_UPDATEPROGRESS:
			Status_UpdateProgress(wParam,lParam);
			break;

		case WM_THEMECHANGED:
			CloseThemeData(App.hThemeButton);
			CloseThemeData(App.hThemeProgress);
			App.hThemeButton = OpenThemeData(hWnd,szThemeButton);
			App.hThemeProgress = OpenThemeData(hWnd,szThemeProgress);
			break;

		case WM_CLOSE:
			if (App.hThread)
				{
				MessageBox(hWnd,Locale_GetText(TEXT_THREAD_RUNNING),Locale_GetText(TEXT_TITLE_INFO),MB_ICONINFORMATION|MB_OK);
				break;
				}
			if (App.Config.bSaveOnExit && !Config_Save(TRUE,&App.Config)) break;
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			Divine_Close();
			Misc_SetWindowText(App.hWnd,&App.pszWindowTitle,szTitle,NULL);
			Status_SetText(NULL);
			ShowWindow(hWnd,SW_HIDE);
			SetMenu(hWnd,NULL);
			Menu_Release(App.hMenu,&MainMenu);
			Font_Destroy(szGameFont,App.GameFont.hFont);
			Taskbar_Reset();
			CloseThemeData(App.hThemeButton);
			CloseThemeData(App.hThemeProgress);
			App.hMenu = NULL;
			PostQuitMessage(0);
			break;

		default:
			if (App.WM_TASKBARBUTTONCREATED && uMsgId == App.WM_TASKBARBUTTONCREATED) Taskbar_Initialize();
			return(DefWindowProc(hWnd,uMsgId,wParam,lParam));
		}

	return(0);
}

//--- Combine deux régions (antiflicker) ---

void Window_CombineRegions(HRGN hrgnDest, HWND hwndFrom, HWND hwndTo)
{
	HRGN	hrgnWindow;
	RECT	rcWindow;

	if (!hrgnDest) return;
	if (!hwndFrom) return;

	GetWindowRect(hwndFrom,&rcWindow);
	MapWindowPoints(NULL,hwndTo,(POINT *)&rcWindow,2);

	hrgnWindow = CreateRectRgnIndirect(&rcWindow);
	if (hrgnWindow)
		{
		CombineRgn(hrgnDest,hrgnDest,hrgnWindow,RGN_DIFF);
		DeleteObject(hrgnWindow);
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_CREATE -- Création d'une nouvelle fenêtre			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

LRESULT Window_Create(HWND hWnd)
{
	App.Font.hFont = GetStockObject(DEFAULT_GUI_FONT);
	App.hWnd = hWnd;
	App.hThemeButton = OpenThemeData(hWnd,szThemeButton);
	App.hThemeProgress = OpenThemeData(hWnd,szThemeProgress);

	if (!App.Font.hFont) goto Error_0;
	if (!Font_GetInfo(hWnd,&App.Font)) goto Error_0;
	if (!(App.GameFont.hFont = Font_Create(hWnd,szGameFont,szGameFontName,10))) goto Error_0;
	if (!Font_GetInfo(hWnd,&App.GameFont)) goto Error_0;
	if (!Taskbar_Prepare()) goto Error_0;
	if (!Status_CreateWindow()) goto Error_0;
	if (!Game_CreateLayout()) goto Error_0;

	Game_SetDefsMenu(App.hMenu);
	Menu_SetFlag(App.hMenu,IDM_CONFIGSAVEONEXIT,App.Config.bSaveOnExit);

	LastFiles_LoadList();
	return(0);

Error_0:Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_WINDOW_CREATE),NULL,MB_ICONHAND);
	App.hWnd = NULL;
	return(-1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_MOVE -- Déplacement de la fenêtre				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Moved(HWND hWnd)
{
	RECT	rcClient;

	GetWindowRect(hWnd,&rcClient);
	App.Config.windowMain.position.iLeft = rcClient.left;
	App.Config.windowMain.position.iTop = rcClient.top;
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_SIZE -- Redimension de la fenêtre				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Resized(HWND hWnd)
{
	RECT	rcWindow;

	GetWindowRect(hWnd,&rcWindow);
	App.Config.windowMain.position.iWidth = rcWindow.right-rcWindow.left;
	App.Config.windowMain.position.iHeight = rcWindow.bottom-rcWindow.top;

	GetClientRect(hWnd,&rcWindow);
	Status_Resize(rcWindow.right);
	Game_Resize();
	InvalidateRect(hWnd,NULL,FALSE);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_DRAWITEM -- Affichage des différents éléments de la fenêtre	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

//--- Dimension des éléments ---

void Window_MeasureItems(HWND hWnd, UINT uCtlId, MEASUREITEMSTRUCT *pMeasure)
{
	if (pMeasure->CtlType == ODT_MENU)
		{
		Menu_MeasureItems(hWnd,uCtlId,pMeasure);
		return;
		}

	if (pMeasure->CtlType == ODT_COMBOBOX)
		{
		if (pMeasure->CtlID == CTLID_CHARACTERS_LIST) Game_MeasureList(pMeasure);
		return;
		}

	if (pMeasure->CtlType == ODT_LISTVIEW)
		{
		if (pMeasure->CtlID == CTLID_INVENTORY) Game_MeasureInventory(pMeasure);
		return;
		}

	return;
}

//--- Affichage des éléments ---

void Window_DrawItems(DRAWITEMSTRUCT *pDraw)
{
	if (pDraw->hwndItem == App.hwndStatus)
		{
		if (pDraw->itemID == STATUS_TEXT) Status_DrawStatusText(pDraw);
		else if (pDraw->itemID == STATUS_PROGRESS) Status_DrawProgressBar(pDraw);
		return;
		}

	if (pDraw->CtlType == ODT_BUTTON)
		{
		Game_PaintButton(pDraw);
		return;
		}

	if (pDraw->CtlType == ODT_STATIC)
		{
		Game_PaintBag(App.Game.pdcCurrent,pDraw);
		return;
		}

	if (pDraw->CtlType == ODT_MENU)
		{
		Menu_DrawItems(pDraw);
		return;
		}

	if (pDraw->CtlType == ODT_COMBOBOX)
		{
		if (pDraw->CtlID == CTLID_CHARACTERS_LIST) Game_DrawList(pDraw);
		return;
		}

	if (pDraw->CtlType == ODT_LISTVIEW)
		{
		if (pDraw->CtlID == CTLID_INVENTORY) Game_DrawInventory(pDraw);
		return;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_NOTIFY -- Messages des contrôles				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Notify(HWND hWnd, UINT idCtrl, NMHDR *pStruct)
{
	switch(pStruct->code)
		{
		case NM_DBLCLK:
			switch(idCtrl)
				{
				case CTLID_INVENTORY:
					Game_InventoryMenu(NULL,-1);
					break;
				}
			break;
		case NM_RCLICK:
			switch(idCtrl)
				{
				case CTLID_INVENTORY:
					Game_InventoryMenu(NULL,0);
					break;
				}
			break;
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ WM_COMMAND -- Déplacement de la souris				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Window_Command(HWND hWnd, UINT uCode, UINT idCtrl, HWND hwndCtrl)
{
	switch(uCode)
		{
		case BN_CLICKED:
		case CBN_SELCHANGE:
			switch(idCtrl)
				{
				case IDM_OPENSAVEGAME:
					Divine_Select();
					break;
				case IDM_OPENSAVEAS:
					Divine_OpenAs();
					break;
				case IDM_RECENTREMOVEOBSOLETE:
					LastFiles_RemoveObsolete();
					break;
				case IDM_RECENTREMOVEALL:
					LastFiles_RemoveAll();
					break;
				case IDM_INFOS:
					Infos_Show();
					break;
				case IDM_WRITESAVEGAME:
					Divine_Write();
					break;
				case IDM_WRITESAVEAS:
					Divine_WriteAs();
					break;
				case IDM_CLOSE:
					Divine_Close();
					break;
				case IDM_QUIT:
					PostMessage(hWnd,WM_CLOSE,0,0);
					break;
				case IDM_SHOWSAVETREE:
					Tree_Open((XML_NODE *)App.Game.Save.nodeXMLRoot.next);
					break;
				case IDM_SHOWMETATREE:
					Infos_Tree();
					break;
				case IDM_SHOWCHARTREE:
					Tree_Open((XML_NODE *)App.Game.pdcCurrent->pxnRoot->children.next);
					break;
				case IDM_REMOVEMODS:
					Mods_Dialog();
					break;
				case IDM_CONVERTER:
					Divine_RunConverter();
					break;
				case IDM_INVNAME:
					Game_InventoryMenu(NULL,IDM_INVNAME);
					break;
				case IDM_INVBACKPACKCLOSE:
					Game_InventoryMenu(NULL,IDM_INVBACKPACKCLOSE);
					break;
				case IDM_INVNODES:
					Game_InventoryMenu(NULL,IDM_INVNODES);
					break;
				case IDM_DB_REBUILDICONS:
					Game_IconsRebuild(hWnd);
					break;
				case IDM_DB_REBUILDITEMSROOTS:
					Game_ItemsRootRebuild(hWnd);
					break;
				case IDM_DB_REBUILDITEMSSTATS:
					Game_ItemsStatsRebuild(hWnd);
					break;
				case IDM_DB_REBUILDITEMSLOCALIZATION:
					Game_ItemsLocalizationRebuild(hWnd);
					break;
				case IDM_DB_REBUILDLANGUAGES:
					Game_LocaleRebuild(hWnd);
					break;
				case IDM_CONFIGMODIFY:
					Config_Modify();
					break;
				case IDM_CONFIGSAVE:
					Config_Save(FALSE,&App.Config);
					break;
				case IDM_CONFIGSAVEONEXIT:
					App.Config.bSaveOnExit = Menu_GetFlag(App.hMenu,IDM_CONFIGSAVEONEXIT);
					if (App.Config.bSaveOnExit) App.Config.bSaveOnExit = FALSE;
					else App.Config.bSaveOnExit = TRUE;
					Menu_SetFlag(App.hMenu,IDM_CONFIGSAVEONEXIT,App.Config.bSaveOnExit);
					break;
				case IDM_CONFIGLANGUAGE:
					Config_SelectLanguage();
					break;
				case IDM_ABOUT:
					About_Display(hWnd);
					break;
				case CTLID_CHARACTERS_LIST:
					Game_SaveTopIndex(App.Game.pdcCurrent,App.Game.Layout.hwndInventory);
					Game_CharacterChanged(TRUE);
					break;
				case CTLID_ABILITIES:
					Game_Abilities();
					break;
				case CTLID_TAGS:
					Game_Tags();
					break;
				case CTLID_TALENTS:
					Game_Talents();
					break;
				case CTLID_SKILLS:
					Game_Skills(hWnd);
					break;
				case CTLID_STATS:
					Game_Stats(hWnd);
					break;
				case CTLID_MENU:
					Game_InventoryMenu(hwndCtrl,0);
					break;
				case CTLID_LIFE:
					if (!xml_GetThisAttrValue(App.Game.pdcCurrent->pxaVitalityMax)) break;
					if (Game_EditSetValue(hWnd,Game_LocaleNameFromLocaleID(TEXT_CHR_LIFE),App.Game.pdcCurrent->pxaVitality,0,wcstol(xml_GetThisAttrValue(App.Game.pdcCurrent->pxaVitalityMax),NULL,10))) InvalidateRect(App.hWnd,NULL,FALSE);
					break;
				case CTLID_PHYSICAL:
					if (!xml_GetThisAttrValue(App.Game.pdcCurrent->pxaArmorMax)) break;
					if (Game_EditSetValue(hWnd,Game_LocaleNameFromLocaleID(TEXT_CHR_PHYSICAL),App.Game.pdcCurrent->pxaArmor,0,wcstol(xml_GetThisAttrValue(App.Game.pdcCurrent->pxaArmorMax),NULL,10))) InvalidateRect(App.hWnd,NULL,FALSE);
					break;
				case CTLID_MAGICAL:
					if (!xml_GetThisAttrValue(App.Game.pdcCurrent->pxaMagicArmorMax)) break;
					if (Game_EditSetValue(hWnd,Game_LocaleNameFromLocaleID(TEXT_CHR_MAGICAL),App.Game.pdcCurrent->pxaMagicArmor,0,wcstol(xml_GetThisAttrValue(App.Game.pdcCurrent->pxaMagicArmorMax),NULL,10))) InvalidateRect(App.hWnd,NULL,FALSE);
					break;
				case CTLID_EXPERIENCE:
				case CTLID_NEXTLEVEL:
					Game_EditXP(hWnd,App.Game.pdcCurrent->pxaExp);
					break;
				default:
					if (idCtrl >= IDM_LASTFILES) LastFiles_Reload(idCtrl);
					else if (idCtrl >= CTLID_ATTRIBUTES && idCtrl <= CTLID_ATTRIBUTES+6)
						{
						if (wcstol(xml_GetThisAttrValue(App.Game.pdcCurrent->pxaAttributes[idCtrl-CTLID_ATTRIBUTES]),NULL,10) > GAME_ATTRIBUTE_MAX && !App.Config.bCapOverride)
							{
							if (MessageBox(hWnd,Locale_GetText(TEXT_OVERRIDE_ATTRIBUTE),Locale_GetText(TEXT_TITLE_REQUEST),MB_YESNO|MB_ICONQUESTION) == IDYES)
								App.Config.bCapOverride = TRUE;
							}
						if (Game_EditSetValue(hWnd,Game_LocaleNameFromLocaleID(TextsAttr[idCtrl-CTLID_ATTRIBUTES]),App.Game.pdcCurrent->pxaAttributes[idCtrl-CTLID_ATTRIBUTES],GAME_ATTRIBUTE_MIN,App.Config.bCapOverride?GAME_ATTRIBUTE_OVERRIDE:GAME_ATTRIBUTE_MAX))
							InvalidateRect(App.hWnd,NULL,FALSE);
						}
					else if (idCtrl >= CTLID_POINTS && idCtrl <= CTLID_POINTS+4)
						{
						if (Game_EditSetValue(hWnd,Game_LocaleNameFromLocaleID(TextsPts[idCtrl-CTLID_POINTS]),App.Game.pdcCurrent->pxaPoints[idCtrl-CTLID_POINTS],GAME_POINTS_MIN,GAME_POINTS_MAX))
							InvalidateRect(App.hWnd,NULL,FALSE);
						}
					break;
				}
			break;
		}

	return;
}

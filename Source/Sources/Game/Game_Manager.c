
//<<>-<>>---------------------------------------------------------------------()
/*
	Affichage des personnages
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
#include "Game.h"
#include "GameEdit.h"
#include "Utils.h"
#include "Menus.h"
#include "XMLTree.h"
#include "Requests.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	InvMenu[];
extern UINT			ExpTable[];

static DOS2ITEM			RootItem;
static DOS2ITEM			ParentItem;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création des éléments «««««««««««««««««««««««««««««««««««««««««««»

int Game_CreateLayout()
{
	LVCOLUMN	lvColumn;
	LVGROUP		lvGroup;
	int		X,Y;
	int		W,H;
	int		i;

	//--- Liste des personnages ---

	App.Game.Layout.hwndList = CreateWindowEx(0,szComboBoxClass,NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_VSCROLL|WS_TABSTOP|WS_GROUP|CBS_DROPDOWNLIST|CBS_OWNERDRAWFIXED,20,20,392,300,App.hWnd,(HMENU)CTLID_CHARACTERS_LIST,App.hInstance,NULL);
	if (!App.Game.Layout.hwndList) return(0);

	X = 378;
	Y = 210;
	W = 20;
	H = App.Font.uFontHeight+6;

	// Attributs & points
	for (i = 0; i != 6; i++, Y += H+16) if (!Game_CreateButton(X,Y,W,H,NULL,CTLID_ATTRIBUTES+i,&App.Game.Layout.hwndAttrBtn[i],BS_OWNERDRAW)) return(0);
	for (Y += 16, i = 0; i != 4; i++, Y += H+16) if (!Game_CreateButton(X,Y,W,H,NULL,CTLID_POINTS+i,&App.Game.Layout.hwndPointsBtn[i],BS_OWNERDRAW)) return(0);

	// Boutons
	if (!Game_CreateButton(30,-54,120,0,Locale_GetText(TEXT_BUTTON_ABILITIES),CTLID_ABILITIES,&App.Game.Layout.hwndAbilitiesBtn,0)) return(0);
	if (!Game_CreateButton(156,-54,120,0,Locale_GetText(TEXT_BUTTON_TAGS),CTLID_TAGS,&App.Game.Layout.hwndTagsBtn,0)) return(0);
	if (!Game_CreateButton(282,-54,120,0,Locale_GetText(TEXT_BUTTON_TALENTS),CTLID_TALENTS,&App.Game.Layout.hwndTalentsBtn,0)) return(0);

	//--- Inventaire ---

	W = 562;
	H = 596;
	App.Game.Layout.hwndInventory = CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_VSCROLL|WS_TABSTOP|WS_GROUP|LVS_REPORT|LVS_SINGLESEL|LVS_NOCOLUMNHEADER|LVS_SHOWSELALWAYS|LVS_OWNERDRAWFIXED,436,20,W,H,App.hWnd,(HMENU)CTLID_INVENTORY,App.hInstance,NULL);
	if (!App.Game.Layout.hwndInventory) return(0);

	SendMessage(App.Game.Layout.hwndInventory,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER,LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER);

	lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = W-GetSystemMetrics(SM_CXVSCROLL)-GetSystemMetrics(SM_CXEDGE)*2;
	lvColumn.pszText = NULL;
	lvColumn.cchTextMax = 0;
	lvColumn.iOrder = 0;
	if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn) == -1) return(0);

	lvGroup.cbSize = sizeof(LVGROUP);
	lvGroup.mask = LVGF_GROUPID|LVGF_HEADER;
	lvGroup.iGroupId = 0;
	lvGroup.pszHeader = Locale_GetText(TEXT_INV_NAV);
	if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) return(0);
	lvGroup.iGroupId = 1;
	lvGroup.pszHeader = Locale_GetText(TEXT_INV_BAGS);
	if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) return(0);
	lvGroup.iGroupId = 2;
	lvGroup.pszHeader = Locale_GetText(TEXT_INV_EQUIPPED);
	if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) return(0);
	lvGroup.iGroupId = 3;
	lvGroup.pszHeader = Locale_GetText(TEXT_INV_ITEMS);
	if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) return(0);

	SendMessage(App.Game.Layout.hwndInventory,LVM_ENABLEGROUPVIEW,(WPARAM)TRUE,0);

	// Boutons
	if (!Game_CreateButton(446,-54,200,0,Locale_GetText(TEXT_BUTTON_MENU),CTLID_MENU,&App.Game.Layout.hwndMenuBtn,0)) return(0);

	// Nom de l'inventaire
	App.Game.Layout.hwndInventoryName = CreateWindowEx(WS_EX_CLIENTEDGE,szStaticClass,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_OWNERDRAW,656,628,332,App.Font.uFontHeight+16,App.hWnd,(HMENU)CTLID_INVENTORYNAME,App.hInstance,NULL);
	if (!App.Game.Layout.hwndInventoryName) return(0);

	Game_Setup(NULL,FALSE,FALSE);
	return(1);
}

//--- Création d'un bouton ---

int Game_CreateButton(int X, int Y, int W, int H, WCHAR *pszLabel, UINT uCtlId, HWND *hwndPtr, UINT uFlags)
{
	if (H == 0)
		H = App.Font.uFontHeight+16;

	if (Y < 0)
		{
		RECT	rcClient;

		GetClientRect(App.hWnd,&rcClient);
		Y = rcClient.bottom+Y-H;
		}

	*hwndPtr = CreateWindowEx(0,szButtonClass,NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_TABSTOP|BS_PUSHBUTTON|BS_TEXT|BS_CENTER|BS_VCENTER|uFlags,X,Y,W,H,App.hWnd,(HMENU)(UINT_PTR)uCtlId,App.hInstance,NULL);
	if (!*hwndPtr) return(0);
	SendMessage(*hwndPtr,WM_SETFONT,(WPARAM)App.Font.hFont,0);
	if (pszLabel) SendMessage(*hwndPtr,WM_SETTEXT,0,(LPARAM)pszLabel);
	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Interactions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Menu contextuel de l'inventaire «««««««««««««««««««««««««««««««««»

void Game_InventoryMenu(HWND hwndCtrl, UINT uMenuId)
{
	DOS2ITEM*	pItem = NULL;
	DOS2INVENTORY*	pInventory = NULL;

	//--- Récupère l'objet sélectionné ---

	if (SendMessage(App.Game.Layout.hwndInventory,LVM_GETSELECTEDCOUNT,0,0))
		{
		LVITEM		lvItem;

		lvItem.iItem = SendMessage(App.Game.Layout.hwndInventory,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
		if (lvItem.iItem != -1)
			{
			lvItem.mask = LVIF_PARAM;
			lvItem.iSubItem = 0;
			lvItem.lParam = 0;
			if (SendMessage(App.Game.Layout.hwndInventory,LVM_GETITEM,0,(LPARAM)&lvItem)) pItem = (DOS2ITEM *)lvItem.lParam;
			}
		}

	//--- Création et affichage du menu ---

	if (!uMenuId)
		{
		HMENU		hMenu;
		HMENU		hSubMenu;
		POINT		ptCursor;
		UINT		uFlags;

		hMenu = Menu_Create(InvMenu);
		if (!hMenu) return;

		if (pItem)
			{
			if (pItem->node.type != ITEM_TYPE_REAL)
				{
				EnableMenuItem(hMenu,IDM_INVEDIT,MF_BYCOMMAND|MF_GRAYED);
				EnableMenuItem(hMenu,IDM_INVNODES,MF_BYCOMMAND|MF_GRAYED);
				}
			EnableMenuItem(hMenu,IDM_INVBOOSTERS,MF_BYCOMMAND|(xml_IsTrue(pItem->pxaIsGenerated)?0:MF_GRAYED));
			EnableMenuItem(hMenu,IDM_INVBONUSES,MF_BYCOMMAND|(pItem->pxnPermanentBoost?0:MF_GRAYED));
			EnableMenuItem(hMenu,IDM_INVRUNES,MF_BYCOMMAND|(pItem->pxaRunes[0]?0:MF_GRAYED));
			EnableMenuItem(hMenu,IDM_INVSYNCHRO,MF_BYCOMMAND|(pItem->pxaLevel?0:MF_GRAYED));
			EnableMenuItem(hMenu,IDM_INVBACKPACKOPEN,MF_BYCOMMAND|(pItem->bIsBackPack?0:MF_GRAYED));
			}
		else
			{
			EnableMenuItem(hMenu,IDM_INVEDIT,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVNAME,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVBOOSTERS,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVBONUSES,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVRUNES,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVSYNCHRO,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVNODES,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVBACKPACKOPEN,MF_BYCOMMAND|MF_GRAYED);
			}

		EnableMenuItem(hMenu,IDM_INVBACKPACKCLOSE,MF_BYCOMMAND|(App.Game.pdcCurrent->uInventoryDepth?0:MF_GRAYED));
		EnableMenuItem(hMenu,IDM_INVBACKPACKCLOSEALL,MF_BYCOMMAND|(App.Game.pdcCurrent->uInventoryDepth > 1?0:MF_GRAYED));

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

		hSubMenu = GetSubMenu(hMenu,0);
		uMenuId = TrackPopupMenu(hSubMenu,uFlags|TPM_NOANIMATION|TPM_NONOTIFY|TPM_RETURNCMD,ptCursor.x,ptCursor.y,0,App.hWnd,NULL);
		Menu_Release(hMenu,InvMenu);
		}

	//--- Fonctions particulières (DoubleClick) ---

	else if (uMenuId == -1)
		{
		if (!pItem) return;

		switch(pItem->node.type)
			{
			case ITEM_TYPE_REAL:
				uMenuId = pItem->bIsBackPack?IDM_INVBACKPACKOPEN:IDM_INVEDIT;
				break;
			case ITEM_TYPE_ROOT:
				uMenuId = IDM_INVBACKPACKCLOSEALL;
				break;
			case ITEM_TYPE_PARENT:
				uMenuId = IDM_INVBACKPACKCLOSE;
				break;
			}
		}

	//--- Appel des fonctions ---

	switch(uMenuId)
		{
		case IDM_INVEDIT:
			if (!pItem) break;
			Game_Edit(pItem,0);
			break;
		case IDM_INVNAME:
			if (!pItem) break;
			Game_Edit(pItem,GAME_PAGE_NAME);
			break;
		case IDM_INVBOOSTERS:
			if (!pItem) break;
			if (!xml_IsTrue(pItem->pxaIsGenerated)) break;
			Game_Edit(pItem,GAME_PAGE_BOOSTERS);
			break;
		case IDM_INVBONUSES:
			if (!pItem) break;
			if (!pItem->pxnPermanentBoost) break;
			Game_Edit(pItem,GAME_PAGE_BONUSES);
			break;
		case IDM_INVRUNES:
			if (!pItem) break;
			if (!pItem->pxaRunes[0]) break;
			Game_Edit(pItem,GAME_PAGE_RUNES);
			break;
		case IDM_INVSYNCHRO:
			if (!pItem) break;
			if (!pItem->pxaLevel) break;
			Game_Edit(pItem,GAME_PAGE_SYNCHRONIZE);
			break;
		case IDM_INVBACKPACKOPEN:
			if (!pItem) break;
			if (!pItem->bIsBackPack) break;
			Game_SaveTopIndex();
			pInventory = Game_BuildInventory(pItem->pxaName,pItem->pxaDisplayName,pItem->pxaInventory,&App.Game.pdcCurrent->nodeInventories);
			if (pInventory)
				{
				App.Game.pdcCurrent->pdiInventory = pInventory;
				App.Game.pdcCurrent->uInventoryDepth++;
				Game_CharacterChanged(TRUE);
				}
			else Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_INVENTORY),NULL,MB_ICONERROR);
			break;
		case IDM_INVBACKPACKCLOSE:
			if (!App.Game.pdcCurrent) break;
			if (App.Game.pdcCurrent->uInventoryDepth)
				{
				NODE*	pNode;
				int	i;

				pNode = &App.Game.pdcCurrent->nodeInventories;
				i = App.Game.pdcCurrent->uInventoryDepth;
				do { pNode = pNode->next; } while(pNode && i--);
				App.Game.pdcCurrent->pdiInventory = (DOS2INVENTORY *)pNode->prev;
				App.Game.pdcCurrent->uInventoryDepth--;
				Game_CharacterChanged(TRUE);
				pInventory = (DOS2INVENTORY *)pNode;
				Game_ReleaseInventory(pInventory);
				InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
				}
			break;
		case IDM_INVBACKPACKCLOSEALL:
			if (!App.Game.pdcCurrent) break;
			if (App.Game.pdcCurrent->uInventoryDepth)
				{
				App.Game.pdcCurrent->pdiInventory = (DOS2INVENTORY *)App.Game.pdcCurrent->nodeInventories.next;
				App.Game.pdcCurrent->uInventoryDepth = 0;
				Game_CharacterChanged(TRUE);
				pInventory = (DOS2INVENTORY *)(((DOS2INVENTORY *)App.Game.pdcCurrent->nodeInventories.next)->node.next);
				while(pInventory)
					{
					Game_ReleaseInventory(pInventory);
					pInventory = (DOS2INVENTORY *)(((DOS2INVENTORY *)App.Game.pdcCurrent->nodeInventories.next)->node.next);
					}
				InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
				}
			break;
		case IDM_INVNODES:
			if (!pItem) break;
			Tree_Open(pItem->pxnXML);
			break;
		case IDM_INVSYNCHROALL:
			Game_SynchronizeAll();
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Gestion des personnages						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Réinitialisation des données relatives au jeu «««««««««««««««««««»

// If bStateOnly is TRUE, pdcList is ignored

void Game_Setup(DOS2CHARACTER *pdcList, BOOL bStateOnly, BOOL bState)
{
	DOS2CHARACTER*	pdc;
	BOOL		bEnable;
	int		i;

	if (!bStateOnly)
		{
		bEnable = pdcList?TRUE:FALSE;
		App.Game.pdcList = pdcList;
		App.Game.pdcCurrent = NULL;
		SendMessage(App.Game.Layout.hwndList,CB_RESETCONTENT,0,0);
		SendMessage(App.Game.Layout.hwndInventory,LVM_DELETEALLITEMS,0,0);
		if (pdcList)
			{
			for (pdc = pdcList; pdc != NULL; pdc = (DOS2CHARACTER *)pdc->node.next) SendMessage(App.Game.Layout.hwndList,CB_ADDSTRING,0,(LPARAM)pdc);
			SendMessage(App.Game.Layout.hwndList,CB_SETCURSEL,0,0);
			Game_CharacterChanged(FALSE);
			}
		}
	else bEnable = bState;

	for (i = 0; i != 6; i++) EnableWindow(App.Game.Layout.hwndAttrBtn[i],bEnable);
	for (i = 0; i != 4; i++) EnableWindow(App.Game.Layout.hwndPointsBtn[i],bEnable);

	EnableWindow(App.Game.Layout.hwndAbilitiesBtn,bEnable);
	EnableWindow(App.Game.Layout.hwndTagsBtn,bEnable);
	EnableWindow(App.Game.Layout.hwndTalentsBtn,bEnable);
	EnableWindow(App.Game.Layout.hwndList,bEnable);
	EnableWindow(App.Game.Layout.hwndInventory,bEnable);
	EnableWindow(App.Game.Layout.hwndMenuBtn,bEnable);

	ShowWindow(App.Game.Layout.hwndInventory,bEnable?SW_SHOW:SW_HIDE);

	InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
	InvalidateRect(App.hWnd,NULL,FALSE);
	return;
}


// «»»» Création de la liste des personnages ««««««««««««««««««««««««««««»

int Game_BuildPlayers()
{
	DOS2CHARACTER*	pdc;
	DOS2INVENTORY*	pInventory;
	XML_NODE*	pxnList;
	XML_NODE*	pxnTemp;
	WCHAR*		pszValue;
	int		i;

	pxnList = xml_GetNodeFromPathFirstChild((XML_NODE *)App.Game.Save.nodeXMLRoot.next,szCharactersPath);
	if (!pxnList) return(0);
	if (!pxnList->children.next) return(0);

	for (; pxnList != NULL; pxnList = (XML_NODE *)pxnList->node.next)
		{
		//--- Vérifications ---

		// Ignore les personnages vides (ne devrait pas se produire)
		if (!pxnList->children.next) continue;

		// Vérifie qu'il s'agit bien d'un personnage (node[id="Character"])
		pszValue = xml_GetAttrValue(pxnList,szXMLid);
		if (wcscmp(pszValue,szXMLCharacter)) continue;

		// Retrouve les stats du personnage
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterStatsPath);
		if (!pxnTemp) continue;

		// Ne conserve que les joueurs
		pxnTemp = xml_GetNode(pxnTemp,szXMLattribute,szXMLid,L"IsPlayer");
		if (!pxnTemp) continue;
		pszValue = xml_GetAttrValue(pxnTemp,szXMLvalue);
		if (wcscmp(pszValue,L"True")) continue;

		//--- Création du personnage ---

		pdc = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DOS2CHARACTER));
		if (!pdc)
			{
			Game_ReleasePlayers();
			return(0);
			}
		pdc->pxnXML = (XML_NODE *)pxnList->children.next;
		List_AddEntry((NODE *)pdc,&App.Game.nodeXMLCharacters);

		// Identifiant de l'inventaire
		pdc->pxaInventoryId = xml_GetXMLValueAttr((XML_NODE *)pxnList->children.next,szXMLattribute,szXMLid,L"Inventory");
		pdc->pxaDamageCount = xml_GetXMLValueAttr((XML_NODE *)pxnList->children.next,szXMLattribute,szXMLid,L"DamageCounter");
		pdc->pxaHealCount = xml_GetXMLValueAttr((XML_NODE *)pxnList->children.next,szXMLattribute,szXMLid,L"HealCounter");
		pdc->pxaKillCount = xml_GetXMLValueAttr((XML_NODE *)pxnList->children.next,szXMLattribute,szXMLid,L"KillCounter");
		// Informations générales
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterCustomDataPath);
		if (pxnTemp)
			{
			pdc->pxaName = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"Name");
			pdc->pxaOriginName = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"OriginName");
			pdc->pxaIsMale = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"IsMale");
			pdc->pxaRace = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"Race");
			}
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterStatsPath);
		if (pxnTemp)
			{
			pdc->pxaExp = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"Experience");
			}
		// Points disponibles
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterPlayerUpgradePath);
		if (pxnTemp)
			{
			pdc->pxaPoints[0] = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"AttributePoints");
			pdc->pxaPoints[1] = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"CombatAbilityPoints");
			pdc->pxaPoints[2] = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"CivilAbilityPoints");
			pdc->pxaPoints[3] = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"TalentPoints");
			}
		// Points dépensés
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterPointsPath);
		if (pxnTemp)
			{
			// Attributs
			for (i = 0; i != 6; pxnTemp = (XML_NODE *)pxnTemp->node.next, i++)
				{
				if (!pxnTemp) break;
				if (!pxnTemp->children.next) continue;
				pdc->pxaAttributes[i] = xml_GetXMLValueAttr((XML_NODE *)pxnTemp->children.next,szXMLattribute,szXMLid,L"Object");
				}
			// Abilities
			for (i = 0; i != 40; pxnTemp = (XML_NODE*)pxnTemp->node.next, i++)
				{
				if (!pxnTemp) break;
				if (!pxnTemp->children.next) continue;
				pdc->pxaAbilities[i] = xml_GetXMLValueAttr((XML_NODE *)pxnTemp->children.next,szXMLattribute,szXMLid,L"Object");
				}
			// Talents
			for (i = 0; i != 4; pxnTemp = (XML_NODE *)pxnTemp->node.next, i++)
				{
				if (!pxnTemp) break;
				if (!pxnTemp->children.next) continue;
				pdc->pxaTalents[i] = xml_GetXMLValueAttr((XML_NODE *)pxnTemp->children.next,szXMLattribute,szXMLid,L"Object");
				}
			}
		// Tags
		pdc->pxaTags = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnList->children.next,szCharacterTagsPath);

		//--- Création de l'inventaire du personnage ---

		pInventory = Game_BuildInventory(NULL,NULL,pdc->pxaInventoryId,&pdc->nodeInventories);
		if (!pInventory)
			{
			Game_ReleasePlayers();
			return(0);
			}
		pdc->pdiInventory = pInventory;
		}

	if (!App.Game.hIconsList) App.Game.hIconsList = LoadLibraryEx(szIconsPath,NULL,DONT_RESOLVE_DLL_REFERENCES|LOAD_LIBRARY_AS_DATAFILE|LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!App.Game.hRunesIconsList) App.Game.hRunesIconsList = LoadLibraryEx(szRunesIconsPath,NULL,DONT_RESOLVE_DLL_REFERENCES|LOAD_LIBRARY_AS_DATAFILE|LOAD_WITH_ALTERED_SEARCH_PATH);
	Game_Setup((DOS2CHARACTER *)App.Game.nodeXMLCharacters.next,FALSE,FALSE);
	return(1);
}


// «»»» Libération de la liste des personnages ««««««««««««««««««««««««««»

void Game_ReleasePlayers()
{
	DOS2CHARACTER*	pdc;
	DOS2INVENTORY*	pInventory;

	Game_Setup(NULL,FALSE,FALSE);
	Tree_Destroy();

	for (pdc = (DOS2CHARACTER *)App.Game.nodeXMLCharacters.next; pdc != NULL; pdc = (DOS2CHARACTER *)pdc->node.next)
		{
		for (pInventory = (DOS2INVENTORY *)pdc->nodeInventories.next; pInventory != NULL; pInventory = (DOS2INVENTORY *)pdc->nodeInventories.next)
			Game_ReleaseInventory(pInventory);
		}

	if (App.Game.hIconsList)
		{
		FreeLibrary(App.Game.hIconsList);
		App.Game.hIconsList = NULL;
		}
	if (App.Game.hRunesIconsList)
		{
		FreeLibrary(App.Game.hRunesIconsList);
		App.Game.hRunesIconsList = NULL;
		}

	List_ReleaseMemory(&App.Game.nodeXMLCharacters);
	return;
}


// «»»» Création d'un inventaire ««««««««««««««««««««««««««««««««««««««««»

DOS2INVENTORY* Game_BuildInventory(XML_ATTR *pxaParentName, XML_ATTR *pxaParentDisplayName, XML_ATTR *pxaInventoryId, NODE *pInventoriesRoot)
{
	XML_NODE*	pxnItemsList;
	XML_NODE*	pxnItem;
	XML_NODE*	pxnTemp;
	DOS2INVENTORY*	pInventory;
	DOS2ITEM*	pItem;
	WCHAR*		pszValue;

	//--- Alloue la structure de l'inventaire ---

	pInventory = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DOS2INVENTORY));
	if (!pInventory) return(NULL);
	pInventory->iSelected = -1;
	pInventory->pszNameRef = xml_GetThisAttrValue(pxaParentName);
	pInventory->pszDisplayName = Game_CopyDisplayName(xml_GetThisAttrValue(pxaParentDisplayName));
	List_AddEntry((NODE *)pInventory,pInventoriesRoot);

	//--- Recherche l'inventaire ---

	pxnItemsList = xml_GetNodeFromPathFirstChild((XML_NODE *)App.Game.Save.nodeXMLRoot.next,szItemsPath);
	if (!pxnItemsList) return(pInventory);
	if (!pxaInventoryId) return(pInventory);
	if (!pxaInventoryId->value) return(pInventory);

	//--- Ajout des objets ---

	for (pxnItem = pxnItemsList; pxnItem != NULL; pxnItem = (XML_NODE *)pxnItem->node.next)
		{
		// Ignore les objets vides (ne devrait pas se produire)
		if (!pxnItem->children.next) continue;
		// Vérifie qu'il s'agit bien d'un objet (node[id="Item"])
		pszValue = xml_GetAttrValue(pxnItem,szXMLid);
		if (wcscmp(pszValue,L"Item")) continue;
		// Vérifie que l'objet appartient bien au personnage
		pxnTemp = xml_GetNode((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Parent");
		if (!pxnTemp) continue;
		pszValue = xml_GetAttrValue(pxnTemp,szXMLvalue);
		if (wcscmp(pszValue,pxaInventoryId->value)) continue;

		//--- Création de l'objet ---

		pItem = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DOS2ITEM));
		if (!pItem)
			{
			Game_ReleaseInventory(pInventory);
			return(NULL);
			}
		pItem->node.type = ITEM_TYPE_REAL;
		pItem->pxnXML = (XML_NODE *)pxnItem->children.next;
		List_AddEntry((NODE *)pItem,&pInventory->nodeItems);

		// Attributs généraux
		pItem->pxaName = xml_GetXMLValueAttr((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Stats");
		pItem->pxaAmount = xml_GetXMLValueAttr((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Amount");
		pItem->pxaIsGenerated = xml_GetXMLValueAttr((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"IsGenerated");
		pItem->pxaInventory = xml_GetXMLValueAttr((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Inventory");
		pItem->pxaSlot = xml_GetXMLValueAttr((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Slot");
		pItem->bIsBackPack = FALSE;

		if (pItem->pxaInventory)
			{
			if (pItem->pxaInventory->value)
				{
				if (wcstol(pItem->pxaInventory->value,NULL,10))
					pItem->bIsBackPack = TRUE;
				}
			}

		// Bonus
		pItem->pxnGeneration = xml_GetNodeFromPath((XML_NODE *)pxnItem->children.next,szItemGeneratorPath);
		pItem->pxnPermanentBoost = xml_GetNodeFromPath((XML_NODE *)pxnItem->children.next,szItemBonusesPath);

		// Noms et description
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnItem->children.next,szItemCustomNamePath);
		if (pxnTemp) pItem->pxaDisplayName = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"CustomDisplayName");
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnItem->children.next,szItemCustomDescriptionPath);
		if (pxnTemp) pItem->pxaDescription = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"CustomDescription");

		// Nombre d'objets (sac)
		if (pItem->bIsBackPack && pItem->pxaInventory && pItem->pxaInventory->value) pItem->uNumItems = Game_GetInventoryItemsCount(pxnItemsList,pItem->pxaInventory->value);

		// Statistiques
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnItem->children.next,szItemStatsPath);
		if (pxnTemp)
			{
			pItem->pxaType = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"ItemType");
			pItem->pxaLevel = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"Level");
			pItem->pxaHasCustomBase = xml_GetXMLValueAttr(pxnTemp,szXMLattribute,szXMLid,L"CustomBaseStats");
			// Runes
			pxnTemp = xml_GetNode(pxnTemp,L"children",NULL,NULL);
			if (pxnTemp)
				{
				int i;
				for (i = 0, pxnTemp = (XML_NODE *)pxnTemp->children.next; pxnTemp != NULL; pxnTemp = (XML_NODE *)pxnTemp->node.next)
					{
					if (i >= 3) break;
					pszValue = xml_GetAttrValue(pxnTemp,szXMLid);
					if (!pszValue) continue;
					if (wcscmp(pszValue,L"RuneSlot")) continue;
					pItem->pxaRunes[i++] = xml_GetXMLValueAttr((XML_NODE *)pxnTemp->children.next,szXMLattribute,szXMLid,L"RuneStatsID");
					}
				}
			}
		pxnTemp = xml_GetNodeFromPathFirstChild((XML_NODE *)pxnItem->children.next,szItemBasePath);
		if (pxnTemp) pItem->pxnBase = pxnTemp;
		}

	//--- Terminé ! ---

	return(pInventory);
}

//--- Compte le nombre d'objet dans un sac --

UINT Game_GetInventoryItemsCount(XML_NODE *pxnItemsList, WCHAR *pszInventoryId)
{
	XML_NODE*	pxnItem;
	XML_NODE*	pxnTemp;
	WCHAR*		pszValue;
	UINT		uNumItems;

	for (uNumItems = 0, pxnItem = pxnItemsList; pxnItem != NULL; pxnItem = (XML_NODE *)pxnItem->node.next)
		{
		if (!pxnItem->children.next) continue;
		pszValue = xml_GetAttrValue(pxnItem,szXMLid);
		if (wcscmp(pszValue,L"Item")) continue;
		pxnTemp = xml_GetNode((XML_NODE *)pxnItem->children.next,szXMLattribute,szXMLid,L"Parent");
		if (!pxnTemp) continue;
		pszValue = xml_GetAttrValue(pxnTemp,szXMLvalue);
		if (wcscmp(pszValue,pszInventoryId)) continue;
		uNumItems++;
		}

	return(uNumItems);
}


// «»»» Libération d'un inventaire ««««««««««««««««««««««««««««««««««««««»

void Game_ReleaseInventory(DOS2INVENTORY *pdiInventory)
{
	List_ReleaseMemory(&pdiInventory->nodeItems);
	List_RemEntry((NODE *)pdiInventory);

	if (pdiInventory->pszDisplayName) HeapFree(App.hHeap,0,pdiInventory->pszDisplayName);
	HeapFree(App.hHeap,0,pdiInventory);
	return;
}


// «»»» Changement de personnage ««««««««««««««««««««««««««««««««««««««««»

void Game_CharacterChanged(BOOL bRefresh)
{
	DOS2CHARACTER*	pdc;
	DOS2ITEM*	pItem;
	LVITEM		lvItem;
	int		i;

	i = SendMessage(App.Game.Layout.hwndList,CB_GETCURSEL,0,0);
	if (i == CB_ERR) return;

	//--- Retrouve le personnage ---

	pdc = (DOS2CHARACTER *)SendMessage(App.Game.Layout.hwndList,CB_GETITEMDATA,(WPARAM)i,0);
	if (pdc == (DOS2CHARACTER *)CB_ERR) return;
	if (!pdc) return;

	//--- Affichage de l'inventaire ---

	SendMessage(App.Game.Layout.hwndInventory,LVM_DELETEALLITEMS,0,0);
	lvItem.mask = LVIF_PARAM|LVIF_GROUPID;
	lvItem.iSubItem = 0;
	for (lvItem.iItem = 0, pItem = (DOS2ITEM *)pdc->pdiInventory->nodeItems.next; pItem != NULL; pItem = (DOS2ITEM *)pItem->node.next, lvItem.iItem++)
		{
		lvItem.iGroupId = 3;
		if (pItem->bIsBackPack) lvItem.iGroupId = 1;
		else if (Game_IsItemEquipped(pItem)) lvItem.iGroupId = 2;
		lvItem.lParam = (LPARAM)pItem;
		if (SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) break;
		}
	SendMessage(App.Game.Layout.hwndInventory,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_ItemsListSort);

	if (pdc->uInventoryDepth)
		{
		RootItem.node.type = pdc->uInventoryDepth > 1?ITEM_TYPE_ROOT:ITEM_TYPE_PARENT;
		lvItem.iItem = 0;
		lvItem.iGroupId = 0;
		lvItem.lParam = (LPARAM)&RootItem;
		SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTITEM,0,(LPARAM)&lvItem);
		if (pdc->uInventoryDepth > 1)
			{
			ParentItem.node.type = ITEM_TYPE_PARENT;
			lvItem.iItem = 1;
			lvItem.lParam = (LPARAM)&ParentItem;
			SendMessage(App.Game.Layout.hwndInventory,LVM_INSERTITEM,0,(LPARAM)&lvItem);
			}
		}

	if (pdc->pdiInventory)
		{
		SendMessage(App.Game.Layout.hwndInventory,LVM_SCROLL,0,(LPARAM)pdc->pdiInventory->iTopIndex);
		if (pdc->pdiInventory->iSelected != -1)
			{
			lvItem.mask = LVIF_STATE;
			lvItem.stateMask = LVIS_SELECTED|LVIS_FOCUSED;
			lvItem.state = LVIS_SELECTED|LVIS_FOCUSED;
			SendMessage(App.Game.Layout.hwndInventory,LVM_SETITEMSTATE,(WPARAM)pdc->pdiInventory->iSelected,(LPARAM)&lvItem);
			}
		}

	//--- Stockage du pointeur du personnage ---

	App.Game.pdcCurrent = pdc;

	//--- Activation des menus et mise-à-jour de l'affichage ---

	InvalidateRect(App.Game.Layout.hwndInventoryName,NULL,FALSE);
	Game_Lock(GAME_LOCK_ENABLED|GAME_LOCK_TREE);
	if (bRefresh) InvalidateRect(App.hWnd,NULL,FALSE);
	return;
}

//--- Tri de la liste des objets ---

int CALLBACK Game_ItemsListSort(LPARAM lParam1, LPARAM lParam2, LPARAM Unused)
{
	DOS2ITEM*	pItem1;
	DOS2ITEM*	pItem2;
	BOOL		bItem1eq;
	BOOL		bItem2eq;
	int		iResult;

	pItem1 = (DOS2ITEM *)lParam1;
	pItem2 = (DOS2ITEM *)lParam2;

	// Backpacks at the top of the list
	if (!pItem1->bIsBackPack && pItem2->bIsBackPack) return(1);
	if (pItem1->bIsBackPack && !pItem2->bIsBackPack) return(-1);

	// Then, Equipped items
	bItem1eq = Game_IsItemEquipped(pItem1);
	bItem2eq = Game_IsItemEquipped(pItem2);
	if (!bItem1eq && bItem2eq) return(1);
	if (bItem1eq && !bItem2eq) return(-1);
	if (bItem1eq && bItem2eq)
		{
		#if GAME_EQUIPPED_SLOT_MAX != 10
		#error Slots need update
		#endif
		// 				Helmet, Chest, Leggings, Weapon1, Weapon2, Ring1, Belt, Boots, Gloves, Amulet, Ring 2
		long static const Slots[11] = { 0,      2,     7,        9,       10,      4,     6,    8,     3,      1,      5};
		long lSlot1 = 0;
		long lSlot2 = 0;
		if (pItem1->pxaSlot && pItem1->pxaSlot->value) lSlot1 = wcstol(pItem1->pxaSlot->value,NULL,10);
		if (pItem2->pxaSlot && pItem2->pxaSlot->value) lSlot2 = wcstol(pItem2->pxaSlot->value,NULL,10);
		if (lSlot1 <= sizeof(Slots)/sizeof(long) && lSlot2 <= sizeof(Slots)/sizeof(long))
			{
			if (Slots[lSlot1] > Slots[lSlot2]) return(1);
			else if (Slots[lSlot1] < Slots[lSlot2]) return(-1);
			}
		}

	// Finally, sort by name
	iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pItem1->pxaName->value,-1,pItem2->pxaName->value,-1,NULL,NULL,0);
	if (iResult == CSTR_LESS_THAN) return(-1);
	if (iResult == CSTR_GREATER_THAN) return(1);

	return(0);
}


// «»»» Sauvegarde les données de la liste ««««««««««««««««««««««««««««««»

void Game_SaveTopIndex()
{
	if (!App.Game.pdcCurrent) return;
	if (App.Game.pdcCurrent->pdiInventory)
		{
		App.Game.pdcCurrent->pdiInventory->iTopIndex = GetScrollPos(App.Game.Layout.hwndInventory,SB_VERT);
		if (!SendMessage(App.Game.Layout.hwndInventory,LVM_GETSELECTEDCOUNT,0,0)) return;
		App.Game.pdcCurrent->pdiInventory->iSelected = SendMessage(App.Game.Layout.hwndInventory,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
		}
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sous-Routines							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Verrouille certaines parties du programme «««««««««««««««««««««««»

void Game_Lock(DWORD uFlags)
{
	UINT	uEnable;

	if (uFlags & GAME_LOCK_ENABLED) uEnable = MF_ENABLED;
	else uEnable = MF_GRAYED;

	if (uFlags & GAME_LOCK_APP)
		{
		EnableMenuItem(App.hMenu,IDM_OPENSAVEGAME,uEnable);
		EnableMenuItem(App.hMenu,MENU_RECENT,uEnable);
		EnableMenuItem(App.hMenu,IDM_QUIT,uEnable);
		EnableMenuItem(App.hMenu,IDM_CONFIGLANGUAGE,uEnable);
		EnableMenuItem(App.hMenu,IDM_CONFIGSAVE,uEnable);
		EnableMenuItem(App.hMenu,IDM_CONFIGSAVEONEXIT,uEnable);
		}
	if (uFlags & GAME_LOCK_FILE)
		{
		EnableMenuItem(App.hMenu,IDM_SHOWMETATREE,uEnable);
		EnableMenuItem(App.hMenu,IDM_SHOWSAVETREE,uEnable);
		EnableMenuItem(App.hMenu,IDM_REMOVEMODS,uEnable);
		EnableMenuItem(App.hMenu,IDM_INFOS,uEnable);
		EnableMenuItem(App.hMenu,IDM_CLOSE,uEnable);
		EnableMenuItem(App.hMenu,IDM_WRITESAVEGAME,uEnable);
		}
	if (uFlags & GAME_LOCK_TREE)
		{
		EnableMenuItem(App.hMenu,IDM_SHOWCHARTREE,uEnable);
		}

	if (uFlags & GAME_LOCK_SETUP) Game_Setup(NULL,TRUE,(uFlags & GAME_LOCK_ENABLED)?TRUE:FALSE);
	return;
}


// «»»» Détermine le niveau en fonction de l'exp ««««««««««««««««««««««««»

UINT Game_GetLevelFromExp(UINT uExperience)
{
	UINT	i;

	for (i = 0; ExpTable[i] != 0; i++)
		if (ExpTable[i] > uExperience) return(i+1);

	return(i);
}


// «»»» Détermine si un objet est équipé ««««««««««««««««««««««««««««««««»

BOOL Game_IsItemEquipped(DOS2ITEM *pItem)
{
	// *Hacky*
	if (!pItem->pxaSlot) return(TRUE);
	if (!pItem->pxaSlot->value) return(FALSE);
	if (wcstol(pItem->pxaSlot->value,NULL,10) <= GAME_EQUIPPED_SLOT_MAX) return(TRUE);
	return(FALSE);
}


// «»»» Copie le nom d'affichage d'un objet «««««««««««««««««««««««««««««»

WCHAR* Game_CopyDisplayName(WCHAR *pszDisplayNameRef)
{
	WCHAR*	pszDisplayName = NULL;

	if (pszDisplayNameRef)
		{
		UINT uLen = Misc_HtmlSpecialCharsDecode(NULL,pszDisplayNameRef);
		if (uLen)
			{
			pszDisplayName = HeapAlloc(App.hHeap,0,++uLen*sizeof(WCHAR));
			if (pszDisplayName) Misc_HtmlSpecialCharsDecode(pszDisplayName,pszDisplayNameRef);
			}
		}

	return(pszDisplayName);
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Objets
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Utils.h"
#include "Requests.h"
#include "Texts.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	InvSelectMenu[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection d'un objet depuis un inventaire			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

DOS2ITEM* Game_ItemSelect(HWND hWnd)
{
	GAMEITEMSELECT*	pSelect;
	DOS2ITEM*	pSelected;
	INT_PTR		iResult;

	pSelected = NULL;
	pSelect = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEITEMSELECT));
	if (!pSelect)
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONHAND|MB_OK);
		goto Done;
		}
	pSelect->hwndParent = hWnd;

	if (!Game_BuildPlayers(&pSelect->players,FALSE))
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONHAND|MB_OK);
		goto Done;
		}

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1300),hWnd,Game_ItemSelectProc,(LPARAM)pSelect);
	if (iResult == -1) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONHAND|MB_OK);
	if (iResult == IDOK && pSelect->pItem) List_RemEntry((NODE *)pSelect->pItem);
	else pSelect->pItem = NULL;

Done:	if (pSelect)
		{
		pSelected = pSelect->pItem;
		Game_ReleasePlayers(&pSelect->players,FALSE);
		HeapFree(App.hHeap,0,pSelect);
		}

	return(pSelected);
}


// «»»» Messages de la boîte de dialogue ««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_ItemSelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMEITEMSELECT*	pSelect;

	if (uMsgId == WM_INITDIALOG)
		{
		DOS2CHARACTER*	pdc;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pSelect = (GAMEITEMSELECT *)lParam;

		if (!Game_CreateInventoryGroups(GetDlgItem(hDlg,300)))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		if (pSelect->players.next)
			{
			pSelect->pdcCurrent = (DOS2CHARACTER *)pSelect->players.next;
			for (pdc = (DOS2CHARACTER *)pSelect->players.next; pdc != NULL; pdc = (DOS2CHARACTER *)pdc->node.next) SendDlgItemMessage(hDlg,200,CB_ADDSTRING,0,(LPARAM)pdc);
			SendDlgItemMessage(hDlg,200,CB_SETCURSEL,0,0);
			Game_BuildItemsList((DOS2CHARACTER *)pSelect->players.next,GetDlgItem(hDlg,300));
			}

		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TITLE_ITEMSELECT));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_OK));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CANCEL));
		Dialog_CenterWindow(hDlg,pSelect->hwndParent);
		return(FALSE);
		}

	pSelect = (GAMEITEMSELECT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			switch(wParam)
				{
				case 200:
					Game_MeasureList((MEASUREITEMSTRUCT *)lParam);
					return(TRUE);
				case 300:
					Game_MeasureInventory((MEASUREITEMSTRUCT *)lParam);
					return(TRUE);
				default:
					if (((MEASUREITEMSTRUCT *)lParam)->CtlType != ODT_MENU) break;
					Menu_MeasureItems(hDlg,wParam,(MEASUREITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_DRAWITEM:
			switch(wParam)
				{
				case 200:
					Game_DrawList((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 300:
					Game_DrawInventory((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 301:
					Game_PaintBag(pSelect->pdcCurrent,(DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				default:
					if (((DRAWITEMSTRUCT *)lParam)->CtlType != ODT_MENU) break;
					Menu_DrawItems((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(wParam)
				{
				case 300:
					switch(((NMHDR *)lParam)->code)
						{
						case LVN_ITEMCHANGED:
							Game_ItemChanged(hDlg,pSelect);
							break;
						case NM_DBLCLK:
							Game_ItemMenu(hDlg,NULL,-1,pSelect);
							break;
						case NM_RCLICK:
							Game_ItemMenu(hDlg,NULL,0,pSelect);
							break;
						}
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
							Game_SaveTopIndex(pSelect->pdcCurrent,GetDlgItem(hDlg,300));
							Game_ItemCharacterChanged(hDlg,pSelect);
							return(TRUE);
						}
					break;

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


// «»»» Changement de personnage ««««««««««««««««««««««««««««««««««««««««»

void Game_ItemCharacterChanged(HWND hDlg, GAMEITEMSELECT *pSelect)
{
	DOS2CHARACTER*	pdc;
	HWND		hwndList;
	int		i;

	hwndList = GetDlgItem(hDlg,200);
	i = SendMessage(hwndList,CB_GETCURSEL,0,0);
	if (i == CB_ERR) return;

	//--- Retrouve le personnage ---

	pdc = (DOS2CHARACTER *)SendMessage(hwndList,CB_GETITEMDATA,(WPARAM)i,0);
	if (pdc == (DOS2CHARACTER *)CB_ERR) return;
	if (!pdc) return;

	//--- Création de la liste ---

	Game_BuildItemsList(pdc,GetDlgItem(hDlg,300));
	Game_ItemChanged(hDlg,pSelect);
	InvalidateRect(GetDlgItem(hDlg,301),NULL,FALSE);
	pSelect->pdcCurrent = pdc;
	return;
}


// «»»» Changement d'objet ««««««««««««««««««««««««««««««««««««««««««««««»

void Game_ItemChanged(HWND hDlg, GAMEITEMSELECT *pSelect)
{
	HWND	hwndInventory;

	hwndInventory = GetDlgItem(hDlg,300);
	pSelect->pItem = NULL;

	if (SendMessage(hwndInventory,LVM_GETSELECTEDCOUNT,0,0))
		{
		LVITEM		lvItem;
		DOS2ITEM*	pItem;

		lvItem.iItem = SendMessage(hwndInventory,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
		if (lvItem.iItem != -1)
			{
			lvItem.mask = LVIF_PARAM;
			lvItem.iSubItem = 0;
			lvItem.lParam = 0;
			if (SendMessage(hwndInventory,LVM_GETITEM,0,(LPARAM)&lvItem))
				{
				pItem = (DOS2ITEM *)lvItem.lParam;
				if (pItem && pItem->node.type == ITEM_TYPE_REAL) pSelect->pItem = pItem;
				}
			}
		}

	EnableWindow(GetDlgItem(hDlg,IDOK),pSelect->pItem?TRUE:FALSE);
	return;
}


// «»»» Menu des objets «««««««««««««««««««««««««««««««««««««««««««««««««»

void Game_ItemMenu(HWND hDlg, HWND hwndCtrl, UINT uMenuId, GAMEITEMSELECT *pSelect)
{
	DOS2ITEM*	pItem;
	DOS2INVENTORY*	pInventory;
	HWND		hwndInventory;
	HWND		hwndInventoryName;

	pItem = NULL;
	pInventory = NULL;
	hwndInventory = GetDlgItem(hDlg,300);
	hwndInventoryName = GetDlgItem(hDlg,301);

	//--- Récupère l'objet sélectionné ---

	if (SendMessage(hwndInventory,LVM_GETSELECTEDCOUNT,0,0))
		{
		LVITEM		lvItem;

		lvItem.iItem = SendMessage(hwndInventory,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
		if (lvItem.iItem != -1)
			{
			lvItem.mask = LVIF_PARAM;
			lvItem.iSubItem = 0;
			lvItem.lParam = 0;
			if (SendMessage(hwndInventory,LVM_GETITEM,0,(LPARAM)&lvItem)) pItem = (DOS2ITEM *)lvItem.lParam;
			}
		}

	//--- Création et affichage du menu ---

	if (!uMenuId)
		{
		HMENU		hMenu;
		HMENU		hSubMenu;
		POINT		ptCursor;
		UINT		uFlags;

		hMenu = Menu_Create(InvSelectMenu);
		if (!hMenu) return;
		if (pItem)
			{
			if (pItem->node.type != ITEM_TYPE_REAL)
				{
				EnableMenuItem(hMenu,IDM_INVSELECT,MF_BYCOMMAND|MF_GRAYED);
				EnableMenuItem(hMenu,IDM_INVNODES,MF_BYCOMMAND|MF_GRAYED);
				}
			EnableMenuItem(hMenu,IDM_INVBACKPACKOPEN,MF_BYCOMMAND|(pItem->bIsBackPack?0:MF_GRAYED));
			}
		else
			{
			EnableMenuItem(hMenu,IDM_INVSELECT,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVNODES,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_INVBACKPACKOPEN,MF_BYCOMMAND|MF_GRAYED);
			}

		EnableMenuItem(hMenu,IDM_INVBACKPACKCLOSE,MF_BYCOMMAND|(pSelect->pdcCurrent->uInventoryDepth?0:MF_GRAYED));
		EnableMenuItem(hMenu,IDM_INVBACKPACKCLOSEALL,MF_BYCOMMAND|(pSelect->pdcCurrent->uInventoryDepth > 1?0:MF_GRAYED));

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
		uMenuId = TrackPopupMenu(hSubMenu,uFlags|TPM_NOANIMATION|TPM_NONOTIFY|TPM_RETURNCMD,ptCursor.x,ptCursor.y,0,hDlg,NULL);
		Menu_Release(hMenu,InvSelectMenu);
		}

	//--- Fonctions particulières (DoubleClick) ---

	else if (uMenuId == -1)
		{
		if (!pItem) return;

		switch(pItem->node.type)
			{
			case ITEM_TYPE_REAL:
				uMenuId = pItem->bIsBackPack?IDM_INVBACKPACKOPEN:IDM_INVSELECT;
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
		case IDM_INVBACKPACKOPEN:
			if (!pItem) break;
			if (!pItem->bIsBackPack) break;
			Game_SaveTopIndex(pSelect->pdcCurrent,hwndInventory);
			pInventory = Game_BuildInventory(pItem,pItem->pxaInventory,&pSelect->pdcCurrent->nodeInventories);
			if (pInventory)
				{
				pSelect->pdcCurrent->pdiInventory = pInventory;
				pSelect->pdcCurrent->uInventoryDepth++;
				Game_ItemCharacterChanged(hDlg,pSelect);
				SetFocus(hwndInventory);
				}
			else Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_INVENTORY),NULL,MB_ICONERROR);
			break;
		case IDM_INVBACKPACKCLOSE:
			if (!pSelect->pdcCurrent) break;
			if (pSelect->pdcCurrent->uInventoryDepth)
				{
				NODE*	pNode;
				int	i;

				pNode = &pSelect->pdcCurrent->nodeInventories;
				i = pSelect->pdcCurrent->uInventoryDepth;
				do { pNode = pNode->next; } while(pNode && i--);
				pSelect->pdcCurrent->pdiInventory = (DOS2INVENTORY *)pNode->prev;
				pSelect->pdcCurrent->uInventoryDepth--;
				Game_ItemCharacterChanged(hDlg,pSelect);
				pInventory = (DOS2INVENTORY *)pNode;
				Game_ReleaseInventory(pInventory);
				InvalidateRect(hwndInventoryName,NULL,FALSE);
				SetFocus(hwndInventory);
				}
			break;
		case IDM_INVBACKPACKCLOSEALL:
			if (!pSelect->pdcCurrent) break;
			if (pSelect->pdcCurrent->uInventoryDepth)
				{
				pSelect->pdcCurrent->pdiInventory = (DOS2INVENTORY *)pSelect->pdcCurrent->nodeInventories.next;
				pSelect->pdcCurrent->uInventoryDepth = 0;
				Game_ItemCharacterChanged(hDlg,pSelect);
				pInventory = (DOS2INVENTORY *)(((DOS2INVENTORY *)pSelect->pdcCurrent->nodeInventories.next)->node.next);
				while(pInventory)
					{
					Game_ReleaseInventory(pInventory);
					pInventory = (DOS2INVENTORY *)(((DOS2INVENTORY *)pSelect->pdcCurrent->nodeInventories.next)->node.next);
					}
				InvalidateRect(hwndInventoryName,NULL,FALSE);
				SetFocus(hwndInventory);
				}
			break;
		case IDM_INVNODES:
			if (!pItem) break;
			Tree_Open((XML_NODE *)pItem->pxnRoot->children.next);
			break;
		case IDM_INVSELECT:
			if (!pItem) break;
			pSelect->pItem = pItem;
			EndDialog(hDlg,IDOK);
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Vérification d'un modèle ««««««««««««««««««««««««««««««««««««««««»

int Game_ItemTemplateCheck(DOS2ITEM *pItem, WCHAR *pszNewTemplate, UINT uNewTemplateType)
{
	WCHAR*	pszOriginalTemplate;
	WCHAR*	pszCurrentTemplate;
	WCHAR*	pszTemplateType;
	UINT	uOriginalTemplateType;
	UINT	uCurrentTemplateType;
	GUID	uidOriginalTemplate;
	GUID	uidCurrentTemplate;
	GUID	uidNewTemplate;
	GUID	uidEmpty;
	WCHAR	szUID[40];

	ZeroMemory(&uidOriginalTemplate,sizeof(GUID));
	ZeroMemory(&uidCurrentTemplate,sizeof(GUID));
	ZeroMemory(&uidNewTemplate,sizeof(GUID));
	ZeroMemory(&uidEmpty,sizeof(GUID));

	//--- Original values ---

	pszCurrentTemplate = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"CurrentTemplate"),szXMLvalue);
	pszOriginalTemplate = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"OriginalTemplate"),szXMLvalue);
	pszTemplateType = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"CurrentTemplateType"),szXMLvalue);
	uCurrentTemplateType = pszTemplateType?wcstol(pszTemplateType,NULL,10):0;
	pszTemplateType = xml_GetAttrValue(xml_GetNode((XML_NODE *)pItem->pxnRoot->children.next,szXMLattribute,szXMLid,L"OriginalTemplateType"),szXMLvalue);
	uOriginalTemplateType = pszTemplateType?wcstol(pszTemplateType,NULL,10):0;
	if (pszCurrentTemplate)
		{
		Misc_Printf(szUID,40,szUIDFmt,pszCurrentTemplate);
		IIDFromString(szUID,&uidCurrentTemplate);
		}
	if (pszOriginalTemplate)
		{
		Misc_Printf(szUID,40,szUIDFmt,pszOriginalTemplate);
		IIDFromString(szUID,&uidOriginalTemplate);
		}

	//--- Vérifications ---

	if (!pszNewTemplate) return(ITEM_TEMPLATE_INVALID);						// No UID

	Misc_Printf(szUID,40,szUIDFmt,pszNewTemplate);
	if (IIDFromString(szUID,&uidNewTemplate) != S_OK) return(ITEM_TEMPLATE_INVALID);		// Invalid UID
	if (!memcmp(&uidNewTemplate,&uidEmpty,sizeof(GUID))) return(ITEM_TEMPLATE_INVALID);		// Empty UID

	if (!memcmp(&uidNewTemplate,&uidCurrentTemplate,sizeof(GUID)))					// UID == CurrentUID
		{
		if (uCurrentTemplateType != uNewTemplateType) return(ITEM_TEMPLATE_TYPEMISMATCH);
		return(ITEM_TEMPLATE_ALREADYDEFINED);
		}
	if (!memcmp(&uidNewTemplate,&uidOriginalTemplate,sizeof(GUID)))					// UID == OriginalUID
		{
		if (uOriginalTemplateType != uNewTemplateType) return(ITEM_TEMPLATE_TYPEMISMATCH);
		return(ITEM_TEMPLATE_ALREADYDEFINED);
		}

	return(ITEM_TEMPLATE_OK);
}


// «»»» Affichage d'erreur ««««««««««««««««««««««««««««««««««««««««««««««»

int Game_ItemTemplateMsg(HWND hWnd, int iMsg)
{
	UINT	uLocaleId = 0;

	switch(iMsg)
		{
		case ITEM_TEMPLATE_INVALID:
			uLocaleId = TEXT_ERR_TEMPLATE_INVALID;
			break;
		case ITEM_TEMPLATE_TYPEMISMATCH:
			uLocaleId = TEXT_ERR_TEMPLATE_TYPEMISMATCH;
			break;
		}

	if (uLocaleId) MessageBox(hWnd,Locale_GetText(uLocaleId),NULL,MB_ICONERROR|MB_OK);
	return(uLocaleId?0:1);
}

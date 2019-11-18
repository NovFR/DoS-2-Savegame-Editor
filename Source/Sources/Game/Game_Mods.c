
//<<>-<>>---------------------------------------------------------------------()
/*
	Gestion des mods
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Mods.h"
#include "Files.h"
#include "Infos.h"
#include "Locale.h"
#include "Dialogs.h"
#include "Requests.h"
#include "Texts.h"

extern APPLICATION	App;
extern WCHAR*		ModsIgnore[];
extern WCHAR*		ModsLarian[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Mods_Dialog()
{
	MODSCONTEXT*	pContext;
	NODE*		pRoot;
	int		iResult;

	if (!Infos_LoadMetaDatas(App.hWnd)) return;

	//--- Alloue la structure ---

	pContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(MODSCONTEXT));
	if (!pContext)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return;
		}

	//--- Création de la liste des mods ---

	pRoot = lsv_GetMetaXML(&App.Game.Save.nodeFiles);
	if (pRoot)
		{
		MODSTRUCT	mod;
		MODSTRUCT*	pMod;
		XML_NODE*	pxn;
		XML_NODE*	pxnChild;
		WCHAR*		pszValue;
		BOOL		bContinue;
		int		i;

		for (pxn = xml_GetNodeFromPath(xml_GetNodeFromPathFirstChild((XML_NODE *)pRoot->next,szInfoMetaDataPath),szInfoGameModsPath); pxn != NULL; pxn = (XML_NODE *)pxn->node.next)
			{
			pszValue = xml_GetAttrValue(pxn,szXMLid);
			if (!pszValue) continue;
			if (wcscmp(pszValue,L"ModuleShortDesc")) continue;
			ZeroMemory(&mod,sizeof(MODSTRUCT));
			mod.bIsActive = TRUE;
			mod.node.type = MOD_USER;
			mod.pxnMeta = pxn;
			for (pxnChild = (XML_NODE *)pxn->children.next; pxnChild != NULL; pxnChild = (XML_NODE *)pxnChild->node.next)
				{
				pszValue = xml_GetAttrValue(pxnChild,szXMLid);
				if (!pszValue) continue;
				if (!wcscmp(pszValue,L"UUID"))
					{
					pszValue = xml_GetAttrValue(pxnChild,szXMLvalue);
					if (!pszValue) continue;
					bContinue = FALSE;
					for (i = 0; ModsIgnore[i] != NULL; i++)
						{
						if (wcscmp(ModsIgnore[i],pszValue)) continue;
						mod.node.type = MOD_CRITICAL;
						bContinue = TRUE;
						break;
						}
					if (bContinue) continue;
					for (i = 0; ModsLarian[i] != NULL; i++)
						{
						if (wcscmp(ModsLarian[i],pszValue)) continue;
						mod.node.type = MOD_LARIAN;
						break;
						}
					continue;
					}
				if (!wcscmp(pszValue,L"Name"))
					{
					pszValue = xml_GetAttrValue(pxnChild,szXMLvalue);
					if (!pszValue) continue;
					mod.pszName = HeapAlloc(App.hHeap,0,wcslen(pszValue)*sizeof(WCHAR)+sizeof(WCHAR));
					if (mod.pszName) wcscpy(mod.pszName,pszValue);
					continue;
					}
				if (!wcscmp(pszValue,L"Version"))
					{
					pszValue = xml_GetAttrValue(pxnChild,szXMLvalue);
					mod.pszVersion = Infos_GetVersionString(pszValue,NULL);
					continue;
					}
				}
			pMod = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(MODSTRUCT));
			if (!pMod)
				{
				if (mod.pszName) HeapFree(App.hHeap,0,mod.pszName);
				if (mod.pszVersion) LocalFree(mod.pszVersion);
				Mods_Release(&pContext->mods);
				HeapFree(App.hHeap,0,pContext);
				SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
				return;
				}
			CopyMemory(pMod,&mod,sizeof(MODSTRUCT));
			List_AddEntry((NODE *)pMod,&pContext->mods);
			}
		}

	//--- Boîte de dialogue ---

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1200),App.hWnd,Mods_Proc,(LPARAM)pContext);
	if (!iResult || iResult == -1)
		{
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		Mods_Release(&pContext->mods);
		HeapFree(App.hHeap,0,pContext);
		return;
		}

	//--- Retrait des mods ---

	if (iResult == IDOK)
		{
		MODSTRUCT*	pMod;

		for (pMod = (MODSTRUCT *)pContext->mods.next; pMod != NULL; pMod = (MODSTRUCT *)pMod->node.next)
			{
			if (pMod->bIsActive) continue;
			// Only removes the mods in the metadatas.
			// This will trigger a warning while loading the savegame but the mods are removed nonetheless.
			xml_ReleaseNode(pMod->pxnMeta);
			}
		}

	//--- Terminé ---

	Mods_Release(&pContext->mods);
	HeapFree(App.hHeap,0,pContext);
	return;
}

//--- Libère la mémoire utilisée ---

void Mods_Release(NODE *pRoot)
{
	MODSTRUCT*	pMod;

	for (pMod = (MODSTRUCT *)pRoot->next; pMod != NULL; pMod = (MODSTRUCT *)pMod->node.next)
		{
		if (pMod->pszVersion) LocalFree(pMod->pszVersion);
		if (pMod->pszName) HeapFree(App.hHeap,0,pMod->pszName);
		}

	List_ReleaseMemory(pRoot);
	return;
}


// «»»» Messages ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Mods_Proc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	MODSCONTEXT*	pContext;

	if (uMsgId == WM_INITDIALOG)
		{
		MODSTRUCT*	pMod;
		LVGROUP		lvGroup;
		LVCOLUMN	lvColumn;
		LVITEM		lvItem;
		LVTILEINFO	lvTileInfo;
		RECT		rcDialog;
		UINT		uColumns[1] = { 1 };
		static UINT	Groups[] = { TEXT_MODS_GROUP_CRITICAL, TEXT_MODS_GROUP_LARIAN, TEXT_MODS_GROUP_USER, 0 };
		int		Height;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pContext = (MODSCONTEXT *)lParam;

		Height = Dialog_GetInfoHeight(hDlg,100,Locale_GetText(TEXT_MODS_INFO));
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);

		SendDlgItemMessage(hDlg,200,LVM_SETEXTENDEDLISTVIEWSTYLE,(WPARAM)LVS_EX_CHECKBOXES|LVS_EX_DOUBLEBUFFER,(LPARAM)LVS_EX_CHECKBOXES|LVS_EX_DOUBLEBUFFER);
		lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 0;
		lvColumn.pszText = NULL;
		lvColumn.cchTextMax = 0;
		lvColumn.iOrder = 0;
		if (SendDlgItemMessage(hDlg,200,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn) == -1)
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		lvColumn.iOrder = 1;
		if (SendDlgItemMessage(hDlg,200,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn) == -1)
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		lvGroup.cbSize = sizeof(LVGROUP);
		lvGroup.mask = LVGF_HEADER|LVGF_GROUPID;
		for (lvGroup.iGroupId = 0; Groups[lvGroup.iGroupId] != 0; lvGroup.iGroupId++)
			{
			lvGroup.pszHeader = Locale_GetText(Groups[lvGroup.iGroupId]);
			if (SendDlgItemMessage(hDlg,200,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			}

		lvItem.mask = LVIF_PARAM|LVIF_TEXT|LVIF_GROUPID;
		for (pMod = (MODSTRUCT *)pContext->mods.next; pMod != NULL; pMod = (MODSTRUCT *)pMod->node.next)
			{
			lvItem.iSubItem = 0;
			lvItem.iGroupId = pMod->node.type;
			lvItem.pszText = pMod->pszName;
			lvItem.lParam = (LPARAM)pMod;
			if (SendDlgItemMessage(hDlg,200,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			lvItem.iSubItem = 1;
			lvItem.pszText = pMod->pszVersion;
			if (SendDlgItemMessage(hDlg,200,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == -1)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			lvItem.state = INDEXTOSTATEIMAGEMASK(pMod->bIsActive?2:1);
			lvItem.stateMask = LVIS_STATEIMAGEMASK;
			SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
			lvTileInfo.cbSize = sizeof(LVTILEINFO);
			lvTileInfo.iItem = lvItem.iItem;
			lvTileInfo.cColumns = 1;
			lvTileInfo.puColumns = uColumns;
			lvTileInfo.piColFmt = NULL;
			if (SendDlgItemMessage(hDlg,200,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == -1)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			}

		SendDlgItemMessage(hDlg,200,LVM_SETVIEW,(WPARAM)LV_VIEW_TILE,(LPARAM)0);
		SendDlgItemMessage(hDlg,200,LVM_ENABLEGROUPVIEW,(WPARAM)TRUE,0);

		Dialog_OffsetY(hDlg,200,Height);
		Dialog_OffsetY(hDlg,IDOK,Height);
		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		Dialog_OffsetY(hDlg,IDCANCEL,Height);
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));

		SetWindowText(hDlg,Locale_GetText(IDM_REMOVEMODS));
		Dialog_CenterWindow(hDlg,App.hWnd);
		pContext->bInitialized = TRUE;
		return(FALSE);
		}

	pContext = (MODSCONTEXT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(Locale_GetText(TEXT_MODS_INFO),(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_ITEMACTIVATE:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 200:
							Mods_Check(hDlg,((NMITEMACTIVATE *)lParam)->iItem,TRUE,pContext);
							return(TRUE);
						}
					break;
				case LVN_ITEMCHANGED:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 200:
							Mods_Check(hDlg,((NMLISTVIEW *)lParam)->iItem,FALSE,pContext);
							return(TRUE);
						}
					break;
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


// «»»» Active/Désactive un talent ««««««««««««««««««««««««««««««««««««««»

void Mods_Check(HWND hDlg, int iItem, BOOL bToggle, MODSCONTEXT *pContext)
{
	MODSTRUCT*	pMod;
	LVITEM		lvItem;
	BOOL		bChecked;

	if (!pContext->bInitialized) return;

	//--- Récupère le talent ---

	lvItem.mask = LVIF_PARAM|LVIF_STATE|LVIF_GROUPID;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
	lvItem.iGroupId = 0;
	lvItem.state = 0;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.lParam = 0;
	SendDlgItemMessage(hDlg,200,LVM_GETITEM,0,(LPARAM)&lvItem);

	//--- Empêche la modification des mods critiques ---

	if (lvItem.iGroupId == MOD_CRITICAL)
		{
		if (lvItem.state & INDEXTOSTATEIMAGEMASK(1))
			{
			lvItem.state = INDEXTOSTATEIMAGEMASK(2);
			lvItem.stateMask = LVIS_STATEIMAGEMASK;
			SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
			}
		return;
		}

	//--- Modifie l'état ---

	pMod = (MODSTRUCT *)lvItem.lParam;
	if (lvItem.state & INDEXTOSTATEIMAGEMASK(1)) bChecked = FALSE;
	else bChecked = TRUE;

	pMod->bIsActive = bChecked;

	//--- Bascule l'état ---

	if (bToggle)
		{
		if (bChecked) bChecked = FALSE;
		else bChecked = TRUE;
		lvItem.state = INDEXTOSTATEIMAGEMASK(bChecked?2:1);
		lvItem.stateMask = LVIS_STATEIMAGEMASK;
		SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
		return;
		}

	return;
}

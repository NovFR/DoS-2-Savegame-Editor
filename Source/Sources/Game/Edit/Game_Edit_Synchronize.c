
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Tout synchroniser
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Dialogs.h"
#include "Texts.h"
#include "Utils.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Synchronisation							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Synchronize un objet sur le niveau désiré «««««««««««««««««««««««»

void Game_Synchronize_Level(int iNewLevel, BOOL bOnlyLowerLevels, XML_ATTR *pxaLevel, XML_ATTR *pxaIsGenerated, XML_NODE *pxnGeneration, XML_NODE **pxnLevelOverride)
{
	WCHAR*		pszLevel;
	WCHAR		szTemp[4];
	int		iLevel;

	if (!pxaLevel) return;

	pszLevel = xml_GetThisAttrValue(pxaLevel);
	if (!pszLevel) return;
	iLevel = wcstol(pszLevel,NULL,10);
	if (bOnlyLowerLevels && iLevel > iNewLevel) return;

	//--- Change level ---

	Misc_Printf(szTemp,4,szLevelFmt,iNewLevel);
	pszLevel = HeapAlloc(App.hHeap,0,wcslen(szTemp)*sizeof(WCHAR)+sizeof(WCHAR));
	if (pszLevel)
		{
		if (pxaLevel->value) HeapFree(App.hHeap,0,pxaLevel->value);
		wcscpy(pszLevel,szTemp);
		pxaLevel->value = pszLevel;
		}

	//--- Change Item Generation's level ---

	if (xml_IsTrue(pxaIsGenerated) && pxnGeneration)
		{
		XML_ATTR* Attr = xml_GetXMLValueAttr((XML_NODE *)pxnGeneration->children.next,L"attribute",L"id",L"Level");
		Game_Synchronize_Level(iNewLevel,bOnlyLowerLevels,Attr,NULL,NULL,NULL);
		}

	//--- Remove Level Override ---

        if (pxnLevelOverride && *pxnLevelOverride)
		{
		xml_ReleaseNode(*pxnLevelOverride);
		*pxnLevelOverride = NULL;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Tout synchroniser «««««««««««««««««««««««««««««««««««««««««««««««»

void Game_SynchronizeAll()
{
	DIALOGSYNCHRO	Synchro;

	ZeroMemory(&Synchro,sizeof(DIALOGSYNCHRO));

	Synchro.dialogStruct.hwndParent = App.hWnd;
	Synchro.dialogStruct.pszTitle = Locale_GetText(TEXT_DIALOG_TITLE_SYNCHRONIZEALL);
	Synchro.dialogStruct.pszText = Locale_GetText(TEXT_DIALOG_INFO_SYNCHRONIZEALL);
	Synchro.dialogStruct.uResID = 1001;
	Synchro.dialogStruct.fnInit = Game_SynchronizeAll_Init;
	Synchro.dialogStruct.fnDraw = Game_SynchronizeAll_Draw;
	Synchro.dialogStruct.fnOk = Game_SynchronizeAll_Ok;

	Synchro.iLevelMin = GAME_LEVEL_MIN;
	Synchro.iLevelMax = GAME_LEVEL_MAX;
	Synchro.iLevel = 1;
	Synchro.bOnlyEquipped = FALSE;
	Synchro.bOnlyLowerLevels = TRUE;

	if (App.Game.pdcCurrent)
		{
		WCHAR*		pszLevel;

		pszLevel = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
		if (!pszLevel) Synchro.iLevel = 1;
		else Synchro.iLevel = Game_GetLevelFromExp(wcstol(pszLevel,NULL,10));
		}

	Dialog_Open((DIALOG *)&Synchro);
	return;
}


// «»»» Initialisation de la boîte de dialogue ««««««««««««««««««««««««««»

int Game_SynchronizeAll_Init(HWND hDlg, RECT *rcDialog, int iHeight, void *pDialog)
{
	DIALOGSYNCHRO*	pSynchro;
	HWND		hwndCtrl;

	pSynchro = (DIALOGSYNCHRO *)pDialog;
	Dialog_OffsetY(hDlg,200,iHeight);
	Dialog_OffsetY(hDlg,201,iHeight);
	Dialog_OffsetY(hDlg,202,iHeight);
	Dialog_OffsetY(hDlg,203,iHeight);

	SetDlgItemInt(hDlg,200,pSynchro->iLevel,FALSE);

	hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,NULL,App.hInstance,0);
	if (hwndCtrl)
		{
		SendMessage(hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
		SendMessage(hwndCtrl,UDM_SETRANGE32,pSynchro->iLevelMin,pSynchro->iLevelMax);
		SendMessage(hwndCtrl,UDM_SETPOS32,0,pSynchro->iLevel);
		}

	CheckDlgButton(hDlg,202,pSynchro->bOnlyEquipped?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(hDlg,203,pSynchro->bOnlyLowerLevels?BST_CHECKED:BST_UNCHECKED);

	SetDlgItemText(hDlg,202,Locale_GetText(TEXT_DIALOG_SYNCHRO_EQUIPPED));
	SetDlgItemText(hDlg,203,Locale_GetText(TEXT_DIALOG_SYNCHRO_LOWERLEVELS));

	SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
	SetFocus(GetDlgItem(hDlg,200));
	return(1);
}


// «»»» Affichage du label ««««««««««««««««««««««««««««««««««««««««««««««»

BOOL Game_SynchronizeAll_Draw(UINT uCtrlId, DRAWITEMSTRUCT *pDraw, void *pDialog)
{
	Dialog_DrawLabel(Locale_GetText(TEXT_DIALOG_SYNCHROLEVEL),pDraw,NULL,DT_RIGHT);
	return(TRUE);
}


// «»»» Validation ««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Game_SynchronizeAll_Ok(HWND hDlg, void *pDialog)
{
	DIALOGSYNCHRO*	pSynchro;
	DOS2ITEM*	pItem;
	int		iLevel;

	//--- Récupère les valeurs ---

	pSynchro = (DIALOGSYNCHRO *)pDialog;
	iLevel = GetDlgItemInt(hDlg,200,NULL,FALSE);
	if (iLevel < pSynchro->iLevelMin)
		{
		SetDlgItemInt(hDlg,200,pSynchro->iLevelMin,FALSE);
		SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
		SetFocus(GetDlgItem(hDlg,200));
		return(0);
		}
	if (iLevel > pSynchro->iLevelMax)
		{
		SetDlgItemInt(hDlg,200,pSynchro->iLevelMax,FALSE);
		SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
		SetFocus(GetDlgItem(hDlg,200));
		return(0);
		}
	pSynchro->bOnlyEquipped = IsDlgButtonChecked(hDlg,202) == BST_CHECKED?TRUE:FALSE;
	pSynchro->bOnlyLowerLevels = IsDlgButtonChecked(hDlg,203) == BST_CHECKED?TRUE:FALSE;

	//--- Modifie les objets de l'inventaire actuel ---

	for (pItem = (DOS2ITEM *)App.Game.pdcCurrent->pdiInventory->nodeItems.next; pItem != NULL; pItem = (DOS2ITEM *)pItem->node.next)
		{
		if (pSynchro->bOnlyEquipped && !Game_IsItemEquipped(pItem)) continue;
		Game_Synchronize_Level(iLevel,pSynchro->bOnlyLowerLevels,pItem->pxaLevel,pItem->pxaIsGenerated,pItem->pxnGeneration,&pItem->pxnLevelOverride);
		}

	//--- Mise à jour de l'affichage ---

	InvalidateRect(App.Game.Layout.hwndInventory,NULL,FALSE);
	return(1);
}

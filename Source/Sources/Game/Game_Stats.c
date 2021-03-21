
//<<>-<>>---------------------------------------------------------------------()
/*
	Statistiques du personnage
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Locale.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Game.h"
#include "XML.h"
#include "Texts.h"

extern APPLICATION	App;
extern GAMEORIGIN	Origins[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Game_Stats(HWND hWnd)
{
	DIALOGSTATS	dialog;

	ZeroMemory(&dialog,sizeof(DIALOGSTATS));

	dialog.pdcCurrent = App.Game.pdcCurrent;
	dialog.pdcList = App.Game.pdcList;
	dialog.bFocus = TRUE;
	dialog.bSetCharts = TRUE;

	dialog.dialogStruct.hwndParent = hWnd;
	dialog.dialogStruct.pszTitle = Locale_GetText(TEXT_BUTTON_STATS);
	dialog.dialogStruct.uResID = 1012;
	dialog.dialogStruct.fnInit = Game_Stats_Init;
	dialog.dialogStruct.fnDraw = Game_Stats_Draw;
	dialog.dialogStruct.fnClick = Game_Stats_Click;

	Dialog_Open((DIALOG *)&dialog);
	return;
}


// «»»» Fonctions de la boîte de dialogue «««««««««««««««««««««««««««««««»

//--- Initialisation de la boîte de dialogue ---

int Game_Stats_Init(HWND hDlg, RECT *rcDialog, int iHeight, void *pDialog)
{
	DIALOGSTATS*	pStats;
	int		i;

	pStats = (DIALOGSTATS *)pDialog;

	SetDlgItemText(hDlg,201,NULL);		// Name
	SetDlgItemText(hDlg,211,NULL);		// Origin
	SetDlgItemText(hDlg,221,NULL);		// Race
	SetDlgItemText(hDlg,231,NULL);		// Class
	SetDlgItemInt(hDlg,241,0,FALSE);	// Level
	SetDlgItemText(hDlg,251,NULL);		// Map
	SetDlgItemText(hDlg,261,szZero);	// Damages
	SetDlgItemText(hDlg,271,szZero);	// Heals
	SetDlgItemText(hDlg,281,szZero);	// Kills

	//--- Information du personnage ---

	if (pStats->pdcCurrent)
		{
		pStats->pxa.pszName = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaName);
		pStats->pxa.pszOrigin = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaOriginName);
		pStats->pxa.pszRace = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaRace);
		pStats->pxa.pszClass = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaClass);
		pStats->pxa.pszMap = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaMap);
		pStats->pxa.pszExp = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaExp);
		pStats->pxa.pszDamages = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaDamageCount);
		pStats->pxa.pszHeals = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaHealCount);
		pStats->pxa.pszKills = xml_GetThisAttrValue(((DOS2CHARACTER *)pStats->pdcCurrent)->pxaKillCount);

		//--- Nom

		if (pStats->pxa.pszName) SetDlgItemText(hDlg,201,pStats->pxa.pszName);

		//--- Origines

		if (pStats->pxa.pszOrigin)
			{
			WCHAR* pszOrigin = Game_Stats_GetOriginName(pStats->pxa.pszOrigin);
			if (pszOrigin) SetDlgItemText(hDlg,211,pszOrigin);
			}

		//--- Race

		if (pStats->pxa.pszRace)
			{
			WCHAR*		pszBuffer;
			WCHAR*		pszRaceName;
			DWORD_PTR	vl[1];

			vl[0] = (DWORD_PTR)pStats->pxa.pszRace;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szRaceFmt,0,0,(LPTSTR)&pszBuffer,1,(va_list *)vl);
			if (pszBuffer)
				{
				CharUpper(pszBuffer);
				pszRaceName = Locale_GetText(Locale_GetTextUID(App.pLocaleTexts,pszBuffer));
				if (pszRaceName) SetDlgItemText(hDlg,221,pszRaceName);
				LocalFree(pszBuffer);
				}
			}

		//--- Classe

		if (pStats->pxa.pszClass)
			{
			WCHAR*		pszBuffer;
			WCHAR*		pszClassName;
			DWORD_PTR	vl[1];

			vl[0] = (DWORD_PTR)pStats->pxa.pszClass;
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szClassFmt,0,0,(WCHAR *)&pszBuffer,1,(va_list *)vl))
				{
				CharUpper(pszBuffer);
				pszClassName = Locale_GetText(Locale_GetTextUID(App.pLocaleTexts,pszBuffer));
				if (pszClassName) SetDlgItemText(hDlg,231,pszClassName);
				LocalFree(pszBuffer);
				}
			}

		//--- Niveau, Carte, Dégâts, etc.

		if (pStats->pxa.pszExp) SetDlgItemInt(hDlg,241,Game_GetLevelFromExp(wcstol(pStats->pxa.pszExp,NULL,10)),FALSE);
		if (pStats->pxa.pszMap) SetDlgItemText(hDlg,251,pStats->pxa.pszMap);
		if (pStats->pxa.pszDamages) Game_Stats_SetNumber(hDlg,261,pStats->pxa.pszDamages);
		if (pStats->pxa.pszHeals) Game_Stats_SetNumber(hDlg,271,pStats->pxa.pszHeals);
		if (pStats->pxa.pszKills) Game_Stats_SetNumber(hDlg,281,pStats->pxa.pszKills);

		//--- Arrows

		EnableWindow(GetDlgItem(hDlg,700),((DOS2CHARACTER *)pStats->pdcCurrent)->node.prev->prev?TRUE:FALSE);
		EnableWindow(GetDlgItem(hDlg,701),((DOS2CHARACTER *)pStats->pdcCurrent)->node.next?TRUE:FALSE);
		}

	//--- Liste ---

	if (pStats->bSetCharts)
		{
		DOS2CHARACTER*	pdc;
		LVCOLUMN	lvColumn;
		LVGROUP		lvGroup;
		static int	iColsWidths[2] = { 300, 120 };
		static int	iColsFmt[2] = { LVCFMT_LEFT, LVCFMT_RIGHT };
		static UINT	uColsTexts[2] = { TEXT_CHR_LABEL, TEXT_CHR_VALUE };
		static UINT	uGroupsTitles[] = { TEXT_CHR_DAMAGES, TEXT_CHR_HEAL, TEXT_CHR_KILLS, 0 };

		SendDlgItemMessage(hDlg,400,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);

		for (i = 0; i != 2; i++)
			{
			lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
			lvColumn.fmt = iColsFmt[i];
			lvColumn.cx = iColsWidths[i];
			lvColumn.pszText = Locale_GetText(uColsTexts[i]);
			lvColumn.cchTextMax = wcslen(lvColumn.pszText);
			lvColumn.iOrder = i;
			if (SendDlgItemMessage(hDlg,400,LVM_INSERTCOLUMN,i,(LPARAM)&lvColumn) == -1) return(0);
			}

		for (i = 0; uGroupsTitles[i] != 0; i++)
			{
			lvGroup.cbSize = sizeof(LVGROUP);
			lvGroup.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_STATE;
			lvGroup.pszHeader = Locale_GetText(uGroupsTitles[i]);
			lvGroup.iGroupId = uGroupsTitles[i];
			lvGroup.stateMask = lvGroup.state = LVGS_COLLAPSIBLE|LVGS_COLLAPSED;
			lvGroup.state &= ~LVGS_COLLAPSED;
			if (SendDlgItemMessage(hDlg,400,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) return(0);
			}

		for (pdc = pStats->pdcList, i = 0; pdc != NULL; pdc = (DOS2CHARACTER *)pdc->node.next)
			{
			WCHAR*	pszName;
			WCHAR*	pszOrigin;

			pszName = xml_GetThisAttrValue(pdc->pxaName);
			pszOrigin = xml_GetThisAttrValue(pdc->pxaOriginName);
			Game_Stats_SetItem(hDlg,400,i++,pszName,pszOrigin,xml_GetThisAttrValue(pdc->pxaDamageCount),TEXT_CHR_DAMAGES);
			Game_Stats_SetItem(hDlg,400,i++,pszName,pszOrigin,xml_GetThisAttrValue(pdc->pxaHealCount),TEXT_CHR_HEAL);
			Game_Stats_SetItem(hDlg,400,i++,pszName,pszOrigin,xml_GetThisAttrValue(pdc->pxaKillCount),TEXT_CHR_KILLS);
			}

		SendDlgItemMessage(hDlg,400,LVM_ENABLEGROUPVIEW,(WPARAM)TRUE,0);
		SendDlgItemMessage(hDlg,400,LVM_SORTITEMS,(WPARAM)0,(LPARAM)Game_Stats_Sort);
		pStats->bSetCharts = FALSE;
		}

	//--- Keyboard focus to OK button ---

	if (pStats->bFocus)
		{
		SetFocus(GetDlgItem(hDlg,IDOK));
		pStats->bFocus = FALSE;
		}

	return(1);
}

//--- Affichage ---

BOOL Game_Stats_Draw(UINT uCtrlId, DRAWITEMSTRUCT *pDraw, void *pDialog)
{
	switch(uCtrlId)
		{
		case 200:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_NAME),pDraw,DT_RIGHT);
			break;
		case 210:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_ORIGIN),pDraw,DT_RIGHT);
			break;
		case 220:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_RACE),pDraw,DT_RIGHT);
			break;
		case 230:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_CLASS),pDraw,DT_RIGHT);
			break;
		case 240:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_LEVEL),pDraw,DT_RIGHT);
			break;
		case 250:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_MAP),pDraw,DT_RIGHT);
			break;
		case 260:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_DAMAGES),pDraw,DT_RIGHT);
			break;
		case 270:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_HEAL),pDraw,DT_RIGHT);
			break;
		case 280:
			Dialog_DrawLabel(Locale_GetText(TEXT_CHR_KILLS),pDraw,DT_RIGHT);
			break;
		case 700:
			Dialog_DrawArrowButton(DFCS_SCROLLLEFT,pDraw);
			break;
		case 701:
			Dialog_DrawArrowButton(DFCS_SCROLLRIGHT,pDraw);
			break;
		}

	return(TRUE);
}

//--- Changement de personnage ---

void Game_Stats_Click(HWND hDlg, UINT uCtlID, void *pDialog)
{
	DIALOGSTATS*	pStats;

	pStats = (DIALOGSTATS *)pDialog;

	switch(uCtlID)
		{
		case 700:
			pStats->pdcCurrent = ((DOS2CHARACTER *)pStats->pdcCurrent)->node.prev;
			Game_Stats_Init(hDlg,NULL,0,pDialog);
			break;
		case 701:
			pStats->pdcCurrent = ((DOS2CHARACTER *)pStats->pdcCurrent)->node.next;
			Game_Stats_Init(hDlg,NULL,0,pDialog);
			break;
		}

	return;
}

//--- Retrouve le nom de l'origine ---

WCHAR* Game_Stats_GetOriginName(WCHAR *pszOrigin)
{
	if (pszOrigin)
		{
		int	i;

		if (!wcsncmp(szGeneric,pszOrigin,7)) return(Locale_GetText(TEXT_CHR_NOORIGIN));

		for (i = 0; Origins[i].pszName != NULL; i++)
			{
			if (wcscmp(pszOrigin,Origins[i].pszName)) continue;
			return(Locale_GetText(Origins[i].uLocaleId));
			}
		}

	return(NULL);
}

//--- Nombre formaté ---

void Game_Stats_SetNumber(HWND hDlg, UINT uCtlID, WCHAR *pszNumber)
{
	WCHAR *pszFmt = Misc_FormatNumber(pszNumber);

	if (pszFmt)
		{
		SetDlgItemText(hDlg,uCtlID,pszFmt);
		HeapFree(App.hHeap,0,pszFmt);
		}
	else SetDlgItemText(hDlg,uCtlID,pszNumber);

	return;
}


//--- Ajoute un personnage dans la liste ---

int Game_Stats_SetItem(HWND hDlg, UINT uCtlID, int iItem, WCHAR *pszName, WCHAR *pszOrigin, WCHAR *pszValue, UINT uLocaleID)
{
	LVITEM		lvItem;
	WCHAR*		pszFmt;

	pszFmt = Misc_FormatNumber(pszValue);

	//--- Character ---

	lvItem.mask = LVIF_PARAM|LVIF_TEXT|LVIF_GROUPID;
	lvItem.iItem = iItem;
	lvItem.iSubItem = 0;
	lvItem.pszText = pszName?pszName:Game_Stats_GetOriginName(pszOrigin);
	lvItem.iGroupId = uLocaleID;
	lvItem.lParam = (LPARAM)wcstol(pszValue,NULL,10);
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) return(0);

	//--- Value ---

	lvItem.iSubItem = 1;
	lvItem.pszText = pszFmt?pszFmt:(pszValue?pszValue:szZero);
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == -1) return(0);

	if (pszFmt) HeapFree(App.hHeap,0,pszFmt);
	return(1);
}

//--- Classement ---

int Game_Stats_Sort(LPARAM lValue1, LPARAM lValue2, LPARAM lSort)
{
	return(lValue2-lValue1);
}

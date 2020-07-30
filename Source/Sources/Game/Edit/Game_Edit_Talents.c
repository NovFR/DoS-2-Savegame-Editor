
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Talents

	BUG: Keyboard navigation is disturbed when selected the last
	item in a group. See Game_TalentsCheck().
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Divine.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION	App;

// NOTE: Divinity Original Sin 2 flags ARE differents

static GAMETALENT	GameTalents[] = {	{ 0x00000008, 0 },				// Opportuniste
						{ 0x00100000, 0 },				// Revenant
						{ 0x02000000, 0 },				// Taille XXL
						{ 0x04000000, 0 },				// Super compétent
						{ 0x00000200, 1 },				// Ami des bêtes
						{ 0x00000400, 1 },				// Maître de l'évasion
						{ 0x00001000, 1 },				// Guérilla
						{ 0x00004000, 1 },				// Lêve-tôt
						{ 0x00010000, 1 },				// Canon de verre
						{ 0x00040000, 1 },				// Empressement
						{ 0x00080000, 1 },				// En retrait
						{ 0x00100000, 1 },				// Sangsue
						{ 0x00200000, 1 },				// Affinité élémentaire
						{ 0x00400000, 1 },				// Dîner cinq étoiles
						{ 0x01000000, 1 },				// Ranger élémentaire
						{ 0x10000000, 1 },				// Loup solitaire
						{ 0x20000000, 1, GAME_TALENT_UNDEAD },		// Mort-vivant
						{ 0x40000000, 1 },				// Démon
						{ 0x80000000, 1 },				// Complètement givré
						{ 0x00000004, 2 },				// Fortitude
						{ 0x00000020, 2 },				// Puanteur
						{ 0x00000100, 2 },				// Santé de fer
						{ 0x00000400, 2 },				// Trouve-flèches
						{ 0x00020000, 2 },				// Jeu du mouchoir
						{ 0x00040000, 2 },				// Fronde
						{ 0x00200000, 2 },				// Maître de la parade
						{ 0x00400000, 2, GAME_TALENT_HUMAN },		// Ingéniosité
						{ 0x00800000, 2, GAME_TALENT_HUMAN },		// Econome
						{ 0x01000000, 2, GAME_TALENT_ELF },		// Savoir ancestral
						{ 0x02000000, 2, GAME_TALENT_ELF },		// Mangeur de cadavres
						{ 0x04000000, 2, GAME_TALENT_DWARF },		// Robuste
						{ 0x08000000, 2, GAME_TALENT_DWARF },		// Ruse de nain
						{ 0x10000000, 2, GAME_TALENT_LIZARD },		// Sophistiqué
						{ 0x20000000, 2, GAME_TALENT_LIZARD },		// Chantesort
						{ 0x40000000, 2 },				// Tête brûlée
						{ 0x80000000, 2 },				// Bourreau
						{ 0x00000001, 3 },				// Sortilège sauvage
						{ 0x00000002, 3 },				// Pion
//						{ 0x00000000, 3 },				// Baiser de l'araignée [For +2 | Con -2] (Towering Oak)
//						{ 0x00000000, 3 },				// Baiser de l'araignée [Int +2 | Con -2] (Curved Quill)
//						{ 0x00000000, 3 },				// Baiser de l'araignée [ 2000G | Con -2] (Golden Chest)
						{ 0x00000010, 3 },				// Baiser de l'araignée [Dex +2 | Con -2] (Stately Dragon)
//						{ 0x00000000, 3 },				// Baiser de l'araignée [  Idol | Con -2] (Transparent Chrysalis)
						{ 0x00000040, 3 },				// Mnémonique
						{ 0x00000080, 3 },				// Secrets professionnels
						{ 0x00000400, 3 },				// Armure vivante
						{ 0x00000800, 3 },				// Tortionnaire
						{ 0x00001000, 3 },				// Ambidextrie
						{ 0x00002000, 3 },				// Instable
						{ 0 }
					};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la boîte de dialogue ««««««««««««««««««««««««««««««««»

void Game_Talents()
{
	GAMETALENTSCONTEXT*	pContext;
	INT_PTR			iResult;
	int			i;

	#if !_DEBUG
	if (!App.Game.pdcCurrent) return;
	#endif
	if (App.Config.uGame == DIVINE_DOS_2)
		{
		MessageBox(App.hWnd,Locale_GetText(TEXT_ERR_NOTCOMPATIBLE),NULL,MB_ICONERROR|MB_OK);
		return;
		}

	//--- Alloue la structure ---

	for (i = 0; GameTalents[i].dwFlag != 0; i++);

	pContext = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMETALENTSCONTEXT)+sizeof(GAMETALENTSTRUCT)*i);
	if (!pContext)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return;
		}

	//--- Copie et traduit les talents ---

	if (!Locale_Load(App.hWnd,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_MISC,(void **)&pContext->pLocale,NULL))
		{
		HeapFree(App.hHeap,0,pContext);
		return;
		}

	pContext->uNumTalents = i;

	for (i = 0; GameTalents[i].dwFlag != 0; i++)
		{
		pContext->Talents[i].Flag.dwFlag = GameTalents[i].dwFlag;
		pContext->Talents[i].Flag.uGroup = GameTalents[i].uGroup;
		pContext->Talents[i].Flag.uRacial = GameTalents[i].uRacial;
		Locale_QueryTalent(pContext->pLocale->db,GameTalents[i].dwFlag,GameTalents[i].uGroup,&pContext->Talents[i].pszName,&pContext->Talents[i].pszDescription);
		}

	Locale_Unload(LOCALE_TYPE_MISC,(void **)&pContext->pLocale,NULL);

	//--- Copie les talents du personnage ---

	if (App.Game.pdcCurrent)
		{
		for (i = 0; i != 4; i++)
			{
			pContext->pszValue = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaTalents[i]);
			if (pContext->pszValue) pContext->dwTalents[i] = wcstoul(pContext->pszValue,NULL,10);
			else pContext->dwTalents[i] = 0;
			pContext->dwUnknownTalents[i] = pContext->dwTalents[i];
			}

		// Set flags of unknown talents
		for (i = 0; GameTalents[i].dwFlag != 0; i++)
			pContext->dwUnknownTalents[GameTalents[i].uGroup] &= ~GameTalents[i].dwFlag;
		}

	//--- Affichage de la boîte de dialogue ---

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1009),App.hWnd,Game_TalentsProc,(LPARAM)pContext);
	if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);

	//--- Applique les talents au personnage ---

	if (iResult && App.Game.pdcCurrent)
		{
		for (i = 0; i != 4; i++)
			{
			if (!App.Game.pdcCurrent->pxaTalents[i]) continue;
			wsprintf(pContext->szTemp,L"%u",pContext->dwTalents[i]);
			xml_SetAttrValue(App.Game.pdcCurrent->pxaTalents[i],pContext->szTemp);
			}
		}

	//--- Terminé ---

	for (i = 0; GameTalents[i].dwFlag != 0; i++)
		{
		if (pContext->Talents[i].pszName) HeapFree(App.hHeap,0,pContext->Talents[i].pszName);
		if (pContext->Talents[i].pszDescription) HeapFree(App.hHeap,0,pContext->Talents[i].pszDescription);
		}

	HeapFree(App.hHeap,0,pContext);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_TalentsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMETALENTSCONTEXT*	pContext;

	if (uMsgId == WM_INITDIALOG)
		{
		static UINT	Groups[] = { TEXT_TALENTS_SET, TEXT_TALENTS_UNSET, TEXT_TALENTS_UNKNOWN,0 };
		LVGROUP		lvGroup;
		LVCOLUMN	lvColumn;
		LVITEM		lvItem;
		RECT		rcDialog;
		DWORD		dwMask;
		int		Height;
		int		i;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pContext = (GAMETALENTSCONTEXT *)lParam;

		Height = Dialog_GetInfoHeight(hDlg,100,Locale_GetText(TEXT_TALENTS_INFO));
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+Height,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,Height,SWP_NOZORDER|SWP_NOMOVE);

		SendDlgItemMessage(hDlg,200,LVM_SETEXTENDEDLISTVIEWSTYLE,(WPARAM)LVS_EX_CHECKBOXES|LVS_EX_INFOTIP|LVS_EX_DOUBLEBUFFER,(LPARAM)LVS_EX_CHECKBOXES|LVS_EX_INFOTIP|LVS_EX_DOUBLEBUFFER);
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
		lvItem.iSubItem = 0;
		for (lvItem.iItem = 0; lvItem.iItem != pContext->uNumTalents; lvItem.iItem++)
			{
			lvItem.pszText = pContext->Talents[lvItem.iItem].pszName?pContext->Talents[lvItem.iItem].pszName:Locale_GetText(TEXT_UNKNOWN);
			lvItem.iGroupId = (pContext->dwTalents[pContext->Talents[lvItem.iItem].Flag.uGroup]&pContext->Talents[lvItem.iItem].Flag.dwFlag)?0:1;
			lvItem.lParam = (LPARAM)&pContext->Talents[lvItem.iItem];
			if (SendDlgItemMessage(hDlg,200,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1)
				{
				EndDialog(hDlg,-1);
				return(FALSE);
				}
			lvItem.state = INDEXTOSTATEIMAGEMASK((pContext->dwTalents[pContext->Talents[lvItem.iItem].Flag.uGroup]&pContext->Talents[lvItem.iItem].Flag.dwFlag)?2:1);
			lvItem.stateMask = LVIS_STATEIMAGEMASK;
			SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
			}
		for (i = 0; i != 4; i++)
			{
			for (dwMask = 1; dwMask != 0; dwMask = dwMask<<1)
				{
				if (pContext->dwUnknownTalents[i]&dwMask)
					{
					wsprintf(pContext->szTemp,L"0x%.8X (%u)",dwMask,i);
					lvItem.pszText = pContext->szTemp;
					lvItem.iGroupId = 2;
					lvItem.lParam = 0;
					if (SendDlgItemMessage(hDlg,200,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1)
						{
						EndDialog(hDlg,-1);
						return(FALSE);
						}
					lvItem.state = INDEXTOSTATEIMAGEMASK(2);
					lvItem.stateMask = LVIS_STATEIMAGEMASK;
					SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
					lvItem.iItem++;
					}
				}
			}

		SendDlgItemMessage(hDlg,200,LVM_SETCOLUMNWIDTH,0,(LPARAM)LVSCW_AUTOSIZE);
		SendDlgItemMessage(hDlg,200,LVM_ENABLEGROUPVIEW,(WPARAM)TRUE,0);
		SendDlgItemMessage(hDlg,200,LVM_SORTITEMS,(WPARAM)pContext,(LPARAM)(PFNLVCOMPARE)Game_TalentsSort);
		pContext->hwndTooltip = (HWND)SendDlgItemMessage(hDlg,200,LVM_GETTOOLTIPS,0,0);
		if (pContext->hwndTooltip)
			{
			LONG_PTR	lptrStyles;
			lptrStyles = GetWindowLongPtr(pContext->hwndTooltip,GWL_STYLE);
			SetWindowLongPtr(pContext->hwndTooltip,GWL_STYLE,lptrStyles|TTS_BALLOON|TTS_NOANIMATE|TTS_NOFADE);
			SendMessage(pContext->hwndTooltip,TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,(LPARAM)1200000);
			}
		Dialog_OffsetY(hDlg,200,Height);
		Dialog_OffsetY(hDlg,IDOK,Height);
		Dialog_OffsetY(hDlg,IDCANCEL,Height);

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
		SetWindowText(hDlg,Locale_GetText(TEXT_BUTTON_TALENTS));
		Dialog_CenterWindow(hDlg,App.hWnd);
		pContext->bInitialized = TRUE;
		SetFocus(GetDlgItem(hDlg,200));
		return(FALSE);
		}

	pContext = (GAMETALENTSCONTEXT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(Locale_GetText(TEXT_TALENTS_INFO),(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
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
							Game_TalentsCheck(hDlg,((NMITEMACTIVATE *)lParam)->iItem,TRUE,pContext);
							return(TRUE);
						}
					break;
				case LVN_ITEMCHANGED:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 200:
							Game_TalentsCheck(hDlg,((NMLISTVIEW *)lParam)->iItem,FALSE,pContext);
							return(TRUE);
						}
					break;
				case LVN_GETINFOTIP:
					switch(((NMHDR *)lParam)->idFrom)
						{
						case 200:
							Game_TalentsTip(hDlg,(NMLVGETINFOTIP *)lParam,pContext);
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


// «»»» Tri des talents «««««««««««««««««««««««««««««««««««««««««««««««««»

int CALLBACK Game_TalentsSort(GAMETALENTSTRUCT *pTalent1, GAMETALENTSTRUCT *pTalent2, GAMETALENTSCONTEXT *pContext)
{
	if (pTalent1 && !pTalent2) return(-1);
	else if (!pTalent1 && pTalent2) return(1);
	else if (!pTalent1 && !pTalent2) return(0);

	int iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pTalent1->pszName,-1,pTalent2->pszName,-1,NULL,NULL,0);
	if (iResult == CSTR_LESS_THAN) return(-1);
	if (iResult == CSTR_GREATER_THAN) return(1);
	return(0);
}


// «»»» Copie la description d'un talent ««««««««««««««««««««««««««««««««»

void Game_TalentsTip(HWND hDlg, NMLVGETINFOTIP *pTip, GAMETALENTSCONTEXT *pContext)
{
	LVITEM		lvItem;

	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = pTip->iItem;
	lvItem.lParam = 0;
	SendDlgItemMessage(hDlg,200,LVM_GETITEM,0,(LPARAM)&lvItem);
	if (!lvItem.lParam) return;

	if (((GAMETALENTSTRUCT *)lvItem.lParam)->pszDescription)
		{
		if (pContext->hwndTooltip)
			{
			SendMessage(pContext->hwndTooltip,TTM_SETTITLE,1,(LPARAM)(((GAMETALENTSTRUCT *)lvItem.lParam)->pszName));
			Misc_StrCpy(pTip->pszText,pTip->cchTextMax,((GAMETALENTSTRUCT *)lvItem.lParam)->pszDescription);
			}
		else
			{
			Misc_StrCpy(pTip->pszText,pTip->cchTextMax,((GAMETALENTSTRUCT *)lvItem.lParam)->pszName);
			Misc_StrCat(pTip->pszText,pTip->cchTextMax,szLF);
			Misc_StrCat(pTip->pszText,pTip->cchTextMax,szLF);
			Misc_StrCat(pTip->pszText,pTip->cchTextMax,((GAMETALENTSTRUCT *)lvItem.lParam)->pszDescription);
			}
		}

	return;
}


// «»»» Active/Désactive un talent ««««««««««««««««««««««««««««««««««««««»

void Game_TalentsCheck(HWND hDlg, int iItem, BOOL bToggle, GAMETALENTSCONTEXT *pContext)
{
	GAMETALENTSTRUCT*	pTalent;
	LVITEM			lvItem;
	DWORD			dwState;
	int			iNewGroupId;

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

	//--- Empêche la modification des talents inconnus ---

	if (lvItem.iGroupId == 2)
		{
		if (lvItem.state & INDEXTOSTATEIMAGEMASK(1))
			{
			lvItem.state = INDEXTOSTATEIMAGEMASK(2);
			lvItem.stateMask = LVIS_STATEIMAGEMASK;
			SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
			}
		return;
		}

	//--- Modifie le talent ---

	pTalent = (GAMETALENTSTRUCT *)lvItem.lParam;
	dwState = pTalent->Flag.dwFlag;
	if (lvItem.state & INDEXTOSTATEIMAGEMASK(1)) dwState = 0;

	pContext->dwTalents[pTalent->Flag.uGroup] &= ~pTalent->Flag.dwFlag;
	pContext->dwTalents[pTalent->Flag.uGroup] |= dwState;

	//--- Bascule l'état du talent ---

	if (bToggle)
		{
		if (dwState) dwState = 0;
		else dwState = pTalent->Flag.dwFlag;
		lvItem.state = INDEXTOSTATEIMAGEMASK(dwState?2:1);
		lvItem.stateMask = LVIS_STATEIMAGEMASK;
		SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvItem.iItem,(LPARAM)&lvItem);
		return;
		}

	//--- Déplace le talent dans un nouveau groupe ---

	iNewGroupId = dwState?0:1;
	if (iNewGroupId != lvItem.iGroupId)
		{
		LVITEM	lvNextItem;
		lvNextItem.state = LVIS_FOCUSED;
		lvNextItem.stateMask = LVIS_FOCUSED;
		lvNextItem.iItem = SendDlgItemMessage(hDlg,200,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)LVNI_BELOW);
		//if (lvNextItem.iItem == -1) SendDlgItemMessage(hDlg,200,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)LVNI_ABOVE);
		if (lvNextItem.iItem != -1)
			{
			SendDlgItemMessage(hDlg,200,LVM_SETITEMSTATE,(WPARAM)lvNextItem.iItem,(LPARAM)&lvNextItem);
			SendDlgItemMessage(hDlg,200,LVM_ENSUREVISIBLE,(WPARAM)lvNextItem.iItem,(LPARAM)FALSE);
			}
		lvItem.mask = LVIF_GROUPID;
		lvItem.iGroupId = iNewGroupId;
		SendDlgItemMessage(hDlg,200,LVM_SETITEM,0,(LPARAM)&lvItem);
		SendDlgItemMessage(hDlg,200,LVM_SORTITEMS,(WPARAM)pContext,(LPARAM)(PFNLVCOMPARE)Game_TalentsSort);
		}

	return;
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Modification d'une valeur
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Texts.h"
#include "Locale.h"

extern APPLICATION	App;

static GAMEEDITFILTER	GameBoostersFilters[] = {
							{ 231, FILTER_ARMORS_ANY, TEXT_DIALOG_FILTER_ARMORS_ANY },
							{ 231, FILTER_ARMORS_AMULET, TEXT_DIALOG_FILTER_ARMORS_AMULET },
							{ 231, FILTER_ARMORS_RING, TEXT_DIALOG_FILTER_ARMORS_RING },
							{ 231, FILTER_ARMORS_BOOTS, TEXT_DIALOG_FILTER_ARMORS_BOOTS },
							{ 231, FILTER_ARMORS_HELMET, TEXT_DIALOG_FILTER_ARMORS_HELMET },
							{ 231, FILTER_ARMORS_BELT, TEXT_DIALOG_FILTER_ARMORS_BELT },
							{ 231, FILTER_ARMORS_GLOVES, TEXT_DIALOG_FILTER_ARMORS_GLOVES },
							{ 231, FILTER_ARMORS_PANTS, TEXT_DIALOG_FILTER_ARMORS_PANTS },
							{ 231, FILTER_ARMORS_UPPERBODY, TEXT_DIALOG_FILTER_ARMORS_UPPERBODY },
							{ 231, FILTER_SHIELDS, TEXT_DIALOG_FILTER_SHIELDS },
							{ 231, FILTER_WEAPONS_ANY, TEXT_DIALOG_FILTER_WEAPONS_ANY },
							{ 241, FILTER_ABILITIES, TEXT_DIALOG_FILTER_ABILITIES },
							{ 241, FILTER_MAGICAL_ARMOR, TEXT_DIALOG_FILTER_MAGICAL_ARMOR },
							{ 241, FILTER_PHYSICAL_ARMOR, TEXT_DIALOG_FILTER_PHYSICAL_ARMOR },
							{ 241, FILTER_RUNE_SLOT, TEXT_DIALOG_FILTER_RUNE_SLOT },
							{ 241, FILTER_PRIMARY_ATTRIBUTE, TEXT_DIALOG_FILTER_PRIMARY_ATTRIBUTE },
							{ 241, FILTER_SECONDARY_ATTRIBUTE, TEXT_DIALOG_FILTER_SECONDARY_ATTRIBUTE },
							{ 241, FILTER_RESISTANCE, TEXT_DIALOG_FILTER_RESISTANCE },
							{ 241, FILTER_SKILL, TEXT_DIALOG_FILTER_SKILL },
							{ 241, FILTER_IMMUNITY, TEXT_DIALOG_FILTER_IMMUNITY },
							{ 241, FILTER_REFLECT, TEXT_DIALOG_FILTER_REFLECT },
							{ 241, FILTER_SPECIAL, TEXT_DIALOG_FILTER_SPECIAL },
							{ 241, FILTER_DAMAGE, TEXT_DIALOG_FILTER_DAMAGE },
							{ 241, FILTER_LIFESTEAL, TEXT_DIALOG_FILTER_LIFESTEAL },
							{ 241, FILTER_STATUS, TEXT_DIALOG_FILTER_STATUS },
							{ 0 }
						};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification d'une valeur numérique				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage de la boîte de dialogue «««««««««««««««««««««««««««««««»

int Game_EditSetValue(HWND hWnd, WCHAR *pszLabel, XML_ATTR* pxa, int iMin, int iMax)
{
	DIALOGVALUE	Value;
	DWORD_PTR	vl[2];

	ZeroMemory(&Value,sizeof(DIALOGVALUE));

	vl[0] = (DWORD_PTR)iMin;
	vl[1] = (DWORD_PTR)iMax;
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,Locale_GetText(TEXT_DIALOG_INFO_SETVALUE),0,0,(WCHAR *)&Value.dialogStruct.pszText,1,(va_list *)vl))
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return(0);
		}

	Value.dialogStruct.hwndParent = hWnd;
	Value.dialogStruct.pszTitle = Locale_GetText(TEXT_DIALOG_TITLE_SETVALUE);
	Value.dialogStruct.uResID = 1002;
	Value.dialogStruct.fnInit = Game_EditSetValue_Init;
	Value.dialogStruct.fnDraw = Game_EditSetValue_Draw;
	Value.dialogStruct.fnOk = Game_EditSetValue_Ok;

	Value.pszLabel = pszLabel;
	Value.iValue = wcstol(pxa->value,NULL,10);
	Value.iValueMin = iMin;
	Value.iValueMax = iMax;

	if (Dialog_Open((DIALOG *)&Value))
		{
		WCHAR*	pszTemp;
		UINT	uLen;

		pszTemp = NULL;
		uLen = Misc_uIntToWCHAR(NULL,Value.iValue);
		pszTemp = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
		if (pszTemp)
			{
			Misc_uIntToWCHAR(pszTemp,Value.iValue);
			if (pxa->value) HeapFree(App.hHeap,0,pxa->value);
			pxa->value = pszTemp;
			return(1);
			}
		else Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		}

	LocalFree(Value.dialogStruct.pszText);
	return(0);
}


// «»»» Fonctions de la boîte de dialogue «««««««««««««««««««««««««««««««»

//--- Initialisation de la boîte de dialogue ---

int Game_EditSetValue_Init(HWND hDlg, RECT *rcDialog, int iHeight, void *pDialog)
{
	DIALOGVALUE*	pValue;
	HWND		hwndCtrl;

	pValue = (DIALOGVALUE *)pDialog;
	Dialog_OffsetY(hDlg,200,iHeight);
	Dialog_OffsetY(hDlg,201,iHeight);
	SetDlgItemInt(hDlg,200,pValue->iValue,FALSE);

	hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,NULL,App.hInstance,0);
	if (hwndCtrl)
		{
		SendMessage(hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
		SendMessage(hwndCtrl,UDM_SETRANGE32,pValue->iValueMin,pValue->iValueMax);
		SendMessage(hwndCtrl,UDM_SETPOS32,0,pValue->iValue);
		}

	SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
	SetFocus(GetDlgItem(hDlg,200));
	return(1);
}

//--- Affichage du label ---

BOOL Game_EditSetValue_Draw(UINT uCtrlId, DRAWITEMSTRUCT *pDraw, void *pDialog)
{
	Dialog_DrawLabel(((DIALOGVALUE *)pDialog)->pszLabel,pDraw,DT_RIGHT);
	return(TRUE);
}

//--- Validation ---

int Game_EditSetValue_Ok(HWND hDlg, void *pDialog)
{
	DIALOGVALUE*	pValue;

	pValue = (DIALOGVALUE *)pDialog;
	pValue->iValue = GetDlgItemInt(hDlg,200,NULL,FALSE);
	if (pValue->iValue < pValue->iValueMin)
		{
		SetDlgItemInt(hDlg,200,pValue->iValueMin,FALSE);
		SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
		SetFocus(GetDlgItem(hDlg,200));
		return(0);
		}
	if (pValue->iValue > pValue->iValueMax)
		{
		SetDlgItemInt(hDlg,200,pValue->iValueMax,FALSE);
		SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
		SetFocus(GetDlgItem(hDlg,200));
		return(0);
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Modification d'une valeur texte					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// Extra parameters:
//
//	DATA_TYPE_BOOSTERS:
//		WCHAR* > Identifier of the object (may be NULL)
//		UINT64* > Pointer to filter flags (may be NULL)
//
//	DATA_TYPE_RUNES:
//		WCHAR* > Identifier of the object (may be NULL)
//
//	DATA_TYPE_TAGS:
//		none

WCHAR* Game_EditValue(HWND hWnd, WCHAR *pszValue, UINT uType, ...)
{
	GAMEEDITVALUE*	pValue;
	WCHAR*		pszResult;
	INT_PTR		iResult;
	va_list		vl;

	pValue = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEEDITVALUE));
	if (!pValue)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return(NULL);
		}
	pValue->hwndParent = hWnd;
	pValue->uType = uType;
	pValue->pszValue = pszValue;
	pValue->uFilter = FILTER_ALL;
	pValue->uSort = DATA_SORT_TEXT;

	va_start(vl,uType);

	switch(uType)
		{
		case DATA_TYPE_BOOSTERS:
			pValue->pszTitle = Locale_GetText(pszValue?TEXT_DIALOG_BOOSTER_TITLE_EDIT:TEXT_DIALOG_BOOSTER_TITLE_ADD);
			pValue->pszFileName = szBoostersDataPath;
			pValue->pszObject = va_arg(vl,WCHAR *);
			pValue->pFilter = va_arg(vl,UINT64 *);
			pValue->uResId = 1003;
			if (pValue->pFilter) pValue->uFilter = *pValue->pFilter;
			break;
		case DATA_TYPE_RUNES:
			pValue->pszTitle = Locale_GetText(pszValue?TEXT_DIALOG_RUNE_TITLE_EDIT:TEXT_DIALOG_RUNE_TITLE_ADD);
			pValue->pszFileName = szRunesDataPath;
			pValue->pszObject = va_arg(vl,WCHAR *);
			pValue->uResId = 1004;
			pValue->uSort = DATA_SORT_RUNE;
			break;
		case DATA_TYPE_TAGS:
			pValue->pszTitle = Locale_GetText(pszValue?TEXT_DIALOG_TAG_TITLE_EDIT:TEXT_DIALOG_TAG_TITLE_ADD);
			pValue->pszFileName = szTagsDataPath;
			pValue->uResId = 1005;
			break;
		default:va_end(vl);
			HeapFree(App.hHeap,0,pValue);
			return(NULL);
		}

	va_end(vl);

	if (!Game_LoadDataFile(hWnd,pValue->pszFileName,uType,&pValue->nodeData))
		{
		HeapFree(App.hHeap,0,pValue);
		return(NULL);
		}

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(pValue->uResId),hWnd,Game_EditValueProc,(LPARAM)pValue);
	if (iResult == -1) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);

	Game_UnloadDataFile(uType,&pValue->nodeData);

	//if (pValue->pFilter) *pValue->pFilter = pValue->uFilter;

	pszResult = pValue->pszResult;
	HeapFree(App.hHeap,0,pValue);
	return(pszResult);
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_EditValueProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMEEDITVALUE*	pValue;

	if (uMsgId == WM_MEASUREITEM)
		{
		if (((MEASUREITEMSTRUCT *)lParam)->CtlType == ODT_LISTBOX && (wParam == 231 || wParam == 241))
			{
			((MEASUREITEMSTRUCT *)lParam)->itemWidth = 0;
			((MEASUREITEMSTRUCT *)lParam)->itemHeight = App.Font.uFontHeight+4;
			return(TRUE);
			}
		else if (Dialog_ViewComboMeasureItem(200,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pValue = (GAMEEDITVALUE *)lParam;

		switch(pValue->uType)
			{
			//--- Augmentations ---

			case DATA_TYPE_BOOSTERS: {
				static int	ColsWidths[4] = { 300, 280, 100, 100 };
				static UINT	ColsTexts[4] = { TEXT_DIALOG_BOOSTER_COLUMN_ID, TEXT_DIALOG_BOOSTER_COLUMN_TEXT, TEXT_DIALOG_BOOSTER_COLUMN_TYPE, TEXT_DIALOG_BOOSTER_COLUMN_SLOT };

				LVCOLUMN	lvColumn;
				LRESULT		lResult;
				int		i;

				SendDlgItemMessage(hDlg,230,WM_SETTEXT,(WPARAM)0,(LPARAM)Locale_GetText(TEXT_DIALOG_BOOSTER_TARGET));
				SendDlgItemMessage(hDlg,240,WM_SETTEXT,(WPARAM)0,(LPARAM)Locale_GetText(TEXT_DIALOG_BOOSTER_TYPE));
				SendDlgItemMessage(hDlg,300,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);

				//--- Colonnes ---
				lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
				for (i = 0; i != 4; i++)
					{
					lvColumn.fmt = LVCFMT_LEFT;
					lvColumn.cx = ColsWidths[i];
					lvColumn.pszText = Locale_GetText(ColsTexts[i]);
					lvColumn.cchTextMax = wcslen(lvColumn.pszText);
					lvColumn.iOrder = i;
					if (SendDlgItemMessage(hDlg,300,LVM_INSERTCOLUMN,i,(LPARAM)&lvColumn) == -1)
						{
						EndDialog(hDlg,-1);
						return(FALSE);
						}
					}

				//--- Entrées ---
				if (!Game_EditValueBuildList(hDlg,TRUE,pValue))
					{
					EndDialog(hDlg,-1);
					return(FALSE);
					}

				//--- Filtres ---
				for (i = 0; GameBoostersFilters[i].uCtrlID != 0; i++)
					{
					lResult = SendDlgItemMessage(hDlg,GameBoostersFilters[i].uCtrlID,LB_ADDSTRING,0,(LPARAM)&GameBoostersFilters[i]);
					if (lResult == LB_ERR || lResult == LB_ERRSPACE)
						{
						EndDialog(hDlg,-1);
						return(FALSE);
						}
					}

				CheckDlgButton(hDlg,245,App.Config.bBoostersGroups?BST_CHECKED:BST_UNCHECKED);
				SendDlgItemMessage(hDlg,245,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_VIEW_GROUPS));
				Game_EditValueSizeObject(hDlg,777);
				Game_EditValueSetBoostersFilters(hDlg,231,pValue->uFilter);
				Game_EditValueSetBoostersFilters(hDlg,241,pValue->uFilter);
				} break;

			//--- Runes ---

			case DATA_TYPE_RUNES: {
				static int	ColsWidths[4] = { 340, 300, 300, 300 };
				static UINT	ColsTexts[4] = { TEXT_DIALOG_RUNE_COLUMN_NAME, TEXT_DIALOG_RUNE_COLUMN_BONUS1, TEXT_DIALOG_RUNE_COLUMN_BONUS2, TEXT_DIALOG_RUNE_COLUMN_BONUS3 };

				LVCOLUMN	lvColumn;
				UINT		uType;
				int		i;

				SendDlgItemMessage(hDlg,300,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);
				if (!SendDlgItemMessage(hDlg,300,LVM_SETITEMCOUNT,(WPARAM)List_EntryCount(&pValue->nodeData),(LPARAM)LVSICF_NOINVALIDATEALL))
					{
					EndDialog(hDlg,-1);
					return(FALSE);
					}

				//--- Colonnes ---
				uType = Game_GetItemType(pValue->pszObject);
				lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
				for (i = 0; i != 4; i++)
					{
					lvColumn.fmt = LVCFMT_LEFT;
					lvColumn.cx = ColsWidths[i];
					if (uType != DATA_TYPE_IS_ITEM) // Affiche toutes les colonnes en cas d'objet inconnu
						{
						if (i == 1 && uType != DATA_TYPE_IS_WEAPON) lvColumn.cx = 0;
						else if (i == 2 && uType != DATA_TYPE_IS_ARMOR) lvColumn.cx = 0;
						else if (i == 3 && uType != DATA_TYPE_IS_ACCESSORY) lvColumn.cx = 0;
						}
					lvColumn.pszText = Locale_GetText(ColsTexts[i]);
					lvColumn.cchTextMax = wcslen(lvColumn.pszText);
					lvColumn.iOrder = i;
					if (SendDlgItemMessage(hDlg,300,LVM_INSERTCOLUMN,i,(LPARAM)&lvColumn) == -1)
						{
						EndDialog(hDlg,-1);
						return(FALSE);
						}
					}

				//--- Entrées ---
				if (!Game_EditValueBuildList(hDlg,TRUE,pValue))
					{
					EndDialog(hDlg,-1);
					return(FALSE);
					}

				CheckDlgButton(hDlg,100,App.Config.bRunesGroups?BST_CHECKED:BST_UNCHECKED);
				SendDlgItemMessage(hDlg,100,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_VIEW_GROUPS));
				if (!Dialog_ViewComboInit(hDlg,200,App.Config.uRunesView,3,LV_VIEW_DETAILS,LV_VIEW_ICON,LV_VIEW_TILE))
					{
					EndDialog(hDlg,-1);
					return(FALSE);
					}
				Game_EditValueSizeObject(hDlg,777);
				} break;

			//--- Mots-clé ---

			case DATA_TYPE_TAGS:
				if (!Game_TagsPopulateList(hDlg,300,&pValue->nodeData,TRUE,TRUE))
					{
					EndDialog(hDlg,-1);
					return(FALSE);
					}
				Game_EditValueSelectLB(hDlg,pValue);
				break;
			}

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
		SetWindowText(hDlg,pValue->pszTitle);
		Dialog_CenterWindow(hDlg,pValue->hwndParent);
		return(FALSE);
		}

	pValue = (GAMEEDITVALUE *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_MEASUREITEM:
			switch(pValue->uType)
				{
				case DATA_TYPE_TAGS:
					switch(wParam)
						{
						case 300:
							Game_TagsMeasureItem(hDlg,(MEASUREITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_DRAWITEM:
			switch(pValue->uType)
				{
				case DATA_TYPE_BOOSTERS:
					switch(wParam)
						{
						case 231:
						case 241:
							Game_EditValueDrawBoostersFilter((DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 232:
						case 242:
							Dialog_DrawIconButton(APP_ICON_SELECT_ALL,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 233:
						case 243:
							Dialog_DrawIconButton(APP_ICON_SELECT_NONE,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 234:
						case 244:
							Dialog_DrawIconButton(APP_ICON_SELECT_INVERT,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 777:
							Game_EditValueDrawObject(pValue->pszObject,NULL,(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				case DATA_TYPE_RUNES:
					switch(wParam)
						{
						case 200:
							Dialog_ViewComboDrawItem((DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						case 777:
							Game_EditValueDrawObject(pValue->pszObject,Game_GetItemTypeName(pValue->pszObject),(DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				case DATA_TYPE_TAGS:
					switch(wParam)
						{
						case 300:
							Game_TagsDrawItemBuffered((DRAWITEMSTRUCT *)lParam);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_KEYDOWN:
					if (((NMLVKEYDOWN *)lParam)->wVKey == 0x43) Game_EditValueCopyID(hDlg,pValue);
					return(TRUE);

				case LVN_ITEMACTIVATE:
					Game_EditValueClose(hDlg,IDOK,pValue);
					return(TRUE);

				case LVN_ITEMCHANGED:
					EnableWindow(GetDlgItem(hDlg,IDOK),Game_EditValueGetSelected(hDlg,TRUE,pValue)?TRUE:FALSE);
					return(TRUE);

				case LVN_COLUMNCLICK:
					switch(pValue->uType)
						{
						case DATA_TYPE_BOOSTERS:
							switch(((NMLISTVIEW *)lParam)->iSubItem)
								{
								default:pValue->uSort = DATA_SORT_ID;
									break;
								case 1:	pValue->uSort = DATA_SORT_TEXT;
									break;
								case 2:	pValue->uSort = DATA_SORT_BOOSTER_TYPE;
									break;
								case 3:	pValue->uSort = DATA_SORT_BOOSTER_SLOT;
									break;
								}
							SendDlgItemMessage(hDlg,300,LVM_SORTITEMS,(WPARAM)pValue,(LPARAM)Game_EditValueCmp);
							break;
						case DATA_TYPE_RUNES:
							switch(((NMLISTVIEW *)lParam)->iSubItem)
								{
								default:pValue->uSort = DATA_SORT_RUNE;
									break;
								case 1:	pValue->uSort = DATA_SORT_RUNE_BONUS1;
									break;
								case 2:	pValue->uSort = DATA_SORT_RUNE_BONUS2;
									break;
								case 3:	pValue->uSort = DATA_SORT_RUNE_BONUS3;
									break;
								}
							SendDlgItemMessage(hDlg,300,LVM_SORTITEMS,(WPARAM)pValue,(LPARAM)Game_EditValueCmp);
							break;
						}
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_DBLCLK:
					switch(LOWORD(wParam))
						{
						case 300:
							Game_EditValueClose(hDlg,IDOK,pValue);
							return(TRUE);
						}
					break;

				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 300:
							EnableWindow(GetDlgItem(hDlg,IDOK),Game_EditValueGetSelected(hDlg,TRUE,pValue)?TRUE:FALSE);
							return(TRUE);
						}
					switch(pValue->uType)
						{
						case DATA_TYPE_BOOSTERS:
							switch(LOWORD(wParam))
								{
								case 231:
									Game_EditValueUpdateBoostersFilters(hDlg,LOWORD(wParam),FILTER_ALL_TARGETS,pValue);
									return(TRUE);
								case 241:
									Game_EditValueUpdateBoostersFilters(hDlg,LOWORD(wParam),FILTER_ALL_TYPES,pValue);
									return(TRUE);
								}
							break;
						case DATA_TYPE_RUNES:
							switch(LOWORD(wParam))
								{
								case 200:
									Dialog_ViewComboChanged(hDlg,200,300,&App.Config.uRunesView);
									return(TRUE);
								}
							break;
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Game_EditValueClose(hDlg,IDOK,pValue);
							return(TRUE);
						case IDCANCEL:
							Game_EditValueClose(hDlg,IDCANCEL,pValue);
							return(TRUE);
						}
					switch(pValue->uType)
						{
						case DATA_TYPE_BOOSTERS:
							switch(LOWORD(wParam))
								{
								case 232:
								case 233:
								case 234:
								case 242:
								case 243:
								case 244:
									Game_EditValueToggleBoostersFilters(hDlg,(LOWORD(wParam)-232)/10,(LOWORD(wParam)-232)%10,pValue);
									return(TRUE);
								case 245:
									App.Config.bBoostersGroups = (IsDlgButtonChecked(hDlg,245) == BST_CHECKED?TRUE:FALSE);
									SendDlgItemMessage(hDlg,300,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bBoostersGroups,0);
									return(TRUE);
								}
							break;
						case DATA_TYPE_RUNES:
							switch(LOWORD(wParam))
								{
								case 100:
									App.Config.bRunesGroups = (IsDlgButtonChecked(hDlg,100) == BST_CHECKED?TRUE:FALSE);
									SendDlgItemMessage(hDlg,300,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bRunesGroups,0);
									return(TRUE);
								}
							break;
						}
					break;
				}
			break;

		case WM_CLOSE:
			Game_EditValueClose(hDlg,IDCANCEL,pValue);
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Création de la liste ««««««««««««««««««««««««««««««««««««««««««««»

BOOL Game_EditValueBuildList(HWND hDlg, BOOL bQuiet, GAMEEDITVALUE *pValue)
{
	static UINT	uRunesGTitles[] = { TEXT_DIALOG_RUNE_GROUP_MASTER, TEXT_DIALOG_RUNE_GROUP_FLAME, TEXT_DIALOG_RUNE_GROUP_FROST, TEXT_DIALOG_RUNE_GROUP_THUNDER, TEXT_DIALOG_RUNE_GROUP_ROCK, TEXT_DIALOG_RUNE_GROUP_VENOM, TEXT_DIALOG_RUNE_GROUP_MISC, 0 };

	static UINT	uBoostersGTitles[] = {	TEXT_DIALOG_FILTER_ABILITIES, 	TEXT_DIALOG_FILTER_MAGICAL_ARMOR,	TEXT_DIALOG_FILTER_PHYSICAL_ARMOR,	TEXT_DIALOG_FILTER_RUNE_SLOT,	TEXT_DIALOG_FILTER_PRIMARY_ATTRIBUTE,	TEXT_DIALOG_FILTER_SECONDARY_ATTRIBUTE,	TEXT_DIALOG_FILTER_RESISTANCE,	TEXT_DIALOG_FILTER_SKILL,	TEXT_DIALOG_FILTER_IMMUNITY,	TEXT_DIALOG_FILTER_REFLECT,	TEXT_DIALOG_FILTER_SPECIAL,	TEXT_DIALOG_FILTER_DAMAGE,	TEXT_DIALOG_FILTER_LIFESTEAL,	TEXT_DIALOG_FILTER_STATUS,	0 };
	static UINT64	uBoostersGFilters[] = {	FILTER_ABILITIES, 		FILTER_MAGICAL_ARMOR,			FILTER_PHYSICAL_ARMOR,			FILTER_RUNE_SLOT,		FILTER_PRIMARY_ATTRIBUTE, 		FILTER_SECONDARY_ATTRIBUTE,		FILTER_RESISTANCE,		FILTER_SKILL,			FILTER_IMMUNITY,		FILTER_REFLECT,			FILTER_SPECIAL,			FILTER_DAMAGE,			FILTER_LIFESTEAL,		FILTER_STATUS,			0 };

	GAMEDATA*	pData;
	LVITEM		lvItem;
	HIMAGELIST	hImageList = NULL;
	UINT		uNumColumns = 0;
	int		i;

	SendDlgItemMessage(hDlg,300,LVM_DELETEALLITEMS,0,0);
	EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);

	//--- Création des images et des groupes pour les runes ---

	if (pValue->uType == DATA_TYPE_RUNES)
		{
		LVGROUP		lvGroup;

		hImageList = ImageList_Create(GAME_ICON_SIZE,GAME_ICON_SIZE,ILC_COLOR32|ILC_MASK,1,0);
		if (!hImageList) goto Failed;

		for (i = 0; uRunesGTitles[i] != 0; i++)
			{
			lvGroup.cbSize = sizeof(LVGROUP);
			lvGroup.mask = LVGF_HEADER|LVGF_GROUPID;
			lvGroup.pszHeader = Locale_GetText(uRunesGTitles[i]);
			lvGroup.iGroupId = i;
			if (SendDlgItemMessage(hDlg,300,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) goto Failed;
			}
		}

	//--- Création des groupes pour les boosters ---

	else if (pValue->uType == DATA_TYPE_BOOSTERS)
		{
		LVGROUP		lvGroup;

		for (i = 0; uBoostersGTitles[i] != 0; i++)
			{
			lvGroup.cbSize = sizeof(LVGROUP);
			lvGroup.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_STATE;
			lvGroup.pszHeader = Locale_GetText(uBoostersGTitles[i]);
			lvGroup.iGroupId = i;
			lvGroup.stateMask = lvGroup.state = LVGS_COLLAPSIBLE|LVGS_COLLAPSED;
			if (Game_EditValueIsBoosterValid(pValue->pszValue,uBoostersGFilters[i],FALSE)) lvGroup.stateMask &= ~LVGS_COLLAPSED;
			if (SendDlgItemMessage(hDlg,300,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) goto Failed;
			}
		}

	//--- Boucle d'insertion des entrées ---

	for (lvItem.iItem = 0, pData = (GAMEDATA *)pValue->nodeData.next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
		{
		if (pValue->uType == DATA_TYPE_BOOSTERS && !Game_EditValueIsBoosterValid(pData->pszId,pValue->uFilter,TRUE)) continue;

		switch(pValue->uType)
			{
			case DATA_TYPE_BOOSTERS:
				//--- Item
				lvItem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_GROUPID;
				lvItem.iSubItem = 0;
				lvItem.pszText = pData->pszId;
				lvItem.lParam = (LPARAM)pData;
				lvItem.iGroupId = 0;
				for (i = 0; uBoostersGFilters[i] != 0; i++)
					{
					if (Game_EditValueIsBoosterValid(pData->pszId,uBoostersGFilters[i],FALSE))
						{
						lvItem.iGroupId = i;
						break;
						}
					}
				if (SendDlgItemMessage(hDlg,300,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Failed;
				//--- Text
				lvItem.iSubItem = 1;
				lvItem.pszText = pData->pszText;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;
				//--- Type
				lvItem.iSubItem = 2;
				lvItem.pszText = pData->booster.pszType;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;
				//--- Slot
				lvItem.iSubItem = 3;
				lvItem.pszText = pData->booster.pszSlot;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;
				break;

			case DATA_TYPE_RUNES: {
				LVTILEINFO	lvTileInfo;
				UINT		uType;
				UINT		uColumns[3];
				UINT		uLen;

				struct GAME_RUNE_CMP { WCHAR *pszCmp; UINT uCmpType; };
				static struct GAME_RUNE_CMP cRunes[] = {{ L"LOOT_Rune_Masterwork_", CMP_TYPE_BEGIN },
									{ L"LOOT_Rune_Flame_", CMP_TYPE_BEGIN },
									{ L"LOOT_Rune_Frost_", CMP_TYPE_BEGIN },
									{ L"LOOT_Rune_Thunder_", CMP_TYPE_BEGIN },
									{ L"LOOT_Rune_Rock_", CMP_TYPE_BEGIN },
									{ L"LOOT_Rune_Venom_", CMP_TYPE_BEGIN },
									{ NULL, 0 }};

				//--- Item
				lvItem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_GROUPID;
				lvItem.iSubItem = 0;
				lvItem.iGroupId = 6;
				lvItem.pszText = pData->pszText?pData->pszText:pData->pszId;
				lvItem.lParam = (LPARAM)pData;
				//--- Item group
				uLen = wcslen(pData->pszId);
				for (i = 0; cRunes[i].pszCmp != NULL; i++)
					{
					if (Game_CompareStrings(cRunes[i].pszCmp,pData->pszId,uLen,cRunes[i].uCmpType))
						{
						lvItem.iGroupId = i;
						break;
						}
					}
				//--- Item icon
				if (App.Game.hRunesIconsList)
					{
					HICON hIcon = LoadImage(App.Game.hRunesIconsList,pData->pszId,IMAGE_ICON,GAME_ICON_SIZE,GAME_ICON_SIZE,LR_DEFAULTCOLOR|LR_SHARED);
					if (!hIcon) hIcon = App.hIcons[APP_ICON_EMPTY];
					lvItem.mask |= LVIF_IMAGE;
					lvItem.iImage = ImageList_ReplaceIcon(hImageList,-1,hIcon);
					}
				if (SendDlgItemMessage(hDlg,300,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Failed;
				//--- Bonus description
				lvItem.iSubItem = 1;
				lvItem.pszText = pData->rune.pszBonus1;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;
				//--- Bonus description
				lvItem.iSubItem = 2;
				lvItem.pszText = pData->rune.pszBonus2;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;
				//--- Bonus description
				lvItem.iSubItem = 3;
				lvItem.pszText = pData->rune.pszBonus3;
				if (SendDlgItemMessage(hDlg,300,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0) goto Failed;

				//--- Tile infos
				uType = Game_GetItemType(pValue->pszObject);
				uColumns[0] = 1;
				uColumns[1] = 2;
				uColumns[2] = 3;
				uNumColumns = 3;
				if (uType == DATA_TYPE_IS_WEAPON)
					{
					uColumns[0] = 1;
					uNumColumns = 1;
					}
				else if (uType == DATA_TYPE_IS_ARMOR)
					{
					uColumns[0] = 2;
					uNumColumns = 1;
					}
				else if (uType == DATA_TYPE_IS_ACCESSORY)
					{
					uColumns[0] = 3;
					uNumColumns = 1;
					}
				lvTileInfo.cbSize = sizeof(LVTILEINFO);
				lvTileInfo.iItem = lvItem.iItem;
				lvTileInfo.cColumns = uNumColumns;
				lvTileInfo.puColumns = uColumns;
				lvTileInfo.piColFmt = NULL;
				if (SendDlgItemMessage(hDlg,300,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == FALSE) goto Failed;
				} break;
			}

		lvItem.iItem++;
		}

	//--- Mode d'affichage pour les runes ---

	if (pValue->uType == DATA_TYPE_RUNES)
		{
		LVTILEVIEWINFO	lvTileView;

		lvTileView.cbSize = sizeof(LVTILEVIEWINFO);
		lvTileView.dwMask = LVTVIM_COLUMNS;
		lvTileView.cLines = ++uNumColumns;
		SendDlgItemMessage(hDlg,300,LVM_SETIMAGELIST,(WPARAM)LVSIL_NORMAL,(LPARAM)hImageList);
		SendDlgItemMessage(hDlg,300,LVM_SETTILEVIEWINFO,0,(LPARAM)&lvTileView);
		SendDlgItemMessage(hDlg,300,LVM_SETVIEW,(WPARAM)App.Config.uRunesView,(LPARAM)0);
		SendDlgItemMessage(hDlg,300,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bRunesGroups,0);
		}

	//--- Mode d'affichage pour les boosters ---

	else if (pValue->uType == DATA_TYPE_BOOSTERS)
		{
		SendDlgItemMessage(hDlg,300,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bBoostersGroups,0);
		}

	//--- Tri et fin ---

	SendDlgItemMessage(hDlg,300,LVM_SORTITEMS,(WPARAM)pValue,(LPARAM)Game_EditValueCmp);
	EnableWindow(GetDlgItem(hDlg,IDOK),Game_EditValueGetSelected(hDlg,TRUE,pValue)?TRUE:FALSE);
	Game_EditValueSelectLV(hDlg,pValue);
	return(TRUE);

	//--- Echec ---

Failed:	if (!bQuiet) Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	if (hImageList) ImageList_Destroy(hImageList);
	SendDlgItemMessage(hDlg,300,LVM_DELETEALLITEMS,0,0);
	return(FALSE);
}


// «»»» Routine de tri ««««««««««««««««««««««««««««««««««««««««««««««««««»

int CALLBACK Game_EditValueCmp(GAMEDATA *pData1, GAMEDATA *pData2, GAMEEDITVALUE *pValue)
{
	WCHAR*		pszText1 = NULL;
	WCHAR*		pszText2 = NULL;
	int		iResult;

	switch(pValue->uSort)
		{
		case DATA_SORT_ID:
		case DATA_SORT_RUNE:
			pszText1 = pData1->pszId;
			pszText2 = pData2->pszId;
			break;
		case DATA_SORT_TEXT:
			pszText1 = pData1->pszText;
			pszText2 = pData2->pszText;
			break;
		case DATA_SORT_RUNE_BONUS1:
			pszText1 = pData1->rune.pszBonus1;
			pszText2 = pData2->rune.pszBonus1;
			break;
		case DATA_SORT_RUNE_BONUS2:
			pszText1 = pData1->rune.pszBonus2;
			pszText2 = pData2->rune.pszBonus2;
			break;
		case DATA_SORT_RUNE_BONUS3:
			pszText1 = pData1->rune.pszBonus3;
			pszText2 = pData2->rune.pszBonus3;
			break;
		case DATA_SORT_BOOSTER_TYPE:
			pszText1 = pData1->booster.pszType;
			pszText2 = pData2->booster.pszType;
			break;
		case DATA_SORT_BOOSTER_SLOT:
			pszText1 = pData1->booster.pszSlot;
			pszText2 = pData2->booster.pszSlot;
			break;
		}

	if (!pszText1 && pszText2) return(-1);
	else if (pszText1 && !pszText2) return(1);
	else if (!pszText1 && !pszText2) return(0);

	iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
	if (iResult == CSTR_LESS_THAN) return(-1);
	if (iResult == CSTR_GREATER_THAN) return(1);
	return(0);
}


// «»»» [BOOSTERS] Liste des filtres «««««««««««««««««»

//--- Modifie les filtres ---

void Game_EditValueToggleBoostersFilters(HWND hDlg, UINT uSide, UINT uAction, GAMEEDITVALUE *pValue)
{
	UINT64	uFilter;
	UINT64	uMask;
	UINT64	uOrigin;
	UINT	uCtrlId;

	uCtrlId = uSide?241:231;
	uMask = uSide?FILTER_ALL_TYPES:FILTER_ALL_TARGETS;

	SendDlgItemMessage(hDlg,uCtrlId,LB_SETSEL,(WPARAM)FALSE,(LPARAM)-1);

	switch(uAction)
		{
		default:
		case 0:	uFilter = pValue->uFilter;
			uFilter &= ~uMask;
			uFilter |= uMask;
			break;
		case 1:	uFilter = pValue->uFilter;
			uFilter &= ~uMask;
			break;
		case 2:	uOrigin = pValue->uFilter;
			uOrigin &= ~uMask;
			uFilter = ~pValue->uFilter;
			uFilter &= uMask;
			uFilter |= uOrigin;
			break;
		}

	Game_EditValueSetBoostersFilters(hDlg,uCtrlId,uFilter);
	Game_EditValueUpdateBoostersFilters(hDlg,uCtrlId,uMask,pValue);
	return;
}

//--- Applique le filtre sur l'identifiant ---

BOOL Game_EditValueIsBoosterValid(WCHAR *pszId, UINT64 uFilter, BOOL bCheckTarget)
{
	struct GAME_STRING_CMP {
		UINT64	uFilter;
		WCHAR*	pszCmp;
		UINT	uCmpType;
	};

	if (!pszId) return(FALSE);
	if (!uFilter) return(FALSE); // No filter, nothing to search for

	if (uFilter != (FILTER_ALL)) // No need to compare if we want all items
		{
		static struct GAME_STRING_CMP	TARGET[] = {

						{ FILTER_ARMORS_RING|FILTER_ARMORS_BOOTS|FILTER_ARMORS_HELMET|FILTER_ARMORS_BELT|FILTER_ARMORS_GLOVES|FILTER_ARMORS_PANTS, L"Boost_Armor_UpperBody_EmptyRuneSlot", CMP_TYPE_FULL },
						{ FILTER_ARMORS_ANY, L"Boost_Armor_All_Armour_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_AMULET, L"Boost_Armor_Amulet_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_RING, L"Boost_Armor_Ring_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_BOOTS, L"Boost_Armor_Boots_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_HELMET, L"Boost_Armor_Helmet_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_BELT, L"Boost_Armor_Belt_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_GLOVES, L"Boost_Armor_Gloves_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_PANTS, L"Boost_Armor_Pants_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_UPPERBODY, L"Boost_Armor_UpperBody_", CMP_TYPE_BEGIN },
						{ FILTER_ARMORS_UPPERBODY, L"Boost_Armor_Upperbody_", CMP_TYPE_BEGIN },
						{ FILTER_SHIELDS, L"Boost_Shield_", CMP_TYPE_BEGIN },
						{ FILTER_WEAPONS_ANY, L"Boost_Weapon_", CMP_TYPE_BEGIN },
						{ 0 }

					};

		static struct GAME_STRING_CMP	TYPE[] = {

						{ FILTER_ABILITIES, L"_Ability_", CMP_TYPE_CONTENT },
						{ FILTER_MAGICAL_ARMOR, L"_Armour_All_", CMP_TYPE_CONTENT },
						{ FILTER_MAGICAL_ARMOR, L"_Armour_Magical_", CMP_TYPE_CONTENT },
						{ FILTER_PHYSICAL_ARMOR, L"_Armour_Physical_", CMP_TYPE_CONTENT },
						{ FILTER_PHYSICAL_ARMOR, L"_Armour_All_", CMP_TYPE_CONTENT },
						{ FILTER_RUNE_SLOT, L"_EmptyRuneSlot", CMP_TYPE_END },
						{ FILTER_PRIMARY_ATTRIBUTE, L"_Primary_", CMP_TYPE_CONTENT },
						{ FILTER_SECONDARY_ATTRIBUTE, L"_Secondary_", CMP_TYPE_CONTENT },
						{ FILTER_RESISTANCE, L"_Resistance_", CMP_TYPE_CONTENT },
						{ FILTER_SKILL, L"_Skill_", CMP_TYPE_CONTENT },
						{ FILTER_IMMUNITY, L"_Immunity_", CMP_TYPE_CONTENT },
						{ FILTER_REFLECT, L"_Reflect_", CMP_TYPE_CONTENT },
						{ FILTER_SPECIAL, L"_Special_", CMP_TYPE_CONTENT },
						{ FILTER_DAMAGE, L"_Damage_", CMP_TYPE_CONTENT },
						{ FILTER_DAMAGE, L"_Cleave_", CMP_TYPE_CONTENT },
						{ FILTER_DAMAGE, L"_Cleave", CMP_TYPE_END },
						{ FILTER_LIFESTEAL, L"_LifeSteal_", CMP_TYPE_CONTENT },
						{ FILTER_STATUS, L"_Status_", CMP_TYPE_CONTENT },
						{ 0 }

					};

		struct GAME_STRING_CMP *Table;
		UINT i;

		UINT uLen = wcslen(pszId);

		//--- Détermine quelle table utiliser pour la comparaison ---

		if (bCheckTarget)
			{
			Table = TARGET;
			if ((uFilter&(FILTER_ALL_TARGETS)) == (FILTER_ALL_TARGETS)) return(Game_EditValueIsBoosterValid(pszId,uFilter,FALSE));
			}
		else
			{
			Table = TYPE;
			if ((uFilter&(FILTER_ALL_TYPES)) == (FILTER_ALL_TYPES)) return(TRUE);
			}

		//--- Boucle de comparaison ---

		for (i = 0; Table[i].uFilter != 0; i++)
			if (uFilter&Table[i].uFilter)
				if (Game_CompareStrings(Table[i].pszCmp,pszId,uLen,Table[i].uCmpType))
					return(!bCheckTarget?TRUE:Game_EditValueIsBoosterValid(pszId,uFilter,FALSE));

		return(FALSE);
		}

	return(TRUE);
}

// --- Définition des filtres ---

void Game_EditValueSetBoostersFilters(HWND hDlg, UINT uCtrlID, UINT64 uFilter)
{
	GAMEEDITFILTER*	pFilter;
	UINT		uCount;

	uCount = SendDlgItemMessage(hDlg,uCtrlID,LB_GETCOUNT,0,0);
	if (uCount == LB_ERR) return;

	while(uCount--)
		{
		pFilter = (GAMEEDITFILTER *)SendDlgItemMessage(hDlg,uCtrlID,LB_GETITEMDATA,(WPARAM)uCount,0);
		if (pFilter == (GAMEEDITFILTER *)LB_ERR) continue;
		if (!(pFilter->uFilter&uFilter)) continue;
		SendDlgItemMessage(hDlg,uCtrlID,LB_SETSEL,(WPARAM)TRUE,(LPARAM)uCount);
		}

	return;
}

//--- Mise à jour des filtres ---

void Game_EditValueUpdateBoostersFilters(HWND hDlg, UINT uCtrlID, UINT64 uMask, GAMEEDITVALUE *pValue)
{
	GAMEEDITFILTER*	pFilter;
	UINT*		uSelectedPtr = NULL;
	UINT		uSelCount;
	UINT64		uFilterTmp, uFilter = 0;

	//--- Retrouve la liste des entrées sélectionnées ---

	uSelCount = SendDlgItemMessage(hDlg,uCtrlID,LB_GETSELCOUNT,0,0);
	if (uSelCount)
		{
		uSelectedPtr = HeapAlloc(App.hHeap,0,uSelCount*sizeof(UINT*));
		if (!uSelectedPtr) return;
		SendDlgItemMessage(hDlg,uCtrlID,LB_GETSELITEMS,(WPARAM)uSelCount,(LPARAM)uSelectedPtr);
		}

	//--- Mise-à-jour des filtres ---

	if (uSelectedPtr)
		{
		while(uSelCount--)
			{
			pFilter = (GAMEEDITFILTER *)SendDlgItemMessage(hDlg,uCtrlID,LB_GETITEMDATA,(WPARAM)uSelectedPtr[uSelCount],0);
			if (pFilter == (GAMEEDITFILTER *)LB_ERR) continue;
			uFilter |= pFilter->uFilter;
			}
		HeapFree(App.hHeap,0,uSelectedPtr);
		}

	//--- Vérifie si une mise-à-jour de la liste est nécessaire ---

	uFilterTmp = pValue->uFilter&uMask;
	if (uFilterTmp == uFilter) return;

	//--- Mise-à-jour de la liste ---

	pValue->uFilter &= ~uMask;
	pValue->uFilter |= uFilter;

	Game_EditValueBuildList(hDlg,FALSE,pValue);
	return;
}


//--- Affichage des filtres ---

void Game_EditValueDrawBoostersFilter(DRAWITEMSTRUCT *pDraw)
{
	GAMEEDITFILTER*	pFilter;
	HFONT		hFont;
	RECT		rcText;
	COLORREF	crText;
	int		iBack;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	pFilter = (GAMEEDITFILTER *)pDraw->itemData;
	if (!pFilter) return;
	if (pFilter == (GAMEEDITFILTER *)-1) return;

	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 4;
	rcText.right -= 4;
	DrawText(pDraw->hDC,Locale_GetText(pFilter->uLocaleID),-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Affichage du séparateur «««««««««««««««««««««««««««««««««««««««««»

void Game_EditValueDrawSeparator(DRAWITEMSTRUCT *pDraw)
{
	DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKENOUTER,BF_RECT);
	return;
}


// «»»» Objet cible «««««««««««««««««««««««««««««««««««««««««««««««««««««»

//--- Redimensionne le contrôle ---

void Game_EditValueSizeObject(HWND hDlg, UINT uCtrlID)
{
	RECT	rcWindow;
	UINT	uHeight;

	uHeight = GAME_ICON_SIZE+4;
	if (uHeight < App.Font.uFontHeight*2+4) uHeight = App.Font.uFontHeight*2+4;

	GetWindowRect(GetDlgItem(hDlg,uCtrlID),&rcWindow);
	MapWindowPoints(NULL,hDlg,(POINT *)&rcWindow,2);

	SetWindowPos(GetDlgItem(hDlg,uCtrlID),NULL,rcWindow.left,rcWindow.top-(uHeight/2),rcWindow.right-rcWindow.left,uHeight,SWP_NOZORDER);
	return;
}

//--- Affichage de l'objet ---

void Game_EditValueDrawObject(WCHAR *pszName, WCHAR *pszMore, DRAWITEMSTRUCT *pDraw)
{
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	if (pszName)
		{
		HFONT		hFont;
		RECT		rcClient;
		RECT		rcDraw;
		COLORREF	crText;
		int		iBack;

		CopyRect(&rcClient,&pDraw->rcItem);

		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
		iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

		//--- Draw item icon
		CopyRect(&rcDraw,&rcClient);
		Game_PaintIcon(pDraw->hDC,pszName,APP_ICON_EMPTY,&rcDraw,GAME_ICON_SIZE,TRUE,TRUE);

		//--- Draw item text
		CopyRect(&rcDraw,&rcClient);
		rcDraw.left += GAME_ICON_SIZE+8;
		DrawText(pDraw->hDC,pszName,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);

		//--- Draw item information
		if (pszMore)
			{
			rcDraw.top += App.Font.uFontHeight+2;
			DrawText(pDraw->hDC,pszMore,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
			}

		SetBkMode(pDraw->hDC,iBack);
		SetTextColor(pDraw->hDC,crText);
		SelectObject(pDraw->hDC,hFont);
		}

	return;
}


// «»»» Sélectionne la valeur dans la liste «««««««««««««««««««««««««««««»

//--- ListBox ---

void Game_EditValueSelectLB(HWND hDlg, GAMEEDITVALUE *pValue)
{
	GAMEDATA*	pData;
	UINT		uCount;

	if (!pValue->pszValue) return;

	uCount = SendDlgItemMessage(hDlg,300,LB_GETCOUNT,0,0);
	if (!uCount) return;

	while(uCount--)
		{
		pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uCount,0);
		if ((LRESULT)pData == LB_ERR) continue;
		if (wcscmp(pData->pszId,pValue->pszValue)) continue;
		SendDlgItemMessage(hDlg,300,LB_SETCURSEL,(WPARAM)uCount,0);
		break;
		}

	return;
}

//--- ListView ---

void Game_EditValueSelectLV(HWND hDlg, GAMEEDITVALUE *pValue)
{
	LVITEM	lvItem;
	UINT	uCount;

	if (!pValue->pszValue) return;

	uCount = SendDlgItemMessage(hDlg,300,LVM_GETITEMCOUNT,0,0);
	if (!uCount) return;

	while(uCount--)
		{
		lvItem.mask = LVIF_PARAM;
		lvItem.iItem = uCount;
		lvItem.iSubItem = 0;
		lvItem.lParam = 0;
		if (!SendDlgItemMessage(hDlg,300,LVM_GETITEM,(WPARAM)0,(LPARAM)&lvItem)) continue;
		if (!lvItem.lParam) continue;
		if (wcscmp(((GAMEDATA *)lvItem.lParam)->pszId,pValue->pszValue)) continue;
		lvItem.stateMask = LVIS_SELECTED;
		lvItem.state = LVIS_SELECTED;
		SendDlgItemMessage(hDlg,300,LVM_SETITEMSTATE,(WPARAM)uCount,(LPARAM)&lvItem);
		SendDlgItemMessage(hDlg,300,LVM_ENSUREVISIBLE,(WPARAM)uCount,(LPARAM)FALSE);
		break;
		}

	return;
}


// «»»» Récupère la valeur sélectionnée «««««««««««««««««««««««««««««««««»

GAMEDATA* Game_EditValueGetSelected(HWND hDlg, BOOL bModifyCheck, GAMEEDITVALUE *pValue)
{
	LVITEM		lvItem;
	UINT		uSelected;
	GAMEDATA*	pData;

	pData = NULL;

	switch(pValue->uType)
		{
		case DATA_TYPE_BOOSTERS:
		case DATA_TYPE_RUNES:
			lvItem.iItem = SendDlgItemMessage(hDlg,300,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)LVNI_SELECTED);
			if (lvItem.iItem == -1) break;
			lvItem.mask = LVIF_PARAM;
			lvItem.iSubItem = 0;
			lvItem.lParam = 0;
			if (!SendDlgItemMessage(hDlg,300,LVM_GETITEM,(WPARAM)0,(LPARAM)&lvItem)) break;
			if (!lvItem.lParam) break;
			pData = (GAMEDATA *)lvItem.lParam;
			break;
		case DATA_TYPE_TAGS:
			uSelected = SendDlgItemMessage(hDlg,300,LB_GETCURSEL,0,0);
			if (uSelected == LB_ERR) break;
			pData = (GAMEDATA *)SendDlgItemMessage(hDlg,300,LB_GETITEMDATA,(WPARAM)uSelected,0);
			if (pData != (GAMEDATA *)LB_ERR) break;
			pData = NULL;
			break;
		}

	if (pData)
		if (bModifyCheck && pValue->pszValue)
			if (!wcscmp(pData->pszId,pValue->pszValue))
				return(NULL);

	return(pData);
}


// «»»» Sauvegarde la valeur ««««««««««««««««««««««««««««««««««««««««««««»

int Game_EditValueSave(HWND hDlg, GAMEEDITVALUE *pValue)
{
	GAMEDATA*	pData;

	pData = Game_EditValueGetSelected(hDlg,TRUE,pValue);
	if (!pData) return(0);

	pValue->pszResult = HeapAlloc(App.hHeap,0,wcslen(pData->pszId)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pValue->pszResult) return(0);

	wcscpy(pValue->pszResult,pData->pszId);

	//--- Vérifie si le tag n'est pas protégé ---

	if (pValue->uType == DATA_TYPE_TAGS)
		{
		int		iResult;

		for (pData = (GAMEDATA *)pValue->nodeData.next; pData != NULL; pData = (GAMEDATA *)pData->node.next)
			{
			if (!pData->tag.bProtected) continue;
			if (wcscmp(pData->pszId,pValue->pszResult)) continue;
			iResult = MessageBox(hDlg,Locale_GetText(TEXT_DIALOG_TAG_PROTECTED),Locale_GetText(TEXT_TITLE_WARNING),MB_ICONWARNING|MB_OKCANCEL);
			if (iResult == IDOK) break;
			HeapFree(App.hHeap,0,pValue->pszResult);
			pValue->pszResult = NULL;
			return(0);
			}
		}

	return(1);
}


// «»»» Fermeture de la boîte de dialogue «««««««««««««««««««««««««««««««»

void Game_EditValueClose(HWND hDlg, INT_PTR nResult, GAMEEDITVALUE *pValue)
{
	if (nResult == IDOK && !Game_EditValueSave(hDlg,pValue)) return;
	EndDialog(hDlg,nResult);
	return;
}


// «»»» Copie un identifiant dans le presse-papier ««««««««««««««««««««««»

void Game_EditValueCopyID(HWND hDlg, GAMEEDITVALUE *pValue)
{
	GAMEDATA*	pData;

	pData = Game_EditValueGetSelected(hDlg,FALSE,pValue);
	if (!pData) return;

	if (OpenClipboard(NULL))
		{
		UINT cch = wcslen(pData->pszId);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE,(cch+1)*sizeof(WCHAR));
		if (hGlobal)
			{
			LPTSTR lptstrCopy = GlobalLock(hGlobal);
			CopyMemory(lptstrCopy,pData->pszId,cch*sizeof(WCHAR));
			GlobalUnlock(hGlobal);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT,hGlobal);
			}
		CloseClipboard();
		}

	return;
}

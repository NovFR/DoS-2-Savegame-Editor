
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Compétences
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Game.h"
#include "GameEdit.h"
#include "Menus.h"
#include "Texts.h"
#include "Lists.h"
#include "Requests.h"
#include "Dialogs.h"
#include "Utils.h"

extern APPLICATION		App;
extern CUSTOMMENUTEMPLATE	SkillsListMenu[];

static UINT			SkillsSort[] = { DATA_SORT_SKILL_NAME, DATA_SORT_SKILL_SCHOOL, DATA_SORT_SKILL_POINTS, DATA_SORT_SKILL_SOURCE, DATA_SORT_SKILL_MEMORY, 0 };

static WCHAR*			SkillsBlackList[] = {	// List of skills known to be broken/unusable
							L"Shout_RecoverArmour",
							L"Target_SingleHandedAttack",
							L"Target_HeavyAttack",
							L"Target_DualWieldingAttack",
							NULL
						};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisation							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Game_Skills(HWND hWnd)
{
	GAMESKILLSCONTEXT*	ctx;
	INT_PTR			iResult;

	ctx = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMESKILLSCONTEXT));
	if (ctx)
		{
		if (Game_SkillsDataFileCreate(hWnd,&ctx->baseSkills.skills))
			{
			if ((ctx->baseSkills.hImageList = Game_SkillsImageListCreate(hWnd,TEXT_ERR_SKILLS_ICONS,&ctx->baseSkills.skills)) != NULL)
				{
				if (Game_SkillsBookCreate(hWnd,TEXT_ERR_SKILLS_BOOK,App.Game.pdcCurrent,ctx))
					{
					ctx->mySkills.hwndParent = hWnd;
					ctx->mySkills.uSort = ctx->baseSkills.uSort = DATA_SORT_SKILL_SCHOOL;
					iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1102),hWnd,Game_SkillsBookProc,(LPARAM)ctx);
					if (iResult == -1) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
					}
				}
			}
		}

	if (ctx)
		{
		Game_UnloadDataFile(DATA_TYPE_SKILLS,&ctx->selectSkills.skills);
		Game_UnloadDataFile(DATA_TYPE_SKILLS,&ctx->mySkills.skills);
		Game_UnloadDataFile(DATA_TYPE_SKILLS,&ctx->baseSkills.skills);
		if (ctx->baseSkills.hImageList) ImageList_Destroy(ctx->baseSkills.hImageList);
		HeapFree(App.hHeap,0,ctx);
		}

	return;
}


// «»»» Création de la liste des compétences ««««««««««««««««««««««««««««»

int Game_SkillsDataFileCreate(HWND hDlg, NODE *pRoot)
{
	if (!pRoot) return(0);
	if (pRoot->next) return(1);
	return(Game_LoadDataFile(hDlg,szSkillsDataPath,DATA_TYPE_SKILLS,pRoot));
}


// «»»» Création de la liste d'icônes «««««««««««««««««««««««««««««««««««»

HIMAGELIST Game_SkillsImageListCreate(HWND hDlg, UINT uMsgId, NODE *pRoot)
{
	GAMEDATASKILL*	pSkill;
	HIMAGELIST	hImageList = NULL;
	HINSTANCE	hSkillsIcons = NULL;
	HICON		hIcon = NULL;
	BOOL		bSet = FALSE;
	WCHAR		szName[16];

	//--- IconsLib
	hSkillsIcons = LoadLibraryEx(szSkillsIconsPath,NULL,DONT_RESOLVE_DLL_REFERENCES|LOAD_LIBRARY_AS_DATAFILE|LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!hSkillsIcons) goto Done;

	//--- ImageList
	hImageList = ImageList_Create(64,64,ILC_COLOR32|ILC_MASK,List_EntryCount(pRoot)+1,0);
	if (!hImageList) goto Done;

	//--- "Unknown" icon
	hIcon = LoadImage(hSkillsIcons,L"SI_0000",IMAGE_ICON,64,64,LR_DEFAULTCOLOR|LR_SHARED);
	if (!hIcon) goto Done;
	if (ImageList_ReplaceIcon(hImageList,-1,hIcon) == -1) goto Done;

	//--- Icons
	for (pSkill = (GAMEDATASKILL *)pRoot->next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
		{
		Misc_Printf(szName,16,L"SI_%.4u",pSkill->infos.uIconID);
		hIcon = LoadImage(hSkillsIcons,szName,IMAGE_ICON,64,64,LR_DEFAULTCOLOR|LR_SHARED);
		if (!hIcon) continue; // Use the "Unknown" icon
		if ((pSkill->iIconIndex = ImageList_ReplaceIcon(hImageList,-1,hIcon)) == -1) goto Done;
		}

	//--- OK
	bSet = TRUE;

Done:	if (!bSet)
		{
		Request_PrintError(hDlg,Locale_GetText(uMsgId),NULL,MB_ICONERROR);
		if (hIcon) DestroyIcon(hIcon);
		if (hImageList) ImageList_Destroy(hImageList);
		hImageList = NULL;
		}
	if (hSkillsIcons) FreeLibrary(hSkillsIcons);
	return(hImageList);
}


// «»»» Création de la liste ««««««««««««««««««««««««««««««««««««««««««««»

int Game_SkillsListCreate(HWND hDlg, UINT uCtlID, HIMAGELIST hImageList, NODE *pRoot, UINT uMsgId, UINT uSort, GAMEDATASKILL *pSelected, BOOL bShareImageList)
{
	LVCOLUMN	lvColumn;
	LVGROUP		lvGroup;
	LVTILEVIEWINFO	lvTileView;
	GAMEDATASKILL*	pSkill;
	static UINT	uSkillsColumns[] = { TEXT_BONUS_SKILL_COLUMN_NAME, TEXT_BONUS_SKILL_COLUMN_SCHOOL, TEXT_BONUS_SKILL_COLUMN_POINTS, 0 };
	static UINT	uGroupsTitles[] = { TEXT_ABILITIES_WARFARE, TEXT_ABILITIES_AIR, TEXT_ABILITIES_HUNTING, TEXT_ABILITIES_ROGUES, TEXT_ABILITIES_EARTH, TEXT_ABILITIES_WATER, TEXT_ABILITIES_SUMMONING, TEXT_ABILITIES_POLYMORPH, TEXT_ABILITIES_DEATH, TEXT_ABILITIES_FIRE, TEXT_ABILITIES_SOURCE, TEXT_ABILITIES_SPECIAL, TEXT_ABILITIES_UNKNOWN, TEXT_BONUS_GROUP_NAMES, TEXT_BONUS_GROUP_POINTS_0, TEXT_BONUS_GROUP_POINTS_1, TEXT_BONUS_GROUP_POINTS_2, TEXT_BONUS_GROUP_POINTS_3, TEXT_BONUS_GROUP_POINTS_4, TEXT_BONUS_GROUP_POINTS_5, TEXT_BONUS_GROUP_POINTS_6, TEXT_BONUS_GROUP_SOURCE_0, TEXT_BONUS_GROUP_SOURCE_1, TEXT_BONUS_GROUP_SOURCE_2, TEXT_BONUS_GROUP_SOURCE_3, TEXT_BONUS_GROUP_MEMORY_0, TEXT_BONUS_GROUP_MEMORY_1, TEXT_BONUS_GROUP_MEMORY_2, TEXT_BONUS_GROUP_MEMORY_3, 0 };
	int		i;

	if (!hImageList) return(0);
	if (!pRoot) return(0);

	//--- Columns
	for (i = 0; uSkillsColumns[i] != 0; i++)
		{
		lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.cx = 0;
		lvColumn.pszText = Locale_GetText(uSkillsColumns[i]);
		lvColumn.cchTextMax = wcslen(lvColumn.pszText);
		lvColumn.iOrder = i;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTCOLUMN,i,(LPARAM)&lvColumn) == -1) goto Error;
		}

	//--- Groups
	for (i = 0; uGroupsTitles[i] != 0; i++)
		{
		lvGroup.cbSize = sizeof(LVGROUP);
		lvGroup.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_STATE;
		lvGroup.pszHeader = Locale_GetText(uGroupsTitles[i]);
		lvGroup.iGroupId = uGroupsTitles[i];
		lvGroup.stateMask = lvGroup.state = LVGS_COLLAPSIBLE|LVGS_COLLAPSED;
		lvGroup.state &= ~LVGS_COLLAPSED; // Expand always
		//if (!pSelected) lvGroup.state &= ~LVGS_COLLAPSED;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) goto Error;
		}

	//--- Items
	for (pSkill = (GAMEDATASKILL *)pRoot->next, i = 0; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next, i++)
		{
		if (!Game_SkillsInsertItem(hDlg,uCtlID,i,uSort,pSkill,pSelected)) goto Error;
		}

	//--- Selected
	Game_SkillsCollapseSelectedGroup(hDlg,uCtlID,pSelected);

	//--- TileView
	lvTileView.cbSize = sizeof(LVTILEVIEWINFO);
	lvTileView.dwMask = LVTVIM_COLUMNS;
	lvTileView.dwFlags = LVTVIF_AUTOSIZE;
	lvTileView.cLines = 3;
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETTILEVIEWINFO,0,(LPARAM)&lvTileView);

	//--- Last
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETIMAGELIST,(WPARAM)LVSIL_NORMAL,(LPARAM)hImageList);
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETICONSPACING,0,(LPARAM)MAKELONG(64+40,64+App.Font.uFontHeight*2+20));
	SendDlgItemMessage(hDlg,uCtlID,LVM_SETVIEW,(WPARAM)App.Config.uSkillsView,(LPARAM)0);
	SendDlgItemMessage(hDlg,uCtlID,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bSkillsGroups,0);
	SendDlgItemMessage(hDlg,uCtlID,LVM_SORTITEMS,(WPARAM)uSort,(LPARAM)Game_SkillsSort);
	return(1);

	//--- Error
Error:	Request_PrintError(hDlg,Locale_GetText(uMsgId),NULL,MB_ICONERROR);
	SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEALLITEMS,0,0);
	if (!bShareImageList) ImageList_Destroy(hImageList);
	return(0);
}

//--- Insère une compétence dans la liste ---

BOOL Game_SkillsInsertItem(HWND hDlg, UINT uCtlID, int i, UINT uSort, GAMEDATASKILL *pSkill, GAMEDATASKILL *pSelected)
{
	LVITEM		lvItem;
	LVTILEINFO	lvTileInfo;
	WCHAR		szName[20];
	UINT		uColumns[2];
	int		j,k;

	//--- Item
	lvItem.mask = LVIF_IMAGE|LVIF_PARAM|LVIF_TEXT|LVIF_GROUPID;
	lvItem.iItem = i;
	lvItem.iSubItem = 0;
	lvItem.pszText = pSkill->pszName?pSkill->pszName:pSkill->pszId;
	lvItem.iImage = pSkill->iIconIndex;
	lvItem.iGroupId = Game_SkillsSetItemGroup(uSort,pSkill);
	lvItem.lParam = (LPARAM)pSkill;
	if (pSelected == pSkill)
		{
		lvItem.mask |= LVIF_STATE;
		lvItem.state = LVIS_SELECTED;
		lvItem.stateMask = LVIS_SELECTED;
		}
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) return(FALSE);
	//--- School
	lvItem.iSubItem = 1;
	lvItem.pszText = Locale_GetText(pSkill->infos.uSchoolLocaleID);
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0)
		{
		SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEITEM,(WPARAM)lvItem.iItem,0);
		return(FALSE);
		}
	//--- Points
	j = 0;
	if (pSkill->infos.uMemorySlots)
		{
		Misc_Printf(szName,2,L"%i",pSkill->infos.uMemorySlots);
		szName[++j] = 0;
		}
	if (j)
		{
		szName[j++] = L' ';
		szName[j++] = L'|';
		szName[j++] = L' ';
		}
	for (k = 0; k != pSkill->infos.uSourcePoints; k++)
		{
		if (k > 3) break;
		szName[j++] = sSourcePoint;
		}
	if (k) szName[j++] = L' ';
	for (k = 0; k != pSkill->infos.uActionPoints; k++)
		{
		if (k > 10) break;
		szName[j++] = sActionPoint;
		}
	szName[j] = 0;
	lvItem.iSubItem = 2;
	lvItem.pszText = szName;
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == 0)
		{
		SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEITEM,(WPARAM)lvItem.iItem,0);
		return(FALSE);
		}
	//--- Tile info
	uColumns[0] = 1;
	uColumns[1] = 2;
	lvTileInfo.cbSize = sizeof(LVTILEINFO);
	lvTileInfo.iItem = lvItem.iItem;
	lvTileInfo.cColumns = 2;
	lvTileInfo.puColumns = uColumns;
	lvTileInfo.piColFmt = NULL;
	if (SendDlgItemMessage(hDlg,uCtlID,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == FALSE)
		{
		SendDlgItemMessage(hDlg,uCtlID,LVM_DELETEITEM,(WPARAM)lvItem.iItem,0);
		return(FALSE);
		}

	return(TRUE);
}


// «»»» Ajout d'une sélection à la liste ««««««««««««««««««««««««««««««««»

BOOL Game_SkillsAppend(HWND hDlg, UINT uListID, UINT uSort, NODE *pDestRoot, NODE *pSrcRoot)
{
	GAMEDATASKILL*	pSkill;
	GAMEDATASKILL*	pNext;
	BOOL		bAdded;
	int		i;

	i = SendDlgItemMessage(hDlg,uListID,LVM_GETITEMCOUNT,0,0);

	for (pSkill = (GAMEDATASKILL *)pSrcRoot->next; pSkill != NULL; pSkill = pNext)
		{
		bAdded = Game_SkillsInsertItem(hDlg,uListID,i,uSort,pSkill,NULL);
		pNext = (GAMEDATASKILL *)pSkill->node.next;
		if (bAdded)
			{
			List_RemEntry((NODE *)pSkill);
			List_AddEntry((NODE *)pSkill,pDestRoot);
			i++;
			}
		}

	return(1);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Livre de compétences						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_SkillsBookProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMESKILLSCONTEXT*	pCtx;

	if (uMsgId == WM_MEASUREITEM)
		{
		if (Dialog_ViewComboMeasureItem(190,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		if (Dialog_ViewComboMeasureItem(191,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pCtx = (GAMESKILLSCONTEXT *)lParam;

		CheckDlgButton(hDlg,180,App.Config.bSkillsGroups?BST_CHECKED:BST_UNCHECKED);
		SendDlgItemMessage(hDlg,180,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_VIEW_GROUPS));
		SendDlgItemMessage(hDlg,193,WM_SETTEXT,0,(LPARAM)Locale_GetText(MENU_LIST));

		if (!Dialog_ViewComboInit(hDlg,190,App.Config.uSkillsView,2,LV_VIEW_ICON,LV_VIEW_TILE))
			{
			EndDialog(hDlg,-1);
			return(0);
			}
		if (!Game_SkillsSortComboInit(hDlg,191,pCtx->mySkills.uSort))
			{
			EndDialog(hDlg,-1);
			return(0);
			}

		if (!Game_SkillsListCreate(hDlg,200,pCtx->baseSkills.hImageList,&pCtx->mySkills.skills,TEXT_ERR_SKILLS_LIST,pCtx->mySkills.uSort,NULL,TRUE))
			{
			EndDialog(hDlg,IDCANCEL);
			return(FALSE);
			}

		SetDlgItemText(hDlg,300,Locale_GetText(TEXT_DIALOG_BUTTON_ADD));
		SetDlgItemText(hDlg,301,Locale_GetText(TEXT_DIALOG_BUTTON_REMOVE));
		SetDlgItemText(hDlg,302,Locale_GetText(TEXT_SKILLS_OPTIONS));

		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TITLE_SKILLS));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_OK));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CANCEL));
		Dialog_CenterWindow(hDlg,pCtx->mySkills.hwndParent);
		return(FALSE);
		}

	pCtx = (GAMESKILLSCONTEXT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 190:
					Dialog_ViewComboDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 191:
					Game_SkillsSortComboDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 192:
					Game_SkillsDrawPoints(hDlg,200,&pCtx->mySkills.skills,pCtx->mySkills.pSelected,(DRAWITEMSTRUCT *)lParam,TRUE);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 190:
							Dialog_ViewComboChanged(hDlg,190,200,&App.Config.uSkillsView);
							return(TRUE);
						case 191:
							Game_SkillsSortComboChanged(hDlg,191,200,&pCtx->mySkills.uSort);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 180:
							App.Config.bSkillsGroups = (IsDlgButtonChecked(hDlg,180) == BST_CHECKED?TRUE:FALSE);
							SendDlgItemMessage(hDlg,200,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bSkillsGroups,0);
							return(TRUE);
						case 193:
							Game_SkillsMenu(hDlg,200,(HWND)lParam);
							return(TRUE);
						case 300:
							pCtx->baseSkills.hwndParent = hDlg;
							if (Game_SkillsCopyList(&pCtx->selectSkills,&pCtx->baseSkills,&pCtx->mySkills,SkillsBlackList))
								{
								if (Game_SkillsSelect(hDlg,&pCtx->selectSkills)) Game_SkillsAppend(hDlg,200,pCtx->mySkills.uSort,&pCtx->mySkills.skills,&pCtx->selectSkills.selection);
								Game_UnloadDataFile(DATA_TYPE_SKILLS,&pCtx->selectSkills.selection);
								Game_UnloadDataFile(DATA_TYPE_SKILLS,&pCtx->selectSkills.skills);
								}
							return(TRUE);
						case 301:
							Game_SkillsBookDelete(hDlg,200);
							return(TRUE);
						case 302:
							Game_SkillsOptions(hDlg,200,&pCtx->options);
							return(TRUE);
						case IDOK:
							if (Game_SkillsBookRebuild(hDlg,TEXT_ERR_SKILLS_BUILD,App.Game.pdcCurrent,&pCtx->mySkills))
								EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
							EndDialog(hDlg,IDCANCEL);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_KEYDOWN:
					if (((NMLVKEYDOWN *)lParam)->wVKey == VK_DELETE) Game_SkillsBookDelete(hDlg,200);
					break;
				case LVN_ITEMACTIVATE:
					Game_SkillsOptions(hDlg,200,&pCtx->options);
				case LVN_ITEMCHANGED:
					Game_SkillsSelectionCollect(hDlg,200,(NMLISTVIEW *)lParam,&pCtx->mySkills);
					InvalidateRect(GetDlgItem(hDlg,192),NULL,FALSE);
					EnableWindow(GetDlgItem(hDlg,301),pCtx->mySkills.pSelected?TRUE:FALSE);
					EnableWindow(GetDlgItem(hDlg,302),pCtx->mySkills.pSelected?TRUE:FALSE);
					break;
				}
			break;

		case WM_CLOSE:
			EndDialog(hDlg,IDCANCEL);
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Compétences du personnage «««««««««««««««««««««««««««««««««««««««»

int Game_SkillsBookCreate(HWND hWnd, UINT uErrorID, DOS2CHARACTER *pdc, GAMESKILLSCONTEXT *pctx)
{
	GAMEDATASKILL*	pSkill;
	GAMEDATASKILL*	pBaseSkill;
	XML_NODE*	pxnSkill;
	XML_NODE*	pxnAttribute;
	WCHAR*		pszValue;
	int		i;

	static WCHAR*	szSkillAttributes[] = { L"MapKey", L"IsLearned", L"IsActivated", L"ZeroMemory", L"ActiveCooldown", L"ScoreModifier", L"StartRound", L"MinimumImpact", L"CasualExplorer", L"Classic", L"TacticianHardcore", L"HonorHardcore", L"OnlyCastOnSelf", L"AIFlags", NULL };

	#if _DEBUG
	if (!pdc) return(1);
	#else
	if (!pdc) return(0);
	#endif

	//--- Création de la liste des compétences ---

	pxnSkill = xml_GetNodeFromPathFirstChild((XML_NODE *)pdc->pxnRoot->children.next,szSkillManagerSkillsPath);
	if (pxnSkill)
		{
		//--- Liste des compétences ---

		for (; pxnSkill != NULL; pxnSkill = (XML_NODE *)pxnSkill->node.next)
			{
			//--- Vérifications ---

			// Vérifie qu'il s'agit d'une node
			if (wcscmp(szXMLnode,pxnSkill->name)) continue;

			// Retrouve l'identifiant
			pszValue = xml_GetAttrValue(pxnSkill,szXMLid);
			if (!pszValue) continue;

			// Vérifie s'il s'agit d'une date (TimeItemAddedToSkillManager)
			if (!wcscmp(L"TimeItemAddedToSkillManager",pszValue))
				{
				WCHAR*	pszMapKey;
				WCHAR*	pszMapValue;

				pszMapKey = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnSkill->children.next,szXMLattribute,szXMLid,L"MapKey"));
				if (pszMapKey)
					{
					pszMapValue = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnSkill->children.next,szXMLattribute,szXMLid,L"MapValue"));
					if (pszMapValue)
						{
						pBaseSkill = Game_SkillsGetById(pszMapKey,&pctx->mySkills.skills);
						if (pBaseSkill) pBaseSkill->options.fTimeAdded = wcstod(pszMapValue,NULL);
						}
					}
				continue;
				}

			// Vérifie qu'il s'agit bien d'une compétence (Skills)
			if (wcscmp(szXMLSkills,pszValue)) continue;

			//--- Ajoute la compétence ---

			pSkill = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEDATASKILL));
			if (!pSkill) goto Error;

			pSkill->options.bIsLearned = TRUE;
			pSkill->infos.uSchoolLocaleID = TEXT_ABILITIES_UNKNOWN;
			List_AddEntry((NODE *)pSkill,&pctx->mySkills.skills);

			//--- Paramètres de la compétence ---

			for (pxnAttribute = (XML_NODE *)pxnSkill->children.next; pxnAttribute != NULL; pxnAttribute = (XML_NODE *)pxnAttribute->node.next)
				{
				// Vérifie qu'il s'agit bien d'un attribut
				if (wcscmp(szXMLattribute,pxnAttribute->name)) continue;

				// Retrouve l'identifiant
				pszValue = xml_GetAttrValue(pxnAttribute,szXMLid);
				if (!pszValue) continue;

				// Vérifie l'identifiant et obtient son index
				for (i = 0; szSkillAttributes[i] != NULL; i++)
					if (!wcscmp(szSkillAttributes[i],pszValue)) break;

				#if _DEBUG
				if (szSkillAttributes[i] == NULL) Request_MessageBoxEx(hWnd,L"Skill :: Unknown attribute[id=%1]",NULL,MB_ICONINFORMATION|MB_OK,pszValue);
				#endif

				// Vérifie si l'identifiant est connu
				if (szSkillAttributes[i] == NULL) continue;

				// Récupère la valeur
				pszValue = xml_GetAttrValue(pxnAttribute,szXMLvalue);
				if (!pszValue) continue;

				switch(i)
					{
					case 0: // MapKey
						if (pSkill->pszId) HeapFree(App.hHeap,0,pSkill->pszId); // should not happen
						pSkill->pszId = Misc_StrCpyAlloc(pszValue);
						break;
					case 1:	// IsLearned
						pSkill->options.bIsLearned = xml_IsValueTrue(pszValue);
						break;
					case 2:	// IsActivated
						pSkill->options.bIsActivated = xml_IsValueTrue(pszValue);
						break;
					case 3:	// ZeroMemory
						pSkill->options.bZeroMemory = xml_IsValueTrue(pszValue);
						break;
					case 4:	// Active Cooldown
						pSkill->options.fActiveCooldown = wcstof(pszValue,NULL);
						break;
					}
				}
			}
		}

	//--- Initialisations finales & Vérifications ---

	for (pSkill = (GAMEDATASKILL *)pctx->mySkills.skills.next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
		{
		if (!pSkill->pszId) goto Error;
		pBaseSkill = Game_SkillsGetById(pSkill->pszId,&pctx->baseSkills.skills);
		if (pBaseSkill) Game_SkillsCopyPaste(pSkill,pBaseSkill,NULL,FALSE); // Errors here are harmless
		}

	return(1);

Error:	Request_PrintError(hWnd,Locale_GetText(uErrorID),NULL,MB_ICONERROR);
	Game_UnloadDataFile(DATA_TYPE_SKILLS,&pctx->mySkills.skills);
	return(0);
}


// «»»» Recréation des compétences du personnage ««««««««««««««««««««««««»

int Game_SkillsBookRebuild(HWND hDlg, UINT uMsgID, DOS2CHARACTER *pdc, GAMEEDITSKILLCONTEXT *pSkills)
{
	NODE		nodeManager;
	XML_NODE*	pxn;
	XML_NODE*	pxnParent;
	XML_NODE*	pxnManager;
	GAMEDATASKILL*	pSkill;
	static WCHAR*	pSkillArray[] = {	szXMLnode, szXMLid, L"Skills", (WCHAR *)1,
							szXMLattribute, szXMLid, L"MapKey", szXMLvalue, L"%1", szXMLtype, L"22", (WCHAR *)0,
							szXMLattribute, szXMLid, L"IsLearned", szXMLvalue, L"%2", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"IsActivated", szXMLvalue, L"%3", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"ZeroMemory", szXMLvalue, L"%4", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"ActiveCooldown", szXMLvalue, L"%5", szXMLtype, L"6", (WCHAR *)0,
							szXMLattribute, szXMLid, L"ScoreModifier", szXMLvalue, L"1", szXMLtype, L"6", (WCHAR *)0,
							szXMLattribute, szXMLid, L"StartRound", szXMLvalue, L"1", szXMLtype, L"4", (WCHAR *)0,
							szXMLattribute, szXMLid, L"MinimumImpact", szXMLvalue, L"0", szXMLtype, L"4", (WCHAR *)0,
							szXMLattribute, szXMLid, L"CasualExplorer", szXMLvalue, L"True", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"Classic", szXMLvalue, L"True", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"TacticianHardcore", szXMLvalue, L"True", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"HonorHardcore", szXMLvalue, L"True", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"OnlyCastOnSelf", szXMLvalue, L"False", szXMLtype, L"19", (WCHAR *)0,
							szXMLattribute, szXMLid, L"AIFlags", szXMLvalue, L"0", szXMLtype, L"1", (WCHAR *)0,
							L"children", (WCHAR *)1,
								szXMLnode, szXMLid, L"SourceConditions", (WCHAR *)1,
									szXMLattribute, szXMLid, L"MinimumHealthPercentage", szXMLvalue, L"0", szXMLtype, L"4", (WCHAR *)0,
									szXMLattribute, szXMLid, L"MaximumHealthPercentage", szXMLvalue, L"100", szXMLtype, L"4", (WCHAR *)0,
									szXMLattribute, szXMLid, L"HasNoPhysicalArmor", szXMLvalue, L"False", szXMLtype, L"19", (WCHAR *)0,
									szXMLattribute, szXMLid, L"HasNoMagicalArmor", szXMLvalue, L"False", szXMLtype, L"19", (WCHAR *)0,
									L"children", (WCHAR *)1,
										szXMLnode, szXMLid, L"Tags", (WCHAR *)0,
										(WCHAR *)-1,
									(WCHAR *)-1,
								szXMLnode, szXMLid, L"TargetConditions", (WCHAR *)1,
									szXMLattribute, szXMLid, L"MinimumHealthPercentage", szXMLvalue, L"0", szXMLtype, L"4", (WCHAR *)0,
									szXMLattribute, szXMLid, L"MaximumHealthPercentage", szXMLvalue, L"100", szXMLtype, L"4", (WCHAR *)0,
									szXMLattribute, szXMLid, L"HasNoPhysicalArmor", szXMLvalue, L"False", szXMLtype, L"19", (WCHAR *)0,
									szXMLattribute, szXMLid, L"HasNoMagicalArmor", szXMLvalue, L"False", szXMLtype, L"19", (WCHAR *)0,
									L"children", (WCHAR *)1,
										szXMLnode, szXMLid, L"Tags", (WCHAR *)0,
									(WCHAR *)-2
						};
	static WCHAR*	pSkillTimeArray[] = {	szXMLnode, szXMLid, L"TimeItemAddedToSkillManager", (WCHAR *)1,
							szXMLattribute, szXMLid, L"MapKey", szXMLvalue, L"%1", szXMLtype, L"22", (WCHAR *)0,
							szXMLattribute, szXMLid, L"MapValue", szXMLvalue, L"%2", szXMLtype, L"7", (WCHAR *)-2
						};

	WCHAR		szBuffer[40];
	WCHAR*		pArgs[6];
	WCHAR**		pSkillArrayPtr;
	WCHAR**		pSkillTimeArrayPtr;
	WCHAR**		pArrayPtr;

	#if _DEBUG
	if (!pdc) return(1);
	#endif

	ZeroMemory(&nodeManager,sizeof(NODE));
	pSkillArrayPtr = pSkillTimeArrayPtr = NULL;
	pxnParent = NULL;

	pSkillArrayPtr = HeapAlloc(App.hHeap,0,sizeof(pSkillArray));
	if (!pSkillArrayPtr) goto Error;
	pSkillTimeArrayPtr = HeapAlloc(App.hHeap,0,sizeof(pSkillTimeArray));
	if (!pSkillTimeArrayPtr) goto Error;

	pxnManager = xml_GetNodeFromPath((XML_NODE *)pdc->pxnRoot->children.next,szSkillManagerPath);
	if (!pxnManager) goto Error;

	//--- Création de la nouvelle liste ---

	if (List_EntryCount(&pSkills->skills))
		{

		//--- Création du noeud enfant ---

		pxnParent = xml_CreateNode(L"children",pxnManager,0);
		if (!pxnParent) goto Error;
		List_Append((NODE *)pxnParent,&nodeManager);

		//--- Création de la liste des compétences ---

		for (pSkill = (GAMEDATASKILL *)pSkills->skills.next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
			{
			pArgs[0] = pSkill->pszId;
			pArgs[1] = pSkill->options.bIsLearned?szXMLTrue:szXMLFalse;
			pArgs[2] = pSkill->options.bIsActivated?szXMLTrue:szXMLFalse;
			pArgs[3] = pSkill->options.bZeroMemory?szXMLTrue:szXMLFalse;
			pArgs[4] = szBuffer;
			Misc_Printf(szBuffer,40,L"%f",pSkill->options.fActiveCooldown);
			Game_SkillsBookRebuildCopyParams(pSkillArrayPtr,pSkillArray,pArgs);

			pArrayPtr = pSkillArrayPtr;
			pxn = xml_CreateNodeArray(&pArrayPtr,pxnParent);
			if (!pxn) goto Error;
			List_Append((NODE *)pxn,&pxnParent->children);
			}

		//--- Création des dates ---

		for (pSkill = (GAMEDATASKILL *)pSkills->skills.next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
			{
			pArgs[0] = pSkill->pszId;
			pArgs[1] = szBuffer;
			Misc_Printf(szBuffer,40,L"%f",pSkill->options.fTimeAdded);
			Game_SkillsBookRebuildCopyParams(pSkillTimeArrayPtr,pSkillTimeArray,pArgs);

			pArrayPtr = pSkillTimeArrayPtr;
			pxn = xml_CreateNodeArray(&pArrayPtr,pxnParent);
			if (!pxn) goto Error;
			List_Append((NODE *)pxn,&pxnParent->children);
			}
		}

	if (pSkillTimeArrayPtr) HeapFree(App.hHeap,0,pSkillTimeArrayPtr);
	if (pSkillArrayPtr) HeapFree(App.hHeap,0,pSkillArrayPtr);

	//--- Destruction et remplacement de l'ancienne liste ---

	xml_ReleaseAll(&pxnManager->children);
	List_Append((NODE *)pxnParent,&pxnManager->children);
	return(1);

	//--- Erreur ---

Error:	Request_PrintError(hDlg,Locale_GetText(uMsgID),NULL,MB_OK);
	if (pSkillTimeArrayPtr) HeapFree(App.hHeap,0,pSkillTimeArrayPtr);
	if (pSkillArrayPtr) HeapFree(App.hHeap,0,pSkillArrayPtr);
	xml_ReleaseAll(&nodeManager);
	return(0);
}

//--- Modifie les arguments ---

void Game_SkillsBookRebuildCopyParams(WCHAR *pDestArray[], WCHAR *pSourceArray[], WCHAR *pArgs[])
{
	int	i;
	int	iIndex;

	for (i = 0; pSourceArray[i] != (WCHAR *)-2; i++)
		{
		pDestArray[i] = pSourceArray[i];
		if (pSourceArray[i] == (WCHAR *)0) continue;
		if (pSourceArray[i] == (WCHAR *)1) continue;
		if (pSourceArray[i] == (WCHAR *)-1) continue;
		if (*(pSourceArray[i]) != L'%') continue;
		iIndex = wcstol(&pSourceArray[i][1],NULL,10);
		pDestArray[i] = pArgs[iIndex-1];
		}

	pDestArray[i] = (WCHAR *)-2;
	return;
}


// «»»» Suppression des compétences du personnage «««««««««««««««««««««««»

void Game_SkillsBookDelete(HWND hDlg, UINT uListID)
{
	LVITEM		lvItem;
	GAMEDATASKILL*	pSkill;

	while ((lvItem.iItem = SendDlgItemMessage(hDlg,uListID,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_SELECTED,0))) != -1)
		{
		lvItem.mask = LVIF_PARAM;
		lvItem.iSubItem = 0;
		lvItem.lParam = 0;
		if (!SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem)) break;
		if (!SendDlgItemMessage(hDlg,uListID,LVM_DELETEITEM,(WPARAM)lvItem.iItem,0)) break;

		pSkill = (GAMEDATASKILL *)lvItem.lParam;
		if (pSkill->pszId) HeapFree(App.hHeap,0,pSkill->pszId);
		if (pSkill->pszName) HeapFree(App.hHeap,0,pSkill->pszName);
		List_RemEntry((NODE *)pSkill);
		HeapFree(App.hHeap,0,pSkill);
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Options des compétences						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void Game_SkillsOptions(HWND hDlg, UINT uListID, GAMESKILLOPTIONS *pOptions)
{
	LVITEM		lvItem;
	INT_PTR		iResult;
	int		n,i;

	n = SendDlgItemMessage(hDlg,uListID,LVM_GETSELECTEDCOUNT,0,0);
	if (!n) return;

	pOptions->pSelected = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,(n+1)*sizeof(GAMEDATASKILL *));
	if (!pOptions->pSelected)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return;
		}

	//--- Collecte la liste des compétences sélectionnées ---

	i = 0;
	lvItem.iItem = -1;
	while ((lvItem.iItem = SendDlgItemMessage(hDlg,uListID,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)MAKELPARAM(LVNI_SELECTED,0))) != -1)
		{
		if (i >= n) break;
		lvItem.mask = LVIF_PARAM;
		lvItem.iSubItem = 0;
		lvItem.lParam = 0;
		if (!SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem))
			{
			Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
			goto Done;
			}
		pOptions->pSelected[i++] = (GAMEDATASKILL *)lvItem.lParam;
		}

	//--- Options ---

	pOptions->hwndParent = hDlg;
	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1104),hDlg,Game_SkillsOptionsProc,(LPARAM)pOptions);
	if (iResult == -1) Request_PrintError(hDlg,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);

Done:	HeapFree(App.hHeap,0,pOptions->pSelected);
	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_SkillsOptionsProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMESKILLOPTIONS*	pOptions;

	if (uMsgId == WM_INITDIALOG)
		{
		RECT	rcDialog;
		int	iHeight;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pOptions = (GAMESKILLOPTIONS *)lParam;

		//--- Dimensions ---

		iHeight = Dialog_GetInfoHeight(hDlg,100,Locale_GetText(TEXT_SKILLS_OPTIONS_INFO));
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+iHeight,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,iHeight,SWP_NOZORDER|SWP_NOMOVE);

		Dialog_OffsetY(hDlg,200,iHeight);
		Dialog_OffsetY(hDlg,201,iHeight);
		Dialog_OffsetY(hDlg,202,iHeight);
		Dialog_OffsetY(hDlg,IDOK,iHeight);
		Dialog_OffsetY(hDlg,IDCANCEL,iHeight);

		//--- Valeurs par défaut ---

		Game_SkillsOptionsGet(hDlg,200,SKILL_OPTION_ISLEARNED,pOptions);
		Game_SkillsOptionsGet(hDlg,201,SKILL_OPTION_ISACTIVATED,pOptions);
		Game_SkillsOptionsGet(hDlg,202,SKILL_OPTION_ZEROMEMORY,pOptions);

		//--- Textes ---

		SetWindowText(hDlg,Locale_GetText(TEXT_SKILLS_OPTIONS));
		SetDlgItemText(hDlg,200,Locale_GetText(TEXT_SKILLS_OPTIONS_ISLEARNED));
		SetDlgItemText(hDlg,201,Locale_GetText(TEXT_SKILLS_OPTIONS_ISACTIVATED));
		SetDlgItemText(hDlg,202,Locale_GetText(TEXT_SKILLS_OPTIONS_ZEROMEMORY));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_OK));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CANCEL));

		Dialog_CenterWindow(hDlg,pOptions->hwndParent);
		return(FALSE);
		}

	pOptions = (GAMESKILLOPTIONS *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(Locale_GetText(TEXT_SKILLS_OPTIONS_INFO),(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Game_SkillsOptionsSet(hDlg,200,SKILL_OPTION_ISLEARNED,pOptions);
							Game_SkillsOptionsSet(hDlg,201,SKILL_OPTION_ISACTIVATED,pOptions);
							Game_SkillsOptionsSet(hDlg,202,SKILL_OPTION_ZEROMEMORY,pOptions);
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


// «»»» Récupération des options ««««««««««««««««««««««««««««««««««««««««»

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
void Game_SkillsOptionsGet(HWND hDlg, UINT uCtlId, UINT uType, GAMESKILLOPTIONS *pOptions)
{
	int*	pOption;
	int	iState;
	int	i;

	switch(uType)
		{
		case SKILL_OPTION_ISLEARNED:
			pOption = &pOptions->iIsLearned;
			break;
		case SKILL_OPTION_ISACTIVATED:
			pOption = &pOptions->iIsActivated;
			break;
		case SKILL_OPTION_ZEROMEMORY:
			pOption = &pOptions->iZeroMemory;
			break;
		default:return;
		}

	*pOption = BST_INDETERMINATE;

	for (i = 0; pOptions->pSelected[i] != NULL; i++)
		{
		switch(uType)
			{
			case SKILL_OPTION_ISLEARNED:
				iState = pOptions->pSelected[i]->options.bIsLearned?BST_CHECKED:BST_UNCHECKED;
				break;
			case SKILL_OPTION_ISACTIVATED:
				iState = pOptions->pSelected[i]->options.bIsActivated?BST_CHECKED:BST_UNCHECKED;
				break;
			case SKILL_OPTION_ZEROMEMORY:
				iState = pOptions->pSelected[i]->options.bZeroMemory?BST_CHECKED:BST_UNCHECKED;
				break;
			}
		if (*pOption == BST_INDETERMINATE)
			{
			*pOption = iState;
			continue;
			}
		if (*pOption != iState)
			{
			*pOption = BST_INDETERMINATE;
			break;
			}
		}

	CheckDlgButton(hDlg,uCtlId,*pOption);
	return;
}
#pragma GCC diagnostic pop


// «»»» Application des options «««««««««««««««««««««««««««««««««««««««««»

void Game_SkillsOptionsSet(HWND hDlg, UINT uCtlId, UINT uType, GAMESKILLOPTIONS *pOptions)
{
	int	iState;
	int	i;

	iState = IsDlgButtonChecked(hDlg,uCtlId);
	if (iState == BST_INDETERMINATE) return;

	for (i = 0; pOptions->pSelected[i] != NULL; i++)
		{
		switch(uType)
			{
			case SKILL_OPTION_ISLEARNED:
				pOptions->pSelected[i]->options.bIsLearned = iState == BST_CHECKED?TRUE:FALSE;
				break;
			case SKILL_OPTION_ISACTIVATED:
				pOptions->pSelected[i]->options.bIsActivated = iState == BST_CHECKED?TRUE:FALSE;
				break;
			case SKILL_OPTION_ZEROMEMORY:
				pOptions->pSelected[i]->options.bZeroMemory = iState == BST_CHECKED?TRUE:FALSE;
				break;
			}
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection de compétence						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

UINT Game_SkillsSelect(HWND hWnd, GAMEEDITSKILLCONTEXT *pSkillEdit)
{
	INT_PTR		iResult;

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1103),hWnd,Game_SkillsSelectProc,(LPARAM)pSkillEdit);
	if (iResult == -1)
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return(0);
		}
	if (iResult != IDOK)
		{
		Game_UnloadDataFile(DATA_TYPE_SKILLS,&pSkillEdit->selection);
		return(0);
		}

	return(List_EntryCount(&pSkillEdit->selection));
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_SkillsSelectProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMEEDITSKILLCONTEXT*	pSkillEdit;

	if (uMsgId == WM_MEASUREITEM)
		{
		if (Dialog_ViewComboMeasureItem(190,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		if (Dialog_ViewComboMeasureItem(191,(MEASUREITEMSTRUCT *)lParam)) return(TRUE);
		return(TRUE);
		}

	if (uMsgId == WM_INITDIALOG)
		{
		RECT	rcDialog;
		int	iHeight;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pSkillEdit = (GAMEEDITSKILLCONTEXT *)lParam;

		iHeight = Dialog_GetInfoHeight(hDlg,100,Locale_GetText(TEXT_SKILLS_SELECT_INFO));
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+iHeight,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,iHeight,SWP_NOZORDER|SWP_NOMOVE);

		Dialog_OffsetY(hDlg,180,iHeight);
		Dialog_OffsetY(hDlg,190,iHeight);
		Dialog_OffsetY(hDlg,191,iHeight);
		Dialog_OffsetY(hDlg,192,iHeight);
		Dialog_OffsetY(hDlg,193,iHeight);
		Dialog_OffsetY(hDlg,200,iHeight);
		Dialog_OffsetY(hDlg,999,iHeight);
		Dialog_OffsetY(hDlg,IDOK,iHeight);
		Dialog_OffsetY(hDlg,IDCANCEL,iHeight);

		CheckDlgButton(hDlg,180,App.Config.bSkillsGroups?BST_CHECKED:BST_UNCHECKED);
		SendDlgItemMessage(hDlg,180,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_VIEW_GROUPS));
		SendDlgItemMessage(hDlg,193,WM_SETTEXT,0,(LPARAM)Locale_GetText(MENU_LIST));

		if (!Dialog_ViewComboInit(hDlg,190,App.Config.uSkillsView,2,LV_VIEW_ICON,LV_VIEW_TILE))
			{
			EndDialog(hDlg,-1);
			return(0);
			}
		if (!Game_SkillsSortComboInit(hDlg,191,pSkillEdit->uSort))
			{
			EndDialog(hDlg,-1);
			return(0);
			}

		if (!Game_SkillsListCreate(hDlg,200,pSkillEdit->hImageList,&pSkillEdit->skills,TEXT_ERR_DIALOG,pSkillEdit->uSort,pSkillEdit->pSelected,TRUE))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}

		EnableWindow(GetDlgItem(hDlg,IDOK),pSkillEdit->pSelected?TRUE:FALSE);

		SetWindowText(hDlg,Locale_GetText(TEXT_DIALOG_TITLE_SKILLS));
		SetDlgItemText(hDlg,IDOK,Locale_GetText(TEXT_OK));
		SetDlgItemText(hDlg,IDCANCEL,Locale_GetText(TEXT_CANCEL));

		Dialog_CenterWindow(hDlg,pSkillEdit->hwndParent);
		return(FALSE);
		}

	pSkillEdit = (GAMEEDITSKILLCONTEXT *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(Locale_GetText(TEXT_SKILLS_SELECT_INFO),(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				case 190:
					Dialog_ViewComboDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 191:
					Game_SkillsSortComboDrawItem((DRAWITEMSTRUCT *)lParam);
					return(TRUE);
				case 192:
					Game_SkillsDrawPoints(hDlg,200,&pSkillEdit->skills,pSkillEdit->pSelected,(DRAWITEMSTRUCT *)lParam,TRUE);
					return(TRUE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					switch(LOWORD(wParam))
						{
						case 190:
							Dialog_ViewComboChanged(hDlg,190,200,&App.Config.uSkillsView);
							return(TRUE);
						case 191:
							Game_SkillsSortComboChanged(hDlg,191,200,&pSkillEdit->uSort);
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case 180:
							App.Config.bSkillsGroups = (IsDlgButtonChecked(hDlg,180) == BST_CHECKED?TRUE:FALSE);
							SendDlgItemMessage(hDlg,200,LVM_ENABLEGROUPVIEW,(WPARAM)App.Config.bSkillsGroups,0);
							return(TRUE);
						case 193:
							Game_SkillsMenu(hDlg,200,(HWND)lParam);
							return(TRUE);
						case IDOK:
							if (!Game_SkillsSelectionCollect(hDlg,200,NULL,pSkillEdit)) break;
							EndDialog(hDlg,IDOK);
							return(TRUE);
						case IDCANCEL:
							EndDialog(hDlg,IDCANCEL);
							return(TRUE);
						}
					break;
				}
			break;

		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
				{
				case LVN_ITEMACTIVATE:
					Game_SkillsSelectionCollect(hDlg,200,NULL,pSkillEdit);
					InvalidateRect(GetDlgItem(hDlg,192),NULL,FALSE);
					EnableWindow(GetDlgItem(hDlg,IDOK),pSkillEdit->pSelected?TRUE:FALSE);
					if (pSkillEdit->pSelected) EndDialog(hDlg,IDOK);
					break;
				case LVN_ITEMCHANGED:
					Game_SkillsSelectionCollect(hDlg,200,(NMLISTVIEW *)lParam,pSkillEdit);
					InvalidateRect(GetDlgItem(hDlg,192),NULL,FALSE);
					EnableWindow(GetDlgItem(hDlg,IDOK),pSkillEdit->pSelected?TRUE:FALSE);
					break;
				}
			break;

		case WM_CLOSE:
			EndDialog(hDlg,IDCANCEL);
			return(TRUE);
		}

	return(FALSE);
}


// «»»» Sélection «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int Game_SkillsSelectionCollect(HWND hDlg, UINT uListID, NMLISTVIEW *nmListView, GAMEEDITSKILLCONTEXT *pSkillEdit)
{
	GAMEDATASKILL*	pSkill;
	LVITEM		lvItem;
	UINT		uCount;

	//--- Uniquement la dernière compétence sélectionnée ---

	if (nmListView)
		{
		pSkillEdit->pSelected = NULL;
		if (nmListView->iItem != -1)
			{
			lvItem.mask = LVIF_PARAM|LVIF_STATE;
			lvItem.iItem = nmListView->iItem;
			lvItem.iSubItem = 0;
			lvItem.state = 0;
			lvItem.stateMask = LVIS_SELECTED;
			lvItem.lParam = 0;
			if (SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem))
				{
				if (lvItem.state) pSkillEdit->pSelected = (GAMEDATASKILL *)lvItem.lParam;
				else
					{
					lvItem.iItem = SendDlgItemMessage(hDlg,uListID,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_SELECTED,0));
					if (lvItem.iItem != -1)
						{
						lvItem.mask = LVIF_PARAM;
						lvItem.lParam = 0;
						if (SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem)) pSkillEdit->pSelected = (GAMEDATASKILL *)lvItem.lParam;
						}
					}
				}
			}
		return(1);
		}

	//--- Collecte les compétences sélectionnée ---

	Game_UnloadDataFile(DATA_TYPE_SKILLS,&pSkillEdit->selection);
	uCount = SendDlgItemMessage(hDlg,uListID,LVM_GETSELECTEDCOUNT,0,0);
	if (!uCount) return(1);

	pSkillEdit->pSelected = pSkill = NULL;
	lvItem.iItem = -1;

	while ((lvItem.iItem = SendDlgItemMessage(hDlg,uListID,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)MAKELPARAM(LVNI_SELECTED,0))) != -1)
		{
		lvItem.mask = LVIF_PARAM;
		lvItem.iSubItem = 0;
		lvItem.lParam = 0;
		if (!SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem))
			{
			Game_UnloadDataFile(DATA_TYPE_SKILLS,&pSkillEdit->selection);
			return(0);
			}
		pSkill = Game_SkillsCopyPaste(NULL,(GAMEDATASKILL *)lvItem.lParam,&pSkillEdit->selection,TRUE);
		if (!pSkill)
			{
			Game_UnloadDataFile(DATA_TYPE_SKILLS,&pSkillEdit->selection);
			return(0);
			}
		}

	pSkillEdit->pSelected = pSkill;
	return(1);
}


// «»»» Affichage des points ««««««««««««««««««««««««««««««««««««««««««««»

void Game_SkillsDrawPoints(HWND hDlg, UINT uListID, NODE *pRoot, GAMEDATASKILL *pSelected, DRAWITEMSTRUCT *pDraw, BOOL bDrawMemorySlots)
{
	HIMAGELIST	hImageList;
	RECT		rcItem;
	UINT		uSourcePts;
	UINT		uActionPts;
	UINT		uMemorySlots;
	int		X,Y,i;

	if (!pSelected) pSelected = Game_SkillsGetSelected(hDlg,uListID);

	if (pSelected)
		{
		uSourcePts = pSelected->infos.uSourcePoints;
		uActionPts = pSelected->infos.uActionPoints;
		uMemorySlots = pSelected->infos.uMemorySlots;
		}
	else
		{
		uSourcePts = 0;
		uActionPts = 0;
		uMemorySlots = 0;
		}

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	X = pDraw->rcItem.left;

	//--- Skill ---
	if (pSelected && (hImageList = (HIMAGELIST)SendDlgItemMessage(hDlg,uListID,LVM_GETIMAGELIST,(WPARAM)LVSIL_NORMAL,0)))
		{
		IMAGELISTDRAWPARAMS	imldp;
		int			H;

		H = pDraw->rcItem.bottom-pDraw->rcItem.top;
		if (H > 64) H = 64;
		Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-H)/2;

		imldp.cbSize = sizeof(IMAGELISTDRAWPARAMS);
		imldp.himl = hImageList;
		imldp.i = pSelected->iIconIndex;
		imldp.hdcDst = pDraw->hDC;
		imldp.x = X;
		imldp.y = Y;
		imldp.cx = H;
		imldp.cy = H;
		imldp.xBitmap = 0;
		imldp.yBitmap = 0;
		imldp.rgbBk = CLR_NONE;
		imldp.rgbFg = CLR_NONE;
		imldp.fStyle = ILD_SCALE;
		imldp.dwRop = SRCCOPY;
		imldp.fState = ILS_NORMAL;
		imldp.Frame = 0;
		imldp.crEffect = 0;

		if (imldp.i != -1)
			{
			ImageList_DrawIndirect(&imldp);
			X += H+8;
			}
		}

	//--- Separator ---
	CopyRect(&rcItem,&pDraw->rcItem);
	rcItem.left = X;
	rcItem.right = X+2;
	DrawEdge(pDraw->hDC,&rcItem,EDGE_SUNKEN,BF_LEFT|BF_RIGHT);
	X += 8;

	//--- Source points ---
	Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-20)/2;
	for (i = 1; i != 4; i++)
		{
		DrawIconEx(pDraw->hDC,X,Y,App.hIcons[i > uSourcePts?APP_ICON_SOURCE_OFF:APP_ICON_SOURCE_ON],20,20,0,NULL,DI_NORMAL);
		X += 20;
		}

	//--- Action points ---
	Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-24)/2;
	for (i = 1; i != 7; i++)
		{
		DrawIconEx(pDraw->hDC,X,Y,App.hIcons[i > uActionPts?APP_ICON_ACTION_OFF:APP_ICON_ACTION_ON],24,24,0,NULL,DI_NORMAL);
		X += 22;
		}

	//--- Memory slots ---
	if (bDrawMemorySlots && uMemorySlots)
		{
		X = pDraw->rcItem.right-24;
		Y = pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-24)/2;
		while (uMemorySlots--)
			{
			DrawIconEx(pDraw->hDC,X,Y,App.hIcons[APP_ICON_STAT_MEM],24,24,0,NULL,DI_NORMAL);
			X -= 24;
			}
		}

	return;
}


// «»»» Menu de la liste ««««««««««««««««««««««««««««««««««««««««««««««««»

void Game_SkillsMenu(HWND hDlg, UINT uListID, HWND hwndCtrl)
{
	GAMEDATASKILL*	pSelected;
	HMENU		hMenu;
	UINT		uMenuId = 0;

	pSelected = Game_SkillsGetSelected(hDlg,uListID);
	hMenu = Menu_Create(SkillsListMenu);
	if (hMenu)
		{
		HMENU		hSubMenu;
		POINT		ptCursor;
		UINT		uFlags;
		BOOL		bGroupView;

		if (hwndCtrl)
			{
			RECT	rcWindow;

			GetWindowRect(hwndCtrl,&rcWindow);
			ptCursor.x = rcWindow.left;
			ptCursor.y = rcWindow.top;
			uFlags = TPM_LEFTALIGN|TPM_TOPALIGN;
			}
		else
			{
			GetCursorPos(&ptCursor);
			uFlags = TPM_LEFTALIGN|TPM_TOPALIGN;
			}

		bGroupView = SendDlgItemMessage(hDlg,uListID,LVM_ISGROUPVIEWENABLED,0,0);
		EnableMenuItem(hMenu,IDM_LIST_SKILLSELECT,MF_BYCOMMAND|(pSelected?0:MF_GRAYED));
		EnableMenuItem(hMenu,IDM_LIST_COLLAPSE,MF_BYCOMMAND|(bGroupView?0:MF_GRAYED));
		EnableMenuItem(hMenu,IDM_LIST_EXPAND,MF_BYCOMMAND|(bGroupView?0:MF_GRAYED));

		hSubMenu = GetSubMenu(hMenu,0);
		uMenuId = TrackPopupMenu(hSubMenu,uFlags|TPM_NOANIMATION|TPM_NONOTIFY|TPM_RETURNCMD,ptCursor.x,ptCursor.y,0,App.hWnd,NULL);
		Menu_Release(hMenu,SkillsListMenu);
		}

	switch(uMenuId)
		{
		case IDM_LIST_SKILLSELECT:
			if (!pSelected) break;
			Game_SkillsCollapseSelectedGroup(hDlg,uListID,pSelected);
			Game_SkillsEnsureVisible(hDlg,uListID,pSelected);
			SetFocus(GetDlgItem(hDlg,uListID));
			break;

		case IDM_LIST_COLLAPSE:
			Game_SkillsToggleGroups(hDlg,uListID,FALSE);
			break;
		case IDM_LIST_EXPAND:
			Game_SkillsToggleGroups(hDlg,uListID,TRUE);
			break;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Tri								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialise les tris «««««««««««««««««««««««««««««««««««««««««««««»

int Game_SkillsSortComboInit(HWND hDlg, UINT uCtlID, UINT uSort)
{
	LRESULT	lResult;
	int	i;

	for (i = 0; SkillsSort[i] != 0; i++)
		{
		lResult = SendDlgItemMessage(hDlg,uCtlID,CB_ADDSTRING,0,(LPARAM)SkillsSort[i]);
		if (lResult == CB_ERR || lResult == CB_ERRSPACE) return(0);
		if (uSort == SkillsSort[i]) SendDlgItemMessage(hDlg,uCtlID,CB_SETCURSEL,(WPARAM)lResult,0);
		}

	return(1);
}


// «»»» Change l'affichage de la liste ««««««««««««««««««««««««««««««««««»

int Game_SkillsSortComboChanged(HWND hDlg, UINT uCtlID, UINT uListID, UINT *uValue)
{
	UINT	uSort;
	LRESULT	lResult;

	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETCURSEL,0,0);
	if (lResult == CB_ERR) return(0);
	lResult = SendDlgItemMessage(hDlg,uCtlID,CB_GETITEMDATA,(WPARAM)lResult,0);
	if (lResult == CB_ERR) return(0);
	uSort = (UINT)lResult;
	if (uListID)
		{
		LVITEM	lvItem;

		lvItem.iItem = -1;
		while ((lvItem.iItem = SendDlgItemMessage(hDlg,uListID,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
			{
			lvItem.mask = LVIF_PARAM;
			lvItem.iSubItem = 0;
			if (SendDlgItemMessage(hDlg,uListID,LVM_GETITEM,0,(LPARAM)&lvItem))
				{
				lvItem.iGroupId = Game_SkillsSetItemGroup(uSort,(GAMEDATASKILL *)lvItem.lParam);
				lvItem.mask = LVIF_GROUPID;
				SendDlgItemMessage(hDlg,uListID,LVM_SETITEM,0,(LPARAM)&lvItem);
				}
			}
		SendDlgItemMessage(hDlg,uListID,LVM_SORTITEMS,(WPARAM)uSort,(LPARAM)Game_SkillsSort);
		Game_SkillsCollapseSelectedGroup(hDlg,uListID,Game_SkillsGetSelected(hDlg,uListID));
		}
	*uValue = uSort;
	return(1);
}


// «»»» Dessine un type d'affichage «««««««««««««««««««««««««««««««««««««»

void Game_SkillsSortComboDrawItem(DRAWITEMSTRUCT *pDraw)
{
	UINT		uSortMode;
	UINT		uSortText;

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));

	uSortText = 0;
	uSortMode = pDraw->itemData;
	switch(uSortMode)
		{
		case DATA_SORT_SKILL_NAME:
			uSortText = TEXT_BONUS_SORT_NAMES;
			break;
		case DATA_SORT_SKILL_SCHOOL:
			uSortText = TEXT_BONUS_SORT_SCHOOLS;
			break;
		case DATA_SORT_SKILL_POINTS:
			uSortText = TEXT_BONUS_SORT_POINTS;
			break;
		case DATA_SORT_SKILL_SOURCE:
			uSortText = TEXT_BONUS_SORT_SOURCE;
			break;
		case DATA_SORT_SKILL_MEMORY:
			uSortText = TEXT_BONUS_SORT_MEMORY;
			break;
		}

	if (uSortText)
		{
		RECT		rcText;
		HFONT		hFont;
		COLORREF	crText;
		int		iBkMode;

		hFont = SelectObject(pDraw->hDC,App.Font.hFont);
		crText = SetTextColor(pDraw->hDC,GetSysColor(pDraw->itemState&ODS_SELECTED?COLOR_HIGHLIGHTTEXT:(pDraw->itemState&ODS_DISABLED?COLOR_GRAYTEXT:COLOR_WINDOWTEXT)));
		iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
		CopyRect(&rcText,&pDraw->rcItem);
		rcText.left += 2;
		rcText.right -= 2;
		DrawText(pDraw->hDC,Locale_GetText(uSortText),-1,&rcText,DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		SetBkMode(pDraw->hDC,iBkMode);
		SetTextColor(pDraw->hDC,crText);
		SelectObject(pDraw->hDC,hFont);
		}

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
	return;
}


// «»»» Tri des compétences «««««««««««««««««««««««««««««««««««««««««««««»

int Game_SkillsSort(LPARAM lFirstSkill, LPARAM lSecondSkill, LPARAM lSort)
{
	int	iResult = 0;

	switch(lSort)
		{
		// Name OR ID
		case DATA_SORT_ID:
		case DATA_SORT_TEXT:
		case DATA_SORT_SKILL_NAME:
			iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
			break;
		// School Name >> Name OR ID
		case DATA_SORT_SKILL_SCHOOL:
			iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)lSort);
			if (iResult == 0) iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
			break;
		// Points >> School Name >> Name OR ID
		case DATA_SORT_SKILL_POINTS:
		case DATA_SORT_SKILL_SOURCE:
		case DATA_SORT_SKILL_MEMORY:
			iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)lSort);
			if (iResult == 0)
				{
				if (iResult == 0) iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)DATA_SORT_SKILL_SCHOOL);
				if (iResult == 0) iResult = Game_SkillsSortCmp((GAMEDATASKILL *)lFirstSkill,(GAMEDATASKILL *)lSecondSkill,(UINT)DATA_SORT_TEXT);
				}
			break;
		}

	return(iResult);
}

//--- Routine de comparaison ---

int Game_SkillsSortCmp(GAMEDATASKILL *pFirstSkill, GAMEDATASKILL *pSecondSkill, UINT uSort)
{
	WCHAR*	pszText1;
	WCHAR*	pszText2;
	int	iResult = 0;

	switch(uSort)
		{
		case DATA_SORT_TEXT:
			pszText1 = pFirstSkill->pszName?pFirstSkill->pszName:pFirstSkill->pszId;
			pszText2 = pSecondSkill->pszName?pSecondSkill->pszName:pSecondSkill->pszId;
			iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
			if (iResult == CSTR_LESS_THAN) return(-1);
			if (iResult == CSTR_GREATER_THAN) return(1);
			break;
		case DATA_SORT_SKILL_SCHOOL:
			pszText1 = Locale_GetText(pFirstSkill->infos.uSchoolLocaleID);
			pszText2 = Locale_GetText(pSecondSkill->infos.uSchoolLocaleID);
			iResult = CompareStringEx(LOCALE_NAME_SYSTEM_DEFAULT,LINGUISTIC_IGNORECASE|SORT_DIGITSASNUMBERS,pszText1,-1,pszText2,-1,NULL,NULL,0);
			if (iResult == CSTR_LESS_THAN) return(-1);
			if (iResult == CSTR_GREATER_THAN) return(1);
			break;
		case DATA_SORT_SKILL_POINTS:
			if (pFirstSkill->infos.uActionPoints < pSecondSkill->infos.uActionPoints) return(-1);
			if (pFirstSkill->infos.uActionPoints > pSecondSkill->infos.uActionPoints) return(1);
			break;
		case DATA_SORT_SKILL_SOURCE:
			if (pFirstSkill->infos.uSourcePoints < pSecondSkill->infos.uSourcePoints) return(-1);
			if (pFirstSkill->infos.uSourcePoints > pSecondSkill->infos.uSourcePoints) return(1);
			break;
		case DATA_SORT_SKILL_MEMORY:
			if (pFirstSkill->infos.uMemorySlots < pSecondSkill->infos.uMemorySlots) return(-1);
			if (pFirstSkill->infos.uMemorySlots > pSecondSkill->infos.uMemorySlots) return(1);
			break;
		}

	return(0);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Copie une liste de compétences ««««««««««««««««««««««««««««««««««»

int Game_SkillsCopyList(GAMEEDITSKILLCONTEXT *pDest, GAMEEDITSKILLCONTEXT *pSrc, GAMEEDITSKILLCONTEXT *pExclude, WCHAR *pszBlackList[])
{
	GAMEDATASKILL*	pSkill;

	ZeroMemory(pDest,sizeof(GAMEEDITSKILLCONTEXT));

	pDest->hwndParent = pSrc->hwndParent;
	pDest->hImageList = pSrc->hImageList;
	pDest->uSort = pSrc->uSort;

	for (pSkill = (GAMEDATASKILL *)pSrc->skills.next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
		{
		if (pszBlackList)
			{
			int	i;
			BOOL	bBlackListed;

			for (bBlackListed = FALSE, i = 0; pszBlackList[i] != NULL; i++)
				{
				if (wcscmp(pSkill->pszId,pszBlackList[i])) continue;
				bBlackListed = TRUE;
				break;
				}
			if (bBlackListed) continue;
			}
		if (Game_SkillsGetById(pSkill->pszId,&pExclude->skills)) continue;
		if (!Game_SkillsCopyPaste(NULL,pSkill,&pDest->skills,TRUE)) goto Error;
		}

	return(1);

Error:	Game_UnloadDataFile(DATA_TYPE_SKILLS,&pDest->skills);
	SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	return(0);
}


// «»»» Copie les données d'une compétence ««««««««««««««««««««««««««««««»

// (in)  pDest - Destination skill (can be NULL)
// (in)  pSrc  - Source skill
// (in)  pRoot - Destination list (can be NULL)
// (out) pDest or NULL in case of error
//
// NOTE: if pDest->pszId or pDest->pszName are not NULL, they are considered allocated by the caller

GAMEDATASKILL* Game_SkillsCopyPaste(GAMEDATASKILL *pDest, GAMEDATASKILL *pSrc, NODE *pRoot, BOOL bCopyOptions)
{
	BOOL	bAllocated = FALSE;

	if (!pDest)
		{
		pDest = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(GAMEDATASKILL));
		if (!pDest) goto Error;
		bAllocated = TRUE;
		}

	if (!pDest->pszId) pDest->pszId = Misc_StrCpyAlloc(pSrc->pszId);
	if (!pDest->pszName) pDest->pszName = Misc_StrCpyAlloc(pSrc->pszName);

	pDest->iIconIndex = pSrc->iIconIndex;
	CopyMemory(&pDest->infos,&pSrc->infos,sizeof(GAMEDATASKILLINFOS));
	if (bCopyOptions) CopyMemory(&pDest->options,&pSrc->options,sizeof(GAMEDATASKILLOPTIONS));

	if (!pDest->pszId) goto Error;
	if (pSrc->pszName && !pDest->pszName) goto Error;

	if (pRoot) List_AddEntry((NODE *)pDest,pRoot);
	return(pDest);

Error:	if (bAllocated && pDest) HeapFree(App.hHeap,0,pDest);
	return(NULL);
}


// «»»» Retrouve la compétence sélectionnée «««««««««««««««««««««««««««««»

GAMEDATASKILL* Game_SkillsGetSelected(HWND hDlg, UINT uCtlID)
{
	LVITEM	lvItem;

	lvItem.iItem = SendDlgItemMessage(hDlg,uCtlID,LVM_GETNEXTITEM,(WPARAM)-1,(LPARAM)MAKELPARAM(LVNI_SELECTED,0));
	if (lvItem.iItem != -1)
		{
		lvItem.mask = LVIF_PARAM;
		lvItem.iSubItem = 0;
		lvItem.lParam = 0;
		SendDlgItemMessage(hDlg,uCtlID,LVM_GETITEM,0,(LPARAM)&lvItem);
		return((GAMEDATASKILL *)lvItem.lParam);
		}

	return(NULL);
}


// «»»» Bascule tous les groupes ««««««««««««««««««««««««««««««««««««««««»

void Game_SkillsToggleGroups(HWND hDlg, UINT uCtlID, BOOL bExpand)
{
	LVITEM	lvItem;
	LVGROUP	lvGroup;

	lvItem.iItem = -1;
	while ((lvItem.iItem = SendDlgItemMessage(hDlg,uCtlID,LVM_GETNEXTITEM,(WPARAM)lvItem.iItem,(LPARAM)MAKELPARAM(LVNI_ALL,0))) != -1)
		{
		lvItem.mask = LVIF_GROUPID;
		lvItem.iSubItem = 0;
		if (SendDlgItemMessage(hDlg,uCtlID,LVM_GETITEM,0,(LPARAM)&lvItem))
			{
			lvGroup.cbSize = sizeof(LVGROUP);
			lvGroup.mask = LVGF_STATE;
			lvGroup.iGroupId = lvItem.iGroupId;
			lvGroup.stateMask = LVGS_COLLAPSED;
			lvGroup.state = 0;
			if (SendDlgItemMessage(hDlg,uCtlID,LVM_GETGROUPINFO,(WPARAM)lvItem.iGroupId,(LPARAM)&lvGroup) != -1)
				{
				if (lvGroup.state&LVGS_COLLAPSED && !bExpand) continue; // Group collapsed, want collapsed
				if (!(lvGroup.state&LVGS_COLLAPSED) && bExpand) continue; // Group expanded, want expanded
				if (bExpand) lvGroup.state = 0;
				else lvGroup.state = LVGS_COLLAPSED;
				SendDlgItemMessage(hDlg,uCtlID,LVM_SETGROUPINFO,(WPARAM)lvItem.iGroupId,(LPARAM)&lvGroup);
				}
			}
		}

	return;
}


// «»»» S'assure que la compétence sélectionnée est visible «««««««««««««»

void Game_SkillsEnsureVisible(HWND hDlg, UINT uCtlID, GAMEDATASKILL *pSelected)
{
	if (pSelected)
		{
		LVFINDINFO	lvFind;
		int		iItem;

		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pSelected;
		iItem = SendDlgItemMessage(hDlg,uCtlID,LVM_FINDITEM,(WPARAM)-1,(LPARAM)&lvFind);
		if (iItem != -1) SendDlgItemMessage(hDlg,uCtlID,LVM_ENSUREVISIBLE,(WPARAM)iItem,(LPARAM)FALSE);
		}
	return;
}


// «»»» Déplie le groupe de la compétence sélecionnée «««««««««««««««««««»

void Game_SkillsCollapseSelectedGroup(HWND hDlg, UINT uCtlID, GAMEDATASKILL *pSelected)
{
	if (pSelected)
		{
		LVFINDINFO	lvFind;
		LVITEM		lvItem;
		LVGROUP		lvGroup;

		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pSelected;
		lvItem.iItem = SendDlgItemMessage(hDlg,uCtlID,LVM_FINDITEM,(WPARAM)-1,(LPARAM)&lvFind);
		if (lvItem.iItem != -1)
			{
			lvItem.mask = LVIF_GROUPID;
			lvItem.iSubItem = 0;
			if (SendDlgItemMessage(hDlg,uCtlID,LVM_GETITEM,(WPARAM)0,(LPARAM)&lvItem))
				{
				lvGroup.cbSize = sizeof(LVGROUP);
				lvGroup.mask = LVGF_STATE;
				lvGroup.stateMask = LVGS_COLLAPSED;
				lvGroup.state = 0;
				SendDlgItemMessage(hDlg,uCtlID,LVM_SETGROUPINFO,(WPARAM)lvItem.iGroupId,(LPARAM)&lvGroup);
				}
			}
		}
	return;
}


// «»»» Définition du groupe en fonction du mode de tri «««««««««««««««««»

int Game_SkillsSetItemGroup(UINT uSort, GAMEDATASKILL *pSkill)
{
	int	iGroupId;

	switch(uSort)
		{
		case DATA_SORT_SKILL_NAME:
			iGroupId = TEXT_BONUS_GROUP_NAMES;
			break;
		case DATA_SORT_SKILL_POINTS:
			iGroupId = TEXT_BONUS_GROUP_POINTS_0+pSkill->infos.uActionPoints;
			if (iGroupId > TEXT_BONUS_GROUP_POINTS_6) iGroupId = TEXT_BONUS_GROUP_POINTS_6;
			break;
		case DATA_SORT_SKILL_SOURCE:
			iGroupId = TEXT_BONUS_GROUP_SOURCE_0+pSkill->infos.uSourcePoints;
			if (iGroupId > TEXT_BONUS_GROUP_SOURCE_3) iGroupId = TEXT_BONUS_GROUP_SOURCE_3;
			break;
		case DATA_SORT_SKILL_MEMORY:
			iGroupId = TEXT_BONUS_GROUP_MEMORY_0+pSkill->infos.uMemorySlots;
			if (iGroupId > TEXT_BONUS_GROUP_MEMORY_3) iGroupId = TEXT_BONUS_GROUP_MEMORY_3;
			break;
		case DATA_SORT_SKILL_SCHOOL:
		default:iGroupId = pSkill->infos.uSchoolLocaleID;
			if (!iGroupId) iGroupId = TEXT_ABILITIES_UNKNOWN;
			break;
		}

	return(iGroupId);
}


// «»»» Retrouve une compétence avec son identifiant ««««««««««««««««««««»

GAMEDATASKILL* Game_SkillsGetById(WCHAR *pszId, NODE *pRoot)
{
	GAMEDATASKILL*	pSkill;

	if (!pszId) return(NULL);
	if (!pRoot) return(NULL);

	for (pSkill = (GAMEDATASKILL *)pRoot->next; pSkill != NULL; pSkill = (GAMEDATASKILL *)pSkill->node.next)
		{
		if (wcscmp(pszId,pSkill->pszId)) continue;
		return(pSkill);
		}

	return(NULL);
}

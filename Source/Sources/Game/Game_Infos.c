
//<<>-<>>---------------------------------------------------------------------()
/*
	Informations sur une sauvegarde
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Divine.h"
#include "Files.h"
#include "Infos.h"
#include "Locale.h"
#include "Texts.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Requests.h"

extern APPLICATION	App;
extern WCHAR*		ModsIgnore[];
extern WCHAR*		ModsLarian[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Infos_Show()
{
	if (Infos_LoadMetaDatas(App.hWnd))
		{
		int iResult = DialogBox(App.hInstance,MAKEINTRESOURCE(1010),App.hWnd,Infos_Proc);
		if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		}

	return;
}


// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Infos_Proc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	if (uMsgId == WM_INITDIALOG)
		{
		if (!Infos_Initialise(hDlg))
			{
			EndDialog(hDlg,-1);
			return(FALSE);
			}
		Infos_PrepareAndUpdate(hDlg,App.Game.Save.pszSaveName,&App.Game.Save.nodeFiles);
		SetWindowText(hDlg,Locale_GetText(IDM_INFOS));
		Dialog_CenterWindow(hDlg,App.hWnd);
		return(FALSE);
		}

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			return(Infos_Draw(hDlg,wParam,(DRAWITEMSTRUCT *)lParam,&App.Game.Save.nodeFiles));

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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Arborescence des métadonnées ««««««««««««««««««««««««««««««««««««»

void Infos_Tree()
{
	if (Infos_LoadMetaDatas(App.hWnd))
		{
		NODE* node = lsv_GetMetaXML(&App.Game.Save.nodeFiles);
		if (node) Tree_Open((XML_NODE *)node->next);
		}
	return;
}


// «»»» Chargement des métadonnées ««««««««««««««««««««««««««««««««««««««»

int Infos_LoadMetaDatas(HWND hWnd)
{
	WCHAR*		pszPath;
	LSFILE*		pFile;

	if (!App.Game.Save.pszSaveName) return(0);
	if (App.Game.Save.nodeFiles.next) return(1);

	//--- Vérifie si la sauvegarde a été modifiée ---

	if (Divine_IsSaveGameChanged(hWnd,Locale_GetText(TEXT_MODIFIED_SGMETA),App.Config.uGame,App.Config.pszProfile,App.Game.Save.pszSaveName,App.Game.Save.pszCustomSavePath,&App.Game.Save.ftLastWrite))
		return(0);

	//--- Unpack LSV ---

	pszPath = Divine_GetSaveGamePath(App.Config.uGame,App.Config.pszProfile,App.Game.Save.pszSaveName,App.Game.Save.pszCustomSavePath);
	if (!pszPath)
		{
		Request_MessageBoxEx(hWnd,Locale_GetText(TEXT_ERR_METADATAS),NULL,MB_ICONERROR,Locale_GetText(TEXT_ERR_NOMEMORY));
		return(0);
		}

	if (!lsv_Load(hWnd,pszPath,&App.Game.Save.nodeFiles,LS_LOAD_META|LS_LOAD_PNG))
		{
		HeapFree(App.hHeap,0,pszPath);
		return(0);
		}

	HeapFree(App.hHeap,0,pszPath);

	//--- Unpack metadatas ---

	pFile = lsv_FindFile(&App.Game.Save.nodeFiles,szMetaLSF,NULL);
	if (!pFile)
		{
		Request_MessageBoxEx(hWnd,Locale_GetText(TEXT_ERR_METADATAS),NULL,MB_ICONERROR,Locale_GetText(TEXT_ERR_NOMETAFILE));
		lsv_Release(&App.Game.Save.nodeFiles);
		return(0);
		}

	if (!lsf_Unpack(hWnd,pFile,0))
		{
		lsv_Release(&App.Game.Save.nodeFiles);
		return(0);
		}

	return(1);
}


// «»»» Initialisations «««««««««««««««««««««««««««««««««««««««««««««««««»

int Infos_Initialise(HWND hDlg)
{
	HIMAGELIST	hImageList;
	RECT		rcItem;
	LVCOLUMN	lvColumn;
	LVGROUP		lvGroup;
	LVTILEVIEWINFO	lvTileView;
	static UINT	uGroupsTitles[] = { TEXT_INFOS_MISC, TEXT_INFOS_TEAM, TEXT_INFOS_MODS, 0 };
	static UINT	uInfosIconsAPP[] = { APP_ICON_INFO_EMPTY, APP_ICON_INFO_SAVEGAME, APP_ICON_INFO_VERSION, APP_ICON_INFO_DIFFICULTY, APP_ICON_INFO_GAMETIME, APP_ICON_INFO_DATE, APP_ICON_INFO_CHARACTER, APP_ICON_INFO_NOTCHARACTER,  APP_ICON_INFO_LARIAN, APP_ICON_INFO_MOD};
	static UINT	uInfosIconsIDS[] = { INFOS_ICON_EMPTY,    INFOS_ICON_SAVEGAME,    INFOS_ICON_VERSION,    INFOS_ICON_DIFFICULTY,    INFOS_ICON_GAMETIME,    INFOS_ICON_DATE,    INFOS_ICON_CHARACTER,    INFOS_ICON_NOTCHARACTER,     INFOS_ICON_LARIANMOD, INFOS_ICON_PLAYERMOD };
	HICON		hInfosIcons[INFOS_ICON_LAST];
	int		i;

	SendDlgItemMessage(hDlg,801,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_DOUBLEBUFFER,LVS_EX_DOUBLEBUFFER);
	hImageList = ImageList_Create(48,48,ILC_COLOR32|ILC_MASK,INFOS_ICON_LAST,0);
	if (!hImageList) goto Error;

	//--- Chargement des icônes
	for (i = 0; i != INFOS_ICON_LAST; i++) hInfosIcons[uInfosIconsIDS[i]] = App.hIcons[uInfosIconsAPP[i]];
	for (i = 0; i != INFOS_ICON_LAST; i++) if (ImageList_ReplaceIcon(hImageList,-1,hInfosIcons[i]) == -1) goto Error;

	//--- Colonnes
	lvColumn.mask = LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 0;
	lvColumn.pszText = NULL;
	lvColumn.cchTextMax = 0;
	lvColumn.iOrder = 0;
	if (SendDlgItemMessage(hDlg,801,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn) == -1) goto Error;
	lvColumn.iOrder = 1;
	if (SendDlgItemMessage(hDlg,801,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn) == -1) goto Error;

	//--- Groupes
	for (i = 0; uGroupsTitles[i] != 0; i++)
		{
		lvGroup.cbSize = sizeof(LVGROUP);
		lvGroup.mask = LVGF_HEADER|LVGF_GROUPID|LVGF_STATE;
		lvGroup.pszHeader = Locale_GetText(uGroupsTitles[i]);
		lvGroup.iGroupId = i;
		lvGroup.stateMask = lvGroup.state = 0;
		if (SendDlgItemMessage(hDlg,801,LVM_INSERTGROUP,(WPARAM)-1,(LPARAM)&lvGroup) == -1) goto Error;
		}

	//--- Paramètres
	GetClientRect(GetDlgItem(hDlg,801),&rcItem);
	lvTileView.cbSize = sizeof(LVTILEVIEWINFO);
	lvTileView.dwMask = LVTVIM_TILESIZE|LVTVIM_COLUMNS;
	lvTileView.dwFlags = LVTVIF_FIXEDSIZE;
	lvTileView.sizeTile.cx = rcItem.right-rcItem.left-GetSystemMetrics(SM_CXVSCROLL)-10;
	lvTileView.sizeTile.cy = (App.Font.uFontHeight*2+8) < (48+8)?(48+8):(App.Font.uFontHeight*2+8);
	lvTileView.cLines = 2;
	SendDlgItemMessage(hDlg,801,LVM_SETTILEVIEWINFO,0,(LPARAM)&lvTileView);

	//--- Affichage
	SendDlgItemMessage(hDlg,801,LVM_SETIMAGELIST,(WPARAM)LVSIL_NORMAL,(LPARAM)hImageList);
	SendDlgItemMessage(hDlg,801,LVM_SETVIEW,(WPARAM)LV_VIEW_TILE,(LPARAM)0);
	SendDlgItemMessage(hDlg,801,LVM_ENABLEGROUPVIEW,(WPARAM)TRUE,0);
	return(1);

Error:	if (hImageList) ImageList_Destroy(hImageList);
	SendDlgItemMessage(hDlg,801,LVM_DELETEALLITEMS,0,0);
	return(0);
}


// «»»» Convertis les fichiers nécessaires et met à jour l'affichage ««««»

void Infos_PrepareAndUpdate(HWND hDlg, WCHAR *pszSaveName, NODE *pRoot)
{
	XML_NODE*	pxnMeta;
	LSFILE*		pMetaFile;
	LSFILE*		pPNGFile;
	LVITEM		lvItem;
	LVTILEINFO	lvTileInfo;
	UINT		uColumns[1] = { 1 };
	WCHAR*		pszTemp = NULL;
	static UINT	uItems[] = { TEXT_INFOS_SAVEGAME, TEXT_INFOS_VERSION, TEXT_INFOS_DIFFICULTY, TEXT_INFOS_GAMETIME, TEXT_INFOS_DATE, 0 };
	int		i,j = 0;

	//--- Chargement des fichiers ---

	pMetaFile = lsv_FindFile(pRoot,szMetaLSF,NULL);
	if (pMetaFile && !pMetaFile->nodeXMLRoot.next) lsf_Unpack(hDlg,pMetaFile,LS_LOAD_QUIET);

	pPNGFile = lsv_FindFile(pRoot,NULL,szPNGext);
	if (pPNGFile && !pPNGFile->hBitmap) pPNGFile->hBitmap = png_Load(pPNGFile->pData);

	if (!pMetaFile) goto Error;
	if (!pMetaFile->nodeXMLRoot.next) goto Error;

	//--- Réinitialisation ---

	SendDlgItemMessage(hDlg,801,LVM_DELETEALLITEMS,0,0);

	pxnMeta = xml_GetNodeFromPathFirstChild((XML_NODE *)pMetaFile->nodeXMLRoot.next,szInfoMetaDataPath);
	if (!pxnMeta) goto Error;

	//--- Informations générales ---

	for (pszTemp = NULL, i = 0; uItems[i] != 0; i++, j++)
		{
		lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_GROUPID;
		lvItem.iItem = j;
		lvItem.iSubItem = 0;
		lvItem.iImage = 0;
		lvItem.pszText = Locale_GetText(uItems[i]);
		lvItem.iGroupId = 0;
		switch(uItems[i])
			{
			case TEXT_INFOS_SAVEGAME:
				lvItem.iImage = INFOS_ICON_SAVEGAME;
				break;
			case TEXT_INFOS_VERSION:
				lvItem.iImage = INFOS_ICON_VERSION;
				break;
			case TEXT_INFOS_DIFFICULTY:
				lvItem.iImage = INFOS_ICON_DIFFICULTY;
				break;
			case TEXT_INFOS_GAMETIME:
				lvItem.iImage = INFOS_ICON_GAMETIME;
				break;
			case TEXT_INFOS_DATE:
				lvItem.iImage = INFOS_ICON_DATE;
				break;
			}
		if (SendDlgItemMessage(hDlg,801,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Error;
		lvItem.iSubItem = 1;
		lvItem.pszText = Infos_Get(INFOS_GROUP_MISC,uItems[i],&pszTemp,pxnMeta);
		if (lvItem.pszText == (WCHAR *)-1) lvItem.pszText = pszSaveName;
		if (lvItem.pszText == NULL) goto Error;
		if (SendDlgItemMessage(hDlg,801,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == -1) goto Error;
		lvTileInfo.cbSize = sizeof(LVTILEINFO);
		lvTileInfo.iItem = lvItem.iItem;
		lvTileInfo.cColumns = 1;
		lvTileInfo.puColumns = uColumns;
		lvTileInfo.piColFmt = NULL;
		if (SendDlgItemMessage(hDlg,801,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == -1) goto Error;
		}

	//--- Équipe ---

	for (i = 0; i != 4; i++, j++)
		{
		lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_GROUPID;
		lvItem.iItem = j;
		lvItem.iSubItem = 0;
		lvItem.iGroupId = 1;
		lvItem.pszText = Infos_Get(INFOS_GROUP_TEAM,i,&pszTemp,pxnMeta);
		lvItem.iImage = lvItem.pszText?INFOS_ICON_CHARACTER:INFOS_ICON_NOTCHARACTER;
		if (lvItem.pszText == NULL) lvItem.pszText = Locale_GetText(TEXT_UNKNOWN);
		if (SendDlgItemMessage(hDlg,801,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Error;
		lvTileInfo.cbSize = sizeof(LVTILEINFO);
		lvTileInfo.iItem = lvItem.iItem;
		lvTileInfo.cColumns = 1;
		lvTileInfo.puColumns = uColumns;
		lvTileInfo.piColFmt = NULL;
		if (SendDlgItemMessage(hDlg,801,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == -1) goto Error;
		}

	//--- Mods ---

	pxnMeta = xml_GetNodeFromPath(pxnMeta,szInfoGameModsPath);
	while (pxnMeta)
		{
		lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_GROUPID;
		lvItem.iItem = j;
		lvItem.iSubItem = 0;
		lvItem.iGroupId = 2;
		lvItem.pszText = Infos_Get(INFOS_GROUP_MODS,INFOS_MODS_NAME,&pszTemp,pxnMeta);
		lvItem.iImage = pszTemp?INFOS_ICON_LARIANMOD:INFOS_ICON_PLAYERMOD;
		if (lvItem.pszText != NULL)
			{
			if (SendDlgItemMessage(hDlg,801,LVM_INSERTITEM,0,(LPARAM)&lvItem) == -1) goto Error;
			lvItem.pszText = Infos_Get(INFOS_GROUP_MODS,INFOS_MODS_VERSION,&pszTemp,pxnMeta);
			lvItem.iSubItem = 1;
			if (SendDlgItemMessage(hDlg,801,LVM_SETITEMTEXT,(WPARAM)lvItem.iItem,(LPARAM)&lvItem) == -1) goto Error;
			lvTileInfo.cbSize = sizeof(LVTILEINFO);
			lvTileInfo.iItem = lvItem.iItem;
			lvTileInfo.cColumns = 1;
			lvTileInfo.puColumns = uColumns;
			lvTileInfo.piColFmt = NULL;
			if (SendDlgItemMessage(hDlg,801,LVM_SETTILEINFO,0,(LPARAM)&lvTileInfo) == -1) goto Error;
			j++;
			}
		pxnMeta = xml_GetNextNode(pxnMeta);
		}

	if (pszTemp) HeapFree(App.hHeap,0,pszTemp);
	InvalidateRect(GetDlgItem(hDlg,800),NULL,FALSE);
	return;

Error:	if (pszTemp) HeapFree(App.hHeap,0,pszTemp);
	SendDlgItemMessage(hDlg,801,LVM_DELETEALLITEMS,0,0);
	InvalidateRect(GetDlgItem(hDlg,800),NULL,FALSE);
	return;
}

//--- Récupère les informations ---

WCHAR* Infos_Get(UINT uGroup, UINT uID, WCHAR **pszTemp, XML_NODE *pRoot)
{
	XML_NODE*	pxnInfo;

	if (!pRoot) return(NULL);

	switch(uGroup)
		{
		//--- Informations générales
		case INFOS_GROUP_MISC:
			switch(uID)
				{
				case TEXT_INFOS_SAVEGAME:
					return((WCHAR *)-1);

				case TEXT_INFOS_VERSION:
					pxnInfo = xml_GetNodeFromPathFirstChild(pRoot,szInfoGameVersionsPath);
					if (pxnInfo)
						{
						XML_NODE*	pxn;

						// Displays only the latest game version
						for (pxn = pxnInfo; pxn->node.next != NULL; pxn = (XML_NODE *)pxn->node.next);
						return(xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxn->children.next,szXMLattribute,szXMLid,szXMLObject)));
						}
					break;

				case TEXT_INFOS_DIFFICULTY:
					pxnInfo = xml_GetNodeFromPath(pRoot,szInfoGameDifficultyPath);
					if (!pxnInfo) break;
					return(xml_GetAttrValue(pxnInfo,szXMLvalue));

				case TEXT_INFOS_GAMETIME:
					pxnInfo = xml_GetNodeFromPath(pRoot,szInfoGameTimePath);
					if (pxnInfo)
						{
						WCHAR*		pszTime;
						UINT		uTime;
						UINT		uTotalLen;
						UINT		uHMS[4];
						WCHAR		szBuffer[6];
						static UINT	uHMSLocales[] = { TEXT_INFOS_HOURS, TEXT_INFOS_MINUTES, TEXT_INFOS_SECONDS };
						int		i;

						pszTime = xml_GetAttrValue(pxnInfo,szXMLvalue);
						if (pszTime)
							{
							uTotalLen = 0;
							uTime = wcstoul(pszTime,NULL,10);
							if (uTime < 5770) uTime = 5770;
							uTime -= 5770;
							uHMS[0] = uTime/3600;
							uHMS[1] = (uTime%3600)/60;
							uHMS[2] = (uTime%60);
							for (i = 0; i != 3; i++)
								{
								if (!uHMS[i]) continue;
								if (uTotalLen) uTotalLen++;
								swprintf(szBuffer,5,L"%u",uHMS[i]);
								uTotalLen += wcslen(Locale_GetText(uHMSLocales[i]));
								uTotalLen += wcslen(szSpace);
								uTotalLen += wcslen(szBuffer);
								}
							if (*pszTemp) HeapFree(App.hHeap,0,*pszTemp);
							*pszTemp = HeapAlloc(App.hHeap,0,uTotalLen*sizeof(WCHAR)+sizeof(WCHAR));
							if (*pszTemp)
								{
								*pszTemp[0] = 0;
								for (i = 0, uTotalLen = 0; i != 3; i++)
									{
									if (!uHMS[i]) continue;
									if (uTotalLen) wcscat(*pszTemp,szSpace);
									swprintf(szBuffer,5,L"%u",uHMS[i]);
									wcscat(*pszTemp,szBuffer);
									wcscat(*pszTemp,szSpace);
									wcscat(*pszTemp,Locale_GetText(uHMSLocales[i]));
									uTotalLen++;
									}
								return(*pszTemp);
								}
							}
						}
					break;

				case TEXT_INFOS_DATE: {
					static WCHAR*	pszPattern[6] = { L"YYYY", L"MM", L"DD", L"hh", L"mm", L"ss" };
					static WCHAR*	pszValue[6] = { L"Year", L"Month", L"Day", L"Hours", L"Minutes", L"Seconds" };
					UINT		pszNumber[6];
					WCHAR		szBuffer[6];
					WCHAR*		pszDate;
					int		i,j;

					pxnInfo = xml_GetNodeFromPathFirstChild(pRoot,szInfoGameDatePath);
					if (!pxnInfo) break;
					for (i = 0; i != 6; i++)
						{
						pszDate = xml_GetThisAttrValue(xml_GetXMLValueAttr(pxnInfo,szXMLattribute,szXMLid,pszValue[i]));
						if (!pszDate) return(NULL);
						pszNumber[i] = wcstol(pszDate,NULL,10);
						if (i == 0) pszNumber[i] += 1900;
						}
					pszDate = Locale_GetText(TEXT_INFOS_DATEFMT);
					if (*pszTemp) HeapFree(App.hHeap,0,*pszTemp);
					*pszTemp = HeapAlloc(App.hHeap,0,wcslen(pszDate)*sizeof(WCHAR)+sizeof(WCHAR));
					if (!*pszTemp) break;

					wcscpy(*pszTemp,pszDate);
					for (i = 0; i != 6; i++)
						{
						swprintf(szBuffer,4,i == 0?L"%04u":L"%02u",pszNumber[i]);
						pszDate = wcsstr(*pszTemp,pszPattern[i]);
						if (!pszDate)
							{
							HeapFree(App.hHeap,0,*pszTemp);
							*pszTemp = NULL;
							return(NULL);
							}
						j = wcslen(pszPattern[i]);
						while (j--) pszDate[j] = szBuffer[j];
						}

					} return(*pszTemp);
				}
			break;

		//--- Composition de l'équipe
		case INFOS_GROUP_TEAM: {
			int	i;

			pxnInfo = xml_GetNodeFromPathFirstChild(pRoot,szInfoGamePartyPath);
			if (!pxnInfo) break;

			for (i = 0; i != uID; i++)
				{
				pxnInfo = xml_GetNextNode(pxnInfo);
				if (!pxnInfo) return(NULL);
				}

			} return(xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pxnInfo->children.next,szXMLattribute,szXMLid,L"CharacterName")));

		//--- Liste des mods
		case INFOS_GROUP_MODS:
			switch(uID)
				{
				case INFOS_MODS_NAME: {
					WCHAR*		UUID;

					if (*pszTemp)
						{
						HeapFree(App.hHeap,0,*pszTemp);
						*pszTemp = NULL;
						}

					UUID = xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pRoot->children.next,szXMLattribute,szXMLid,L"UUID"));
					if (UUID)
						{
						int	i;

						for (i = 0; ModsIgnore[i] != NULL; i++)
							if (!wcscmp(ModsIgnore[i],UUID)) return(NULL);

						for (i = 0; ModsLarian[i] != NULL; i++)
							if (!wcscmp(ModsLarian[i],UUID))
								{
								*pszTemp = Misc_StrCpyAlloc(szLarianStudios);
								break;
								}
						}

					} return(xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pRoot->children.next,szXMLattribute,szXMLid,L"Name")));

				case INFOS_MODS_VERSION: {
					WCHAR*		pszVersion;

					if ((pszVersion = Infos_GetVersionString(xml_GetThisAttrValue(xml_GetXMLValueAttr((XML_NODE *)pRoot->children.next,szXMLattribute,szXMLid,L"Version")),*pszTemp)) != NULL)
						{
						if (*pszTemp) HeapFree(App.hHeap,0,*pszTemp);
						*pszTemp = Misc_StrCpyAlloc(pszVersion);
						LocalFree(pszVersion);
						return(*pszTemp);
						}

					} break;
				}
			break;
		}

	return(NULL);
}

//--- Conversion de la version ---

WCHAR* Infos_GetVersionString(WCHAR *pszVersion, WCHAR *pszExtra)
{
	if (pszVersion)
		{
		DWORD_PTR	vl[5];
		WCHAR*		pszFmt;
		WCHAR*		pszString;
		LONG		lVersion;

		pszString = NULL;
		lVersion = wcstol(pszVersion,NULL,10);
		vl[0] = (DWORD_PTR)((lVersion&0xF0000000)>>28);
		vl[1] = (DWORD_PTR)((lVersion&0x0F000000)>>24);
		vl[2] = (DWORD_PTR)((lVersion&0x00FF0000)>>16);
		vl[3] = (DWORD_PTR)((lVersion&0x0000FFFF));
		vl[4] = (DWORD_PTR)pszExtra;
		if (pszExtra) pszFmt = L"%1!i!.%2!i!.%3!i!.%4!i! [%5]";
		else pszFmt = L"%1!i!.%2!i!.%3!i!.%4!i!";
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszFmt,0,0,(WCHAR *)&pszString,1,(va_list *)&vl)) return(pszString);
		}

	return(NULL);
}


// «»»» Affichage ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL Infos_Draw(HWND hDlg, UINT uCtlId, DRAWITEMSTRUCT *pDraw, NODE *pRoot)
{
	switch(uCtlId)
		{
		case 800:
			Infos_DrawImage(pDraw,pRoot);
			return(TRUE);
		}

	return(FALSE);
}

//--- Affichage de l'image ---

void Infos_DrawImage(DRAWITEMSTRUCT *pDraw, NODE *pRoot)
{
	LSFILE*		pFile;

	pFile = lsv_FindFile(pRoot,NULL,szPNGext);
	if (pFile)
		{
		if (pFile->hBitmap)
			{
			HDC		hDC;
			HBITMAP		hDefBitmap;
			BITMAP		bm;
			int		iBltMode;

			hDC = CreateCompatibleDC(pDraw->hDC);
			if (hDC)
				{
				hDefBitmap = SelectObject(hDC,pFile->hBitmap);
				iBltMode = SetStretchBltMode(pDraw->hDC,COLORONCOLOR);
				GetObject(pFile->hBitmap,sizeof(BITMAP),&bm);
				StretchBlt(pDraw->hDC,pDraw->rcItem.left,pDraw->rcItem.top,pDraw->rcItem.right-pDraw->rcItem.left,pDraw->rcItem.bottom-pDraw->rcItem.top,hDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
				SetStretchBltMode(pDraw->hDC,iBltMode);
				SelectObject(hDC,hDefBitmap);
				DeleteDC(hDC);
				DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKENOUTER,BF_RECT);
				return;
				}
			}
		}

	FillRect(pDraw->hDC,&pDraw->rcItem,GetStockObject(BLACK_BRUSH));
	DrawEdge(pDraw->hDC,&pDraw->rcItem,BDR_SUNKENOUTER,BF_RECT);
	return;
}

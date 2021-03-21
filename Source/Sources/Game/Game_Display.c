
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
#include "Utils.h"
#include "Dialogs.h"

extern APPLICATION	App;
extern GAMEQUALITY	Qualities[];
extern GAMEORIGIN	Origins[];
extern UINT		TextsAttr[];
extern UINT		TextsPts[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage de la liste des personnages «««««««««««««««««««««««««««»

//--- Mesure la hauteur des éléments ---

void Game_MeasureList(MEASUREITEMSTRUCT *pMeasure)
{
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = App.Font.uFontHeight+4;
	if (pMeasure->itemHeight < 24+4) pMeasure->itemHeight = 24+4;
	return;
}

//--- Affichage d'un élément ---

void Game_DrawList(DRAWITEMSTRUCT *pDraw)
{
	DOS2CHARACTER*	pdc;
	WCHAR*		pszText;
	DWORD_PTR	pszArgs[3];
	HFONT		hFont;
	RECT		rcText;
	COLORREF	crText;
	int		iBack;

	pszText = NULL;
	pdc = (DOS2CHARACTER *)pDraw->itemData;
	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush((pDraw->itemState&ODS_SELECTED)?COLOR_HIGHLIGHT:COLOR_WINDOW));
	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	CopyRect(&rcText,&pDraw->rcItem);
	rcText.left += 2;
	rcText.right -= 2;
	DrawIconEx(pDraw->hDC,rcText.left,rcText.top+(rcText.bottom-rcText.top-24)/2,App.hIcons[APP_ICON_PLAYER],24,24,0,NULL,DI_NORMAL);
	rcText.left += 26;

	if (pdc && pdc != (DOS2CHARACTER *)-1)
		{
		WCHAR*		pszRace;

		pszRace = NULL;
		pszArgs[0] = (DWORD_PTR)xml_GetThisAttrValue(pdc->pxaName);
		pszArgs[1] = (DWORD_PTR)xml_GetThisAttrValue(pdc->pxaOriginName);
		pszArgs[2] = (DWORD_PTR)xml_GetThisAttrValue(pdc->pxaRace);

		//--- Origines ---

		if (pszArgs[1])
			{
			if (!wcsncmp(szGeneric,(WCHAR *)pszArgs[1],7)) pszArgs[1] = (DWORD_PTR)Locale_GetText(TEXT_CHR_NOORIGIN);
			else
				{
				int	i;

				for (i = 0; Origins[i].pszName != NULL; i++)
					{
					if (wcscmp((WCHAR *)pszArgs[1],Origins[i].pszName)) continue;
					pszArgs[1] = (DWORD_PTR)Locale_GetText(Origins[i].uLocaleId);
					break;
					}
				}
			}

		//--- Race ---

		if (pszArgs[2])
			{
			DWORD_PTR	vl[1];

			vl[0] = pszArgs[2];
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szRaceFmt,0,0,(LPTSTR)&pszRace,1,(va_list *)vl);
			if (pszRace)
				{
				CharUpper(pszRace);
				pszArgs[2] = (DWORD_PTR)Locale_GetText(Locale_GetTextUID(App.pLocaleTexts,pszRace));
				}
			}

		if (!pszArgs[0]) pszArgs[0] = (DWORD_PTR)Locale_GetText(TEXT_UNKNOWN);
		if (!pszArgs[1]) pszArgs[1] = (DWORD_PTR)Locale_GetText(TEXT_CHR_NOORIGIN);
		if (!pszArgs[2]) pszArgs[2] = (DWORD_PTR)Locale_GetText(TEXT_UNKNOWN);
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szPlayerName,0,0,(LPTSTR)&pszText,1,(va_list *)pszArgs);
		if (pszRace) LocalFree(pszRace);
		}

	if (pszText)
		{
		DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		LocalFree(pszText);
		}

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Affichage de la liste des objets ««««««««««««««««««««««««««««««««»

//--- Mesure la hauteur des éléments ---

void Game_MeasureInventory(MEASUREITEMSTRUCT *pMeasure)
{
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = (App.Font.uFontHeight+8)*2+4;
	if (pMeasure->itemHeight < GAME_ICON_SIZE+8) pMeasure->itemHeight = GAME_ICON_SIZE+8;
	return;
}

//--- Affichage d'un élément ---

void Game_DrawInventory(DRAWITEMSTRUCT *pDraw)
{
	DOS2ITEM*	pItem;
	WCHAR*		pszText;
	WCHAR*		pszDisplayName;
	HFONT		hFont;
	RECT		rcDraw;
	RECT		rcClient;
	COLORREF	crText;
	int		iBack;

	pszText = NULL;
	pszDisplayName = NULL;
	pItem = (DOS2ITEM *)pDraw->itemData;
	hFont = SelectObject(pDraw->hDC,App.Font.hFont);
	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
	iBack = SetBkMode(pDraw->hDC,TRANSPARENT);

	CopyRect(&rcClient,&pDraw->rcItem);
	rcClient.left += 2;
	rcClient.top += 2;
	rcClient.right -= 2;
	rcClient.bottom -= 2;

	//--- Affichage du fond ---

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_WINDOW));

	//--- Affichage des entrées de navigation ---

	if (pItem && pItem != (DOS2ITEM *)-1)
		{
		//--- Affichage des sacs ---

		if (pItem->node.type != ITEM_TYPE_REAL)
			{
			switch(pItem->node.type)
				{
				case ITEM_TYPE_ROOT:
					pszText = Locale_GetText(IDM_INVBACKPACKCLOSEALL);
					break;
				case ITEM_TYPE_PARENT:
					pszText = Locale_GetText(IDM_INVBACKPACKCLOSE);
					break;
				}
			CopyRect(&rcDraw,&rcClient);
			DrawIconEx(pDraw->hDC,rcDraw.left+2,rcDraw.top+(rcDraw.bottom-rcDraw.top-48)/2,App.hIcons[APP_ICON_BACK],48,48,0,NULL,DI_NORMAL);
			rcDraw.left += GAME_ICON_SIZE+8;
			rcDraw.bottom = rcDraw.top+App.Font.uFontHeight+8;
			DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
			}

		//--- Affichage de l'objet ---

		else
			{
			WCHAR*		pszLevel = NULL;
			DWORD_PTR	dptrLevel = 0;

			//--- Icon, Name and Level ---

			//--- Get custom name or stats name or unknown
			pszText = pszDisplayName = Game_CopyDisplayName(xml_GetThisAttrValue(pItem->pxaDisplayName));
			if (!pszText)
				{
				pszText = xml_GetThisAttrValue(pItem->pxaName);
				if (!pszText) pszText = Locale_GetText(TEXT_UNKNOWN);
				}

			//--- Draw icon
			CopyRect(&rcDraw,&rcClient);
			Game_PaintIcon(pDraw->hDC,xml_GetThisAttrValue(pItem->pxaName),pItem->bIsBackPack?APP_ICON_BACKPACK:APP_ICON_EMPTY,&rcDraw,GAME_ICON_SIZE,TRUE,TRUE);

			//--- Draw name
			CopyRect(&rcDraw,&rcClient);
			if (pItem->pxaLevel)
				{
				if (pItem->pxaLevel->value)
					{
					dptrLevel = wcstol(pItem->pxaLevel->value,NULL,10);
					if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,Locale_GetText(TEXT_OBJ_LEVEL),0,0,(WCHAR *)&pszLevel,1,(va_list *)&dptrLevel))
						{
						SIZE	Size;

						GetTextExtentPoint32(pDraw->hDC,pszLevel,wcslen(pszLevel),&Size);
						rcDraw.right -= Size.cx+8;
						}
					}
				}
			rcDraw.left += GAME_ICON_SIZE+8;
			rcDraw.bottom = rcDraw.top+App.Font.uFontHeight+8;
			DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);

			//--- Display level & free level text buffer
			if (pszLevel)
				{
				rcDraw.right = rcClient.right-8;
				DrawText(pDraw->hDC,pszLevel,-1,&rcDraw,DT_END_ELLIPSIS|DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
				LocalFree(pszLevel);
				}

			//--- Free display name buffer
			if (pszDisplayName)
				HeapFree(App.hHeap,0,pszDisplayName);

			//--- Runes ---

			CopyRect(&rcDraw,&rcClient);
			rcDraw.left += GAME_ICON_SIZE+8;
			rcDraw.top += App.Font.uFontHeight+8;
			rcDraw.right -= 8;

			if (pItem->pxaRunes[0] && pItem->pxaRunes[1] && pItem->pxaRunes[2])
				{
				DrawIconEx(pDraw->hDC,rcDraw.left,rcDraw.top,App.hIcons[(pItem->pxaRunes[0]->value)?APP_ICON_CHECKED:APP_ICON_UNCHECKED],16,16,0,NULL,DI_NORMAL);
				rcDraw.left += 16;
				DrawIconEx(pDraw->hDC,rcDraw.left,rcDraw.top,App.hIcons[(pItem->pxaRunes[1]->value)?APP_ICON_CHECKED:APP_ICON_UNCHECKED],16,16,0,NULL,DI_NORMAL);
				rcDraw.left += 16;
				DrawIconEx(pDraw->hDC,rcDraw.left,rcDraw.top,App.hIcons[(pItem->pxaRunes[2]->value)?APP_ICON_CHECKED:APP_ICON_UNCHECKED],16,16,0,NULL,DI_NORMAL);
				rcDraw.left += 20;
				}

			//--- Equipped ? ---

			SetTextColor(pDraw->hDC,GetSysColor(COLOR_GRAYTEXT));

			if (Game_IsItemEquipped(pItem))
				{
				WCHAR*	Text;
				SIZE	Size;

				Text = Locale_GetText(TEXT_OBJ_EQUIPPED);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Generated ? ---

			if (xml_IsTrue(pItem->pxaIsGenerated))
				{
				WCHAR*	Text;
				SIZE	Size;

				Text = Locale_GetText(TEXT_OBJ_GENERATED);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Custom Base ? ---

			if (xml_IsTrue(pItem->pxaHasCustomBase))
				{
				WCHAR*		Text;
				SIZE		Size;

				Text = Locale_GetText(TEXT_OBJ_CUSTOM);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Amount ---

			if (pItem->pxaAmount)
				{
				if (pItem->pxaAmount->value)
					{
					WCHAR*		Text;
					SIZE		Size;

					Text = Locale_GetText(TEXT_OBJ_AMOUNT);
					DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
					GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
					rcDraw.left += Size.cx+2;
					DrawText(pDraw->hDC,pItem->pxaAmount->value,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
					GetTextExtentPoint32(pDraw->hDC,pItem->pxaAmount->value,wcslen(pItem->pxaAmount->value),&Size);
					rcDraw.left += Size.cx+8;
					}
				}

			//--- NumItems (bags) ---

			if (pItem->bIsBackPack)
				{
				WCHAR*		pszBuffer;
				WCHAR*		pszText;
				SIZE		Size;

				pszBuffer = NULL;

				if (pItem->uNumItems > 1) pszText = Locale_GetText(TEXT_OBJ_ITEMS_MULTIPLE);
				else if (pItem->uNumItems == 1) pszText = Locale_GetText(TEXT_OBJ_ITEMS_SINGLE);
				else pszText = Locale_GetText(TEXT_OBJ_EMPTY);

				if (pItem->uNumItems > 1)
					{
					DWORD_PTR	vl[1];

					vl[0] = pItem->uNumItems;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,pszText,0,0,(WCHAR *)&pszBuffer,1,(va_list *)vl);
					pszText = pszBuffer;
					}

				DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);

				if (pszBuffer) LocalFree(pszBuffer);

				rcDraw.left += Size.cx+8;
				}

			//--- Quality ---

			if (pItem->pxaType)
				{
				if (pItem->pxaType->value)
					{
					int		i;

					for (i = 0; Qualities[i].xmlName != NULL; i++)
						{
						if (wcscmp(pItem->pxaType->value,Qualities[i].xmlName)) continue;
						SetTextColor(pDraw->hDC,Qualities[i].crColor);
						DrawText(pDraw->hDC,Locale_GetText(Qualities[i].uNameId),-1,&rcDraw,DT_END_ELLIPSIS|DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
						}
					}
				}
			}
		}

	//--- Affichage du rectangle de focus ---

	if (pDraw->itemState&ODS_FOCUS)
		{
		SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
		DrawFocusRect(pDraw->hDC,&pDraw->rcItem);
		}

	//--- Surlignage si sélectionné ---

	if (pDraw->itemState&ODS_SELECTED)
		{
		HDC		hDC;
		HBITMAP		hBitmap;
		HBITMAP		hDefBitmap;
		RECT		rcRect;
		BLENDFUNCTION	blend;

		hDC = CreateCompatibleDC(pDraw->hDC);
		if (hDC)
			{
			hBitmap = CreateCompatibleBitmap(pDraw->hDC,16,16);
			if (hBitmap)
				{
				hDefBitmap = SelectObject(hDC,hBitmap);
				rcRect.left = 0;
				rcRect.top = 0;
				rcRect.right = 16;
				rcRect.bottom = 16;
				FillRect(hDC,&rcRect,GetSysColorBrush(COLOR_HIGHLIGHT));
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 50;
				blend.AlphaFormat = 0;
				AlphaBlend(pDraw->hDC,rcClient.left,rcClient.top,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top,hDC,0,0,8,8,blend);
				SelectObject(hDC,hDefBitmap);
				DeleteObject(hBitmap);
				}
			DeleteDC(hDC);
			}
		}

	SetBkMode(pDraw->hDC,iBack);
	SetTextColor(pDraw->hDC,crText);
	SelectObject(pDraw->hDC,hFont);
	return;
}


// «»»» Affichage de la fenêtre «««««««««««««««««««««««««««««««««««««««««»

void Game_Paint(HWND hWnd, HDC hDC, RECT *rcClient)
{
	static UINT	uAttrIcons[] = { APP_ICON_STAT_STR, APP_ICON_STAT_DEX, APP_ICON_STAT_INT, APP_ICON_STAT_CON, APP_ICON_STAT_MEM, APP_ICON_STAT_PER };
	GAMEDRAWCONTEXT	ctx;
	int		i;

	ZeroMemory(&ctx,sizeof(GAMEDRAWCONTEXT));

	if (App.Game.pdcCurrent)
		{
		ctx.pszLevel = ctx.szLevel;
		ctx.pszClass = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaClass);
		ctx.pszExp = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
		ctx.pszDamages = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaDamageCount);
		ctx.pszHeal = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaHealCount);
		ctx.pszKills = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaKillCount);
		ctx.pszArmor = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaArmor);
		ctx.pszMagicArmor = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaMagicArmor);
		ctx.pszVitality = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaVitality);
		if (!ctx.pszExp) ctx.pszExp = ctx.pszLevel = Locale_GetText(TEXT_UNKNOWN);
		else
			{
			Misc_Printf(ctx.szLevel,4,szLevelFmt,Game_GetLevelFromExp(wcstol(ctx.pszExp,NULL,10)));
			Misc_Printf(ctx.szNextLevel,12,szLevelFmt,Game_GetNextLevelFromExp(wcstol(ctx.pszExp,NULL,10)));
			}
		if (ctx.pszClass)
			{
			WCHAR*		pszBuffer;
			DWORD_PTR	vl[1];

			vl[0] = (DWORD_PTR)ctx.pszClass;
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,szClassFmt,0,0,(WCHAR *)&pszBuffer,1,(va_list *)vl))
				{
				CharUpper(pszBuffer);
				ctx.pszClass = Locale_GetText(Locale_GetTextUID(App.pLocaleTexts,pszBuffer));
				LocalFree(pszBuffer);
				}
			else ctx.pszClass = NULL;
			}
		}

	//--- Fiche du personnage ---

	CopyRect(&ctx.rcArea,rcClient);
	ctx.rcArea.left += 16;
	ctx.rcArea.top += 16;
	ctx.rcArea.bottom -= 16;
	ctx.rcArea.right = ctx.rcArea.left+400;
	DrawEdge(hDC,&ctx.rcArea,BDR_RAISEDINNER,BF_RECT);

	GetWindowRect(App.Game.Layout.hwndList,&ctx.rcWork);
	MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcWork,2);

	ctx.hFont = SelectObject(hDC,App.Font.hFont);
	ctx.crColor = SetTextColor(hDC,GetSysColor(COLOR_BTNTEXT));
	ctx.iBack = SetBkMode(hDC,TRANSPARENT);

	//--- Niveau

	ctx.rcWork.left += 10;
	ctx.rcWork.right -= 10;
	ctx.rcWork.top = ctx.rcWork.bottom+10;
	ctx.rcWork.bottom = ctx.rcWork.top+100;

	ctx.level.pszText = ctx.pszLevel?ctx.pszLevel:szZero;
	CopyRect(&ctx.rcText,&ctx.rcWork);
	if (ctx.pszClass)
		{
		GetTextExtentPoint32(hDC,ctx.pszClass,wcslen(ctx.pszClass),&ctx.level.sizeClass);
		GetTextExtentPoint32(hDC,szClassSpace,wcslen(szClassSpace),&ctx.level.sizeClassSpace);
		}
	GetTextExtentPoint32(hDC,Locale_GetText(TEXT_CHR_LEVEL),wcslen(Locale_GetText(TEXT_CHR_LEVEL)),&ctx.level.sizeLabel);
	GetTextExtentPoint32(hDC,szSpace,wcslen(szSpace),&ctx.level.sizeSpace);
	GetTextExtentPoint32(hDC,ctx.level.pszText,wcslen(ctx.level.pszText),&ctx.level.sizeLevel);
	ctx.rcText.left = ctx.rcText.left+((ctx.rcText.right-ctx.rcText.left)-(ctx.level.sizeClass.cx+ctx.level.sizeClassSpace.cx+ctx.level.sizeLabel.cx+ctx.level.sizeSpace.cx+ctx.level.sizeLevel.cx))/2;
	if (ctx.pszClass)
		{
		DrawText(hDC,ctx.pszClass,-1,&ctx.rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE);
		ctx.rcText.left += ctx.level.sizeClass.cx;
		DrawText(hDC,szClassSpace,-1,&ctx.rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE);
		ctx.rcText.left += ctx.level.sizeClassSpace.cx;
		}
	DrawText(hDC,Locale_GetText(TEXT_CHR_LEVEL),-1,&ctx.rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE);
	ctx.rcText.left += ctx.level.sizeLabel.cx;
	DrawText(hDC,szSpace,-1,&ctx.rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE);
	ctx.rcText.left += ctx.level.sizeSpace.cx;
	DrawText(hDC,ctx.level.pszText,-1,&ctx.rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE);

	ctx.rcText.left = ctx.rcArea.left;
	ctx.rcText.right = ctx.rcArea.right;
	ctx.rcText.top += ctx.level.sizeLabel.cy > ctx.level.sizeLevel.cy?ctx.level.sizeLabel.cy:ctx.level.sizeLevel.cy+10;
	ctx.rcText.bottom = ctx.rcText.top+2;
	DrawEdge(hDC,&ctx.rcText,EDGE_RAISED,BF_TOP|BF_BOTTOM);
	Game_PaintDecoration(hDC,ctx.rcText.left-8,ctx.rcText.top-4,App.Game.Layout.hDecoTop);
	ctx.rcText.top = ctx.rcText.bottom+10;

	//--- Attributs

	ctx.rcText.bottom += 20;
	for (i = 0; i != 6; i++)
		{
		SetWindowPos(App.Game.Layout.hwndAttrBtn[i],NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
		Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndAttrBtn[i],Locale_GetText(TextsAttr[i]),App.Game.pdcCurrent?xml_GetThisAttrValue(App.Game.pdcCurrent->pxaAttributes[i]):NULL,uAttrIcons[i]);
		ctx.rcText.bottom += App.Font.uFontHeight+20;
		}

	//--- Points à dépenser

	ctx.rcText.bottom += 10;
	for (i = 0; i != 4; i++)
		{
		SetWindowPos(App.Game.Layout.hwndPointsBtn[i],NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
		Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndPointsBtn[i],Locale_GetText(TextsPts[i]),App.Game.pdcCurrent?xml_GetThisAttrValue(App.Game.pdcCurrent->pxaPoints[i]):NULL,APP_ICON_STAT_POINT);
		ctx.rcText.bottom += App.Font.uFontHeight+20;
		}

	//--- Vitalité/Armures

	ctx.rcText.bottom += 10;
	SetWindowPos(App.Game.Layout.hwndLifeBtn,NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndLifeBtn,Locale_GetText(TEXT_CHR_LIFE),ctx.pszVitality,APP_ICON_STAT_LIFE);
	ctx.rcText.bottom += App.Font.uFontHeight+20;
	SetWindowPos(App.Game.Layout.hwndPhysicalBtn,NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndPhysicalBtn,Locale_GetText(TEXT_CHR_PHYSICAL),ctx.pszArmor,APP_ICON_STAT_PHYSICAL);
	ctx.rcText.bottom += App.Font.uFontHeight+20;
	SetWindowPos(App.Game.Layout.hwndMagicalBtn,NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndMagicalBtn,Locale_GetText(TEXT_CHR_MAGICAL),ctx.pszMagicArmor,APP_ICON_STAT_MAGICAL);
	ctx.rcText.bottom += App.Font.uFontHeight+20;

	//--- Expérience

	ctx.rcText.bottom += 10;
	SetWindowPos(App.Game.Layout.hwndExperienceBtn,NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndExperienceBtn,Locale_GetText(TEXT_CHR_EXP),ctx.pszExp,APP_ICON_STAT_XP);
	ctx.rcText.bottom += App.Font.uFontHeight+20;
	SetWindowPos(App.Game.Layout.hwndNextLevelBtn,NULL,ctx.rcArea.right-MAIN_ATTR_BTN_WIDTH-20,ctx.rcText.bottom,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndNextLevelBtn,Locale_GetText(TEXT_CHR_NEXTLEVEL),ctx.szNextLevel,APP_ICON_STAT_NEXT);
	//ctx.rcText.bottom += App.Font.uFontHeight+20;

	//--- Séparateur

	GetWindowRect(App.Game.Layout.hwndAbilitiesBtn,&ctx.rcText);
	MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcText,2);
	ctx.rcText.left = ctx.rcArea.left;
	ctx.rcText.right = ctx.rcArea.right;
	ctx.rcText.top -= 20;
	ctx.rcText.bottom = ctx.rcText.top+2;
	DrawEdge(hDC,&ctx.rcText,EDGE_RAISED,BF_TOP|BF_BOTTOM);
	Game_PaintDecoration(hDC,ctx.rcText.left-8,ctx.rcText.top-24,App.Game.Layout.hDecoBottom);

	//--- Inventaire ---

	CopyRect(&ctx.rcArea,rcClient);
	ctx.rcArea.left += 432;
	ctx.rcArea.top += 16;
	ctx.rcArea.bottom -= 16;
	ctx.rcArea.right = ctx.rcArea.left+570;
	DrawEdge(hDC,&ctx.rcArea,BDR_RAISEDINNER,BF_RECT);

	if (!IsWindowEnabled(App.Game.Layout.hwndInventory))
		{
		GetWindowRect(App.Game.Layout.hwndInventory,&ctx.rcArea);
		MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcArea,2);
		DrawEdge(hDC,&ctx.rcArea,BDR_SUNKEN,BF_RECT);
		}

	SetBkMode(hDC,ctx.iBack);
	SetTextColor(hDC,ctx.crColor);
	SelectObject(hDC,ctx.hFont);
	return;
}

//--- Affichage d'une décoration ---

void Game_PaintDecoration(HDC hDC, int X, int Y, HBITMAP hImage)
{
	if (hImage)
		{
		HDC hDrawDC = CreateCompatibleDC(NULL);
		if (hDrawDC)
			{
			BLENDFUNCTION blendfn;
			HBITMAP hDrawBitmap = SelectObject(hDrawDC,hImage);
			blendfn.BlendOp = AC_SRC_OVER;
			blendfn.BlendFlags = 0;
			blendfn.SourceConstantAlpha = 255;
			blendfn.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend(hDC,X,Y,415,30,hDrawDC,0,0,415,30,blendfn);
			SelectObject(hDC,hDrawBitmap);
			DeleteDC(hDrawDC);
			}
		}
	return;
}

//--- Affichage des attributs et des points ---

void Game_PaintValue(HDC hDC, LONG iLeft, HWND hButton, WCHAR *pszLabel, WCHAR *pszValue, UINT uIconId)
{
	HPEN	hPen;
	RECT	rcArea;
	RECT	rcEdge;

	GetWindowRect(hButton,&rcArea);
	MapWindowPoints(NULL,App.hWnd,(POINT *)&rcArea,2);
	rcArea.top += 4;
	rcArea.bottom -= 6;
	CopyRect(&rcEdge,&rcArea);

	rcArea.right = rcArea.left-8-60;
	rcArea.left = iLeft;
	if (uIconId)
		{
		DrawIconEx(hDC,rcArea.left,rcArea.top,App.hIcons[uIconId],24,24,0,NULL,DI_NORMAL);
		rcArea.left += 26;
		}
	DrawText(hDC,pszLabel,-1,&rcArea,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);
	rcArea.left = rcArea.right;
	rcArea.right += 60;
	DrawText(hDC,pszValue,-1,&rcArea,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_RIGHT);

	rcEdge.right = rcEdge.left-8;
	rcEdge.left = iLeft+(uIconId?26:0);
	rcEdge.top = rcEdge.bottom+4;
	hPen = CreatePen(PS_DOT,1,GetSysColor(COLOR_BTNSHADOW));
	if (hPen)
		{
		HPEN hDefPen = SelectObject(hDC,hPen);
		MoveToEx(hDC,rcEdge.left,rcEdge.top,NULL);
		LineTo(hDC,rcEdge.right,rcEdge.top);
		SelectObject(hDC,hDefPen);
		DeleteObject(hPen);
		}

	return;
}

//--- Affichage des boutons ---

void Game_PaintButton(DRAWITEMSTRUCT *pDraw)
{
	Dialog_DrawTextButton(szEdit,pDraw);
	//Dialog_DrawArrowButton(DFCS_SCROLLRIGHT,pDraw);
	return;
}

//--- Affichage du sac en cours ---

void Game_PaintBag(DRAWITEMSTRUCT *pDraw)
{
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	if (App.Game.pdcCurrent)
		{
		if (App.Game.pdcCurrent->pdiInventory)
			{
			WCHAR*		pszText;

			pszText = App.Game.pdcCurrent->pdiInventory->pszDisplayName;
			if (!pszText) pszText = App.Game.pdcCurrent->pdiInventory->pszNameRef;

			if (pszText)
				{
				HFONT		hFont;
				RECT		rcDraw;
				COLORREF	crText;
				int		iBkMode;

				hFont = SelectObject(pDraw->hDC,App.Font.hFont);
				crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
				iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);
				CopyRect(&rcDraw,&pDraw->rcItem);
				rcDraw.right = rcDraw.left+GAME_ICON_SIZE;
				Game_PaintIcon(pDraw->hDC,App.Game.pdcCurrent->pdiInventory->pszNameRef,APP_ICON_BACKPACK,&rcDraw,GAME_ICON_SIZE,FALSE,FALSE);
				CopyRect(&rcDraw,&pDraw->rcItem);
				rcDraw.left += GAME_ICON_SIZE;
				DrawEdge(pDraw->hDC,&rcDraw,BDR_SUNKENOUTER,BF_LEFT);
				rcDraw.left += 4;
				rcDraw.right -= 4;
				DrawText(pDraw->hDC,pszText,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
				SetTextColor(pDraw->hDC,crText);
				SetBkMode(pDraw->hDC,iBkMode);
				SelectObject(pDraw->hDC,hFont);
				}
			}
		}

	return;
}

//--- Affichage de l'icône ---

//!\ Modifie RECT

void Game_PaintIcon(HDC hDC, WCHAR *pszObjectName, UINT uDefIcon, RECT *rcDraw, int iSize, BOOL bDrawBack, BOOL bDrawFrame)
{
	HICON	hIcon = NULL;

	if (App.Game.hRunesIconsList && pszObjectName)
		{
		hIcon = LoadImage(App.Game.hRunesIconsList,pszObjectName,IMAGE_ICON,GAME_ICON_SIZE,GAME_ICON_SIZE,LR_DEFAULTCOLOR|LR_SHARED);
		}
	if (App.Game.hIconsList && !hIcon)
		{
		WCHAR *pszIconName = Game_GetItemIconName(pszObjectName);
		if (pszIconName) hIcon = LoadImage(App.Game.hIconsList,pszIconName,IMAGE_ICON,GAME_ICON_SIZE,GAME_ICON_SIZE,LR_DEFAULTCOLOR|LR_SHARED);
		}
	if (!hIcon) hIcon = App.hIcons[uDefIcon];
	if (bDrawBack)
		{
		int iBackSize = iSize+4;
		DrawIconEx(hDC,rcDraw->left,rcDraw->top+(rcDraw->bottom-rcDraw->top-iBackSize)/2,App.hIcons[APP_ICON_BACKGROUND],iBackSize,iBackSize,0,NULL,DI_NORMAL);
		rcDraw->left += 2;
		rcDraw->right = rcDraw->left+iSize;
		}
	else rcDraw->right = rcDraw->left+iSize;
	DrawIconEx(hDC,rcDraw->left+(rcDraw->right-rcDraw->left-iSize)/2,rcDraw->top+(rcDraw->bottom-rcDraw->top-iSize)/2,hIcon,iSize,iSize,0,NULL,DI_NORMAL);
	if (bDrawFrame) DrawIconEx(hDC,rcDraw->left,rcDraw->top+(rcDraw->bottom-rcDraw->top-iSize)/2,App.hIcons[APP_ICON_FRAME],iSize,iSize,0,NULL,DI_NORMAL);
	return;
}

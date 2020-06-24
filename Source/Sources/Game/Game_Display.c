
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

extern APPLICATION	App;
extern GAMEQUALITY	Qualities[];
extern GAMEPORTRAIT	Portraits[];
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
			if (!wcsncmp(L"Generic",(WCHAR *)pszArgs[1],7)) pszArgs[1] = (DWORD_PTR)Locale_GetText(TEXT_CHR_NOORIGIN);
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
			DWORD_PTR	vl[2];

			vl[0] = pszArgs[2];
			vl[1] = (DWORD_PTR)(xml_IsTrue(pdc->pxaIsMale)?szMale:szFemale);
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
				WCHAR*		Text;
				SIZE		Size;

				Text = Locale_GetText(pItem->uNumItems?TEXT_OBJ_ITEMS:TEXT_OBJ_EMPTY);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				if (pItem->uNumItems)
					{
					WCHAR		szBuffer[11];

					rcDraw.left += Size.cx+2;
					swprintf(szBuffer,10,L"%u",pItem->uNumItems);
					DrawText(pDraw->hDC,szBuffer,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
					GetTextExtentPoint32(pDraw->hDC,szBuffer,wcslen(szBuffer),&Size);
					}
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
	GAMEDRAWCONTEXT	ctx;
	int		i;

	ZeroMemory(&ctx,sizeof(GAMEDRAWCONTEXT));
	ctx.uPortraitIcon = APP_ICON_MAN;

	if (App.Game.pdcCurrent)
		{
		ctx.pszLevel = ctx.szLevel;
		ctx.bIsMale = xml_IsTrue(App.Game.pdcCurrent->pxaIsMale);
		ctx.pszExp = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
		ctx.pszDamages = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaDamageCount);
		ctx.pszHeal = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaHealCount);
		ctx.pszKills = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaKillCount);
		ctx.uPortraitIcon = ctx.bIsMale?APP_ICON_MAN:APP_ICON_WOMAN;
		if (App.Game.pdcCurrent->pxaOriginName)
			{
			WCHAR* Origin = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaOriginName);
			if (Origin)
				{
				for (i = 0; Portraits[i].pszName != NULL; i++)
					{
					if (wcscmp(Origin,Portraits[i].pszName)) continue;
					ctx.uPortraitIcon = Portraits[i].uIconId;
					break;
					}
				}
			}
		if (!ctx.pszExp) ctx.pszExp = ctx.pszLevel = Locale_GetText(TEXT_UNKNOWN);
		else wsprintf(ctx.szLevel,szLevelFmt,Game_GetLevelFromExp(wcstol(ctx.pszExp,NULL,10)));
		}

	//--- Fiche du personnage ---

	CopyRect(&ctx.rcArea,rcClient);
	ctx.rcArea.left += 16;
	ctx.rcArea.top += 16;
	ctx.rcArea.bottom -= 16;
	ctx.rcArea.right = ctx.rcArea.left+400;
	DrawEdge(hDC,&ctx.rcArea,BDR_RAISEDINNER,BF_RECT);

	//--- Portrait

	GetWindowRect(App.Game.Layout.hwndList,&ctx.rcWork);
	MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcWork,2);
	ctx.rcWork.left = ctx.rcArea.left+20;
	ctx.rcWork.right = ctx.rcWork.left+80;
	ctx.rcWork.top = ctx.rcWork.bottom+20;
	ctx.rcWork.bottom = ctx.rcWork.top+100;
	DrawIconEx(hDC,ctx.rcWork.left,ctx.rcWork.top,App.hIcons[ctx.uPortraitIcon],80,100,0,NULL,DI_NORMAL);
	DrawEdge(hDC,&ctx.rcWork,BDR_SUNKENOUTER,BF_RECT);

	ctx.hFont = SelectObject(hDC,App.Font.hFont);
	ctx.crColor = SetTextColor(hDC,GetSysColor(COLOR_BTNTEXT));
	ctx.iBack = SetBkMode(hDC,TRANSPARENT);

	//--- Informations

	ctx.rcText.left = ctx.rcWork.right+8;
	ctx.rcText.top = ctx.rcWork.top;
	ctx.rcText.right = ctx.rcArea.right-20;
	ctx.rcText.bottom = ctx.rcText.top+App.Font.uFontHeight;
	Game_PaintText(hDC,ctx.pszLevel,Locale_GetText(TEXT_CHR_LEVEL),&ctx.rcText);
	Game_PaintText(hDC,ctx.pszExp,Locale_GetText(TEXT_CHR_EXP),&ctx.rcText);
	Game_PaintText(hDC,ctx.pszDamages,Locale_GetText(TEXT_CHR_DAMAGES),&ctx.rcText);
	Game_PaintText(hDC,ctx.pszHeal,Locale_GetText(TEXT_CHR_HEAL),&ctx.rcText);
	Game_PaintText(hDC,ctx.pszKills,Locale_GetText(TEXT_CHR_KILLS),&ctx.rcText);

	ctx.rcText.left = ctx.rcArea.left;
	ctx.rcText.top = ctx.rcWork.bottom+20;
	ctx.rcText.bottom = ctx.rcText.top+2;
	ctx.rcText.right = ctx.rcArea.right;
	DrawEdge(hDC,&ctx.rcText,BDR_SUNKENOUTER,BF_TOP|BF_BOTTOM);

	//--- Attributs

	for (i = 0; i != 6; i++)
		Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndAttrBtn[i],Locale_GetText(TextsAttr[i]),App.Game.pdcCurrent?App.Game.pdcCurrent->pxaAttributes[i]->value:NULL);

	//--- Points à dépenser

	for (i = 0; i != 4; i++)
		Game_PaintValue(hDC,ctx.rcArea.left+24,App.Game.Layout.hwndPointsBtn[i],Locale_GetText(TextsPts[i]),App.Game.pdcCurrent?App.Game.pdcCurrent->pxaPoints[i]->value:NULL);

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

//--- Affichage des textes ---

void Game_PaintText(HDC hDC, WCHAR *pszText, WCHAR *pszLabel, RECT *rcText)
{
	HPEN	hPen;
	SIZE	sizeLabel;
	SIZE	sizeText;
	LONG	lSave;

	ZeroMemory(&sizeText,sizeof(SIZE));
	GetTextExtentPoint32(hDC,pszLabel,wcslen(pszLabel),&sizeLabel);
	DrawText(hDC,pszLabel,-1,rcText,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
	if (pszText)
		{
		WCHAR*	pszNumber;
		WCHAR*	pszRealText;
		pszNumber = Misc_FormatNumber(pszText);
		pszRealText = pszNumber?pszNumber:pszText;
		lSave = rcText->left;
		rcText->left += sizeLabel.cx+4;
		GetTextExtentPoint32(hDC,pszRealText,wcslen(pszRealText),&sizeText);
		DrawText(hDC,pszRealText,-1,rcText,DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER);
		rcText->left = lSave;
		if (pszNumber) HeapFree(App.hHeap,0,pszNumber);
		}

	hPen = CreatePen(PS_DOT,1,GetSysColor(COLOR_BTNSHADOW));
	if (hPen)
		{
		HPEN hpenDefault = SelectObject(hDC,hPen);
		int y = rcText->top+sizeLabel.cy/2;
		MoveToEx(hDC,rcText->left+sizeLabel.cx+10,y,NULL);
		LineTo(hDC,rcText->right-sizeText.cx-10,y);
		SelectObject(hDC,hpenDefault);
		DeleteObject(hPen);
		}

	rcText->top += App.Font.uFontHeight+4;
	rcText->bottom = rcText->top+App.Font.uFontHeight;
	return;
}

//--- Affichage des attributs et des points ---

void Game_PaintValue(HDC hDC, LONG iLeft, HWND hButton, WCHAR *pszLabel, WCHAR *pszValue)
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
	DrawText(hDC,pszLabel,-1,&rcArea,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_LEFT);
	rcArea.left = rcArea.right;
	rcArea.right += 60;
	DrawText(hDC,pszValue,-1,&rcArea,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_RIGHT);

	rcEdge.right = rcEdge.left-8;
	rcEdge.left = iLeft;
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
	SIZE		Size;
	COLORREF	crText;
	int		iBkMode;

	crText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(pDraw->hDC,TRANSPARENT);

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	DrawEdge(pDraw->hDC,&pDraw->rcItem,EDGE_ETCHED,BF_RECT);
	GetTextExtentPoint32(pDraw->hDC,szEdit,wcslen(szEdit),&Size);
	DrawState(pDraw->hDC,NULL,NULL,(LPARAM)szEdit,0,pDraw->rcItem.left+(pDraw->rcItem.right-pDraw->rcItem.left-Size.cx)/2,pDraw->rcItem.top+(pDraw->rcItem.bottom-pDraw->rcItem.top-Size.cy)/2,Size.cx,Size.cy,DST_TEXT|((pDraw->itemState&ODS_DISABLED)?DSS_DISABLED:0));

	if (pDraw->itemState&ODS_FOCUS) DrawFocusRect(pDraw->hDC,&pDraw->rcItem);

	SetTextColor(pDraw->hDC,crText);
	SetBkMode(pDraw->hDC,iBkMode);
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

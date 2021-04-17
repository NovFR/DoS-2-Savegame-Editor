
//<<>-<>>---------------------------------------------------------------------()
/*
	Affichage
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
// ¤¤¤ Affichage de la liste des personnages				  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mesure la hauteur des éléments ««««««««««««««««««««««««««««««««««»

void Game_MeasureList(MEASUREITEMSTRUCT *pMeasure)
{
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = App.Font.uFontHeight+4;
	if (pMeasure->itemHeight < 24+4) pMeasure->itemHeight = 24+4;
	return;
}


// «»»» Affichage d'un élément ««««««««««««««««««««««««««««««««««««««««««»

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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage de la liste des objets					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mesure la hauteur des éléments ««««««««««««««««««««««««««««««««««»

void Game_MeasureInventory(MEASUREITEMSTRUCT *pMeasure)
{
	pMeasure->itemWidth = 0;
	pMeasure->itemHeight = (App.Font.uFontHeight+8)*2+4;
	if (pMeasure->itemHeight < GAME_ICON_SIZE+8) pMeasure->itemHeight = GAME_ICON_SIZE+8;
	return;
}


// «»»» Affichage d'un élément ««««««««««««««««««««««««««««««««««««««««««»

void Game_DrawInventory(DRAWITEMSTRUCT *pDraw)
{
	DOS2ITEM*	pItem;
	WCHAR*		pszText;
	HFONT		hFont;
	RECT		rcDraw;
	RECT		rcClient;
	COLORREF	crText;
	int		iBack;

	pszText = NULL;
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
			SIZE		Size;
			RECT		rcText;
			WCHAR*		pszLevel = NULL;
			DWORD_PTR	dptrLevel = 0;

			//--- Icon ---

			CopyRect(&rcDraw,&rcClient);
			Game_PaintIcon(pDraw->hDC,xml_GetThisAttrValue(pItem->pxaStats),pItem->bIsBackPack?APP_ICON_BACKPACK:APP_ICON_EMPTY,&rcDraw,GAME_ICON_SIZE,TRUE,TRUE);

			//--- Display area & Level text ---

			CopyRect(&rcDraw,&rcClient);
			if (pItem->pxaLevel)
				{
				if (pItem->pxaLevel->value)
					{
					dptrLevel = wcstol(pItem->pxaLevel->value,NULL,10);
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,Locale_GetText(TEXT_OBJ_LEVEL),0,0,(WCHAR *)&pszLevel,1,(va_list *)&dptrLevel);
					}
				}
			rcDraw.left += GAME_ICON_SIZE+8;
			rcDraw.right -= 8;

			//--- DisplayName, Level ---

			// Area
			CopyRect(&rcText,&rcDraw);
			if (pszLevel)
				{
				GetTextExtentPoint32(pDraw->hDC,pszLevel,wcslen(pszLevel),&Size);
				rcText.right -= Size.cx+8;
				}
			// DisplayName
			Game_ItemDisplayName(pItem);
			pszText = pItem->pszDisplayName;
			if (!pszText) pszText = Locale_GetText(TEXT_UNKNOWN);
			GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
			DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
			// Level
			if (pszLevel)
				{
				DrawText(pDraw->hDC,pszLevel,-1,&rcDraw,DT_END_ELLIPSIS|DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				LocalFree(pszLevel);
				}
			rcDraw.top += Size.cy+1;

			//--- Stats and Quality ---

			// Area
			CopyRect(&rcText,&rcDraw);
			pszText = xml_GetThisAttrValue(pItem->pxaType);
			if (pszText)
				{
				GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
				rcText.right -= Size.cx+8;
				}
			// Stats
			pszText = xml_GetThisAttrValue(pItem->pxaStats);
			if (!pszText) pszText = szNULLItem;
			SetTextColor(pDraw->hDC,App.Config.crStats);
			GetTextExtentPoint32(pDraw->hDC,pszText,wcslen(pszText),&Size);
			DrawText(pDraw->hDC,pszText,-1,&rcText,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
			SetTextColor(pDraw->hDC,GetSysColor(COLOR_WINDOWTEXT));
			// Quality
			if ((pszText = xml_GetThisAttrValue(pItem->pxaType)))
				{
				COLORREF	crText;
				int		i;

				for (i = 0; Qualities[i].xmlName != NULL; i++)
					{
					if (wcscmp(pItem->pxaType->value,Qualities[i].xmlName)) continue;
					crText = SetTextColor(pDraw->hDC,Qualities[i].crColor);
					DrawText(pDraw->hDC,Locale_GetText(Qualities[i].uNameId),-1,&rcDraw,DT_END_ELLIPSIS|DT_RIGHT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
					SetTextColor(pDraw->hDC,crText);
					break;
					}
				}
			rcDraw.top += Size.cy+1;

			//--- Runes ---

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
				WCHAR *Text = Locale_GetText(TEXT_OBJ_EQUIPPED);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Generated ? ---

			if (xml_IsTrue(pItem->pxaIsGenerated))
				{
				WCHAR *Text = Locale_GetText(TEXT_OBJ_GENERATED);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Custom Base ? ---

			if (xml_IsTrue(pItem->pxaHasCustomBase))
				{
				WCHAR *Text = Locale_GetText(TEXT_OBJ_CUSTOM);
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Stolen ? ---

			if (!Game_ItemBelongToCharacter(pItem) && !Game_ItemBelongToParty(pItem))
				{
				WCHAR*		Text;
				COLORREF	crText;

				Text = Locale_GetText(TEXT_OBJ_STOLEN);
				crText = SetTextColor(pDraw->hDC,RGB(220,0,0));
				DrawText(pDraw->hDC,Text,-1,&rcDraw,DT_END_ELLIPSIS|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_TOP);
				SetTextColor(pDraw->hDC,crText);
				GetTextExtentPoint32(pDraw->hDC,Text,wcslen(Text),&Size);
				rcDraw.left += Size.cx+8;
				}

			//--- Amount ---

			if (pItem->pxaAmount)
				{
				if (pItem->pxaAmount->value)
					{
					WCHAR *Text = Locale_GetText(TEXT_OBJ_AMOUNT);
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage de la fenêtre						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Mise-à-jour de l'affichage (WM_PAINT) «««««««««««««««««««««««««««»

void Game_Paint(HWND hWnd, HDC hDC, RECT *rcClient)
{
	GAMEDRAWCONTEXT	ctx;

	ZeroMemory(&ctx,sizeof(GAMEDRAWCONTEXT));

	if (App.Game.pdcCurrent)
		{
		ctx.pszLevel = ctx.szLevel;
		ctx.pszClass = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaClass);
		ctx.pszExp = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
		if (!ctx.pszExp) ctx.pszExp = ctx.pszLevel = Locale_GetText(TEXT_UNKNOWN);
		else Misc_Printf(ctx.szLevel,4,szLevelFmt,Game_GetLevelFromExp(wcstol(ctx.pszExp,NULL,10)));
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

	//--- Attributs

	GetWindowRect(App.Game.Layout.hwndAbilitiesBtn,&ctx.rcRgn);
	MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcRgn,2);
	ctx.rcRgn.bottom = ctx.rcRgn.top-20;
	ctx.rcRgn.top = ctx.rcText.top+(ctx.level.sizeLabel.cy > ctx.level.sizeLevel.cy?ctx.level.sizeLabel.cy:ctx.level.sizeLevel.cy+10);
	ctx.rcRgn.left = ctx.rcArea.left+1;
	ctx.rcRgn.right = ctx.rcArea.right-1;
	SetWindowPos(App.Game.Layout.hwndContainer,NULL,ctx.rcRgn.left,ctx.rcRgn.top,ctx.rcRgn.right-ctx.rcRgn.left,ctx.rcRgn.bottom-ctx.rcRgn.top,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	InvalidateRect(App.Game.Layout.hwndContainer,NULL,FALSE);

	//--- Inventaire ---

	CopyRect(&ctx.rcArea,rcClient);
	ctx.rcArea.left += 432;
	ctx.rcArea.top += 16;
	ctx.rcArea.bottom -= 16;
	ctx.rcArea.right -= 16;
	DrawEdge(hDC,&ctx.rcArea,BDR_RAISEDINNER,BF_RECT);

	if (!IsWindowEnabled(App.Game.Layout.hwndInventory))
		{
		GetWindowRect(App.Game.Layout.hwndInventory,&ctx.rcArea);
		MapWindowPoints(NULL,hWnd,(POINT *)&ctx.rcArea,2);
		FillRect(hDC,&ctx.rcArea,GetSysColorBrush(COLOR_WINDOW));
		DrawEdge(hDC,&ctx.rcArea,BDR_SUNKEN,BF_RECT);
		}

	SetBkMode(hDC,ctx.iBack);
	SetTextColor(hDC,ctx.crColor);
	SelectObject(hDC,ctx.hFont);
	return;
}


// «»»» Affichage d'une décoration ««««««««««««««««««««««««««««««««««««««»

//--- Affichage des bordures ---

void Game_PaintDecorationBorder(DRAWITEMSTRUCT *pDraw, BOOL bTop)
{
	RECT	rcDraw;

	CopyRect(&rcDraw,&pDraw->rcItem);
	if (bTop)
		{
		rcDraw.bottom = rcDraw.top+4;
		FillRect(pDraw->hDC,&rcDraw,GetSysColorBrush(COLOR_BTNFACE));
		rcDraw.top += 4;
		rcDraw.bottom = rcDraw.top+2;
		DrawEdge(pDraw->hDC,&rcDraw,EDGE_RAISED,BF_TOP|BF_BOTTOM);
		}
	else
		{
		rcDraw.top = rcDraw.bottom-4;
		FillRect(pDraw->hDC,&rcDraw,GetSysColorBrush(COLOR_BTNFACE));
		rcDraw.top -= 2;
		rcDraw.bottom = rcDraw.top+2;
		DrawEdge(pDraw->hDC,&rcDraw,EDGE_RAISED,BF_TOP|BF_BOTTOM);
		}

	return;
}

//--- Affichage du PNG ---

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
			AlphaBlend(hDC,X,Y,WINDOW_DECO_WIDTH,WINDOW_DECO_HEIGHT,hDrawDC,0,0,WINDOW_DECO_WIDTH,WINDOW_DECO_HEIGHT,blendfn);
			SelectObject(hDC,hDrawBitmap);
			DeleteDC(hDrawDC);
			}
		}
	return;
}


// «»»» Affichage des attributs «««««««««««««««««««««««««««««««««««««««««»

void Game_PaintAttributes(HWND hWnd, HDC hDC, RECT *rcClient)
{
	static UINT	uAttrIcons[] = { APP_ICON_STAT_STR, APP_ICON_STAT_DEX, APP_ICON_STAT_INT, APP_ICON_STAT_CON, APP_ICON_STAT_MEM, APP_ICON_STAT_PER };
	GAMEDRAWCONTEXT	ctx;
	int		i;

	ZeroMemory(&ctx,sizeof(GAMEDRAWCONTEXT));

	if (App.Game.pdcCurrent)
		{
		ctx.pszArmor = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaArmor);
		ctx.pszMagicArmor = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaMagicArmor);
		ctx.pszVitality = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaVitality);
		ctx.pszExp = xml_GetThisAttrValue(App.Game.pdcCurrent->pxaExp);
		if (!ctx.pszExp)
			{
			ctx.pszExp = szZero;
			wcscpy(ctx.szNextLevel,szZero);
			}
		else Misc_Printf(ctx.szNextLevel,12,szLevelFmt,Game_GetNextLevelFromExp(wcstol(ctx.pszExp,NULL,10)));
		}

	ctx.hFont = SelectObject(hDC,App.Font.hFont);
	ctx.crColor = SetTextColor(hDC,GetSysColor(COLOR_WINDOWTEXT));
	ctx.iBack = SetBkMode(hDC,TRANSPARENT);

	GetClientRect(hWnd,&ctx.rcArea);
	ctx.rcArea.top += 20;

	// Attributs
	for (i = 0; i != 6; i++) ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndAttrBtn[i],&ctx.rcArea,TextsAttr[i],App.Game.pdcCurrent?xml_GetThisAttrValue(App.Game.pdcCurrent->pxaAttributes[i]):NULL,uAttrIcons[i]);

	// Points à dépenser
	ctx.rcArea.top += 10;
	for (i = 0; i != 4; i++) ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndPointsBtn[i],&ctx.rcArea,TextsPts[i],App.Game.pdcCurrent?xml_GetThisAttrValue(App.Game.pdcCurrent->pxaPoints[i]):NULL,APP_ICON_STAT_POINT);

	// Vitalité/Armures
	ctx.rcArea.top += 10;
	ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndLifeBtn,&ctx.rcArea,TEXT_CHR_LIFE,ctx.pszVitality,APP_ICON_STAT_LIFE);
	ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndPhysicalBtn,&ctx.rcArea,TEXT_CHR_PHYSICAL,ctx.pszArmor,APP_ICON_STAT_PHYSICAL);
	ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndMagicalBtn,&ctx.rcArea,TEXT_CHR_MAGICAL,ctx.pszMagicArmor,APP_ICON_STAT_MAGICAL);

	// Expérience
	ctx.rcArea.top += 10;
	ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndExperienceBtn,&ctx.rcArea,TEXT_CHR_EXP,ctx.pszExp,APP_ICON_STAT_XP);
	ctx.rcArea.top = Game_PaintStat(hDC,App.Game.Layout.hwndNextLevelBtn,&ctx.rcArea,TEXT_CHR_NEXTLEVEL,ctx.szNextLevel,APP_ICON_STAT_NEXT);

	ctx.rcArea.top += 20;
	if (ctx.rcArea.top != ctx.rcArea.bottom)
		{
		RECT	rcContainer;

		GetClientRect(App.Game.Layout.hwndContainer,&rcContainer);
		if (rcContainer.bottom < ctx.rcArea.top) ctx.rcArea.right -= MAIN_ATTR_SCROLLBAR_TOTALWIDTH-MAIN_ATTR_RIGHTMARGIN;
		SetWindowPos(hWnd,NULL,0,0,ctx.rcArea.right,ctx.rcArea.top,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
		}

	SetBkMode(hDC,ctx.iBack);
	SetTextColor(hDC,ctx.crColor);
	SelectObject(hDC,ctx.hFont);
	return;
}


// «»»» Affichage d'un attribut «««««««««««««««««««««««««««««««««««««««««»

int Game_PaintStat(HDC hDC, HWND hwndButton, RECT *rcDrawArea, UINT uLocaleLabelId, WCHAR *pszValue, UINT uIconId)
{
	SetWindowPos(hwndButton,NULL,rcDrawArea->right-MAIN_ATTR_BTN_WIDTH-MAIN_ATTR_RIGHTMARGIN,rcDrawArea->top,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	Game_PaintValue(hDC,rcDrawArea->left+24,hwndButton,Locale_GetText(uLocaleLabelId),pszValue,uIconId);
	return(rcDrawArea->top+App.Font.uFontHeight+20);
}


// «»»» Affichage des attributs et des points «««««««««««««««««««««««««««»

void Game_PaintValue(HDC hDC, LONG iLeft, HWND hButton, WCHAR *pszLabel, WCHAR *pszValue, UINT uIconId)
{
	HPEN	hPen;
	RECT	rcArea;
	RECT	rcEdge;

	GetWindowRect(hButton,&rcArea);
	MapWindowPoints(NULL,App.Game.Layout.hwndAttributes,(POINT *)&rcArea,2);
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


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichages divers						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage des boutons «««««««««««««««««««««««««««««««««««««««««««»

void Game_PaintButton(DRAWITEMSTRUCT *pDraw)
{
	Dialog_DrawTextButton(szEdit,pDraw);
	//Dialog_DrawArrowButton(DFCS_SCROLLRIGHT,pDraw);
	return;
}


// «»»» Affichage du sac en cours «««««««««««««««««««««««««««««««««««««««»

void Game_PaintBag(DOS2CHARACTER *pdcCurrent, DRAWITEMSTRUCT *pDraw)
{
	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));

	if (pdcCurrent)
		{
		if (pdcCurrent->pdiInventory)
			{
			WCHAR*		pszText = NULL;

			if (pdcCurrent->pdiInventory->pParentItem)
				{
				Game_ItemDisplayName(pdcCurrent->pdiInventory->pParentItem);
				pszText = ((DOS2ITEM *)pdcCurrent->pdiInventory->pParentItem)->pszDisplayName;
				}

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
				Game_PaintIcon(pDraw->hDC,xml_GetThisAttrValue(((DOS2ITEM *)pdcCurrent->pdiInventory->pParentItem)->pxaStats),APP_ICON_BACKPACK,&rcDraw,GAME_ICON_SIZE,FALSE,FALSE);
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


// «»»» Affichage de l'icône ««««««««««««««««««««««««««««««««««««««««««««»

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

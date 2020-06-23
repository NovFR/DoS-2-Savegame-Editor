
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Capacités
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "Locale.h"
#include "Dialogs.h"
#include "Menus.h"
#include "Requests.h"

extern APPLICATION	App;

static GAMEABILITY	GameAbilities[] = {	{ -1, TEXT_ABILITIES_WEAPONS,		200, 0,  0 },
						{ 34, TEXT_ABILITIES_DUALWIELD,		210, 0, 10 },
						{  4, TEXT_ABILITIES_TWOHANDED,		220, 0, 10 },
						{  6, TEXT_ABILITIES_RANGED,		230, 0, 10 },
						{  3, TEXT_ABILITIES_ONEHANDED, 	240, 0, 10 },
						{ -1, TEXT_ABILITIES_DEFENSE,		300, 0, 0 },
						{  5, TEXT_ABILITIES_PAINREFLECTION,	310, 0, 10 },
						{ 32, TEXT_ABILITIES_LEADERSHIP,	320, 0, 10 },
						{ 36, TEXT_ABILITIES_PERSEVERANCE,	330, 0, 10 },
						{ -1, TEXT_ABILITIES_ABILITIES,		400, 0, 0 },
						{  0, TEXT_ABILITIES_WARFARE,		410, 0, 10 },
						{ 15, TEXT_ABILITIES_AIR,		420, 0, 10 },
						{  1, TEXT_ABILITIES_HUNTING,		430, 0, 10 },
						{  2, TEXT_ABILITIES_ROGUES,		440, 0, 10 },
						{ 16, TEXT_ABILITIES_EARTH,		450, 0, 10 },
						{ 14, TEXT_ABILITIES_WATER,		460, 0, 10 },
						{ 18, TEXT_ABILITIES_SUMMONING,		470, 0, 10 },
						{ 19, TEXT_ABILITIES_POLYMORPH,		480, 0, 99 },
						{ 17, TEXT_ABILITIES_DEATH,		490, 0, 10 },
						{ 13, TEXT_ABILITIES_FIRE,		500, 0, 10 },
						{ -1, TEXT_ABILITIES_PERSONNALITY,	600, 0, 0 },
						{ 33, TEXT_ABILITIES_LUCK,		610, 0,  5 },
						{ 27, TEXT_ABILITIES_BARTERING,		620, 0,  5 },
						{ 31, TEXT_ABILITIES_PERSUASION,	630, 0,  5 },
						{ -1, TEXT_ABILITIES_HABILITY,		700, 0, 0 },
						{ 25, TEXT_ABILITIES_LOREMASTER,	710, 0,  5 },
						{ 20, TEXT_ABILITIES_TELEKINESIS,	720, 0,  5 },
						{ -1, TEXT_ABILITIES_MISDEED,		800, 0, 0 },
						{ 24, TEXT_ABILITIES_LOCKPICKING,	810, 0,  5 },
						{ 22, TEXT_ABILITIES_SNEAKING,		820, 0,  5 },
						{ -2, 0, 0, 0, 0 } };


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisation							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage de la boîte de dialogue «««««««««««««««««««««««««««««««»

void Game_Abilities()
{
	GAMEABILITY*	Abilities;
	INT_PTR		iResult;
	UINT		uCount;
	BOOL		bAlreadyAsked;

	for (uCount = 0; GameAbilities[uCount].uIndex != -2; uCount++);

	Abilities = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,++uCount*sizeof(GAMEABILITY));
	if (!Abilities)
		{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return;
		}
	CopyMemory(Abilities,GameAbilities,uCount*sizeof(GAMEABILITY));

	for (bAlreadyAsked = FALSE, uCount = 0; Abilities[uCount].uIndex != -2; uCount++)
		{
		if (Abilities[uCount].uIndex == -1) continue;
		Abilities[uCount].uValue = wcstol(xml_GetThisAttrValue(App.Game.pdcCurrent->pxaAbilities[Abilities[uCount].uIndex]),NULL,10);
		//--- Cap check
		if (Abilities[uCount].uValue > Abilities[uCount].uMax && !App.Config.bCapOverride && !bAlreadyAsked)
			{
			bAlreadyAsked = TRUE;
			if (MessageBox(App.hWnd,Locale_GetText(TEXT_OVERRIDE_ABILITIES),Locale_GetText(TEXT_TITLE_REQUEST),MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
				App.Config.bCapOverride = TRUE;
				Menu_SetFlag(App.hMenu,IDM_CONFIGCAPOVERRIDE,App.Config.bCapOverride);
				}
			}
		}

	iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1007),App.hWnd,Game_AbilitiesProc,(LPARAM)Abilities);
	if (iResult == -1) Request_PrintError(App.hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);

	if (iResult == IDOK)
		{
		for (uCount = 0; Abilities[uCount].uIndex != -2; uCount++)
			{
			if (Abilities[uCount].uIndex == -1) continue;
			xml_SetAttrValueNumber(App.Game.pdcCurrent->pxaAbilities[Abilities[uCount].uIndex],Abilities[uCount].uValue);
			}
		}

	HeapFree(App.hHeap,0,Abilities);
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boîte de dialogue						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Processus de la boîte de dialogue «««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_AbilitiesProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMEABILITY*	Abilities;

	if (uMsgId == WM_INITDIALOG)
		{
		int	i;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		Abilities = (GAMEABILITY *)lParam;

		for (i = 0; Abilities[i].uIndex != -2; i++)
			{
			if (Abilities[i].uIndex != -1)
				{
				SetDlgItemInt(hDlg,Abilities[i].uCtlID+1,Abilities[i].uValue,FALSE);
				SendDlgItemMessage(hDlg,Abilities[i].uCtlID+1,EM_LIMITTEXT,2,0);
				SendDlgItemMessage(hDlg,Abilities[i].uCtlID+2,UDM_SETRANGE32,0,(LPARAM)App.Config.bCapOverride?GAME_ABILITIES_OVERRIDE:Abilities[i].uMax);
				SendDlgItemMessage(hDlg,Abilities[i].uCtlID+2,UDM_SETPOS32,0,(LPARAM)Abilities[i].uValue);
				}
			}

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));
		SetWindowText(hDlg,Locale_GetText(TEXT_ABILITIES_TITLE));
		Dialog_CenterWindow(hDlg,App.hWnd);
		return(FALSE);
		}

	Abilities = (GAMEABILITY *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			Game_AbilitiesDraw((DRAWITEMSTRUCT *)lParam,Abilities);
			return(TRUE);

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							Game_AbilitiesSaveValues(hDlg,Abilities);
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


// «»»» Sauvegarde les valeurs ««««««««««««««««««««««««««««««««««««««««««»

void Game_AbilitiesSaveValues(HWND hDlg, GAMEABILITY *Abilities)
{
	UINT		uIndex;
	UINT		uMax;

	for (uIndex = 0; Abilities[uIndex].uIndex != -2; uIndex++)
		{
		if (Abilities[uIndex].uIndex == -1) continue;
		Abilities[uIndex].uValue = GetDlgItemInt(hDlg,Abilities[uIndex].uCtlID+1,NULL,FALSE);
		uMax = Abilities[uIndex].uMax;
		if (App.Config.bCapOverride) uMax = GAME_ABILITIES_OVERRIDE;
		if (Abilities[uIndex].uValue > uMax) Abilities[uIndex].uValue = uMax;
		}

	return;
}


// «»»» Affichage «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Game_AbilitiesDraw(DRAWITEMSTRUCT *ItemStruct, GAMEABILITY *Abilities)
{
	HFONT		hFont;
	UINT		uIndex;
	COLORREF	crText;
	int		iBkMode;

	for (uIndex = 0; Abilities[uIndex].uIndex != -2; uIndex++) if (Abilities[uIndex].uCtlID == ItemStruct->CtlID) break;
	if (Abilities[uIndex].uIndex == -2) return;

	hFont = SelectObject(ItemStruct->hDC,App.Font.hFont);
	crText = SetTextColor(ItemStruct->hDC,GetSysColor(COLOR_BTNTEXT));
	iBkMode = SetBkMode(ItemStruct->hDC,TRANSPARENT);

	if (Abilities[uIndex].uIndex == -1)
		{
		TRIVERTEX	Gradient[2];
		GRADIENT_RECT	GradientRect;
		COLORREF	crStartColor;
		COLORREF	crEndColor;

		DrawText(ItemStruct->hDC,Locale_GetText(Abilities[uIndex].uLocaleID),-1,&ItemStruct->rcItem,DT_END_ELLIPSIS|DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		crStartColor = GetSysColor(COLOR_BTNTEXT);
		crEndColor = GetSysColor(COLOR_BTNFACE);
		Gradient[0].x = ItemStruct->rcItem.left;
		Gradient[0].y = ItemStruct->rcItem.bottom-1;
		Gradient[0].Red = RGB_R(crStartColor)*255;
		Gradient[0].Green = RGB_G(crStartColor)*255;
		Gradient[0].Blue = RGB_B(crStartColor)*255;
		Gradient[0].Alpha = 0;
		Gradient[1].x = ItemStruct->rcItem.right;
		Gradient[1].y = ItemStruct->rcItem.bottom;
		Gradient[1].Red = RGB_R(crEndColor)*255;
		Gradient[1].Green = RGB_G(crEndColor)*255;
		Gradient[1].Blue = RGB_R(crEndColor)*255;
		Gradient[1].Alpha = 0;
		GradientRect.UpperLeft = 0;
		GradientRect.LowerRight = 1;
		GradientFill(ItemStruct->hDC,&Gradient[0],2,&GradientRect,1,GRADIENT_FILL_RECT_H);
		}
	else
		{
		HPEN	hPen;

		DrawText(ItemStruct->hDC,Locale_GetText(Abilities[uIndex].uLocaleID),-1,&ItemStruct->rcItem,DT_END_ELLIPSIS|DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
		hPen = CreatePen(PS_DOT,1,GetSysColor(COLOR_BTNSHADOW));
		if (hPen)
			{
			HPEN hDefPen = SelectObject(ItemStruct->hDC,hPen);
			MoveToEx(ItemStruct->hDC,ItemStruct->rcItem.left,ItemStruct->rcItem.bottom-1,NULL);
			LineTo(ItemStruct->hDC,ItemStruct->rcItem.right,ItemStruct->rcItem.bottom-1);
			SelectObject(ItemStruct->hDC,hDefPen);
			DeleteObject(hPen);
			}
		}

	SelectObject(ItemStruct->hDC,hFont);
	SetTextColor(ItemStruct->hDC,crText);
	SetBkMode(ItemStruct->hDC,iBkMode);
	return;
}

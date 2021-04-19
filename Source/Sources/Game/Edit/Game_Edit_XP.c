
//<<>-<>>---------------------------------------------------------------------()
/*
	Edition | Modification de l'expérience
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "GameEdit.h"
#include "GameLocale.h"
#include "Dialogs.h"
#include "Utils.h"
#include "Requests.h"
#include "Texts.h"
#include "Locale.h"

extern APPLICATION	App;
extern UINT		ExpTable[];


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Sélection du niveau / Modification de l'XP			  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage de la boîte de dialogue «««««««««««««««««««««««««««««««»

void Game_EditXP(HWND hWnd, XML_ATTR* pxa)
{
	GAMEEDITXP	XP;

	ZeroMemory(&XP,sizeof(GAMEEDITXP));
	XP.hwndParent = hWnd;
	XP.uValue = xml_GetThisAttrValue(pxa)?wcstol(xml_GetThisAttrValue(pxa),NULL,10):0;
	XP.uMin = 0;
	XP.uMax = Game_GetMaxXP();
	XP.uMinLevel = 1;
	XP.uMaxLevel = Game_GetMaxLevel();
	XP.uLevel = Game_GetLevelFromExp(XP.uValue);
	XP.uNextLevel = Game_GetNextLevelFromExp(XP.uValue);
	XP.pszInfo = Locale_GetText(TEXT_DIALOG_XP_INFO);

	XP.iResult = DialogBoxParam(App.hInstance,MAKEINTRESOURCE(1011),hWnd,Game_EditXPProc,(LPARAM)&XP);
	if (!XP.iResult || XP.iResult == -1)
		{
		Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
		return;
		}
	if (XP.iResult == IDCANCEL) return;

	if (!xml_SetAttrValueNumber(pxa,XP.uValue)) Request_PrintError(hWnd,Locale_GetText(TEXT_ERR_DIALOG),NULL,MB_ICONERROR);
	else InvalidateRect(hWnd,NULL,FALSE);
	return;
}


// «»»» Messages ««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

INT_PTR CALLBACK Game_EditXPProc(HWND hDlg, UINT uMsgId, WPARAM wParam, LPARAM lParam)
{
	GAMEEDITXP*	pXP;
	UINT		uValue;

	if (uMsgId == WM_INITDIALOG)
		{
		HWND	hwndCtrl;
		RECT	rcDialog;
		int	iHeight;

		SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG_PTR)lParam);
		pXP = (GAMEEDITXP *)lParam;

		iHeight = Dialog_GetInfoHeight(hDlg,100,pXP->pszInfo);
		GetWindowRect(hDlg,&rcDialog);
		SetWindowPos(hDlg,NULL,0,0,rcDialog.right-rcDialog.left,rcDialog.bottom-rcDialog.top+iHeight,SWP_NOZORDER|SWP_NOMOVE);
		GetWindowRect(GetDlgItem(hDlg,100),&rcDialog);
		SetWindowPos(GetDlgItem(hDlg,100),NULL,0,0,rcDialog.right-rcDialog.left,iHeight,SWP_NOZORDER|SWP_NOMOVE);

		Dialog_OffsetY(hDlg,200,iHeight);
		Dialog_OffsetY(hDlg,201,iHeight);
		Dialog_OffsetY(hDlg,210,iHeight);
		Dialog_OffsetY(hDlg,211,iHeight);
		Dialog_OffsetY(hDlg,220,iHeight);
		Dialog_OffsetY(hDlg,221,iHeight);
		Dialog_OffsetY(hDlg,IDOK,iHeight);
		Dialog_OffsetY(hDlg,IDCANCEL,iHeight);

		SetDlgItemInt(hDlg,200,pXP->uLevel,FALSE);
		SetDlgItemInt(hDlg,210,pXP->uValue,FALSE);
		SetDlgItemInt(hDlg,220,pXP->uNextLevel,FALSE);

		hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,(HMENU)202,App.hInstance,0);
		if (hwndCtrl)
			{
			SendMessage(hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,200),0);
			SendMessage(hwndCtrl,UDM_SETRANGE32,pXP->uMinLevel,pXP->uMaxLevel);
			SendMessage(hwndCtrl,UDM_SETPOS32,0,pXP->uLevel);
			}
		hwndCtrl = CreateWindowEx(0,UPDOWN_CLASS,NULL,WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_ARROWKEYS|UDS_SETBUDDYINT|UDS_NOTHOUSANDS,0,0,0,0,hDlg,(HMENU)212,App.hInstance,0);
		if (hwndCtrl)
			{
			SendMessage(hwndCtrl,UDM_SETBUDDY,(WPARAM)GetDlgItem(hDlg,210),0);
			SendMessage(hwndCtrl,UDM_SETRANGE32,pXP->uMin,pXP->uMax);
			SendMessage(hwndCtrl,UDM_SETPOS32,0,pXP->uValue);
			}

		hwndCtrl = CreateWindowEx(0,TRACKBAR_CLASS,NULL,WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS,0,0,0,0,hDlg,(HMENU)203,App.hInstance,0);
		if (hwndCtrl)
			{
			RECT	rcChild;

			GetClientRect(hDlg,&rcDialog);
			GetWindowRect(GetDlgItem(hDlg,GetDlgItem(hDlg,202)?202:200),&rcChild);
			MapWindowPoints(NULL,hDlg,(POINT *)&rcChild,2);
			SetWindowPos(hwndCtrl,NULL,rcChild.right,rcChild.top,rcDialog.right-rcChild.right-20,rcChild.bottom-rcChild.top,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOZORDER);
			SendMessage(hwndCtrl,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELPARAM(pXP->uMinLevel,pXP->uMaxLevel));
			SendMessage(hwndCtrl,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)pXP->uLevel);
			}

		SendDlgItemMessage(hDlg,200,EM_SETSEL,0,(LPARAM)-1);
		SetFocus(GetDlgItem(hDlg,200));

		SendDlgItemMessage(hDlg,IDOK,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_OK));
		SendDlgItemMessage(hDlg,IDCANCEL,WM_SETTEXT,0,(LPARAM)Locale_GetText(TEXT_CANCEL));

		SetWindowText(hDlg,Game_LocaleNameFromLocaleID(TEXT_CHR_EXP));
		Dialog_CenterWindow(hDlg,pXP->hwndParent);
		pXP->bInitialized = TRUE;
		return(FALSE);
		}

	pXP = (GAMEEDITXP *)GetWindowLongPtr(hDlg,GWLP_USERDATA);

	switch(uMsgId)
		{
		case WM_DRAWITEM:
			switch(wParam)
				{
				case 100:
					Dialog_DrawInfo(pXP->pszInfo,(DRAWITEMSTRUCT *)lParam,BF_BOTTOM);
					return(TRUE);
				case 201:
					Dialog_DrawLabel(Game_LocaleNameFromLocaleID(TEXT_CHR_LEVEL),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 211:
					Dialog_DrawLabel(Game_LocaleNameFromLocaleID(TEXT_CHR_EXP),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				case 221:
					Dialog_DrawLabel(Game_LocaleNameFromLocaleID(TEXT_CHR_NEXTLEVEL),(DRAWITEMSTRUCT *)lParam,NULL,DT_RIGHT);
					return(TRUE);
				}
			break;

		case WM_HSCROLL:
			if (!pXP->bInitialized) break;
			uValue = SendDlgItemMessage(hDlg,203,TBM_GETPOS,0,0);
			if (uValue != pXP->uLevel)
				{
				pXP->uLevel = uValue;
				pXP->uValue = Game_GetExpFromLevel(pXP->uLevel);
				pXP->uNextLevel = Game_GetNextLevelFromExp(pXP->uValue);
				SetDlgItemInt(hDlg,200,pXP->uLevel,FALSE);
				SendDlgItemMessage(hDlg,202,UDM_SETPOS32,0,pXP->uLevel);
				SetDlgItemInt(hDlg,210,pXP->uValue,FALSE);
				SetDlgItemInt(hDlg,220,pXP->uNextLevel,FALSE);
				}
			break;

		case WM_COMMAND:
			switch(HIWORD(wParam))
				{
				case EN_CHANGE:
					switch(LOWORD(wParam))
						{
						case 200:
							if (!pXP->bInitialized) break;
							if (Dialog_GetUIntValue(hDlg,200,pXP->uMinLevel,pXP->uMaxLevel,&uValue))
								{
								if (pXP->uLevel != uValue)
									{
									pXP->uLevel = uValue;
									pXP->uValue = Game_GetExpFromLevel(pXP->uLevel);
									pXP->uNextLevel = Game_GetNextLevelFromExp(pXP->uValue);
									SendDlgItemMessage(hDlg,202,UDM_SETPOS32,0,pXP->uLevel);
									SendDlgItemMessage(hDlg,203,TBM_SETPOS,(WPARAM)TRUE,pXP->uLevel);
									SetDlgItemInt(hDlg,210,pXP->uValue,FALSE);
									SetDlgItemInt(hDlg,220,pXP->uNextLevel,FALSE);
									}
								}
							return(TRUE);
						case 210:
							if (!pXP->bInitialized) break;
							if (Dialog_GetUIntValue(hDlg,210,pXP->uMin,pXP->uMax,&uValue))
								{
								if (pXP->uValue != uValue)
									{
									pXP->uValue = uValue;
									pXP->uLevel = Game_GetLevelFromExp(pXP->uValue);
									pXP->uNextLevel = Game_GetNextLevelFromExp(pXP->uValue);
									SendDlgItemMessage(hDlg,212,UDM_SETPOS32,0,pXP->uValue);
									SendDlgItemMessage(hDlg,203,TBM_SETPOS,(WPARAM)TRUE,pXP->uLevel);
									SetDlgItemInt(hDlg,200,pXP->uLevel,FALSE);
									SetDlgItemInt(hDlg,220,pXP->uNextLevel,FALSE);
									}
								}
							return(TRUE);
						}
					break;

				case BN_CLICKED:
					switch(LOWORD(wParam))
						{
						case IDOK:
							if (!pXP->bInitialized) break;
							if (!Dialog_GetUIntValue(hDlg,200,pXP->uMinLevel,pXP->uMaxLevel,&uValue)) return(TRUE);
							if (!Dialog_GetUIntValue(hDlg,210,pXP->uMin,pXP->uMax,&uValue)) return(TRUE);
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

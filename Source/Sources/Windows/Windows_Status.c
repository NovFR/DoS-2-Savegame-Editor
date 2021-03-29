
//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtre de statut
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

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions relatives à la fenêtre					  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Création de la fenêtre de status ««««««««««««««««««««««««««««««««»

int Status_CreateWindow()
{
	App.hwndStatus = CreateStatusWindow(WS_CHILD|WS_VISIBLE,NULL,App.hWnd,CTLID_STATUSWND);
	if (!App.hwndStatus) return(0);

	Status_UpdateParts();
	return(1);
}


// «»»» Modifie la taille des parties de la barre de statut «««««««««««««»

void Status_UpdateParts()
{
	RECT	rcClient;
	int	iStatusParts[2] = {0,0};

	GetClientRect(App.hWnd,&rcClient);
	iStatusParts[1] = rcClient.right-GetSystemMetrics(SM_CXSMICON)-4;
	iStatusParts[0] = iStatusParts[1]-128;
	SendMessage(App.hwndStatus,SB_SETPARTS,(WPARAM)2,(LPARAM)&iStatusParts);
	SendMessage(App.hwndStatus,SB_SETTEXT,(WPARAM)STATUS_TEXT|SBT_OWNERDRAW,(LPARAM)NULL);
	SendMessage(App.hwndStatus,SB_SETTEXT,(WPARAM)STATUS_PROGRESS|SBT_OWNERDRAW,(LPARAM)NULL);
	return;
}


// «»»» Modifie la taille des parties de la barre de statut «««««««««««««»

void Status_Resize(int iWidth)
{
	SetWindowPos(App.hwndStatus,NULL,0,0,iWidth,0,SWP_NOMOVE|SWP_NOZORDER);
	Status_UpdateParts();
	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage du texte de statut ««««««««««««««««««««««««««««««««««««»

void Status_DrawStatusText(DRAWITEMSTRUCT *pDraw)
{
	EnterCriticalSection(&App.CriticalSection);

	FillRect(pDraw->hDC,&pDraw->rcItem,GetSysColorBrush(COLOR_BTNFACE));
	if (App.pszStatusText)
		{
		RECT		rcDrawArea;
		COLORREF	crDefText;
		int		crDefBack;

		crDefText = SetTextColor(pDraw->hDC,GetSysColor(COLOR_BTNTEXT));
		crDefBack = SetBkMode(pDraw->hDC,TRANSPARENT);

		CopyRect(&rcDrawArea,&pDraw->rcItem);
		rcDrawArea.left += 4;
		rcDrawArea.right -= 4;
		DrawText(pDraw->hDC,App.pszStatusText,-1,&rcDrawArea,DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_PATH_ELLIPSIS);

		SetBkMode(pDraw->hDC,crDefBack);
		SetTextColor(pDraw->hDC,crDefText);
		}

	LeaveCriticalSection(&App.CriticalSection);
	return;
}

// --- Modifie le texte de statut ---

void Status_SetText(WCHAR *pszText, ...)
{
	va_list		vl;

	EnterCriticalSection(&App.CriticalSection);

	if (App.pszStatusText) LocalFree(App.pszStatusText);
	App.pszStatusText = NULL;

	if (pszText)
		{
		va_start(vl,pszText);
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING,pszText,0,0,(WCHAR *)&App.pszStatusText,1,&vl);
		va_end(vl);
		}

	LeaveCriticalSection(&App.CriticalSection);
	SendMessage(App.hwndStatus,SB_SETTEXT,(WPARAM)STATUS_TEXT|SBT_OWNERDRAW,(LPARAM)NULL);
	return;
}


// «»»» Affichage de la barre de progression ««««««««««««««««««««««««««««»

void Status_DrawProgressBar(DRAWITEMSTRUCT *pDraw)
{
	RECT	rcArea;
	int	iWidth;

	CopyRect(&rcArea,&pDraw->rcItem);

	iWidth = (int)(((float)rcArea.right-(float)rcArea.left)*((float)App.uProgression/100.0f));
	if (iWidth > rcArea.right-rcArea.left) iWidth = rcArea.right-rcArea.left;
	if (iWidth > 0)
		{
		rcArea.right = rcArea.left+iWidth;
		if (App.hThemeProgress)
			{
			DrawThemeBackground(App.hThemeProgress,pDraw->hDC,PP_BAR,0,&rcArea,NULL);
			DrawThemeBackground(App.hThemeProgress,pDraw->hDC,PP_FILL,PBFS_NORMAL,&rcArea,NULL);
			}
		else FillRect(pDraw->hDC,&rcArea,GetSysColorBrush(COLOR_HIGHLIGHT));
		}
	return;
}

//--- Modifie la progression ---

void Status_UpdateProgress(UINT uCurrent, UINT uMax)
{
	if (uCurrent == -1 && uMax == -1) App.uProgression = 0;
	else App.uProgression = (UINT)(((float)uCurrent/(float)uMax)*100.0f);
	if (App.uProgression < 0) App.uProgression = 0;
	if (App.uProgression > 100) App.uProgression = 100;

	// Prevents flood
	if (App.uProgression != 0 && App.uProgression != 100 && App.dwProgressionTime != 0)
		{
		DWORD dwTime = GetTickCount();
		if (dwTime-App.dwProgressionTime < STATUS_REFRESH_MS) return;
		}
	App.dwProgressionTime = GetTickCount();

	SendMessage(App.hwndStatus,SB_SETTEXT,(WPARAM)STATUS_PROGRESS|SBT_OWNERDRAW,(LPARAM)NULL);
	InvalidateRect(App.hwndStatus,NULL,FALSE);
	return;
}

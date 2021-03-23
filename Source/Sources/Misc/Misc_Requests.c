
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines d'affichage des requêtes
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"
#include "Requests.h"
#include "Texts.h"
#include "Taskbar.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Affichage des erreurs						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Affichage d'une erreur système avec arguments «««««««««««««««««««»

int Request_MessageBoxEx(HWND hWnd, WCHAR *pszWindowText, WCHAR *pszWindowTitle, UINT uFlags, ...)
{
	WCHAR*		pszErrorMsg;
	va_list		vl;
	int		iResult;

	va_start(vl,uFlags);

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING,pszWindowText,0,0,(WCHAR *)&pszErrorMsg,1,&vl))
		{
		va_end(vl);
		return(0);
		}

	if (uFlags&(MB_ICONHAND|MB_ICONERROR))
		{
		Taskbar_SetProgressValue(1,1);
		Taskbar_SetProgressState(TBPF_ERROR);
		}

	iResult = MessageBox(hWnd,pszErrorMsg,pszWindowTitle,uFlags);

	if (uFlags&(MB_ICONHAND|MB_ICONERROR)) Taskbar_SetProgressState(TBPF_NOPROGRESS);

	LocalFree(pszErrorMsg);
	va_end(vl);
	return(iResult);
}


// «»»» Affichage d'une erreur système avec arguments «««««««««««««««««««»

void Request_PrintErrorEx(HWND hWnd, WCHAR *pszWindowText, WCHAR *pszWindowTitle, UINT uFlags, ...)
{
	WCHAR*		pszErrorMsg;
	va_list		vl;

	va_start(vl,uFlags);

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING,pszWindowText,0,0,(WCHAR *)&pszErrorMsg,1,&vl))
		{
		va_end(vl);
		return;
		}

	Request_PrintError(hWnd,pszErrorMsg,pszWindowTitle,uFlags);
	LocalFree(pszErrorMsg);
	va_end(vl);
	return;
}


// «»»» Affichage d'une erreur système ««««««««««««««««««««««««««««««««««»

void Request_PrintError(HWND hWnd, WCHAR *pszWindowText, WCHAR *pszWindowTitle, UINT uFlags)
{
	WCHAR*		pszErrorMsg;
	WCHAR*		pszFinalMsg;

	if (GetLastError() == ERROR_SUCCESS)
		goto Simple;

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,GetLastError(),LOCALE_USER_DEFAULT,(WCHAR *)&pszErrorMsg,1,NULL))
		goto Simple;

	pszFinalMsg = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,(wcslen(pszWindowText)+wcslen(szLF)*2+wcslen(pszErrorMsg)+1)*sizeof(WCHAR));
	if (!pszFinalMsg)
		goto Failed;

	Taskbar_SetProgressValue(1,1);
	Taskbar_SetProgressState(TBPF_ERROR);
	wcscpy(pszFinalMsg,pszWindowText);
	wcscat(pszFinalMsg,szLF);
	wcscat(pszFinalMsg,szLF);
	wcscat(pszFinalMsg,pszErrorMsg);
	MessageBox(hWnd,pszFinalMsg,pszWindowTitle,MB_OK|uFlags);
	Taskbar_SetProgressState(TBPF_NOPROGRESS);

	HeapFree(App.hHeap,0,pszFinalMsg);
	LocalFree(pszErrorMsg);
	return;

//--- Affichage du message brut ---

Failed:	LocalFree(pszErrorMsg);
Simple:	Taskbar_SetProgressValue(1,1);
	Taskbar_SetProgressState(TBPF_ERROR);
	MessageBox(hWnd,pszWindowText,pszWindowTitle,MB_OK|uFlags);
	Taskbar_SetProgressState(TBPF_NOPROGRESS);
	return;
}

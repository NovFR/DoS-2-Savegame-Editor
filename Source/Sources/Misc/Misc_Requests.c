
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

	iResult = MessageBox(hWnd,pszErrorMsg,pszWindowTitle,uFlags);

	LocalFree(pszErrorMsg);
	va_end(vl);
	return(iResult);
}


// «»»» Affichage d'une erreur système avec arguments «««««««««««««««««««»

int Request_PrintErrorEx(HWND hWnd, WCHAR *pszWindowText, WCHAR *pszWindowTitle, UINT uFlags, ...)
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

	iResult = Request_PrintError(hWnd,pszErrorMsg,pszWindowTitle,uFlags);
	LocalFree(pszErrorMsg);
	va_end(vl);
	return(iResult);
}


// «»»» Affichage d'une erreur système ««««««««««««««««««««««««««««««««««»

int Request_PrintError(HWND hWnd, WCHAR *pszWindowText, WCHAR *pszWindowTitle, UINT uFlags)
{
	TASKDIALOGCONFIG	dialog;
	WCHAR*			pszErrorMsg;
	int			iResult;

	ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
	dialog.cbSize = sizeof(TASKDIALOGCONFIG);
	dialog.hwndParent = hWnd;
	dialog.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
	dialog.pszWindowTitle = pszWindowTitle?pszWindowTitle:szTitle;
	dialog.pszContent = pszWindowText;

	switch(uFlags&0x0000000F)
		{
		case MB_YESNOCANCEL:
			dialog.dwCommonButtons = TDCBF_YES_BUTTON|TDCBF_NO_BUTTON|TDCBF_CANCEL_BUTTON;
			break;
		case MB_YESNO:
			dialog.dwCommonButtons = TDCBF_YES_BUTTON|TDCBF_NO_BUTTON;
			break;
		case MB_OKCANCEL:
			dialog.dwCommonButtons = TDCBF_OK_BUTTON|TDCBF_CANCEL_BUTTON;
			break;
		default:dialog.dwCommonButtons = TDCBF_OK_BUTTON;
		}

	switch(uFlags&0x000000F0)
		{
		case MB_ICONINFORMATION:
		case MB_ICONQUESTION:
			dialog.pszMainIcon = TD_INFORMATION_ICON;
			break;
		case MB_ICONEXCLAMATION:
			dialog.pszMainIcon = TD_WARNING_ICON;
			break;
		default:dialog.pszMainIcon = TD_ERROR_ICON;
		}

	pszErrorMsg = NULL;
	if (GetLastError() != ERROR_SUCCESS) FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,GetLastError(),LOCALE_USER_DEFAULT,(WCHAR *)&pszErrorMsg,1,NULL);

	dialog.pszExpandedInformation = pszErrorMsg;
	dialog.pszExpandedControlText = Locale_GetText(TEXT_ERR_HIDEERROR);
	dialog.pszCollapsedControlText = Locale_GetText(TEXT_ERR_SHOWERROR);

	iResult = 0;
	TaskDialogIndirect(&dialog,&iResult,NULL,NULL);
	if (pszErrorMsg) LocalFree(pszErrorMsg);
	return(iResult);
}


//<<>-<>>---------------------------------------------------------------------()
/*
	A propos...
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Version.h"
#include "Application.h"
#include "About.h"
#include "Locale.h"
#include "Dialogs.h"
#include "Utils.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Texte d'information						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void About_Display(HWND hWnd)
{
	TASKDIALOGCONFIG	dialog;
	WCHAR*			pszAbout;
	WCHAR*			pszText;
	WCHAR			szYear[] = L"0000";
	WCHAR			szMonth[] = L"00";
	WCHAR			szDay[] = L"00";
	WCHAR			szHour[] = L"00";
	WCHAR			szMinutes[] = L"00";
	DWORD_PTR		vl[6];
	int			i;

	static WCHAR*		pszMonths[] = { L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec", NULL };

	pszText = TOWIDECHAR(VERSION_TIME);
	szHour[0] = pszText[0];
	szHour[1] = pszText[1];
	szMinutes[0] = pszText[3];
	szMinutes[1] = pszText[4];

	pszText = TOWIDECHAR(VERSION_DATE);
	szYear[0] = pszText[7];
	szYear[1] = pszText[8];
	szYear[2] = pszText[9];
	szYear[3] = pszText[10];
	szDay[0] = pszText[4] > L'0'?pszText[4]:L'0';
	szDay[1] = pszText[5];

	for (i = 0; pszMonths[i] != NULL; i++)
		{
		if (wcsncmp(pszMonths[i],pszText,3)) continue;
		Misc_Printf(szMonth,3,L"%02u",i+1);
		break;
		}

	vl[0] = (DWORD_PTR)TOWIDECHAR(VERSION_STRING_NZ);
	vl[1] = (DWORD_PTR)szDay;
	vl[2] = (DWORD_PTR)szMonth;
	vl[3] = (DWORD_PTR)szYear;
	vl[4] = (DWORD_PTR)szHour;
	vl[5] = (DWORD_PTR)szMinutes;

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,Locale_GetText(TEXT_ABOUT),0,0,(WCHAR *)&pszAbout,1,(va_list *)&vl))
		{
		ZeroMemory(&dialog,sizeof(TASKDIALOGCONFIG));
		dialog.cbSize = sizeof(TASKDIALOGCONFIG);
		dialog.hwndParent = hWnd;
		dialog.dwFlags = TDF_USE_HICON_MAIN|TDF_ENABLE_HYPERLINKS|TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW|TDF_SIZE_TO_CONTENT;
		dialog.dwCommonButtons = TDCBF_CLOSE_BUTTON;
		dialog.pszWindowTitle = Locale_GetText(IDM_ABOUT);
		dialog.hMainIcon = App.hIcons[APP_ICON_APP_LARGE];
		dialog.pszContent = pszAbout;
		dialog.nDefaultButton = IDCLOSE;
		dialog.pfCallback = Dialog_TaskDialogProc;
		TaskDialogIndirect(&dialog,NULL,NULL,NULL);
		LocalFree(pszAbout);
		}

	return;
}

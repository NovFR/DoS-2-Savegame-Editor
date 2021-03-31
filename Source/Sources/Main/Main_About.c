
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
	DWORD_PTR		vl[4];

	vl[0] = VERSION_MAJOR;
	vl[1] = REVISION_MAJOR;
	vl[2] = VERSION_MINOR;
	vl[3] = REVISION_MINOR;

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

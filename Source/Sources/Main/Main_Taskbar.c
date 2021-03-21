
//<<>-<>>---------------------------------------------------------------------()
/*
	Barre des tâches
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Taskbar.h"

extern APPLICATION		App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Initialisations							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation du message de création ««««««««««««««««««««««««««««»

int Taskbar_Prepare()
{
	App.WM_TASKBARBUTTONCREATED = RegisterWindowMessage(L"TaskbarButtonCreated");
	if (!App.WM_TASKBARBUTTONCREATED) return(0);
	return(1);
}


// «»»» Initialise l'interface ««««««««««««««««««««««««««««««««««««««««««»

int Taskbar_Initialize()
{
	HRESULT		hr;

	hr = CoCreateInstance(&CLSID_TaskbarList,NULL,CLSCTX_INPROC,&IID_ITaskbarList3,(void *)&App.pTaskbar);
	if (FAILED(hr)) return(0);

	hr = App.pTaskbar->lpVtbl->HrInit(App.pTaskbar);
	if (FAILED(hr))
		{
		App.pTaskbar->lpVtbl->Release(App.pTaskbar);
		App.pTaskbar = NULL;
		return(0);
		}

	return(1);
}


// «»»» Libère l'interface ««««««««««««««««««««««««««««««««««««««««««««««»

void Taskbar_Reset()
{
	if (App.pTaskbar)
		{
		App.pTaskbar->lpVtbl->Release(App.pTaskbar);
		App.pTaskbar = NULL;
		}

	return;
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Modifie l'état du bouton ««««««««««««««««««««««««««««««««««««««««»

void Taskbar_SetProgressState(UINT uFlags)
{
	if (!App.pTaskbar) return;
	App.pTaskbar->lpVtbl->SetProgressState(App.pTaskbar,App.hWnd,uFlags);
	return;
}


// «»»» Modifie la valeur de la barre de progression ««««««««««««««««««««»

void Taskbar_SetProgressValue(ULONGLONG uCurrent, ULONGLONG uTotal)
{
	if (!App.pTaskbar) return;
	App.pTaskbar->lpVtbl->SetProgressValue(App.pTaskbar,App.hWnd,uCurrent,uTotal);
	return;
}

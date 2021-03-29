
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de démarrage
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Startup.h"
#include "Texts.h"
#include "Menus.h"
#include "Game.h"
#include "LastFiles.h"
#include "Requests.h"
#include "Debug.h"

extern APPLICATION		App;
extern APPICON			AppIcons[];
extern SHORTCUT			Shortcuts[];
extern CUSTOMMENUTEMPLATE	MainMenu;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Boucle principale						  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int APIENTRY WinMain(HINSTANCE hWinCInstance, HINSTANCE hWinPInstance, LPSTR CmdLine, int CmdShow)
{
	MSG	msg;

	ZeroMemory(&msg,sizeof(MSG));
	App.hInstance = hWinCInstance;
	App.hHeap = GetProcessHeap();

	SetUnhandledExceptionFilter(Debug_ExceptionHandler);
	CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
	InitializeCriticalSection(&App.CriticalSection);

	if (!Config_Defaults(&App.Config)) goto Done;
	if (!Locale_Load(NULL,szLangPath,App.Config.pszLocaleName,LOCALE_TYPE_APPLICATION,(void **)&App.pLocaleTexts,&App.pszLocaleName)) goto Done;
	if (!Game_LoadDataFile(NULL,szItemsDataPath,DATA_TYPE_ITEMS,&App.Game.nodeDataItems)) goto Done;
	if (!Initialise_Accelerators()) goto Done;
	if (!Initialise_Icons()) goto Done;
	if (!Initialise_WindowsClasses()) goto Done;
	if (!Initialise_Window()) goto Done;

	while (GetMessage(&msg,NULL,0,0))
		{
		if (IsWindow(App.xmlTree.hWnd) && GetActiveWindow() == App.xmlTree.hWnd)
			{
			if (IsDialogMessage(App.hWnd,&msg)) continue;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
			}
		if (App.hShortcuts && TranslateAccelerator(App.hWnd,App.hShortcuts,&msg)) continue;
		if (IsDialogMessage(App.hWnd,&msg)) continue;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

Done:	if (App.hShortcuts) DestroyAcceleratorTable(App.hShortcuts);
	Reset_Icons();
	Game_UnloadDataFile(DATA_TYPE_ITEMS,&App.Game.nodeDataItems);
	Locale_Unload(LOCALE_TYPE_APPLICATION,(void **)&App.pLocaleTexts,&App.pszLocaleName);
	LastFiles_ReleaseAll();
	Config_Release(&App.Config);

	CoUninitialize();
	DeleteCriticalSection(&App.CriticalSection);
	return(msg.wParam);
}


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Initialisation des classes de fenêtres ««««««««««««««««««««««««««»

int Initialise_WindowsClasses()
{
	INITCOMMONCONTROLSEX	Controls;
	WNDCLASSEX		wndClass;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = (WNDPROC)Window_Proc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = App.hInstance;
	wndClass.hIcon = LoadImage(App.hInstance,MAKEINTRESOURCE(1),IMAGE_ICON,16,16,0);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szWindowClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (!RegisterClassEx(&wndClass))
		{
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_WINDOW_CLASS),NULL,MB_ICONHAND);
		return(0);
		}

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_DBLCLKS|CS_SAVEBITS|CS_BYTEALIGNWINDOW;
	wndClass.lpfnWndProc = DefDlgProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = DLGWINDOWEXTRA;
	wndClass.hInstance = App.hInstance;
	wndClass.hIcon = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szDialogClass;
	wndClass.hIconSm = wndClass.hIcon;
	if (!RegisterClassEx(&wndClass))
		{
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_WINDOW_CLASS),NULL,MB_ICONHAND);
		return(0);
		}

	Controls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	Controls.dwICC = ICC_WIN95_CLASSES|ICC_STANDARD_CLASSES;
	if (!InitCommonControlsEx(&Controls))
		{
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_COMCTRL),NULL,MB_ICONHAND);
		return(0);
		}
	return(1);
}


// «»»» Initialisation de la fenêtre des messages «««««««««««««««««««««««»

int Initialise_Window()
{
	HWND	hWnd;
	int	X,Y;

	App.hMenu = Menu_Create(&MainMenu);
	if (!App.hMenu)
		{
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_WINDOW_CREATE),NULL,MB_ICONHAND);
		return(0);
		}

	X = CW_USEDEFAULT;
	Y = CW_USEDEFAULT;

	if (X == CW_USEDEFAULT || Y == CW_USEDEFAULT)
		{
		HMONITOR	hMonitor;
		MONITORINFO	Info;

		hMonitor = MonitorFromWindow(NULL,MONITOR_DEFAULTTOPRIMARY);
		Info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMonitor,&Info);
		if (X == CW_USEDEFAULT) X = ((Info.rcMonitor.right-Info.rcMonitor.left)-MAIN_WINDOW_WIDTH)/2;
		if (Y == CW_USEDEFAULT) Y = ((Info.rcMonitor.bottom-Info.rcMonitor.top)-MAIN_WINDOW_HEIGHT)/2;
		if (X < 0) X = 0;
		if (Y < 0) Y = 0;
		}

	hWnd = CreateWindowEx(0,szWindowClass,szTitle,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_SIZEBOX,X,Y,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT,NULL,App.hMenu,App.hInstance,(void *)WINDOW_MAIN);
	if (!hWnd)
		{
		// NO DIALOG HERE because WM_QUIT has been posted
		Menu_Release(App.hMenu,&MainMenu);
		return(0);
		}

	ShowWindow(hWnd,SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	return(1);
}


// «»»» Initialisation/Supprime les icônes ««««««««««««««««««««««««««««««»

//--- Initialisation ---

int Initialise_Icons()
{
	SHSTOCKICONINFO	info;
	int		i;

	//--- App Icons
	for (i = 0; i != APP_MAX_ICONS-1; i++)
		{
		#if _DEBUG
		if (AppIcons[i].id == 0)
			{
			Request_MessageBoxEx(NULL,L"Missing ICON definition for ICON(%1!i!)",L"Internal Error",MB_ICONHAND|MB_OK,i+1);
			return(0);
			}
		#endif
		App.hIcons[AppIcons[i].id] = LoadImage(App.hInstance,MAKEINTRESOURCE(AppIcons[i].res),IMAGE_ICON,AppIcons[i].width,AppIcons[i].height,LR_DEFAULTCOLOR);
		if (App.hIcons[AppIcons[i].id]) continue;
		return(0);
		}

	//--- System Icon
	info.cbSize = sizeof(SHSTOCKICONINFO);
	info.hIcon = NULL;
	SHGetStockIconInfo(SIID_INFO,SHGSI_ICON|SHGSI_SHELLICONSIZE,&info);
	App.hIconInfo = info.hIcon;
	return(1);
}

//--- Suppression ---

void Reset_Icons()
{
	int	i;

	for (i = 0; i != APP_MAX_ICONS-1; i++)
		{
		if (!App.hIcons[AppIcons[i].id]) continue;
		DestroyIcon(App.hIcons[AppIcons[i].id]);
		}

	if (App.hIconInfo) DestroyIcon(App.hIconInfo);
	return;
}


// «»»» Initialisation les raccourcis «««««««««««««««««««««««««««««««««««»

int Initialise_Accelerators()
{
	ACCEL*		Accelerators;
	WCHAR*		pszSpace;
	WCHAR*		pszShortcut;
	WCHAR		szTemp[16];
	WORD		wKey;
	int		iCount;
	int		i,j;

	for (iCount = 0; Shortcuts[iCount].acc != 0; iCount++);
	Accelerators = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,iCount*sizeof(ACCEL));
	if (!Accelerators)
		{
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_SHORTCUTS),NULL,MB_ICONHAND);
		return(0);
		}

	for (iCount = 0; Shortcuts[iCount].acc != 0; iCount++)
		{
		Accelerators[iCount].fVirt = FVIRTKEY;
		Accelerators[iCount].key = wKey = 0xFFFF;
		Accelerators[iCount].cmd = Shortcuts[iCount].cmd;
		// --- Test si une touche de fonction est spécifiée ---
		pszShortcut = Locale_GetText(Shortcuts[iCount].acc);
		for (j = 0; j != 3; j++)
			{
			pszSpace = wcschr(pszShortcut,L'+');
			if (pszSpace)
				{
				for (i = 0; *pszShortcut != L' ' && i < 16; i++) szTemp[i] = *pszShortcut++;
				szTemp[i] = 0;
				if (!wcscmp(szTemp,Locale_GetText(KEY_SHIFT))) Accelerators[iCount].fVirt |= FSHIFT;
				else if (!wcscmp(szTemp,Locale_GetText(KEY_CTRL))) Accelerators[iCount].fVirt |= FCONTROL;
				else if (!wcscmp(szTemp,Locale_GetText(KEY_ALT))) Accelerators[iCount].fVirt |= FALT;
				pszShortcut = pszSpace+1;
				while (*pszShortcut++ == L' ');
				pszShortcut--;
				}
			}
		// --- Traduit la touche ---
		if (wcslen(pszShortcut) > 1)
			{
			if (!wcscmp(pszShortcut,Locale_GetText(KEY_F1))) wKey = VK_F1;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F2))) wKey = VK_F2;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F3))) wKey = VK_F3;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F4))) wKey = VK_F4;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F5))) wKey = VK_F5;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F6))) wKey = VK_F6;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F7))) wKey = VK_F7;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F8))) wKey = VK_F8;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F9))) wKey = VK_F9;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F10))) wKey = VK_F10;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F11))) wKey = VK_F11;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_F12))) wKey = VK_F12;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_RETURN))) wKey = VK_RETURN;
			else if (!wcscmp(pszShortcut,Locale_GetText(KEY_DEL))) wKey = VK_DELETE;
			}
		else
			{
			wKey = VkKeyScanEx(*pszShortcut,GetKeyboardLayout(0));
			if (wKey != 0xFFFF) wKey &= 0x00FF;
			}
		// --- Vérifie que la touche est valide ---
		if (wKey == 0xFFFF)
			{
			// In case of error, shortcuts are just deactivated...
			// Request_MessageBoxEx(NULL,Locale_GetText(TEXT_ERR_INVALIDSHORTCUT),NULL,MB_ICONWARNING,Locale_GetText(Shortcuts[iCount].acc));
			HeapFree(App.hHeap,0,Accelerators);
			return(1);
			}
		Accelerators[iCount].key = wKey;
		}

	App.hShortcuts = CreateAcceleratorTable(Accelerators,iCount);
	if (!App.hShortcuts)
		{
		HeapFree(App.hHeap,0,Accelerators);
		Request_PrintError(NULL,Locale_GetText(TEXT_ERR_SHORTCUTS),NULL,MB_ICONHAND);
		return(0);
		}

	HeapFree(App.hHeap,0,Accelerators);
	return(1);
}


//<<>-<>>---------------------------------------------------------------------()
/*
	Fenêtres
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _APPWINDOWS_INCLUDE
#define _APPWINDOWS_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "XML.h"

#define STATUS_REFRESH_MS		25

#define MAIN_WINDOW_WIDTH		1024
#define MAIN_WINDOW_HEIGHT		942

#define MAIN_ATTR_BTN_WIDTH		20

enum {
	WINDOW_MAIN			= 0,
	WINDOW_TREE
};

enum {
	WM_MESSAGEBOX			= WM_APP,
	WM_UPDATEPROGRESS
};

enum {
	STATUS_TEXT			= 0,
	STATUS_PROGRESS
};

enum {
	CTLID_CHARACTERS_LIST		= 10000,
	CTLID_ATTRIBUTES,
	CTLID_POINTS			= CTLID_ATTRIBUTES+7,
	CTLID_INVENTORY			= CTLID_POINTS+5,
	CTLID_LIFE,
	CTLID_PHYSICAL,
	CTLID_MAGICAL,
	CTLID_EXPERIENCE,
	CTLID_NEXTLEVEL,
	CTLID_MENU,
	CTLID_ABILITIES,
	CTLID_TAGS,
	CTLID_TALENTS,
	CTLID_SKILLS,
	CTLID_STATS,
	CTLID_INVENTORYNAME,
	CTLID_RUNES,
	CTLID_EDIT,
	CTLID_TREEVIEW,
	CTLID_STATUSWND,
	CTLID_TREESAVE,
	CTLID_TREEHELP,
	CTLID_TREECLOSE
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Main ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

LRESULT CALLBACK	Window_Proc(HWND,UINT,WPARAM,LPARAM);
LRESULT			Window_ProcessMessages(HWND,UINT,WPARAM,LPARAM);

LRESULT			Window_Create(HWND);
void			Window_MeasureItems(HWND,UINT,MEASUREITEMSTRUCT *);
void			Window_DrawItems(DRAWITEMSTRUCT *);
void			Window_Notify(HWND,UINT,NMHDR *);
void			Window_Command(HWND,UINT,UINT,HWND);

// «»»» Status ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int			Status_CreateWindow(void);
void			Status_UpdateParts(void);

void			Status_DrawStatusText(DRAWITEMSTRUCT *);
void			Status_SetText(WCHAR *,...);

void			Status_DrawProgressBar(DRAWITEMSTRUCT *);
void			Status_UpdateProgress(UINT,UINT);

#endif

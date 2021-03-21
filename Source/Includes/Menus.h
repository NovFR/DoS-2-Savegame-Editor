
//<<>-<>>---------------------------------------------------------------------()
/*
	Définitions des menus
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

enum {
	MENU_STOP		= 0,
	MENU_POPUP,
	MENU_SUBPOPUP,
	MENU_ITEM,
	MENU_SEPARATOR,
	MENU_BLANK,
};

#define MENU_FLAG_CHECKBOX	0x00000001
#define MENU_FLAG_SEPARATOR	0x00000004
#define MENU_FLAG_CAPTION	0x00000008
#define MENU_FLAG_NOICON	0x00000010
#define MENU_FLAG_COLORICON	0x00000020
#define MENU_FLAG_FILENAME	0x00000040

#define MENU_IMAGEWIDTH		16
#define MENU_IMAGEHEIGHT	16

#define MENU_RIGHTPADDING	24
#define MENU_BLANKHEIGHT	8
#define MENU_FILENAMEMAXWIDTH	600


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct CUSTOMMENU {
	HMENU			hMenu;
} CUSTOMMENU;

typedef struct CUSTOMMENUTEMPLATE {
	UINT			uType;
	UINT			uId;
	UINT			uFlags;
	UINT			uKeyId;
	UINT			uIconId;
	UINT			uSpecialFlags;
	COLORREF*		crColor;
	//--- Set by functions
	BOOL			bIsBlank;
	HICON			hIcon;
	HBITMAP			hImage;
	WCHAR*			pszText;
	ULONG			uTextLen;
	WCHAR*			pszKey;
} CUSTOMMENUTEMPLATE;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

HMENU			Menu_Create(CUSTOMMENUTEMPLATE *);
BOOL			Menu_Create_Append(HMENU,CUSTOMMENUTEMPLATE **);
BOOL			Menu_Create_AppendMenu(HMENU,CUSTOMMENUTEMPLATE **);
BOOL			Menu_Create_AppendSubMenu(HMENU,CUSTOMMENUTEMPLATE **);
BOOL			Menu_Create_AppendItem(HMENU,CUSTOMMENUTEMPLATE **);
BOOL			Menu_Create_AppendSeparator(HMENU,CUSTOMMENUTEMPLATE **);
BOOL			Menu_Create_AppendBlank(HMENU,CUSTOMMENUTEMPLATE **);

void			Menu_Release(HMENU,CUSTOMMENUTEMPLATE *);
void			Menu_Release_ObjectsLoop(CUSTOMMENUTEMPLATE **);
void			Menu_Release_Objects(CUSTOMMENUTEMPLATE **);

void			Menu_MeasureItems(HWND,UINT,MEASUREITEMSTRUCT *);
void			Menu_DrawItems(DRAWITEMSTRUCT *);

void			Menu_SetFlag(HMENU,UINT,BOOL);
BOOL			Menu_GetFlag(HMENU,UINT);

#endif

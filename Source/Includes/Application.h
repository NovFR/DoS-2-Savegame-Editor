
//<<>-<>>---------------------------------------------------------------------()
/*
	Application
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _APPLICATION_INCLUDE
#define _APPLICATION_INCLUDE

#include "_Global.h"
#include "Locale.h"
#include "Lists.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define FILE_HEADER_CONFIG		0xED0000CF
#define FILE_HEADER_LASTFILES		0xED00001F

#define RGB_R(a)			((a)&0x000000FF)
#define RGB_G(a)			(((a)&0x0000FF00)>>8)
#define RGB_B(a)			(((a)&0x00FF0000)>>16)

typedef struct FILEHEADER {
	UINT				head;
	UINT				version;
	UINT				size;
} FILEHEADER;

#include "Config.h"
#include "Font.h"
#include "AppWindows.h"
#include "XMLTree.h"

enum {
	APP_ICON_APPLICATION = 1,
	APP_ICON_MAN,
	APP_ICON_WOMAN,
	APP_ICON_PLAYER,
	APP_ICON_CHECKED,
	APP_ICON_UNCHECKED,
	APP_ICON_UP,
	APP_ICON_DOWN,
	APP_ICON_WEAPON,
	APP_ICON_ARMOR,
	APP_ICON_ACCESSORY,
	APP_ICON_BACKGROUND,
	APP_ICON_FRAME,
	APP_ICON_EMPTY,
	APP_ICON_EDIT,
	APP_ICON_BACKPACK,
	APP_ICON_BACK,
	APP_ICON_MODIFY,
	APP_ICON_REMOVE,
	APP_ICON_HIDDEN,
	APP_ICON_PROTECTED,
	APP_ICON_SELECT_ALL,
	APP_ICON_SELECT_NONE,
	APP_ICON_SELECT_INVERT,
	APP_ICON_PROFILE,
	APP_ICON_FOLDER,
	APP_ICON_SAVEGAME,
	APP_ICON_PORTRAIT_IFAN,
	APP_ICON_PORTRAIT_LOHSE,
	APP_ICON_PORTRAIT_REDPRINCE,
	APP_ICON_PORTRAIT_SEBILLE,
	APP_ICON_PORTRAIT_FANE,
	APP_ICON_PORTRAIT_BEAST,
	APP_ICON_INFO_EMPTY,
	APP_ICON_INFO_SAVEGAME,
	APP_ICON_INFO_VERSION,
	APP_ICON_INFO_DIFFICULTY,
	APP_ICON_INFO_GAMETIME,
	APP_ICON_INFO_DATE,
	APP_ICON_INFO_CHARACTER,
	APP_ICON_INFO_NOTCHARACTER,
	APP_ICON_INFO_LARIAN,
	APP_ICON_INFO_MOD,
	APP_MAX_ICONS
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structure						  	  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct APPICON {
	UINT				id;
	UINT				res;
	UINT				width;
	UINT				height;
} APPICON;

typedef struct SHORTCUT {
	UINT				acc;
	WORD				cmd;
	WORD				_unused;
} SHORTCUT;

typedef struct APPLICATION {
	//--- Configuration
	CONFIG				Config;
	//--- Données diverses
	HINSTANCE			hInstance;
	HANDLE				hHeap;
	HWND				hWnd;
	HWND				hwndStatus;
	HMENU				hMenu;
	HACCEL				hShortcuts;
	HANDLE				hThread;
	HICON				hIcons[APP_MAX_ICONS];
	FONTINFO			Font;
	WCHAR*				pszStatusText;
	WCHAR*				pszWindowTitle;
	CRITICAL_SECTION		CriticalSection;
	XMLTREE				xmlTree;
	NODE				nodeLastFiles;
	//--- Données d'affichage
	UINT				uProgression;
	DWORD				dwProgressionTime;
	//--- Données du jeu
	struct {
		HINSTANCE		hIconsList;
		HINSTANCE		hRunesIconsList;
		NODE			nodeDataItems;
		NODE			nodeXMLCharacters;
		struct DOS2CHARACTER*	pdcList;
		struct DOS2CHARACTER*	pdcCurrent;
		struct {
			HWND		hwndList;
			HWND		hwndAttrBtn[6];
			HWND		hwndPointsBtn[4];
			HWND		hwndAbilitiesBtn;
			HWND		hwndTagsBtn;
			HWND		hwndTalentsBtn;
			HWND		hwndInventory;
			HWND		hwndMenuBtn;
			HWND		hwndInventoryName;
		} Layout;
		struct {
			WCHAR*		pszSaveName;
			FILETIME	ftLastWrite;
			NODE		nodeFiles;
			NODE		nodeXMLRoot;
		} Save;
	} Game;
	//--- Textes traduits de l'application
	LOCALE_TEXT*			pLocaleTexts;
	WCHAR*				pszLocaleName;
} APPLICATION;

#endif

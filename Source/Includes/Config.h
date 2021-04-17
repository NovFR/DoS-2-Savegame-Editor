
//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _CONFIG_INCLUDE
#define _CONFIG_INCLUDE

#include "Locale.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define CONFIG_THIS_VERSION	2

#define CONFIG_REDRAW_WINDOW	0x00000001
#define CONFIG_REDRAW_ITEMS	0x00000002

enum {
	CONFIG_TYPE_TEXT = 0,
	CONFIG_TYPE_UINT,
	CONFIG_TYPE_BOOL,
	CONFIG_TYPE_WINDOW,
};

enum {
	CONFIG_CATEGORY_INIT = 1,
	CONFIG_CATEGORY_HIDE,
	CONFIG_CATEGORY_SHOW,
};

enum {
	CONFIG_IDENT_SAVEONEXIT_V1 = 1, // Don't change order
	CONFIG_IDENT_LOCALENAME_V1,
	CONFIG_IDENT_GAME_V1,
	CONFIG_IDENT_PROFILE_V1,
	CONFIG_IDENT_SHOWHIDDENTAGS_V1,
	CONFIG_IDENT_RUNESGROUPS_V1,
	CONFIG_IDENT_RUNESVIEW_V1,
	CONFIG_IDENT_SKILLSGROUPS_V1,
	CONFIG_IDENT_SKILLSVIEW_V1,
	CONFIG_IDENT_BOOSTERSGROUPS_V1,
	CONFIG_IDENT_CAPOVERRIDE_V1,
	CONFIG_IDENT_SAVELOCATION_V1,
	CONFIG_IDENT_TEMPLOCATION_V1,
	CONFIG_IDENT_ITEMSDISPLAYNAME_V1,
	CONFIG_IDENT_ITEMSRESOLVE_V1,
	CONFIG_IDENT_LOCALENAME_V2,
	CONFIG_IDENT_WINDOW_MAIN_V1,
	CONFIG_IDENT_WINDOW_TV_V1,
	CONFIG_IDENT_TVSEARCHCASESENSITIVE_V1,
	CONFIG_IDENT_TVSEARCHOPACITY_V1,
	CONFIG_IDENT_TVSEARCHALPHA_V1,
	CONFIG_IDENT_TVSEARCHHISTORY_V1,
	CONFIG_IDENT_LOCALENAMELS_V1,
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct CONFIGWINDOW {
	struct {
		int		iLeft;
		int		iTop;
		int		iWidth;
		int		iHeight;
	} position;
	struct {
		BOOL		bCoords;
		BOOL		bSize;
	} usedefault;
} CONFIGWINDOW;

typedef struct CONFIG {
	FILEHEADER		header;
	//--- Config
	BOOL			bSaveOnExit;
	//--- Locale
	WCHAR*			pszLocaleName;
	WCHAR*			pszLocaleNameLS;
	//--- Jeu
	WCHAR*			pszTempPath;
	WCHAR*			pszLarianPath;
	UINT			uGame;
	WCHAR*			pszProfile;
	//--- Fenêtre
	CONFIGWINDOW		windowMain;
	CONFIGWINDOW		windowTreeView;
	BOOL			bTVDebug;
	//--- Affichage
	BOOL			bItemsDisplayName;
	BOOL			bItemsResolve;
	COLORREF		crStats;
	//--- Recherche (TreeView)
	BOOL			bTVSearchCaseSensitive;
	BOOL			bTVSearchOpacity;
	UINT			uTVSearchAlpha;
	BOOL			bTVSearchHistory;
	//--- Edition
	BOOL			bShowHiddenTags;
	BOOL			bRunesGroups;
	UINT			uRunesView;
	BOOL			bSkillsGroups;
	UINT			uSkillsView;
	BOOL			bBoostersGroups;
	BOOL			bCapOverride;
} CONFIG;

typedef struct CONFIGCONTEXT {
	NODE			nRoot;
	NODE			nRootLS;
	BOOL			bIsLimited;
	CONFIG*			pConfig;
	WCHAR*			pszLarianPath;
	WCHAR*			pszTempPath;
	UINT			uCategory;
} CONFIGCONTEXT;

typedef struct CONFIGENTRY {
	NODE			node;
	UINT			uIdent;
	union {
		WCHAR*		pszValue;
		UINT		uValue;
		BOOL		bValue;
		CONFIGWINDOW	window;
	};
} CONFIGENTRY;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Fonctions principales «««««««««««««««««««««««««««««««««««««««««««»

void			Config_Load(CONFIG *);
BOOL			Config_Save(BOOL,CONFIG *);
int			Config_WriteEntry(HANDLE,UINT,UINT,void *);

int			Config_Defaults(CONFIG *);
BOOL			Config_DefaultTempLocation(WCHAR **,BOOL);
BOOL			Config_DefaultSaveLocation(WCHAR **,BOOL);
WCHAR*			Config_DefaultLocaleLS(WCHAR *);
void			Config_Release(CONFIG *);

// «»»» Langage «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void			Config_SelectLanguage(void);
INT_PTR CALLBACK	Config_SelectLanguageProc(HWND,UINT,WPARAM,LPARAM);
BOOL			Config_SelectLanguageInit(HWND,UINT,NODE *,BOOL);
BOOL			Config_SelectLanguageDrawItem(DRAWITEMSTRUCT *);
int			Config_SelectLanguageSet(HWND);
int			Config_SetLanguage(HWND,WCHAR *);

// «»»» Modification ««««««««««««««««««««««««««««««««««««««««««««««««««««»

void			Config_Modify(void);

INT_PTR CALLBACK	Config_ModifyProc(HWND,UINT,WPARAM,LPARAM);
void			Config_ModifyDrawWarning(WCHAR *,DRAWITEMSTRUCT *);
void			Config_ModifyDrawTitle(DRAWITEMSTRUCT *,UINT);
void			Config_ModifyDrawArrow(HWND,UINT,DRAWITEMSTRUCT *);
void			Config_ModifyDrawLabel(DRAWITEMSTRUCT *,UINT);

int			Config_ModifyCategories(HWND,UINT,CONFIGCONTEXT *);
void			Config_ModifyCategoriesDraw(DRAWITEMSTRUCT *);

int			Config_ModifyApplyChanges(HWND,CONFIGCONTEXT *);
void			Config_ModifyCopyDlgText(HWND,UINT,UINT);
int			Config_ModifyGetLanguage(HWND,UINT,UINT,CONFIGCONTEXT *);
int			Config_ModifyGetPath(HWND,UINT,WCHAR **);
void			Config_ModifyMovePath(WCHAR **,WCHAR **);
void			Config_ModifySelectLocation(HWND,UINT,WCHAR *);

#endif

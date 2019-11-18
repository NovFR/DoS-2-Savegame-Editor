
//<<>-<>>---------------------------------------------------------------------()
/*
	Structures des sauvegardes
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _DIVINE_DEFINITIONS
#define _DIVINE_DEFINITIONS


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "GameEdit.h"

#define DIVINE_SIMULATION	0 // If == 1 then files are written in TEMP folder but savegame is not changed

#define DIVINE_ICON_SIZE	24

#define DIVINE_DOS_2		1 // Don't change
#define DIVINE_DOS_2EE		2 // Don't change

enum {
	DIVINE_EXTRACTARCHIVE = 0,
	DIVINE_CREATEARCHIVE,
	DIVINE_GLOBALSLSFTOLSX,
	DIVINE_GLOBALSLSXTOLSF,
	DIVINE_METALSXTOLSF
};

enum {
	DIVINE_DEBUG_ERROR = 0,
	DIVINE_DEBUG_ALL,
};

enum {
	DIVINE_PROFILE_LIST = 0,
	DIVINE_SAVEGAMES_LIST,
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct DIVINESGCONTEXT {
	UINT		uGame;
	WCHAR*		pszProfile;
	WCHAR*		pszSaveName;
	NODE		Profiles;
	NODE		Savegames;
	NODE		GameFiles;
	UINT		uGameImg;
	HBITMAP		hBitmap;
} DIVINESGCONTEXT;

typedef struct DIVINECONTEXT {
	UINT		uDebugLevel;
	UINT		uGame;
	WCHAR*		pszProfile;
	WCHAR*		pszSaveName;
	WCHAR*		pszPath;
	char*		pszLog;
	FILETIME	ftLastWrite;
	DWORD		dwResult;
	BOOL		bNoErrorMsg;
	NODE		Log;
} DIVINECONTEXT;

typedef struct DIVINELOGHEADER {
	char*		pszHeader;
	BOOL		bIsError;
	WCHAR*		pszResIcon;
} DIVINELOGHEADER;

typedef struct DIVINELOG {
	NODE		node;
	WCHAR*		icon;
	WCHAR		line[];
} DIVINELOG;

typedef struct DIVINEITEM {
	NODE		node;
	WCHAR*		name;
	FILETIME	time;
} DIVINEITEM;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Divine_Select(void);
BOOL CALLBACK		Divine_SelectProc(HWND,UINT,WPARAM,LPARAM);
int			Divine_SelectActivate(HWND,GAMEEDITPAGECONTEXT *);
LONG_PTR		Divine_SelectSetPrevPage(HWND,GAMEEDITPAGECONTEXT *);
LONG_PTR		Divine_SelectSetNextPage(HWND,BOOL,GAMEEDITPAGECONTEXT *);
void			Divine_SelectDrawGame(DRAWITEMSTRUCT *);
void			Divine_SelectDrawItem(DRAWITEMSTRUCT *);
void			Divine_SelectDrawSeparator(DRAWITEMSTRUCT *);
void			Divine_SelectDrawGameImage(DRAWITEMSTRUCT *,DIVINESGCONTEXT *);
void			Divine_SelectLoadImage(HWND,DIVINESGCONTEXT *);
void			Divine_SelectLoadGameInfos(HWND,DIVINESGCONTEXT *);
int			Divine_SelectCreateList(HWND,UINT,DIVINESGCONTEXT *);
void			Divine_SelectReleaseList(NODE *);

void			Divine_Open(UINT,WCHAR *,WCHAR *);
DWORD WINAPI		Divine_LoadThread(DIVINECONTEXT *);

void			Divine_Write(void);
DWORD WINAPI		Divine_SaveThread(DIVINECONTEXT *);

DWORD			Divine_Execute(UINT,DIVINECONTEXT *);
void			Divine_ReleaseContext(DIVINECONTEXT *);
void			Divine_Close(void);
void			Divine_Cleanup(void);
int			Divine_CleanupLoop(WCHAR *);

DWORD			Divine_ParseLog(DIVINECONTEXT *);
DIVINELOGHEADER*	Divine_GetHeader(char *);
void			Divine_ShowLog(DIVINECONTEXT *);
INT_PTR CALLBACK	Divine_LogProc(HWND,UINT,WPARAM,LPARAM);
void			Divine_DrawLogLine(DRAWITEMSTRUCT *);

int			Divine_IsSaveGameChanged(HWND,WCHAR *,UINT,WCHAR *,WCHAR *,FILETIME *);
WCHAR*			Divine_CreateTempPath(UINT,...);
WCHAR*			Divine_GetTempPath(UINT,...);
WCHAR*			Divine_GetSaveGamePath(UINT,WCHAR *,WCHAR *);
WCHAR*			Divine_GetGameName(UINT);
UINT			Divine_GetGameFromName(WCHAR *,UINT);
void			Divine_RunConverter(void);

#endif

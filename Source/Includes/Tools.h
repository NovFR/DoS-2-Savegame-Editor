
//<<>-<>>---------------------------------------------------------------------()
/*
	Définitions des outils
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _TOOLS_INCLUDE
#define _TOOLS_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct TLSCONTENT {
	GUID		guid;
	WCHAR*		pszText;
} TLSCONTENT;

typedef struct TLSCONTEXT {
	HWND		hwndParent;
	TLSCONTENT*	pTable;
	LOCALE_MISC*	pLocale;
	WCHAR*		pszStatsLSXPath;
	WCHAR*		pszLocaleXMLPath;
	WCHAR*		pszLocale;
	NODE		nodeStats;
	NODE		nodeLocale;
} TLSCONTEXT;

typedef struct TLSBLACKLIST {
	UINT		uLen;
	WCHAR*		pszBegin;
} TLSBLACKLIST;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Tools_TranslateItems(HWND,WCHAR *,WCHAR *,WCHAR *);
void			Tools_TranslateItemsRelease(TLSCONTEXT *ctx);
DWORD WINAPI		Tools_TranslateItemsThread(TLSCONTEXT *);
int			Tools_TranslateItemsLoad(WCHAR *,NODE *);
int			Tools_LarianStringToGUID(WCHAR *,GUID *);

#endif

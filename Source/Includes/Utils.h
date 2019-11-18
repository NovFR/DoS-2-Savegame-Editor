
//<<>-<>>---------------------------------------------------------------------()
/*
	Fonctions diverses
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _UTILS_INCLUDE
#define _UTILS_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int			Misc_GetFileTime(const WCHAR *,FILETIME *,FILETIME *,FILETIME *);
WCHAR*			Misc_FormatNumber(WCHAR *);
UINT			Misc_HtmlSpecialCharsDecode(WCHAR *,WCHAR *);
UINT			Misc_HtmlSpecialChars(WCHAR *,WCHAR *);
UINT			Misc_uIntToWCHAR(WCHAR *,UINT);
int			Misc_SetEditWndInt(HWND,HWND,WCHAR *);
void			Misc_DrawLabel(HWND,HWND,HDC,WCHAR *,int,BOOL);
WCHAR*			Misc_StrCpyAlloc(WCHAR *);
void			Misc_StrCpy(WCHAR *,int,WCHAR *);
void			Misc_StrCat(WCHAR *,int,WCHAR *);
void			Misc_SetWindowText(HWND,WCHAR **,WCHAR *,WCHAR *,...);
WCHAR*			Misc_UTF8ToWideCharNZ(const char *, const int);
WCHAR*			Misc_UTF8ToWideChar(const char *);
char*			Misc_WideCharToUTF8(const WCHAR *);
#if _DEBUG
void			Misc_DebugOut(const WCHAR *, ...);
#endif

#endif

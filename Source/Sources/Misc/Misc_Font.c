
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de gestion des caractères
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "_Global.h"
#include "Application.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Obtention des informations concernant des caractères ««««««««««««»

int Font_GetInfo(HWND hWnd, FONTINFO *pfiFont)
{
	HDC		hDC;
	HFONT		hOldFont;
	TEXTMETRIC	tmFontInfo;
	int		iResult = 0;

	hDC = GetDC(hWnd);
	if (hDC)
		{
		hOldFont = SelectObject(hDC,pfiFont->hFont);
		if (hOldFont)
			{
			GetTextMetrics(hDC,&tmFontInfo);
			pfiFont->uFontHeight = tmFontInfo.tmExternalLeading+tmFontInfo.tmHeight;
			pfiFont->uFontMaxWidth = tmFontInfo.tmMaxCharWidth;
			pfiFont->uFontAveWidth = tmFontInfo.tmAveCharWidth;
			SelectObject(hDC,hOldFont);
			iResult = 1;
			}
		ReleaseDC(hWnd,hDC);
		}
	return(iResult);
}


// «»»» Création de petits caractères «««««««««««««««««««««««««««««««««««»

int Font_CreateCopy(HWND hWnd, FONTINFO *pfiFromFont, FONTINFO *pfiToFont, LONG lHeight, LONG lWeight)
{
	LOGFONT		lf;

	GetObject(pfiFromFont->hFont,sizeof(LOGFONT),&lf);
	if (lHeight) lf.lfHeight = lHeight;
	if (lWeight) lf.lfWeight = lWeight;
	pfiToFont->hFont = CreateFontIndirect(&lf);
	if (!pfiToFont->hFont) return(0);
	return(Font_GetInfo(hWnd,pfiToFont));
}


// «»»» Jeu de caractères du jeu ««««««««««««««««««««««««««««««««««««««««»

HFONT Font_Create(WCHAR *pszFontFile, WCHAR *pszFontName, LONG lHeight)
{
	LOGFONT	lf;

	if (!AddFontResourceEx(pszFontFile,FR_PRIVATE,NULL)) return(NULL);
	lf.lfHeight = lHeight;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_REGULAR;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy(lf.lfFaceName,pszFontName);
	return(CreateFontIndirect(&lf));
}


// «»»» Libération d'un jeu de caractères «««««««««««««««««««««««««««««««»

void Font_Destroy(WCHAR *pszFontFile, HFONT hFont)
{
	if (hFont)
		{
		DeleteObject(hFont);
		if (pszFontFile) RemoveFontResourceEx(pszFontFile,FR_PRIVATE,NULL);
		}
	return;
}

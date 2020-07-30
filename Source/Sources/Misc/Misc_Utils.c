
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines diverses
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION	App;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Récupération des dimensions de la fenêtre principale ««««««««««««»

void Misc_GetMainWindowClientRect(RECT *rcClient)
{
	RECT	rcWindow;
	RECT	rcStatus;

	GetClientRect(App.hWnd,&rcWindow);
	GetWindowRect(App.hwndStatus,&rcStatus);
	MapWindowPoints(NULL,App.hWnd,(POINT *)&rcStatus,2);
	SubtractRect(rcClient,&rcWindow,&rcStatus);
	return;
}


// «»»» Récupération des dates d'un fichier «««««««««««««««««««««««««««««»

int Misc_GetFileTime(const WCHAR *pszPath, FILETIME *pCreationTime, FILETIME *pLastAccessTime, FILETIME *pLastWriteTime)
{
	HANDLE	hFile;

	if (!pszPath) return(0);

	hFile = CreateFile(pszPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE) return(0);

	if (!GetFileTime(hFile,pCreationTime,pLastAccessTime,pLastWriteTime))
		{
		CloseHandle(hFile);
		return(0);
		}

	CloseHandle(hFile);
	return(1);
}


// «»»» Conversion d'un nombre ««««««««««««««««««««««««««««««««««««««««««»

//!\ The function returns NULL in case of error but also when the string does not need to be modified

WCHAR* Misc_FormatNumber(WCHAR *pszText)
{
	WCHAR*	pszNumber;
	WCHAR*	pszPtr;
	int	uLen;
	int	uParts;
	int	uRemains;

	if (!pszText) return(NULL);
	uLen = wcslen(pszText);
	if (!uLen) return(NULL);
	uParts = uLen/3;
	if (!uParts) return(NULL);
	uRemains = uLen%3;
	pszText += uLen-1;
	uLen = uParts*3+uRemains;
	if (uRemains) uLen += uParts;
	else uLen += uParts-1;

	pszNumber = HeapAlloc(App.hHeap,0,uLen*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszNumber) return(NULL);

	pszNumber[uLen] = 0;
	pszPtr = pszNumber+uLen-1;
	uParts = 0;

	while (uLen > 0)
		{
		if (uParts == 3)
			{
			*pszPtr = L' ';
			uParts = 0;
			pszPtr--;
			uLen--;
			}
		*pszPtr = *pszText;
		pszPtr--;
		pszText--;
		uParts++;
		uLen--;
		}

	return(pszNumber);
}



// «»»» Conversion d'un texte XML «««««««««««««««««««««««««««««««««««««««»

UINT Misc_HtmlSpecialCharsDecode(WCHAR *pszCopy, WCHAR *pszText)
{
	static WCHAR*	htmlEntities[] = { L"&amp;",L"&", L"&lt;",L"<", L"&gt;",L">", L"&#xa",L"\n", L"&quot;",L"\"", NULL };
	UINT		i,j,k,l;
	UINT		uLen;

	if (!pszText) return(0);

	for (i = 0, j = 0, uLen = 0; pszText[i] != 0; i++)
		{
		if (pszText[i] == '&')
			{
			for (k = 0; htmlEntities[k] != NULL; k += 2)
				{
				l = wcslen(htmlEntities[k]);
				if (!wcsncmp(htmlEntities[k],&pszText[i],l))
					{
					i += l-1;
					l = wcslen(htmlEntities[k+1]);
					uLen += l;
					if (pszCopy)
						{
						CopyMemory(&pszCopy[j],htmlEntities[k+1],l*sizeof(WCHAR));
						j += l;
						}
					break;
					}
				}
			}
		else
			{
			if (pszCopy) pszCopy[j++] = pszText[i];
			uLen++;
			}
		}
	if (pszCopy) pszCopy[j] = L'\0';

	return(uLen);
}


// «»»» Convertion d'un texte pour le XML «««««««««««««««««««««««««««««««»

// pszCopy may be NULL
// pszText may be NULL
// Returns the size of the buffer NOT including the terminating null character

UINT Misc_HtmlSpecialChars(WCHAR *pszCopy, WCHAR *pszText)
{
	UINT	i,j;
	UINT	uLen;

	if (!pszText) return(0);

	for (i = 0, j = 0, uLen = 0; pszText[i] != 0; i++)
		{
		if (pszText[i] == L'&')
			{
			if (pszCopy)
				{
				pszCopy[j++] = L'&';
				pszCopy[j++] = L'a';
				pszCopy[j++] = L'm';
				pszCopy[j++] = L'p';
				pszCopy[j++] = L';';
				}
			uLen += 5;
			}
		else if (pszText[i] == L'<')
			{
			if (pszCopy)
				{
				pszCopy[j++] = L'&';
				pszCopy[j++] = L'l';
				pszCopy[j++] = L't';
				pszCopy[j++] = L';';
				}
			uLen += 4;
			}
		else if (pszText[i] == L'>')
			{
			if (pszCopy)
				{
				pszCopy[j++] = L'&';
				pszCopy[j++] = L'g';
				pszCopy[j++] = L't';
				pszCopy[j++] = L';';
				}
			uLen += 4;
			}
		else if (pszText[i] == L'\n')
			{
			if (pszCopy)
				{
				pszCopy[j++] = L'&';
				pszCopy[j++] = L'#';
				pszCopy[j++] = L'x';
				pszCopy[j++] = L'A';
				}
			uLen += 4;
			}
		else if (pszText[i] == L'"')
			{
			if (pszCopy)
				{
				pszCopy[j++] = L'&';
				pszCopy[j++] = L'q';
				pszCopy[j++] = L'u';
				pszCopy[j++] = L'o';
				pszCopy[j++] = L't';
				pszCopy[j++] = L';';
				}
			uLen += 6;
			}
		else if (pszText[i] == L'\n' || pszText[i] >= L' ')
			{
			if (pszCopy) pszCopy[j++] = pszText[i];
			uLen++;
			}
		}
	if (pszCopy) pszCopy[j] = L'\0';

	return(uLen);
}


// «»»» Convertion d'un UINT en WCHAR «««««««««««««««««««««««««««««««««««»

// pszBuffer can be NULL
// Returns the size of the buffer NOT including the terminating null character

UINT Misc_uIntToWCHAR(WCHAR *pszBuffer, UINT uValue)
{
	static WCHAR	digits[] = L"0123456789";
	WCHAR*		p;
	int		s;
	UINT		l;

	if (pszBuffer) p = pszBuffer;
	else p = NULL;
	s = uValue;
	l = 0;

	do {
		if (p) ++p;
		l++;
		s = s/10;
	} while(s);

	if (!p) return(l);

	*p = L'\0';
	do {
		*--p = digits[uValue%10];
		uValue = uValue/10;
	} while(uValue);

	return(l);
}


// «»»» Modifie la valeur d'une EDITBOX «««««««««««««««««««««««««««««««««»

int Misc_SetEditWndInt(HWND hWnd, HWND hwndUpDown, WCHAR *pszInt)
{
	int n = 0;

	if (pszInt) n = wcstol(pszInt,NULL,10);
	else pszInt = szZero;
	SendMessage(hWnd,WM_SETTEXT,0,(LPARAM)pszInt);
	if (hwndUpDown) SendMessage(hwndUpDown,UDM_SETPOS32,0,n);
	return(n);
}


// «»»» Affiche le label d'une EDITBOX ««««««««««««««««««««««««««««««««««»

void Misc_DrawLabel(HWND hWnd, HWND hwndUpDown, HDC hDC, WCHAR *pszText, int iLimitX, BOOL bRight)
{
	RECT	rcWindow;
	UINT	uFormat;

	GetWindowRect(hWnd,&rcWindow);
	MapWindowPoints(NULL,App.hWnd,(POINT *)&rcWindow,2);

	if (bRight)
		{
		if (hwndUpDown)
			{
			RECT	rcTmp;
			GetWindowRect(hwndUpDown,&rcTmp);
			MapWindowPoints(NULL,App.hWnd,(POINT *)&rcTmp,2);
			rcWindow.right = rcTmp.right;
			}
		rcWindow.left = rcWindow.right+8;
		rcWindow.right = iLimitX;
		uFormat = DT_LEFT;
		}
	else
		{
		rcWindow.right = rcWindow.left-8;
		rcWindow.left = iLimitX;
		uFormat = DT_RIGHT;
		}

	DrawText(hDC,pszText,-1,&rcWindow,DT_NOPREFIX|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|uFormat);
	return;
}


// «»»» Copie une chaîne de caractères ««««««««««««««««««««««««««««««««««»

WCHAR* Misc_StrCpyAlloc(WCHAR *src)
{
	WCHAR*	res;

	if (!src) return(NULL);
	res = HeapAlloc(App.hHeap,0,wcslen(src)*sizeof(WCHAR)+sizeof(WCHAR));
	if (!res) return(NULL);
	wcscpy(res,src);
	return(res);
}


// «»»» Copie "sécurisée" d'une chaîne de caractères ««««««««««««««««««««»

void Misc_StrCpy(WCHAR *dst, int size, WCHAR *src)
{
	if (!dst) return;
	if (!size) return;
	if (!src)
		{
		*dst = 0;
		return;
		}

	if (wcslen(src) < size)
		{
		wcscpy(dst,src);
		return;
		}
	wcsncpy(dst,src,size-1);
	dst[size-1] = 0;
	return;
}


// «»»» Ajout "sécurisé" d'une chaîne de caractères «««««««««««««««««««««»

void Misc_StrCat(WCHAR *dst, int size, WCHAR *src)
{
	int	rem;

	if (!dst) return;
	if (!size) return;
	if (!src) return;

	rem = size-wcslen(dst);
	if (rem <= 0) return;

	if (wcslen(src) < rem)
		{
		wcscat(dst,src);
		return;
		}
	wcsncat(dst,src,rem-1);
	dst[size-1] = 0;
	return;
}


// «»»» Modifie le titre d'une fenêtre ««««««««««««««««««««««««««««««««««»

void Misc_SetWindowText(HWND hWnd, WCHAR **pszTitlePtr, WCHAR *pszDefault, WCHAR *pszFmt, ...)
{
	va_list		vl;

	va_start(vl,pszFmt);

	if (*pszTitlePtr)
		{
		LocalFree(*pszTitlePtr);
		*pszTitlePtr = NULL;
		}

	if (!pszFmt)
		{
		SetWindowText(hWnd,pszDefault);
		va_end(vl);
		return;
		}

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING,pszFmt,0,0,(WCHAR *)pszTitlePtr,1,&vl))
		{
		SetWindowText(hWnd,pszDefault);
		va_end(vl);
		return;
		}

	SetWindowText(hWnd,*pszTitlePtr);
	va_end(vl);
	return;
}


// «»»» Conversion d'un texte UTF-8 en WCHAR ««««««««««««««««««««««««««««»

WCHAR* Misc_UTF8ToWideCharNZ(const char *pszText, const int iLen)
{
	WCHAR*	pszResult;
	int	iSize;

	if (!pszText) return(NULL);
	iSize = MultiByteToWideChar(CP_UTF8,0,pszText,iLen,NULL,0);
	if (!iSize) return(NULL);
	pszResult = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR)+sizeof(WCHAR));
	if (!pszResult) return(NULL);
	iSize = MultiByteToWideChar(CP_UTF8,0,pszText,iLen,pszResult,iSize);
	if (iSize)
		{
		pszResult[iSize] = 0;
		return(pszResult);
		}
	HeapFree(App.hHeap,0,pszResult);
	return(NULL);
}

WCHAR* Misc_UTF8ToWideChar(const char *pszText)
{
	WCHAR*	pszResult;
	int	iSize;

	if (!pszText) return(NULL);
	iSize = MultiByteToWideChar(CP_UTF8,0,pszText,-1,NULL,0);
	if (!iSize) return(NULL);
	pszResult = HeapAlloc(App.hHeap,0,iSize*sizeof(WCHAR));
	if (!pszResult) return(NULL);
	iSize = MultiByteToWideChar(CP_UTF8,0,pszText,-1,pszResult,iSize);
	if (iSize) return(pszResult);
	HeapFree(App.hHeap,0,pszResult);
	return(NULL);
}


// «»»» Conversion d'un texte WCHAR en UTF-8 ««««««««««««««««««««««««««««»

char* Misc_WideCharToUTF8(const WCHAR *pszText)
{
	char*	pszResult;
	int	iSize;

	if (!pszText) return(NULL);
	iSize = WideCharToMultiByte(CP_UTF8,0,pszText,-1,NULL,0,NULL,NULL);
	if (!iSize) return(NULL);
	pszResult = HeapAlloc(App.hHeap,0,iSize);
	if (!pszResult) return(NULL);
	iSize = WideCharToMultiByte(CP_UTF8,0,pszText,-1,pszResult,iSize,NULL,NULL);
	if (iSize) return(pszResult);
	HeapFree(App.hHeap,0,pszResult);
	return(NULL);
}


// «»»» Données de debug dans un fichier ««««««««««««««««««««««««««««««««»

#if _DEBUG
void Misc_DebugOut(const WCHAR *pszFmt, ...)
{
	HANDLE	hFile;
	WCHAR*	pszBuffer;
	DWORD	dwBytes;
	va_list	vl;

	va_start(vl,pszFmt);
	pszBuffer = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,65535);
	if (pszBuffer)
		{
		wvsprintf(pszBuffer,pszFmt,vl);
		hFile = CreateFile(L"debug.txt",FILE_APPEND_DATA,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			{
			SetEndOfFile(hFile);
			WriteFile(hFile,pszBuffer,wcslen(pszBuffer)*sizeof(WCHAR),&dwBytes,NULL);
			WriteFile(hFile,szLF,wcslen(szLF)*sizeof(WCHAR),&dwBytes,NULL);
			CloseHandle(hFile);
			}
		HeapFree(App.hHeap,0,pszBuffer);
		}
	va_end(vl);
	return;
}
#endif

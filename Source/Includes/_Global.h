
//<<>-<>>---------------------------------------------------------------------()
/*
	Définitions principales
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _GLOBAL_INCLUDE
#define _GLOBAL_INCLUDE

#undef  _WIN32_WINNT
#define _WIN32_WINNT		0x0601 // Windows 7
#define UNICODE
#define OEMRESOURCE
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NOATOM
#define NOMETAFILE
#define NOMINMAX
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <stdio.h>

#include "sqlite3.h"

#endif

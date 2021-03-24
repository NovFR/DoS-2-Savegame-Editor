
//<<>-<>>---------------------------------------------------------------------()
/*
	Routines de debug
									      */
//()-------------------------------------------------------------------<<>-<>>//

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Debug.h"
#include "Utils.h"
#include "Texts.h"

extern APPLICATION		App;

static DEBUGTYPE		DebugLogTypes[] = {	{ DEBUG_LOG_INFO, L"[INFO]" },
							{ DEBUG_LOG_WARNING, L"[WARNING]" },
							{ DEBUG_LOG_ERROR, L"[ERROR]" },
							{ 0, NULL },
						};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Fonctions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Crash :'( «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

LONG WINAPI Debug_ExceptionHandler(EXCEPTION_POINTERS *ep)
{
	static WCHAR	szDebugMsg[] = L"Program crashed :/";
	static WCHAR	szDebugException[] = L"\n\n0x%x :: %s\n";
	static WCHAR	szDebugStack[] = L"\n[%i] :: 0x%x :: %s";
	static WCHAR	szDebugUnknown[] = L"EXCEPTION_UNKNOWN";

	static WCHAR*	szExceptions[] = { 	(WCHAR *)EXCEPTION_ACCESS_VIOLATION, L"EXCEPTION_ACCESS_VIOLATION",
						(WCHAR *)EXCEPTION_ARRAY_BOUNDS_EXCEEDED, L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED",
						(WCHAR *)EXCEPTION_BREAKPOINT, L"EXCEPTION_BREAKPOINT",
						(WCHAR *)EXCEPTION_DATATYPE_MISALIGNMENT, L"EXCEPTION_DATATYPE_MISALIGNMENT",
						(WCHAR *)EXCEPTION_FLT_DENORMAL_OPERAND, L"EXCEPTION_FLT_DENORMAL_OPERAND",
						(WCHAR *)EXCEPTION_FLT_DIVIDE_BY_ZERO, L"EXCEPTION_FLT_DIVIDE_BY_ZERO",
						(WCHAR *)EXCEPTION_FLT_INEXACT_RESULT, L"EXCEPTION_FLT_INEXACT_RESULT",
						(WCHAR *)EXCEPTION_FLT_INVALID_OPERATION, L"EXCEPTION_FLT_INVALID_OPERATION",
						(WCHAR *)EXCEPTION_FLT_OVERFLOW, L"EXCEPTION_FLT_OVERFLOW",
						(WCHAR *)EXCEPTION_FLT_STACK_CHECK, L"EXCEPTION_FLT_STACK_CHECK",
						(WCHAR *)EXCEPTION_FLT_UNDERFLOW, L"EXCEPTION_FLT_UNDERFLOW",
						(WCHAR *)EXCEPTION_ILLEGAL_INSTRUCTION, L"EXCEPTION_ILLEGAL_INSTRUCTION",
						(WCHAR *)EXCEPTION_IN_PAGE_ERROR, L"EXCEPTION_IN_PAGE_ERROR",
						(WCHAR *)EXCEPTION_INT_DIVIDE_BY_ZERO, L"EXCEPTION_INT_DIVIDE_BY_ZERO",
						(WCHAR *)EXCEPTION_INT_OVERFLOW, L"EXCEPTION_INT_OVERFLOW",
						(WCHAR *)EXCEPTION_INVALID_DISPOSITION, L"EXCEPTION_INVALID_DISPOSITION",
						(WCHAR *)EXCEPTION_NONCONTINUABLE_EXCEPTION, L"EXCEPTION_NONCONTINUABLE_EXCEPTION",
						(WCHAR *)EXCEPTION_PRIV_INSTRUCTION, L"EXCEPTION_PRIV_INSTRUCTION",
						(WCHAR *)EXCEPTION_SINGLE_STEP, L"EXCEPTION_SINGLE_STEP",
						(WCHAR *)EXCEPTION_STACK_OVERFLOW, L"EXCEPTION_STACK_OVERFLOW",
						NULL, NULL };

	DEBUGHELP*	pDebug;
	BOOL		bDisplayed;
	int		i;

	bDisplayed = FALSE;
	pDebug = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(DEBUGHELP));
	if (pDebug)
		{
		pDebug->pszFormat = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(WCHAR)*(wcslen(szDebugMsg)+wcslen(szDebugException)+wcslen(szDebugStack)*DEBUG_MAXTRACE)+sizeof(WCHAR));
		if (pDebug->pszFormat)
			{
			wcscat(pDebug->pszFormat,szDebugMsg);
			wcscat(pDebug->pszFormat,szDebugException);

			//--- Exception
			pDebug->pArgs[0] = (DWORD_PTR)ep->ExceptionRecord->ExceptionAddress;
			for (pDebug->pArgs[1] = (DWORD_PTR)szDebugUnknown, i = 0; szExceptions[i+1] != NULL; i += 2)
				{
				if (ep->ExceptionRecord->ExceptionCode != (DWORD)(UINT64)szExceptions[i]) continue;
				pDebug->pArgs[1] = (DWORD_PTR)szExceptions[i+1];
				break;
				}

			//--- Stack trace
			pDebug->hProcess = GetCurrentProcess();
			pDebug->pInfo = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,sizeof(SYMBOL_INFO)+sizeof(char)*DEBUG_NAMELEN);
			if (pDebug->pInfo)
				{
				if (SymInitialize(pDebug->hProcess,NULL,TRUE))
					{
					int j,n = RtlCaptureStackBackTrace(1,DEBUG_MAXTRACE,pDebug->pBackTrace,NULL);
					for (i = 0, j = 2; i != n; i++)
						{
						pDebug->pInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
						pDebug->pInfo->MaxNameLen = DEBUG_NAMELEN-1;
						pDebug->pInfo->Name[0] = 0;
						SymFromAddr(pDebug->hProcess,(DWORD64)pDebug->pBackTrace[i],NULL,pDebug->pInfo);
						MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pDebug->pInfo->Name,-1,&pDebug->pszNames[i][0],DEBUG_NAMELEN-1);
						pDebug->pArgs[j++] = (DWORD_PTR)i;
						pDebug->pArgs[j++] = (DWORD_PTR)pDebug->pBackTrace[i];
						pDebug->pArgs[j++] = (DWORD_PTR)&pDebug->pszNames[i][0];
						wcscat(pDebug->pszFormat,szDebugStack);
						}
					SymCleanup(pDebug->hProcess);
					}
				HeapFree(App.hHeap,0,pDebug->pInfo);
				}

			//--- Message
			i = Debug_Printf(NULL,pDebug->pszFormat,pDebug->pArgs);
			pDebug->pszMessage = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,i*sizeof(WCHAR)+sizeof(WCHAR));
			if (pDebug->pszMessage)
				{
				Debug_Printf(pDebug->pszMessage,pDebug->pszFormat,pDebug->pArgs);
				MessageBox(NULL,pDebug->pszMessage,NULL,MB_ICONHAND|MB_OK);
				HeapFree(App.hHeap,0,pDebug->pszMessage);
				bDisplayed = TRUE;
				}

			HeapFree(App.hHeap,0,pDebug->pszFormat);
			}
		HeapFree(App.hHeap,0,pDebug);
		}

	if (!bDisplayed) MessageBox(NULL,szDebugMsg,NULL,MB_ICONHAND|MB_OK);
	return(EXCEPTION_EXECUTE_HANDLER);
}


// «»»» Formatage simple ««««««««««««««««««««««««««««««««««««««««««««««««»

int Debug_Printf(WCHAR *pszMessage, WCHAR *pszFormat, DWORD_PTR pArgs[])
{
	WCHAR	szBuffer[32];
	int	i,j,k,l;

	if (pszMessage) *pszMessage = 0;

	for (i = 0, j = 0, k = 0, l = 0; pszFormat[i] != 0;)
		{
		if (pszFormat[i] != '%')
			{
			if (pszMessage) pszMessage[j++] = pszFormat[i];
			i++;
			l++;
			continue;
			}
		if (pszFormat[++i] == 0) break;
		switch(pszFormat[i])
			{
			case 'i':
				Misc_Printf(szBuffer,32,L"%li",pArgs[k++]);
				if (pszMessage) wcscat(pszMessage,szBuffer);
				j += wcslen(szBuffer);
				l += wcslen(szBuffer);
				break;
			case 'x':
				Misc_Printf(szBuffer,32,L"%lX",pArgs[k++]);
				if (pszMessage) wcscat(pszMessage,szBuffer);
				j += wcslen(szBuffer);
				l += wcslen(szBuffer);
				break;
			case 's':
				if (pszMessage) wcscat(pszMessage,(WCHAR *)pArgs[k]);
				j += wcslen((WCHAR *)pArgs[k]);
				l += wcslen((WCHAR *)pArgs[k++]);
				break;
			case '%':
				if (pszMessage) pszMessage[j++] = L'%';
				l++;
				break;
			}
		i++;
		}

	if (pszMessage) pszMessage[j] = 0;
	return(l);
}


// «»»» Logs ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void Debug_Log(UINT uLogType, WCHAR *pszFmt, ...)
{
	SYSTEMTIME	time;
	HANDLE		hFile;
	WCHAR*		pszBuffer;
	WCHAR*		pszWriteBuffer;
	WCHAR*		pszType;
	WCHAR		pszTime[40];
	DWORD		dwBytes;
	int		n;
	va_list		vl;

	if (!pszFmt)
		{
		DeleteFile(szDebugLogFileName);
		return;
		}

	va_start(vl,pszFmt);
	n = vsnwprintf(NULL,0,pszFmt,vl);
	va_end(vl);

	va_start(vl,pszFmt);
	pszBuffer = HeapAlloc(App.hHeap,HEAP_ZERO_MEMORY,n*sizeof(WCHAR)+sizeof(WCHAR));
	if (pszBuffer)
		{
		wvsprintf(pszBuffer,pszFmt,vl);
		hFile = CreateFile(szDebugLogFileName,FILE_APPEND_DATA,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			{
			//--- Time
			GetLocalTime(&time);
			wsprintf(pszTime,szTimeFmt,time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
			//--- Type
			for (pszType = NULL, n = 0; DebugLogTypes[n].pszType != NULL; n++)
				{
				if (DebugLogTypes[n].uType != uLogType) continue;
				pszType = DebugLogTypes[n].pszType;
				break;
				}
			if (pszType == NULL) pszType = DebugLogTypes[0].pszType;
			//--- Write line
			n = wcslen(pszTime)+wcslen(pszType)+wcslen(szSpace)+wcslen(pszBuffer)+wcslen(szLF);
			n *= sizeof(WCHAR);
			pszWriteBuffer = HeapAlloc(App.hHeap,0,n+sizeof(WCHAR));
			if (pszWriteBuffer)
				{
				wcscpy(pszWriteBuffer,pszTime);
				wcscat(pszWriteBuffer,pszType);
				wcscat(pszWriteBuffer,szSpace);
				wcscat(pszWriteBuffer,pszBuffer);
				wcscat(pszWriteBuffer,szLF);
				SetEndOfFile(hFile);
				WriteFile(hFile,pszWriteBuffer,n,&dwBytes,NULL);
				HeapFree(App.hHeap,0,pszWriteBuffer);
				}
			CloseHandle(hFile);
			}
		HeapFree(App.hHeap,0,pszBuffer);
		}
	va_end(vl);
	return;
}

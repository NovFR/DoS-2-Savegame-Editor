
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

extern APPLICATION		App;


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


//<<>-<>>---------------------------------------------------------------------()
/*
	Bo褾es de dialogue
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _DIALOGS_INCLUDE
#define _DIALOGS_INCLUDE


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� D嶨initions							  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

#define DIALOG_SUBCLASS_FILTER		0


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Structures							  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

typedef int (*DIALOGFNINIT)(HWND,RECT *,int,void *);
typedef BOOL (*DIALOGFNDRAW)(UINT,DRAWITEMSTRUCT *,void *);
typedef int (*DIALOGFNOK)(HWND,void *);
typedef int (*DIALOGFNCANCEL)(HWND,void *);

typedef struct DIALOG {
	HWND		hwndParent;
	WCHAR*		pszTitle;
	WCHAR*		pszText;
	UINT		uResID;
	DIALOGFNINIT	fnInit;
	DIALOGFNDRAW	fnDraw;
	DIALOGFNOK	fnOk;
	DIALOGFNCANCEL	fnCancel;
} DIALOG;

typedef struct DIALOGSYNCHRO {
	DIALOG		dialogStruct;
	int		iLevel;
	int		iLevelMin;
	int		iLevelMax;
} DIALOGSYNCHRO;

typedef struct DIALOGVALUE {
	DIALOG		dialogStruct;
	WCHAR*		pszLabel;
	int		iValue;
	int		iValueMin;
	int		iValueMax;
} DIALOGVALUE;


// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //
// 中�									  中� //
// 中� Prototypes							  中� //
// 中�									  中� //
// 中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中中 //

int			Dialog_Open(DIALOG *);
INT_PTR CALLBACK	Dialog_Proc(HWND,UINT,WPARAM,LPARAM);

void			Dialog_DrawTextButton(WCHAR *,DRAWITEMSTRUCT *);
void			Dialog_DrawArrowButton(UINT,DRAWITEMSTRUCT *);
void			Dialog_DrawIconButton(UINT,DRAWITEMSTRUCT *);
void			Dialog_DrawLabel(WCHAR *,DRAWITEMSTRUCT *,UINT);
void			Dialog_DrawInfo(WCHAR *,DRAWITEMSTRUCT *,UINT);

int			Dialog_ViewComboInit(HWND,UINT,UINT,UINT,...);
void			Dialog_ViewComboChanged(HWND,UINT,UINT,UINT *);
int			Dialog_ViewComboMeasureItem(UINT,MEASUREITEMSTRUCT *);
void			Dialog_ViewComboDrawItem(DRAWITEMSTRUCT *);

LRESULT CALLBACK	Dialog_SubDlgProc(HWND,UINT,WPARAM,LPARAM,UINT_PTR,DWORD_PTR);
void			Dialog_OffsetY(HWND,UINT,int);
int			Dialog_GetInfoHeight(HWND,UINT,WCHAR *);
void			Dialog_CenterWindow(HWND,HWND);
int			Dialog_GetIntValue(HWND,UINT,int,int,int *);

#endif

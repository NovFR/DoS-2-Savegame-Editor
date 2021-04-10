
//<<>-<>>---------------------------------------------------------------------()
/*
	Boîtes de dialogue
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _DIALOGS_INCLUDE
#define _DIALOGS_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define DIALOG_SUBCLASS_FILTER		0


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef int (*DIALOGFNINIT)(HWND,RECT *,int,void *);
typedef BOOL (*DIALOGFNDRAW)(UINT,DRAWITEMSTRUCT *,void *);
typedef int (*DIALOGFNOK)(HWND,void *);
typedef int (*DIALOGFNCANCEL)(HWND,void *);
typedef void (*DIALOGFNCLICK)(HWND,UINT,void *);

typedef struct DIALOG {
	HWND		hwndParent;
	WCHAR*		pszTitle;
	WCHAR*		pszText;
	UINT		uResID;
	DIALOGFNINIT	fnInit;
	DIALOGFNDRAW	fnDraw;
	DIALOGFNCLICK	fnClick;
	DIALOGFNOK	fnOk;
	DIALOGFNCANCEL	fnCancel;
} DIALOG;

typedef struct DIALOGSYNCHRO {
	DIALOG		dialogStruct;
	int		iLevel;
	int		iLevelMin;
	int		iLevelMax;
	BOOL		bOnlyEquipped;
	BOOL		bOnlyLowerLevels;
} DIALOGSYNCHRO;

typedef struct DIALOGVALUE {
	DIALOG		dialogStruct;
	WCHAR*		pszLabel;
	int		iValue;
	int		iValueMin;
	int		iValueMax;
} DIALOGVALUE;

typedef struct DIALOGSTATS {
	DIALOG		dialogStruct;
	void*		pdcList;
	void*		pdcCurrent;
	struct {
		WCHAR*	pszName;
		WCHAR*	pszOrigin;
		WCHAR*	pszRace;
		WCHAR*	pszClass;
		WCHAR*	pszMap;
		WCHAR*	pszExp;
		WCHAR*	pszDamages;
		WCHAR*	pszHeals;
		WCHAR*	pszKills;
	} pxa;
	BOOL		bFocus;
	BOOL		bSetCharts;
} DIALOGSTATS;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int			Dialog_Open(DIALOG *);
INT_PTR CALLBACK	Dialog_Proc(HWND,UINT,WPARAM,LPARAM);

void			Dialog_DrawTextButton(WCHAR *,DRAWITEMSTRUCT *);
void			Dialog_DrawArrowButton(UINT,DRAWITEMSTRUCT *);
void			Dialog_DrawIconButton(UINT,DRAWITEMSTRUCT *);
void			Dialog_DrawIconText(HICON,UINT,WCHAR *,DRAWITEMSTRUCT *,HBRUSH);
void			Dialog_DrawLabel(WCHAR *,DRAWITEMSTRUCT *,HBRUSH,UINT);
void			Dialog_DrawInfo(WCHAR *,DRAWITEMSTRUCT *,UINT);
void			Dialog_DrawTitle(WCHAR *,DRAWITEMSTRUCT *);

int			Dialog_ViewComboInit(HWND,UINT,UINT,UINT,...);
int			Dialog_ViewComboChanged(HWND,UINT,UINT,UINT *);
int			Dialog_ViewComboMeasureItem(UINT,MEASUREITEMSTRUCT *);
void			Dialog_ViewComboDrawItem(DRAWITEMSTRUCT *);

LRESULT CALLBACK	Dialog_SubDlgProc(HWND,UINT,WPARAM,LPARAM,UINT_PTR,DWORD_PTR);
void			Dialog_OffsetY(HWND,UINT,int);
int			Dialog_GetInfoHeight(HWND,UINT,WCHAR *);
void			Dialog_CenterWindow(HWND,HWND);
int			Dialog_GetIntValue(HWND,UINT,int,int,int *);
int			Dialog_GetUIntValue(HWND,UINT,UINT,UINT,UINT *);
HRESULT			Dialog_TaskDialogProc(HWND,UINT,WPARAM,LPARAM,LONG_PTR);
void			Dialog_NotImplemented(HWND);

#endif

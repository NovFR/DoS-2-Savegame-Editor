
//<<>-<>>---------------------------------------------------------------------()
/*
	Structures XML
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _XMLTREE_DEFINITIONS
#define _XMLTREE_DEFINITIONS


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Dialogs.h"

#define TREEVIEW_WIDTH			800
#define TREEVIEW_HEIGHT			800
#define TREEVIEW_MIN_WIDTH		640
#define TREEVIEW_MIN_HEIGHT		480

#define TREEVIEW_BUTTON_WIDTH		150
#define TREEVIEW_BUTTON_PADDING		16
#define TREEVIEW_BUTTON_MARGIN_BOTTOM	14
#define TREEVIEW_BUTTON_MARGIN_RIGHT	20
#define TREEVIEW_BUTTON_MARGIN_LEFT	20
#define TREEVIEW_BUTTON_SPACING		8

#define TREEVIEW_MARGIN_LEFT		16
#define TREEVIEW_MARGIN_TOP		16
#define TREEVIEW_MARGIN_RIGHT		16
#define TREEVIEW_MARGIN_BOTTOM		16

#define TREEVIEW_TAB_VPADDING		20
#define TREEVIEW_TAB_VMARGIN		16

#define TREEVIEW_DEBUG_PADDING		16

#define TREEVIEW_WATCH_ID		0x00000001
#define TREEVIEW_WATCH_TYPE		0x00000002
#define TREEVIEW_WATCH_VALUE		0x00000004
#define TREEVIEW_WATCH_HANDLE		0x00000008

enum {
	TV_INSERT_BEFORE = 0,
	TV_INSERT_AFTER,
	TV_INSERT_ASCHILD,
};

enum {
	TV_SEARCH_NODE = 0,
	TV_SEARCH_ATTR,
	TV_SEARCH_OPTIONS,
	TV_SEARCH_NONE,
};

enum {
	TV_SEARCH_TAB_HIDE = 0,
	TV_SEARCH_TAB_SHOW,
	TV_SEARCH_TAB_FOCUS,
	TV_SEARCH_TAB_SETPOS,
	TV_SEARCH_TAB_GETSEL,
};

enum { // Saved in history file, don't remove entries and don't change their order
	TV_SEARCH_HISTORY_NONE = 0,
	TV_SEARCH_HISTORY_ID,
	TV_SEARCH_HISTORY_VALUE,
	TV_SEARCH_HISTORY_HANDLE,
	TV_SEARCH_HISTORY_ALL,
};

typedef struct XMLTREE {
	HWND			hWnd;
	HWND			hwndTree;
	HWND			hwndMenu;
	HWND			hwndDebug;
	HWND			hwndClose;
	WCHAR*			pszWindowTitle;
	XML_NODE*		pxn;
	BOOL			bDontAskAgain;
	BOOL			bEditing;
	BOOL			bEditAborted;
	struct {
		HWND		hWnd;
		NODE*		node;
	} debug;
	struct {
		HWND		hDlg;
		HWND		hwndTab;
		BOOL		bMoved;
		LONG		lLeft;
		LONG		lTop;
		UINT		uMode;
		BOOL		bMatchAny;
		WCHAR*		pszId;
		WCHAR*		pszType;
		WCHAR*		pszValue;
		WCHAR*		pszHandle;
		WCHAR*		pszAll;
		NODE		history;
	} search;
} XMLTREE;

typedef struct XMLINSERT {
	DIALOG			dialogStruct;
	NODE			inserts;
	HTREEITEM		hItem;
} XMLINSERT;

typedef struct XMLVALIDATION {
	WCHAR*			pszNode;
	WCHAR*			pszAttributes[5];
} XMLVALIDATION;

typedef struct XMLNAMES {
	UINT			uCount;
	WCHAR*			pszNames[];
} XMLNAMES;

typedef struct TVSEARCHSTRING {
	NODE			node;
	WCHAR			szText[];
} TVSEARCHSTRING;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Tree_Open(XML_NODE *);
LRESULT			Tree_Create(HWND);
void			Tree_Destroy(void);

int			Tree_CreateTreeView(XML_NODE *);
int			Tree_CreateNodeTree(XML_NODE *,HTREEITEM);
int			Tree_CreateAttrTree(XML_NODE *,HTREEITEM);
void			Tree_DestroyTreeView(void);

LRESULT			Tree_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
void			Tree_Move(HWND);
void			Tree_Resize(HWND);
void			Tree_Menu(HWND,HWND);
void			Tree_Command(HWND,HWND,WORD,BOOL);
BOOL			Tree_Confirm(HWND,WCHAR *,WCHAR *,BOOL *);

void			Tree_InsertXML(HWND,HTREEITEM);
int			Tree_InsertXML_Init(HWND,RECT *,int,void *);
UINT			Tree_InsertXML_GetMode(HWND);
void			Tree_InsertXML_Click(HWND,UINT,void *);
int			Tree_InsertXML_Ok(HWND,void *);
void			Tree_InsertXML_Verify(HWND,XMLINSERT *);
BOOL			Tree_InsertXML_Check(HWND,NODE *,HTREEITEM,UINT);
BOOL			Tree_InsertXML_CheckNode(HWND,NODE *,BOOL *,BOOL *);
BOOL			Tree_InsertXML_CheckAttr(HWND,NODE *,BOOL *,BOOL *);
void			Tree_InsertXML_Menu(HWND,HWND,XMLINSERT *);

void			Tree_InsertAttribute(HWND,HTREEITEM);
int			Tree_InsertAttr_Init(HWND,RECT *,int,void *);
BOOL			Tree_InsertAttr_Draw(UINT,DRAWITEMSTRUCT *,void *);
int			Tree_InsertAttr_Ok(HWND,void *);

void			Tree_SearchDialog(HWND);
INT_PTR CALLBACK	Tree_SearchDialogProc(HWND,UINT,WPARAM,LPARAM);
void			Tree_SearchGetType(HWND,BOOL);
void			Tree_SearchGetParams(HWND);
void			Tree_SearchGetString(HWND,UINT);
void			Tree_SearchEnableOk(HWND);
DWORD			Tree_SearchIsEnabled(void);
void			Tree_SearchTabControl(HWND,UINT,...);
void			Tree_SearchHistoryLoad(HWND);
void			Tree_SearchHistoryClear(HWND);
void			Tree_SearchHistoryRelease(void);
void			Tree_SearchHistoryAdd(HWND,UINT,WCHAR *,UINT);

void			Tree_Search(HWND);
DWORD			Tree_SearchStringIsOk(WCHAR *,DWORD);
DWORD			Tree_SearchMatch(WCHAR *,WCHAR *,DWORD,BOOL,BOOL);
NODE*			Tree_SearchNextItem(NODE *);
void			Tree_SearchSelectItem(NODE *);
HTREEITEM		Tree_SearchSelectedItemParent(HTREEITEM,NODE **,UINT);
void			Tree_SearchRelease(void);

void			Tree_Debug(BOOL);
int			Tree_DebugCreate(HWND);
LRESULT			Tree_DebugMessages(HWND,UINT,WPARAM,LPARAM);
void			Tree_DebugAttach(RECT *,RECT *);
void			Tree_DebugUpdate(HTREEITEM);
int			Tree_DebugPaint(HDC,RECT *,BOOL);
int			Tree_DebugPaintLF(int,RECT *,BOOL);
int			Tree_DebugPaintDrawAddr(HDC,RECT *,WCHAR *,void *,int,BOOL);

BOOL			Tree_DisplayError(HWND,UINT,BOOL *,...);
BOOL			Tree_IsValueValid(WCHAR *);
WCHAR*			Tree_GetHtmlEntities(void);
BOOL			Tree_IsAttrValidForNode(WCHAR *,WCHAR *);
#define			Tree_IsNodeKnown(a) Tree_IsAttrValidForNode(NULL,a)
#define			Tree_IsAttrKnown(a) Tree_IsAttrValidForNode(a,NULL)
BOOL			Tree_IsNodeHasAttributes(WCHAR *);
XMLNAMES*		Tree_CollectNames(NODE *,XMLNAMES *);
void			Tree_ReleaseNames(XMLNAMES *);
int			Tree_SortNames(const void *,const void *);
void			Tree_CompareNames(HWND,NODE *,XMLNAMES *,BOOL *,BOOL *);
XML_NODE*		Tree_GetRootNode(TVITEMEX *);

#endif

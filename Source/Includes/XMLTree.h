
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

#define TREEVIEW_WIDTH			800
#define TREEVIEW_HEIGHT			800
#define TREEVIEW_MIN_WIDTH		640
#define TREEVIEW_MIN_HEIGHT		480

#define TREEVIEW_BUTTON_WIDTH		150
#define TREEVIEW_BUTTON_PADDING		16
#define TREEVIEW_BUTTON_MARGIN_BOTTOM	14
#define TREEVIEW_BUTTON_MARGIN_RIGHT	4
#define TREEVIEW_BUTTON_SPACING		8

#define TREEVIEW_MARGIN_LEFT		16
#define TREEVIEW_MARGIN_TOP		16
#define TREEVIEW_MARGIN_RIGHT		16
#define TREEVIEW_MARGIN_BOTTOM		16

typedef struct XMLTREE {
	HWND		hWnd;
	HWND		hwndTree;
	HWND		hwndSave;
	HWND		hwndHelp;
	HWND		hwndClose;
	WCHAR*		pszWindowTitle;
	XML_NODE*	pxn;
} XMLTREE;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Tree_Open(XML_NODE *);
LRESULT			Tree_Create(HWND);
void			Tree_Destroy(void);

int			Tree_CreateTreeView(XML_NODE *);
#if _DEBUG
void			Tree_CreateDebugInfos(void *,BOOL,HTREEITEM);
void			Tree_CreateDebugNode(WCHAR *,NODE *,HTREEITEM);
void			Tree_CreateDebugInfo(WCHAR *,DWORD64,HTREEITEM);
#endif
int			Tree_CreateNodeTree(XML_NODE *,HTREEITEM);
int			Tree_CreateAttrTree(XML_NODE *,HTREEITEM);
void			Tree_DestroyTreeView(void);

LRESULT			Tree_ProcessMessages(HWND,UINT,WPARAM,LPARAM);
void			Tree_Move(HWND);
void			Tree_Resize(HWND);

#endif

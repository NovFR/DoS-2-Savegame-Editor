
//<<>-<>>---------------------------------------------------------------------()
/*
	Structures XML
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _XML_DEFINITIONS
#define _XML_DEFINITIONS


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define XML_TABS_BUFFER_LEN		128
#define XML_WRITE_BUFFER_LEN		65535

#define XML_FLAG_HASHEADER		0x00000001		// File content starts with <?xml ...>
#define XML_FLAG_HASCONTENT		0x00000002		// Stores text inside tags (<tag>text</tag>)
#define XML_FLAG_FROMMEMORY		0x00000100		// pszFilePath contains the Source Buffer (The buffer is copied and encoded in UTF-8)
#define XML_FLAG_UTF8MEMORY		0x00000200		// Source Buffer is already UTF-8 encoded (1) (The buffer must be allocated with HeapAlloc(), it is released by the function)
#define XML_FLAG_DISPLAYMSG		0x10000000		// Use MessageBox() instead of SendMessage() (2) (Mutually exclusive with XML_FLAG_DEBUGMSG)
#define XML_FLAG_DEBUGMSG		0x20000000		// Use Debug_Log() instead of SendMessage() (2) (Mutually exclusive with XML_FLAG_DISPLAYMSG)
#define XML_FLAG_NOSTATUSMSG		0x40000000		// Don't display status msg (3)
#define XML_FLAG_NOPROGRESS		0x80000000		// Don't update progressbar (2)

// (1) Has no effect if XML_FLAG_FROMMEMORY is not set.
// (2) SendMessage() and ProgressBar can't be used if called from the main thread.
// (3) Must be set if XML_FLAG_FROMMEMORY is set.

#include "Lists.h"

enum {
	XML_TYPE_NODE = 0,
	XML_TYPE_ATTR
};

enum {
	XML_TARGET_GLOBALS = 0,
	XML_TARGET_META,
	XML_TARGET_CUSTOM
};

enum {
	XML_ERROR_NONE = 0,
	XML_ERROR_EOF,
	XML_ERROR_FROM_SYSTEM,
	XML_ERROR_FROM_LOCALE
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct XML_NODE {
	NODE		node;
	NODE		children;
	NODE		attributes;
	void*		parent;
	WCHAR*		content;
	WCHAR		name[];
} XML_NODE;

typedef struct XML_ATTR {
	NODE		node;
	void*		parent;
	WCHAR*		value;
	WCHAR		name[];
} XML_ATTR;

typedef struct XML_PARSER {
	HWND		hWnd;
	WCHAR*		pszFilePath;
	DWORD		dwFlags;
	HANDLE		hFile;
	DWORD		dwLastErrorType;
	DWORD		dwLastErrorMsg;
	int		iResult;
	//--- Load
	NODE*		pRoot;
	BYTE*		pFileBuffer;
	DWORD		dwFileSize;
	DWORD		dwCursor;
	DWORD		dwTagBegin;
	DWORD		dwTagEnd;
	DWORD		dwTagSize;
	//--- Save
	WCHAR*		pszTabsBuffer;
	XML_NODE*	pxnBegin;
	UINT		uTabsCount;
	UINT		uTabsToAllocate;
	UINT		uNodesCount;
	UINT		uNodesTotal;
	void*		pWriteBuffer;
	UINT		uWriteBufferCursor;
	UINT		uWriteBufferSize;
} XML_PARSER;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int			xml_LoadFile(HWND,WCHAR *,NODE *,DWORD);
int			xml_ReadFile(XML_PARSER *);
int			xml_ReadTag(XML_PARSER *);
int			xml_ParseNodes(XML_PARSER *,XML_NODE *,NODE *);
int			xml_ParseAttributes(XML_PARSER *,XML_NODE *);

int			xml_SaveFile(WCHAR *,UINT,XML_NODE *);
int			xml_WriteFile(XML_PARSER *);
DWORD			xml_WriteNodes(XML_PARSER *,XML_NODE *);
BOOL			xml_WriteTabs(XML_PARSER *);
BOOL			xml_WriteUTF8(XML_PARSER *,WCHAR *);
BOOL			xml_WriteToBuffer(XML_PARSER *,void *,UINT,BOOL);

void			xml_ReleaseAll(NODE *);
void			xml_ReleaseNode(XML_NODE *);
void			xml_ReleaseAttr(XML_ATTR *);
void			xml_FreeParser(XML_PARSER *);

XML_NODE*		xml_GetNodeFromPath(XML_NODE *,WCHAR *[]);
XML_NODE*		xml_GetNodeFromPathFirstChild(XML_NODE *,WCHAR *[]);
XML_NODE*		xml_GetNode(XML_NODE *,WCHAR *,WCHAR *,WCHAR *);
XML_NODE*		xml_GetNextNode(XML_NODE *);
XML_ATTR*		xml_GetAttr(XML_NODE *,WCHAR *);
XML_ATTR*		xml_GetXMLValueAttr(XML_NODE *,WCHAR *,WCHAR *,WCHAR *);
WCHAR*			xml_GetAttrValue(XML_NODE *,WCHAR *);
WCHAR*			xml_GetThisAttrValue(XML_ATTR *);
BOOL			xml_IsTrue(XML_ATTR *);
BOOL			xml_IsValueTrue(WCHAR *);
UINT			xml_TotalNodesCount(XML_NODE *,BOOL);

BOOL			xml_InsertChildNode(XML_NODE *,XML_NODE *,WCHAR *,WCHAR *,WCHAR *,BOOL);
XML_NODE*		xml_CreateNode(WCHAR *,XML_NODE *,UINT,...);
XML_NODE*		xml_CreateNodeArray(WCHAR ***,XML_NODE *);
XML_ATTR*		xml_CreateAttr(WCHAR *,XML_NODE *);
BOOL			xml_SetAttrValue(XML_ATTR *,WCHAR *);
BOOL			xml_SetAttrValueNumber(XML_ATTR *,UINT);
WCHAR*			xml_BuildWideCharPath(XML_NODE *);
WCHAR*			xml_AttrToWideChar(XML_NODE *);
WCHAR*			xml_ValueToWideChar(XML_ATTR *);
WCHAR*			xml_AppendWideCharValue(XML_ATTR *,WCHAR *);

void			xml_SendErrorMsg(HWND,UINT,UINT,DWORD);
void			xml_UpdateProgress(UINT,UINT,DWORD);

#endif

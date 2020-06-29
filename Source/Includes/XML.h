
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
#define XML_WRITE_BUFFER_LEN		2048
#define XML_BUFFER_INCREASE_MULT	2

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
	WCHAR		name[];
} XML_NODE;

typedef struct XML_ATTR {
	NODE		node;
	void*		parent;
	WCHAR*		value;
	WCHAR		name[];
} XML_ATTR;

typedef struct XML_PARSER {
	WCHAR*		pszFilePath;
	HANDLE		hFile;
	DWORD		dwLastErrorType;
	DWORD		dwLastErrorMsg;
	int		iResult;
	//--- Load
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

int			xml_LoadFile(WCHAR *);
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
UINT			xml_TotalNodesCount(XML_NODE *);

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

void			xml_SendErrorMsg(UINT,UINT);
void			xml_UpdateProgress(UINT,UINT);

#endif

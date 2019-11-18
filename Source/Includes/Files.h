
//<<>-<>>---------------------------------------------------------------------()
/*
	Fichiers
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _FILES_INCLUDE
#define _FILES_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "PNG.h"

#define COMPRESSION_METHOD		0x0F
#define COMPRESSION_METHOD_NONE		0x00
#define COMPRESSION_METHOD_ZLIB		0x01
#define COMPRESSION_METHOD_LZ4		0x02

#define COMPRESSION_MODE		0xF0
#define COMPRESSION_MODE_FAST		0x10
#define COMPRESSION_MODE_DEFAULT	0x20
#define COMPRESSION_MODE_MAX		0x40

#define LS_MODE_QUIET			0x00000001
#define LS_MODE_SAVEINFO		0x00000002

#define LS_TYPE_UNKNOWN			0x00000000
#define LS_TYPE_META			0x00000001
#define LS_TYPE_GLOBALS			0x00000002
#define LS_TYPE_LSF			0x00000004
#define LS_TYPE_PNG			0x00000008

enum {
	LSF_UNPACK_NAMES = 1,
	LSF_UNPACK_NODES,
	LSF_UNPACK_ATTRIBUTES,
	LSF_UNPACK_VALUES
};

enum {
	DT_None = 0,
	DT_Byte = 1,
	DT_Short = 2,
	DT_UShort = 3,
	DT_Int = 4,
	DT_UInt = 5,
	DT_Float = 6,
	DT_Double = 7,
	DT_IVec2 = 8,
	DT_IVec3 = 9,
	DT_IVec4 = 10,
	DT_Vec2 = 11,
	DT_Vec3 = 12,
	DT_Vec4 = 13,
	DT_Mat2 = 14,
	DT_Mat3 = 15,
	DT_Mat3x4 = 16,
	DT_Mat4x3 = 17,
	DT_Mat4 = 18,
	DT_Bool = 19,
	DT_String = 20,
	DT_Path = 21,
	DT_FixedString = 22,
	DT_LSString = 23,
	DT_ULongLong = 24,
	DT_ScratchBuffer = 25,
	DT_Long = 26,
	DT_Int8 = 27,
	DT_TranslatedString = 28,
	DT_WString = 29,
	DT_LSWString = 30,
	DT_UUID = 31,
	DT_Unknown32 = 32,
	DT_TranslatedFSString = 33
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct PNGIMAGEINFO {
	int		iWidth;
	int		iHeight;
	int		iBitDepth;
	int		iColorType;
	void*		pImage;
	UINT		uOffset;
} PNGIMAGEINFO;

typedef struct LSPKHEADER13 {
	int		Version;
	int		FileListOffset;
	int		FileListSize;
	short int	NumParts;
	short int	SomePartVar;
	unsigned char	MD5[16];
} LSPKHEADER13;

typedef struct FILEENTRY13 {
	char		Name[256];
	int		OffsetInFile;
	int		SizeOnDisk;
	int		UncompressedSize;
	int		ArchivePart;
	int		Flags;
	int		CRC;
} FILEENTRY13;

typedef struct LSFHEADER {
	BYTE		Signature[4];
	UINT32		Version;
	UINT32		EngineVersion;
	UINT32		StringsUncompressedSize;
	UINT32		StringsSizeOnDisk;
	UINT32		NodesUncompressedSize;
	UINT32		NodesSizeOnDisk;
	UINT32		AttributesUncompressedSize;
	UINT32		AttributesSizeOnDisk;
	UINT32		ValuesUncompressedSize;
	UINT32		ValuesSizeOnDisk;
	BYTE		CompressionFlags;
	BYTE		Unknown2;
	UINT16		Unknown3;
	UINT32		Extended;
} LSFHEADER;

typedef struct NODEENTRYV2 {
	UINT32		NameHashTableIndex;
	UINT32		FirstAttributeIndex;
	UINT32		ParentIndex;
} NODEENTRYV2;

typedef struct NODEENTRYV3 {
	UINT32		NameHashTableIndex;
	UINT32		ParentIndex;
	UINT32		NextSiblingIndex;
	UINT32		FirstAttributeIndex;
} NODEENTRYV3;

typedef struct ATTRIBUTEENTRYV2 {
	UINT32		NameHashTableIndex;
	UINT32		TypeAndLength;
	UINT32		NodeIndex;
} ATTRIBUTEENTRYV2;

typedef struct ATTRIBUTEENTRYV3 {
	UINT32		NameHashTableIndex;
	UINT32		TypeAndLength;
	UINT32		NextAttributeIndex;
	UINT32		Offset;
} ATTRIBUTEENTRYV3;

typedef struct LSFILE {
	NODE		node;
	UINT		uSize;
	BYTE*		pData;
	WCHAR*		pszName;
	NODE		nodeXMLRoot;
	HBITMAP		hBitmap;
	DWORD		dwType;
} LSFILE;

typedef struct LSVREADER {
	HANDLE		hFile;
	DWORD		dwFileSize;
	DWORD		dwBytes;
	BYTE*		pFileBuffer;
	BYTE*		pFileList; // Ref
	LSPKHEADER13*	pHeader; // Ref
	FILEENTRY13*	pFileListBuffer;
	UINT		uLastError;
	int		iNumFiles;
	int		iDecompressed;
} LSVREADER;

typedef struct LSFREADER {
	LSFHEADER*	pHeader; // Ref
	BYTE*		pBuffer; // Ref
	NODE		nodeNames;
	NODE		nodeNodes;
	NODE		nodeAttrs;
	BYTE*		pValues;
	BOOL		bIsCompressed;
	BOOL		bChunked;
	UINT		uIndex;
	UINT		uLastError;
} LSFREADER;

typedef struct LFSNODEINFO {
	int		ParentIndex;
	int		NameIndex;
	int		NameOffset;
	int		FirstAttributeIndex;
} LFSNODEINFO;

typedef struct LFSATTRINFO {
	int		NameIndex;
	int		NameOffset;
	unsigned int	TypeId;
	unsigned int	Length;
	unsigned int	DataOffset;
	int		NextAttributeIndex;
} LFSATTRINFO;

typedef struct LFSNAMES {
	NODE		node;
	UINT		uNumStrings;
	WCHAR*		pszNames[];
} LFSNAMES;

typedef struct LFSNODES {
	NODE		node;
	UINT		uNumNodes;
	LFSNODEINFO	nodeInfos[];
} LFSNODES;

typedef struct LFSATTRS {
	NODE		node;
	UINT		uNumAttrs;
	LFSATTRINFO	attrInfos[];
} LFSATTRS;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» LSV «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL			lsv_Load(HWND,WCHAR *,NODE *,DWORD);
void			lsv_Release(NODE *);

LSFILE*			lsv_FindFile(NODE *,WCHAR *,WCHAR *);
NODE*			lsv_GetMetaXML(NODE *);

// «»»» LSF «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

BOOL			lsf_Create(LSFREADER *,LSFILE *);
BOOL			lsf_CreateNode(LSFREADER *,UINT,XML_NODE *,NODE *);
WCHAR*			lsf_GetName(LSFREADER *,UINT,UINT);
WCHAR*			lsf_ReadAttribute(UINT,void *,UINT,UINT *);

BOOL			lsf_Unpack(HWND,LSFILE *,DWORD);
BOOL			lsf_UnpackList(LSFREADER *,DWORD,DWORD,DWORD);
void			lsf_ReleaseNames(NODE *);
void			lsf_ReleaseNodes(NODE *);
void			lsf_ReleaseAttrs(NODE *);
void			lsf_ReleaseValues(BYTE *);

// «»»» PNG ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

HBITMAP			png_LoadImage(WCHAR *);
HBITMAP			png_Load(void *);
void			png_Read(png_struct *,png_byte *,png_size_t);

// «»»» Divers «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

UINT			lsa_Decompress(BYTE,BYTE *,DWORD,BYTE *,DWORD,DWORD);

#endif

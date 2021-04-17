
//<<>-<>>---------------------------------------------------------------------()
/*
	Définitions
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _DEFINITIONS_INCLUDE
#define _DEFINITIONS_INCLUDE

// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define DEFS_EXT_UNCOMPRESSED	0x00000001
#define DEFS_EXT_COMPRESSED	0x00000002
#define DEFS_EXT_ALL		0x0000000F

enum {
	DEFS_RESULT_OK = 0,
	DEFS_RESULT_FAILED = 1,
	DEFS_RESULT_ABORTED = 2,
};

enum {
	DEFS_SQL_TEXT = 0,
	DEFS_SQL_NULLTEXT,
	DEFS_SQL_INT32,
};

enum {
	DEFS_FLUSH_GROUP = 0,
	DEFS_FLUSH_ENTRY,
};

enum {
	DEFS_STATS_BEGIN = 0,
	DEFS_STATS_END,
	DEFS_STATS_SINGLE,
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct DOS2TEXSIZE {
	int			iWidth;
	int			iHeight;
} DOS2TEXSIZE;

typedef struct DOS2TEXPATH {
	WCHAR*			pszUUID;
	WCHAR*			pszPath;
} DOS2TEXPATH;

typedef struct DOS2OBJECT {
	WCHAR*			NameW;
	char*			MapKey;
	char*			Type;
	char*			Name;
	char*			Stats;
	char*			DisplayName;
	char*			Description;
	char*			Icon;
	unsigned int		maxStackAmount;
	char*			SubSection;
	char*			Tags;
} DOS2OBJECT;

typedef struct DOS2ITEMGROUP {
	int			minLevel;
	int			maxLevel;
	char*			quality;
	char*			root;
	char*			name;
	int			cool;
	char*			icon;
} DOS2ITEMGROUP;

typedef struct DOS2ITEMENTRY {
	char*			Type;
	char*			Using;
	char*			ItemGroup;
	char*			Slot;
	char*			InventoryTab;
	char*			ArmorType;
	char*			ItemColor;
	int			RuneSlots;
	int			RuneSlots_V1;
} DOS2ITEMENTRY;

typedef struct DEFSQL {
	UINT			uId;
	sqlite3*		pSql;
} DEFSQL;

typedef struct DEFSTATS {
	DWORD			dwTicks;
	DWORD			dwEntries;
} DEFSTATS;

typedef struct DEFSCONTEXT {
	NODE			files;
	BOOL			bWipe;
	HWND			hwndParent;
	WCHAR*			pszConfirm;
	int			iPos[2];
	int			iShowLogs;
	int			iResult;
	DEFSQL			db;
} DEFSCONTEXT;

typedef struct DEFSFILE {
	NODE			node;
	WCHAR*			pszPath;
	BYTE*			pData;
	NODE			xmlRoot;
	int			iSize;
	struct {
		DOS2TEXSIZE	IconSize;
		DOS2TEXSIZE	TextureSize;
		DOS2TEXPATH	Path;
	} atlas;
} DEFSFILE;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			defs_Rebuild(HWND,UINT,BOOL,WCHAR *,LPTHREAD_START_ROUTINE);
HRESULT			defs_RebuildProc(HWND,UINT,WPARAM,LPARAM,LONG_PTR);
void			defs_DoneMessage(DEFSCONTEXT *);
void			defs_Stats(DEFSTATS *,UINT);
void			defs_ReleaseAll(DEFSCONTEXT *);

int			defs_CreateLanguage(DEFSFILE *,DEFSCONTEXT *);
int			defs_NextLine(char **);
int			defs_GetContentUID(char **,char **);
int			defs_GetContent(char **,char **);

int			defs_UpdateLocalization(DEFSQL *,DEFSFILE *);

int			defs_UpdateStats(DEFSQL *,DEFSFILE *);
int			defs_UpdateStatsTable(DEFSQL *,DEFSFILE *);
int			defs_UpdateStatsTableFlush(DEFSQL *,int,char *,UINT,void *);
char*			defs_UpdateStatsTableExplode(char *,char ***);
void			defs_UpdateStatsCommandPrint(UINT,char **);
void			defs_UpdateStatsCommandToWideChar(char **);

int			defs_UpdateRootTemplates(DEFSQL *,DEFSFILE *);
void			defs_BindText(sqlite3_stmt *,int,char *);
void			defs_ResetObject(DOS2OBJECT *);

int			defs_UpdateIconsUV(DEFSQL *,DEFSFILE *);
int			defs_UpdateTextureAtlas(DEFSQL *,DEFSFILE *);

int			defs_CreateFileList(WCHAR *,DEFSCONTEXT *);
int			defs_LoadData(DEFSFILE *);
int			defs_LoadXML(DEFSFILE *);

int			defs_IsEmpty(char *);
char*			defs_StrCpyAlloc(char *);
UINT			defs_HtmlSpecialCharsDecode(char *,char *);
int			defs_ExtIsKnown(WCHAR *,DWORD);
WCHAR*			defs_GetFilePath(WCHAR *,WCHAR *);
void			defs_SQLWipeTables(DEFSQL *,BOOL,int,...);
int			defs_SQLQuery(DEFSQL *,char *,int,...);
void			defs_SQLErrorMsg(sqlite3 *);
void			defs_CreateMsgQueue(void);
int			defs_ThreadAbort(void);

#endif

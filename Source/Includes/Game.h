
//<<>-<>>---------------------------------------------------------------------()
/*
	Structures du jeu
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _GAME_DEFINITIONS
#define _GAME_DEFINITIONS


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "XML.h"
#include "Menus.h"
#include "Definitions.h"

#define GAME_ICON_SIZE			48
#define GAME_EQUIPPED_SLOT_MAX		10

#define GAME_LEVEL_MIN			1
#define GAME_LEVEL_MAX			35
#define GAME_ATTRIBUTE_MIN		1
#define GAME_ATTRIBUTE_MAX		40
#define GAME_ATTRIBUTE_OVERRIDE		999
#define GAME_POINTS_MIN			0
#define GAME_POINTS_MAX			99
#define GAME_ABILITIES_OVERRIDE		99

#define GAME_DISPLAY_BARHEIGHT		20
#define GAME_DISPLAY_BARVPADDING	4
#define GAME_DISPLAY_BARHPADDING	4

#define GAME_LOCK_DISABLED		0x00000000 // default
#define GAME_LOCK_ENABLED		0x00000001
#define GAME_LOCK_APP			0x00000002
#define GAME_LOCK_FILE			0x00000004
#define GAME_LOCK_TREE			0x00000008
#define GAME_LOCK_SETUP			0x00000010
#define GAME_LOCK_ALL			GAME_LOCK_APP|GAME_LOCK_FILE|GAME_LOCK_TREE|GAME_LOCK_SETUP

enum {
	ITEM_TYPE_REAL = 1,
	ITEM_TYPE_ROOT,
	ITEM_TYPE_PARENT,
};

enum {
	DATA_TYPE_ITEMS = 1,
	DATA_TYPE_BOOSTERS,
	DATA_TYPE_RUNES,
	DATA_TYPE_TAGS,
	DATA_TYPE_SKILLS,
	DATA_TYPE_INSERTIONS,
};

enum {
	DATA_TYPE_IS_ITEM = 0,
	DATA_TYPE_IS_ARMOR,
	DATA_TYPE_IS_WEAPON,
	DATA_TYPE_IS_ACCESSORY,
};

enum {
	CMP_TYPE_FULL = 1,
	CMP_TYPE_BEGIN,
	CMP_TYPE_END,
	CMP_TYPE_CONTENT,
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

//--- Structures générales ---

typedef struct DOS2INVENTORY {
	NODE			node;
	NODE			nodeItems;
	void*			pParentItem;
	int			iTopIndex;
	int			iSelected;
} DOS2INVENTORY;

typedef struct DOS2CHARACTER {
	NODE			node;
	NODE			nodeInventories;
	UINT			uInventoryDepth;
	//--- References
	XML_NODE*		pxnRoot;
	XML_ATTR*		pxaInventoryId;
	XML_ATTR*		pxaArmor;
	XML_ATTR*		pxaArmorMax;
	XML_ATTR*		pxaMagicArmor;
	XML_ATTR*		pxaMagicArmorMax;
	XML_ATTR*		pxaVitality;
	XML_ATTR*		pxaVitalityMax;
	XML_ATTR*		pxaMap;
	XML_ATTR*		pxaDamageCount;
	XML_ATTR*		pxaHealCount;
	XML_ATTR*		pxaKillCount;
	XML_ATTR*		pxaName;
	XML_ATTR*		pxaOriginName;
	XML_ATTR*		pxaRace;
	XML_ATTR*		pxaClass;
	XML_ATTR*		pxaExp;
	XML_ATTR*		pxaPoints[4];
	XML_ATTR*		pxaAttributes[6];
	XML_ATTR*		pxaAbilities[40];
	XML_ATTR*		pxaTalents[4];
	XML_NODE*		pxaTags;
	DOS2INVENTORY*		pdiInventory;
} DOS2CHARACTER;

typedef struct DOS2ITEM {
	NODE			node;
	BOOL			bIsBackPack;
	UINT			uNumItems;
	WCHAR*			pszDisplayName;
	//--- References
	XML_NODE*		pxnRoot;
	XML_ATTR*		pxaHandle;
	XML_ATTR*		pxaStats;
	XML_ATTR*		pxaDisplayName;
	XML_ATTR*		pxaDescription;
	XML_ATTR*		pxaAmount;
	XML_ATTR*		pxaIsGenerated;
	XML_ATTR*		pxaHasCustomBase;
	XML_ATTR*		pxaSlot;
	XML_ATTR*		pxaInventory;
	XML_ATTR*		pxaType;
	XML_ATTR*		pxaLevel;
	XML_ATTR*		pxaLevelGroupIndex;
	XML_ATTR*		pxaNameIndex;
	XML_ATTR*		pxaRunes[3];
	XML_NODE*		pxnGeneration;
	XML_NODE*		pxnPermanentBoost;
	XML_NODE*		pxnBase;
	XML_NODE*		pxnLevelOverride;
} DOS2ITEM;

//--- Affichage ---

typedef struct DOS2ICON {
	NODE			node;
	HICON			hIcon;
	WCHAR*			pszName;
} DOS2ICON;

typedef struct GAMEQUALITY {
	WCHAR*			xmlName;
	UINT			uNameId;
	COLORREF		crColor;
} GAMEQUALITY;

typedef struct GAMEORIGIN {
	WCHAR*			pszName;
	UINT			uLocaleId;
} GAMEORIGIN;

typedef struct GAMEDRAWCONTEXT {
	//--- GDI
	HFONT			hFont;
	RECT			rcArea;
	RECT			rcWork;
	RECT			rcText;
	RECT			rcRgn;
	COLORREF		crColor;
	int			iBack;
	//--- Game
	WCHAR*			pszClass;
	WCHAR*			pszExp;
	WCHAR*			pszLevel;
	WCHAR			szLevel[4];
	WCHAR*			pszArmor;
	WCHAR*			pszMagicArmor;
	WCHAR*			pszVitality;
	WCHAR			szNextLevel[12];
	struct {
		SIZE		sizeClass;
		SIZE		sizeClassSpace;
		SIZE		sizeLabel;
		SIZE		sizeSpace;
		SIZE		sizeLevel;
		WCHAR*		pszText;
	} level;
} GAMEDRAWCONTEXT;

//--- Données ---

typedef struct GAMEDATABOOSTER {
	WCHAR*			pszType;
	WCHAR*			pszSlot;
} GAMEDATABOOSTER;

typedef struct GAMEDATARUNE {
	WCHAR*			pszBonus1;
	WCHAR*			pszBonus2;
	WCHAR*			pszBonus3;
} GAMEDATARUNE;

typedef struct GAMEDATATAG {
	BOOL			bProtected;
	BOOL			bHidden;
	WCHAR*			pszDescription;
} GAMEDATATAG;

typedef struct GAMEDATA {
	NODE			node;
	WCHAR*			pszId;
	WCHAR*			pszText;
	union {
		GAMEDATABOOSTER	booster;
		GAMEDATARUNE	rune;
		GAMEDATATAG	tag;
	};
} GAMEDATA;

typedef struct GAMEDATAFLAG {
	char*			pszFlagName;
	UINT			uType;
	UINT64			uFlags;
} GAMEDATAFLAG;

typedef struct GAMEDATASCHOOL {
	char*			pszSchool;
	UINT			uLocaleId;
} GAMEDATASCHOOL;

typedef struct GAMEDATAITEM {
	NODE			node;
	UINT			uType;
	UINT64			uFlags;
	WCHAR*			pszIconName;
	struct {
		WCHAR*		pszFull;
		WCHAR*		pszBegin;
		WCHAR*		pszContent;
		WCHAR*		pszEnd;
	} filter;
} GAMEDATAITEM;

typedef struct GAMEDATASKILLINFOS {
	UINT			uSourcePoints;
	UINT			uActionPoints;
	UINT			uMemorySlots;
	UINT			uIconID;
	UINT			uSchoolLocaleID;
} GAMEDATASKILLINFOS;

typedef struct GAMEDATASKILLOPTIONS {
	BOOL			bIsLearned;
	BOOL			bIsActivated;
	BOOL			bZeroMemory;
	float			fActiveCooldown;
	double			fTimeAdded;
} GAMEDATASKILLOPTIONS;

typedef struct GAMEDATASKILL {
	NODE			node;
	WCHAR*			pszId;
	WCHAR*			pszName;
	int			iIconIndex;
	GAMEDATASKILLINFOS	infos;
	GAMEDATASKILLOPTIONS	options;
} GAMEDATASKILL;

typedef struct GAMEDATAINSERT {
	NODE			node;
	WCHAR*			pszName;
	WCHAR*			pszContent;
	CUSTOMMENUTEMPLATE	cMenu;
} GAMEDATAINSERT;

typedef struct GAMEDATAPARSER {
	BOOL			bSuccess;
	HANDLE			hFile;
	DWORD			dwFileSize;
	BYTE*			pFileBuffer;
	BYTE*			pFilePtr;
	BYTE*			pLinePtr;
	BYTE*			pLineBegin;
	BYTE*			pLineEnd;
	LOCALE_MISC*		pLocale;
	WCHAR*			pszTemp;
	WCHAR*			pszResults[4];
	union {
		GAMEDATAITEM*	pItem;
		GAMEDATA*	pData;
		GAMEDATASKILL*	pSkill;
		GAMEDATAINSERT*	pInsert;
	};
} GAMEDATAPARSER;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

// «»»» Gestion «««««««««««««««««««««««««««««««««««««««««««««««««««««««««»

int			Game_CreateLayout(void);
int			Game_CreateInventoryGroups(HWND);
int			Game_CreateButton(HWND,int,int,int,int,WCHAR *,UINT,HWND *,UINT);
void			Game_Resize(void);

void			Game_InventoryMenu(HWND,UINT);
int CALLBACK		Game_ItemsListSort(LPARAM,LPARAM,LPARAM);

void			Game_Setup(DOS2CHARACTER *,BOOL,BOOL);
int			Game_BuildPlayers(NODE *,BOOL);
void			Game_ReleasePlayers(NODE *,BOOL);
DOS2INVENTORY*		Game_BuildInventory(DOS2ITEM *,XML_ATTR *,NODE *);
UINT			Game_GetInventoryItemsCount(XML_NODE *,WCHAR *);
void			Game_ReleaseInventory(DOS2INVENTORY *);
void			Game_ReleaseItem(DOS2ITEM *);
void			Game_ReleaseDisplayNames(void);
void			Game_CharacterChanged(BOOL);
void			Game_BuildItemsList(DOS2CHARACTER *,HWND);
void			Game_UpdateButtons(void);
void			Game_SaveTopIndex(DOS2CHARACTER *,HWND);

void			Game_SetDefsMenu(HMENU);
void			Game_Lock(DWORD);
UINT			Game_GetLevelFromExp(UINT);
UINT			Game_GetExpFromLevel(UINT);
UINT			Game_GetNextLevelFromExp(UINT);
UINT			Game_GetMaxXP(void);
UINT			Game_GetMaxLevel(void);

BOOL			Game_IsItemEquipped(DOS2ITEM *);
void			Game_ItemDisplayName(DOS2ITEM *);
void			Game_ItemDisplayNameRelease(DOS2ITEM *);
XML_NODE*		Game_ItemGetOwner(DOS2ITEM *,WCHAR *);
WCHAR*			Game_ItemGetOwnerHandle(DOS2ITEM *,WCHAR *);
BOOL			Game_ItemBelongToCharacter(DOS2ITEM *);
BOOL			Game_ItemBelongToParty(DOS2ITEM *);

// «»»» Affichage «««««««««««««««««««««««««««««««««««««««««««««««««««««««»

void			Game_MeasureList(MEASUREITEMSTRUCT *);
void			Game_DrawList(DRAWITEMSTRUCT *);

void			Game_MeasureInventory(MEASUREITEMSTRUCT *);
void			Game_DrawInventory(DRAWITEMSTRUCT *);

void			Game_Paint(HWND,HDC,RECT *);
void			Game_PaintDecorationBorder(DRAWITEMSTRUCT *,BOOL);
void			Game_PaintDecoration(HDC,int,int,HBITMAP);
void			Game_PaintAttributes(HWND,HDC,RECT *);
int			Game_PaintStat(HDC,HWND,RECT *,UINT,WCHAR *,UINT);
void			Game_PaintValue(HDC,LONG,HWND,WCHAR *,WCHAR *,UINT);

void			Game_PaintButton(DRAWITEMSTRUCT *);
void			Game_PaintBag(DOS2CHARACTER *,DRAWITEMSTRUCT *);
void			Game_PaintIcon(HDC,WCHAR *,UINT,RECT *,int,BOOL,BOOL);

// «»»» Fenêtre des attributs «««««««««««««««««««««««««««««««««««««««««««»

LRESULT			Game_ContainerCreate(HWND);
LRESULT			Game_ProcessContainerMessages(HWND,UINT,WPARAM,LPARAM);
void			Game_ContainerResize(HWND,int,int);
void			Game_ContainerMouseWheel(HWND,int);
void			Game_ContainerScroll(UINT);
long			Game_ContainerTopOffset(LONG,RECT *);
void			Game_ContainerScrollInfo(UINT);

LRESULT			Game_AttributesCreate(HWND);
LRESULT			Game_ProcessAttributesMessages(HWND,UINT,WPARAM,LPARAM);
void			Game_AttributesResize(HWND,int,int);
void			Game_AttributesMakeFocusVisible(void);

// «»»» Fichiers de données «««««««««««««««««««««««««««««««««««««««««««««»

BOOL			Game_LoadDataFile(HWND,WCHAR *,UINT,NODE *);
BYTE*			Game_LoadSplitWord(BYTE *);
BYTE*			Game_LoadLeadingSpaces(BYTE *,BYTE *,DWORD);
BYTE*			Game_LoadNextLine(BYTE *,BYTE *,DWORD,BOOL);
void			Game_LoadGetItemInfos(char *,GAMEDATAITEM *);
void			Game_LoadGetItemInfo(char *,GAMEDATAITEM *);

void			Game_UnloadDataFile(UINT,NODE *);

WCHAR*			Game_GetItemIconName(WCHAR *);
WCHAR*			Game_GetItemTypeName(WCHAR *);
UINT			Game_GetItemType(WCHAR *);
UINT64			Game_GetItemFlags(WCHAR *);
GAMEDATAITEM*		Game_GetItemData(WCHAR *);
BOOL			Game_CompareStrings(WCHAR *,WCHAR *,UINT,UINT);

// «»»» Statistiques ««««««««««««««««««««««««««««««««««««««««««««««««««««»

void			Game_Stats(HWND);
int			Game_Stats_Init(HWND,RECT *,int,void *);
BOOL			Game_Stats_Draw(UINT,DRAWITEMSTRUCT *,void *);
void			Game_Stats_Click(HWND,UINT,void *);
WCHAR*			Game_Stats_GetOriginName(WCHAR *);
void			Game_Stats_SetNumber(HWND,UINT,WCHAR *);
int			Game_Stats_SetItem(HWND,UINT,int,WCHAR *,WCHAR *,WCHAR *,UINT);
int			Game_Stats_Sort(LPARAM,LPARAM,LPARAM);

// «»»» Icônes & Objets «««««««««««««««««««««««««««««««««««««««««««««««««»

void			Game_ItemsCopyToClipboard(void *,UINT);
WCHAR*			Game_ItemsGetDisplayName(DOS2ITEM *);

void			Game_ItemsLocalizationRebuild(HWND);
DWORD WINAPI		Game_ItemsLocalizationRebuildThread(DEFSCONTEXT *);
void			Game_IconsRebuild(HWND);
DWORD WINAPI		Game_IconsRebuildThread(DEFSCONTEXT *);
void			Game_ItemsStatsRebuild(HWND);
DWORD WINAPI		Game_ItemsStatsRebuildThread(DEFSCONTEXT *);
void			Game_ItemsRootRebuild(HWND hWnd);
DWORD WINAPI		Game_ItemsRootRebuildThread(DEFSCONTEXT *);

#endif

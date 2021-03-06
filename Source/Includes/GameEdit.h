
//<<>-<>>---------------------------------------------------------------------()
/*
	Structures du jeu
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _GAMEEDIT_DEFINITIONS
#define _GAMEEDIT_DEFINITIONS


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� D�finitions							  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

#include "Game.h"

#define BONUS_NUMBER_MAX_LEN		15
#define BONUS_NUMBER_MAX		9999999
#define BONUS_NUMBER_MIN		-9999999

#define FILTER_ARMORS_ANY		0x0000000000000001
#define FILTER_ARMORS_AMULET		0x0000000000000002
#define FILTER_ARMORS_RING		0x0000000000000004
#define FILTER_ARMORS_BOOTS		0x0000000000000008
#define FILTER_ARMORS_HELMET		0x0000000000000010
#define FILTER_ARMORS_BELT		0x0000000000000020
#define FILTER_ARMORS_GLOVES		0x0000000000000040
#define FILTER_ARMORS_PANTS		0x0000000000000080
#define FILTER_ARMORS_UPPERBODY		0x0000000000000100
#define FILTER_SHIELDS			0x0000000000001000
#define FILTER_WEAPONS_ANY		0x0000000000010000
#define FILTER_ALL_TARGETS		FILTER_ARMORS_ANY|FILTER_ARMORS_AMULET|FILTER_ARMORS_RING|FILTER_ARMORS_BOOTS|FILTER_ARMORS_HELMET|FILTER_ARMORS_BELT|FILTER_ARMORS_GLOVES|FILTER_ARMORS_PANTS|FILTER_ARMORS_UPPERBODY|FILTER_SHIELDS|FILTER_WEAPONS_ANY
#define FILTER_ABILITIES		0x0000000100000000
#define FILTER_ARMORS			0x0000000200000000
#define FILTER_MAGICAL_ARMOR		0x0000000400000000
#define FILTER_PHYSICAL_ARMOR		0x0000000800000000
#define FILTER_RUNE_SLOT		0x0000001000000000
#define FILTER_PRIMARY_ATTRIBUTE	0x0000002000000000
#define FILTER_SECONDARY_ATTRIBUTE	0x0000004000000000
#define FILTER_RESISTANCE		0x0000008000000000
#define FILTER_SKILL			0x0000010000000000
#define FILTER_IMMUNITY			0x0000020000000000
#define FILTER_REFLECT			0x0000040000000000
#define FILTER_SPECIAL			0x0000080000000000
#define FILTER_DAMAGE			0x0000100000000000
#define FILTER_LIFESTEAL		0x0000200000000000
#define FILTER_STATUS			0x0000400000000000
#define FILTER_ALL_TYPES		FILTER_ABILITIES|FILTER_ARMORS|FILTER_MAGICAL_ARMOR|FILTER_PHYSICAL_ARMOR|FILTER_RUNE_SLOT|FILTER_PRIMARY_ATTRIBUTE|FILTER_SECONDARY_ATTRIBUTE|FILTER_RESISTANCE|FILTER_SKILL|FILTER_IMMUNITY|FILTER_REFLECT|FILTER_SPECIAL|FILTER_DAMAGE|FILTER_LIFESTEAL|FILTER_STATUS
#define FILTER_ALL			FILTER_ALL_TARGETS|FILTER_ALL_TYPES

#define BONUS_FLAG_TARGET_ARMOR		0x00000001 // [EDITION]
#define BONUS_FLAG_TARGET_WEAPON	0x00000002 // [EDITION]
#define BONUS_FLAG_TARGET_SHIELD	0x00000004 // [EDITION]
#define BONUS_FLAG_TARGET_ALL		BONUS_FLAG_TARGET_ARMOR|BONUS_FLAG_TARGET_WEAPON|BONUS_FLAG_TARGET_SHIELD
#define BONUS_FLAG_INT			0x00000100
#define BONUS_FLAG_UINT			0x00000200
#define BONUS_FLAG_FLOAT		0x00000400
#define BONUS_FLAG_DAMAGETYPE		0x00001000 // [EDITION]
#define BONUS_FLAG_REFLECTION		0x00002000
#define BONUS_FLAG_SKILL		0x00004000
#define BONUS_FLAG_COLORS		0x00008000
#define BONUS_FLAG_PERCENT		0x00100000 // [DISPLAY]
#define BONUS_FLAG_DEGREES		0x00200000 // [DISPLAY]
#define BONUS_FLAG_METERS		0x00400000 // [DISPLAY]
#define BONUS_FLAG_SPECIAL1		0x10000000 // [DISPLAY] value is mul by 100 (1 = 100)
#define BONUS_FLAG_SPECIAL2		0x20000000 // [DISPLAY] value is div by 100 (1 = 0.01)
#define BONUS_FLAG_ATTRIBUTE		0x40000000 // [XML] attributes are stored in another node
#define BONUS_FLAG_TITLE		0x80000000 // [DISPLAY] title

enum {
	BOOSTER_UPDATE_FROMLIST = 1,
	BOOSTER_UPDATE_FROMTEXTBOX,
	BOOSTER_UPDATE_FROMVALUE
};

enum {
	GAME_PAGE_NAME = 1,
	GAME_PAGE_AMOUNT,
	GAME_PAGE_QUALITY,
	GAME_PAGE_BOOSTERS,
	GAME_PAGE_BONUSES,
	GAME_PAGE_RUNES,
	GAME_PAGE_SYNCHRONIZE,
	GAME_PAGE_SAVEGAME_PROFILE,
	GAME_PAGE_SAVEGAME_LIST
};

enum {
	GAME_PAGE_BONUS_TARGET = 1,
	GAME_PAGE_BONUS_LIST,
	GAME_PAGE_BONUS_VALUE,
	GAME_PAGE_BONUS_DAMAGETYPE,
	GAME_PAGE_BONUS_REFLECTION,
	GAME_PAGE_BONUS_SKILLS,
	GAME_PAGE_BONUS_COLORS,
};

enum {
	DATA_SORT_ID = 0,
	DATA_SORT_TEXT,
	DATA_SORT_RUNE,
	DATA_SORT_RUNE_BONUS1,
	DATA_SORT_RUNE_BONUS2,
	DATA_SORT_RUNE_BONUS3,
	DATA_SORT_BOOSTER_TYPE,
	DATA_SORT_BOOSTER_SLOT,
	DATA_SORT_SKILL_SCHOOL,
	DATA_SORT_SKILL_POINTS,
};

enum {
	BONUS_NODE_TYPE_ATTR = 0,
	BONUS_NODE_TYPE_SKILL,
	BONUS_NODE_TYPE_REFLECT,
};

enum {
	GAME_DAMAGETYPE_SETUP = 0,
	GAME_DAMAGETYPE_STORE,
	GAME_DAMAGETYPE_DISPLAY,
};

enum {
	GAME_TALENT_HUMAN = 1,
	GAME_TALENT_DWARF,
	GAME_TALENT_ELF,
	GAME_TALENT_UNDEAD,
	GAME_TALENT_LIZARD,
};


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Structures							  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

//--- Edition d'une valeur ---

typedef struct GAMEEDITVALUE {
	HWND			hwndParent;
	UINT			uType;
	UINT			uResId;
	UINT			uSort;
	UINT64			uFilter;
	UINT64*			pFilter;
	NODE			nodeData;
	WCHAR*			pszFileName;
	WCHAR*			pszValue;
	WCHAR*			pszObject;
	WCHAR*			pszResult;
	WCHAR*			pszTitle;
	GAMEDATA		localData;
} GAMEEDITVALUE;

//--- Edition des mots-cl�s ---

typedef struct GAMETAGS {
	NODE			nodeTags;
	NODE			nodeUsedTags;
} GAMETAGS;

//--- Edition des talents ---

typedef struct GAMETALENT {
	DWORD			dwFlag;
	UINT			uGroup;
	UINT			uRacial;
} GAMETALENT;

typedef struct GAMETALENTSTRUCT {
	GAMETALENT		Flag;
	WCHAR*			pszName;
	WCHAR*			pszDescription;
} GAMETALENTSTRUCT;

typedef struct GAMETALENTSCONTEXT {
	BOOL			bInitialized;
	HWND			hwndTooltip;
	WCHAR*			pszValue;
	WCHAR			szTemp[16];
	LOCALE_MISC*		pLocale;
	DWORD			dwTalents[4];
	DWORD			dwUnknownTalents[4];
	UINT			uNumTalents;
	GAMETALENTSTRUCT	Talents[];
} GAMETALENTSCONTEXT;

//--- Capacit�s ---

typedef struct GAMEABILITY {
	UINT			uIndex;
	UINT			uLocaleID;
	UINT			uCtlID;
	UINT			uValue;
	UINT			uMax;
} GAMEABILITY;

//--- Bonus ---

typedef struct GAMEBONUS {
	WCHAR*			pszId;
	UINT			uLocaleID;
	UINT			uFlags;
} GAMEBONUS;

typedef struct GAMESKILL {
	WCHAR*			pszId;
	WCHAR*			pszName;
	UINT			uSourcePoints;
	UINT			uActionPoints;
	UINT			uIconID;
	UINT			uSchoolLocaleID;
} GAMESKILL;

typedef struct GAMEITEMCOLORS {
	WCHAR*			pszId;
	WCHAR*			pszName;
	DWORD			colors[3];
} GAMEITEMCOLORS;

//--- Pages ---

typedef struct GAMEEDITPAGE {
	UINT			uPageID;
	UINT			uTitleID;
	UINT			uInfoID;
	UINT			uResID;
} GAMEEDITPAGE;

typedef struct GAMEEDITFILTER {
	UINT			uCtrlID;
	UINT64			uFilter;
	UINT			uLocaleID;
} GAMEEDITFILTER;

typedef struct GAMEEDITBOOSTER {
	NODE			node;
	WCHAR*			pszId;
	WCHAR*			pszName;
} GAMEEDITBOOSTER;

typedef struct GAMEEDITBONUS {
	NODE			node;
	WCHAR*			pszId;
	UINT			uLocaleID;
	UINT			uType;		// Can be one of the following:
						// BONUS_FLAG_INT or BONUS_FLAG_UINT or BONUS_FLAG_FLOAT,
						// BONUS_FLAG_REFLECTION, BONUS_FLAG_SKILL or BONUS_FLAG_COLOR
	UINT			uFlags;
	// Do not use union here
	struct {
		WCHAR	szVal[BONUS_NUMBER_MAX_LEN+1];
	} value;
	struct {
		WCHAR*	pszId;
		WCHAR*	pszName;
	} skill;
	struct {
		BOOL	bIsSet;
		WCHAR	szPercentage[BONUS_NUMBER_MAX_LEN+1];
		WCHAR	szDamageType[BONUS_NUMBER_MAX_LEN+1];
	} reflection;
	struct {
		WCHAR*	pszId;
		WCHAR*	pszName;
	} colors;
} GAMEEDITBONUS;

typedef struct GAMEEDITRUNE {
	WCHAR*			pszId;
	WCHAR*			pszName;
	WCHAR*			pszBonuses[3];
} GAMEEDITRUNE;

typedef struct GAMEEDITITEMCONTEXT {
	WCHAR*			pszName;
	WCHAR*			pszDisplayName;
	WCHAR*			pszDescription;
	BOOL			bIsGenerated;
	BOOL			bHasRunes;
	BOOL			bBonuses;
	UINT			uSlot;
	int			iAmount;
	int			iAmountOld;
	UINT			uLevel;
	UINT			uNewLevel;
	BOOL			bSetLevel;
	BOOL			bSetGenLevel;
	LOCALE_MISC*		pLocale;
	UINT64			uFilter;
	NODE			nodeBoosters;
	NODE			nodeBonuses;
	GAMEEDITRUNE		runes[3];
} GAMEEDITITEMCONTEXT;

typedef struct GAMEEDITBONUSCONTEXT {
	GAMEEDITBONUS*		pEdit;
	LOCALE_MISC*		pLocale;
	WCHAR*			pszObject;
	GAMEBONUS*		pSelected;
	DWORD			uNewListMask;
	DWORD			uListMask;
} GAMEEDITBONUSCONTEXT;

typedef struct GAMEEDITPAGECONTEXT {
	BOOL			bPageSet;
	UINT			uPageID;
	WCHAR*			pszInfo;
	union {
		struct {
			GAMEEDITITEMCONTEXT*	pContext;
			HWND			hwndCtrl[2];
		} item;
		struct {
			GAMEEDITBONUSCONTEXT*	pContext;
			UINT			uSort;
			HWND			hwndCtrl;
		} bonus;
		struct {
			struct DIVINESGCONTEXT*	pContext;
		} savegame;
	};
} GAMEEDITPAGECONTEXT;


// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //
// い�									  い� //
// い� Prototypes							  い� //
// い�									  い� //
// いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい //

// �算� Edition ����������������������������������������������������������

void			Game_Edit(DOS2ITEM *,UINT);

BOOL CALLBACK		Game_EditProc(HWND,UINT,WPARAM,LPARAM);
BOOL			Game_EditInit(HWND,GAMEEDITPAGECONTEXT *);
int			Game_EditActivate(HWND,GAMEEDITPAGECONTEXT *);
int			Game_EditApply(HWND,GAMEEDITPAGECONTEXT *);
void			Game_EditShowList(HWND,NODE *);

BOOL			Game_EditNameCopy(XML_ATTR *,WCHAR **);
BOOL			Game_EditNameSet(DOS2ITEM *,XML_ATTR **,WCHAR *,WCHAR *);

void			Game_EditBoostersDrawArrow(UINT,DRAWITEMSTRUCT *);
void			Game_EditBoostersSelected(HWND,GAMEEDITITEMCONTEXT *);
void			Game_EditBoostersDraw(DRAWITEMSTRUCT *);
int			Game_EditBoostersCopy(GAMEEDITITEMCONTEXT *,XML_NODE *);
int			Game_EditBoostersRebuild(GAMEEDITITEMCONTEXT *,XML_ATTR *,XML_NODE *);
void			Game_EditBoostersRelease(NODE *);
void			Game_EditBoostersChange(HWND,BOOL,GAMEEDITITEMCONTEXT *);
void			Game_EditBoostersRemove(HWND,GAMEEDITITEMCONTEXT *);

void			Game_EditBonusSelected(HWND,GAMEEDITITEMCONTEXT *);
void			Game_EditBonusDraw(DRAWITEMSTRUCT *);
int			Game_EditBonusCopy(GAMEEDITITEMCONTEXT *,XML_NODE *);
int			Game_EditBonusCopyCreate(GAMEEDITITEMCONTEXT *,XML_NODE *,WCHAR *,WCHAR *,GAMEBONUS *,LOCALE_MISC *);
int			Game_EditBonusRebuild(GAMEEDITITEMCONTEXT *,XML_NODE *);
UINT			Game_EditBonusRebuildGetSkills(GAMEEDITITEMCONTEXT *,WCHAR *);
void			Game_EditBonusRebuildCleanup(GAMEEDITITEMCONTEXT *,XML_NODE *);
void			Game_EditBonusChange(HWND,BOOL,GAMEEDITITEMCONTEXT *);
void			Game_EditBonusRemove(HWND,GAMEEDITITEMCONTEXT *);

int			Game_EditRunesCopy(DOS2ITEM *,GAMEEDITITEMCONTEXT *);
void			Game_EditRunesRelease(GAMEEDITRUNE *);
void			Game_EditRunesDrawName(GAMEEDITITEMCONTEXT *,UINT,DRAWITEMSTRUCT *);
void			Game_EditRunesDrawBonus(GAMEEDITITEMCONTEXT *,UINT,DRAWITEMSTRUCT *);
void			Game_EditRunesChange(HWND,UINT,GAMEEDITITEMCONTEXT *);
void			Game_EditRunesRemove(HWND,UINT,GAMEEDITITEMCONTEXT *);

// �算� S�lection des bonus ����������������������������������������������

GAMEEDITBONUS*		Game_Bonus(HWND,GAMEEDITBONUS *,WCHAR *);

BOOL CALLBACK		Game_BonusProc(HWND,UINT,WPARAM,LPARAM);
LRESULT	CALLBACK	Game_BonusEditSubclass(HWND,UINT,WPARAM,LPARAM,UINT_PTR,DWORD_PTR);
LONG_PTR		Game_BonusSetPrevPage(HWND,GAMEEDITPAGECONTEXT *);
LONG_PTR		Game_BonusSetNextPage(HWND,BOOL,GAMEEDITPAGECONTEXT *);
LONG_PTR		Game_BonusGetPage(UINT);
int			Game_BonusActivate(HWND,GAMEEDITPAGECONTEXT *);
int			Game_BonusInitColors(HWND,UINT,GAMEEDITPAGECONTEXT *);
int			Game_BonusSortColors(LPARAM,LPARAM,LPARAM);
int			Game_BonusInitSkills(HWND,UINT,GAMEEDITPAGECONTEXT *);
int			Game_BonusSortSkills(LPARAM,LPARAM,LPARAM);
int			Game_BonusSortSkillsCmp(GAMESKILL *,GAMESKILL *,UINT);
void			Game_BonusDraw(DRAWITEMSTRUCT *);
void			Game_BonusDrawSeparator(WCHAR *,DRAWITEMSTRUCT *);
void			Game_BonusDrawResult(GAMEEDITPAGECONTEXT *,DRAWITEMSTRUCT *);
void			Game_BonusEditMove(HWND,UINT,int,GAMEEDITPAGECONTEXT *);

WCHAR*			Game_BonusGetDisplayText(GAMEEDITBONUS *);
WCHAR*			Game_BonusDamageType(HWND,WCHAR *,UINT);
GAMEBONUS*		Game_BonusGetFromID(WCHAR *);
int			Game_BonusSetInfos(GAMEEDITBONUS *,GAMEBONUS *);
int			Game_BonusSetVal(GAMEEDITBONUS *,UINT,...);
int			Game_BonusCopy(GAMEEDITBONUS *,GAMEEDITBONUS *);
void			Game_BonusReleaseAll(NODE *);
void			Game_BonusRelease(GAMEEDITBONUS *,BOOL);

void			Game_BonusIncreaseNumber(WCHAR *,int,BOOL,BOOL);
WCHAR*			Game_BonusGetPositiveSign(WCHAR *);
BOOL			Game_BonusIsEmptyNumber(WCHAR *);
void			Game_BonusShiftNumber(WCHAR *,BOOL);
void			Game_BonusCleanNumber(WCHAR *,BOOL,BOOL);
void			Game_BonusCleanFloat(WCHAR *);

// �算� Synchronisation ��������������������������������������������������

void			Game_Synchronize_Level(int,XML_ATTR *,XML_ATTR *,XML_NODE *);

void			Game_SynchronizeAll(void);
int			Game_SynchronizeAll_Init(HWND,RECT *,int,void *);
BOOL			Game_SynchronizeAll_Draw(UINT,DRAWITEMSTRUCT *,void *);
int			Game_SynchronizeAll_Ok(HWND,void *);

// �算� Mots-cl�s ��������������������������������������������������������

void			Game_Tags(void);
INT_PTR CALLBACK	Game_TagsProc(HWND,UINT,WPARAM,LPARAM);

void			Game_TagsToggle(HWND,GAMETAGS *);
void			Game_TagsSelected(HWND,GAMETAGS *);
void			Game_TagsAdd(HWND,GAMETAGS *);
void			Game_TagsEdit(HWND,GAMETAGS *);
void			Game_TagsRemove(HWND,GAMETAGS *);
void			Game_TagsMove(HWND,GAMETAGS *,BOOL);
BOOL			Game_TagsExists(WCHAR *,NODE *,GAMEDATA *);
int			Game_TagsPopulateList(HWND,UINT,NODE *,BOOL,BOOL);

void			Game_TagsMeasureItem(HWND,MEASUREITEMSTRUCT *);
void			Game_TagsDrawItemBuffered(DRAWITEMSTRUCT *);
void			Game_TagsDrawItem(DRAWITEMSTRUCT *);

// �算� Talents ����������������������������������������������������������

void			Game_Talents();

INT_PTR CALLBACK	Game_TalentsProc(HWND,UINT,WPARAM,LPARAM);
int CALLBACK		Game_TalentsSort(GAMETALENTSTRUCT *,GAMETALENTSTRUCT *,GAMETALENTSCONTEXT *);
void			Game_TalentsTip(HWND,NMLVGETINFOTIP *,GAMETALENTSCONTEXT *);
void			Game_TalentsCheck(HWND,int,BOOL,GAMETALENTSCONTEXT *);

// �算� Valeurs ����������������������������������������������������������

int			Game_EditSetValue(HWND,WCHAR *,XML_ATTR *,int,int);
int			Game_EditSetValue_Init(HWND,RECT *,int,void *);
BOOL			Game_EditSetValue_Draw(UINT,DRAWITEMSTRUCT *,void *);
int			Game_EditSetValue_Ok(HWND,void *);

WCHAR*			Game_EditValue(HWND,WCHAR *,UINT,...);
INT_PTR CALLBACK	Game_EditValueProc(HWND,UINT,WPARAM,LPARAM);
BOOL			Game_EditValueBuildList(HWND,BOOL,GAMEEDITVALUE *);
int CALLBACK		Game_EditValueCmp(GAMEDATA *,GAMEDATA *,GAMEEDITVALUE *);
void			Game_EditValueToggleBoostersFilters(HWND,UINT,UINT,GAMEEDITVALUE *);
BOOL			Game_EditValueIsBoosterValid(WCHAR *,UINT64,BOOL);
BOOL			Game_EditValueInitBoostersFilters(HWND,GAMEEDITVALUE *);
void			Game_EditValueSetBoostersFilters(HWND,UINT,UINT64);
void			Game_EditValueUpdateBoostersFilters(HWND,UINT,UINT64,GAMEEDITVALUE *);
void			Game_EditValueDrawBoostersFilter(DRAWITEMSTRUCT *);
void			Game_EditValueDrawSeparator(DRAWITEMSTRUCT *);
void			Game_EditValueSizeObject(HWND,UINT);
void			Game_EditValueDrawObject(WCHAR *,WCHAR *,DRAWITEMSTRUCT *);
void			Game_EditValueSelectLB(HWND,GAMEEDITVALUE *);
void			Game_EditValueSelectLV(HWND,GAMEEDITVALUE *);
void			Game_EditValueSelectListEntry(HWND,WCHAR *);
GAMEDATA*		Game_EditValueGetSelected(HWND,BOOL,GAMEEDITVALUE *);
void			Game_EditValueUpdateBooster(HWND,DWORD,BOOL,GAMEEDITVALUE *);
int			Game_EditValueSave(HWND,GAMEEDITVALUE *);
void			Game_EditValueClose(HWND,INT_PTR,GAMEEDITVALUE *);

// �算� Capacit�s ��������������������������������������������������������

void			Game_Abilities(void);
INT_PTR CALLBACK	Game_AbilitiesProc(HWND,UINT,WPARAM,LPARAM);

void			Game_AbilitiesSaveValues(HWND,GAMEABILITY *);
void			Game_AbilitiesDraw(DRAWITEMSTRUCT *,GAMEABILITY *);

#endif

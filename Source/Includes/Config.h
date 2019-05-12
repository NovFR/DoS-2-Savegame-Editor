
//<<>-<>>---------------------------------------------------------------------()
/*
	Configuration
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _CONFIG_INCLUDE
#define _CONFIG_INCLUDE

#include "Locale.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#define CONFIG_THIS_VERSION	1

enum {
	CONFIG_TYPE_TEXT = 0,
	CONFIG_TYPE_UINT,
	CONFIG_TYPE_BOOL
};

enum {
	CONFIG_IDENT_SAVEONEXIT_V1 = 1, // Don't change order
	CONFIG_IDENT_LOCALENAME_V1,
	CONFIG_IDENT_GAME_V1,
	CONFIG_IDENT_PROFILE_V1,
	CONFIG_IDENT_SHOWHIDDENTAGS_V1,
	CONFIG_IDENT_RUNESGROUPS_V1,
	CONFIG_IDENT_RUNESVIEW_V1,
	CONFIG_IDENT_SKILLSGROUPS_V1,
	CONFIG_IDENT_SKILLSVIEW_V1,
	CONFIG_IDENT_BOOSTERSGROUPS_V1,
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct CONFIG {
	FILEHEADER		header;
	//--- Config
	BOOL			bSaveOnExit;
	//--- Locale
	WCHAR*			pszLocaleName;
	//--- Jeu
	WCHAR*			pszTempPath;
	WCHAR*			pszLarianPath;
	UINT			uGame;
	WCHAR*			pszProfile;
	//--- Edition
	BOOL			bShowHiddenTags;
	BOOL			bRunesGroups;
	UINT			uRunesView;
	BOOL			bSkillsGroups;
	UINT			uSkillsView;
	BOOL			bBoostersGroups;
} CONFIG;

typedef struct CONFIGENTRY {
	NODE			node;
	UINT			uIdent;
	union {
		WCHAR*		pszValue;
		UINT		uValue;
		BOOL		bValue;
	};
} CONFIGENTRY;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

void			Config_Load(CONFIG *);
BOOL			Config_Save(BOOL,CONFIG *);
int			Config_WriteEntry(HANDLE,UINT,UINT,void *);

int			Config_Defaults(CONFIG *);
void			Config_Release(CONFIG *);

#endif

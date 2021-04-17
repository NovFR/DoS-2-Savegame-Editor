
//<<>-<>>---------------------------------------------------------------------()
/*
	Définitions pour les localisations
									      */
//()-------------------------------------------------------------------<<>-<>>//

#ifndef _LOCALE_INCLUDE
#define _LOCALE_INCLUDE


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Définitions							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

enum {
	LOCALE_TYPE_APPLICATION			= 0,
	LOCALE_TYPE_MISC,
	LOCALE_TYPE_MISC_WRITE,
	LOCALE_TYPE_GAME
};

#include "Lists.h"

#define FOREACH_AppText(LOCALE) \
		LOCALE(MENU_FILE), \
		LOCALE(MENU_RECENT), \
		LOCALE(MENU_EDIT), \
		LOCALE(MENU_ADVANCED), \
		LOCALE(MENU_DEFINITIONS), \
		LOCALE(MENU_DB_REBUILD), \
		LOCALE(MENU_SETTINGS), \
		LOCALE(MENU_HELP), \
		LOCALE(MENU_LIST), \
		LOCALE(MENU_TVEDIT), \
		LOCALE(MENU_TVINSERT), \
		LOCALE(MENU_INVENTORY), \
		LOCALE(MENU_COPYTOCLIPBOARD), \
		LOCALE(IDM_OPENSAVEGAME), \
		LOCALE(IDM_OPENSAVEAS), \
		LOCALE(IDM_INFOS), \
		LOCALE(IDM_CLOSE), \
		LOCALE(IDM_WRITESAVEGAME), \
		LOCALE(IDM_WRITESAVEAS), \
		LOCALE(IDM_RECENTREMOVEOBSOLETE), \
		LOCALE(IDM_RECENTREMOVEALL), \
		LOCALE(IDM_CONVERTER), \
		LOCALE(IDM_QUIT), \
		LOCALE(IDM_SHOWCHARTREE), \
		LOCALE(IDM_SHOWMETATREE), \
		LOCALE(IDM_SHOWSAVETREE), \
		LOCALE(IDM_REMOVEMODS), \
		LOCALE(IDM_CONFIGMODIFY), \
		LOCALE(IDM_CONFIGLANGUAGE), \
		LOCALE(IDM_CONFIGSAVE), \
		LOCALE(IDM_CONFIGSAVEONEXIT), \
		LOCALE(IDM_ABOUT), \
		LOCALE(IDM_INVSELECTED), \
		LOCALE(IDM_INVEDIT), \
		LOCALE(IDM_INVNAME), \
		LOCALE(IDM_INVBOOSTERS), \
		LOCALE(IDM_INVBONUSES), \
		LOCALE(IDM_INVRUNES), \
		LOCALE(IDM_INVSYNCHRO), \
		LOCALE(IDM_INVNODES), \
		LOCALE(IDM_INVBACKPACK), \
		LOCALE(IDM_INVBACKPACKOPEN), \
		LOCALE(IDM_INVBACKPACKCLOSE), \
		LOCALE(IDM_INVBACKPACKCLOSEALL), \
		LOCALE(IDM_INVALL), \
		LOCALE(IDM_INVSYNCHROALL), \
		LOCALE(IDM_INVSELECT), \
		LOCALE(IDM_CANCEL), \
		LOCALE(IDM_LIST_SELECTED), \
		LOCALE(IDM_LIST_EXPAND), \
		LOCALE(IDM_LIST_COLLAPSE), \
		LOCALE(IDM_LIST_SELECTALL), \
		LOCALE(IDM_LIST_SELECTNONE), \
		LOCALE(IDM_LIST_SELECTINVERT), \
		LOCALE(IDM_TV_INSERTXML), \
		LOCALE(IDM_TV_INSERTATTR), \
		LOCALE(IDM_TV_REMOVE), \
		LOCALE(IDM_TV_MODIFY), \
		LOCALE(IDM_TV_SEARCH), \
		LOCALE(IDM_DB_REBUILDICONS), \
		LOCALE(IDM_DB_REBUILDITEMSROOTS), \
		LOCALE(IDM_DB_REBUILDITEMSTEMPLATES), \
		LOCALE(IDM_DB_REBUILDITEMSSTATS), \
		LOCALE(IDM_DB_REBUILDITEMSLOCALIZATION), \
		LOCALE(IDM_DB_REBUILDLANGUAGES), \
		LOCALE(IDM_DB_OPTIMIZE), \
		LOCALE(IDM_COPYDISPLAYNAME), \
		LOCALE(IDM_COPYSTATSNAME), \
		LOCALE(IDM_COPYTEMPLATENAME), \
		LOCALE(MENUKEY_OPENSAVEGAME), \
		LOCALE(MENUKEY_WRITESAVEGAME), \
		LOCALE(MENUKEY_CONVERTER), \
		LOCALE(MENUKEY_QUIT), \
		LOCALE(MENUKEY_SHOWCHARTREE), \
		LOCALE(MENUKEY_INVNAME), \
		LOCALE(MENUKEY_INVNODES), \
		LOCALE(MENUKEY_INVBACKPACKCLOSE), \
		LOCALE(MENUKEY_TVINSERTXML), \
		LOCALE(MENUKEY_TVINSERTATTR), \
		LOCALE(MENUKEY_TVREMOVE), \
		LOCALE(MENUKEY_TVMODIFY), \
		LOCALE(MENUKEY_TVSEARCH), \
		LOCALE(KEY_SHIFT), \
		LOCALE(KEY_CTRL), \
		LOCALE(KEY_ALT), \
		LOCALE(KEY_RETURN), \
		LOCALE(KEY_DEL), \
		LOCALE(KEY_F1), \
		LOCALE(KEY_F2), \
		LOCALE(KEY_F3), \
		LOCALE(KEY_F4), \
		LOCALE(KEY_F5), \
		LOCALE(KEY_F6), \
		LOCALE(KEY_F7), \
		LOCALE(KEY_F8), \
		LOCALE(KEY_F9), \
		LOCALE(KEY_F10), \
		LOCALE(KEY_F11), \
		LOCALE(KEY_F12), \
		LOCALE(TEXT_FILE_TYPE), \
		LOCALE(TEXT_ORIGIN_IFAN), \
		LOCALE(TEXT_ORIGIN_LOHSE), \
		LOCALE(TEXT_ORIGIN_REDPRINCE), \
		LOCALE(TEXT_ORIGIN_SEBILLE), \
		LOCALE(TEXT_ORIGIN_BEAST), \
		LOCALE(TEXT_ORIGIN_FANE), \
		LOCALE(TEXT_CLASS_FIGHTER), \
		LOCALE(TEXT_CLASS_WIZARD), \
		LOCALE(TEXT_CLASS_KNIGHT), \
		LOCALE(TEXT_CLASS_CLERIC), \
		LOCALE(TEXT_CLASS_BATTLEMAGE), \
		LOCALE(TEXT_CLASS_WAYFARER), \
		LOCALE(TEXT_CLASS_SHADOWBLADE), \
		LOCALE(TEXT_CLASS_INQUISITOR), \
		LOCALE(TEXT_CLASS_METAMORPH), \
		LOCALE(TEXT_CLASS_CONJURER), \
		LOCALE(TEXT_CLASS_RANGER), \
		LOCALE(TEXT_CLASS_WITCH), \
		LOCALE(TEXT_CLASS_ROGUE), \
		LOCALE(TEXT_CLASS_ENCHANTER), \
		LOCALE(TEXT_CHR_NAME), \
		LOCALE(TEXT_CHR_ORIGIN), \
		LOCALE(TEXT_CHR_NOORIGIN), \
		LOCALE(TEXT_CHR_RACE), \
		LOCALE(TEXT_CHR_CLASS), \
		LOCALE(TEXT_CHR_LEVEL), \
		LOCALE(TEXT_CHR_FOR), \
		LOCALE(TEXT_CHR_DEX), \
		LOCALE(TEXT_CHR_INT), \
		LOCALE(TEXT_CHR_CON), \
		LOCALE(TEXT_CHR_MEM), \
		LOCALE(TEXT_CHR_PER), \
		LOCALE(TEXT_CHR_LIFE), \
		LOCALE(TEXT_CHR_PHYSICAL), \
		LOCALE(TEXT_CHR_MAGICAL), \
		LOCALE(TEXT_CHR_EXP), \
		LOCALE(TEXT_CHR_NEXTLEVEL), \
		LOCALE(TEXT_CHR_ELF), \
		LOCALE(TEXT_CHR_DWARF), \
		LOCALE(TEXT_CHR_HUMAN), \
		LOCALE(TEXT_CHR_UNDEAD), \
		LOCALE(TEXT_CHR_UNDEAD_HUMAN), \
		LOCALE(TEXT_CHR_UNDEAD_DWARF), \
		LOCALE(TEXT_CHR_UNDEAD_ELF), \
		LOCALE(TEXT_CHR_UNDEAD_LIZARD), \
		LOCALE(TEXT_CHR_LIZARD), \
		LOCALE(TEXT_CHR_MAP), \
		LOCALE(TEXT_CHR_DAMAGES), \
		LOCALE(TEXT_CHR_HEAL), \
		LOCALE(TEXT_CHR_KILLS), \
		LOCALE(TEXT_CHR_LABEL), \
		LOCALE(TEXT_CHR_VALUE), \
		LOCALE(TEXT_PTS_ATTRIBUTE), \
		LOCALE(TEXT_PTS_COMBAT), \
		LOCALE(TEXT_PTS_CIVIL), \
		LOCALE(TEXT_PTS_TALENT), \
		LOCALE(TEXT_BUTTON_ABILITIES), \
		LOCALE(TEXT_BUTTON_TAGS), \
		LOCALE(TEXT_BUTTON_TALENTS), \
		LOCALE(TEXT_BUTTON_SKILLS), \
		LOCALE(TEXT_BUTTON_STATS), \
		LOCALE(TEXT_BUTTON_MENU), \
		LOCALE(TEXT_INV_NAV), \
		LOCALE(TEXT_INV_BAGS), \
		LOCALE(TEXT_INV_EQUIPPED), \
		LOCALE(TEXT_INV_ITEMS), \
		LOCALE(TEXT_OBJ_COMMON), \
		LOCALE(TEXT_OBJ_UNCOMMON), \
		LOCALE(TEXT_OBJ_RARE), \
		LOCALE(TEXT_OBJ_EPIC), \
		LOCALE(TEXT_OBJ_LEGENDARY), \
		LOCALE(TEXT_OBJ_UNIQUE), \
		LOCALE(TEXT_OBJ_DIVINE), \
		LOCALE(TEXT_OBJ_EQUIPPED), \
		LOCALE(TEXT_OBJ_GENERATED), \
		LOCALE(TEXT_OBJ_CUSTOM), \
		LOCALE(TEXT_OBJ_LEVEL), \
		LOCALE(TEXT_OBJ_AMOUNT), \
		LOCALE(TEXT_OBJ_ITEMS_SINGLE), \
		LOCALE(TEXT_OBJ_ITEMS_MULTIPLE), \
		LOCALE(TEXT_OBJ_EMPTY), \
		LOCALE(TEXT_OBJ_STOLEN), \
		LOCALE(TEXT_INFOS_MISC), \
		LOCALE(TEXT_INFOS_TEAM), \
		LOCALE(TEXT_INFOS_MODS), \
		LOCALE(TEXT_INFOS_SAVEGAME), \
		LOCALE(TEXT_INFOS_VERSION), \
		LOCALE(TEXT_INFOS_DIFFICULTY), \
		LOCALE(TEXT_INFOS_GAMETIME), \
		LOCALE(TEXT_INFOS_DATE), \
		LOCALE(TEXT_INFOS_CHARACTER), \
		LOCALE(TEXT_INFOS_DATEFMT), \
		LOCALE(TEXT_INFOS_HOURS), \
		LOCALE(TEXT_INFOS_MINUTES), \
		LOCALE(TEXT_INFOS_SECONDS), \
		LOCALE(TEXT_MODS_INFO), \
		LOCALE(TEXT_MODS_GROUP_CRITICAL), \
		LOCALE(TEXT_MODS_GROUP_LARIAN), \
		LOCALE(TEXT_MODS_GROUP_USER), \
		LOCALE(TEXT_DIALOG_SAVE_DOS), \
		LOCALE(TEXT_DIALOG_SAVE_DOSDE), \
		LOCALE(TEXT_DIALOG_SAVE_LOCATION), \
		LOCALE(TEXT_DIALOG_TITLE_SETVALUE), \
		LOCALE(TEXT_DIALOG_TITLE_NAME), \
		LOCALE(TEXT_DIALOG_TITLE_AMOUNT), \
		LOCALE(TEXT_DIALOG_TITLE_BOOSTERS), \
		LOCALE(TEXT_DIALOG_TITLE_BONUSES), \
		LOCALE(TEXT_DIALOG_TITLE_RUNES), \
		LOCALE(TEXT_DIALOG_TITLE_SYNCHRONIZE), \
		LOCALE(TEXT_DIALOG_TITLE_SYNCHRONIZEALL), \
		LOCALE(TEXT_DIALOG_TITLE_LOG), \
		LOCALE(TEXT_DIALOG_TITLE_SKILLS), \
		LOCALE(TEXT_DIALOG_TITLE_TEMPLATE), \
		LOCALE(TEXT_DIALOG_TITLE_OWNERSHIP), \
		LOCALE(TEXT_DIALOG_TITLE_ITEMSELECT), \
		LOCALE(TEXT_DIALOG_INFO_SETVALUE), \
		LOCALE(TEXT_DIALOG_INFO_NAME), \
		LOCALE(TEXT_DIALOG_INFO_AMOUNT), \
		LOCALE(TEXT_DIALOG_INFO_BOOSTERS), \
		LOCALE(TEXT_DIALOG_INFO_BONUSES), \
		LOCALE(TEXT_DIALOG_INFO_RUNES), \
		LOCALE(TEXT_DIALOG_INFO_SYNCHRONIZE), \
		LOCALE(TEXT_DIALOG_INFO_SYNCHRONIZEALL), \
		LOCALE(TEXT_DIALOG_INFO_EMPTYBOOSTERS), \
		LOCALE(TEXT_DIALOG_INFO_EMPTYBONUSES), \
		LOCALE(TEXT_DIALOG_INFO_TEMPLATE), \
		LOCALE(TEXT_DIALOG_INFO_OWNERSHIP), \
		LOCALE(TEXT_DIALOG_BUTTON_ADD), \
		LOCALE(TEXT_DIALOG_BUTTON_EDIT), \
		LOCALE(TEXT_DIALOG_BUTTON_REMOVE), \
		LOCALE(TEXT_DIALOG_OBJECT_NEWLEVEL), \
		LOCALE(TEXT_DIALOG_OBJECT_LEVEL), \
		LOCALE(TEXT_DIALOG_OBJECT_ORIGINALTEMPLATE), \
		LOCALE(TEXT_DIALOG_OBJECT_CURRENTTEMPLATE), \
		LOCALE(TEXT_DIALOG_OBJECT_TEMPLATETYPE), \
		LOCALE(TEXT_DIALOG_OBJECT_RESETTEMPLATE), \
		LOCALE(TEXT_DIALOG_OBJECT_COPYTEMPLATE), \
		LOCALE(TEXT_DIALOG_OBJECT_SELECTTEMPLATE), \
		LOCALE(TEXT_DIALOG_OBJECT_OWNERSHIP), \
		LOCALE(TEXT_DIALOG_OBJECT_CHAROWNED), \
		LOCALE(TEXT_DIALOG_OBJECT_PARTYOWNED), \
		LOCALE(TEXT_DIALOG_OBJECT_MAYBESTOLEN), \
		LOCALE(TEXT_DIALOG_AMOUNT), \
		LOCALE(TEXT_DIALOG_EDIT), \
		LOCALE(TEXT_DIALOG_RUNE), \
		LOCALE(TEXT_DIALOG_BOOSTER_TITLE_ADD), \
		LOCALE(TEXT_DIALOG_BOOSTER_TITLE_EDIT), \
		LOCALE(TEXT_DIALOG_BOOSTER_TARGET), \
		LOCALE(TEXT_DIALOG_BOOSTER_TYPE), \
		LOCALE(TEXT_DIALOG_BOOSTER_COLUMN_ID), \
		LOCALE(TEXT_DIALOG_BOOSTER_COLUMN_TEXT), \
		LOCALE(TEXT_DIALOG_BOOSTER_COLUMN_TYPE), \
		LOCALE(TEXT_DIALOG_BOOSTER_COLUMN_SLOT), \
		LOCALE(TEXT_DIALOG_RUNE_TITLE_ADD), \
		LOCALE(TEXT_DIALOG_RUNE_TITLE_EDIT), \
		LOCALE(TEXT_DIALOG_RUNE_COLUMN_NAME), \
		LOCALE(TEXT_DIALOG_RUNE_COLUMN_BONUS1), \
		LOCALE(TEXT_DIALOG_RUNE_COLUMN_BONUS2), \
		LOCALE(TEXT_DIALOG_RUNE_COLUMN_BONUS3), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_MASTER), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_FLAME), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_FROST), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_THUNDER), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_ROCK), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_VENOM), \
		LOCALE(TEXT_DIALOG_RUNE_GROUP_MISC), \
		LOCALE(TEXT_DIALOG_TAG_TITLE_ADD), \
		LOCALE(TEXT_DIALOG_TAG_TITLE_EDIT), \
		LOCALE(TEXT_DIALOG_TAG_PROTECTED), \
		LOCALE(TEXT_DIALOG_TAG_SHOWHIDDEN), \
		LOCALE(TEXT_DIALOG_TAG_MODIFYPROTECTED), \
		LOCALE(TEXT_DIALOG_SYNCHRO_ENABLE), \
		LOCALE(TEXT_DIALOG_SYNCHRO_EQUIPPED), \
		LOCALE(TEXT_DIALOG_SYNCHRO_LOWERLEVELS), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_ANY), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_AMULET), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_RING), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_BOOTS), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_HELMET), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_BELT), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_GLOVES), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_PANTS), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS_UPPERBODY), \
		LOCALE(TEXT_DIALOG_FILTER_SHIELDS), \
		LOCALE(TEXT_DIALOG_FILTER_WEAPONS_ANY), \
		LOCALE(TEXT_DIALOG_FILTER_ABILITIES), \
		LOCALE(TEXT_DIALOG_FILTER_ARMORS), \
		LOCALE(TEXT_DIALOG_FILTER_MAGICAL_ARMOR), \
		LOCALE(TEXT_DIALOG_FILTER_PHYSICAL_ARMOR), \
		LOCALE(TEXT_DIALOG_FILTER_RUNE_SLOT), \
		LOCALE(TEXT_DIALOG_FILTER_PRIMARY_ATTRIBUTE), \
		LOCALE(TEXT_DIALOG_FILTER_SECONDARY_ATTRIBUTE), \
		LOCALE(TEXT_DIALOG_FILTER_RESISTANCE), \
		LOCALE(TEXT_DIALOG_FILTER_SKILL), \
		LOCALE(TEXT_DIALOG_FILTER_IMMUNITY), \
		LOCALE(TEXT_DIALOG_FILTER_REFLECT), \
		LOCALE(TEXT_DIALOG_FILTER_SPECIAL), \
		LOCALE(TEXT_DIALOG_FILTER_DAMAGE), \
		LOCALE(TEXT_DIALOG_FILTER_LIFESTEAL), \
		LOCALE(TEXT_DIALOG_FILTER_STATUS), \
		LOCALE(TEXT_DIALOG_XP_INFO), \
		LOCALE(TEXT_DIALOG_TV_INFO), \
		LOCALE(TEXT_DIALOG_TV_VERIFY), \
		LOCALE(TEXT_DIALOG_TV_NOERROR) ,\
		LOCALE(TEXT_DIALOG_TV_EMPTY), \
		LOCALE(TEXT_DIALOG_TV_ABORTVERIFICATION), \
		LOCALE(TEXT_DIALOG_TV_INSERT_BEFORE), \
		LOCALE(TEXT_DIALOG_TV_INSERT_AFTER), \
		LOCALE(TEXT_DIALOG_TV_INSERT_ASFIRSTCHILD), \
		LOCALE(TEXT_DIALOG_TV_ATTR_INFO), \
		LOCALE(TEXT_DIALOG_TV_ATTR_NAME), \
		LOCALE(TEXT_DIALOG_TV_ATTR_VALUE), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_INFO), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_TITLE), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_NEXT), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_NOTFOUND), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_EOF), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_STRING), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_NODE), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_ATTR), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_OPTIONS), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_ANY), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_CASE), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_OPACITY), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_HISTORY), \
		LOCALE(TEXT_DIALOG_TV_SEARCH_HISTORYCLEAR), \
		LOCALE(TEXT_DIALOG_DEFS_WIPE), \
		LOCALE(TEXT_DIALOG_DEFS_CONFIRM), \
		LOCALE(TEXT_DIALOG_DEFS_CONFIRMNOWIPE), \
		LOCALE(TEXT_DIALOG_DEFS_WAIT), \
		LOCALE(TEXT_DIALOG_DEFS_DONE), \
		LOCALE(TEXT_DIALOG_DEFS_FAILED), \
		LOCALE(TEXT_ABILITIES_SPECIAL), \
		LOCALE(TEXT_ABILITIES_SOURCE), \
		LOCALE(TEXT_ABILITIES_TITLE), \
		LOCALE(TEXT_ABILITIES_WEAPONS), \
		LOCALE(TEXT_ABILITIES_TWOHANDED), \
		LOCALE(TEXT_ABILITIES_DUALWIELD), \
		LOCALE(TEXT_ABILITIES_RANGED), \
		LOCALE(TEXT_ABILITIES_ONEHANDED), \
		LOCALE(TEXT_ABILITIES_DEFENSE), \
		LOCALE(TEXT_ABILITIES_PAINREFLECTION), \
		LOCALE(TEXT_ABILITIES_LEADERSHIP), \
		LOCALE(TEXT_ABILITIES_PERSEVERANCE), \
		LOCALE(TEXT_ABILITIES_ABILITIES), \
		LOCALE(TEXT_ABILITIES_WARFARE), \
		LOCALE(TEXT_ABILITIES_AIR), \
		LOCALE(TEXT_ABILITIES_HUNTING), \
		LOCALE(TEXT_ABILITIES_ROGUES), \
		LOCALE(TEXT_ABILITIES_EARTH), \
		LOCALE(TEXT_ABILITIES_WATER), \
		LOCALE(TEXT_ABILITIES_SUMMONING), \
		LOCALE(TEXT_ABILITIES_POLYMORPH), \
		LOCALE(TEXT_ABILITIES_DEATH), \
		LOCALE(TEXT_ABILITIES_FIRE), \
		LOCALE(TEXT_ABILITIES_PERSONNALITY), \
		LOCALE(TEXT_ABILITIES_LUCK), \
		LOCALE(TEXT_ABILITIES_BARTERING), \
		LOCALE(TEXT_ABILITIES_PERSUASION), \
		LOCALE(TEXT_ABILITIES_HABILITY), \
		LOCALE(TEXT_ABILITIES_LOREMASTER), \
		LOCALE(TEXT_ABILITIES_TELEKINESIS), \
		LOCALE(TEXT_ABILITIES_MISDEED), \
		LOCALE(TEXT_ABILITIES_LOCKPICKING), \
		LOCALE(TEXT_ABILITIES_SNEAKING), \
		LOCALE(TEXT_ABILITIES_UNKNOWN), \
		LOCALE(TEXT_BONUS_TITLE_TARGET), \
		LOCALE(TEXT_BONUS_TITLE_LIST), \
		LOCALE(TEXT_BONUS_TITLE_VALUE), \
		LOCALE(TEXT_BONUS_TITLE_DAMAGETYPE), \
		LOCALE(TEXT_BONUS_TITLE_REFLECT), \
		LOCALE(TEXT_BONUS_TITLE_SKILLS), \
		LOCALE(TEXT_BONUS_TITLE_COLORS), \
		LOCALE(TEXT_BONUS_INFO_TARGET), \
		LOCALE(TEXT_BONUS_INFO_LIST), \
		LOCALE(TEXT_BONUS_INFO_VALUE), \
		LOCALE(TEXT_BONUS_INFO_DAMAGETYPE), \
		LOCALE(TEXT_BONUS_INFO_REFLECT), \
		LOCALE(TEXT_BONUS_INFO_SKILLS), \
		LOCALE(TEXT_BONUS_INFO_COLORS), \
		LOCALE(TEXT_BONUS_TYPE_ARMOR), \
		LOCALE(TEXT_BONUS_TYPE_WEAPON), \
		LOCALE(TEXT_BONUS_TYPE_SHIELD), \
		LOCALE(TEXT_BONUS_PRIMARIES), \
		LOCALE(TEXT_BONUS_OTHERS), \
		LOCALE(TEXT_BONUS_COLORS), \
		LOCALE(TEXT_BONUS_DAMAGEBOOST), \
		LOCALE(TEXT_BONUS_ARMOR_MAGICAL), \
		LOCALE(TEXT_BONUS_ARMOR_PHYSICAL), \
		LOCALE(TEXT_BONUS_CRITICALCHANCE), \
		LOCALE(TEXT_BONUS_ACCURACY), \
		LOCALE(TEXT_BONUS_DODGE), \
		LOCALE(TEXT_BONUS_VITALITY), \
		LOCALE(TEXT_BONUS_DAMAGETYPE), \
		LOCALE(TEXT_BONUS_MINDAMAGE), \
		LOCALE(TEXT_BONUS_MAXDAMAGE), \
		LOCALE(TEXT_BONUS_CLEAVEANGLE), \
		LOCALE(TEXT_BONUS_CLEAVEPERCENTAGE), \
		LOCALE(TEXT_BONUS_WEAPONRANGE), \
		LOCALE(TEXT_BONUS_LIFESTEAL), \
		LOCALE(TEXT_BONUS_BLOCKING), \
		LOCALE(TEXT_BONUS_MOVEMENT), \
		LOCALE(TEXT_BONUS_INITIATIVE), \
		LOCALE(TEXT_BONUS_HASREFLECTION), \
		LOCALE(TEXT_BONUS_SKILL), \
		LOCALE(TEXT_BONUS_ABILITIES), \
		LOCALE(TEXT_BONUS_UTILITY), \
		LOCALE(TEXT_BONUS_RUNESLOTS), \
		LOCALE(TEXT_BONUS_VALUE), \
		LOCALE(TEXT_BONUS_RESISTANCES), \
		LOCALE(TEXT_BONUS_PIERCINGRESISTANCE), \
		LOCALE(TEXT_BONUS_PHYSICALRESISTANCE), \
		LOCALE(TEXT_BONUS_FIRERESISTANCE), \
		LOCALE(TEXT_BONUS_WATERRESISTANCE), \
		LOCALE(TEXT_BONUS_EARTHRESISTANCE), \
		LOCALE(TEXT_BONUS_AIRRESISTANCE), \
		LOCALE(TEXT_BONUS_POISONRESISTANCE), \
		LOCALE(TEXT_BONUS_SORTBYNAME), \
		LOCALE(TEXT_BONUS_SORTBYSCHOOL), \
		LOCALE(TEXT_BONUS_SORTBYPOINTS), \
		LOCALE(TEXT_BONUS_REFLECTIONPERCENT), \
		LOCALE(TEXT_BONUS_RESULT), \
		LOCALE(TEXT_BONUS_TARGET), \
		LOCALE(TEXT_BONUS_TARGETTYPE), \
		LOCALE(TEXT_BONUS_SKILL_COLUMN_NAME), \
		LOCALE(TEXT_BONUS_SKILL_COLUMN_SCHOOL), \
		LOCALE(TEXT_BONUS_SKILL_COLUMN_POINTS), \
		LOCALE(TEXT_BONUS_GROUP_NAMES), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_0), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_1), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_2), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_3), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_4), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_5), \
		LOCALE(TEXT_BONUS_GROUP_POINTS_6), \
		LOCALE(TEXT_BONUS_GROUP_SOURCE_0), \
		LOCALE(TEXT_BONUS_GROUP_SOURCE_1), \
		LOCALE(TEXT_BONUS_GROUP_SOURCE_2), \
		LOCALE(TEXT_BONUS_GROUP_SOURCE_3), \
		LOCALE(TEXT_BONUS_GROUP_MEMORY_0), \
		LOCALE(TEXT_BONUS_GROUP_MEMORY_1), \
		LOCALE(TEXT_BONUS_GROUP_MEMORY_2), \
		LOCALE(TEXT_BONUS_GROUP_MEMORY_3), \
		LOCALE(TEXT_BONUS_SORT_NAMES), \
		LOCALE(TEXT_BONUS_SORT_SCHOOLS), \
		LOCALE(TEXT_BONUS_SORT_POINTS), \
		LOCALE(TEXT_BONUS_SORT_SOURCE), \
		LOCALE(TEXT_BONUS_SORT_MEMORY), \
		LOCALE(TEXT_REFLECTION_ACTIVATED), \
		LOCALE(TEXT_REFLECTION_DEACTIVATED), \
		LOCALE(TEXT_DAMAGE_PHYSICAL), \
		LOCALE(TEXT_DAMAGE_PIERCING), \
		LOCALE(TEXT_DAMAGE_FIRE), \
		LOCALE(TEXT_DAMAGE_AIR), \
		LOCALE(TEXT_DAMAGE_WATER), \
		LOCALE(TEXT_DAMAGE_EARTH), \
		LOCALE(TEXT_DAMAGE_POISON), \
		LOCALE(TEXT_TITLE_TREE), \
		LOCALE(TEXT_TITLE_INFO), \
		LOCALE(TEXT_TITLE_REQUEST), \
		LOCALE(TEXT_TITLE_WARNING), \
		LOCALE(TEXT_TITLE_ERROR), \
		LOCALE(TEXT_TITLE_LINK), \
		LOCALE(TEXT_VIEW_GROUPS), \
		LOCALE(TEXT_VIEW_DETAILS), \
		LOCALE(TEXT_VIEW_ICONS), \
		LOCALE(TEXT_VIEW_TILES), \
		LOCALE(TEXT_REMOVE_OBSOLETE), \
		LOCALE(TEXT_REMOVE_OBSNONE), \
		LOCALE(TEXT_REMOVE_ALL), \
		LOCALE(TEXT_LOADING), \
		LOCALE(TEXT_LOADING_EXTRACT), \
		LOCALE(TEXT_LOADING_CONVERTLSF), \
		LOCALE(TEXT_SAVING), \
		LOCALE(TEXT_SAVING_CONVERTLSX), \
		LOCALE(TEXT_SAVING_CREATE), \
		LOCALE(TEXT_SAVEGAME_TITLE_PROFILE), \
		LOCALE(TEXT_SAVEGAME_TITLE_LIST), \
		LOCALE(TEXT_SAVEGAME_INFO_PROFILE), \
		LOCALE(TEXT_SAVEGAME_INFO_LIST), \
		LOCALE(TEXT_ITEM_IS_ACCESSORY), \
		LOCALE(TEXT_ITEM_IS_ARMOR), \
		LOCALE(TEXT_ITEM_IS_WEAPON), \
		LOCALE(TEXT_TALENTS_INFO), \
		LOCALE(TEXT_TALENTS_SET), \
		LOCALE(TEXT_TALENTS_UNSET), \
		LOCALE(TEXT_TALENTS_UNKNOWN), \
		LOCALE(TEXT_BUILDING_TREE), \
		LOCALE(TEXT_THREAD_RUNNING), \
		LOCALE(TEXT_NODES_REMOVE), \
		LOCALE(TEXT_MODIFIED_SAVEGAME), \
		LOCALE(TEXT_MODIFIED_SGMETA), \
		LOCALE(TEXT_OVERRIDE_ATTRIBUTE), \
		LOCALE(TEXT_OVERRIDE_ABILITIES), \
		LOCALE(TEXT_CONFIG_WRITTEN), \
		LOCALE(TEXT_CONFIG_OPENEDSAVEGAME), \
		LOCALE(TEXT_CONFIG_TITLE), \
		LOCALE(TEXT_CONFIG_PATHS), \
		LOCALE(TEXT_CONFIG_TEMP), \
		LOCALE(TEXT_CONFIG_SAVEGAMES), \
		LOCALE(TEXT_CONFIG_DEFAULT), \
		LOCALE(TEXT_CONFIG_MODIFY), \
		LOCALE(TEXT_CONFIG_LANG), \
		LOCALE(TEXT_CONFIG_LANG_INTERFACE), \
		LOCALE(TEXT_CONFIG_LANG_GAME), \
		LOCALE(TEXT_CONFIG_EDITION), \
		LOCALE(TEXT_CONFIG_OVERRIDE), \
		LOCALE(TEXT_CONFIG_RUNES), \
		LOCALE(TEXT_CONFIG_SKILLS), \
		LOCALE(TEXT_CONFIG_BOOSTERS), \
		LOCALE(TEXT_CONFIG_ITEMSDISPLAYNAME), \
		LOCALE(TEXT_CONFIG_ITEMSRESOLVE), \
		LOCALE(TEXT_CONFIG_WINDOWS), \
		LOCALE(TEXT_CONFIG_WINDOWS_USEDEFAULTPOS), \
		LOCALE(TEXT_CONFIG_WINDOWS_USEDEFAULTSIZE), \
		LOCALE(TEXT_CONFIG_WINDOWS_MAIN), \
		LOCALE(TEXT_CONFIG_WINDOWS_TREE), \
		LOCALE(TEXT_CONFIG_DISPLAY), \
		LOCALE(TEXT_SKILLS_SELECT_INFO), \
		LOCALE(TEXT_SKILLS_OPTIONS), \
		LOCALE(TEXT_SKILLS_OPTIONS_ISLEARNED), \
		LOCALE(TEXT_SKILLS_OPTIONS_ISACTIVATED), \
		LOCALE(TEXT_SKILLS_OPTIONS_ZEROMEMORY), \
		LOCALE(TEXT_SKILLS_OPTIONS_INFO), \
		LOCALE(TEXT_LINK), \
		LOCALE(TEXT_COPY), \
		LOCALE(TEXT_OPEN), \
		LOCALE(TEXT_ABOUT), \
		LOCALE(TEXT_OK), \
		LOCALE(TEXT_CANCEL), \
		LOCALE(TEXT_DONE), \
		LOCALE(TEXT_UNKNOWN), \
		LOCALE(TEXT_UNDEFINED), \
		LOCALE(TEXT_HELP), \
		LOCALE(TEXT_SAVE), \
		LOCALE(TEXT_CLOSE), \
		LOCALE(TEXT_RESET), \
		LOCALE(TEXT_DEBUG_TREE), \
		LOCALE(TEXT_DONTASKAGAIN), \
		LOCALE(TEXT_DEVTOOL), \
		LOCALE(TEXT_ERR_NOTIMPLEMENTED), \
		LOCALE(TEXT_ERR_SHOWERROR), \
		LOCALE(TEXT_ERR_HIDEERROR), \
		LOCALE(TEXT_ERR_RUNNING), \
		LOCALE(TEXT_ERR_COMCTRL), \
		LOCALE(TEXT_ERR_CONFIG), \
		LOCALE(TEXT_ERR_GETMSG), \
		LOCALE(TEXT_ERR_WINDOW_CLASS), \
		LOCALE(TEXT_ERR_WINDOW_CREATE), \
		LOCALE(TEXT_ERR_SHORTCUTS), \
		LOCALE(TEXT_ERR_INVALIDSHORTCUT), \
		LOCALE(TEXT_ERR_TREE_CREATE), \
		LOCALE(TEXT_ERR_DIALOG), \
		LOCALE(TEXT_ERR_LOADING), \
		LOCALE(TEXT_ERR_SAVING), \
		LOCALE(TEXT_ERR_MEMORY), \
		LOCALE(TEXT_ERR_THREAD), \
		LOCALE(TEXT_ERR_XML_UNEXPECTED_EOF), \
		LOCALE(TEXT_ERR_XML_HEADER_UNKNOWN), \
		LOCALE(TEXT_ERR_XML_TAG_EMPTY), \
		LOCALE(TEXT_ERR_XML_TAG_CLOSE_MISMATCH), \
		LOCALE(TEXT_ERR_XML_ATTR_EMPTY), \
		LOCALE(TEXT_ERR_LOCALE), \
		LOCALE(TEXT_ERR_LOCALE_MISC), \
		LOCALE(TEXT_ERR_LOCALE_MISSINGAPP), \
		LOCALE(TEXT_ERR_LOCALE_LANG), \
		LOCALE(TEXT_ERR_LOCALE_ENUM), \
		LOCALE(TEXT_ERR_LOCALE_SELECT), \
		LOCALE(TEXT_ERR_GENBOOSTUNKNOWNNODE), \
		LOCALE(TEXT_ERR_GENBOOSTEMPTY), \
		LOCALE(TEXT_ERR_GENBOOSTTOOMANYATTR), \
		LOCALE(TEXT_ERR_GENBOOSTUNKNOWNATTR), \
		LOCALE(TEXT_ERR_GENBOOSTATTREMPTY), \
		LOCALE(TEXT_ERR_INVENTORY),\
		LOCALE(TEXT_ERR_DATAFILE), \
		LOCALE(TEXT_ERR_TAG_ADD), \
		LOCALE(TEXT_ERR_TAG_EXISTS), \
		LOCALE(TEXT_ERR_MISSINGCONVERTER), \
		LOCALE(TEXT_ERR_DIVINE), \
		LOCALE(TEXT_ERR_NOTCOMPATIBLE), \
		LOCALE(TEXT_ERR_METADATAS), \
		LOCALE(TEXT_ERR_NOMEMORY), \
		LOCALE(TEXT_ERR_NOMETAFILE), \
		LOCALE(TEXT_ERR_TASKRUNNING), \
		LOCALE(TEXT_ERR_OPENEDSAVEGAME), \
		LOCALE(TEXT_ERR_CONFIGLOAD), \
		LOCALE(TEXT_ERR_CONFIGWRITE), \
		LOCALE(TEXT_ERR_CONFIGWRITEONEXIT), \
		LOCALE(TEXT_ERR_CONFIGCUSTOMSAVELOCATION), \
		LOCALE(TEXT_ERR_CONFIGCUSTOMTEMPLOCATION), \
		LOCALE(TEXT_ERR_CONFIGCUSTOMLOCATION), \
		LOCALE(TEXT_ERR_CONFIGSAVELOCATION), \
		LOCALE(TEXT_ERR_CONFIGTEMPLOCATION), \
		LOCALE(TEXT_ERR_CONFIGCHANGE), \
		LOCALE(TEXT_ERR_LSV_LOAD), \
		LOCALE(TEXT_ERR_LSV_LOADEX), \
		LOCALE(TEXT_ERR_LSV_UNKNOWN), \
		LOCALE(TEXT_ERR_LSV_VERSION), \
		LOCALE(TEXT_ERR_LSV_EMPTY), \
		LOCALE(TEXT_ERR_LSV_FILESLIST), \
		LOCALE(TEXT_ERR_LSV_CRC32), \
		LOCALE(TEXT_ERR_LSF_LOAD), \
		LOCALE(TEXT_ERR_LSF_LOADEX), \
		LOCALE(TEXT_ERR_LSF_UNKNOWN), \
		LOCALE(TEXT_ERR_LSF_ATTRIBUTE), \
		LOCALE(TEXT_ERR_LSF_NOXMLROOT), \
		LOCALE(TEXT_ERR_LSA_UNPACK), \
		LOCALE(TEXT_ERR_LSA_METHOD), \
		LOCALE(TEXT_ERR_SKILLS_ICONS), \
		LOCALE(TEXT_ERR_SKILLS_BOOK), \
		LOCALE(TEXT_ERR_SKILLS_LIST), \
		LOCALE(TEXT_ERR_SKILLS_BUILD), \
		LOCALE(TEXT_ERR_TV_ATTREXISTS), \
		LOCALE(TEXT_ERR_TV_NODEBADATTR), \
		LOCALE(TEXT_ERR_TV_NODEHASNOATTR), \
		LOCALE(TEXT_ERR_TV_BADNODE), \
		LOCALE(TEXT_ERR_TV_BADATTR), \
		LOCALE(TEXT_ERR_TV_BADVALUE), \
		LOCALE(TEXT_ERR_TEMPLATE_INVALID), \
		LOCALE(TEXT_ERR_TEMPLATE_TYPEMISMATCH), \
		LOCALE(TEXT_LAST) // NOT A REAL TEXT, FOR INTERNAL USE ONLY !

#define LOCALE_EnumValue(ID) ID
#define LOCALE_EnumCharToWide(ID) L ## ID
#define LOCALE_EnumString(ID) LOCALE_EnumCharToWide(#ID)

enum {
	FOREACH_AppText(LOCALE_EnumValue)
};


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Structures							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

typedef struct LOCALE_PARSER {
	int		iResult;
	int		iFlags;
	char*		pszDataBasePath;
	sqlite3*	db;
	WCHAR*		pszFilePath;
	WCHAR*		pszLastError;
	WCHAR*		pszSQLiteErr;
} LOCALE_PARSER;

typedef struct LOCALE_MISC {
	sqlite3*	db;
} LOCALE_MISC;

typedef struct LOCALE_TEXT {
	LONG		lUID;
	WCHAR*		pszUID;
	WCHAR*		pszText;
} LOCALE_TEXT;

typedef struct LOCALE_ENUM {
	NODE		node;
	HICON		hIcon;
	WCHAR*		pszName;
	WCHAR		szLang[LOCALE_NAME_MAX_LENGTH];
} LOCALE_ENUM;


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Prototypes							  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

int			Locale_Enum(HWND,WCHAR *,NODE *,UINT);
void			Locale_EnumRelease(NODE *);

int			Locale_Load(HWND,WCHAR *,WCHAR *,LONG,void **,WCHAR **);
void			Locale_Unload(LONG,void **,WCHAR **);

int			Locale_Language(void *,int,char **,char **);
int			Locale_Parse(void *,int,char **,char **);
BOOL			Locale_QueryID(sqlite3 *,WCHAR *,WCHAR *,WCHAR *,WCHAR *[],UINT);
void			Locale_QueryBonuses(sqlite3 *,WCHAR *,WCHAR *[]);
void			Locale_QueryTalent(sqlite3 *,DWORD,UINT,WCHAR **,WCHAR **);

WCHAR*			Locale_GetText(LONG);
LONG			Locale_GetTextUID(LOCALE_TEXT *,WCHAR *);
void			Locale_SetText(LOCALE_TEXT *,LONG,WCHAR *);

#endif

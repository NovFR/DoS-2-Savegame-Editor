
//<<>-<>>---------------------------------------------------------------------()
/*
	GlEd
									      */
//()-------------------------------------------------------------------<<>-<>>//


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Includes								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

#include "Application.h"
#include "Game.h"
#include "Menus.h"


// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤ Données								  ¤¤¤ //
// ¤¤¤									  ¤¤¤ //
// ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ //

APPLICATION		App;
WCHAR*			TextsIds[] = { FOREACH_AppText(LOCALE_EnumString) };
UINT			ExpTable[] = { 2000,8000,20000,40000,70000,112000,168000,240000,340000,479000,672000,941000,1315000,1834000,2556000,3559000,4954000,6893000,9588000,13334000,18540000,25777000,35836000,49818000,69253000,96268000,133818000,186013000,258564000,359410000,499586000,694430000,965264000,1341723000,0 };
UINT			TextsAttr[6] = { TEXT_CHR_FOR, TEXT_CHR_DEX, TEXT_CHR_INT, TEXT_CHR_CON, TEXT_CHR_MEM, TEXT_CHR_PER };
UINT			TextsPts[4] = { TEXT_PTS_ATTRIBUTE, TEXT_PTS_COMBAT, TEXT_PTS_CIVIL, TEXT_PTS_TALENT };

APPICON			AppIcons[APP_MAX_ICONS] = {

				{ APP_ICON_APPLICATION, 1, 16, 16 },
				{ APP_ICON_APP_LARGE, 1, 48, 48 },
				{ APP_ICON_PLAYER, 12, 24, 24 },
				{ APP_ICON_CHECKED, 13, 16, 16 },
				{ APP_ICON_UNCHECKED, 14, 16, 16 },
				{ APP_ICON_WEAPON, 17, 15, 16 },
				{ APP_ICON_ARMOR, 18, 15, 16 },
				{ APP_ICON_ACCESSORY, 19, 15, 16 },
				{ APP_ICON_BACKGROUND, 20, 52, 52 },
				{ APP_ICON_FRAME, 21, 48, 48 },
				{ APP_ICON_EMPTY, 22, 48, 48 },
				{ APP_ICON_BACKPACK, 24, 48, 48 },
				{ APP_ICON_BACK, 25, 48, 48 },
				{ APP_ICON_REMOVE, 27, 16, 16 },
				{ APP_ICON_HIDDEN, 28, 16, 16 },
				{ APP_ICON_PROTECTED, 29, 16, 16 },
				{ APP_ICON_SELECT_ALL, 30, 16, 16 },
				{ APP_ICON_SELECT_NONE, 31, 16, 16 },
				{ APP_ICON_SELECT_INVERT, 32, 16, 16 },
				{ APP_ICON_PROFILE, 33, 16, 16 },
				{ APP_ICON_FOLDER, 34, 16, 16 },
				{ APP_ICON_SAVEGAME, 35, 16, 16 },
				{ APP_ICON_ACTION_OFF, 40, 24, 24 },
				{ APP_ICON_ACTION_ON, 41, 24, 24 },
				{ APP_ICON_SOURCE_OFF, 42, 20, 20 },
				{ APP_ICON_SOURCE_ON, 43, 20, 20 },
				{ APP_ICON_INFO_EMPTY, 50, 48, 48 },
				{ APP_ICON_INFO_SAVEGAME, 51, 48, 48 },
				{ APP_ICON_INFO_VERSION, 52, 48, 48 },
				{ APP_ICON_INFO_DIFFICULTY, 53, 48, 48 },
				{ APP_ICON_INFO_GAMETIME, 54, 48, 48 },
				{ APP_ICON_INFO_DATE, 55, 48, 48 },
				{ APP_ICON_INFO_CHARACTER, 56, 48, 48 },
				{ APP_ICON_INFO_NOTCHARACTER, 57, 48, 48 },
				{ APP_ICON_INFO_LARIAN, 58, 48, 48 },
				{ APP_ICON_INFO_MOD, 59, 48, 48 },
				{ APP_ICON_STAT_STR, 80, 24, 24 },
				{ APP_ICON_STAT_DEX, 81, 24, 24 },
				{ APP_ICON_STAT_INT, 82, 24, 24 },
				{ APP_ICON_STAT_CON, 83, 24, 24 },
				{ APP_ICON_STAT_MEM, 84, 24, 24 },
				{ APP_ICON_STAT_PER, 85, 24, 24 },
				{ APP_ICON_STAT_POINT, 86, 24, 24 },
				{ APP_ICON_STAT_XP, 87, 24, 24 },
				{ APP_ICON_STAT_NEXT, 88, 24, 24 },
				{ APP_ICON_STAT_MAGICAL, 89, 24, 24 },
				{ APP_ICON_STAT_PHYSICAL, 90, 24, 24 },
				{ APP_ICON_STAT_LIFE, 91, 24, 24 },
				{ APP_ICON_SKILLACTIVATED, 44, 64, 64 },

			};

//--- Menus ---

CUSTOMMENUTEMPLATE	MainMenu[] = {

				// type, id, flags, keyId, iconId, specialFlags
				{ MENU_POPUP, MENU_FILE, 0 },
					{ MENU_ITEM, IDM_OPENSAVEGAME, 0, MENUKEY_OPENSAVEGAME, 100 },
					{ MENU_SUBPOPUP, MENU_RECENT },
						{ MENU_SEPARATOR },
						{ MENU_ITEM, IDM_RECENTREMOVEOBSOLETE, MFS_GRAYED },
						{ MENU_ITEM, IDM_RECENTREMOVEALL, MFS_GRAYED },
						{ MENU_STOP },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_OPENSAVEAS, 0, 0, 102 },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_INFOS, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_WRITESAVEGAME, MFS_GRAYED, MENUKEY_WRITESAVEGAME, 101 },
					{ MENU_ITEM, IDM_WRITESAVEAS, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CLOSE, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_QUIT, 0, MENUKEY_QUIT },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_EDIT, 0 },
					{ MENU_ITEM, IDM_CONVERTER, 0, MENUKEY_CONVERTER },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_ADVANCED, 0 },
					{ MENU_ITEM, IDM_SHOWCHARTREE, MFS_GRAYED, MENUKEY_SHOWCHARTREE },
					{ MENU_ITEM, IDM_SHOWMETATREE, MFS_GRAYED },
					{ MENU_ITEM, IDM_SHOWSAVETREE, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_REMOVEMODS, MFS_GRAYED },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_SETTINGS, 0 },
					{ MENU_ITEM, IDM_CONFIGMODIFY },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONFIGLANGUAGE },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONFIGSAVE, 0, 0, 101 },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONFIGSAVEONEXIT, 0, 0, 0, MENU_FLAG_CHECKBOX },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_HELP, 0},
					{ MENU_ITEM, IDM_ABOUT },
					{ MENU_STOP },
				{ MENU_STOP }

			};

CUSTOMMENUTEMPLATE	InvMenu[] = {

				{ MENU_POPUP, MENU_INVENTORY },
					{ MENU_ITEM, IDM_INVSELECTED, MFS_GRAYED, 0, 0, MENU_FLAG_SEPARATOR },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVEDIT },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVNAME, 0, MENUKEY_INVNAME },
					{ MENU_ITEM, IDM_INVBOOSTERS },
					{ MENU_ITEM, IDM_INVBONUSES },
					{ MENU_ITEM, IDM_INVRUNES },
					{ MENU_ITEM, IDM_INVSYNCHRO },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVNODES, 0, MENUKEY_INVNODES },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVBACKPACK, MFS_GRAYED, 0, 0, MENU_FLAG_SEPARATOR },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVBACKPACKOPEN },
					{ MENU_ITEM, IDM_INVBACKPACKCLOSE, 0, MENUKEY_INVBACKPACKCLOSE },
					{ MENU_ITEM, IDM_INVBACKPACKCLOSEALL },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVALL, MFS_GRAYED, 0, 0, MENU_FLAG_SEPARATOR },
					{ MENU_BLANK },
					{ MENU_ITEM, IDM_INVSYNCHROALL },
					{ MENU_BLANK },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CANCEL },
					{ MENU_STOP },
				{ MENU_STOP }

			};

CUSTOMMENUTEMPLATE	SkillsListMenu[] = {

				{ MENU_POPUP, MENU_LIST },
					{ MENU_ITEM, IDM_LIST_SELECTED, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_LIST_EXPAND, MFS_GRAYED },
					{ MENU_ITEM, IDM_LIST_COLLAPSE, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_LIST_SELECTALL, MFS_GRAYED },
					{ MENU_ITEM, IDM_LIST_SELECTNONE, MFS_GRAYED },
					{ MENU_ITEM, IDM_LIST_SELECTINVERT, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CANCEL },
					{ MENU_STOP },
				{ MENU_STOP }

			};

//--- Raccourcis ---

SHORTCUT		Shortcuts[] = {

				{ MENUKEY_OPENSAVEGAME, IDM_OPENSAVEGAME },
				{ MENUKEY_WRITESAVEGAME, IDM_WRITESAVEGAME },
				{ MENUKEY_CONVERTER, IDM_CONVERTER },
				{ MENUKEY_QUIT, IDM_QUIT },
				{ MENUKEY_SHOWCHARTREE, IDM_SHOWCHARTREE },
				{ MENUKEY_INVNAME, IDM_INVNAME },
				{ MENUKEY_INVNODES, IDM_INVNODES },
				{ MENUKEY_INVBACKPACKCLOSE, IDM_INVBACKPACKCLOSE },
				{ 0 }

			};

//--- Divers ---

GAMEQUALITY		Qualities[] = {

				{ L"Common", TEXT_OBJ_COMMON, RGB(0,0,0) },
				{ L"Uncommon", TEXT_OBJ_UNCOMMON, RGB(2,158,1) },
				{ L"Rare", TEXT_OBJ_RARE, RGB(50,190,235) },
				{ L"Epic", TEXT_OBJ_EPIC, RGB(163,70,233) },
				{ L"Legendary", TEXT_OBJ_LEGENDARY, RGB(187,2,111) },
				{ L"Unique", TEXT_OBJ_UNIQUE, RGB(190,159,84) },
				{ L"Divine", TEXT_OBJ_DIVINE, RGB(235,200,8) },
				{ NULL }

			};

GAMEORIGIN		Origins[] = {

				{ L"Ifan", TEXT_ORIGIN_IFAN },
				{ L"Lohse", TEXT_ORIGIN_LOHSE },
				{ L"RedPrince", TEXT_ORIGIN_REDPRINCE },
				{ L"Sebille", TEXT_ORIGIN_SEBILLE },
				{ L"Beast", TEXT_ORIGIN_BEAST },
				{ L"Fane", TEXT_ORIGIN_FANE },
				{ NULL }

			};

WCHAR*			ModsIgnore[] = {

				L"1301db3d-1f54-4e98-9be5-5094030916e4", // Divinity: Original Sin 2
				L"2bd9bdbe-22ae-4aa2-9c93-205880fc6564", // Shared
				L"eedf7638-36ff-4f26-a50a-076b87d53ba0", // Shared_DOS
				NULL

			};

WCHAR*			ModsLarian[] = {

				L"9b45f7e5-d4e2-4fc2-8ef7-3b8e90a5256c", // 8 Action Points
				L"015de505-6e7f-460c-844c-395de6c2ce34", // AS_BlackCatPlus
				L"38608c30-1658-4f6a-8adf-e826a5295808", // AS_GrowYourHerbs
				L"423fae51-61e3-469a-9c1f-8ad3fd349f02", // Animal Empathy
				L"f33ded5d-23ab-4f0c-b71e-1aff68eee2cd", // CMP_BarterTweaks
				L"f30953bb-10d3-4ba4-958c-0f38d4906195", // Combat Randomizer
				L"68a99fef-d125-4ed0-893f-bb6751e52c5e", // Crafter's Kit
				L"2d42113c-681a-47b6-96a1-d90b3b1b07d3", // Fort Joy Magic Mirror
				L"f243c84f-9322-43ac-96b7-7504f990a8f0", // Improved Organisation
				L"d2507d43-efce-48b8-ba5e-5dd136c715a7", // Pet Power
				L"a945eefa-530c-4bca-a29c-a51450f8e181", // Shiny Gear
				L"ec27251d-acc0-4ab8-920e-dbc851e79bb4", // ToggleSpeedAddon
				L"ca32a698-d63e-4d20-92a7-dd83cba7bc56", // Divine Talents
				L"8fe1719c-ef8f-4cb7-84bd-5a474ff7b6c1", // Enhanced Spirit Vision
				L"1273be96-6a1b-4da9-b377-249b98dc4b7e", // AS_RestRestorePoints
				L"af4b3f9c-c5cb-438d-91ae-08c5804c1983", // AS_Resturrect
				L"3da57b9d-8b41-46c7-a33c-afb31eea38a3", // ArmorSets
				NULL

			};

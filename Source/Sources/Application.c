
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
				{ APP_ICON_MAN, 10, 80, 100 },
				{ APP_ICON_WOMAN, 11, 80, 100 },
				{ APP_ICON_PLAYER, 12, 24, 24 },
				{ APP_ICON_CHECKED, 13, 16, 16 },
				{ APP_ICON_UNCHECKED, 14, 16, 16 },
				{ APP_ICON_UP, 15, 16, 16 },
				{ APP_ICON_DOWN, 16, 16, 16 },
				{ APP_ICON_WEAPON, 17, 15, 16 },
				{ APP_ICON_ARMOR, 18, 15, 16 },
				{ APP_ICON_ACCESSORY, 19, 15, 16 },
				{ APP_ICON_BACKGROUND, 20, 52, 52 },
				{ APP_ICON_FRAME, 21, 48, 48 },
				{ APP_ICON_EMPTY, 22, 48, 48 },
				{ APP_ICON_EDIT, 23, 16, 16 },
				{ APP_ICON_BACKPACK, 24, 48, 48 },
				{ APP_ICON_BACK, 25, 48, 48 },
				{ APP_ICON_MODIFY, 26, 16, 16 },
				{ APP_ICON_REMOVE, 27, 16, 16 },
				{ APP_ICON_HIDDEN, 28, 16, 16 },
				{ APP_ICON_PROTECTED, 29, 16, 16 },
				{ APP_ICON_SELECT_ALL, 30, 16, 16 },
				{ APP_ICON_SELECT_NONE, 31, 16, 16 },
				{ APP_ICON_SELECT_INVERT, 32, 16, 16 },
				{ APP_ICON_PROFILE, 33, 16, 16 },
				{ APP_ICON_FOLDER, 34, 16, 16 },
				{ APP_ICON_SAVEGAME, 35, 16, 16 },
				{ APP_ICON_PORTRAIT_IFAN, 80, 80, 100 },
				{ APP_ICON_PORTRAIT_LOHSE, 81, 80, 100 },
				{ APP_ICON_PORTRAIT_REDPRINCE, 82, 80, 100 },
				{ APP_ICON_PORTRAIT_SEBILLE, 83, 80, 100 },
				{ APP_ICON_PORTRAIT_FANE, 84, 80, 100 },
				{ APP_ICON_PORTRAIT_BEAST, 85, 80, 100 },

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
					{ MENU_ITEM, IDM_CLOSE, MFS_GRAYED },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_WRITESAVEGAME, MFS_GRAYED, MENUKEY_WRITESAVEGAME, 101 },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_QUIT, 0, MENUKEY_QUIT },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_EDIT, 0 },
					{ MENU_ITEM, IDM_TREE, MFS_GRAYED, MENUKEY_TREE },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONVERTER, 0, MENUKEY_CONVERTER },
					{ MENU_STOP },
				{ MENU_POPUP, MENU_SETTINGS, 0 },
					{ MENU_ITEM, IDM_CONFIGLANGUAGE },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONFIGSAVE, 0, 0, 101 },
					{ MENU_SEPARATOR },
					{ MENU_ITEM, IDM_CONFIGSAVEONEXIT, 0, 0, 0, MENU_FLAG_CHECKBOX },
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

//--- Raccourcis ---

SHORTCUT		Shortcuts[] = {

				{ MENUKEY_OPENSAVEGAME, IDM_OPENSAVEGAME },
				{ MENUKEY_WRITESAVEGAME, IDM_WRITESAVEGAME },
				{ MENUKEY_CONVERTER, IDM_CONVERTER },
				{ MENUKEY_QUIT, IDM_QUIT },
				{ MENUKEY_TREE, IDM_TREE },
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

GAMEPORTRAIT		Portraits[] = {

				{ L"Ifan", APP_ICON_PORTRAIT_IFAN },
				{ L"Lohse", APP_ICON_PORTRAIT_LOHSE },
				{ L"RedPrince", APP_ICON_PORTRAIT_REDPRINCE },
				{ L"Sebille", APP_ICON_PORTRAIT_SEBILLE },
				{ L"Beast", APP_ICON_PORTRAIT_BEAST },
				{ L"Fane", APP_ICON_PORTRAIT_FANE },
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

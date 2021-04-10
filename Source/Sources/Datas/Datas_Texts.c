
//<<>-<>>---------------------------------------------------------------------()
/*
	Textes fixes
									      */
//()-------------------------------------------------------------------<<>-<>>//

#include "_Global.h"

#if _DEBUG
WCHAR	szTitle[]			= L"GlEd - Debug";
#else
WCHAR	szTitle[]			= L"GlEd";
#endif
WCHAR	szTitleFmt[]			= L"%2 - %1";
WCHAR	szTitleFmtLong[]		= L"%3 - %2 - %1";
WCHAR	szTimeFmt[]			= L"%04d/%02d/%02d %02d:%02d:%02d - ";

WCHAR	szWindowClass[]			= L"GlEdWindowClass32";
WCHAR	szDialogClass[]			= L"GlEdDialog32";
WCHAR	szComboBoxClass[]		= L"COMBOBOX";
WCHAR	szEditClass[]			= L"EDIT";
WCHAR	szListBoxClass[]		= L"LISTBOX";
WCHAR	szButtonClass[]			= L"BUTTON";
WCHAR	szStaticClass[]			= L"STATIC";
WCHAR	szScrollClass[]			= L"SCROLLBAR";
WCHAR	szThemeButton[]			= L"BUTTON";
WCHAR	szThemeProgress[]		= L"PROGRESS";

WCHAR	szRootPath[]			= L".";
WCHAR	szParentPath[]			= L"..";
WCHAR	szWild[]			= L"*.*";
WCHAR	szLF[]				= L"\n";
WCHAR	szZero[]			= L"0";
WCHAR	szSpace[]			= L" ";
WCHAR	szBlank[]			= L"";
WCHAR	sSourcePoint			= L'S';
WCHAR	sActionPoint			= L'A';
WCHAR	szEdit[]			= L"...";
WCHAR	szClassSpace[]			= L" - ";
WCHAR	szGeneric[]			= L"Generic";

WCHAR	szPlayerName[]			= L"%1 (%2) (%3)";
WCHAR	szLevelFmt[]			= L"%u";
WCHAR	szRaceFmt[]			= L"TEXT_CHR_%1";
WCHAR	szClassFmt[]			= L"TEXT_CLASS_%1";

WCHAR	szPlus[]			= L"+";
WCHAR	szEmpty[]			= L"";
WCHAR	szBonusSkillFmt[]		= L"%1: %2";
WCHAR	szBonusPercentFmt[]		= L"%3%2%% %1";
WCHAR	szBonusDegreesFmt[]		= L"%3%2° %1";
WCHAR	szBonusMetersFmt[]		= L"%3%2m %1";
WCHAR	szBonusFmt[]			= L"%3%2 %1";
WCHAR	szBonusDamageTypeFmt[]		= L"%1: %2";
WCHAR	szBonusColorsFmt[]		= L"%1: %2";
WCHAR	szUIDFmt[]			= L"{%s}";

WCHAR	szDebugLogFileName[]		= L"Log.txt";
WCHAR	szConverterEXE[]		= L"C\\ConverterApp.exe";
WCHAR	szDivineEXE[]			= L"C\\divine.exe";
WCHAR	szDivineExtract[]		= L"divine.exe -l %1 -s \"%2\" -i lsv -d \"%3\" -a extract-package -g %4";
WCHAR	szDivineCreate[]		= L"divine.exe -l %1 -s \"%2\" -o lsv -d \"%3\" -a create-package -g %4";
WCHAR	szDivineConvertLSF[]		= L"divine.exe -l %1 -s \"%2\" -i lsf -o lsx -d \"%3\" -a convert-resource -g %4";
WCHAR	szDivineConvertLSX[]		= L"divine.exe -l %1 -s \"%2\" -i lsx -o lsf -d \"%3\" -a convert-resource -g %4";
WCHAR	szTempPath[]			= L"GlEd";
WCHAR	szLarianStudios[]		= L"Larian Studios";
WCHAR	szGameName[]			= L"Divinity Original Sin 2";
WCHAR	szGameNameEE[]			= L"Divinity Original Sin 2 Definitive Edition";
WCHAR	szPlayerProfiles[]		= L"PlayerProfiles";
WCHAR	szSavegames[]			= L"Savegames";
WCHAR	szStory[]			= L"Story";
WCHAR	szMetaLSF[]			= L"meta.lsf";
WCHAR	szMetaLSX[]			= L"meta.lsx";
WCHAR	szGlobalsLSF[]			= L"globals.lsf";
WCHAR	szGlobalsLSX[]			= L"globals.lsx";
WCHAR	szLSVext[]			= L".lsv";
WCHAR	szPNGext[]			= L".png";
WCHAR	szLSVwext[]			= L"*.lsv";
WCHAR	szDefinitiveEdition[]		= L" (Definitive Edition)";
WCHAR	szStandardEdition[]		= L" (Standard Edition)";

WCHAR	szConfigFolderPath[]		= L"Prefs\\";
WCHAR	szTVSearchHistoryPath[]		= L"Prefs\\GlEd.history";
WCHAR	szLastFilesPath[]		= L"Prefs\\GlEd.recents";
WCHAR	szConfigPath[]			= L"Prefs\\GlEd.config";
WCHAR	szItemsDataPath[]		= L"Datas\\Items.txt";
WCHAR	szBoostersDataPath[]		= L"Datas\\Boosters.txt";
WCHAR	szRunesDataPath[]		= L"Datas\\Runes.txt";
WCHAR	szTagsDataPath[]		= L"Datas\\Tags.txt";
WCHAR	szSkillsDataPath[]		= L"Datas\\Skills.txt";
WCHAR	szInsertsDataPath[]		= L"Datas\\Inserts.txt";
WCHAR	szIconsPath[]			= L"Icons\\Items.icl";
WCHAR	szSkillsIconsPath[]		= L"Icons\\Skills.icl";
WCHAR	szRunesIconsPath[]		= L"Icons\\Runes.icl";
WCHAR	szLangPath[]			= L"Locales\\%1.db";
WCHAR	szLangIcoPath[]			= L"Locales\\%1.ico";
WCHAR	szDOS2PNGPath[]			= L"Images\\DOS2.png";
WCHAR	szDOS2EEPNGPath[]		= L"Images\\DOS2EE.png";
WCHAR	szLangDefault[]			= L"en-US";
WCHAR	szWarning[]			= L"Warning";
WCHAR	szConfigErr[]			= L"Unable to set configuration's default values.";
WCHAR	szConfigLoadErr[]		= L"An error occurred while loading the configuration.\nConfiguration will use default values.";
WCHAR	szLocaleFatalErr[]		= L"Unable to load language database.";
WCHAR	szLocaleErr[]			= L"Unable to load language database \"%1\".";
WCHAR	szSQLiteErr[]			= L"%1\n\n[SQLite:%2!i!]: %3";
WCHAR	szLocaleMissingAppErr[]		= L"Translation for %2 is missing in language file \"%1\"\n\nPress OK to continue.\nPress CANCEL to abort.\n\nIn any case, file won't be loaded.";
WCHAR	szLocaleLangErr[]		= L"Unable to obtain language name in database \"%1\".";
WCHAR	szConfigCustSaveLocationErr[]	= L"The savegame location stored in your configuration is not found. Default location will be used.";
WCHAR	szConfigCustTempLocationErr[]	= L"The temp location stored in your configuration is not found. Default location will be used.";
WCHAR	szConfigCustLocationErr[]	= L"The temporary locations stored in your configuration are not found. Default locations will be used.";
WCHAR	szConfigSaveLocationErr[]	= L"An error occurred while creating default savegame location.";
WCHAR	szConfigTempLocationErr[]	= L"An error occurred while creating default temp location.";

WCHAR	szDataBaseRunes[]		= L"runes";
WCHAR	szDataBaseTags[]		= L"tags";
WCHAR	szDataBaseBoosters[]		= L"boosters";
WCHAR	szDataBaseSkills[]		= L"skills";
WCHAR	szDataBaseColors[]		= L"colors";
WCHAR	szDataBaseItems[]		= L"items";

char	szLocaleLangQuery[]		= "SELECT text FROM language WHERE id = 'lang' LIMIT 1";
char	szLocaleAppQuery[]		= "SELECT * FROM application";
WCHAR	szLocaleTextQuery[]		= L"SELECT %2 FROM %1 WHERE id = ? LIMIT 1";

char	szXMLHeader[]			= "?xml";
char	szXMLFullHeader[]		= "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
WCHAR	szXMLEmpty[]			= L"";
WCHAR	szXMLValue[]			= L"%1 = \"%2\"";
WCHAR	szXMLAttr[]			= L"%1";
WCHAR	szXMLAttrId[]			= L"%1[id=%2]";
WCHAR	szXMLSeparator[]		= L"\\";

WCHAR	szXMLregion[]			= L"region";
WCHAR	szXMLnode[]			= L"node";
WCHAR	szXMLid[]			= L"id";
WCHAR	szXMLvalue[]			= L"value";
WCHAR	szXMLhandle[]			= L"handle";
WCHAR	szXMLattribute[]		= L"attribute";
WCHAR	szXMLCharacter[]		= L"Character";
WCHAR	szXMLLevel[]			= L"Level";
WCHAR	szXMLtype[]			= L"type";
WCHAR	szXMLBoost[]			= L"Boost";
WCHAR	szXMLObject[]			= L"Object";
WCHAR	szXMLTarget[]			= L"Target";
WCHAR	szXMLSkills[]			= L"Skills";
WCHAR	szXMLTrue[]			= L"True";
WCHAR	szXMLFalse[]			= L"False";

WCHAR*	szCharactersPath[]		= { L"save",NULL, L"region",L"Characters", L"node",L"Characters", L"children",NULL, L"node",L"CharacterFactory", L"children",NULL, L"node",L"Characters", L"children",NULL, NULL };
WCHAR*	szCharacterStatsPath[]		= { L"children",NULL, L"node",L"Stats", NULL };
WCHAR*	szCharacterCustomDataPath[]	= { L"children",NULL, L"node",L"PlayerData", L"children",NULL, L"node",L"PlayerCustomData", NULL };
WCHAR*	szCharacterPlayerUpgradePath[]	= { L"children",NULL, L"node",L"PlayerUpgrade", NULL };
WCHAR*	szCharacterTagsPath[]		= { L"children",NULL, L"node",L"Tags", L"children",NULL, NULL };
WCHAR*	szCharacterPointsPath[]		= { L"children",NULL, L"node",L"PlayerUpgrade", L"children",NULL, NULL };
WCHAR*	szItemsPath[]			= { L"save",NULL, L"region",L"Items", L"node",L"Items", L"children",NULL, L"node",L"ItemFactory", L"children",NULL, L"node",L"Items", L"children",NULL, NULL };
WCHAR*	szItemGeneratorPath[]		= { L"children",NULL, L"node",L"Generation", L"children",NULL, L"node",L"ItemGeneration", NULL };
WCHAR*	szItemStatsPath[]		= { L"children",NULL, L"node",L"Stats", NULL };
WCHAR*	szItemBasePath[]		= { L"children",NULL, L"node",L"Stats", L"children",NULL, L"node",L"Base", NULL };
WCHAR*	szItemCustomNamePath[]		= { L"children",NULL, L"node",L"CustomDisplayName", NULL };
WCHAR*	szItemCustomDescriptionPath[]	= { L"children",NULL, L"node",L"CustomDescription", NULL };
WCHAR*	szItemBonusesPath[]		= { L"children",NULL, L"node",L"Stats", L"children",NULL, L"node",L"PermanentBoost", NULL };
WCHAR*	szItemReflectionPath[]		= { L"children",NULL, L"node",L"Reflection", NULL };
WCHAR*	szItemReflectionMapPath[]	= { L"children",NULL, L"node",L"Reflection", L"children",NULL, L"node",L"MapValue", NULL };

WCHAR*	szInfoMetaDataPath[]		= { L"save",NULL, L"region",L"MetaData", L"node",L"MetaData", L"children",NULL, L"node",L"MetaData", NULL };
WCHAR*	szInfoGameModdedPath[]		= { L"attribute",L"Modded", NULL };
WCHAR*	szInfoGameDifficultyPath[]	= { L"attribute",L"Difficulty", NULL };
WCHAR*	szInfoGameTimePath[]		= { L"attribute",L"TimeStamp", NULL };
WCHAR*	szInfoGameModsPath[]		= { L"children",NULL, L"node",L"ModuleSettings", L"children",NULL, L"node",L"Mods", L"children",NULL, L"node",L"ModuleShortDesc", NULL };
WCHAR*	szInfoGameVersionsPath[]	= { L"children",NULL, L"node",L"GameVersions", L"children",NULL };
WCHAR*	szInfoGamePartyPath[]		= { L"children",NULL, L"node",L"PartyMetaData", L"children",NULL, NULL };
WCHAR*	szInfoGameDatePath[]		= { L"children",NULL, L"node",L"SaveTime", NULL };

WCHAR*	szSkillManagerPath[]		= { L"children",NULL, L"node",L"SkillManager", NULL };
WCHAR*	szSkillManagerSkillsPath[]	= { L"children",NULL, L"node",L"SkillManager", L"children",NULL, NULL };

WCHAR*	szPartyManagerPath[]		= { L"save",NULL, L"region",L"PartyManager", L"node",L"PartyManager", L"children",NULL, L"node",L"PartyFormation", L"children",NULL, NULL };

WCHAR*	htmlEntities[]			= { L"&apos;",L"'", L"&amp;",L"&", L"&lt;",L"<", L"&gt;",L">", L"&#xa",L"\n", L"&quot;",L"\"", NULL };

# Savegame Editor for Divinity Original Sin II

This program allows you to modify various things in a **Divinity Original Sin II** savegame.

It is technically, more a front end for [Norbyte's LSlib](https://github.com/Norbyte/lslib) than a real savegame editor.


## Language

_English_, _French_, and _Simplified Chinese_ languages are available.

You can create more translations by adding new databases in the **Locales** folder.

Databases use SQLite format. I personnaly use [DB Browser for SQLite](https://sqlitebrowser.org/).

Starting with version 1.1.0.2, language files must match naming convention as described here: [Locale Names](https://docs.microsoft.com/en-us/windows/win32/intl/locale-names).


## Savegame corruption

If the game complain about savegame corruption, disable cloud storage before saving and modifying your savegames.


## Better be safe than sorry

Use a special save to make your modifications. Save your game once then a second time, and only edit that second save. If something goes wrong, you won't loose much progression.


## Known issues

- Using keyboard to (un)select Talents may be messy sometimes.
- Last line of Tags description may be truncated.
- Modifying the original savegame during edition may leads to unpredicable results if metadatas are loaded after (either by displaying informations, removing mods, or opening the tree view)
- Changes done in the tree structures are not always taken into account by other functions and may crash the program.


## Requirements

- Windows 7 or better.
- [Norbyte's LSlib](https://github.com/Norbyte/lslib) (used to load and write savegames) (included)

The game is **not** required. This program does not communicate with the game or modify any files related to the game (aside the savegames obviously). Don't touch your game installation if you have an issue with this program, post an [issue](https://github.com/NovFR/DoS-2-Savegame-Editor/issues) instead.

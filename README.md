# Savegame Editor for Divinity Original Sin II

This program allows you to modify various things in a Divinity Original Sin II savegame.

It is technically, more a front end for Norbyte's LSlib than a real savegame editor.


## Language

Only English and French languages are available at this time.

You can create more translations by adding new databases in the Locales folder.

Databases use SQLite format. I personnaly use [DB Browser for SQLite](https://sqlitebrowser.org/).

It is highly suggested (but not required) to use [ISO 639-2 format](https://en.wikipedia.org/wiki/List_of_ISO_639-2_codes) for databases filenames.


## Savegame corruption

If the game complain about savegame corruption, disable cloud storage before saving and modifying your savegames.


## Better be safe than sorry

Use a special save to make your modifications. Save your game once then a second time, and only edit that second save. If something goes wrong, you won't loose much progression.


## Known issues

- You can only select the amount of an item if the amount is known (in DoS2 DE, this means the stockpile have to be greater than 1)
- Using keyboard to (un)select Talents may be messy sometimes.
- Last line of Tags description may be truncated.
- Modifying the original savegame during edition may leads to unpredicable results if metadatas are loaded after (either by displaying informations, removing mods, or opening the tree view)
- Savegames are sorted by system time, not by savegame time (order may differs from the game selection)


## Requirements

Base application requires Windows 7 or better.

It requires [Norbyte LSlib](https://github.com/Norbyte/lslib) to load and modify savegames (included in Binaries/C/)

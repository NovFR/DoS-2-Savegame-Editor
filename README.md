# Savegame Editor for Divinity Original Sin II

This program allows you to modify various things in a Divinity Original Sin II savegame.

It is technically, more a front end for Norbyte's LSlib than a real savegame editor for now.

# Language

Only English and French languages are available at this time.

You can create more translations by adding new databases in the Locales folder.

Databases use SQLite format. I personnaly use DB Browser for SQLite (https://sqlitebrowser.org/)

It is highly suggested (but not required) to use ISO 639-2 format (https://fr.wikipedia.org/wiki/Liste_des_codes_ISO_639-2) for databases filenames.

# Savegame corruption

If the game complain about savegame corruption, disable Cloud storage before saving and modifying your savegames.

# Better be safe than sorry

Use a special save to make your modifications. Save your game once then a second time, and only edit that second save. If something goes wrong, you won't loose much progression.

# Known issues

o You can only select the amount of an item if the <amount> tag exists in the XML structure.
o Using keyboard to (un)select Talents may be messy sometimes.
o Last line of Tags description may be truncated.

# Requirements

Base application requires Windows 7 or better.

It requires Norbyte LSlib to load and modify savegames (included in Binaries/C/)

Links:
https://github.com/Norbyte/lslib
http://larian.com/forums/ubbthreads.php?ubb=showflat&Number=636561

SSBDS
=====

News/Updates
------------

UPDATE 1/31/09: I really like how far I've gotten with this, and I'm nearing the point where I can use lot of help (character creation). But, junior year has finally kicked in and I don't have enough free time to work often on this project. I don't plan to be doing much on this until after APs (Mid-May), but come end of year and over the summer I hope to progress this to the point where a few in-person instructional sessions will allow for faster production and let the game to hit beta-testing status.

Sorry for how poorly worded that was; it's late and it's hard to put what I'm trying to say into words.

NOTE: Just in case anyone actually reads this, you can safely ignore the above message. Although Spartan97 CLAIMS he is not going to have time to work on this, he actually will. And he'll probably decide to spend some of it on this project. Even if he doesn't, I might. So we can and probably will keep having updates even before APs are over. 
-ComputerDruid

Directions
----------

1. Go to [the PALib website][main] and download/install the latest version of PAlib. This is a pain, but necessary
	- Helpful [installation guide][guide]
2. read up and learn as much about PAlib as you can. Otherwise this whole project will be completely over your head; it's different from most CS projects
	- [main page][main] for PAlib
	- [PAlib forum][forum]
	- PAlib [function documentation][docs]
		- Useful to help you find methods you need, and the syntax for those you can't remember.
	- PAlib [wiki/tutorial][tutorial]. 
		- Very outdated, but useful nevertheless.
		- Has instructions to help you install PALib

[main]:     http://forum.palib.info/ "PALib Main Page" 
[forum]:    http://forum.palib.info/index.php?action=forum "PALib Forum"
[docs]:     http://palib.info/Doc/PAlibDoc%20Eng/modules.html "PALib Function Documentation"
[tutorial]: http://www.palib.info/wiki/doku.php "PALib Tutorial/Wiki"
[guide]:    http://forum.palib.info/index.php?topic=6319.0 "Installation guide for PAlib"

Files
-----

- /SSBDS/data/
	- all the .raw files (sound FX) are stored here. They are then converted into and loaded as .h files.
- /SSBDS/source/
	- all of the source code for the project
	- The code itself should be pretty well commented, but for more information try looking at the PAlib examples, wiki, etc.
	- gfx/
		- Just the graphics files that are included by main.cpp (in .c form)
		- Modifying this is slightly more difficult, but see the palib examples and wiki if you want to try to learn
	- main.cpp: Contains the main game logic, game types, and initialization
	- circle.cpp: represents a circle, usually part of a hitbox
	- display.cpp
	- effect.cpp: a visual effect that will stay on the screen for a period of time
	- fighter.cpp: the base class for the characters 
		- Implements many of the character functions, along with the AI and input
	- Characters:
		- ike.cpp: Ike, a swordsman from fire emblem
		- kirby.cpp: Kirby, a fluffy pufffball
		- mario.cpp: Mario, our favorite plumber (unless you like a different plumber)
		- mewtwo.cpp: Mewtwo, the powerful psychic pokemon that is the result of genetic experimentation
		- sandbag.cpp: Sandbag, a simple dummy character for use in training mode
	- global.cpp: implements generic functions that are useful throughout the code
	- hitbox.cpp: represents a hitbox
	- knockback.cpp
	- projectiles.cpp: represents a flying/falling/other disconnected-from-player attack
	- scoreboard.cpp: draws? and keeps track of deaths/kills/SDs
	- stage.cpp: represents a stage. This is extended by various other stages
	- stages.cpp: the code and data for the various stages
	- computerserver/
		- Contains a computer server to host DS's that I made. It isn't used anymore, but I included it nevertheless.
	- hiboxmaker.java
		- a GUI I made to create hitboxes for characters... slightly outdated; don't worry about it for now.
- /SSBDS/includes/
	- contains the definitions for the classes that are implemented (see above list)
- /SSBDS/SSBDS\_Files/
	- External files that are loaded by the ROM
	- must be loaded onto your flashcard along with the ROM
	- sprites/
		- .bin files for character sprites; very large files (I'm working on cutting them down but it involves editing the main.cpp, too)
	- gifs/
		- gif files loaded to be screen backgrounds
	- music/
		- mp3 files for background music
	- hitboxes/
		- variables for hitboxes of characters (txt files) 
- /SSBDS/Makefile
	- the file containing build instructions. Set up PAlib, then CD into the /SSBDS directory and type "make" to compile the ROM.
- /SSBDS/SSBDS.NDS
	- the ROM file... pretty simple. Just load this onto your flashcard if you just want to play the game.
	- You also need to load /SSBDS\_Files into the root of your flashcard

Everything else is just framework for the makefile or not used; don't worry about the other files unless you really know what you're doing and how to use them.

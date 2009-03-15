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

1. Go to www.palib.info and download/install the latest version of PAlib. This is a pain, but necessary
2. read up and learn as much about PAlib as you can. Otherwise this whole project will be completely over your head; it's different that most CS projects
	- [main page][main] for PAlib
	- [PAlib fourm][forum]
	- PAlib [function documentation][docs]
	- PAlib [wiki/tutorial][tutorial]. Very outdated, but useful nevertheless.

[main]:     http://forum.palib.info/ "PALib Main Page" 
[forum]:    http://forum.palib.info/index.php?action=forum "PALib Forum"
[docs]:     http://palib.info/Doc/PAlibDoc%20Eng/modules.html "PALib Function Documentation"
[tutorial]: http://www.palib.info/wiki/doku.php "PALib Tutorial/Wiki"

Now here are what all the important files are/do in this project.

- /SSBDS/data
	- all the .raw files (sound FX) are stored here. They are then converted into and loaded as .h files.
- /SSBDS/source
	- all of the source code for the project
	- /gfx
		- Just the graphics files that are included by main.cpp (in .c form)
		- Modifying this is slightly more difficult, but see the palib examples and wiki if you want to try to learn
	- /main.cpp
		- The main code file that is compiled by the makefile. This is the code that you will be editing. 
		- The file itself should be pretty well commented, but for more information try looking at the PAlib examples, wiki, etc.
	- /computerserver
		- Contains a computer server to host DS's that I made. It isn't used anymore, but I included it nevertheless.
	- /hiboxmaker.java
		- a GUI I made to create hitboxes for characters... slightly outdated; don't worry about it for now.
- /SSBDS/SSBDS_Files
	- External files that are loaded by the ROM
	- must be loaded onto your flashcard along with the ROM
	- /sprites
		- .bin files for character sprites; very large files (I'm working on cutting them down but it involves editing the main.cpp, too)
	- /gifs
		- gif files loaded to be screen backgrounds
	- /music
		- mp3 files for background music; not working ATM
	- /hitboxes
		- variables for hitboxes of characters (txt files) 
- /SSBDS/makefile
	- the file containing build instructions. Set up PAlib, then CD into the /SSBDS directory and type "make" to compile the ROM.
- /SSBDS/SSBDS.NDS
	- the ROM file... pretty simple. Just load this onto your flashcard if you just want to play the game.
	- You also need to load /SSBDS_Files into the root of your flashcard

Everything else is just framework for the makefile or not used; don't worry about the other files unless you really know what you're doing and how to use them.

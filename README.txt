It seems that everything is set up. Feel free to edit, but ***LEAVE DESCRIPTIVE COMMITS*** I don't want to have to spend forever figuring out who edited what when. Also, check recent commits before you edit, to get a feel for what is being done, what needs to be done, and what hasn't been started yet. Also check out the wiki for a simple TODO list (thanks to Patrick!).

As for directions:

First go to www.palib.info and download/install the latest version of PAlib. This is a pain, but necessary
Second, read up and learn as much about PAlib as you can. Otherwise this whole project will be completely over your head; it's different that most CS projects
	- http://forum.palib.info/ 				--> main page for PAlib
	- http://forum.palib.info/index.php?action=forum	--> PAlib fourm
	- http://palib.info/Doc/PAlibDoc%20Eng/modules.html	--> PAlib function documentation
	- http://www.palib.info/wiki/doku.php			--> PAlib wiki/tutorial. Very outdated, but useful nevertheless.

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

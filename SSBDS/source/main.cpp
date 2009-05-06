// Game designed by TJgames of TJHSST
// Head Developer(s): Jeremy Vercillo, Daniel Johnson
// Assistant Developer(s): Dylan Ladwig, Tyler Haines, Patrick Stalcup
// Head Advisor: Andrew Kim
// 6/08 - ???

// The rights to Super Smash Bros. and all the related characters, stages, and items
// belong to Nintendo and other 3rd party companies. This is a fan made game; it not
// made for profit, just for fun.

//#define DEBUG_ON // turns on printing of information to screen
// turns certain features on and off

//PALib:
#include <PA9.h> // DS functions that we use come from here
#include "nds_loader_arm9.h" // loading externa ROMs

//C++ Library Functions:
#include <math.h> // math!
#include <vector> // vectors!
#include <string> // strings!
#include <sstream> // int to string
#include <stdio.h> // standard file functions
#include <stdlib.h> // standard C functions
#include <map> // maps

//Data Files:
#include "gfx/all_gfx.c" // all image files

//Allow us to skip us to skip the std:: prefix on tandard types
using namespace std;
//FIXME: should probably only use individual things, the above is considered bad practice
//example: using std::vector;

//Project classes:
#include "global.h"
#include "effect.h"
#include "stages.h"
#include "projectiles.h" // projectiles
#include "fighters.h" // individual characters
#include "fighter.h"
#include "display.h"
#include "scoreboard.h"

//Global variables and definitions for what they mean:
#define CAMERATYPE_FOLLOWUSER 0
#define CAMERATYPE_FOLLOWALL 1
int cameratype = CAMERATYPE_FOLLOWUSER;
// the kind of camera used in battles

#define GAMEMODE_TIME 0
#define GAMEMODE_STOCK 1
int gamemode = GAMEMODE_STOCK;
int timelimit = 2;
int stocklimit = 3;
int sdcost = 1;
// game settings

vector<Fighter*> players;
// stores all fighters for playing a match

vector<Projectile> projectiles;
// stores all projectiles

Display display = Display();
// stores all visual effects

//FIXME still quite a few things use these
#define effects (*display.getEffects())
#define scrollx display.scrollx
#define scrolly display.scrolly
#define score (*display.score)

map<int, int> customcontrols; // custom control mapping
bool shieldgrabon; // use a while shielding to grab
bool tapjumpon; // use up dpad to jump
bool cstickstylus; // smashes and aerials with stylus

//wrapper methods to get global variables from other classes
//FIXME: these should probably exist somewhere other than main if they need
//to be accessible to lots of other classes. maybe a customcontrols class or
//global or something.
map<int , int> getcustomcontrols(){
	return customcontrols;
}
bool getTapJumpOn(){
	return tapjumpon;
}
bool getCStickStylus(){
	return cstickstylus;
}
bool getShieldGrabOn(){
	return shieldgrabon;
}
void* getProj(){
	return &projectiles;
}
void removeProj(int prnum) {
	vector<Projectile> temp;
	for(int n = 0; n < (int)projectiles.size(); n++) {
		Projectile p = projectiles[n];
		if(p.num != prnum) {
			temp.push_back(p);
		}
	}
	projectiles = temp;
	PA_SetSpriteXY(MAIN_SCREEN, prnum, -64, -64);
}
bool custom_action(int action, int typecheck) {
	if(customcontrols[action] == BUTTON_A) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.A) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.A) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.A) return true;
		}
	}
	else if(customcontrols[action] == BUTTON_B) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.B) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.B) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.B) return true;
		}
	}
	else if(customcontrols[action] == BUTTON_AB) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.A && Pad.Held.B) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.A && Pad.Newpress.B) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.A || Pad.Released.B) return true;
		}	
	}
	else if(customcontrols[action] == BUTTON_X) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.X) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.X) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.X) return true;
		}	
	}
	else if(customcontrols[action] == BUTTON_Y) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.Y) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.Y) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.Y) return true;
		}
	}
	else if(customcontrols[action] == BUTTON_L) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.L) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.L) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.L) return true;
		}	
	}
	else if(customcontrols[action] == BUTTON_R) {
		if(typecheck == PAD_HELD) {
			if(Pad.Held.R) return true;
		}
		else if(typecheck == PAD_NEWPRESS) {
			if(Pad.Newpress.R) return true;
		}
		else if(typecheck == PAD_RELEASED) {
			if(Pad.Released.R) return true;
		}
	}
	else if(customcontrols[action] == BUTTON_NONE) {
		return false;
	}
	if(action == ACTION_JUMP) return custom_action(ACTION_JUMP2, typecheck);
	if(action == ACTION_SHIELD) return custom_action(ACTION_SHIELD2, typecheck);
	return false;
} // takes action and checks if it is done by custom controls

//Graphics functions.:
//FIXME: Should these move to display?
void printMemoryUsage() {
#ifdef DEBUG_ON
	PA_OutputText(MAIN_SCREEN, 0, 5, "                                                  ");
	PA_OutputText(MAIN_SCREEN, 0, 7, "                                                  ");
	PA_OutputText(MAIN_SCREEN, 0, 9, "                                                  ");
	struct mallinfo info = mallinfo(); // memory allocation info
	PA_OutputText(MAIN_SCREEN,0,5,"Memory in use: %d bytes", info.usmblks + info.uordblks);
	PA_OutputText(MAIN_SCREEN,0,7,"Total heap size: %d bytes", info.arena);
	PA_OutputText(MAIN_SCREEN,0,9,"Memory in free: %d bytes", info.fsmblks + info.fordblks);
#endif
} 
// prints memory usage to main screen. requires text to be initialized.
// only prints if debugging

char* gifbuffer = NULL; // the array which stores the gif being printed
void openGif(int screen, string path) {
	delete gifbuffer; // clears the gif buffer
	FILE* imgFile = fopen (path.c_str(), "rb"); // opens the file at path for reading	
	if(imgFile) {
		u32 imgSize;
		fseek (imgFile, 0 , SEEK_END);
		imgSize = ftell (imgFile);
		rewind (imgFile);
		// obtains file size
	
		gifbuffer = NULL;
		gifbuffer = (char*) malloc (sizeof(char)*imgSize);
		fread (gifbuffer, 1, imgSize, imgFile);
		// allocate memory to store the image file
	
		fclose (imgFile); // close the file
		PA_LoadGif(screen, (void *)gifbuffer); // Show gif on screen
	} // loads the gif if the image file exists
} // opens the gif at path onto screen

void fadeOut() {
   	for(int i = 0; i >= -31; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		AS_SetMP3Volume((i+31)*4);
		for(int n = 0; n < 16; n++) AS_SetSoundVolume(n, (i+31)*4);
		PA_WaitForVBL();
		PA_WaitForVBL();
	} // slowly darkens the screen into black
	AS_MP3Stop();
	PA_ResetBgSys();
	PA_FatFreeSfxBuffers();
} // fades both screens out
void fadeIn() {
   	for(int i = -31; i <= 0; i++) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		AS_SetMP3Volume((i+31)*4);
		for(int n = 0; n < 16; n++) AS_SetSoundVolume(n, (i+31)*4);
		PA_WaitForVBL();
		PA_WaitForVBL();
	} // slowly brightens the screen to normal
} // fades both screens in
void flash() {
	for(int i = 1; i <= 31; i+=3) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // brightens the screen to white realy quickly
	for(int i = 31; i >= 0; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // darkens the screen to normal
} // flashes the screen brightly

#import "minimap.cpp" // minimap for during battles

int selectedStage = -1; //the stage currently selected

Stage setStage(int selStage) {
	Stage picked; // the stage which is chosen
	if(selStage == FINALDESTINATION) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, finaldestination);
		picked = FinalDestination();
	} // loads final destination if it was chosen
	if(selStage == POKEMONSTADIUM) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, pokemonstadium);
		picked = PokemonStadium();
	} // loads pokemon stadium if it was chosen
	if(selStage == CASTLESIEGE) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, castlesiege);
		picked = CastleSiege();
	} // loads castle siege if it was chosen
	if(selStage == CORNERIA) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, corneria);
		picked = Corneria();
	} // loads corneria if it was chosen
	for(int n = 0; n < (int)players.size(); n++) {
		players[n] -> setStage(&picked);
	} // sets the stage of the players to the picked stage
	return picked; // returns the picked stage
} // displays the stage on the main screen

//Set up sprite stuff:
void initFX() {
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, 15, "specialFX");
	PA_FatLoadSprite(254, "specialFX");
	for(int n = 5; n < 21; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)sprite_gfx[254], OBJ_SIZE_64X64, COLOR256, 15, -64, -64);
	}
	// loads all special effect sprites
} // initializes special effects
void initProjectiles() {
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, 14, "projectiles");
	PA_FatLoadSprite(255, "projectiles");
	for(int n = 50; n < 54; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)sprite_gfx[255], OBJ_SIZE_64X64, COLOR256, 14, -64, -64);
	} // loads all projectile sprites
} // initializes projectiles

//Controls saving and loading:
void initControls() {
	FILE* file = fopen("/SSBDS_Files/saves/controls.sav", "rb");
	if(file) {
		u32 size;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);
				
		char line[4];
		char line2[4];
		int n = 0;
		while(n < 16) {
			fgets(line, 4, file);
			fgets(line2, 4, file);
			customcontrols[atoi(strtok(line, " \t"))] = atoi(strtok(line2, " \t"));
			n += 2;
		}
		fgets(line, 4, file);
		if(atoi(strtok(line, " \t")) == 1) cstickstylus = true;
		else cstickstylus = false;
		fgets(line, 4, file);
		if(atoi(strtok(line, " \t")) == 1) shieldgrabon = true;
		else shieldgrabon = false;
		fgets(line, 4, file);
		if(atoi(strtok(line, " \t")) == 1) tapjumpon = true;
		else tapjumpon = false;
		fclose(file);
	}
	else {	
		customcontrols[ACTION_BASIC] = BUTTON_A;
		customcontrols[ACTION_SPECIAL] = BUTTON_B;
		customcontrols[ACTION_SMASH] = BUTTON_AB;
		customcontrols[ACTION_JUMP] = BUTTON_X;
		customcontrols[ACTION_JUMP2] = BUTTON_Y;
		customcontrols[ACTION_SHIELD] = BUTTON_R;
		customcontrols[ACTION_SHIELD2] = BUTTON_L;
		customcontrols[ACTION_GRAB] = BUTTON_NONE;
		cstickstylus = false;
		shieldgrabon = true;
		tapjumpon = true;
	}
} // inits default or saved control setup
void saveControls() {
	ostringstream buffer;
	for(int n = 0; n <= ACTION_GRAB; n++) {
		buffer << n;
		buffer << "\n";
		buffer << customcontrols[n];
		buffer << "\n";
	}
	buffer << (int)cstickstylus;
	buffer << "\n";
	buffer << (int)shieldgrabon;
	buffer << "\n";
	buffer << (int)tapjumpon;
	buffer << "\n";

	FILE* file = fopen("/SSBDS_Files/saves/controls.sav", "wb");
	fprintf(file, "%s", buffer.str().c_str());
	fclose(file);
} // saves default control setup

//Menu screens:
// selecting char/stage
void stageSelect() {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white	
		
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 0, "stagesel");
	PA_FatLoadSprite(31, "stagesel");
	PA_CreateSprite(SUB_SCREEN, FINALDESTINATION, (void*)sprite_gfx[31], OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, FINALDESTINATION, FINALDESTINATION, FINALDESTINATION, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, POKEMONSTADIUM, (void*)sprite_gfx[31], OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, POKEMONSTADIUM, POKEMONSTADIUM, POKEMONSTADIUM, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, CASTLESIEGE, (void*)sprite_gfx[31], OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CASTLESIEGE, CASTLESIEGE, CASTLESIEGE, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, CORNERIA, (void*)sprite_gfx[31], OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CORNERIA, CORNERIA, CORNERIA, 1, ANIM_LOOP, -1);
	// loads sprites just like in characterSelect()

	PA_FatLoadSfx("confirm", "menuconfirm");

	fadeIn();
		
	while(true) {
		if(Stylus.Newpress) {
			for(int n = 0; n < 10; n++) { // through all possible stages
				if(PA_SpriteTouched(n)) {
					PA_FatPlaySfx("confirm"); // menu confirm sound
					fadeOut();
					PA_ResetSpriteSysScreen(SUB_SCREEN); // resets sprites
					PA_FatFreeSprite(31);
					PA_FatFreePalette(0);
					selectedStage = n; // sets selected stage, just like in characterSelect()
					return;
				}
			}
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void characterSelect(bool train = false) {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white

	PA_InitText(SUB_SCREEN, 0); // init text on sub screen
	PA_SetTextCol(SUB_SCREEN, 0,0,0); // text color of black		

	PA_FatEasyLoadSpritePal(SUB_SCREEN, 0, "charsel");
	PA_FatLoadSprite(0, "charsel");
	// creates a paleete... see mainMenu() for more details
	// loads palette with all character selection icons, so it only uses 1 palette for all the sprites
	PA_CreateSprite(SUB_SCREEN, KIRBY, (void*)sprite_gfx[0], OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
	// creates a sprite... see mainMenu() 
	PA_StartSpriteAnimEx(SUB_SCREEN, KIRBY, KIRBY, KIRBY, 1, ANIM_LOOP, -1);
	// animates a sprite Arguments are:
	// - screen the sprite is on
	// - reference number to the sprite
	// - starting frame of sprite animation
	// - ending frame of sprite animation
	// - FPS of animation (doesn't matter in this case b/c it's the same frame the whole time)
	// - the type of animation, in this casea loop
	// - how long to loop for (-1 is infinite)	
	PA_CreateSprite(SUB_SCREEN, MEWTWO, (void*)sprite_gfx[0], OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MEWTWO, MEWTWO, MEWTWO, 1, ANIM_LOOP, -1);
	// another sprite
	PA_CreateSprite(SUB_SCREEN, MARIO, (void*)sprite_gfx[0], OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MARIO, MARIO, MARIO, 1, ANIM_LOOP, -1);
	// a third sprite
	PA_CreateSprite(SUB_SCREEN, IKE, (void*)sprite_gfx[0], OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, IKE, IKE, IKE, 1, ANIM_LOOP, -1);

	PA_LoadSpritePal(MAIN_SCREEN, 0, (void*)charprev_Pal);
	PA_CreateSprite(MAIN_SCREEN, 0, (void*)charprev, OBJ_SIZE_64X64, COLOR256, 0, 0, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 0, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 1, (void*)charprev, OBJ_SIZE_64X64, COLOR256, 0, 64, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 1, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 2, (void*) charprev, OBJ_SIZE_64X64, COLOR256, 0, 128, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 2, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 3, (void*) charprev, OBJ_SIZE_64X64, COLOR256, 0, 192, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 3, 0, 0, 1, ANIM_LOOP, -1);	
	// loads and animates character previews on top screen.

	PA_FatLoadSfx("ffa", "free_for_all");
	PA_FatLoadSfx("confirm", "menuconfirm");
	PA_FatLoadSfx("kirby", "kirbyname");
//	PA_FatLoadSfx("mewtwo", "mewtwoname");
	PA_FatLoadSfx("mario", "marioname");
	PA_FatLoadSfx("ike", "ikename");

	AS_MP3StreamPlay("SSBDS_Files/music/select.mp3");

	fadeIn();
	PA_FatPlaySfx("ffa");
	// plays free for all sound byte

	int humanselected = -1; // which character was chosen for the human
	int cpu1selected = -1; // which character was chosen for computer 1
	int cpu2selected = -1;
	int cpu3selected = -1;
	int selecting = 0; // who is currently selecting the character 
	// (I'll change to cursors like the previous smashes later)
	// 0 = human, 1 = cpu1... 2 and 3 would be for other CPUs if I get that far

	while(true) {
		// changes who is choosing
		if(Pad.Newpress.R){
			selecting++;
			if(selecting > 3) 
				selecting = 0;
		}
		if(Pad.Newpress.L){
			selecting--;
			if(selecting < 0) 
				selecting = 3;
		}
		// loops around selection number: max number of players is 1
		PA_OutputText(SUB_SCREEN, 7, 23, "Select for player %d", selecting+1);
		// prints who is being selected for... like I said: cursors will come later
		if(Pad.Newpress.Start && humanselected != -1) {
// if start is pressed and both players are ready
			PA_FatPlaySfx("confirm");
			fadeOut();
			PA_ResetSpriteSys(); // restes all sprites
			PA_FatFreeSprBuffers();
			PA_OutputText(SUB_SCREEN, 7, 23, "                     "); // clears text
	
			PA_LoadSpritePal(MAIN_SCREEN, 13, (void*)shield_Pal);
										
			if(humanselected == KIRBY) players.push_back(new Kirby(1, &players, &display));
			else if(humanselected == MEWTWO) players.push_back(new Mewtwo(1, &players, &display));
			else if(humanselected == MARIO) players.push_back(new Mario(1, &players, &display));
			else if(humanselected == IKE) players.push_back(new Ike(1, &players, &display));		  
			// adds a new player class (fighter*) for the human
			
			if(!train && !(cpu1selected == -1 && cpu2selected == -1 && cpu3selected == -1)) {				
				if(cpu1selected == KIRBY) players.push_back(new Kirby(2, &players, &display, true));
				else if(cpu1selected == MEWTWO) players.push_back(new Mewtwo(2, &players, &display, true));
				else if(cpu1selected == MARIO) players.push_back(new Mario(2, &players, &display, true));
				else if(cpu1selected == IKE) players.push_back(new Ike(2, &players, &display, true));		 

				if(cpu2selected == KIRBY) players.push_back(new Kirby(3, &players, &display, true));
				else if(cpu2selected == MEWTWO) players.push_back(new Mewtwo(3, &players, &display, true));
				else if(cpu2selected == MARIO) players.push_back(new Mario(3, &players, &display, true));
				else if(cpu2selected == IKE) players.push_back(new Ike(3, &players, &display, true));		 

				if(cpu3selected == KIRBY) players.push_back(new Kirby(4, &players, &display, true));
				else if(cpu3selected == MEWTWO) players.push_back(new Mewtwo(4, &players, &display, true));
				else if(cpu3selected == MARIO) players.push_back(new Mario(4, &players, &display, true));
				else if(cpu3selected == IKE) players.push_back(new Ike(4, &players, &display, true));		 
				// adds a new player class (fighter*) for the cpu1
			}
			else players.push_back(new Sandbag(2, &players, &display, true));			
			
			return;
		}
		if(Stylus.Newpress) {
// if the screen is touched
			bool spritetouched = false;
			for(int n = 0; n < 10; n++) { // through the last character number
				if(PA_SpriteTouched(n)) {
					spritetouched = true;
					if(n == KIRBY) PA_FatPlaySfx("kirby");
//					else if(n == MEWTWO) PA_FatPlaySfx("mewtwo");
					else if(n == MARIO) PA_FatPlaySfx("mario");
					else if(n == IKE) PA_FatPlaySfx("ike");
					// plays a sound byte of the player's name
					if(selecting == 0) {
						humanselected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 0, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 1) {
						cpu1selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 1, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 2) {
						cpu2selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 2, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 3) {
						cpu3selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 3, n, n, 1, ANIM_LOOP, -1);
					}
// changes the character number of the selecting player to the character 
// that was clicked and changes the frame of that player's character preview
// if kirby was clicked, the humanselected/cpu1selected = 1 (or static const int KIRBY)
// I told you those public variables would come in handy
				}
			}
			if(!spritetouched) {
				if(selecting == 0) {
					humanselected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 0, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 1) {
					cpu1selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 1, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 2) {
					cpu2selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 2, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 3) {
					cpu3selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 3, -1, -1, 1, ANIM_LOOP, -1);
				}
			}	
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}

// In-game functions:
void Pause() {
	for(int n = 0; n < 128; n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, n, 1);
		PA_SpriteAnimPause(SUB_SCREEN, n, 1);
	}
	PA_WaitForVBL();
	PA_OutputText(MAIN_SCREEN, 12, 10, "PAUSED");
	while(!Pad.Newpress.Start) PA_WaitForVBL();
	PA_OutputText(MAIN_SCREEN, 12, 10, "         ");
	for(int n = 0; n < 128; n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, n, 0);
		PA_SpriteAnimPause(MAIN_SCREEN, n, 0);
	}
} // pauses the game; will add pause menu later
void scrollScreen() {
	if(cameratype == CAMERATYPE_FOLLOWALL) {
		double maxx = -10000;
		double minx = 10000;
		double maxy = -10000;
		double miny = 10000;
		// initializes min and max x and y
		for(int n = 0; n < (int)players.size(); n++) {
			if(score.getDeaths(n)+sdcost*score.getSDs(n) < stocklimit && gamemode == GAMEMODE_STOCK) {
				double x = players[n] -> x;
				double y = players[n] -> y;
				if(x > maxx) maxx = x;
				if(x < minx) minx = x;
				if(y > maxy) maxy = y;
				if(y < miny) miny = y;
			}
		} // finds the minimum and maximum x and y position
		scrollx = (maxx + minx) / 2;
		scrolly = (maxy + miny) / 2;
		// scrolls so that the camera is centred between the min and max x and y
	} // if the camera follows everyone
	else if(cameratype == CAMERATYPE_FOLLOWUSER) {
		scrollx = players[0] -> x;
		scrolly = players[0] -> y;
		// centers camera on user
	} // if the camer follows the user
	scrollx = scrollx - 128 + 32;
	scrolly = scrolly - 96 + 32; 
	// centers the camera on the sprites and screen
	if(scrollx < -128) scrollx = -128;
	if(scrolly < -192) scrolly = -192;
	if(scrollx > 512-256+128) scrollx = 512-256+128;
	if(scrolly > 256-192) scrolly = 256-192;
	// wraps scrolling around
	PA_LargeScrollX(MAIN_SCREEN, 0, 256+(int)scrollx);
	PA_LargeScrollY(MAIN_SCREEN, 0, 256+(int)scrolly);
	// scrolls the screen
	for(int n = 0; n < (int)players.size(); n++) {
		players[n] -> scroll(scrollx, scrolly);
	} // scrolls the players
	for(int n = 0; n < (int)effects.size(); n++) {
		if(effects[n].type == FX_DEATH) {} // don't move sprite
		else if(effects[n].type == FX_AIRJUMP) PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM)+32);
		else PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM));
	} // scrolls the special effects
}

void displayResults() {		
	PA_Init8bitBg(MAIN_SCREEN, 3);
	PA_Init8bitBg(SUB_SCREEN, 3);
	deleteMinimap();
	for(int n = 0; n < 128; n++) {
		PA_SetSpriteXY(MAIN_SCREEN, n, -64, -64);
		PA_SetSpriteXY(SUB_SCREEN, n, -64, -64);
	}
	for(int n = 0; n < (int)players.size(); n++) {
		PA_DeleteSprite(MAIN_SCREEN, (players[n]->SPRITENUM)-4);
		players[n] -> fall();
		players[n] -> idle(); 
		// ensures that it goes into idling animation
		PA_SetSpriteHflip(MAIN_SCREEN, players[n] -> SPRITENUM, 0);
		PA_SetSpriteXY(MAIN_SCREEN, players[n] -> SPRITENUM, (int)(n*48) +48, 0);
	} // moves character sprites into position above their scores
	
	int winner = 0; // winner of the game, based on location in players
	bool draw = false; // whether or not the winners are tied
	for(int n = 1; n < (int)players.size(); n++) {
		if(score.getTotal(n) > score.getTotal(winner)) {
			draw = false;
			winner = n;
		} // if the winner's score is surpassed, there is a new leader
		else if(score.getTotal(n) == score.getTotal(winner)) draw = true;
		// or if the winner's score is equaled, there is a draw
	}
	// calculates winner
	
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// loads an image on the sub screen

	PA_FatLoadSfx("nc", "nocontest");
	PA_FatLoadSfx("kirby", "kirbyname");
//	PA_FatLoadSfx("mewtwo", "mewtwoname");
	PA_FatLoadSfx("mario", "marioname");
	PA_FatLoadSfx("ike", "ikename");
	PA_FatLoadSfx("winneris", "thewinneris");
	PA_FatLoadSfx("confirm", "menuconfirm");
	
	if(draw) {} // doesn't display a main screen bg
	else {
		if(players[winner] -> name == "kirby") {
			if(players[winner] -> SPRITENUM-100 == 1) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/kirby1.gif");
			if(players[winner] -> SPRITENUM-100 == 2) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/kirby2.gif");
			if(players[winner] -> SPRITENUM-100 == 3) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/kirby3.gif");
			if(players[winner] -> SPRITENUM-100 == 4) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/kirby4.gif");
			AS_MP3StreamPlay("SSBDS_Files/music/victories/kirby.mp3");
		}
		if(players[winner] -> name == "mewtwo") {
			if(players[winner] -> SPRITENUM-100 == 1) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/pokemon1.gif");
			if(players[winner] -> SPRITENUM-100 == 2) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/pokemon2.gif");
			if(players[winner] -> SPRITENUM-100 == 3) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/pokemon3.gif");
			if(players[winner] -> SPRITENUM-100 == 4) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/pokemon4.gif");			
			AS_MP3StreamPlay("SSBDS_Files/music/victories/pokemon.mp3");
		}
		if(players[winner] -> name == "mario") {
			if(players[winner] -> SPRITENUM-100 == 1) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/mariobros1.gif");
			if(players[winner] -> SPRITENUM-100 == 2) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/mariobros2.gif");
			if(players[winner] -> SPRITENUM-100 == 3) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/mariobros3.gif");
			if(players[winner] -> SPRITENUM-100 == 4) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/mariobros4.gif");
			AS_MP3StreamPlay("SSBDS_Files/music/victories/mariobros.mp3");
		}
		if(players[winner] -> name == "ike") {
			if(players[winner] -> SPRITENUM-100 == 1) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/fireemblem1.gif");
			if(players[winner] -> SPRITENUM-100 == 2) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/fireemblem2.gif");
			if(players[winner] -> SPRITENUM-100 == 3) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/fireemblem3.gif");
			if(players[winner] -> SPRITENUM-100 == 4) openGif(MAIN_SCREEN, "SSBDS_Files/gifs/victories/fireemblem4.gif");
			AS_MP3StreamPlay("SSBDS_Files/music/victories/fireemblem.mp3");
		}
		AS_SetMP3Loop(false);
	} 
	// displays the series icon of the winner in the winner's color
	
	PA_InitText(MAIN_SCREEN, 1);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	PA_OutputSimpleText(MAIN_SCREEN, 0, 8, "Total:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 11, "Kills:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 14, "Deaths:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 17, "SDs:");
	for(int n = 0; n < (int)players.size(); n++) {
		int total = score.getTotal(n);
		int kills = score.getKills(n);
		int deaths = score.getDeaths(n);
		int SDs = sdcost*score.getSDs(n);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 8, "%d", total);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 11, "%d", kills);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 14, "%d", deaths);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 17, "%d", SDs);
	}
	// prints the scoreboard stats for each player

	fadeIn();

	if(draw) {
		PA_FatPlaySfx("nc");
		for(int n = 0; n < 90; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying nobody won
	else {
		PA_FatPlaySfx("winneris");
		for(int n = 0; n < 80; n++) {
			PA_WaitForVBL();
		}
		if(players[winner] -> name == "kirby") PA_FatPlaySfx("kirby");
//		else if(players[winner] -> name == "mewtwo") PA_FatPlaySfx("mewtwo");
		else if(players[winner] -> name == "mario") PA_FatPlaySfx("mario");
		else if(players[winner] -> name == "ike") PA_FatPlaySfx("ike");
						
		for(int n = 0; n < 60; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying ther winner

	while(true) {
		if(Stylus.Newpress) {
			PA_FatPlaySfx("confirm");
			fadeOut();
			AS_SetMP3Loop(true);
			score.clear(); // clears the scoreboard
			effects.clear(); // clears the effects
			for(int n = 0; n < 128; n++) {
				PA_DeleteSprite(MAIN_SCREEN, n);
				PA_DeleteSprite(SUB_SCREEN, n);
			}
			for(int n = 0; n < (int)players.size(); n++) {
				PA_FatFreeSprite(players[n] -> MYCHAR);
				delete players[n];
			} // deletes the sprites of all players
			PA_FatInitAllBuffers();
			players.clear(); // clears players vector
			for(int n = 0; n < 16; n++) AS_SoundStop(n);
			// stop sounds
			scrollx = 0;
			scrolly = 0;
			// resets game scrolls
			return;
		} // returns to title screen
		PA_WaitForVBL();
	}
 }

int oldcam = cameratype;
bool camchanged = false;

void gameOver() {
	PA_FatPlaySfx("game");
	for(int n = 0; n < 128; n++) {
		PA_StopSpriteAnim(MAIN_SCREEN, n);
		PA_StopSpriteAnim(SUB_SCREEN, n);
	}
	if(gamemode == GAMEMODE_TIME) PA_OutputText(MAIN_SCREEN, 13, 0, "0:00"); // displays 0 as the time
	for(int n = 0; n < 60; n++) PA_WaitForVBL(); // waits for 1 second
	fadeOut();
	cameratype = oldcam;
	camchanged = false;
	return displayResults();
}

// Game modes/logic
void match(int param) {
	int time=0;
	int stock=0;
	if (gamemode==GAMEMODE_TIME) time = param*60*60 + 60; // minutes -> vblanks
	else if(gamemode == GAMEMODE_STOCK) stock = param;	
	
	characterSelect(); // select characters
	for(int n = 0; n < (int)players.size(); n++) {
		players[n] -> players = players;
	}
	stageSelect(); // select stage
	initFX(); // inits the special FX
	initProjectiles(); // inits the projectiles
			
	Stage stage = setStage(selectedStage); 
	// sets the stage to the stage chosen in stageSelect
	PA_InitText(MAIN_SCREEN,1); // inits text on the main screen (displays time)
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
	initMinimap(selectedStage); // inits minimap
	PA_InitText(SUB_SCREEN,1); // inits test on sub screen (displays percentages)
	PA_SetTextCol(SUB_SCREEN, 31,31,31); // text color = white

	// initializes scoreboard
	score = Scoreboard(players.size()); // initializes a new scoreboard

	PA_LargeScrollX(MAIN_SCREEN, 0, stage.width/2+128);
	PA_LargeScrollY(MAIN_SCREEN, 0, stage.height/2+96);

	for(int n = 0; n < (int)players.size(); n++) {
		players[n] -> initSounds();
	}
	
	PA_FatLoadSfx("game", "game");
	PA_FatLoadSfx("3", "three");
	PA_FatLoadSfx("2", "two");
	PA_FatLoadSfx("1", "one");
	PA_FatLoadSfx("go", "go");
	
	PA_FatLoadSfx("shieldbreak", "shieldbreak");
	PA_FatLoadSfx("hit1", "hit1");
	PA_FatLoadSfx("hit2", "hit2");
	PA_FatLoadSfx("hit3", "hit3");
	PA_FatLoadSfx("death", "deathsound");

	fadeIn();

	PA_FatPlaySfx("3");
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	PA_FatPlaySfx("2");
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	PA_FatPlaySfx("1");
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	PA_FatPlaySfx("go");
	// counts down to start game
		
	while(true) {
		if(PA_CheckLid()) Pause(); // if the lid is closed it pauses
		if(Pad.Newpress.Start) Pause(); // checks to see if the game was paused by start button
		if (gamemode==GAMEMODE_TIME){
			if(time-60 == 0) {
				return gameOver();
			}
		}
		else if (gamemode==GAMEMODE_STOCK){
			int playersstillalive=players.size(); //and while you're dying...
			for(int n = 0; n < (int)players.size(); n++) {
				if(score.getDeaths(n)+sdcost*score.getSDs(n) >= stock)
					playersstillalive--;
			}
			if (playersstillalive==1) return gameOver();
		}
		for(int n = 0; n < (int)players.size(); n++) {
			if(score.getDeaths(n)+sdcost*score.getSDs(n) < stock) players[n] -> act();
			if(score.getDeaths(n)+sdcost*score.getSDs(n) >= stock) {
				players[n] -> beDead();
				if((players[n] -> isCPU) == false) {
					if(!camchanged) {
						oldcam = cameratype;
						camchanged = true;
					}
					cameratype = CAMERATYPE_FOLLOWALL;
				}
			}
		} // all players act
		for(int n = 0; n < (int)players.size(); n++) {
			if(score.getDeaths(n)+sdcost*score.getSDs(n) >= stock) {}
			else {
				for(int m = 0; m < (int)players.size(); m++) {
					if(score.getDeaths(m)+sdcost*score.getSDs(m) >= stock) {}
					else {
						if(m != n) players[m] = players[n] -> checkHits(players[m]);
					}
				}
			}
		}
		for(int n = 0; n < (int)players.size(); n++) {
			players[n]->allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, players[n]->SPRITENUM)].enabled = false;
		}
		// checks to see if any player hit another
		scrollScreen(); // scrolls the screen
		for(int n = 0; n < (int)projectiles.size(); n++) {
			if(projectiles[n].act()) {
				removeProj(projectiles[n].num);
			}
			for(int m = 0; m < (int)players.size(); m++) {
				if(projectiles[n].owner != m) players[m] = projectiles[n].checkHits(players[m]);
			}
		}
		// acts and checks intersections of all projectiles
		display.updateEffects(); // acts all effects
		displayMinimap(); // changes sub screen display
		displayPercentages(); // displays percentages on sub screen
		if(gamemode == GAMEMODE_STOCK) displayLives(stock);
		PA_OutputText(MAIN_SCREEN, 13,0, "          "); // clears time
		if((int)((time/60)%60) < 10) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:0%d",(int)((time/60)/60), (int)((time/60)%60));
		else if((int)((time/60)%60) == 0) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:00",(int)((time/60)/60));
		else PA_OutputText(MAIN_SCREEN, 13, 0, "%d:%d", (int)((time/60)/60), (int)((time/60)%60));		
		// redisplays time
		printMemoryUsage();
		PA_WaitForVBL();
		if (gamemode==GAMEMODE_TIME) time--; // another tick off the clock!
		else time++; // time counts up if its not a time match
	}
}
void trainingMode() {
	characterSelect(true); // select characters
	for(int n = 0; n < (int)players.size(); n++) {
		players[n] -> players = players;
	}
	stageSelect(); // select stage
	initFX(); // inits the special FX
	initProjectiles(); // inits the projectiles
			
	Stage stage = setStage(selectedStage); 
	// sets the stage to the stage chosen in stageSelect
	PA_InitText(MAIN_SCREEN,1); // inits text on the main screen (displays time)
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
	initMinimap(selectedStage); // inits minimap
	PA_InitText(SUB_SCREEN,1); // inits test on sub screen (displays percentages)
	PA_SetTextCol(SUB_SCREEN, 31,31,31); // text color = white

	fadeIn();
																																												
	while(true) {
		PA_CheckLid(); // if the lid is closed it pauses
		if(Pad.Newpress.Start) Pause(); 
		// checks to see if the game was paused by start button
		for(int n = 0; n < (int)players.size(); n++) players[n] -> act(); // all players act
		for(int n = 0; n < (int)players.size(); n++) {
			for(int m = 0; m < (int)players.size(); m++) {
				if(m != n) {
					players[m] = players[n] -> checkHits(players[m]);
				}
			}
		}
		// checks to see if any player hit another
		scrollScreen(); // scrolls the screen
		for(int n = 0; n < (int)projectiles.size(); n++) {
			if(projectiles[n].act()) {
				removeProj(projectiles[n].num);
			}
			for(int m = 0; m < (int)players.size(); m++) {
				if(projectiles[n].owner != m) players[m] = projectiles[n].checkHits(players[m]);
			}
		}
		// acts and checks intersections of all projectiles
		//for(int n = 0; n < (int)effects.size(); n++) effects[n].act();
		display.updateEffects();
		// acts all effects
		displayMinimap(); // changes sub screen display
		displayPercentages(); // displays percentages on sub screen
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // training mode

//More menu screens:
void controlOptions() {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
		
	PA_InitText(SUB_SCREEN, 0);
	PA_SetTextCol(SUB_SCREEN, 0, 0, 0); // black text
	
	map<int, const char*> buttonstrs;
	buttonstrs[BUTTON_NONE] = "None";
	buttonstrs[BUTTON_A] = "A";
	buttonstrs[BUTTON_B] = "B";
	buttonstrs[BUTTON_AB] = "AB";
	buttonstrs[BUTTON_X] = "X";
	buttonstrs[BUTTON_Y] = "Y";
	buttonstrs[BUTTON_L] = "L";
	buttonstrs[BUTTON_R] = "R";

	map<int, const char*> actionstrs;
	actionstrs[ACTION_BASIC] = "Basic";
	actionstrs[ACTION_SPECIAL] = "Special";
	actionstrs[ACTION_SMASH] = "Smash";
	actionstrs[ACTION_JUMP] = "Jump";
	actionstrs[ACTION_JUMP2] = "Jump";
	actionstrs[ACTION_SHIELD] = "Shield";
	actionstrs[ACTION_SHIELD2] = "Shield";
	actionstrs[ACTION_GRAB] = "Grab";
	
	int selected = ACTION_BASIC;

	for(int action = ACTION_BASIC; action <= ACTION_GRAB; action++) {
		PA_OutputText(SUB_SCREEN, 4, action, "%s", actionstrs[action]);
		PA_OutputText(SUB_SCREEN, 16, action, "%s", buttonstrs[customcontrols[action]]);
	}
	PA_OutputText(SUB_SCREEN, 4, 8, "C-Stick:");
	if(cstickstylus) PA_OutputText(SUB_SCREEN, 16, 8, "on");
	else PA_OutputText(SUB_SCREEN, 16, 8, "off");
	PA_OutputText(SUB_SCREEN, 4, 9, "Shield Grab:");
	if(shieldgrabon) PA_OutputText(SUB_SCREEN, 16, 9, "on");
	else PA_OutputText(SUB_SCREEN, 16, 9, "off");
	PA_OutputText(SUB_SCREEN, 4, 10, "Tap Jump:");
	if(tapjumpon) PA_OutputText(SUB_SCREEN, 16, 10, "on");
	else PA_OutputText(SUB_SCREEN, 16, 10, "off");
	PA_OutputText(SUB_SCREEN, 0, 0, " **");
	PA_OutputText(SUB_SCREEN, 0, 20, "Putting multiple actions on one button can cause errors");
	
	fadeIn();
	while(true) {
		if(Pad.Newpress.Up) selected--;
		if(Pad.Newpress.Down) selected++;
		if(selected > ACTION_GRAB+3) selected = ACTION_GRAB+3;
		if(selected < ACTION_BASIC) selected = ACTION_BASIC;
		if(Pad.Newpress.Up || Pad.Newpress.Down) {
			for(int mark = ACTION_BASIC; mark <= ACTION_GRAB+3; mark++) {
				if(mark == selected) PA_OutputText(SUB_SCREEN, 0, mark, " ** ");
				else PA_OutputText(SUB_SCREEN, 0, mark, "    ");
			}
		}
		// change which action your switching controls for
		
		if(Pad.Newpress.Right) {
			if(selected == 10) {
				tapjumpon = !tapjumpon;
			}
			else if(selected == 9) {
				shieldgrabon = !shieldgrabon;
			}
			else if(selected == 8) {
				cstickstylus = !cstickstylus;
			}
			else {
				customcontrols[selected] += 1;
				if(customcontrols[selected] > BUTTON_R) customcontrols[selected] = BUTTON_NONE;
			}
		}
		if(Pad.Newpress.Left) {
			if(selected == 10) {
				tapjumpon = !tapjumpon;
			}
			else if(selected == 9) {
				shieldgrabon = !shieldgrabon;
			}
			else if(selected == 8) {
				cstickstylus = !cstickstylus;
			}
			else {
				customcontrols[selected] -= 1;
				if(customcontrols[selected] < BUTTON_NONE) customcontrols[selected] = BUTTON_R;
			}
		}
		if(Pad.Newpress.Right || Pad.Newpress.Left) {
			for(int action = ACTION_BASIC; action <= ACTION_GRAB; action++) {
				PA_OutputText(SUB_SCREEN, 16, action, "     ");
				PA_OutputText(SUB_SCREEN, 16, action, "%s", buttonstrs[customcontrols[action]]);
			}
			PA_OutputText(SUB_SCREEN, 16, 8, "    ");
			PA_OutputText(SUB_SCREEN, 16, 9, "    ");
			PA_OutputText(SUB_SCREEN, 16, 10, "    ");
			if(cstickstylus) PA_OutputText(SUB_SCREEN, 16, 8, "on");
			else PA_OutputText(SUB_SCREEN, 16, 8, "off");
			if(shieldgrabon) PA_OutputText(SUB_SCREEN, 16, 9, "on");
			else PA_OutputText(SUB_SCREEN, 16, 9, "off");
			if(tapjumpon) PA_OutputText(SUB_SCREEN, 16, 10, "on");
			else PA_OutputText(SUB_SCREEN, 16, 10, "off");
		}
		// chnage the action for the selected control		
			
		if(Pad.Newpress.B || Pad.Newpress.A || Pad.Newpress.Start) {			
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN);
			saveControls();
			return;
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // edit custom controls
void cameraOptions() {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
		
	PA_InitText(SUB_SCREEN, 0);
	PA_SetTextCol(SUB_SCREEN, 0, 0, 0); // black text
	
	map<int, const char*> camerastrs;
	camerastrs[CAMERATYPE_FOLLOWUSER] = "Follow Me";
	camerastrs[CAMERATYPE_FOLLOWALL] = "Follow All";
	
	fadeIn();
	while(true) {
		if(Pad.Newpress.Left) {
			cameratype -= 1;
			if(cameratype < CAMERATYPE_FOLLOWUSER) cameratype = CAMERATYPE_FOLLOWUSER;
		}
		if(Pad.Newpress.Right) {
			cameratype += 1;
			if(cameratype > CAMERATYPE_FOLLOWALL) cameratype = CAMERATYPE_FOLLOWALL;
		}
		if(Pad.Newpress.B || Pad.Newpress.A || Pad.Newpress.Start) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN);
			return;
		}
		PA_OutputText(SUB_SCREEN, 0, 0, "Camera Mode: %s", camerastrs[cameratype]);
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // edit camera options
void gameOptions() {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
		
	PA_InitText(SUB_SCREEN, 0);
	PA_SetTextCol(SUB_SCREEN, 0, 0, 0); // black text

	int selected = 0;

	PA_OutputText(SUB_SCREEN, 4, 0, "Game Mode:");
	PA_OutputText(SUB_SCREEN, 4, 1, "Stock/Time:");
	PA_OutputText(SUB_SCREEN, 4, 2, "SD cost:");

	if(gamemode == GAMEMODE_STOCK) PA_OutputText(SUB_SCREEN, 16, 0, "stock");
	else if(gamemode == GAMEMODE_TIME) PA_OutputText(SUB_SCREEN, 16, 0, "time");
	if(gamemode == GAMEMODE_STOCK) PA_OutputText(SUB_SCREEN, 16, 1, "%d", stocklimit);
	else if(gamemode == GAMEMODE_TIME) PA_OutputText(SUB_SCREEN, 16, 1, "%d:00", timelimit);
	PA_OutputText(SUB_SCREEN, 16, 2, "%d", sdcost);

	PA_OutputText(SUB_SCREEN, 0, 0, " ** ");

	fadeIn();
	while(true) {
		if(Pad.Newpress.Up) selected--;
		if(Pad.Newpress.Down) selected++;
		if(selected > 2) selected = 2;
		if(selected < 0) selected = 0;
		if(Pad.Newpress.Up || Pad.Newpress.Down) {
			for(int mark = 0; mark <= 2; mark++) {
				if(mark == selected) PA_OutputText(SUB_SCREEN, 0, mark, " ** ");
				else PA_OutputText(SUB_SCREEN, 0, mark, "    ");
			}
		}
		
		if(Pad.Newpress.Right) {
			if(selected == 0) {
				gamemode++;
				if(gamemode > 1) gamemode = 1;
			}
			else if(selected == 1) {
				if(gamemode == GAMEMODE_STOCK) {
					stocklimit++;
					if(stocklimit > 99) stocklimit = 99;
				}
				else if(gamemode == GAMEMODE_TIME) {
					timelimit++;
					if(timelimit > 99) timelimit = 99;
				}
			}
			else if(selected == 2) {
				sdcost++;
				if(sdcost > 2) sdcost = 2;
			}
		}
		if(Pad.Newpress.Left) {
			if(selected == 0) {
				gamemode--;
				if(gamemode < 0) gamemode = 0;
			}
			else if(selected == 1) {
				if(gamemode == GAMEMODE_STOCK) {
					stocklimit--;
					if(stocklimit < 1) stocklimit = 1;
				}
				else if(gamemode == GAMEMODE_TIME) {
					timelimit--;
					if(timelimit < 1) timelimit = 1;
				}
			}
			else if(selected == 2) {
				sdcost--;
				if(sdcost < 0) sdcost = 0;
			}
		}
		if(Pad.Newpress.Right || Pad.Newpress.Left) {
			PA_OutputText(SUB_SCREEN, 16, 0, "         ");
			if(gamemode == GAMEMODE_STOCK) PA_OutputText(SUB_SCREEN, 16, 0, "stock");
			else if(gamemode == GAMEMODE_TIME) PA_OutputText(SUB_SCREEN, 16, 0, "time");
			PA_OutputText(SUB_SCREEN, 16, 1, "         ");
			if(gamemode == GAMEMODE_STOCK) PA_OutputText(SUB_SCREEN, 16, 1, "%d", stocklimit);
			else if(gamemode == GAMEMODE_TIME) PA_OutputText(SUB_SCREEN, 16, 1, "%d:00", timelimit);		
			PA_OutputText(SUB_SCREEN, 16, 2, "%d", sdcost);
		}
		
		if(Pad.Newpress.B || Pad.Newpress.A || Pad.Newpress.Start) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN);
			return;
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // edit match style
void initOptions() {
	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white

	PA_InitText(SUB_SCREEN, 0);
	PA_SetTextCol(SUB_SCREEN, 0, 0, 0);
	
	PA_OutputText(SUB_SCREEN, 4, 0, "Control Options");
	PA_OutputText(SUB_SCREEN, 4, 1, "Camera Options");
	PA_OutputText(SUB_SCREEN, 4, 2, "Game Options");
	
	PA_OutputText(SUB_SCREEN, 0, 0, " ** ");

	fadeIn();
}
void options() {
	int selected = 0;
	initOptions();
	while(true) {
		if(Pad.Newpress.Down) {
			selected++;
			if(selected > 2) selected = 2;
		}
		if(Pad.Newpress.Up) {
			selected--;
			if(selected < 0) selected = 0;
		}
		if(Pad.Newpress.Up || Pad.Newpress.Down) {
			for(int mark = 0; mark <= 2; mark++) {
				if(mark == selected) PA_OutputText(SUB_SCREEN, 0, mark, " ** ");
				else PA_OutputText(SUB_SCREEN, 0, mark, "    ");
			}
		}
		if(Pad.Newpress.A || Pad.Newpress.Start) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN);
			if(selected == 0) {
				controlOptions();
				initOptions();
				selected = 0;
			}
			if(selected == 1) {
				cameraOptions();
				initOptions();
				selected = 0;
			}
			if(selected == 2) {
				gameOptions();
				initOptions();
				selected = 0;
			}
		}
		if(Pad.Newpress.B) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN); // gets rid of menu sprites
			return; // back
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // options menu, in progress
void extras() {

} // extras menu, uncoded

// (Even more) pre-game menus
void initMainMenu() {
	PA_Init8bitBg(SUB_SCREEN, 3);
	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	// opens gif background. no need to reinit, just loads over the old gif for this screen.
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	//put title screen on top screen when at main menu.

	PA_FatLoadSfx("no", "menuno");
	PA_FatLoadSfx("confirm", "menuconfirm");

	AS_MP3StreamPlay("/SSBDS_Files/music/menu.mp3");
	// plays main menu music

	PA_ResetSpriteSysScreen(MAIN_SCREEN);

	PA_InitText(MAIN_SCREEN, 0);
	PA_SetTextCol(MAIN_SCREEN, 31, 31, 31);

	fadeIn();
}
void mainMenu() {
	initMainMenu();
	while(true) {
		if(Pad.Newpress.B) {
			PA_FatPlaySfx("no");
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN); // gets rid of menu sprites
			return; // back to title screen
		}
		if(Stylus.Newpress) {
			int x = Stylus.X;
			int y = Stylus.Y;
			if(x > 105 && x < 228 && y > 38 && y < 64) {
				PA_FatPlaySfx("confirm");
				fadeOut();
				if(gamemode == GAMEMODE_TIME) {
					match(timelimit);
					initMainMenu();
				}
				else if(gamemode == GAMEMODE_STOCK) {
					match(stocklimit);
					initMainMenu();
				}
			}
			else if(x > 82 && x < 205 && y > 70 && y < 99) {				
				PA_FatPlaySfx("confirm");
				fadeOut();
				runNdsFile("SSBDS_Files/LAN/LAN.nds");
				initMainMenu();
			}
			else if(x > 60 && x < 183 && y > 104 && y < 131) {				
				PA_FatPlaySfx("confirm");
				fadeOut();
				extras();
				initMainMenu();
			}
			else if(x > 38 && x < 162 && y > 136 && y < 164) {
				PA_FatPlaySfx("confirm");
				fadeOut();
				options();
				initMainMenu();
			}
		}
		if(Pad.Newpress.Start) {
			trainingMode();
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void titleScreen() {
	PA_ResetSpriteSys(); // clears all sprites on both screens

	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/title.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");
	// opens the gif from the path on the main screen
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white

	PA_FatLoadSfx("confirm", "menuconfirm");

	AS_MP3StreamPlay("SSBDS_Files/music/title.mp3"); // title screen music

	fadeIn();
		
	while(true) {
		if(Stylus.Newpress) { // if the stylus is pressed
			PA_FatPlaySfx("confirm"); // menu confirm sound byte
			fadeOut();
			PA_ResetSpriteSys(); // resets sprites
			return mainMenu();
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}

//Set-up for Palib and gets the system rolling:
int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib 
	PA_InitVBL(); // Initializes a standard VBlank (FPS handler)
	PA_InitRand();

	vramSetBankE(VRAM_E_MAIN_SPRITE);

#ifdef DEBUG_ON
	defaultExceptionHandler(); // "red screen of death" error, hopefully won't happen
#endif

	if(!fatInitDefault()) {
		PA_InitText(MAIN_SCREEN, 0);
		PA_SetTextCol(MAIN_SCREEN, 31, 31, 31);
		PA_OutputText(MAIN_SCREEN, 0, 0, "FAT INIT FAILED!!!");
		while(true) {}
	} // Init for libfat. if it fails it freezes with an error
	PA_FatInitAllBuffers(); // Initialize all the memory buffers
	PA_FatSetBasePath("/SSBDS_Files");  // Set a base path
	// initializes external file system. very important!!!

	fadeOut();

	PA_VBLFunctionInit(AS_SoundVBL); // easy way to make sure that AS_SoundVBL() is called every frame
    AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound
	
	initControls();
		
	while(true) titleScreen(); // permanently runs the game
	return 0; //never happens, but then again, DS games don't quit
} // End of main()

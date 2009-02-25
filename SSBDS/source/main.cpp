// Game designed by TJgames of TJHSST
// Head Developer(s): Jeremy Vercillo
// Assistant Developer(s): Tyler Haines, Daniel Johnson, Patrick Stalcup
// Head Advisor: Andrew Kim
// 6/08 - ???

// The rights to Super Smash Bros. and all the related characters, stages, and items
// belong to Nintendo and other 3rd party companies. This is a fan made game; it not
// made for profit, just for fun.

#define DEBUG_ON // turns on printing of information to screen

#define SFX_ON
#define PROJECTILES_ON
//#define SLOPEDSTAGES_ON // Castle Seige and Corneria
//#define LAN_ON // REMEMBER TO CHANGE MAKEFILE TOO!!!!
//#define MP3_ON
// turns certain features on and off

#include <PA9.h> // DS functions that we use come from here
#include <math.h> // math!
#include <vector> // vectors!
#include <string> // strings!
#include <sstream> // int to string
#include <stdio.h> // standard file functions
#include <stdlib.h> // standard C functions
#include <map> // maps

#include "gfx/all_gfx.c" // all image files

#ifdef SFX_ON
#include "gfx/all_sounds.c" // all sound effects (just small ones, not MP3s)
#endif

using namespace std; // standard naming of variables

#define PI 3.1415926 // PI

#define MAIN_SCREEN 1 // top screen
#define SUB_SCREEN 0 // bottom screen
#define COLOR256 1 // 256 color mode

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

class Fighter;
vector<Fighter*> players;
// stores all fighters for playing a match

#ifdef PROJECTILES_ON
class Projectile;
vector<Projectile> projectiles;
// stores all projectiles
#endif
#ifdef SFX_ON
class Effect;
vector<Effect> effects;
// stores all visual effects
#endif
class Scoreboard; // keeps score of the game

double scrollx = 0;
double scrolly = 0;
// how far the screen is scrolled (for stages)

int BUTTON_NONE = -1, BUTTON_A = 0, BUTTON_B = 1, BUTTON_AB = 2, BUTTON_X = 3, BUTTON_Y = 4, BUTTON_L = 5, BUTTON_R = 6; 
// buttons (for custom controls)
int ACTION_BASIC = 0, ACTION_SPECIAL = 1, ACTION_SMASH = 2, ACTION_JUMP = 3, ACTION_JUMP2 = 4, ACTION_SHIELD = 5, ACTION_SHIELD2 = 6, ACTION_GRAB = 7;
// actions (for custom controls)
int PAD_HELD = 0, PAD_NEWPRESS = 1, PAD_RELEASED = 2; // Press types (for custom controls)

map<int, int> customcontrols; // custom control mapping
bool shieldgrabon; // use a while shielding to grab
bool tapjumpon; // use up dpad to jump
bool cstickstylus; // smashes and aerials with stylus

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
} // takes action and checks if it is done by custom controls, uncoded

class Scoreboard {
	vector<int> kills; // player numbers of the kills (in order) -- -1 is a SD
	vector<int> deaths; // player numbers of the deaths (in order)
	public:
		int playercount; // total number of players
		Scoreboard(int count) {
			playercount = count;
		} // creates a scoreboard
		void addDeath(int killer, int killed) {
			kills.push_back(killer);
			deaths.push_back(killed);
		} // registers a kill
		void clear() {
			kills.clear();
			deaths.clear();
		} // clears the score
		int getKills(int player) {
			int count = 0;
			for(int n = 0; n < kills.size(); n++) {
				if(kills[n] == player) count++;
			}
			return count;
		} // the number of kills by player with number player
		int getDeaths(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(deaths[n] == player && kills[n] != -1) count++;
			}
			return count;
		} // the number of deaths by player with number player
		int getSDs(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(deaths[n] == player && kills[n] == -1) count++;
			}
			return count;
		} // the number of SDs by player with number player
		int getTotal(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(kills[n] == player) count++;
				if(deaths[n] == player) count--;
			}
			return count;
		} // totals score for a player with number player
}; // tracks all scores
Scoreboard score = Scoreboard(0); // the scoreboard for smash
#import "framework.cpp" // Stuff for the fighting engine (hitboxes, effects, etc)
#import "stage.cpp" // Info about the stage and the stage superclass
#import "stages.cpp" // individual stages
#import "fighter.cpp" // fighter superclass
#import "projectiles.cpp" // projectiles
#import "fighters.cpp" // individual characters

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
void fadeOut() {
   	for(int i = 0; i >= -31; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // slowly darkens the screen into black
} // fades both screens out
void fadeIn() {
   	for(int i = -31; i <= 0; i++) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
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

#import "minimap.cpp" // minimap for during battles

string stagename; // the name of the current stage

Stage setStage(string name) {
	PA_ResetBgSysScreen(MAIN_SCREEN); // resets bg on the main screen
	Stage picked; // the stage which is chosen
	if(name == "finaldestination") {
//		PA_Init8bitBg(MAIN_SCREEN, 3);
//		openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/finaldestinationbackground.gif");
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, finaldestination);
		picked = FinalDestination();
	} // loads final destination if it was chosen
	if(name == "pokemonstadium") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, pokemonstadium);
		picked = PokemonStadium();
	} // loads pokemon stadium if it was chosen
#ifdef SLOPEDSTAGES_ON
	if(name == "castleseige") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, castleseige);
		picked = CastleSeige();
	} // loads castle seige if it was chosen
	if(name == "corneria") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, corneria);
		picked = Corneria();
	} // loads corneria if it was chosen
#endif
	for(int n = 0; n < players.size(); n++) {
		players[n] -> setStage(picked);
	} // sets the stage of the players to the picked stage
	return picked; // returns the picked stage
} // displays the stage on the main screen
#ifdef SFX_ON
void initFX() {
	PA_LoadSpritePal(MAIN_SCREEN, 15, (void*)specialFX_Pal);
	for(int n = 5; n < 25; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)specialFX, OBJ_SIZE_64X64, COLOR256, 15, -64, -64);
	}
	// loads all special effect sprites
} // initializes special effects
#endif
#ifdef PROJECTILES_ON
void initProjectiles() {
	PA_LoadSpritePal(MAIN_SCREEN, 14, (void*)projectilesprites_Pal);
	for(int n = 50; n < 55; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)projectilesprites, OBJ_SIZE_64X64, COLOR256, 14, -64, -64);
	} // loads all projectile sprites
} // initializes projectiles
#endif
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
//	FILE* file = fopen("/SSBDS_Files/saves/controls.sav", "wb");

//	fwrite((void*)ACTION_BASIC, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_BASIC], sizeof(int), 1, file);
//	fwrite((void*)ACTION_SPECIAL, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_SPECIAL], sizeof(int), 1, file);
//	fwrite((void*)ACTION_SMASH, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_SMASH], sizeof(int), 1, file);
//	fwrite((void*)ACTION_JUMP, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_JUMP], sizeof(int), 1, file);
//	fwrite((void*)ACTION_JUMP2, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_JUMP2], sizeof(int), 1, file);
//	fwrite((void*)ACTION_SHIELD, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_SHIELD], sizeof(int), 1, file);
//	fwrite((void*)ACTION_SHIELD2, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_SHIELD2], sizeof(int), 1, file);
//	fwrite((void*)ACTION_GRAB, sizeof(int), 1, file);
//	fwrite((void*)customcontrols[ACTION_GRAB], sizeof(int), 1, file);
	
//	if(cstickstylus) fwrite((void*)1, sizeof(int), 1, file);
//	else fwrite((void*)0, sizeof(int), 1, file);
//	if(shieldgrabon) fwrite((void*)1, sizeof(int), 1, file);
//	else fwrite((void*)0, sizeof(int), 1, file);
//	if(tapjumpon) fwrite((void*)1, sizeof(int), 1, file);
//	else fwrite((void*)0, sizeof(int), 1, file);
		
//	fclose(file);
} // saves default control setup

// selecting char/stage
void stageSelect() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/blank.gif"); // blank background
	
	fadeIn();
	
	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)stagesel_Pal);
	PA_CreateSprite(SUB_SCREEN, FINALDESTINATION, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, FINALDESTINATION, FINALDESTINATION, FINALDESTINATION, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, POKEMONSTADIUM, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, POKEMONSTADIUM, POKEMONSTADIUM, POKEMONSTADIUM, 1, ANIM_LOOP, -1);
#ifdef SLOPEDSTAGES_ON
	PA_CreateSprite(SUB_SCREEN, CASTLESEIGE, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CASTLESEIGE, CASTLESEIGE, CASTLESEIGE, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, CORNERIA, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CORNERIA, CORNERIA, CORNERIA, 1, ANIM_LOOP, -1);
#endif
	// loads sprites just like in characterSelect()
	
	int selected = -1; // which stage has been selected, by the static const ints
	
	while(true) {
		if(Pad.Newpress.Start && selected != -1) {
// if start is pressed and a stage is selected
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif
// menu confirmation sound
			fadeOut();
			
			PA_ResetSpriteSysScreen(SUB_SCREEN); // resets sprites
			
			if(selected == FINALDESTINATION) stagename = "finaldestination";
			if(selected == POKEMONSTADIUM) stagename = "pokemonstadium";
#ifdef SLOPEDSTAGES_ON
			if(selected == CASTLESEIGE) stagename = "castleseige";
			if(selected == CORNERIA) stagename = "corneria";
#endif
			// set stagename based on selected
		
			return;
		}
		if(Stylus.Newpress) {
			for(int n = 0; n < 10; n++) { // through all possible stages
				if(PA_SpriteTouched(n)) {
#ifdef SFX_ON
					AS_SoundQuickPlay(menuconfirm);
#endif
					// menu confirm sound

					if(n == 1) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/finaldestinationprev.gif");
					if(n == 2) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/pokemonstadiumprev.gif");
#ifdef SLOPEDSTAGES_ON
					if(n == 3) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/castleseigeprev.gif");
					if(n == 4) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/corneriaprev.gif");
#endif
					// stage preview depending on what the new stage is
					
					selected = n; // sets selected stage, just like in characterSelect()

				}
			}
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void characterSelect() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/blank.gif");
	// blank background
	
	PA_InitText(SUB_SCREEN, 0); // init text on sub screen
	PA_SetTextCol(SUB_SCREEN, 0,0,0); // text color of black

	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)charsel_Pal);
	// creates a paleete... see mainMenu() for more details
	// loads palette with all character selection icons, so it only uses 1 palette for all the sprites
	PA_CreateSprite(SUB_SCREEN, KIRBY, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
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
	PA_CreateSprite(SUB_SCREEN, MEWTWO, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MEWTWO, MEWTWO, MEWTWO, 1, ANIM_LOOP, -1);
	// another sprite
	PA_CreateSprite(SUB_SCREEN, MARIO, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MARIO, MARIO, MARIO, 1, ANIM_LOOP, -1);
	// a third sprite
	PA_CreateSprite(SUB_SCREEN, IKE, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
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

	fadeIn();
#ifdef SFX_ON	
	AS_SoundQuickPlay(free_for_all);
#endif	
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
		if(Pad.Newpress.Start && humanselected != -1 && cpu1selected != -1) {
// if start is pressed and both players are ready
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif			
			// menu confirmation sound byte
			fadeOut();
			PA_ResetSpriteSys(); // restes all sprites
			PA_OutputText(SUB_SCREEN, 7, 23, "                     "); // clears text
			if(humanselected == KIRBY) players.push_back(new Kirby(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == MEWTWO) players.push_back(new Mewtwo(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == MARIO) players.push_back(new Mario(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == IKE) players.push_back(new Ike(512/2 -96 -32, 256/3 -33, 1));		  
			// adds a new player class (fighter*) for the human
					
			if(cpu1selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 2, true));		 

			if(cpu2selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 3, true));		 

			if(cpu3selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 4, true));		 
			// adds a new player class (fighter*) for the cpu1
			
			return;
		}
		if(Stylus.Newpress) {
// if the screen is touched
			bool spritetouched = false;
			for(int n = 0; n < 10; n++) { // through the last character number
				if(PA_SpriteTouched(n)) {
					spritetouched = true;
#ifdef SFX_ON
					if(n == KIRBY) AS_SoundQuickPlay(kirby);
//					else if(n == MEWTWO) AS_SoundQuickPlay(mewtwo);
					else if(n == MARIO) AS_SoundQuickPlay(mario);
					else if(n == IKE) AS_SoundQuickPlay(ike);
#endif
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

// in-game functions
void Pause() {
	for(int n = 0; n < players.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, players[n] -> SPRITENUM, 1);
	} // pauses all player animations
	// - screen
	// - sprite number
	// - on/off (1/0)
	for(int n = 0; n < effects.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, effects[n].mynum, 1);
	} // pauses all effect animations
	for(int n = 0; n < projectiles.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, projectiles[n].num, 1);
	} // pauses all projectile animations
	PA_WaitForVBL();
	PA_OutputText(MAIN_SCREEN, 12, 10, "PAUSED"); // prints that it's paused
	while(!Pad.Newpress.Start) {
		PA_WaitForVBL();
	} // waits for unpause
	PA_OutputText(MAIN_SCREEN, 12, 10, "          "); // clears text
	for(int n = 0; n < players.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, players[n] -> SPRITENUM, 0);
	} // unpauses players
	for(int n = 0; n < effects.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, effects[n].mynum, 0);
	} // unpauses effects
	for(int n = 0; n < projectiles.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, projectiles[n].num, 0);
	} // unpauses projectiles
} // pauses the game; will add pause menu later
void scrollScreen() {
	if(cameratype == CAMERATYPE_FOLLOWALL) {
		double maxx = players[0] -> x;
		double minx = players[0] -> x;
		double maxy = players[0] -> y;
		double miny = players[0] -> y;
		// initializes min and max x and y
		for(int n = 0; n < players.size(); n++) {
			double x = players[n] -> x;
			double y = players[n] -> y;
			if(x > maxx) maxx = x;
			if(x < minx) minx = x;
			if(y > maxy) maxy = y;
			if(y < miny) miny = y;
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
	for(int n = 0; n < players.size(); n++) {
		players[n] -> scroll(scrollx, scrolly);
	} // scrolls the players
#ifdef SFX_ON
	for(int n = 0; n < effects.size(); n++) {
		if(effects[n].type == FX_DEATH) {} // don't move sprite
		else if(effects[n].type == FX_AIRJUMP) PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM)+32);
		else PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM));
	} // scrolls the special effects
#endif
}

void displayResults() {		
	PA_ResetBg(MAIN_SCREEN);
	PA_ResetBg(SUB_SCREEN);
	// resets the background for both screens
	
	PA_Init8bitBg(MAIN_SCREEN, 3);
	PA_Init8bitBg(SUB_SCREEN, 3);
	// initializes a gif on both screens
	for(int n = 0; n < 5; n++) {
		PA_StopSpriteAnim(SUB_SCREEN,n);
		PA_DeleteSprite(SUB_SCREEN,n);
	} // stops and deletes minimap object sprites
#ifdef SFX_ON
	for(int n = 5; n < 20; n++) {
		PA_StopSpriteAnim(MAIN_SCREEN, n);
		PA_DeleteSprite(MAIN_SCREEN, n);
	} // stops and deletes special effects
#endif
#ifdef PROJECTILES_ON
	for(int n = 50; n < 55; n++) {
		PA_StopSpriteAnim(MAIN_SCREEN, n);
		PA_DeleteSprite(MAIN_SCREEN, n);
	} // stops and deletes projectile sprites
#endif
	for(int n = 0; n < players.size(); n++) {
		PA_SetSpriteXY(MAIN_SCREEN, players[n] -> SPRITENUM, -64,-64);
	} // stops the characters from moving
	
	int winner = 0; // winner of the game, based on location in players
	bool draw = false; // whether or not the winners are tied
	for(int n = 1; n < players.size(); n++) {
		if(score.getTotal(n) > score.getTotal(winner)) {
			draw = false;
			winner = n;
		} // if the winner's score is surpassed, there is a new leader
		else if(score.getTotal(n) == score.getTotal(winner)) draw = true;
		// or if the winner's score is equaled, there is a draw
	}
	// calculates winner
	
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/smashball.gif");
	// loads an image on the sub screen
	
	if(draw) {} // doesn't display a main screen bg
	else {
		if(players[winner] -> name == "kirby") {
			if(players[winner] -> charnum == 0) { 
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/kirbywin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/kirbywin2.gif");
			}
		}
		if(players[winner] -> name == "mewtwo") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/pokemonwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/pokemonwin2.gif");
			}
 		}
		if(players[winner] -> name == "mario") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/mariowin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/mariowin2.gif");
			}
		}
		if(players[winner] -> name == "ike") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/fireemblemwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/fireemblemwin2.gif");
			}
		}
	} 
	// displays the series icon of the winner in the winner's color
	
	PA_InitText(MAIN_SCREEN, 1);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	PA_OutputSimpleText(MAIN_SCREEN, 0, 8, "Total:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 11, "Kills:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 14, "Deaths:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 17, "SDs:");
	for(int n = 0; n < players.size(); n++) {
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
	
	for(int n = 0; n < players.size(); n++) {
		players[n] -> fall();
		players[n] -> idle(); 
		// ensures that it goes into idling animation
		PA_SetSpriteHflip(MAIN_SCREEN, players[n] -> SPRITENUM, 0);
		PA_SetSpriteXY(MAIN_SCREEN, players[n] -> SPRITENUM, (int)(n*48) +48, 0);
	} 
	// moves character sprites into position above their scores

	fadeIn();

#ifdef SFX_ON
	if(draw) {
		AS_SoundQuickPlay(nocontest);
		for(int n = 0; n < 90; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying nobody won
	else {
		AS_SoundQuickPlay(thewinneris);
		for(int n = 0; n < 80; n++) {
			PA_WaitForVBL();
		}
		if(players[winner] -> name == "kirby") AS_SoundQuickPlay(kirby);
//		else if(players[winner] -> name == "mewtwo") AS_SoundQuickPlay(mewtwo);
		else if(players[winner] -> name == "mario") AS_SoundQuickPlay(mario);
		else if(players[winner] -> name == "ike") AS_SoundQuickPlay(ike);
//		else if(players[winner] -> name == "shadow") AS_SoundQuickPlay(shadow);
						
		for(int n = 0; n < 60; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying ther winner
#endif

	while(true) {
		if(Stylus.Newpress) {
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif			
			fadeOut();
			score.clear(); // clears the scoreboard
			effects.clear(); // clears the effects
//			for(int n = 0; n < players.size(); n++) {
//				players[n] -> deleteSprite();
//				delete players[n];
//			} // deletes the sprites of all players
			players.clear(); // clears players vector
			PA_FatFreeSprBuffers(); // frees sprite loading space
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

void gameOver() {
#ifdef SFX_ON
	AS_SoundQuickPlay(game);
#endif
	// end of game sound clip
	for(int n = 0; n < players.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, players[n] -> SPRITENUM);
	// stops sprite anim for all players
#ifdef PROJECTILES_ON
	for(int n = 0; n < projectiles.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, projectiles[n].num);
#endif			
	// stops all projectile animations
#ifdef SFX_ON
	for(int n = 0; n < effects.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, effects[n].mynum);
#endif			
	// stops all effect animations
	if(gamemode == GAMEMODE_TIME) PA_OutputText(MAIN_SCREEN, 13, 0, "0:00"); // displays 0 as the time
	for(int n = 0; n < 60; n++) PA_WaitForVBL(); // waits for 1 second
	fadeOut();
	return displayResults();
}

// game modes
void match(int param) {
	int time=0;
	int stock=0;
	if (gamemode==GAMEMODE_TIME) time = param*60*60 + 60; // minutes -> vblanks
	else if(gamemode == GAMEMODE_STOCK) stock = param;
	
	characterSelect(); // select characters
	stageSelect(); // select stage
#ifdef SFX_ON
	initFX(); // inits the special FX
#endif	
#ifdef PROJECTILES_ON
	initProjectiles(); // inits the projectiles
#endif
			
	Stage stage = setStage(stagename); 
	// sets the stage to the stage chosen in stageSelect
	PA_InitText(MAIN_SCREEN,1); // inits text on the main screen (displays time)
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
	initMinimap(stagename); // inits minimap
	PA_InitText(SUB_SCREEN,1); // inits test on sub screen (displays percentages)
	PA_SetTextCol(SUB_SCREEN, 31,31,31); // text color = white

	// initializes scoreboard
	score = Scoreboard(players.size()); // initializes a new scoreboard

	fadeIn();

#ifdef SFX_ON		
	AS_SoundQuickPlay(three);
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	AS_SoundQuickPlay(two);
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	AS_SoundQuickPlay(one);
	for(int n = 0; n < 60; n++) PA_WaitForVBL();
	AS_SoundQuickPlay(go);
#endif	
	// counts down to start game
																																										
	while(true) {
		PA_CheckLid(); // if the lid is closed it pauses
		if(Pad.Newpress.Start) Pause(); 
		// checks to see if the game was paused by start button
		if (gamemode==GAMEMODE_TIME){
			if(time-60 == 0) {
				return gameOver();
			}
		}
		else if (gamemode==GAMEMODE_STOCK){
			int playersstillalive=players.size(); //and while you're dying...
			for(int n = 0; n < players.size(); n++) {
				if(score.getDeaths(n)+sdcost*score.getSDs(n) >= stock)
					playersstillalive--;
			}
			if (playersstillalive==1) return gameOver();
		}
		for(int n = 0; n < players.size(); n++) players[n] -> act(); // all players act
		for(int n = 0; n < players.size(); n++) {
			for(int m = 0; m < players.size(); m++) {
				if(m != n) {
					players[m] = players[n] -> checkHits(players[m]);
				}
			}
		}
		// checks to see if any player hit another
		scrollScreen(); // scrolls the screen
#ifdef PROJECTILES_ON
		for(int n = 0; n < projectiles.size(); n++) {
			projectiles[n].act();
			for(int m = 0; m < players.size(); m++) {
				if(projectiles[n].owner != m) players[m] = projectiles[n].checkHits(players[m]);
			}
		}
#endif
		// acts and checks intersections of all projectiles
#ifdef SFX_ON
		for(int n = 0; n < effects.size(); n++) effects[n].act();
#endif
		// acts all effects
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

} // training mode
void controlOptions() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	
#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/Menu.mp3");
#endif

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
				int oldaction = customcontrols[selected];
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
				int oldaction = customcontrols[selected];
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
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	
#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/Menu.mp3");
#endif

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
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	
#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/Menu.mp3");
#endif

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
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	// opens gif background. no need to reinit, just loads over the old gif for this screen.

#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/Menu.mp3");
	// plays main menu music
#endif

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
			PA_ResetBg(SUB_SCREEN); // gets rid of bgs
			return; // back
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
} // options menu, in progress
void extras() {

} // extras menu, uncoded

#import "LAN.cpp"

// pre-game menus
void initMainMenu() {
	PA_Init8bitBg(SUB_SCREEN, 3);
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	// opens gif background. no need to reinit, just loads over the old gif for this screen.

#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/Menu.mp3");
	// plays main menu music
#endif
	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)menusolo_Pal);
	// inits a sprite palette Arguments are:
	// - the screen the palette exists on
	// - the reference number to the palette from 0 to 15
	// - a void* cast of the location the paleete exists in... see the all_gfx.c
	PA_CreateSprite(SUB_SCREEN, 0, (void*)menusolo, OBJ_SIZE_64X64, COLOR256, 0, 48, 16);
	// inits a sprite Arguments are:
	// - the screen the sprite exists on
	// - the reference number to the sprite from 0 to 127
	// - a void* cast of the location the sprite exists in... see the all_gfx.c
	// - size of the sprite, almost always we'll use 64X64, but there are other options
	// - the color mode used to display the sprite... always 256 colors that's why I #defined it
	// - the reference number of the palette used to display the sprite, see the palette loading
	// - the x position on the screen from 0 to 255
	// - the y position on the screen form 0 to 191
	
	PA_LoadSpritePal(SUB_SCREEN, 1, (void*)menulan_Pal);
	PA_CreateSprite(SUB_SCREEN, 1, (void*)menulan, OBJ_SIZE_64X64, COLOR256, 1, 256-64-48, 16);
	// another menu button

	PA_LoadSpritePal(SUB_SCREEN, 2, (void*)menuoptions_Pal);
	PA_CreateSprite(SUB_SCREEN, 2, (void*)menuoptions, OBJ_SIZE_64X64, COLOR256, 2, 48, 192-64-16);
	// a third menu button

	PA_LoadSpritePal(SUB_SCREEN, 3, (void*)menuextras_Pal);
	PA_CreateSprite(SUB_SCREEN, 3, (void*)menuextras, OBJ_SIZE_64X64, COLOR256, 3, 256-64-48, 192-64-16);
	// a fourth menu button

	PA_ResetSpriteSysScreen(MAIN_SCREEN);
	PA_ResetBgSysScreen(MAIN_SCREEN);

	fadeIn();
}
void mainMenu() {
	initMainMenu();
	while(true) {
		if(Pad.Newpress.B) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN); // gets rid of menu sprites
			return; // back to title screen
		}
		if(Stylus.Newpress) {
			for(int n = 0; n < 4; n++) { // loops through all sprites (menu buttons)
				if(PA_SpriteTouched(n)) { // checks to see if the referenced sprite was touched
#ifdef SFX_ON			
					AS_SoundQuickPlay(menuconfirm);
					// menu sound byte
#endif
#ifdef MP3_ON
					AS_MP3Stop();
					// stops background music
#endif
					fadeOut();
					PA_ResetSpriteSysScreen(SUB_SCREEN); // resets sprites on sub screen
					if(n == 0) {
						if(gamemode == GAMEMODE_TIME) {
							match(timelimit);
							initMainMenu();
						}
						else if(gamemode == GAMEMODE_STOCK) {
							match(stocklimit);
							initMainMenu();
						}
					}
					if(n == 1) {
#ifdef LAN_ON
						LAN();
#endif					
						initMainMenu();
					}
					if(n == 2) {
						options();
						initMainMenu();
					}
					if(n == 3) {
						extras();
						initMainMenu();
					}
				}
			}
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void titleScreen() {
	PA_ResetBgSys(); // clears all bgs on both screens
	PA_ResetSpriteSys(); // clears all sprites on both screens

	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/title2.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/blank.gif");
	// opens the gif from the path on the main screen
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white

#ifdef MP3_ON	
	AS_MP3StreamPlay("SSBDS_Files/music/MeleeThemeRemix.mp3");
	// title screen music
#endif
	fadeIn();
	while(true) {
		if(Stylus.Newpress) { // if the stylus is pressed
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
			// menu confirm sound byte
#endif
#ifdef MP3_ON
			AS_MP3Stop();
			// stops sound
#endif						
			fadeOut();
			PA_ResetSpriteSys(); // resets sprites
			return mainMenu();
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib 
	PA_InitVBL(); // Initializes a standard VBlank (FPS handler)
	PA_InitRand();

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

	PA_SetBrightness(MAIN_SCREEN, -31);
	PA_SetBrightness(SUB_SCREEN, -31);
	// screen brightness (not related to backlight setting)
	// -31 = all black
	// 31 = all white
	// 0 = normal

#ifdef LAN_ON
	IPC_Init() ;
	LOBBY_Init() ;
# endif
	// inits LAN functions

    AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound

#ifdef LAN_ON
	PA_VBLFunctionInit(customVBL); // custom VBL function called every frame
#else
	PA_VBLFunctionInit(AS_SoundVBL); // easy way to make sure that AS_SoundVBL() is called every frame
#endif
	
	initControls();
	
	while(true) titleScreen(); // permanently runs the game
	return 0;
} // End of main()

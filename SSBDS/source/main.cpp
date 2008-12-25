// Game designed by TJgames of TJHSST
// Head Developer: Jeremy Vercillo
// Assistant Developer(s): Tyler Haines, Daniel Johnson, Patrick Stalcup
// 6/08 - ???

// The rights to Super Smash Bros. and all the related characters, stages, and items
// belong to Nintendo and other 3rd party companies. This is a fan made game; it not
// intended for profit, just for fun

#define DEBUG_ON // turns on printing of information to screen

#define SFX_ON
#define PROJECTILES_ON
//#define SANDBAG_ON
//#define SHADOW_ON
//#define MINIMAP_ON
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

#ifdef MINIMAP_ON
static const int UPARR = 1, DOWNARR = 2, LEFTARR = 3, RIGHTARR = 4, P1MINI = 5, P2MINI = 6, P3MINI = 7, P4MINI = 8, MINIBOX = 9; 
// shortcuts for minimap sprites
#endif

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
	if(imgFile){
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
#ifdef MINIMAP_ON
void initMinimap(string name) {
	PA_ResetBgSysScreen(SUB_SCREEN);
	// initializes the minimap image
//	if(name == "finaldestination") {
//		PA_LoadPAGfxLargeBg(SUB_SCREEN, 0, finaldestinationminimap);
//	}
	
	PA_LoadSpritePal(SUB_SCREEN, P1MINI, (void*)p1minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P1MINI, (void*)p1minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P1MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P2MINI, (void*)p2minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P2MINI, (void*)p2minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P2MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P3MINI, (void*)p3minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P3MINI, (void*)p3minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P3MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P4MINI, (void*)p4minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P4MINI, (void*)p4minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P4MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, MINIBOX, (void*)visibleminimap_Pal);
	PA_CreateSprite(SUB_SCREEN, MINIBOX, (void*)visibleminimap_Sprite, OBJ_SIZE_32X32, COLOR256, MINIBOX, -8, -8);
	// loads the sprite icons for the minimap
	
	PA_LoadSpritePal(SUB_SCREEN, UPARR, (void*)ArrowUp_Pal);
	PA_CreateSprite(SUB_SCREEN, UPARR, (void*)ArrowUp_Sprite, OBJ_SIZE_64X64, COLOR256, UPARR, 96, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, UPARR, 0, 2, 1, ANIM_LOOP, -1);

	PA_LoadSpritePal(SUB_SCREEN, DOWNARR, (void*)ArrowDown_Pal);
	PA_CreateSprite(SUB_SCREEN, DOWNARR, (void*)ArrowDown_Sprite, OBJ_SIZE_64X64, COLOR256, DOWNARR, 96, 128);
	PA_StartSpriteAnimEx(SUB_SCREEN, DOWNARR, 0, 2, 1, ANIM_LOOP, -1);
	
	PA_LoadSpritePal(SUB_SCREEN, LEFTARR, (void*)ArrowLeft_Pal);
	PA_CreateSprite(SUB_SCREEN, LEFTARR, (void*)ArrowLeft_Sprite, OBJ_SIZE_64X64, COLOR256, LEFTARR, 0, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, LEFTARR, 0, 2, 1, ANIM_LOOP, -1);
	
	PA_LoadSpritePal(SUB_SCREEN, RIGHTARR, (void*)ArrowRight_Pal);
	PA_CreateSprite(SUB_SCREEN, RIGHTARR, (void*)ArrowRight_Sprite, OBJ_SIZE_64X64, COLOR256, RIGHTARR, 192, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, RIGHTARR, 0, 2, 1, ANIM_LOOP, -1);
	// loads and animates the touch smash arrows on the screen
} // displays the minimap on the sub screen
#endif
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
#ifdef SHADOW_ON
	PA_CreateSprite(SUB_SCREEN, SHADOW, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 0, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, SHADOW, SHADOW, SHADOW, 1, ANIM_LOOP, -1);
#endif
	// etc, etc, etc

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
	// frame 0 is blank

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
		if(Pad.Newpress.R) selecting++;
		if(Pad.Newpress.L) selecting--;
		// changes who is choosing
		if(selecting < 0) selecting = 3;
		if(selecting > 3) selecting = 0;
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
#ifdef SHADOW_ON
			else if(humanselected == SHADOW) players.push_back(new Shadow(512/2 -96 -32, 256/3 -32, 1));
#endif	
			// adds a new player class (fighter*) for the human
					
			if(cpu1selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 2, true));		 
#ifdef SHADOW_ON
			else if(cpu1selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 2, true));
#endif			

			if(cpu2selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 3, true));		 
#ifdef SHADOW_ON
			else if(cpu2selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 3, true));
#endif					

			if(cpu3selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 4, true));		 
#ifdef SHADOW_ON
			else if(cpu3selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 4, true));
#endif		
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
//					else if(n == SHADOW) AS_SoundQuickPlay(shadow);
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
#ifdef MINIMAP_ON
void displaySubScreen() {
	if(players.size() >= 1) PA_SetSpriteXY(SUB_SCREEN, P1MINI, (int)((players[0] -> x)/8 + 64 + 256/8), (int)((players[0] -> y)/8 + 64 + 256/8));
	if(players.size() >= 2) PA_SetSpriteXY(SUB_SCREEN, P2MINI, (int)((players[1] -> x)/8 + 64 + 256/8), (int)((players[1] -> y)/8 + 64 + 256/8));
	if(players.size() >= 3) PA_SetSpriteXY(SUB_SCREEN, P3MINI, (int)((players[2] -> x)/8 + 64 + 256/8), (int)((players[2] -> y)/8 + 64 + 256/8));
	if(players.size() >= 4) PA_SetSpriteXY(SUB_SCREEN, P4MINI, (int)((players[3] -> x)/8 + 64 + 256/8), (int)((players[3] -> y)/8 + 64 + 256/8));
	// displays the two sprites in the correct position on a scaled down map
	PA_SetSpriteXY(SUB_SCREEN, MINIBOX, (int)((scrollx/8)+64 + 256/8), (int)((scrolly/8)+64 + 256/8));
	// displays a box representing the current screen
} // displays the minimap
#endif
void displayPercentages() {
	PA_OutputText(SUB_SCREEN, 0, 0, "                                           ");
	PA_OutputText(SUB_SCREEN, 0, 23, "                                           ");
	// clears the text
	if(players.size() >=1) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_RED);
		int damage = (int)(players[0]->getDamagePercent());
		PA_OutputText(SUB_SCREEN, 0, 0, "%d %", damage);
	} // displays damage percent of player 1
	if(players.size() >= 2) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_BLUE);
		int damage = (int)(players[1]->getDamagePercent());
		stringstream ss;
		ss << damage;
		int length = ss.str().size();
		PA_OutputText(SUB_SCREEN, 30-length, 0, "%d %", damage);
	} // displays damage percent of player 2
	if(players.size() >= 3) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_YELLOW);
		int damage = (int)(players[2]->getDamagePercent());
		PA_OutputText(SUB_SCREEN, 0, 23, "%d %", damage);
	} // damage of player 3
	if(players.size() >= 4) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_GREEN);
		int damage = (int)(players[3]->getDamagePercent());
		stringstream ss;
		ss << damage;
		int length = ss.str().size();
		PA_OutputText(SUB_SCREEN, 30-length, 23, "%d %", damage);
	}
}

void displayResults() {		
	PA_ResetBg(MAIN_SCREEN);
	PA_ResetBg(SUB_SCREEN);
	// resets the background for both screens
	
	PA_Init8bitBg(MAIN_SCREEN, 3);
	PA_Init8bitBg(SUB_SCREEN, 3);
	// initializes a gif on both screens
#ifdef MINIMAP_ON
	for(int n = 0; n < MINIBOX+1; n++) {
		PA_StopSpriteAnim(SUB_SCREEN,n);
		PA_DeleteSprite(SUB_SCREEN,n);
	} // stops and deletes minimap object sprites
#endif
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
#ifdef SHADOW_ON
		if(players[winner] -> name == "shadow") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/shadowwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/shadowwin2.gif");
			}
		}
#endif
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
		int SDs = score.getSDs(n);
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
			for(int n = 0; n < players.size(); n++) {
				players[n] -> deleteSprite();
				delete players[n];
			} // deletes the sprites of all players
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

// game modes
void timeMatch(int minutes) {
	int time = minutes*60*60 + 60; // minutes -> vblanks
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
#ifdef MINIMAP_ON	
	initMinimap(stagename); // inits minimap
#endif
	PA_InitText(SUB_SCREEN,1); // inits test on sub screen (displays percentages)
	PA_SetTextCol(SUB_SCREEN, 31,31,31); // text color = white

	// initializes scoreboard
	score = Scoreboard(players.size()); // initializes a new scoreboard

	fadeIn();

#ifdef SFX_ON		
	AS_SoundQuickPlay(three);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(two);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(one);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(go);
#endif	
	// counts down to start game
																																										
	while(true) {
		PA_CheckLid(); // if the lid is closed it pauses
		if(Pad.Newpress.Start) Pause(); 
		// checks to see if the game was paused by start button
		if(time-60 == 0) {
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
			PA_OutputText(MAIN_SCREEN, 13, 0, "0:00"); // displays 0 as the time
			for(int n = 0; n < 60; n++) PA_WaitForVBL(); // waits for 1 second
			fadeOut();
			return displayResults();
		}
		for(int n = 0; n < players.size(); n++) {
			players[n] -> act();
		} // all players act
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
		for(int n = 0; n < effects.size(); n++) {
			effects[n].act();
		}
#endif
		// acts all effects
#ifdef MINIMAP_ON
		displaySubScreen(); // changes sub screen display
#endif
		displayPercentages(); // displays percentages on sub screen
		PA_OutputText(MAIN_SCREEN, 13,0, "          "); // clears time
		if((int)((time/60)%60) < 10) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:0%d",(int)((time/60)/60), (int)((time/60)%60));
		else if((int)((time/60)%60) == 0) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:00",(int)((time/60)/60));
		else PA_OutputText(MAIN_SCREEN, 13, 0, "%d:%d", (int)((time/60)/60), (int)((time/60)%60));		
		// redisplays time
		printMemoryUsage();
		PA_WaitForVBL();
		time--; // another tick off the clock!
	}
}
void stockMatch(int stockcount) {

} // stock match, uncoded
void trainingMode() {

} // training mode, uncoded
void options() {

} // options menu, uncoded
void extras() {

} // extras menu, uncoded

#import "LAN.cpp"

// pre-game menus
void mainMenu() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	// opens gif background. no need to reinit, just loads over the old gif for this screen.

#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/mainmenu.mp3");
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

	fadeIn();

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
					if(n == 0) return timeMatch(1);
					if(n == 1) {
#ifdef LAN_ON
						return LAN();
#else
						return;
#endif					
					}
					if(n == 2) {
						return options();
					}
					if(n == 3) {
						return extras();
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
	AS_MP3StreamPlay("SSBDS_Files/music/title.mp3");
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

	defaultExceptionHandler(); // "red screen of death" error, hopefully won't happen

	if(!fatInitDefault()) {
		PA_InitText(MAIN_SCREEN, 0);
		PA_SetTextCol(MAIN_SCREEN, 31,31,31);
		PA_OutputText(MAIN_SCREEN, 0,0, "FAT INIT FAILED!!!");
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

	AS_Init(AS_MODE_MP3 | AS_MODE_16CH); // initializes AS_Lib
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound

#ifdef LAN_ON
	PA_VBLFunctionInit(customVBL); // custom VBL function called every frame
#else
	PA_VBLFunctionInit(AS_SoundVBL); // easy way to make sure that AS_SoundVBL() is called every frame
#endif
	
	while(true) titleScreen(); // permanently runs the game
	return 0;
} // End of main()

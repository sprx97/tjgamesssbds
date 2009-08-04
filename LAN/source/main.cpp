// Includes
#include <PA9.h>       // Include for PA_Lib
#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h>
// DS <-> DS
#include "nds_loader_arm9.h"

//C++ Library Functions:
#include <math.h> // math!
#include <vector> // vectors!
#include <string> // strings!
#include <sstream> // int to string
#include <stdio.h> // standard file functions
#include <stdlib.h> // standard C functions
#include <map> // maps

#include "gfx/all_gfx.c"
#include "efs_lib.h"

using namespace std;

//Project classes:
#include "global.h"
#include "effect.h"
#include "stages.h"
#include "projectiles.h" // projectiles
#include "fighters.h" // individual characters
#include "fighter.h"
#include "display.h"
#include "scoreboard.h"

#define ROOM_NAME "SSBDS"
#define MAX_PLAYERS 2
#define GAME_CODE 558
#define GAME_VER 0

int playernum = -1;
bool charactersselected = false;

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
map<int , int> getcustomcontrols() {
	return customcontrols;
}
bool getTapJumpOn() {
	return tapjumpon;
}
bool getCStickStylus() {
	return cstickstylus;
}
bool getShieldGrabOn() {
	return shieldgrabon;
}
void* getProj() {
	return &projectiles;
}
void removeProj(int prnum) {
	vector<Projectile> temp;
	for (int n = 0; n < (int)projectiles.size(); n++) {
		Projectile p = projectiles[n];
		if (p.num != prnum) {
			temp.push_back(p);
		}
	}
	projectiles = temp;
	PA_SetSpriteXY(MAIN_SCREEN, prnum, -64, -64);
}
bool custom_action(int action, int typecheck) {
	if (customcontrols[action] == BUTTON_A) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.A) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.A) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.A) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_B) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.B) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.B) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.B) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_AB) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.A && Pad.Held.B) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.A && Pad.Newpress.B) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.A || Pad.Released.B) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_X) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.X) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.X) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.X) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_Y) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.Y) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.Y) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.Y) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_L) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.L) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.L) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.L) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_R) {
		if (typecheck == PAD_HELD) {
			if (Pad.Held.R) return true;
		}
		else if (typecheck == PAD_NEWPRESS) {
			if (Pad.Newpress.R) return true;
		}
		else if (typecheck == PAD_RELEASED) {
			if (Pad.Released.R) return true;
		}
	}
	else if (customcontrols[action] == BUTTON_NONE) {
		return false;
	}
	if (action == ACTION_JUMP) return custom_action(ACTION_JUMP2, typecheck);
	if (action == ACTION_SHIELD) return custom_action(ACTION_SHIELD2, typecheck);
	return false;
} // takes action and checks if it is done by custom controls

char* gifbuffer = NULL; // the array which stores the gif being printed
void openGif(int screen, string path) {
	delete gifbuffer; // clears the gif buffer
	FILE* imgFile = fopen(path.c_str(), "rb");  // opens the file at path for reading
	if (imgFile) {
		u32 imgSize;
		fseek(imgFile, 0 , SEEK_END);
		imgSize = ftell(imgFile);
		rewind(imgFile);
		// obtains file size

		gifbuffer = NULL;
		gifbuffer = (char*) malloc(sizeof(char) * imgSize);
		fread(gifbuffer, 1, imgSize, imgFile);
		// allocate memory to store the image file

		fclose(imgFile);  // close the file
		PA_LoadGif(screen, (void *)gifbuffer); // Show gif on screen
	} // loads the gif if the image file exists
} // opens the gif at path onto screen

vector<int> mainx;
vector<int> subx;
void fadeOut() {
	for (int i = 0; i >= -31; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		AS_SetMP3Volume((i + 31)*4);
		for (int n = 0; n < 16; n++) AS_SetSoundVolume(n, (i + 31)*4);
		PA_WaitForVBL();
		PA_WaitForVBL();
	} // slowly darkens the screen into black
	AS_MP3Stop();
	PA_ResetBgSys();
	PA_FatFreeSfxBuffers();
} // fades both screens out
void fadeIn() {
	for (int i = -31; i <= 0; i++) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		AS_SetMP3Volume((i + 31)*4);
		for (int n = 0; n < 16; n++) AS_SetSoundVolume(n, (i + 31)*4);
		PA_WaitForVBL();
		PA_WaitForVBL();
	} // slowly brightens the screen to normal
} // fades both screens in

void initFX() {
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, 15, "specialFX");
	PA_FatLoadSprite(254, "specialFX");
	for (int n = 5; n < 9; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)sprite_gfx[254], OBJ_SIZE_64X64, COLOR256, 15, -64, -64);
	}
	// loads all special effect sprites
} // initializes special effects
void initProjectiles() {
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, 14, "projectiles");
	PA_FatLoadSprite(255, "projectiles");
	for (int n = 50; n < 54; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)sprite_gfx[255], OBJ_SIZE_64X64, COLOR256, 14, -64, -64);
	} // loads all projectile sprites
} // initializes projectiles

#import "minimap.cpp" // minimap for during battles

Stage setStage(int selStage) {
	Stage picked; // the stage which is chosen
	if (selStage == FINALDESTINATION) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, finaldestination);
		picked = FinalDestination();
	} // loads final destination if it was chosen
	if (selStage == POKEMONSTADIUM) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, pokemonstadium);
		picked = PokemonStadium();
	} // loads pokemon stadium if it was chosen
	if (selStage == CASTLESIEGE) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, castlesiege);
		picked = CastleSiege();
	} // loads castle siege if it was chosen
	if (selStage == CORNERIA) {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, corneria);
		picked = Corneria();
	} // loads corneria if it was chosen
	for (int n = 0; n < (int)players.size(); n++) {
		players[n] -> setStage(&picked);
	} // sets the stage of the players to the picked stage
	return picked; // returns the picked stage
} // displays the stage on the main screen

void scrollScreen() {
	if (cameratype == CAMERATYPE_FOLLOWALL) {
		double maxx = -10000;
		double minx = 10000;
		double maxy = -10000;
		double miny = 10000;
		// initializes min and max x and y
		for (int n = 0; n < (int)players.size(); n++) {
			if (score.getDeaths(n) + sdcost*score.getSDs(n) < stocklimit && gamemode == GAMEMODE_STOCK) {
				double x = players[n] -> x;
				double y = players[n] -> y;
				if (x > maxx) maxx = x;
				if (x < minx) minx = x;
				if (y > maxy) maxy = y;
				if (y < miny) miny = y;
			}
		} // finds the minimum and maximum x and y position
		scrollx = (maxx + minx) / 2;
		scrolly = (maxy + miny) / 2;
		// scrolls so that the camera is centred between the min and max x and y
	} // if the camera follows everyone
	else if (cameratype == CAMERATYPE_FOLLOWUSER) {
		scrollx = players[playernum] -> x;
		scrolly = players[playernum] -> y;
		// centers camera on user
	} // if the camer follows the user
	scrollx = scrollx - 128 + 32;
	scrolly = scrolly - 96 + 32;
	// centers the camera on the sprites and screen
	if (scrollx < -128) scrollx = -128;
	if (scrolly < -192) scrolly = -192;
	if (scrollx > 512 - 256 + 128) scrollx = 512 - 256 + 128;
	if (scrolly > 256 - 192) scrolly = 256 - 192;
	// wraps scrolling around
	PA_LargeScrollX(MAIN_SCREEN, 0, 256 + (int)scrollx);
	PA_LargeScrollY(MAIN_SCREEN, 0, 256 + (int)scrolly);
	// scrolls the screen
	for (int n = 0; n < (int)players.size(); n++) {
		players[n] -> scroll(scrollx, scrolly);
	} // scrolls the players
	for (int n = 0; n < (int)effects.size(); n++) {
		if (effects[n].type == FX_DEATH) {} // don't move sprite
		else if (effects[n].type == FX_AIRJUMP) PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM) + 32);
		else PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM));
	} // scrolls the special effects
}

void customVBL(void) {
	IPC_RcvCompleteCheck();
	LOBBY_Update();
	AS_SoundVBL();
}

LPLOBBY_ROOM LOBBY_GetMyRoom() {
	return LOBBY_GetRoomByUser(LOBBY_GetUserByID(USERID_MYSELF));
}

bool receiving = false;
void ReceiveCharsel(unsigned char *data, int length, LPLOBBY_USER from) {
	if(receiving) return; // don't interrupt
	receiving = true;
	if((bool)(data[4])) {
		fadeOut();
		int selections[] = { -1, -1, -1, -1};
		for (int n = 0; n < 4; n++) {
			int x = PA_GetSpriteX(SUB_SCREEN, n) + 16;
			int y = PA_GetSpriteY(SUB_SCREEN, n) + 16;
			for (int m = KIRBY; m < MAX_CHAR; m++) {
				if (x > PA_GetSpriteX(SUB_SCREEN, m + 4) && x < PA_GetSpriteX(SUB_SCREEN, m + 4) + 64 && y > PA_GetSpriteY(SUB_SCREEN, m + 4) && y < PA_GetSpriteY(SUB_SCREEN, m + 4) + 64) {
					selections[n] = m;
				}
			}
		}
		PA_ResetSpriteSys();
		PA_FatFreeSprBuffers();
		PA_LoadSpritePal(MAIN_SCREEN, 13, (void*)shield_Pal);
		for(int n = 0; n < 4; n++) {
			if (selections[n] == KIRBY) players.push_back(new Kirby(n + 1, &players, &display, false));
			else if (selections[n] == MEWTWO) players.push_back(new Mewtwo(n + 1, &players, &display, false));
			else if (selections[n] == MARIO) players.push_back(new Mario(n + 1, &players, &display, false));
			else if (selections[n] == IKE) players.push_back(new Ike(n + 1, &players, &display, false));
			else if (selections[n] == FOX) players.push_back(new Fox(n + 1, &players, &display, false));
		}
		charactersselected = true;
	}
	else {
		int spr = (int)data[0];
		int x = (int)data[1];
		int y = (int)data[2];
		bool released = (bool)data[3];
		PA_SetSpriteXY(SUB_SCREEN, spr, x, y);
		if(released) {
			int cx = PA_GetSpriteX(SUB_SCREEN, spr) + 16;
			int cy = PA_GetSpriteY(SUB_SCREEN, spr) + 16;
			bool onchar = false;
			for(int n = KIRBY; n < MAX_CHAR; n++) {
				if (cx > PA_GetSpriteX(SUB_SCREEN, n + 4) && cx < PA_GetSpriteX(SUB_SCREEN, n + 4) + 64 && cy > PA_GetSpriteY(SUB_SCREEN, n + 4) && cy < PA_GetSpriteY(SUB_SCREEN, n + 4) + 64) {
					onchar = true;
					PA_StartSpriteAnimEx(MAIN_SCREEN, spr, n, n, 1, ANIM_LOOP, -1);
				}
			}
			if(!onchar) PA_StartSpriteAnimEx(MAIN_SCREEN, spr, 0, 0, 1, ANIM_LOOP, -1);
		}
	}
	receiving = false;
}
void ReceiveMatch(unsigned char *data, int length, LPLOBBY_USER from) {
	players[(int)(data[0])] -> x = ((int)(data[1]) * (int)(data[2])) - 256;
	players[(int)(data[0])] -> y = ((int)(data[3]) * (int)(data[4])) - 256;
	PA_SetSpriteAnimFrame(MAIN_SCREEN, players[(int)(data[0])]->SPRITENUM, (int)(data[5]));
	players[(int)(data[0])] -> setDirection((int)(data[6] - 1));
	players[(int)(data[0])] -> action = (int)(data[7]);
}

void initControls() {
	FILE* file = fopen("/SSBDS_Files/saves/controls.sav", "rb");
	if (file) {
		u32 size;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		char line[4];
		char line2[4];
		int n = 0;
		while (n < 16) {
			fgets(line, 4, file);
			fgets(line2, 4, file);
			customcontrols[atoi(strtok(line, " \t"))] = atoi(strtok(line2, " \t"));
			n += 2;
		}
		fgets(line, 4, file);
		if (atoi(strtok(line, " \t")) == 1) cstickstylus = true;
		else cstickstylus = false;
		fgets(line, 4, file);
		if (atoi(strtok(line, " \t")) == 1) shieldgrabon = true;
		else shieldgrabon = false;
		fgets(line, 4, file);
		if (atoi(strtok(line, " \t")) == 1) tapjumpon = true;
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

int selectedstage = -1;

void characterSelectLAN() {
	PA_Init8bitBg(SUB_SCREEN, 3);
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/default.gif");

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/default.gif");

	PA_FatEasyLoadSpritePal(SUB_SCREEN, 0, "cursors");
	PA_FatLoadSprite(0, "cursors");
	for (int n = 0; n < 2; n++) { 
		PA_CreateSprite(SUB_SCREEN, n, (void*)sprite_gfx[0], OBJ_SIZE_32X32, COLOR256, 0, n*48 + 40, 152);
		PA_StartSpriteAnimEx(SUB_SCREEN, n, n, n, 1, ANIM_LOOP, -1);
	} // only 2 players for now

	PA_FatEasyLoadSpritePal(SUB_SCREEN, 1, "charsel");
	PA_FatLoadSprite(1, "charsel");
	for (int n = KIRBY; n < MAX_CHAR; n++) {
		PA_CreateSprite(SUB_SCREEN, 4 + n, (void*)sprite_gfx[1], OBJ_SIZE_64X64, COLOR256, 1, ((n - 1) % 3)*80 + 16, ((n - 1) / 3)*72);
		PA_StartSpriteAnimEx(SUB_SCREEN, 4 + n, n, n, 1, ANIM_LOOP, -1);
	}
	PA_LoadSpritePal(MAIN_SCREEN, 0, (void*)charprev_Pal);
	for (int n = 0; n < 2; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)charprev, OBJ_SIZE_64X64, COLOR256, 0, 64*n, 128);
		PA_StartSpriteAnimEx(MAIN_SCREEN, n, 0, 0, 1, ANIM_LOOP, -1);
	} // only 2 players for now

	fadeIn();

	int selectedcursor = -1;
	while (true) {
		if(charactersselected) return;
		if (Stylus.Newpress) {
			for (int n = 0; n < 4; n++) {
				if (PA_SpriteTouched(n)) selectedcursor = n;
			}
		}
		else if (Stylus.Held && selectedcursor != -1) {
			PA_SetSpriteXY(SUB_SCREEN, selectedcursor, Stylus.X - 16, Stylus.Y - 16);
			char data[10];
			data[0] = (char)selectedcursor;
			data[1] = (char)(Stylus.X - 16);
			data[2] = (char)(Stylus.Y - 16);
			data[3] = (char)(false);
			data[4] = (char)(false);
			LOBBY_SendToRoom(LOBBY_GetMyRoom(), 0x0001, (unsigned char*)data, 10);
		}
		else if(Stylus.Released && selectedcursor != -1) {
			int cx = PA_GetSpriteX(SUB_SCREEN, selectedcursor) + 16;
			int cy = PA_GetSpriteY(SUB_SCREEN, selectedcursor) + 16;
			bool onchar = false;
			for(int n = KIRBY; n < MAX_CHAR; n++) {
				if(cx > PA_GetSpriteX(SUB_SCREEN, n+4) && cx < PA_GetSpriteX(SUB_SCREEN, n+4) + 64 && cy > PA_GetSpriteY(SUB_SCREEN, n+4) && cy < PA_GetSpriteY(SUB_SCREEN, n+4) + 64) {
					onchar = true;
					PA_StartSpriteAnimEx(MAIN_SCREEN, selectedcursor, n, n, 1, ANIM_LOOP, -1);
				}
			}
			if(!onchar) PA_StartSpriteAnimEx(MAIN_SCREEN, selectedcursor, 0, 0, 1, ANIM_LOOP, -1);
						
			char data[10];
			data[0] = (char)selectedcursor;
			data[1] = (char)(Stylus.X - 16);
			data[2] = (char)(Stylus.Y - 16);
			data[3] = (char)(true);
			data[4] = (char)(false);
			LOBBY_SendToRoom(LOBBY_GetMyRoom(), 0x0001, (unsigned char*)data, 10);	
			
			selectedcursor = -1;
		}
		else if(Pad.Newpress.A || Pad.Newpress.Start) {
			int selections[] = { -1, -1, -1, -1};
			for (int n = 0; n < 4; n++) {
				int x = PA_GetSpriteX(SUB_SCREEN, n) + 16;
				int y = PA_GetSpriteY(SUB_SCREEN, n) + 16;
				for (int m = KIRBY; m < MAX_CHAR; m++) {
					if (x > PA_GetSpriteX(SUB_SCREEN, m + 4) && x < PA_GetSpriteX(SUB_SCREEN, m + 4) + 64 && y > PA_GetSpriteY(SUB_SCREEN, m + 4) && y < PA_GetSpriteY(SUB_SCREEN, m + 4) + 64) {
						selections[n] = m;
					}
				}
			}
			if(selections[0] != -1 && selections[1] != -1) {
				char data[10];
				data[4] = (char)(true);
				LOBBY_SendToRoom(LOBBY_GetMyRoom(), 0x0001, (unsigned char*)data, 10);
				fadeOut();
				PA_ResetSpriteSys();
				PA_FatFreeSprBuffers();
				PA_LoadSpritePal(MAIN_SCREEN, 13, (void*)shield_Pal);
				for(int n = 0; n < 4; n++) {
					if (selections[n] == KIRBY) players.push_back(new Kirby(n + 1, &players, &display, false));
					else if (selections[n] == MEWTWO) players.push_back(new Mewtwo(n + 1, &players, &display, false));
					else if (selections[n] == MARIO) players.push_back(new Mario(n + 1, &players, &display, false));
					else if (selections[n] == IKE) players.push_back(new Ike(n + 1, &players, &display, false));
					else if (selections[n] == FOX) players.push_back(new Fox(n + 1, &players, &display, false));
				}
				charactersselected = true;
			}
		}
		PA_WaitForVBL();
	}
}
void LANmatch() {
	fadeIn();

	PA_InitText(MAIN_SCREEN, 0);
	PA_SetTextCol(MAIN_SCREEN, 31, 31, 31);
	PA_OutputText(MAIN_SCREEN, 0, 0, "Starting LAN game.");
	PA_OutputText(MAIN_SCREEN, 1, 1, "Press Start to host.");
	PA_OutputText(MAIN_SCREEN, 1, 2, "Press Select to join.");

	while (playernum == -1) {
		if (Pad.Newpress.Start) {
			PA_OutputText(MAIN_SCREEN, 0, 1, "                                 ");
			PA_OutputText(MAIN_SCREEN, 0, 2, "                                 ");
			PA_OutputText(MAIN_SCREEN, 0, 1, "You are the host.");
			playernum = 0;
			LOBBY_SetOwnName("Host");
			LOBBY_CreateRoom(ROOM_NAME, MAX_PLAYERS, GAME_CODE, GAME_VER);
		}
		else if (Pad.Newpress.Select) {
			PA_OutputText(MAIN_SCREEN, 0, 1, "                                 ");
			PA_OutputText(MAIN_SCREEN, 0, 2, "                                 ");
			PA_OutputText(MAIN_SCREEN, 0, 1, "You are the client.");
			playernum = 1;
			LOBBY_SetOwnName("Client");
			LOBBY_JoinRoom(LOBBY_GetRoomByGame(0, GAME_CODE));
		}
		PA_WaitForVBL();
	}
	PA_OutputText(MAIN_SCREEN, 0, 2, "Waiting for more players.");
	while (LOBBY_GetUsercountInRoom(LOBBY_GetMyRoom()) != 2) PA_WaitForVBL();
	PA_OutputText(MAIN_SCREEN, 0, 3, "Opponent(s) found!");

	fadeOut();

	characterSelectLAN();
	for (int n = 0; n < (int)players.size(); n++) {
		players[n] -> players = players;
	}
	
	initFX();
	initProjectiles();
	
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, 12, "revivalplatform");
	PA_FatLoadSprite(253, "revivalplatform");
	for (int n = 55; n < 60; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)sprite_gfx[253], OBJ_SIZE_64X64, COLOR256, 12, -64, -64);
	}
	
	selectedstage = FINALDESTINATION;
	Stage stage = setStage(selectedstage);
	PA_LargeScrollX(MAIN_SCREEN, 0, stage.width / 2 + 128);
	PA_LargeScrollY(MAIN_SCREEN, 0, stage.height / 2 + 96);
	
	PA_FatLoadSfx("shieldbreak", "shieldbreak");
	PA_FatLoadSfx("hit1", "hit1");
	PA_FatLoadSfx("hit2", "hit2");
	PA_FatLoadSfx("hit3", "hit3");
	PA_FatLoadSfx("death", "deathsound");	
	
	for (int n = 0; n < (int)players.size(); n++) {
		players[n] -> initSounds();
	}
	
	for (int n = 0; n < (int)players.size(); n++) {
		players[n] -> fall();
		while (players[n] -> aerial) {
			players[n] -> fall();
			players[n] -> move();
			mainx[players[n]->SPRITENUM] = PA_GetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM);
			PA_SetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM, -64);
		}
		players[n] -> idle();
		mainx[players[n]->SPRITENUM] = PA_GetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM);
		PA_SetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM, -64);
	}
	
	scrollScreen();
	
	for (int n = 0; n < (int)players.size(); n++) {
		mainx[players[n]->SPRITENUM] = PA_GetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM);
		PA_SetSpriteX(MAIN_SCREEN, players[n]->SPRITENUM, -64);
	}	
	
	PA_InitText(SUB_SCREEN, 0);
	PA_SetTextCol(SUB_SCREEN, 31, 31, 31);
	
	fadeIn();
	
	while(true) {
		players[playernum] -> act();
		char data[10];
		data[0] = (char)playernum;
		double mult = 1;
		double val = (players[playernum] -> x) + 256;
		while(val > 256) {
			val /= 2;
			mult *= 2;
		}
		data[1] = (char)val;
		data[2] = (char)mult;
		mult = 1;
		val = (players[playernum] -> y) + 256;
		while(val > 256) {
			val /= 2;
			mult *= 2;
		}
		data[3] = (char)val;
		data[4] = (char)mult;
		data[5] = (char)PA_GetSpriteAnimFrame(MAIN_SCREEN, players[playernum]->SPRITENUM);
		data[6] = (char)(players[playernum] -> direction + 1);
		data[7] = (char)(players[playernum] -> action);
		LOBBY_SendToRoom(LOBBY_GetMyRoom(), 0x0002, (unsigned char*)data, 10);
		for(int n = 0; n <  (int)players.size(); n++) {
			if(n != playernum) {
				players[playernum] = players[n] -> checkHits(players[playernum]);
			}
		}
		PA_OutputText(SUB_SCREEN, 0, 0, "                       ");
		PA_OutputText(SUB_SCREEN, 0, 0, "%d %d %d %d %d %d %d %d", (int)data[0], (int)data[1], (int)data[2], (int)data[3], (int)data[4], (int)data[5], (int)data[6], (int)data[7]);
//		for (int n = 0; n < (int)players.size(); n++) {
//			for (int m = 0; m < (int)players.size(); m++) {
//				if (m != n) players[m] = players[n] -> checkHits(players[m]);
//			}
//		}
//		for (int n = 0; n < (int)players.size(); n++) players[n]->allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, players[n]->SPRITENUM)].enabled = false;
		scrollScreen();
//		for (int n = 0; n < (int)projectiles.size(); n++) {
//			if (projectiles[n].act()) removeProj(projectiles[n].num);
//			for (int m = 0; m < (int)players.size(); m++) if (projectiles[n].owner != m) players[m] = projectiles[n].checkHits(players[m]);
//		}		
		display.updateEffects(); // acts all effects		
		PA_WaitForVBL();
	}
}

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	defaultExceptionHandler();

	PA_VBLFunctionInit(customVBL);
	if (!IPC_Init()) {
		PA_OutputText(MAIN_SCREEN, 0, 0, "IPC INIT FAILED!!!");
		while (true) {}
	}
	IPC_SetChannelCallback(0, &LWIFI_IPC_Callback);
	LOBBY_Init();
	// inits/preps DS <-> DS

	LOBBY_SetStreamHandler(0x0001, &ReceiveCharsel);
	LOBBY_SetStreamHandler(0x0002, &ReceiveMatch);

	if (!EFS_Init(EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL)) {
		PA_OutputText(0, 1, 1, "EFS init error!!!");
		while (true) {}
	}
	PA_FatInitAllBuffers(); // Initialize all the memory buffers
	PA_FatSetBasePath("SSBDS_Files");  // Set a base path

	for (int n = 0; n < 128; n++) {
		mainx.push_back(-64);
		subx.push_back(-64);
	}
	fadeOut();

	AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound

	initControls();

	// Infinite loop to keep the program running
	while (1) LANmatch();
	return 0;
} // End of main()

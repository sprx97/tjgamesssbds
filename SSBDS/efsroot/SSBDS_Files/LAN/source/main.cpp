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

#include "../../../../source/gfx/all_gfx.c"

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

char senddata[10];
int playernum = -1;
bool received = false;

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

void customVBL(void) {
	IPC_RcvCompleteCheck();
	LOBBY_Update();
	AS_SoundVBL();
}

void receive(unsigned char *data, int length, LPLOBBY_USER from) {
	received = true;
	PA_SetSpriteXY(MAIN_SCREEN, players[0] -> SPRITENUM, data[0], data[1]);
	PA_SetSpriteAnimFrame(MAIN_SCREEN, players[0] -> SPRITENUM, data[2]);
}

void LANGame() {
	fadeIn();
	PA_InitText(MAIN_SCREEN, 0);
	PA_SetTextCol(MAIN_SCREEN, 31, 31, 31);	
	PA_OutputText(MAIN_SCREEN, 0, 0, "LAN game began");
	while(playernum == -1) {
		if(Pad.Newpress.Start) {
			PA_OutputText(MAIN_SCREEN, 0, 1, "You are player 0, the host");
			playernum = 0;
			LOBBY_SetOwnName("Host");
			LOBBY_CreateRoom(ROOM_NAME, MAX_PLAYERS, GAME_CODE, GAME_VER);
		}
		else if(Pad.Newpress.Select) {
			PA_OutputText(MAIN_SCREEN, 0, 1, "You are player 1, the client");
			playernum = 1;
			LOBBY_SetOwnName("Client");
			LOBBY_JoinRoom(LOBBY_GetRoomByGame(0, GAME_CODE));
		}
		PA_WaitForVBL();
	}
	PA_OutputText(MAIN_SCREEN, 0, 2, "Waiting for more players");
	while(LOBBY_GetUsercountInRoom(LOBBY_GetRoomByUser(LOBBY_GetUserByID(USERID_MYSELF))) != 2) {
		PA_WaitForVBL();
	}
	PA_OutputText(MAIN_SCREEN, 0, 3, "Opponent found!");
	players.push_back(new Kirby(1, &players, &display));
	Stage stage = setStage(FINALDESTINATION);
	
	while(true) {
		if(playernum == 0) {
			players[0] -> act();
			senddata[0] = (int)(players[0] -> x);
			senddata[1] = (int)(players[0] -> y);
			senddata[2] = (int)(PA_GetSpriteAnimFrame(MAIN_SCREEN, players[0]->SPRITENUM));
			LOBBY_SendToUser(LOBBY_GetUserByID(0), 0x0001, (unsigned char*)senddata, 10);
		}
		else if(playernum == 1) {
			while(!received) {}
			received = false;
		}
		scrollScreen();
		PA_WaitForVBL();
	}
}

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	defaultExceptionHandler();

	if(!fatInitDefault()) {
		PA_InitText(MAIN_SCREEN, 0);
		PA_SetTextCol(MAIN_SCREEN, 31, 31, 31);
		PA_OutputText(MAIN_SCREEN, 0, 0, "FAT INIT FAILED!!!");
		while(true) {}
	} // Init for libfat. if it fails it freezes with an error
	PA_FatInitAllBuffers(); // Initialize all the memory buffers
	PA_FatSetBasePath("SSBDS_Files");  // Set a base path
	// initializes external file system. very important!!!

	fadeOut();

	PA_VBLFunctionInit(AS_SoundVBL); // easy way to make sure that AS_SoundVBL() is called every frame
    AS_Init(AS_MODE_MP3 | AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound

	if(!IPC_Init()) {
		PA_OutputText(MAIN_SCREEN, 0, 0, "IPC INIT FAILED");
		while(true) {}
	}
	IPC_SetChannelCallback(0, &LWIFI_IPC_Callback);
	PA_VBLFunctionInit(customVBL);
	// inits/preps DS <-> DS	
			
	LOBBY_Init();
	LOBBY_SetStreamHandler(0x0001, &receive);

	// Infinite loop to keep the program running
	while (1) {
		LANGame();
	}
	return 0;
} // End of main()

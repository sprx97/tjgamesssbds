// Includes
#include <PA9.h>       // Include for PA_Lib
#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h>
// DS <-> DS

#include "global.h"

#include <vector>

#define DEBUG_ON

#define ROOM_NAME "SSBDS"
#define MAX_PLAYERS 2
#define GAME_CODE 558
#define GAME_VER 0

char senddata[10];
int playernum = -1;
bool received = false;

//vector<Fighter*> players;

void customVBL(void) {
	IPC_RcvCompleteCheck();
	LOBBY_Update();
	AS_SoundVBL();
}

void receive(unsigned char *data, int length, LPLOBBY_USER from) {
	received = true;
//	PA_SetSpriteXY(MAIN_SCREEN, players[0] -> SPRITENUM, data[0], data[1]);
//	PA_SetSpriteAnimFrame(MAIN_SCREEN, players[0] -> SPRITENUM, data[2]);
}

void waitForGame() {
	while(playernum == -1) {
		if(Pad.Newpress.Start) {
			playernum = 0;
			LOBBY_SetOwnName("Host");
			LOBBY_CreateRoom(ROOM_NAME, MAX_PLAYERS, GAME_CODE, GAME_VER);
		}
		else if(Pad.Newpress.Select) {
			playernum = 1;
			LOBBY_SetOwnName("Client");
			LOBBY_JoinRoom(LOBBY_GetRoomByGame(0, GAME_CODE));
		}
		PA_WaitForVBL();
	}
	while(LOBBY_GetUsercountInRoom(LOBBY_GetRoomByUser(LOBBY_GetUserByID(USERID_MYSELF))) != 2) PA_WaitForVBL();
//	players.push_back(new Kirby(1, &players, &display));
//	Stage stage = setStage(FINALDESTINATION);
	
	fadeIn();

//	while(true) {
//		if(playernum == 0) {
//			players[0] -> act();
//			senddata[0] = (int)(players[0] -> x);
//			senddata[1] = (int)(players[0] -> y);
//			senddata[2] = (int)(PA_GetSpriteAnimFrame(MAIN_SCREEN, players[0]->SPRITENUM);
//			LOBBY_SendToUser(LOBBY_GetUserByID(0), 0x0001, (unsigned char*)senddata);
//		}
//		else if(playernum == 1) {
//			while(!received) {}
//			received = false;
//		}
//		scrollscreen();
//		PA_WaitForVBL();
//	}
}

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

#ifdef DEBUG_ON
	defaultExceptionHandler();
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
		waitForGame();
	}
	return 0;
} // End of main()

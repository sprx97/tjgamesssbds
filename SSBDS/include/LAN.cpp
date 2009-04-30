#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h>
// DS <-> DS


#define ROOM_NAME "SSBDS"
#define MAX_PLAYERS 2
#define GAME_CODE 558
#define GAME_VER 0

char dat[10];
bool inLAN = false;
int playernum = -1;

void customVBL(void) {
	if(inLAN) {
		IPC_RcvCompleteCheck();
		LOBBY_Update();
	}
	AS_SoundVBL();
}

void receive(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inLAN) return;
}

void LAN() {
	inLAN = true;
	while(playernum == -1) {
		if(Pad.Newpress.Start) {
			playernum = 0;
			LOBBY_CreateRoom(ROOM_NAME, MAX_PLAYERS, GAME_CODE, GAME_VER);
		} // host a game
		else if(Pad.Newpress.Select) {
			playernum = 1;
			LOBBY_JoinRoom(LOBBY_GetRoomByGame(0, GAME_CODE));
		} // join a hosted game
	}
	
	players.push_back(new Kirby(1, &players, &display));
	Stage stage = setStage(FINALDESTINATION);
	fadeIn();
	
	while(true) {
	
	}
}


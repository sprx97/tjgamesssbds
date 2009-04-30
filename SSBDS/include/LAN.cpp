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
int playernumber = -1;

void customVBL(void) {
	IPC_RcvCompleteCheck();
	LOBBY_Update();
	AS_SoundVBL();
}

void receive(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inLAN) return;
}

void LAN() {

}


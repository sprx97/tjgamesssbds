#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h> // DS <-> DS!

#define GAMECODE 558 // lol SSB in 1337
#define GAMEVERSION 0 // use to avoid desynching b/c of changes
#define MAX_ROOMS 8
#define ROOM_NAMES 8
// variables for running a LAN game
#define MENU_MAIN 0 
#define MENU_CREATE_ROOM 1
// LAN Rooms

LPLOBBY_USER roomOwners[MAX_ROOMS];
LPLOBBY_ROOM myRoom;
char* roomNames[ROOM_NAMES] = {
	"SSBDS 1",
	"SSBDS 2",
	"SSBDS 3",
	"SSBDS 4",
	"SSBDS 5",
	"SSBDS 6",
	"SSBDS 7",
	"SSBDS 8"
};
int roomNameIndex = 0;
int roomMaxUsers = 2;
unsigned short roomGameCode = 558;
unsigned short roomGameVersion = 1;
int roomVisible = 1;
int roomCount = 0;
int logCount = 0;
int menuState = MENU_MAIN;
int selected = 0 ;

bool inlan = false;
void customVBL(void) {
	if(inlan) {
		IPC_RcvCompleteCheck();
		LOBBY_Update();
	}
	AS_SoundVBL();
}
void receiveGameData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	// update character based on data string
}
int roomSlotOf(LPLOBBY_USER user) {
	for(int n = 0; n < LOBBY_GetUsercountInRoom(myRoom); n++) {
		if(user==LOBBY_GetRoomUserBySlot(myRoom, n)) return n;
	}
}
void receiveCharData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(data == (unsigned char*)"kirby") {
		players[roomSlotOf(from)] = new Kirby(512/2 -32, 256/3 -32, roomSlotOf(from), 1);
	}
	if(data == (unsigned char*)"mewtwo") {
		players[roomSlotOf(from)] = new Mewtwo(512/2 -32, 256/3 -32, roomSlotOf(from), 1);
	}
}
void receiveUserData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(++logCount > 10) logCount=0;
	PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user msg %s: %s", LOBBY_GetUserName(from), data);
}
void receiveRoomData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(++logCount>10) logCount=0;
	PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- room msg %s: %s", LOBBY_GetUserName(from), data);
}
void userCallback(LPLOBBY_USER user, unsigned long reason) {
	if(!inlan) return;
	if(++logCount>10) logCount=0;
	switch(reason) {
		case USERINFO_REASON_REGOGNIZE:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s recognized", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_TIMEOUT:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s timeout", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_RETURN:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s returned", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_ROOMCREATED:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s created a room", LOBBY_GetUserName(user));
			for(int i=0;i<roomCount;i++) {
				if(roomOwners[i]==user)
					return;
			}
			roomOwners[roomCount] = user;
			roomCount++;
		break;
		case USERINFO_REASON_ROOMCHANGE:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s changed room", LOBBY_GetUserName(user));
			if(user==LOBBY_GetUserByID(USERID_MYSELF)) { // join/create room succeeded
				menuState=MENU_LOBBY;
				selected=0;
			}
		break;
	}
}
bool allSelected() {
	for(int n = 0; n < players.size(); n++) {
		if(players[n] == NULL) return false;
	}
	return true;
}
void LANgame() {
	if(!inlan) return;
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SetRoomVisibility(0); // hides room
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SendToRoom(myRoom, 0x0003, (unsigned char*)"STARTED", 10);
	players.clear();
	for(int n = 0; n < LOBBY_GetUsercountInRoom(myRoom); n++) {
		players.push_back(NULL);
	}
	players[roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF))] = new Kirby(512/2 -32, 256/3 -32, roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF)), 1);
	LOBBY_SendToRoom(myRoom, 0x0004, (unsigned char*)"kirby", 10);
	while(!allSelected()) {}
	
	stagename = "finaldestination";
	Stage stage = setStage(stagename);
	PA_InitText(MAIN_SCREEN,1);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	initMinimap(stagename);
	PA_InitText(SUB_SCREEN,1);
	PA_SetTextCol(SUB_SCREEN, 31,31,31);
	
	int time = 1*60*60;
	score = Scoreboard(players.size());
	if(score.playercount == 0) while(true);	

	while(true) {
		if(time-60 == 0) return displayResults();
		players[roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF))] -> act();
		// send character info to room
		// display/update other characters
		// check hits
		
		scrollScreen();
		displaySubScreen();
		displayPercentages();
		PA_OutputText(MAIN_SCREEN, 13,0, "          ");
		if((int)((time/60)%60) < 10) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:0%d",(int)((time/60)/60), (int)((time/60)%60));
		else if((int)((time/60)%60) == 0) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:00",(int)((time/60)/60));
		else PA_OutputText(MAIN_SCREEN, 13, 0, "%d:%d", (int)((time/60)/60), (int)((time/60)%60));		
		PA_WaitForVBL();
		time--;
	}	
	
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SetRoomVisibility(1); // unhides room
}
void receiveStartData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	LANgame();
}
void LAN() {
	int j = 0;

	inlan = true;
	
	IPC_SetChannelCallback(0,&LWIFI_IPC_Callback) ;
	LOBBY_SetStreamHandler(0x0001,&receiveUserData);
	LOBBY_SetStreamHandler(0x0002,&receiveRoomData);
	LOBBY_SetStreamHandler(0x0003,&receiveStartData);
	LOBBY_SetStreamHandler(0x0004,&receiveCharData);
	LOBBY_SetStreamHandler(0x0005,&receiveGameData);
	LOBBY_SetUserInfoCallback(&userCallback);

	PA_VBLFunctionInit(customVBL);

	PA_InitText(MAIN_SCREEN, 0);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	
	int max=1;

	while (1)  {
		// handle input
		switch(menuState) {
			case MENU_MAIN: {
				max = 1+roomCount;
				if(Pad.Newpress.A) {
					if(selected==0) { // go to create room menu
						menuState=MENU_CREATE_ROOM;
						selected=0;
					}
					else { // join existing room
						myRoom = LOBBY_GetRoomByUser(roomOwners[selected-1]);
						LOBBY_JoinRoom(LOBBY_GetRoomByUser(roomOwners[selected-1]));
					}
				}
			}
			break;
	
			case MENU_CREATE_ROOM: {
				max = 5;
				if(Pad.Newpress.Right) {
					switch(selected) {
						case 0:
							if(++roomNameIndex>=ROOM_NAMES) roomNameIndex=0;
						break;

						case 1:
							if(++roomMaxUsers>8) roomMaxUsers=2;
						break;

						case 2:
							if(++roomGameCode>=65535) roomGameCode=1;
						break;

						case 3:
							if(++roomGameVersion>=65535) roomGameVersion=1;
						break;
					}
				}
				if(Pad.Newpress.Left) {
					switch(selected) {
						case 0:
							if(--roomNameIndex<0) roomNameIndex = ROOM_NAMES-1;
						break;

						case 1:
							if(--roomMaxUsers<2) roomMaxUsers=8;
						break;

						case 2:
							if(--roomGameCode<1) roomGameCode=65535;
						break;

						case 3:
							if(--roomGameVersion<1) roomGameVersion=65535;
						break;
					}
				}
				if(Pad.Newpress.B) {
					menuState = MENU_MAIN;
					selected=0;
				}
				if(Pad.Newpress.A) {
					if(selected==4) { // create a new room
						LOBBY_CreateRoom(roomNames[roomNameIndex], roomMaxUsers, roomGameCode, roomGameVersion);
						myRoom = LOBBY_GetRoomByUser(LOBBY_GetUserByID(USERID_MYSELF));
					}
				}
			}
			break;

			case MENU_LOBBY: {
				int inc = (LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)?1:0);
				max = 1 + LOBBY_GetUsercountInRoom(myRoom) + inc;
				if(Pad.Newpress.A) {
					if(inc&&selected==0) { // toggle room visibility
						roomVisible = 1-roomVisible;
						LOBBY_SetRoomVisibility(roomVisible);
					}
					if(selected==inc) { // leave room
						LOBBY_LeaveRoom();
						menuState = MENU_MAIN;
						selected=0;
					}
					else if(selected==inc+1) // send message to all room members
						LOBBY_SendToRoom(myRoom, 0x0002, (unsigned char *)"hi all!", 8);
					else { // send message to a specific room member
						j=selected-inc-2;
						if(LOBBY_GetRoomUserBySlot(myRoom, j)==LOBBY_GetUserByID(USERID_MYSELF)) // skip sending messages to yourself
							j++;
						LOBBY_SendToUser(LOBBY_GetRoomUserBySlot(myRoom, j), 0x0001, (unsigned char *)"hi you!", 8);
					}
				}
				if(Pad.Newpress.Start && LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) {
					LANgame();
				}
			}
			break;
		}
		
		if (Pad.Newpress.Down) {
			if(++selected>=max) selected = 0;
		}
		if (Pad.Newpress.Up) {
			if(--selected<0) selected = max-1;
		}

		// handle output
		for(int i = 0; i < 10; i++) PA_OutputText(MAIN_SCREEN, 0, i, "                                 ");
		j=0;
		switch(menuState) {
			case MENU_MAIN: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== room main ==================");
				PA_OutputText(MAIN_SCREEN, 0, 1, "%s create room", (selected==j?"->":"  ")); j++;
				for(int i=0;i<roomCount;i++) {
					LPLOBBY_ROOM roomInfo = LOBBY_GetRoomByUser(roomOwners[i]);
					if(roomInfo!=NULL) {
						PA_OutputText(MAIN_SCREEN, 0, 2, "%s join room %s: %s [%d/%d]",
							(selected==j?"->":"  "),
							LOBBY_GetUserName(LOBBY_GetRoomUserBySlot(roomInfo, 0)),
							LOBBY_GetRoomName(roomInfo),
							LOBBY_GetUsercountInRoom(roomInfo),
							LOBBY_GetMaxUsercountInRoom(roomInfo)
						);
						j++;
					}
				}
			}
			break;
	
			case MENU_CREATE_ROOM: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== create room ================");
				PA_OutputText(MAIN_SCREEN, 0, 1, "%s room name: %s", (selected==j?"->":"  "), roomNames[roomNameIndex]); j++;
				PA_OutputText(MAIN_SCREEN, 0, 2, "%s max usercount: %d", (selected==j?"->":"  "), roomMaxUsers); j++;
				PA_OutputText(MAIN_SCREEN, 0, 3, "%s gamecode: %d", (selected==j?"->":"  "), roomGameCode); j++;
				PA_OutputText(MAIN_SCREEN, 0, 4, "%s version: %d", (selected==j?"->":"  "), roomGameVersion); j++;
				PA_OutputText(MAIN_SCREEN, 0, 5, "%s create room", (selected==j?"->":"  ")); j++;
			}
			break;

			case MENU_LOBBY: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== %s [%d/%d] v%d / code:%d =",
					LOBBY_GetRoomName(myRoom),
					LOBBY_GetUsercountInRoom(myRoom),
					LOBBY_GetMaxUsercountInRoom(myRoom),
					LOBBY_GetRoomGameVersion(myRoom),
					LOBBY_GetRoomGameCode(myRoom)
				);
				if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) { // we're leading this room
					PA_OutputText(MAIN_SCREEN, 0, 1, "%s is the room visible: %s", (selected==j?"->":"  "), (roomVisible?"yes":"no")); j++;
				}
				PA_OutputText(MAIN_SCREEN, 0, 2, "%s leave room", (selected==j?"->":"  ")); j++;
				PA_OutputText(MAIN_SCREEN, 0, 3, "%s send msg to all room users", (selected==j?"->":"  ")); j++;
				for(int i=0;i<LOBBY_GetUsercountInRoom(myRoom);i++) {
					if(LOBBY_GetRoomUserBySlot(myRoom, i)!=LOBBY_GetUserByID(USERID_MYSELF)) { // don't display yourself in the list
						PA_OutputText(MAIN_SCREEN, 0, 4+i, "%s send msg to user %s", (selected==j?"->":"  "), LOBBY_GetUserName(LOBBY_GetRoomUserBySlot(myRoom, i))); j++;
					}
				}
			}
			break;
		}
		PA_WaitForVBL() ;
	}
}

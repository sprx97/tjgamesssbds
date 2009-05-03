// Includes
#include <PA9.h>       // Include for PA_Lib
#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h>
// DS <-> DS

#define DEBUG_ON

#define MAIN_SCREEN 1
#define SUB_SCREEN 2

#define ROOM_NAME "SSBDS"
#define MAX_PLAYERS 2
#define GAME_CODE 558
#define GAME_VER 0

char dat[10];
int playernum = -1;

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

void customVBL(void) {
	IPC_RcvCompleteCheck();
	LOBBY_Update();
	AS_SoundVBL();
}

void receive(unsigned char *data, int length, LPLOBBY_USER from) {

}

// Function: main()
int main(int argc, char ** argv)
{
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
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()

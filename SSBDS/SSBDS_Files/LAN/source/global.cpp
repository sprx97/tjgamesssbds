#include <PA9.h>
#include "global.h"

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

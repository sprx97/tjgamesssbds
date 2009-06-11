#include <PA9.h> // DS functions that we use come from here
#include "global.h"
#include "effect.h"
#include <vector>
using std::vector;
// shortcuts for visual effects
Effect::Effect(double xpos, double ypos, int t, int pn, double scrollx, double scrolly) {
	x = xpos, y = ypos;
	type = t;
	playernum = pn;
	mynum = 4*playernum + 5;
	if(type == FX_AIRJUMP) mynum+=1;
	else if(type == FX_DEATH) mynum+=2;
	else { // all knockback sprites are the same spritenum
		mynum+=3;
	} // creates the effect... avoids overriding multipe sprites
	// different sprite for different types of effects
	if(x-scrollx < 256 && x-scrollx > 0-64 && y-scrolly < 192 && y-scrolly > 0-64) {
		PA_SetSpriteXY(MAIN_SCREEN, mynum, (int)(x-scrollx), (int)(y-scrolly));
	}
	// sets the position of the sprite on the screen
	if(type == FX_WEAKERHIT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 0, 2, 12, ANIM_ONESHOT);
		delay = 60/12 * 3;
	} // sets animation for a weaker hit
	else if(type == FX_WEAKHIT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 3, 5, 12, ANIM_ONESHOT);
		delay = 60/12 * 3;
	} // sets animation for a weak hit
	else if(type == FX_STRONGHIT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 6, 8, 12, ANIM_ONESHOT);
		delay = 60/12 * 3;
	} // sets animation for a strong hit
	else if(type == FX_AIRJUMP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 9, 10, 15, ANIM_ONESHOT);
		delay = 60/15 * 2;
	} // sets animation for air jump
	else if(type == FX_DEATH) {
		if(x-scrollx == 0-10) {
			PA_SetSpriteHflip(MAIN_SCREEN, mynum, true);
		}
		if(x-scrollx == 256-64+10) {
			PA_SetSpriteHflip(MAIN_SCREEN, mynum, false);
		}
		if(y-scrolly == 0-10) {
			PA_SetRotsetNoZoom(MAIN_SCREEN, mynum, 128);
			PA_SetSpriteRotEnable(MAIN_SCREEN, mynum, mynum);
		}
		if(y-scrolly == 192-64+10) {
			PA_SetRotsetNoZoom(MAIN_SCREEN, mynum, -128);
			PA_SetSpriteRotEnable(MAIN_SCREEN, mynum, mynum);
		}
		PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 11, 17, 10, ANIM_ONESHOT);
		delay = 60/10 * 7;
	}
	// sets animation for a death
	// else if...
	// else if...
} // creates a new effect
bool Effect::act() {
	delay--;
	if(delay <= 0) {
		PA_SetSpriteRotDisable(MAIN_SCREEN, mynum);
		PA_SetSpriteXY(MAIN_SCREEN, mynum, -64, -64); // hides sprite
		return true;
	} // removes once animation is done
	return false; //don't kill me yet
} // acts; called every frame

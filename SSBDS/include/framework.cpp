#include "global.h"
// shortcuts for visual effects
class Effect {
	public:
		double x, y; // x and y pos of the effect
		int mynum; // the reference number of the effect
		int delay; // how long the effect waits before changing
		int playernum; // which player the effect is created by
		int type; // what effect type it is
		Effect(double xpos, double ypos, int t, int pn) {
			x = xpos, y = ypos;
			type = t;
			playernum = pn;
			mynum = 5*playernum + 5;
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
		void act() {
			delay--;
			if(delay <= 0) {
				vector<Effect> temp;
				for(int n = 0; n < (int)effects.size(); n++) {
					Effect e = effects[n];
					if(e.x != x || e.y != y || e.delay != delay || e.playernum != playernum) {
						temp.push_back(e);
					}
				} // keeps the effect
				PA_SetSpriteRotDisable(MAIN_SCREEN, mynum);
				PA_SetSpriteXY(MAIN_SCREEN, mynum, -64, -64); // hides sprite
				effects = temp;
			} // removes once animation is done
		} // acts; called every frame
}; // a visual effect

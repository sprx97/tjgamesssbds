#include "fox.h"
#include "fighter.h"
#include "display.h"
#include "projectiles.h"
#include <PA9.h>
#include <vector>
using std::vector;

//constructor
Fox::Fox(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num,listplayers,disp,"fox",AI) {
	weight = .910;
	w2value  = 3.41;
	jumpspeed = 8.5;
	doublejumpspeed = 5;
	shieldstr = 64;
	runspeed = 5;
	handx = 42;
	handy = 11;
	MYCHAR = FOX;
	series = "starfox";
	topside = 10;
	bottomside = 45;
	rightside = 45;
	leftside = 25;
	gravity = 2.5;
	jumpmax = 2;
	upcount = downcount = leftcount = rightcount = 0;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Fox::initSounds() {
	int alreadymade = 0;
	for(int n = 0; n < charnum; n++) {
		if(players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if(alreadymade == 0) {
		PA_FatLoadSfx("foxfsmash", "foxfsmash");
		PA_FatLoadSfx("foxbup", "foxbup");
		PA_FatLoadSfx("foxdair", "foxdair");
		PA_FatLoadSfx("foxjab", "foxjab");
		PA_FatLoadSfx("foxdashatk", "foxdashatk");
		PA_FatLoadSfx("foxbdown", "foxbdown");
		PA_FatLoadSfx("foxbneut", "foxbneut");
		PA_FatLoadSfx("foxbside", "foxbside");
		PA_FatLoadSfx("foxutilt", "foxutilt");
	}
}
void Fox::initPalettes() {
	palettes.push_back("fox");
	palettes.push_back("fox");
	palettes.push_back("fox");
	palettes.push_back("fox");
}
// sounds
void Fox::playsound(int sndnum) {
	if(sndnum == SMASHRIGHT || sndnum == SMASHLEFT) PA_FatPlaySfx("foxfsmash");
	if(sndnum == DAIR) PA_FatPlaySfx("foxdair");
	if(sndnum == JAB) PA_FatPlaySfx("foxjab");
	if(sndnum == DASHATTACK) PA_FatPlaySfx("foxdashatk");
	if(sndnum == UTILT) PA_FatPlaySfx("foxutilt");
}
// actions
void Fox::bside() {
	if(action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 118, 120, 20, ANIM_LOOP, -1);
		delay = 60/20 * 3;
		y -= 1;
		aerial = true;
		dy = -gravity;
		dy = fastfall = DI = 0;
		setDirection();
		action = BSIDE;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 120) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 121, 121, 20, ANIM_LOOP, -1);
		delay = 60/20 * 5;
		PA_FatPlaySfx("foxbside");
		if(direction == RIGHT) dx = 8;
		else dx = -8;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 121) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 123, 20, ANIM_LOOP, -1);
		delay = 60/20 * 2;
	}
}
void Fox::bup() {
	if(action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 124, 126, 15, ANIM_LOOP, -1);
		delay = 60/15 * 3 * 5;
		y -= 1;
		aerial = true;
		dy = -gravity;
		dx = DI = fastfall = 0;
		upcount = downcount = leftcount = rightcount = 0;
		action = BUP;
		setDirection();
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 126 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 127, 129, 10, ANIM_LOOP, -1);
		delay = 60/10 * 3;
		aerial = true;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 127 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 128 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 129) {
		x += (rightcount-leftcount)*10;
		y += (downcount-upcount)*10;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 124 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM == 125) || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 126) {
		if(!isCPU) {
			if(Pad.Held.Up) upcount += 1;
			if(Pad.Held.Down) downcount += 1;
			if(Pad.Held.Right) rightcount += 1;
			if(Pad.Held.Left) leftcount += 1;
		}
		else {
			Floor mainfloor = stage -> getFloors()[0];
			if(y > mainfloor.y) upcount += 1;
			if(x > mainfloor.x + mainfloor.length) leftcount += 1;
			if(x < mainfloor.x) rightcount += 1;
		}
	}
	if(upcount > 10) upcount = 10;
	if(downcount > 10) downcount = 10;
	if(rightcount > 10) rightcount = 10;
	if(leftcount > 10) leftcount = 10;
}
void Fox::bdown() {

}
void Fox::bneut() {

}
void Fox::fthrow() {

}
void Fox::bthrow() {

}
void Fox::uthrow() {


}
void Fox::dthrow() {

}
void Fox::jaywalk() {

}
Fox::~Fox() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

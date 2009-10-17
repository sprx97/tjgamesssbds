#include "fox.h"
#include "fighter.h"
#include "display.h"
#include "projectiles.h"
#include <PA9.h>
#include <math.h>
#include <vector>
using std::vector;

//constructor
Fox::Fox(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num, listplayers, disp, "fox", AI) {
	w1 = .910;
	w2  = 3.41;
	jumpheight = 120;
	doublejumpheight = 100;
	shieldstr = 64;
	runspeed = 5;
	handx = 64 - 45;
	handy = 30;
	hangx = 64 - 39;
	hangy = 12;
	MYCHAR = FOX;
	series = "starfox";
	topside = 10;
	bottomside = 45;
	rightside = 45;
	leftside = 25;
	gravity = GLOBALGRAVITY + w2/5;
	jumpmax = 2;
	upcount = downcount = leftcount = rightcount = 0;
	laseragain = false;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Fox::initSounds() {
	int alreadymade = 0;
	for (int n = 0; n < charnum; n++) {
		if (players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if (alreadymade == 0) {
		PA_FatLoadSfx("foxfsmash", "characters/fox/fsmash");
		PA_FatLoadSfx("foxbup", "characters/fox/bup");
		PA_FatLoadSfx("foxdair", "characters/fox/dair");
		PA_FatLoadSfx("foxjab", "characters/fox/jab");
		PA_FatLoadSfx("foxdashatk", "characters/fox/dashatk");
		PA_FatLoadSfx("foxbdown", "characters/fox/bdown");
		PA_FatLoadSfx("foxbneut", "characters/fox/bneut");
		PA_FatLoadSfx("foxbside", "characters/fox/bside");
		PA_FatLoadSfx("foxutilt", "characters/fox/utilt");
	}
}
void Fox::initPalettes() {
	palettes.push_back("characters/fox");
	palettes.push_back("characters/foxwhite");
	palettes.push_back("characters/foxblue");
	palettes.push_back("characters/foxgreen");
}
// sounds
void Fox::playsound(int sndnum) {
	if (sndnum == SMASHRIGHT || sndnum == SMASHLEFT) PA_FatPlaySfx("foxfsmash");
	if (sndnum == DAIR) PA_FatPlaySfx("foxdair");
	if (sndnum == JAB) PA_FatPlaySfx("foxjab");
	if (sndnum == DASHATTACK) PA_FatPlaySfx("foxdashatk");
	if (sndnum == UTILT) PA_FatPlaySfx("foxutilt");
}
// actions
void Fox::bside() {
	if (action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 118, 120, 10, ANIM_LOOP, -1);
		delay = 60 / 10 * 3;
		if (aerial) dy = -gravity;
		else dy = 0;
		dx = 0;
		ymomentum = fastfall = DI = 0;
		setDirection();
		action = BSIDE;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 120) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 121, 121, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 2;
		PA_FatPlaySfx("foxbside");
		if (direction == RIGHT) dx = 25;
		else dx = -25;
		if (aerial) dy = -gravity;
		else dy = 0;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 121) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 123, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 2;
		dx = 0;
		if (aerial) dy = -gravity;
		else dy = 0;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 123) {
		permafall();
	}
	if(action == BSIDE && aerial) dy = -gravity;
}
void Fox::bup() {
	if (action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 124, 126, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 3 * 5;
		if (aerial) dy = -gravity;
		else dy = 0;
		ymomentum = dx = DI = fastfall = 0;
		upcount = downcount = leftcount = rightcount = 0;
		action = BUP;
		setDirection();
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 126 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 127, 129, 10, ANIM_LOOP, -1);
		delay = 60 / 10 * 3;
		int ydiff = (downcount - upcount);
		int xdiff = (rightcount - leftcount);
		if (xdiff > 0) setDirection(RIGHT);
		else setDirection(LEFT);
		if (ydiff > 0) PA_SetSpriteVflip(MAIN_SCREEN, SPRITENUM, 1);
		else PA_SetSpriteVflip(MAIN_SCREEN, SPRITENUM, 0);
		PA_FatPlaySfx("foxbup");
		aerial = true;
		dy = -gravity;
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 129 && delay == 1) {
		upcount = downcount = rightcount = leftcount = 0;
		PA_SetSpriteVflip(MAIN_SCREEN, SPRITENUM, 0);
		permafall();
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 127 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 128 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 129) {
		dx = (rightcount - leftcount) * 10 / 15;
		dy = (downcount - upcount) * 10 / 15;
	}
	else if (delay < 40 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 124 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM == 125) || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 126)) {
		if (!isCPU) {
			if (Pad.Held.Up) upcount += 1;
			if (Pad.Held.Down) downcount += 1;
			if (Pad.Held.Right) rightcount += 1;
			if (Pad.Held.Left) leftcount += 1;
		}
		else {
			Floor mainfloor = stage -> getFloors()[0];
			if (y > mainfloor.y) upcount += 1;
			if (x > mainfloor.x + mainfloor.length) leftcount += 1;
			if (x < mainfloor.x) rightcount += 1;
		}
	}
	if(checkFloorCollision() || checkCeilingCollision()) upcount = downcount = 0;
	if(checkWallCollision()) leftcount = rightcount = 0;
	if (upcount > 10) upcount = 10;
	if (downcount > 10) downcount = 10;
	if (rightcount > 10) rightcount = 10;
	if (leftcount > 10) leftcount = 10;
}
void Fox::bdown() {
	if (action != BDOWN) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 114, 114, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 1;
		dx = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		ymomentum = fastfall = DI = 0;
		PA_FatPlaySfx("foxbdown");
		setDirection();
		action = BDOWN;
	}
	else if (delay == 1 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 114 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 117)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 115, 117, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 3;
		CAPE = true;
		dx = 0;
		ymomentum = fastfall = DI = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		setDirection();
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 114 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 115 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 116 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 117) {
		dx = 0;
		ymomentum = fastfall = DI = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		setDirection();
	}
	if (custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
		CAPE = false;
		if (checkFloorCollision()) idle();
		else fall();
	}
}
void Fox::bneut() {
	if (action != BNEUT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 110, 110, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 1;
		action = BNEUT;
		laseragain = false;
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 110 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 111, 113, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 3;
		PA_FatPlaySfx("foxbneut");
		int directionmodifier = 1;
		if (direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(createAtkbox(17, 32, 1, Knockback(0, 0, 0), 1));
		tempbox.addCircle(createAtkbox(46, 32, 1, Knockback(0, 0, 0), 1));
		Projectile p = Projectile(x, y, -10 * directionmodifier, 0, 200, FOXLASER, charnum, tempbox, stage, display);
		((vector<Projectile>*)getProj())->push_back(p);
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 113 && delay == 1) {
		if (checkFloorCollision()) idle();
		else fall();
		if(laseragain) bneut();
	}
	else if (aerial && checkFloorCollision()) dy = 0;
	if(Pad.Newpress.B) laseragain = true;
}
void Fox::fthrow() {
	if (action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 138, 12, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60 / 12 * 4;
		action = FTHROW;
		if (direction == RIGHT) {
			dx = -2;
			grabbedenemy -> dx = -2;
		}
		else {
			dx = 2;
			grabbedenemy -> dx = 2;
		}
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1.5, -1.5, 7);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 6;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Fox::bthrow() {
	if (action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 139, 142, 15, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60 / 15 * 4;
		action = BTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -3;
		else grabbedenemy -> dx = 3;
		grabbedenemy -> dy = -1;
	}
	if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 145, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 3;
		int mult = 1;
		grabbedenemy -> k = Knockback(1.5, -2.5, 7);
		if (direction == RIGHT) mult = -1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 6;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	else if (delay <= 0) {
		grabbedenemy -> percentage += 2;
		if (direction == RIGHT) setDirection(LEFT);
		else setDirection(RIGHT);
		idle();
	}
}
void Fox::uthrow() {
	if (action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 146, 149, 12, ANIM_LOOP, -1);
		delay = 60 / 12 * 4;
		playsound(UTHROW);
		action = UTHROW;
		int mult = -1;
		grabbedenemy -> k = Knockback(.5, -2.5, 6);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 7;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if (delay <= 0) 	idle();
}
void Fox::dthrow() {
	if (action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 100, 103, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 4;
		playsound(DTHROW);
		action = DTHROW;
		grabbedenemy -> dy = .25;
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(.5, -2, 7);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 5;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Fox::jaywalk() {
	if ((PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 47 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 48) && direction == RIGHT) x += 4;
	if ((PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 47 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 48) && direction == LEFT) x -= 4;
}
Fox::~Fox() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

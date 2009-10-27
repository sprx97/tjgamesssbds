#include "sandbag.h"
#include "fighter.h"
#include "display.h"
#include "projectiles.h"
#include <PA9.h>
#include <vector>
using std::vector;

// constructor
Sandbag::Sandbag(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num, listplayers, disp, "sandbag", AI) {
	w1 = 1.0;
	w2 = 0.0;
	jumpheight = 0;
	doublejumpheight = 0;
	shieldstr = 0;
	runspeed = 0;
	handx = 0;
	handy = 0;
	shieldx = shieldy = 0;
	hangx = hangy = 0;
	MYCHAR = SANDBAG;
	series = "smashbros";
	topside = 6;
	bottomside = 47;
	rightside = 44;
	leftside = 19;
	gravity = GLOBALGRAVITY + w2/10;
	jumpmax = 0;
	initPalettes();
	initFrames();
	initSprite();
	fall();
}
void Sandbag::initSounds() {}
void Sandbag::playsound(int sndnum) {}
void Sandbag::initPalettes() {
	palettes.push_back("characters/sandbag");
	palettes.push_back("characters/sandbag");
	palettes.push_back("characters/sandbag");
	palettes.push_back("characters/sandbag");
}
void Sandbag::act() {
	if(freezelen > 0) {
		freezelen--;
		invincibility--;
		if(freezelen == 0) {
			if(action == STUN) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[STUN], endframes[STUN], framespeeds[STUN], ANIM_LOOP, -1);
			else spriteanims[MAIN_SCREEN][SPRITENUM] = tempanim;
		}
		else PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM), PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM), 1, ANIM_LOOP, -1);
		return;
	}
	if (respawntimer > 0) {
		return respawn();
	}
	if(invincibility > 0) invincibility--;
	if (effectwait > 0) effectwait--;
	if (ledgewait > 0) ledgewait--;
	if (hitstun > k.length*2) {
		if (y != stage->getFloors()[0].y) aerial = true;
		hitstun--;
		dx = kx;
		dy = ky;
		if (hitstun == 0) {
			if (aerial) fall();
			else idle();
		}
		if (checkFloorCollision() || checkCeilingCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
		}
		if(checkWallCollision()) {
			kx *= -1;
			dx = kx;
		}
	}
	else if (hitstun > 0) {
		if (y != stage->getFloors()[0].y) aerial = true;
		hitstun--;
		if (kx > 0) {
			kx -= kx / (hitstun / 3);
			if (kx < 0) kx = 0;
		}
		else if (kx < 0) {
			kx -= kx / (hitstun / 3);
			if (kx > 0) kx = 0;
		}
		if (ky > 0) {
			ky -= ky / (hitstun / 3);
			if (ky < 0) ky = 0;
		}
		else if (ky < 0) {
			ky -= ky / (hitstun / 3);
			if (ky > 0) ky = 0;
		}
		if (hitstun == 0) {
			action = STUN;
			if (aerial) fall();
			else idle();
		}
		dx = kx;
		dy = ky;
		if (checkFloorCollision() || checkCeilingCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
		}
		if(checkWallCollision()) {
			kx *= -1;
			dx = kx;
		}
	}
	else if (action == FALL && checkFloorCollision()) idle();
	else if(action == FALL) {
		if (dx > 0) {
			dx -= traction;
			if (dx <= 0) dx = 0;
		}
		else if (dx < 0) {
			dx += traction;
			if (dx >= 0) dx = 0;
		}	
	}
	else if (action == IDLE) idle();
	move();
}
Sandbag::~Sandbag() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

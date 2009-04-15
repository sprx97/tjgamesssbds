#include "sandbag.h"
#include "fighter.h"
#include "display.h"
#include "projectiles.h"
#include <PA9.h>
#include <vector>
#include "gfx/all_sounds.c"
using std::vector;

// constructor
Sandbag::Sandbag(int xpos, int ypos, int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(xpos,ypos,num,listplayers,disp,"sandbag",AI) {
	weight = 1.0;
	w2value = 0.0;
	shieldstr = 0;
	runspeed = 0;
	handx = 0;
	handy = 0;
	MYCHAR = SANDBAG;
	topside = 6;
	bottomside = 47;
	rightside = 44;
	leftside = 19;
	gravity = 2.5;
	jumpmax = 0;
	initFrames();
	initSprite();
}
void Sandbag::initFrames() {
	//LAND
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SHIELD
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// ROLL
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DODGE
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// AIRDODGE
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// CROUCH
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// FALL
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// IDLE
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// RUN
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SHORTHOP
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// JUMP
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DOUBLEJUMP
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// JAB
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DASHATTACK
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// FTILT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// UTILT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DTILT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// CHARGELEFT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// CHARGERIGHT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// CHARGEUP
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// CHARGEDOWN
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SMASHLEFT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SMASHRIGHT
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SMASHUP
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// SMASHDOWN
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// FAIR
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// BAIR
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// UAIR
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DAIR
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// NAIR
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// STUN
	startframes.push_back(1);
	endframes.push_back(4);
	framespeeds.push_back(20);

	// SLIDE
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// HANG
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// GRABBED
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// GRAB
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// GRABATK
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// DEAD
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);		
}
void Sandbag::act() {
	if(effectwait > 0) effectwait--;
	if(ledgewait > 0) ledgewait--;
	if(hitstun > k.length*2) {
		if(y != stage->getFloors()[0].y ) aerial = true;
		hitstun--;
		dx = kx;
		dy = ky;
		if(hitstun == 0) {
			if(aerial) fall();
			else idle();
		}
		if(checkFloorCollision()) idle();
	}
	else if(hitstun > 0) {
		if(y != stage->getFloors()[0].y ) aerial = true;
		hitstun--;
		if(kx > 0) {
			kx -= kx/(hitstun/3);
			if(kx < 0) kx = 0;
		}
		else if(kx < 0) {
			kx -= kx/(hitstun/3);
			if(kx > 0) kx = 0;
		}
		if(ky > 0) {
			ky -= ky/(hitstun/3);
			if(ky < 0) ky = 0;
		}
		else if(ky < 0) {
			ky -= ky/(hitstun/3);
			if(ky > 0) ky = 0;
		}
		if(hitstun == 0) {
			action = STUN;
			if(aerial) fall();
			else idle();
		}
		dx = kx;
		dy = ky;
		if(checkFloorCollision()) idle();
	}
	else if(action == FALL) if(checkFloorCollision()) idle();
	else if(action == IDLE) idle();
	move();
}
Sandbag::~Sandbag() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

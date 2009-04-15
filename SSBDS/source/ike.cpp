#include "ike.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
#include "gfx/all_sounds.c"
using std::vector;

// constructor
Ike::Ike(int xpos, int ypos, int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(xpos,ypos,num,listplayers,disp,"ike",AI) {
	weight = 1.035;
	w2value = 0.28;
	shieldstr = 64;
	runspeed = 2.75;
	handx = 14;
	handy = 8;
	eruptioncharge = 0;
	quickdrawcharge = 0;
	CAPE = false;
	COUNTER = false;
	MYCHAR = IKE;
	topside = 15;
	bottomside = 47;
	rightside = 37;
	leftside = 13;
	gravity = 3;
	jumpmax = 2;
	initFrames();
	initSprite();
} // initializes all of the variables
// initializers
void Ike::playsound(int sndnum) {
	if(sndnum == BAIR) AS_SoundQuickPlay(ikebair);
	if(sndnum == DASHATTACK) AS_SoundQuickPlay(ikedashattack);
	if(sndnum == FAIR) AS_SoundQuickPlay(ikefair);
	if(sndnum == SMASHLEFT || sndnum == SMASHRIGHT) AS_SoundQuickPlay(ikefsmash);
	if(sndnum == FTILT) AS_SoundQuickPlay(ikeftilt);
	if(sndnum == JUMP) AS_SoundQuickPlay(ikejump);
	if(sndnum == UTILT) AS_SoundQuickPlay(ikeutilt);
}
void Ike::initFrames() {
	// LAND
	startframes.push_back(80);
	endframes.push_back(80);
	framespeeds.push_back(10);

	// SHIELD
	startframes.push_back(142);
	endframes.push_back(142);
	framespeeds.push_back(20);

	// ROLL
	startframes.push_back(130);
	endframes.push_back(134);
	framespeeds.push_back(12);

	// DODGE
	startframes.push_back(115);
	endframes.push_back(118);
	framespeeds.push_back(6);
	// should be updown

	// AIRDODGE
	startframes.push_back(13);
	endframes.push_back(13);
	framespeeds.push_back(3);

	// CROUCH
	startframes.push_back(7);
	endframes.push_back(7);
	framespeeds.push_back(20);

	// FALL
	startframes.push_back(12);
	endframes.push_back(12);
	framespeeds.push_back(20);

	// IDLE
	startframes.push_back(0);
	endframes.push_back(6);
	framespeeds.push_back(10);

	// RUN
	startframes.push_back(14);
	endframes.push_back(17);
	framespeeds.push_back(10);

	// SHORTHOP
	startframes.push_back(8);
	endframes.push_back(11);
	framespeeds.push_back(10);

	// JUMP
	startframes.push_back(8);
	endframes.push_back(11);
	framespeeds.push_back(10);

	// DOUBLEJUMP
	startframes.push_back(130);
	endframes.push_back(134);
	framespeeds.push_back(15);

	// JAB
	startframes.push_back(18);
	endframes.push_back(21);
	framespeeds.push_back(12);

	// DASHATTACK
	startframes.push_back(32);
	endframes.push_back(35);
	framespeeds.push_back(15);

	// FTILT
	startframes.push_back(143);
	endframes.push_back(146);
	framespeeds.push_back(10);

	// UTILT
	startframes.push_back(36);
	endframes.push_back(40);
	framespeeds.push_back(10);

	// DTILT
	startframes.push_back(41);
	endframes.push_back(46);
	framespeeds.push_back(10);

	// CHARGELEFT
	startframes.push_back(22);
	endframes.push_back(23);
	framespeeds.push_back(20);

	// CHARGERIGHT
	startframes.push_back(22);
	endframes.push_back(23);
	framespeeds.push_back(20);

	// CHARGEUP
	startframes.push_back(90);
	endframes.push_back(91);
	framespeeds.push_back(20);

	// CHARGEDOWN
	startframes.push_back(80);
	endframes.push_back(81);
	framespeeds.push_back(20);

	// SMASHLEFT
	startframes.push_back(24);
	endframes.push_back(30);
	framespeeds.push_back(15);

	// SMASHRIGHT
	startframes.push_back(24);
	endframes.push_back(30);
	framespeeds.push_back(15);

	// SMASHUP
	startframes.push_back(92);
	endframes.push_back(97);
	framespeeds.push_back(15);

	// SMASHDOWN
	startframes.push_back(82);
	endframes.push_back(89);
	framespeeds.push_back(15);

	// FAIR
	startframes.push_back(56);
	endframes.push_back(60);
	framespeeds.push_back(10);

	// BAIR
	startframes.push_back(74);
	endframes.push_back(78);
	framespeeds.push_back(12);

	// UAIR
	startframes.push_back(61);
	endframes.push_back(67);
	framespeeds.push_back(15);

	// DAIR
	startframes.push_back(67);
	endframes.push_back(74);
	framespeeds.push_back(15);

	// NAIR
	startframes.push_back(47);
	endframes.push_back(56);
	framespeeds.push_back(15);

	// STUN
	startframes.push_back(55);
	endframes.push_back(55);
	framespeeds.push_back(20);

	// SLIDE
	startframes.push_back(7);
	endframes.push_back(7);
	framespeeds.push_back(20);

	// HANG
	startframes.push_back(123);
	endframes.push_back(123);
	framespeeds.push_back(20);

	// GRABBED
	startframes.push_back(0);
	endframes.push_back(0);
	framespeeds.push_back(20);

	// GRAB
	startframes.push_back(147);
	endframes.push_back(148);
	framespeeds.push_back(10);

	// GRABATK
	startframes.push_back(149);
	endframes.push_back(151);
	framespeeds.push_back(15);

	// DEAD
	startframes.push_back(169);
	endframes.push_back(169);
	framespeeds.push_back(20);
}
// actions
void Ike::bside() {
	if(action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 108, 108, 12, ANIM_LOOP, -1);
		delay = 60/12 * 1;
		setDirection();
		dx = 0;
		if(aerial) dy = -gravity/2;
		else dy = 0;
		action = BSIDE;
		quickdrawcharge = 0;
	}
	else if(custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 110, 110, 12, ANIM_LOOP, -1);
		AS_SoundQuickPlay(ikebside);
		delay = quickdrawcharge/2;
		if(direction == RIGHT) dx = 2*(int)(quickdrawcharge / 20) + 4;
		else dx = -1*2*(int)(quickdrawcharge / 20) - 4;
	}
	else if((delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 108) || (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 109, 109, 12, ANIM_LOOP, -1);
		delay = 60/12 * 1;
		quickdrawcharge++;
		if(quickdrawcharge > 120) quickdrawcharge = 120;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 110) {
		if(checkFloorCollision()) idle();
		else fall();
	}
	else if(aerial && checkFloorCollision()) idle();
	else {
		quickdrawcharge++;
		if(quickdrawcharge > 120) quickdrawcharge = 120;

	}
}
void Ike::bup() {
	if(action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 118, 122, 15, ANIM_LOOP, -1);
		delay = 60/15 * 5;
		setDirection();
		dx = 0;
		if(aerial) dy = -gravity;
		else dy = 0;
		action = BUP;
		int directionmodifier = 1;
		if(direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(Circle(32, 32, 15, Knockback(0, 0, 2), 9));
		Projectile p = Projectile(x, y, -.1*directionmodifier, -4, 30, IKESWORD, charnum, tempbox, stage, display);
		p.miny = y - 120;
		p.maxy = y;
		((vector<Projectile>*)getProj())->push_back(p);
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 127, 127, 15, ANIM_LOOP, -1);
		AS_SoundQuickPlay(ikebup);
		delay = 60/15 * 1;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 127) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 128, 128, 15, ANIM_LOOP, -1);
		delay = 60/15 * 3;
		dy = -gravity-6;
		aerial = true;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 128) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 129, 134, 15, ANIM_LOOP, -1);
		delay = 60/15 * 6;
		dy = -gravity;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 135 && checkFloorCollision()) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 136, 138, 15, ANIM_LOOP, -1);
		delay = 60/15 * 3;
		dy = 0;
		aerial = false;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 138) {
		idle();
	}
	else if((delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134) || (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 135)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 135, 15, ANIM_LOOP, -1);
		delay = 60/15 * 1;
		dy = -gravity + 6;
	}
}
void Ike::bdown() {
	if(action != BDOWN) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 139, 142, 10, ANIM_UPDOWN, 5);
		AS_SoundQuickPlay(ikebdown);
		delay = 60/10 * 5;
		setDirection();
		dx = 0;
		if(aerial) dy = -gravity/2;
		else dy = 0;
		action = BDOWN;
		COUNTER = true;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
		COUNTER = false;
	}
	else if(aerial && checkFloorCollision()) dy = 0;
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
		COUNTER = false;
		if(checkFloorCollision()) idle();
		else fall();
	}
}
void Ike::bneut() {
	if(action != BNEUT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 99, 15, ANIM_LOOP, -1);
		delay = 60/15 * 1;
		setDirection();
		dx = 0;
		if(aerial) dy = -gravity/2;
		else dy = 0;
		action = BNEUT;
		eruptioncharge = 0;				
	}
	else if((custom_action(ACTION_SPECIAL, PAD_RELEASED) && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) || (eruptioncharge == 300 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 100, 109, 10, ANIM_LOOP, -1);
		AS_SoundQuickPlay(ikebneut);
		delay = 60/10 * 10;
	}
	else if(checkFloorCollision() && aerial) dy = 0;
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109) {
		if(checkFloorCollision()) idle();
		else fall();
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 99, 15, ANIM_LOOP, -1);
		delay = 60/15 * 1;
		eruptioncharge++;
		if(eruptioncharge > 300) eruptioncharge = 300;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) {
		eruptioncharge++;
		if(eruptioncharge > 300) eruptioncharge = 300;
	}		}
void Ike::fthrow() {
	if(action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 152, 155, 15, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60/15 * (155-152+1);
		action = FTHROW;
		if(direction == RIGHT) grabbedenemy -> dx = -2;
		else grabbedenemy -> dx = 2;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 154 or PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 155) {
		if(direction == RIGHT) grabbedenemy -> dx = 4;
		else grabbedenemy -> dx = -4;
	}
	if(delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(2, -2, 7);
		if(direction == RIGHT) {
			mult = 1;
		}
		grabbedenemy -> hitstun = (int) (grabbedenemy -> k.length * 3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;			
		idle();	
	}
}
void Ike::bthrow() {
	if(action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 156, 159, 10, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60/10 * (159-156+1);
		action = BTHROW;
		if(direction == RIGHT) grabbedenemy -> dx = -3;
		else grabbedenemy -> dx = 3;
		grabbedenemy -> dy = -1;
	}
	if(delay <= 0) {
		int mult = 1;
		grabbedenemy -> k = Knockback(2, -2.5, 7);
		if(direction == RIGHT) {
			mult = -1;
		}
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;	
		if(direction == RIGHT) setDirection(LEFT);
		else setDirection(RIGHT);
		idle();		
	}
}
void Ike::uthrow() {
	if(action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 160, 163, 12, ANIM_LOOP, -1);
		playsound(UTHROW);
		delay = 60/12 * (163-160+1);
		action = UTHROW;
		if(direction == RIGHT) grabbedenemy -> dx = -.5;
		else grabbedenemy -> dx = .5;
		grabbedenemy -> dy = -1;
	}
	if(delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(.5, -3, 6);
		if(direction == RIGHT) {
			mult = 1;
		}
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;	
		idle();						
	}
}
void Ike::dthrow() {
	if(action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 165, 168, 10, ANIM_LOOP, -1);
		playsound(DTHROW);
		delay = 60/10 * (168-165+1);
		action = DTHROW;
		if(direction == RIGHT) grabbedenemy -> dx = -1;
		else grabbedenemy -> dx = 1;
		grabbedenemy -> dy = .5;
	}
	if(delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1, -2.5, 7);
		if(direction == RIGHT) {
			mult = 1;
		}
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Ike::jaywalk() {}
Ike::~Ike() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);		
}

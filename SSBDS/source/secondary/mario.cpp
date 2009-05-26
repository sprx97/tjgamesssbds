#include "mario.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
using std:: vector;

// constructor
Mario::Mario(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num,listplayers,disp,"mario",AI) {
	weight = 1.0;
	w2value = 0.0;
	jumpspeed = 8;
	doublejumpspeed = 4;
	shieldstr = 64;
	runspeed = 3.75;
	handx = 25;
	handy = 19;
	fluddcharge = 0;
	MYCHAR = MARIO;
	topside = 17;
	bottomside = 47;
	rightside = 39;
	leftside = 24;
	gravity = 3;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Mario::initSounds() {
	int alreadymade = 0;
	for(int n = 0; n < charnum; n++) {
		if(players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if(alreadymade == 0) {
		PA_FatLoadSfx("mariojump", "mariojump");
		PA_FatLoadSfx("mariodoublejump", "mariodoublejump");
		PA_FatLoadSfx("marioutilt", "marioutilt");
		PA_FatLoadSfx("mariodashattack", "mariodashattack");
		PA_FatLoadSfx("mariodsmash", "mariodsmash");
		PA_FatLoadSfx("mariousmash", "mariousmash");
		PA_FatLoadSfx("mariobup", "mariobup");
		PA_FatLoadSfx("mariobneut", "mariobneut");
	}
}
void Mario::initPalettes() {
	palettes.push_back("mariored");
	palettes.push_back("marioredwhite");
	palettes.push_back("marioyellow");
	palettes.push_back("marioblackwhite");
}
void Mario::playsound(int sndnum) {
	if(sndnum == JUMP) PA_FatPlaySfx("mariojump");
	if(sndnum == DOUBLEJUMP) PA_FatPlaySfx("mariodoublejump");
	if(sndnum == UTILT) PA_FatPlaySfx("marioutilt");
	if(sndnum == DASHATTACK) PA_FatPlaySfx("mariodashattack");
	if(sndnum == SMASHDOWN) PA_FatPlaySfx("mariodsmash");
	if(sndnum == SMASHUP) PA_FatPlaySfx("mariousmash");
}
// actions
void Mario::bside() {
	if(action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 148, 10, ANIM_ONESHOT);
		delay = 60/10 * 6;
		setDirection();
		dx = 0;
		action = BSIDE;
		CAPE = true;
	}
	else {
		if(delay == 1) {
			if(checkFloorCollision()) idle();
			else fall();
			CAPE = false;
		}
	}
}
void Mario::bup() {
	if(action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 137, 142, 20, ANIM_ONESHOT);
		aerial = true;
		delay = 60/20 * 6;
		dy = -7;
		dx = 0;
		fastfall = 0;
		DI = 0;
		setDirection();
		action = BUP;
		PA_FatPlaySfx("mariobup");
	}
	else {
		if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
			if(!checkFloorCollision()) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 142, 142, 20, ANIM_LOOP, -1);
				delay = 60/20 * 1;
				dy = 0;
			}
			else {
				idle();
			}
		}
	}
}
void Mario::bdown() {
	if(action != BDOWN) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 149, 149, 20, ANIM_LOOP, -1);
		delay = 60/20 * 1;
		fluddcharge = 0;
		dx = 0;
		if(aerial) dy = -gravity/2;
		else dy = 0;
		action = BDOWN;
	}
	else if(custom_action(ACTION_SPECIAL, PAD_RELEASED) && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 149) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 150, 150, 12, ANIM_LOOP, -1);
		delay = 60/12 * 1;
		int directionmodifier = 1;
		if(direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(createAtkbox(58, 32, 5, Knockback((-1*directionmodifier), -.5, 6), 60));
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y-16, -5*directionmodifier, 0, 2*fluddcharge, FLUDDWATER, charnum, tempbox, stage, display));
		fluddcharge = 0;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 150) {
		if(delay == 1) {
			if(checkFloorCollision()) idle();
			else fall();
		}
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 149) {
		delay = 60/20 * 1;
		fluddcharge++;
	}
	else if(aerial && checkFloorCollision()) {
		dy = 0;
	}
	else {
		fluddcharge++;
		if(fluddcharge > 30) fluddcharge = 30;
	}
}
void Mario::bneut() {
	if(action != BNEUT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 133, 12, ANIM_LOOP, -1);
		delay = 60/12 * 2;
		dx = 0;
		action = BNEUT;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 133 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 134, 134, 10, ANIM_LOOP, -1);
		delay = 60/10 * 1;
		PA_FatPlaySfx("mariobneut");
		int directionmodifier = 1;
		if(direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(createAtkbox(32, 32, 6, Knockback(-.25*directionmodifier, 0, 6), 10));
		Projectile p = Projectile(x, y, -3*directionmodifier, 1.5, 90, FIREBALL, charnum, tempbox, stage, display);
		p.miny = stage->getFloors()[0].y-32;
		p.maxy = y;
		((vector<Projectile>*)getProj())->push_back(p);
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 136, 12, ANIM_LOOP, -1);
		delay = 60/12 * 2;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 136 && delay == 1) {
		if(checkFloorCollision()) idle();
		else fall();
	}
	else if(aerial && checkFloorCollision()) dy = 0;
	else {}
}
void Mario::fthrow() {
	if(action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 153, 155, 12, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60/12 * (155-153+1);
		action = FTHROW;		
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 153) {
		if(direction == LEFT) grabbedenemy -> dx = 8;
		else grabbedenemy -> dx = -8;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 154 or PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 155) {
		if(direction == LEFT) grabbedenemy -> dx = -8;
		else grabbedenemy -> dx = 8;
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
		grabbedenemy -> percentage += 7;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;	
		idle();
	}
}
void Mario::bthrow() {
	if(action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 156, 162, 12, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60/12 * (162-156+1);
		action = BTHROW;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 156 or PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 157) {
		if(direction == LEFT) grabbedenemy -> dx = 4;
		else grabbedenemy -> dx = -4;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 158) {
		grabbedenemy -> dx = 0;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 159 or PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 160) {
		if(direction == LEFT) grabbedenemy -> dx = -4;
		else grabbedenemy -> dx = 4;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 161) {
		grabbedenemy -> dx = 0;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 162) {
		if(direction == LEFT) grabbedenemy -> dx = 8;
		else grabbedenemy -> dx = -8;
	}
	if(delay <= 0) {
		int mult = 1;
		grabbedenemy -> k = Knockback(2.5, -2, 7);
		if(direction == RIGHT) {
			mult = -1;
		}
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 9;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Mario::uthrow() {
	if(action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 163, 165, 12, ANIM_LOOP, -1);
		playsound(UTHROW);
		delay = 60/12 * (165-163+1);
		action = UTHROW;
		if(direction == LEFT) grabbedenemy -> dx = 2;
		else grabbedenemy -> dx = -2;
	}
	if(delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(0, -3, 5);
		if(direction == RIGHT) {
			mult = 1;
		}
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 6;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();		
	}
}
void Mario::dthrow() {
	if(action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 165, 168, 12, ANIM_LOOP, -1);
		playsound(DTHROW);
		delay = 60/12 * (168-165+1);
		action = DTHROW;
		if(direction == LEFT) grabbedenemy -> dx = 2;
		else grabbedenemy -> dx = -2;
		grabbedenemy -> dy = .5;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 167) {
		grabbedenemy -> dx = 0;
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
		grabbedenemy -> percentage += 6;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Mario::jaywalk() {}
Mario::~Mario() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);		
}

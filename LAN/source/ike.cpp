#include "ike.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
using std::vector;

// constructor
Ike::Ike(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num, listplayers, disp, "ike", AI) {
	w1 = 1.035;
	w2 = 0.28;
	jumpheight = 100;
	doublejumpheight = 90;
	shieldstr = 64;
	runspeed = 2.75;
	handx = 64 - 35;
	handy = 28;
	hangx = 64 - 37;
	hangy = 10;
	shieldx = 19;
	shieldy = 33;
	eruptioncharge = 0;
	quickdrawcharge = 0;
	CAPE = false;
	COUNTER = false;
	MYCHAR = IKE;
	series = "fireemblem";
	topside = 15;
	bottomside = 47;
	rightside = 37;
	leftside = 13;
	gravity = GLOBALGRAVITY + w2/10;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Ike::initSounds() {
	int alreadymade = 0;
	for (int n = 0; n < charnum; n++) {
		if (players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if (alreadymade == 0) {
		PA_FatLoadSfx("ikebair", "characters/ike/bair");
		PA_FatLoadSfx("ikedashattack", "characters/ike/dashattack");
		PA_FatLoadSfx("ikefair", "characters/ike/fair");
		PA_FatLoadSfx("ikefsmash", "characters/ike/fsmash");
		PA_FatLoadSfx("ikeftilt", "characters/ike/ftilt");
		PA_FatLoadSfx("ikejump", "characters/ike/jump");
		PA_FatLoadSfx("ikeutilt", "characters/ike/utilt");
		PA_FatLoadSfx("ikebside", "characters/ike/bside");
		PA_FatLoadSfx("ikebup", "characters/ike/bup");
		PA_FatLoadSfx("ikebneut", "characters/ike/bneut");
		PA_FatLoadSfx("ikebdown", "characters/ike/bdown");
	}
}
void Ike::initPalettes() {
	palettes.push_back("characters/ikeblue");
	palettes.push_back("characters/ikewhite");
	palettes.push_back("characters/ikegold");
	palettes.push_back("characters/ikegreen");
}
void Ike::playsound(int sndnum) {
	if (sndnum == BAIR) PA_FatPlaySfx("ikebair");
	if (sndnum == DASHATTACK) PA_FatPlaySfx("ikedashattack");
	if (sndnum == FAIR) PA_FatPlaySfx("ikefair");
	if (sndnum == SMASHLEFT || sndnum == SMASHRIGHT) PA_FatPlaySfx("ikefsmash");
	if (sndnum == FTILT) PA_FatPlaySfx("ikeftilt");
	if (sndnum == JUMP) PA_FatPlaySfx("ikejump");
	if (sndnum == UTILT) PA_FatPlaySfx("ikeutilt");
}
// actions
double Ike::cpu_specialweight() { return .01; }
void Ike::cpu_dospecial() {
	if (action != BNEUT) bneut();
	else if (cpu_target_distance < 50 && eruptioncharge > 10) erupt();
}
void Ike::bside() {
	if (action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 108, 108, 12, ANIM_LOOP, -1);
		delay = 60 / 12 * 1;
		setDirection();
		dx = 0;
		dy = 0;
		action = BSIDE;
		quickdrawcharge = 0;
	}
	else if (custom_action(ACTION_SPECIAL, PAD_RELEASED) && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 108 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 110, 110, 1, ANIM_LOOP, -1);
		PA_FatPlaySfx("ikebside");
		delay = quickdrawcharge / 2;
		if (direction == RIGHT) dx = 2 * (int)(quickdrawcharge / 20) + 4;
		else dx = -1 * 2 * (int)(quickdrawcharge / 20) - 4;
	}
	else if ((delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 108) || (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 109, 109, 12, ANIM_LOOP, -1);
		delay = 60 / 12 * 1;
		quickdrawcharge++;
		if (quickdrawcharge > 120) quickdrawcharge = 120;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 108 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109) {
		quickdrawcharge++;
		if(quickdrawcharge > 120) quickdrawcharge = 120;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 110) {
		dx = 0;
		permafall();
	}
	else if (aerial && checkFloorCollision()) {
		aerial = false;
		dy = 0;
	}
	else {
		for(int n = 0; n < (int)getAtkbox().getCircles().size(); n++) {
			allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)].circles[n].damage = (int)(quickdrawcharge/10.0 + 1); // 1-12
			allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)].circles[n].setKnockback(-quickdrawcharge/120.0, quickdrawcharge/60.0, getAtkbox().getCircles()[n].getKnockback().length);
		}
		allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)].enabled = true;
	}
	if(!aerial && !checkFloorCollision()) dx = 0;
}
void Ike::bup() {
	if (action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 118, 122, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 5;
		setDirection();
		dx = 0;
		if (aerial) dy = -gravity;
		else dy = 0;
		action = BUP;
		int directionmodifier = 1;
		if (direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(Circle(32, 32, 15, Knockback(0, 0, 2), 3));
		Projectile p = Projectile(x, y, -.1 * directionmodifier, -4, 30, IKESWORD, charnum, tempbox, stage, display);
		((vector<Projectile>*)getProj())->push_back(p);
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 127, 127, 15, ANIM_LOOP, -1);
		PA_FatPlaySfx("ikebup");
		delay = 60 / 15 * 1;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 127) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 128, 128, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 3;
		dy = -gravity - 6;
		aerial = true;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 128) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 129, 134, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 6;
		dy = -gravity;
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 135 && checkFloorCollision()) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 136, 138, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 3;
		dy = 0;
		aerial = false;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 138) {
		idle();
	}
	else if ((delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134) || (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 135)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 135, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 1;
		dy = -gravity + 6;
	}
}
void Ike::bdown() {
	if (action != BDOWN) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 139, 142, 5, ANIM_UPDOWN, 5);
		PA_FatPlaySfx("ikebdown");
		delay = 60 / 5 * 5;
		setDirection();
		dx = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		action = BDOWN;
		COUNTER = true;
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 141 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
		COUNTER = false;
	}
	else if (aerial && checkFloorCollision()) dy = 0;
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 142) {
		COUNTER = false;
		fall();
	}
}
void Ike::erupt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 100, 109, 10, ANIM_LOOP, -1);
	PA_FatPlaySfx("ikebneut");
	delay = 60 / 10 * 10;
	eruptioncharge = 0;
}
void Ike::bneut() {
	if (action != BNEUT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 99, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 1;
		setDirection();
		dx = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		action = BNEUT;
		eruptioncharge = 0;
	}
	else if ((!isCPU && (custom_action(ACTION_SPECIAL, PAD_RELEASED) && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99)) || (eruptioncharge >= 300 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99)) {
		erupt();
	}
	else if (checkFloorCollision() && aerial) dy = 0;
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 109) {
		fall();
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 99, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 1;
		eruptioncharge++;
		if (eruptioncharge > 300) eruptioncharge = 300;
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) {
		eruptioncharge++;
		if (eruptioncharge > 300) eruptioncharge = 300;
	}
	if(isCPU) cpu_dospecial(); //give it a chance to think about erupting
}
void Ike::fthrow() {
	if (action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 152, 155, 15, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60 / 15 * (155 - 152 + 1);
		action = FTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -2;
		else grabbedenemy -> dx = 2;
	}
	if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 154 or PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 155) {
		if (direction == RIGHT) grabbedenemy -> dx = 4;
		else grabbedenemy -> dx = -4;
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(2, -2, 7);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Ike::bthrow() {
	if (action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 156, 159, 10, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60 / 10 * (159 - 156 + 1);
		action = BTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -3;
		else grabbedenemy -> dx = 3;
		grabbedenemy -> dy = -1;
	}
	if (delay <= 0) {
		int mult = 1;
		grabbedenemy -> k = Knockback(2, -2.5, 7);
		if (direction == RIGHT) mult = -1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		if (direction == RIGHT) setDirection(LEFT);
		else setDirection(RIGHT);
		idle();
	}
}
void Ike::uthrow() {
	if (action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 160, 163, 12, ANIM_LOOP, -1);
		playsound(UTHROW);
		delay = 60 / 12 * (163 - 160 + 1);
		action = UTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -.5;
		else grabbedenemy -> dx = .5;
		grabbedenemy -> dy = -1;
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(.5, -3, 6);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Ike::dthrow() {
	if (action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 165, 168, 10, ANIM_LOOP, -1);
		playsound(DTHROW);
		delay = 60 / 10 * (168 - 165 + 1);
		action = DTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -1;
		else grabbedenemy -> dx = 1;
		grabbedenemy -> dy = .5;
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1, -2.5, 7);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 8;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Ike::jaywalk() {
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 96 && bottomside == 47) {
		bottomside = 64;
		y -= 17;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 98 && bottomside == 64) {
		bottomside = 47;
		y += 17;
	}
}
Ike::~Ike() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

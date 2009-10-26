#include "mewtwo.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
using std::vector;

// constructor
Mewtwo::Mewtwo(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num, listplayers, disp, "mewtwo", AI) {
	w1 = 1.01; // same as lucario for now
	w2 = -0.4; // same as lucario for now
	jumpheight = 140;
	doublejumpheight = 112;
	shieldstr = 64;
	runspeed = 4.5;
	rollspeed = 4.0;
	handx = 64 - 46;
	handy = 33;
	hangx = 64 - 37;
	hangy = 26;
	shieldx = 33;
	shieldy = 39;
	shadowballcharge = 0;
	upcount = downcount = leftcount = rightcount = 0;
	MYCHAR = MEWTWO;
	series = "pokemon";
	topside = 8;
	bottomside = 52;
	rightside = 44;
	leftside = 22;
	gravity = GLOBALGRAVITY + w2/10;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Mewtwo::initSounds() {
	int alreadymade = 0;
	for (int n = 0; n < charnum; n++) {
		if (players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if (alreadymade == 0) {
		PA_FatLoadSfx("mewtwousmash", "characters/mewtwo/usmash");
		PA_FatLoadSfx("mewtwobair", "characters/mewtwo/bair");
		PA_FatLoadSfx("mewtwofair", "characters/mewtwo/fair");
		PA_FatLoadSfx("mewtwobneut", "characters/mewtwo/bneut");
		PA_FatLoadSfx("mewtwofsmash", "characters/mewtwo/fsmash");
		PA_FatLoadSfx("mewtwodair", "characters/mewtwo/dair");
		PA_FatLoadSfx("mewtwouair", "characters/mewtwo/uair");
		PA_FatLoadSfx("mewtwodoublejump", "characters/mewtwo/doublejump");
		PA_FatLoadSfx("mewtwobdown", "characters/mewtwo/bdown");
		PA_FatLoadSfx("mewtwobup", "characters/mewtwo/bup");
	}
}
void Mewtwo::playsound(int sndnum) {
	if (sndnum == SMASHUP) PA_FatPlaySfx("mewtwousmash");
	if (sndnum == BAIR) PA_FatPlaySfx("mewtwobair");
	if (sndnum == FAIR) PA_FatPlaySfx("mewtwofair");
	if (sndnum == SMASHLEFT || sndnum == SMASHRIGHT) PA_FatPlaySfx("mewtwofsmash");
	if (sndnum == DAIR) PA_FatPlaySfx("mewtwodair");
	if (sndnum == UAIR) PA_FatPlaySfx("mewtwouair");
	if (sndnum == DOUBLEJUMP) PA_FatPlaySfx("mewtwodoublejump");
}
// sounds
void Mewtwo::initPalettes() {
	palettes.push_back("characters/mewtwo");
	palettes.push_back("characters/mewtwoblue");
	palettes.push_back("characters/mewtwogreen");
	palettes.push_back("characters/mewtwored");
}
// actions
void Mewtwo::bside() {
	if (action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 130, 135, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 6;
		setDirection();
		dx = 0;
		action = BSIDE;
	}
	else if(delay == 1) {
		fall();
	}
}
void Mewtwo::bup() {
	if (action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 125, 12, ANIM_ONESHOT);
		aerial = true;
		delay = 60 / 12 * 4;
		dy = -gravity; // hovers
		dx = 0;
		DI = 0;
		fastfall = 0;
		upcount = downcount = leftcount = rightcount = 0;
		action = BUP;
		setDirection();
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 125 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 126, 129, 12, ANIM_ONESHOT);
		aerial = true;
		delay = 60 / 12 * 4;
		dx = (rightcount - leftcount) * 10;
		dy = (downcount - upcount) * 10;
		PA_FatPlaySfx("mewtwobup");
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 129 && delay == 1) {
		upcount = downcount = rightcount = leftcount = 0;
		permafall();
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 126) dx = dy = 0;
	if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 123 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 124 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 125) {
		if (!isCPU) {
			if (Pad.Held.Up) upcount += 2;
			if (Pad.Held.Down) downcount += 2;
			if (Pad.Held.Right) rightcount += 2;
			if (Pad.Held.Left) leftcount += 2;
		}
		else {
			Floor mainfloor = stage -> getFloors()[0];
			if (y > mainfloor.y) upcount += 2;
			if (x > mainfloor.x + mainfloor.length) leftcount += 2;
			if (x < mainfloor.x) rightcount += 2;
		}
	}
	if (upcount > 10) upcount = 10;
	if (downcount > 10) downcount = 10;
	if (rightcount > 10) rightcount = 10;
	if (leftcount > 10) leftcount = 10;
}
void Mewtwo::bdown() {
	if (action != BDOWN || (custom_action(ACTION_SPECIAL, PAD_HELD) && delay == 1)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 136, 138, 10, ANIM_LOOP, -1);
		delay = 60 / 10 * 3;
		if (aerial) dy = -gravity / 2;
		dx = 0;
		action = BDOWN;
		PA_FatPlaySfx("mewtwobdown");
		ABSORB = true;
	}
	else if (!custom_action(ACTION_SPECIAL, PAD_HELD)) {
		ABSORB = false;
		fall();
	}
	else if (aerial && checkFloorCollision()) dy = 0;
}
void Mewtwo::fireshadowball() {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 147, 152, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 6;
		int SHADOWBALL_SIZE = -1;
		if (shadowballcharge >= 80) SHADOWBALL_SIZE = SHADOWBALL_LARGE;
		else if (shadowballcharge >= 40) SHADOWBALL_SIZE = SHADOWBALL_MEDIUM;
		else SHADOWBALL_SIZE = SHADOWBALL_SMALL;
		int directionmodifier = 1;
		if (direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		int rad = 0;
		double kbmod = 1;
		if (SHADOWBALL_SIZE == SHADOWBALL_LARGE) {
			rad = 14;
			kbmod = 1;
		}
		else if (SHADOWBALL_SIZE == SHADOWBALL_MEDIUM) {
			rad = 10;
			kbmod = 1.5;
		}
		else {
			rad = 8;
			kbmod = 3;
		}
		tempbox.addCircle(createAtkbox(32, 32, rad, Knockback(-3*directionmodifier / kbmod, -1.5 / kbmod, 8), 5 + (int)(shadowballcharge / 6)));
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_SIZE, charnum, tempbox, stage, display));
		shadowballcharge = 0;
		PA_FatPlaySfx("mewtwobneut");
}
void Mewtwo::bneut() {
	if (action != BNEUT) {
		if (shadowballcharge < 40) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 140, 141, 15, ANIM_LOOP, -1);
			delay = 60 / 15 * 2 * 5;
			dx = 0;
			action = BNEUT;
		}
		else if (shadowballcharge < 80) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 142, 143, 15, ANIM_LOOP, -1);
			delay = 60 / 15 * 2 * 5;
			dx = 0;
			action = BNEUT;
		}
		else if (shadowballcharge < 120) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 144, 145, 15, ANIM_LOOP, -1);
			delay = 60 / 15 * 2 * 5;
			dx = 0;
			action = BNEUT;
		}
		else if (shadowballcharge == 120) {
			action = BNEUT;
			fireshadowball();
		}
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 148 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 149 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 150 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 151) {}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 147) {
		if (delay == 1) {
			int directionmodifier = 1;
			if (direction == RIGHT) directionmodifier = -1;
			Hitbox tempbox;
			tempbox.addCircle(createAtkbox(32, 32, 14, Knockback(-3*directionmodifier, -1.5, 8), 240));
			((vector<Projectile>*)getProj())->push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_LARGE, charnum, tempbox, stage, display));
		}
	}
	else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 152) {
		if (delay == 1) {
			fall();
		}
	}
	else if (!isCPU && custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
		fireshadowball();
	}
	else if (!isCPU && custom_action(ACTION_SHIELD, PAD_NEWPRESS)) shield();
	else {
		shadowballcharge += 1;
		if (shadowballcharge > 120) shadowballcharge = 120;
		if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 141) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 142, 143, 15, ANIM_LOOP, -1);
			delay = 60 / 15 * 2 * 5;
		}
		else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 143) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 144, 145, 15, ANIM_LOOP, -1);
			delay = 60 / 15 * 2 * 5;
		}
		else if (delay == 1 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 145 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM == 146))) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 146, 146, 20, ANIM_LOOP, -1);
			delay = 60 / 20 * 1 * 5;
		}
	}
	if (!isCPU && !aerial) {
		if (Pad.Held.Right) roll(1);
		if (Pad.Held.Left) roll(-1);
	}
	if (isCPU) cpu_dospecial();
}
void Mewtwo::fthrow() {
	if (action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 156, 162, 20, ANIM_LOOP, -1);
		delay = 60 / 20 * 7;
		action = FTHROW;
		playsound(FTHROW);
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(2, -2, 10);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 10;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
	else {
		if (direction == RIGHT) grabbedenemy -> dx = 1;
		else grabbedenemy -> dx = -1;
		grabbedenemy -> dy = -1;
	}
}
void Mewtwo::bthrow() {
	if (action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 163, 166, 10, ANIM_LOOP, -1);
		delay = 60 / 10 * 4;
		action = BTHROW;
		playsound(BTHROW);
		int mult = 1;
		grabbedenemy -> k = Knockback(2, -2, 12);
		if (direction == RIGHT) mult = -1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 7;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if (delay <= 0) idle();
}
void Mewtwo::uthrow() {
	if (action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 174, 177, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 4;
		playsound(UTHROW);
		action = UTHROW;
		grabbedenemy -> dy = -2.5;
	}
	if (delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1, -2, 7);
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
void Mewtwo::dthrow() {
	if (action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 167, 173, 15, ANIM_LOOP, -1);
		delay = 60 / 15 * 7;
		playsound(DTHROW);
		action = DTHROW;
		if (direction == RIGHT) grabbedenemy -> dx = -.5;
		else grabbedenemy -> dx = .5;
		grabbedenemy -> dy = .25;
	}
	if (grabbedenemy != NULL && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 168) {
		int mult = -1;
		grabbedenemy -> k = Knockback(.5, -2, 7);
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 9;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if (delay <= 0) idle();
}
void Mewtwo::jaywalk() {}
double Mewtwo::cpu_specialweight() { return .1; }
void Mewtwo::cpu_dospecial() {
	if (action == BNEUT) {
		if (shadowballcharge == 120) fireshadowball();
		else if (cpu_target_distance < 20) fireshadowball();
	}
	else if (shadowballcharge < 120) {
		if (cpu_target_distance > 30) bneut();
	}
	else fireshadowball();
}
Mewtwo::~Mewtwo() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

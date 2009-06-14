#include "mewtwo.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
using std::vector;

// constructor
Mewtwo::Mewtwo(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num,listplayers,disp,"mewtwo",AI) {
	weight = 1.01; // same as lucario for now
	w2value = -0.4; // same as lucario for now
	jumpspeed = 7;
	doublejumpspeed = 7;
	shieldstr = 64;
	runspeed = 4.5;
	handx = 16;
	handy = 12;
	shadowballcharge = 0;
	upcount = downcount = leftcount = rightcount = 0;
	MYCHAR = MEWTWO;
	series = "pokemon";
	topside = 8;
	bottomside = 52;
	rightside = 44;
	leftside = 22;
	gravity = 2.5;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Mewtwo::initSounds() {
	int alreadymade = 0;
	for(int n = 0; n < charnum; n++) {
		if(players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if(alreadymade == 0) {
		PA_FatLoadSfx("mewtwousmash", "mewtwousmash");
		PA_FatLoadSfx("mewtwobair", "mewtwobair");
		PA_FatLoadSfx("mewtwofair", "mewtwofair");
		PA_FatLoadSfx("mewtwobneut", "mewtwobneut");
		PA_FatLoadSfx("mewtwofsmash", "mewtwofsmash");
		PA_FatLoadSfx("mewtwodair", "mewtwodair");
		PA_FatLoadSfx("mewtwouair", "mewtwouair");
		PA_FatLoadSfx("mewtwodoublejump", "mewtwodoublejump");
		PA_FatLoadSfx("mewtwobdown", "mewtwobdown");
		PA_FatLoadSfx("mewtwobup", "mewtwobup");
	}
}
void Mewtwo::playsound(int sndnum) {
	if(sndnum == SMASHUP) PA_FatPlaySfx("mewtwousmash");
	if(sndnum == BAIR) PA_FatPlaySfx("mewtwobair");
	if(sndnum == FAIR) PA_FatPlaySfx("mewtwofair");
	if(sndnum == SMASHLEFT || sndnum == SMASHRIGHT) PA_FatPlaySfx("mewtwofsmash");
	if(sndnum == DAIR) PA_FatPlaySfx("mewtwodair");
	if(sndnum == UAIR) PA_FatPlaySfx("mewtwouair");
	if(sndnum == DOUBLEJUMP) PA_FatPlaySfx("mewtwodoublejump");
}
// sounds
void Mewtwo::initPalettes() {
	palettes.push_back("mewtwo");
	palettes.push_back("mewtwoblue");
	palettes.push_back("mewtwogreen");
	palettes.push_back("mewtwored");
}
// actions
void Mewtwo::bside() {
	if(action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 136, 10, ANIM_LOOP, -1);
		delay = 60/10 * 2;
		setDirection();
		dx = 0;
		action = BSIDE;
	}
	else if((PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 141 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 136) && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 137, 141, 10, ANIM_LOOP, -1);
		delay = 60/10 * 5;
	}
	else if(custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
		if(checkFloorCollision()) idle();
		else fall();
	}
}
void Mewtwo::bup() {
	if(action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 119, 122, 10, ANIM_ONESHOT);
		aerial = true;
		delay = 60/10 * 4;
		dy = -gravity; // hovers
		dx = 0;
		DI = 0;
		fastfall = 0;
		upcount = downcount = leftcount = rightcount = 0;
		action = BUP;
		setDirection();
	}
	else if(action == BUP && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 125, 10, ANIM_ONESHOT);
		aerial = true;
		delay = 60/10 * 4;
		x += (rightcount-leftcount)*10;
		y += (downcount-upcount)*10;
		PA_FatPlaySfx("mewtwobup");
	}
	else if(action == BUP && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 125 && delay == 1) {
		upcount = downcount = rightcount = leftcount = 0;
		if(!checkFloorCollision()) fall();
		else idle();
	}
	if(action == BUP && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 120 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 121 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122) {
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
void Mewtwo::bdown() {
	if(action != BDOWN || (custom_action(ACTION_SPECIAL, PAD_HELD) && delay == 1)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 145, 10, ANIM_LOOP, -1);
		delay = 60/10 * 3;
		if(aerial) dy = -gravity/2;
		dx = 0;
		action = BDOWN;
		PA_FatPlaySfx("mewtwobdown");
	}
	else if(!custom_action(ACTION_SPECIAL, PAD_HELD)) {
		if(checkFloorCollision()) idle();
		else fall();
	}
	else if(aerial && checkFloorCollision()) dy = 0;
}
void Mewtwo::bneut() {
	if(action != BNEUT) {
		if(shadowballcharge < 40) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 126, 127, 15, ANIM_LOOP, -1);
			delay = 60/15 * 2 * 5;
			dx = 0;
			action = BNEUT;				
		}
		else if(shadowballcharge < 80) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 128, 129, 15, ANIM_LOOP, -1);
			delay = 60/15 * 2 * 5;
			dx = 0;
			action = BNEUT;
		}
		else if(shadowballcharge < 120) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 130, 131, 15, ANIM_LOOP, -1);
			delay = 60/15 * 2 * 5;
			dx = 0;
			action = BNEUT;
		}
		else if(shadowballcharge == 120) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 134, 15, ANIM_LOOP, -1);
			delay = 60/15 * 3;
			int directionmodifier = 1;
			if(direction == RIGHT) directionmodifier = -1;
			Hitbox tempbox;
			tempbox.addCircle(createAtkbox(32, 32, 14, Knockback(-3*directionmodifier, -1.5, 8), 240));
			((vector<Projectile>*)getProj())->push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_LARGE, charnum, tempbox, stage, display));
			shadowballcharge = 0;
			action = BNEUT;
			dx = 0;
			PA_FatPlaySfx("mewtwobneut");
		}
	}
	else if(custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 134, 15, ANIM_LOOP, -1);
		delay = 60/15 * 3;
		int SHADOWBALL_SIZE = -1;
		if(shadowballcharge >= 80) SHADOWBALL_SIZE = SHADOWBALL_LARGE;
		else if(shadowballcharge >= 40) SHADOWBALL_SIZE = SHADOWBALL_MEDIUM;
		else SHADOWBALL_SIZE = SHADOWBALL_SMALL;
		int directionmodifier = 1;
		if(direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		int rad = 0;
		double kbmod = 1;
		if(SHADOWBALL_SIZE == SHADOWBALL_LARGE) {
			rad = 14;
			kbmod = 1;
		}
		else if(SHADOWBALL_SIZE == SHADOWBALL_MEDIUM) {
			rad = 10;
			kbmod = 1.5;
		}
		else {
			rad = 8;
			kbmod = 3;
		}
		tempbox.addCircle(createAtkbox(32, 32, rad, Knockback(-3*directionmodifier/kbmod, -1.5/kbmod, 8), 5 + (int)(shadowballcharge/6)));
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_SIZE, charnum, tempbox, stage, display));
		shadowballcharge = 0;
		PA_FatPlaySfx("mewtwobneut");
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 132) {}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 133) {}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134) {
		if(delay == 1) {
			if(checkFloorCollision()) idle();
			else fall();
		}
	}
	else if(custom_action(ACTION_SHIELD, PAD_NEWPRESS)) shield();
	else {
		shadowballcharge+=1;
		if(shadowballcharge > 120) shadowballcharge = 120;
		if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 127) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 128, 129, 15, ANIM_LOOP, -1);
			delay = 60/15 * 2 * 5;
		}
		else if(delay == 1 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 129 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 131)) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 130, 131, 15, ANIM_LOOP, -1);
			delay = 60/15 * 2 * 5;
		}				
	}
}
void Mewtwo::fthrow() {
	if(action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 150, 154, 10, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60/15 * (154-150+1);
		action = FTHROW;
		int mult = -1;
		grabbedenemy -> k = Knockback(2, -3, 12);
		if(direction == RIGHT) {
			mult = 1;
		}
		grabbedenemy -> hitstun = (int) (grabbedenemy -> k.length * 3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 10;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if(delay <= 0) idle();
}
void Mewtwo::bthrow() {
	if(action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 148, 149, 10, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60/10 * (149-148+1);
		action = BTHROW;
		int mult = 1;
		grabbedenemy -> k = Knockback(2, -2, 14);
		if(direction == RIGHT) {
			mult = -1;
		}
		grabbedenemy -> hitstun = (int) (grabbedenemy -> k.length * 3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 7;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if(delay <= 0) idle();
}
void Mewtwo::uthrow() {
	if(action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 147, 147, 20, ANIM_LOOP, -1);
		playsound(UTHROW);
		delay = 60/20 * (147-147+1) * 5;
		action = UTHROW;
		grabbedenemy -> x = x;
		grabbedenemy -> y = y-32;
	}
	else if(delay <= 0) {
		grabbedenemy -> k = Knockback(0, -3.5, 10);
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
		grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx;
		grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 10;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}						
}
void Mewtwo::dthrow() {
	if(action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 155, 155, 10, ANIM_LOOP, -1);
		playsound(DTHROW);
		delay = 60/10 * (155-155+1);
		action = DTHROW;
		if(direction == RIGHT) dx = 3;
		else dx = -3;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 155) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 156, 157, 10, ANIM_LOOP, -1);
		delay = 60/10 * (157-156+1) * 5;
		dx = 0;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 157) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1, -2, 12);
		if(direction == RIGHT) {
			mult = 1;
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
void Mewtwo::jaywalk() {}
Mewtwo::~Mewtwo() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);		
}

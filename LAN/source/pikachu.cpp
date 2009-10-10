#include "pikachu.h"
#include <vector>
#include <PA9.h>
#include "display.h"
#include "projectiles.h"
#include "fighter.h"
using std:: vector;

// constructor
Pikachu::Pikachu(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num, listplayers, disp, "pikachu", AI) {
	weight = 0.904;
	w2value = 0.40;
	jumpspeed = 7;
	doublejumpspeed = 5.5;
	shieldstr = 64;
	runspeed = 4.0;
	handx = 64 - 48;
	handy = 33;
	hangx = 64 - 36;
	hangy = 27;
	skullbashcharge = 0;
	upcount = downcount = leftcount = rightcount = firstdir = 0;
	MYCHAR = PIKACHU;
	series = "pokemon";
	topside = 24;
	bottomside = 48;
	rightside = 44;
	leftside = 23;
	gravity = 2.5;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();

} // initializes all of the variables
// initializers
void Pikachu::initSounds() {
	int alreadymade = 0;
	for (int n = 0; n < charnum; n++) {
		if (players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if (alreadymade == 0) {
		PA_FatLoadSfx("pikachubdown", "characters/pikachu/bdown");
		PA_FatLoadSfx("pikachubneut", "characters/pikachu/bneut");
		PA_FatLoadSfx("pikachubside1", "characters/pikachu/bside1");
		PA_FatLoadSfx("pikachubside2", "characters/pikachu/bside2");
		PA_FatLoadSfx("pikachubup", "characters/pikachu/bup");
		PA_FatLoadSfx("pikachudsmash", "characters/pikachu/dsmash");
		PA_FatLoadSfx("pikachujab", "characters/pikachu/jab");
		PA_FatLoadSfx("pikachunair", "characters/pikachu/nair");
	}
}
void Pikachu::initPalettes() {
	palettes.push_back("characters/pikachuyellow");
	palettes.push_back("characters/pikachublue");
	palettes.push_back("characters/pikachured");
	palettes.push_back("characters/pikachugreen");
}
void Pikachu::playsound(int sndnum) {
	if (sndnum == JUMP) PA_FatPlaySfx("pikachubup");
	if (sndnum == DOUBLEJUMP) PA_FatPlaySfx("pikachubup");
	if (sndnum == SMASHDOWN) PA_FatPlaySfx("pikachudsmash");
	if (sndnum == NAIR) PA_FatPlaySfx("pikachunair");
	if (sndnum == JAB) PA_FatPlaySfx("pikachujab");
}
// actions
void Pikachu::bside() {
	if (action != BSIDE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 134, 138, 10, ANIM_LOOP, -1);
		delay = 60 / 10 * 5 * 5;
		setDirection();
		dx = 0;
		if (aerial) dy = -gravity / 2;
		else dy = 0;
		action = BSIDE;
		skullbashcharge = 0;
		PA_FatPlaySfx("pikachubside1");
	}
	else {
		if(aerial && checkFloorCollision()) dy = 0;
		if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) >= 134 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) <= 138 && custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 139, 142, 20, ANIM_LOOP, -1);
			delay = 60 / 20 * 4 * 2;
			dx = skullbashcharge/30.0;
			if(direction == LEFT) dx *= -1;
			for(int n = 0; n < (int)getAtkbox().getCircles().size(); n++) getAtkbox().getCircles()[n].damage = (int)(skullbashcharge/12.0 + 1);
			PA_FatPlaySfx("pikachubside2");
		}
		else if (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) >= 134 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) <= 138 && delay == 1) {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 134, 134, 10, ANIM_LOOP, -1);
			delay = 60 / 10 * 1 * 10;
		}
		else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) >= 139 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) <= 142) {
			if(checkFloorCollision()) idle();
			else fall();
		}
		else {
			skullbashcharge+=2;
			if(skullbashcharge > 300) skullbashcharge = 300;
		}
	}
}
void Pikachu::bup() {
	if(action != BUP) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 146, 15, ANIM_LOOP, -1);
		aerial = true;
		delay = 60/15 * 4;
		dy = -gravity;
		dx = 0;
		upcount = downcount = leftcount = rightcount = firstdir = 0;
		action = BUP;
	}
	else if (delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 146) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 147, 147, 15, ANIM_LOOP, -1);
		delay = 60/15 * 5;
		PA_FatPlaySfx("pikachubup");
		int xdiff = rightcount-leftcount;
		if(xdiff > 0) setDirection(RIGHT);
		else if(xdiff < 0) setDirection(LEFT);
		int ydiff = downcount-upcount;
		if(xdiff == 0 && ydiff == 0) ydiff = -1;
		if(xdiff == 0 && ydiff < 0) {
			y -= 50;
			firstdir = 90;
		}
		else if(xdiff == 0 && ydiff > 0) {
			y += 50; 
			firstdir = 270;
		}
		else if(xdiff < 0 && ydiff == 0) {
			x -= 50;
			firstdir = 180;
		}
		else if(xdiff > 0 && ydiff == 0) {
			x += 50;
			firstdir = 0;
		}
		else if(ydiff > 4*xdiff && ydiff > 0 && xdiff > 0) {
			x += 25;
			y += 43;
			firstdir = 300;
		}
		else if(ydiff > 4*xdiff && ydiff > 0 && xdiff < 0) {
			x -= 25;
			y += 43;
			firstdir = 240;
		}
		else if(ydiff > 4*xdiff && ydiff < 0 && xdiff > 0) {
			x += 25;
			y -= 43;
			firstdir = 60;
		}
		else if(ydiff > 4*xdiff && ydiff < 0 && xdiff < 0) {
			x -= 25;
			y -= 43;
			firstdir = 120;
		}
		else if(xdiff > 4*ydiff && xdiff > 0 && ydiff > 0) {
			x += 43;
			y += 25;
			firstdir = 330;
		}
		else if(xdiff > 4*ydiff && xdiff > 0 && ydiff < 0) {
			x += 43;
			y -= 25;
			firstdir = 30;
		}
		else if(xdiff > 4*ydiff && xdiff < 0 && ydiff > 0) {
			x -= 43;
			y += 25;
			firstdir = 210;
		}
		else if(xdiff > 4*ydiff && xdiff < 0 && ydiff < 0) {
			x -= 43;
			y -= 25;
			firstdir = 150;
		}
		else if(xdiff > 0 && ydiff > 0) {
			x += 35;
			y += 35;
			firstdir = 315;
		}
		else if(xdiff > 0 && ydiff < 0) {
			x += 35;
			y -= 35;
			firstdir = 45;
		}
		else if(xdiff < 0 && ydiff > 0) {
			x -= 35;
			y += 35;
			firstdir = 225;
		}
		else if(xdiff < 0 && ydiff < 0) {
			x -= 35;
			y -= 35;
			firstdir = 135;
		}
		upcount = downcount = leftcount = rightcount = 0;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 147) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 148, 148, 10, ANIM_LOOP, -1);
		delay = 60/10 * 1;
		int xdiff = rightcount-leftcount;
		if(xdiff > 0) setDirection(RIGHT);
		else if(xdiff < 0) setDirection(LEFT);
		int ydiff = downcount-upcount;
		bool again = true;
		if(xdiff == 0 && ydiff == 0) ydiff = -1;
		if(xdiff == 0 && ydiff < 0 && firstdir != 90) {
			y -= 50;
		}
		else if(xdiff == 0 && ydiff > 0 && firstdir != 270) {
			y += 50; 
		}
		else if(xdiff < 0 && ydiff == 0 && firstdir != 180) {
			x -= 50;
		}
		else if(xdiff > 0 && ydiff == 0 && firstdir != 0) {
			x += 50;
		}
		else if(ydiff > 4*xdiff && ydiff > 0 && xdiff > 0 && firstdir != 300) {
			x += 25;
			y += 43;
		}
		else if(ydiff > 4*xdiff && ydiff > 0 && xdiff < 0 && firstdir != 240) {
			x -= 25;
			y += 43;
		}
		else if(ydiff > 4*xdiff && ydiff < 0 && xdiff > 0 && firstdir != 60) {
			x += 25;
			y -= 43;
		}
		else if(ydiff > 4*xdiff && ydiff < 0 && xdiff < 0 && firstdir != 120) {
			x -= 25;
			y -= 43;
		}
		else if(xdiff > 4*ydiff && xdiff > 0 && ydiff > 0 && firstdir != 330) {
			x += 43;
			y += 25;
		}
		else if(xdiff > 4*ydiff && xdiff > 0 && ydiff < 0 && firstdir != 30) {
			x += 43;
			y -= 25;
		}
		else if(xdiff > 4*ydiff && xdiff < 0 && ydiff > 0 && firstdir != 210) {
			x -= 43;
			y += 25;
		}
		else if(xdiff > 4*ydiff && xdiff < 0 && ydiff < 0 && firstdir != 150) {
			x -= 43;
			y -= 25;
		}
		else if(xdiff > 0 && ydiff > 0 && firstdir != 315) {
			x += 35;
			y += 35;
		}
		else if(xdiff > 0 && ydiff < 0 && firstdir != 45) {
			x += 35;
			y -= 35;
		}
		else if(xdiff < 0 && ydiff > 0 && firstdir != 225) {
			x -= 35;
			y += 35;
		}
		else if(xdiff < 0 && ydiff < 0 && firstdir != 135) {
			x -= 35;
			y -= 35;
		}
		else again = false;
		if(again) PA_FatPlaySfx("pikachubup");
		firstdir = 0;

	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 146 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 147) {
		if(Pad.Held.Up) upcount++;
		if(Pad.Held.Down) downcount++;
		if(Pad.Held.Left) leftcount++;
		if(Pad.Held.Right) rightcount++;
	}
	else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 148) {
		permafall();
	}
}
void Pikachu::bdown() {
	if(action != BDOWN) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 149, 157, 10, ANIM_LOOP, -1);
		delay = 60/10 * 9;
		if(aerial) dy = -gravity/2;
		else dy = 0;
		if(Pad.Held.Right) dx = 1;
		else if(Pad.Held.Left) dx = -1;
		else dx = 0;
		
		Hitbox tempbox1;
		tempbox1.addCircle(createAtkbox(20, 32, 12, Knockback(-.5, -1.5, 7), 12));
		tempbox1.addCircle(createAtkbox(44, 32, 12, Knockback(.5, -1.5, 7), 12));
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y-96, 0, 0, delay, THUNDER1, charnum, tempbox1, stage, display));	
		
		PA_FatPlaySfx("pikachubdown");
		action = BDOWN;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 153 && aerial) {		
		dy = -gravity;
		dx = 0;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 157 && delay == 1) {
		if(aerial) fall();
		else idle();
	}
	if(delay == 52) {
		Hitbox tempbox2;
		for(int n = 4; n < 64; n+=8) {
			tempbox2.addCircle(createAtkbox(28, n, 4, Knockback(-1, -.5, 6), 10));
			tempbox2.addCircle(createAtkbox(36, n, 4, Knockback(1, -.5, 6), 10));
		}
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y-64, 0, 0, delay, THUNDER2, charnum, tempbox2, stage, display));		
	}
	if(delay == 50) {
		Hitbox tempbox3;
		for(int n = 4; n < 64; n+=8) {
			tempbox3.addCircle(createAtkbox(28, n, 4, Knockback(-1, -.5, 6), 10));
			tempbox3.addCircle(createAtkbox(36, n, 4, Knockback(1, -.5, 6), 10));
		}
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y, 0, 0, delay, THUNDER3, charnum, tempbox3, stage, display));	
	}
	if(delay == 25) {
		Hitbox tempbox4;
		tempbox4.addCircle(createAtkbox(22, 48, 15, Knockback(-2, -1.25, 8), 15));
		tempbox4.addCircle(createAtkbox(42, 48, 15, Knockback(2, -1.25, 8), 15));
		((vector<Projectile>*)getProj())->push_back(Projectile(x, y, 0, 0, delay, THUNDER4, charnum, tempbox4, stage, display));	
	}
	if(checkFloorCollision()) dx = 0;
}
void Pikachu::bneut() {
	if(action != BNEUT) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 129, 131, 10, ANIM_LOOP, -1);
		delay = 60/10 * 3;
		if(Pad.Held.Right) dx = 1;
		else if(Pad.Held.Left) dx = -1;
		else dx = 0;
		dy = 0;
		PA_FatPlaySfx("pikachubneut");
		action = BNEUT;
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 131 && delay == 1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 134, 10, ANIM_LOOP, -1);
		delay = 60/10 * 3;
		int directionmodifier = 1;
		if (direction == RIGHT) directionmodifier = -1;
		Hitbox tempbox;
		tempbox.addCircle(createAtkbox(32, 54, 10, Knockback((-.25*directionmodifier), -.5, 6), 10));
		Projectile p = Projectile(x+16, y-16, -2*directionmodifier, 3, 100, THUNDERSHOCK, charnum, tempbox, stage, display);
		p.miny = stage->getFloors()[0].y - 64;
		p.maxy = y;
		((vector<Projectile>*)getProj())->push_back(p);
	}
	else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134 && delay == 1) {
		if(checkFloorCollision()) idle();
		else fall();
	}
	if(checkFloorCollision()) dx = 0;
}
void Pikachu::fthrow() {
	if(action != FTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 162, 167, 15, ANIM_LOOP, -1);
		playsound(FTHROW);
		delay = 60 / 15 * 6;
		action = FTHROW;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 162) {
		if(direction == LEFT) grabbedenemy -> dx = -2;
		else grabbedenemy -> dx = 2;
	}
	else grabbedenemy -> dx = 0;
	if(delay <= 0) {
		int mult = -1;
		grabbedenemy -> k = Knockback(2, -2, 7);
		if(direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 9;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
		idle();
	}
}
void Pikachu::bthrow() {
	if(action != BTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 167, 176, 15, ANIM_LOOP, -1);
		playsound(BTHROW);
		delay = 60 / 15 * 10;
		action = BTHROW;
		if(direction == LEFT) {
			dx = 2;
			grabbedenemy -> dx = 2;
		}
		else {
			dx = -2;
			grabbedenemy -> dx = -2;
		}
	}
	if(delay <= 0) {
		int mult = 1;
		grabbedenemy -> k = Knockback(2.25, -1.75, 7);
		if (direction == RIGHT) mult = -1;
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
void Pikachu::uthrow() {
	if(action != UTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 183, 188, 10, ANIM_LOOP, -1);
		playsound(UTHROW);
		delay = 60/10 * 6;
		action = UTHROW;
		grabbedenemy -> dy = -2;
		if(direction == LEFT) grabbedenemy -> dx = 1.5;
		else grabbedenemy -> dx = -1.5;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 186 && grabbedenemy != NULL) {
		int mult = -1;
		if (direction == RIGHT) mult = 1;
		grabbedenemy -> dx = 0;
		grabbedenemy -> k = Knockback(0, -2.5, 7);
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
void Pikachu::dthrow() {
	if(action != DTHROW) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 177, 182, 10, ANIM_LOOP, -1);
		playsound(DTHROW);
		delay = 60/10 * 6;
		action = DTHROW;
		if(direction == LEFT) {
			grabbedenemy -> dx = -2.5;
			dx = -2;
		}
		else {
			grabbedenemy -> dx = 2.5;
			dx = 2;
		}
		grabbedenemy -> dy = .25;
	}
	if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 179 && grabbedenemy != NULL) {
		int mult = -1;
		grabbedenemy -> k = Knockback(1, -2.25, 7);
		if(direction == RIGHT) mult = 1;
		grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length * 3 * (1 + (grabbedenemy -> percentage / 100)));
		grabbedenemy -> kx = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dx * mult;
		grabbedenemy -> ky = (1 + (grabbedenemy -> percentage / 100)) * grabbedenemy -> k.dy;
		grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
		grabbedenemy -> percentage += 7;
		grabbedenemy -> stun();
		grabbedenemy -> lasthitby = charnum;
		grabbedenemy = NULL;
	}
	if(delay <= 0) {
		if(direction == RIGHT) direction = LEFT;
		else direction = RIGHT;
		idle();
	}
}
void Pikachu::jaywalk() {}
Pikachu::~Pikachu() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}
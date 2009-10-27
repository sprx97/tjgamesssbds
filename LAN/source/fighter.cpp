#include <PA9.h> // DS functions that we use come from here
#include "hitbox.h"
#include "knockback.h"
#include "circle.h"
#include "fighter.h"
#include "stage.h"
#include "effect.h"
#include <vector>
#include <math.h>
#include <map> // maps
#include "gfx/all_gfx.h" //may want to reduce this later

using std::vector;

Fighter::Fighter(int num, vector<Fighter*>* listplayers, Display *disp, string n, bool AI) {
	freezelen = 0;
	invincibility = 0;
	walkspeed = 1.0;
	rollspeed = 2.0;
	DIval = 1.0;
	grabatkdamage = 3.0;
	traction = .25;
	airresistance = .2;
	name = n;
	display = disp;
	players = *listplayers;
	shieldstr = 64;
	shieldbroken = false;
	myledge = -1;
	x = 0;
	y = 0;
	respawntimer = 0;
	hangtime = 0;
	ledgewait = 0;
	CAPE = false;
	ABSORB = false;
	iswalking = false;
	COUNTER = false;
	PERMAFALL = false;
	effectwait = 0;
	chargecount = 0;
	isCPU = AI;
	isdead = false;
	lasthitby = -1;
	SPRITENUM = num + 100;
	charnum = players.size();
	startx = x;
	starty = y;
	aerial = false;
	delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
	dx = dy = fastfall = DI = 0.0;
	k = Knockback(0, 0, 0);
	kx = ky = 0;
	percentage = 0.0;
	ymomentum = 0.0;
	momentumtime = 0;
	grabtimeleft = 0;
	direction = RIGHT;
	initAtkbox();
	initDefbox();
} // initializes all of the variables
// virtual methods
void Fighter::initDefbox() {
	for (int n = 0; n < 250; n++) {
		Hitbox h;
		alldefbox.push_back(h);
	}
	FILE* file = fopen(("/SSBDS_Files/hitboxes/" + name+ "/" + name + ".def").c_str(), "rb");
	if (!file) while (true) {}
	char line[64];
	while (true) {
		fgets(line, 64, file);
		int frame = atoi(strtok(line, " \t"));
		if (frame == -1) break;
		double xpos = atof(strtok(NULL, " \t"));
		double ypos = atof(strtok(NULL, " \t"));
		double radius = atof(strtok(NULL, " \t"));

		alldefbox[frame].addCircle(Circle(xpos, (int)(ypos) % 64, radius, FX_NONE));
	}
	fclose(file);
}
void Fighter::initAtkbox() {
	for (int n = 0; n < 250; n++) {
		Hitbox h;
		allatkbox.push_back(h);
	}
	FILE* file = fopen(("/SSBDS_Files/hitboxes/" + name + "/" + name + ".atk").c_str(), "rb");
	if (!file) while (true) {}
	char line[64];
	while (true) {
		fgets(line, 64, file);

		int frame = atoi(strtok(line, " \t"));
		if (frame == -1) break;
		double xpos = atof(strtok(NULL, " \t"));
		double ypos = atof(strtok(NULL, " \t"));
		double radius = atof(strtok(NULL, " \t"));
		double kbx = atof(strtok(NULL, " \t"));
		double kby = atof(strtok(NULL, " \t"));
		int kblen = atoi(strtok(NULL, " \t"));
		int dmg = atoi(strtok(NULL, " \t"));
		int prior = atoi(strtok(NULL, "\t"));

		allatkbox[frame].addCircle(createAtkbox(xpos, (int)(ypos) % 64, radius, Knockback(kbx, kby, kblen), dmg, prior));
		allatkbox[frame].enabled = true;
	}
	fclose(file);
}
void Fighter::initSprite() {
	int alreadymade = 0;
	for (uint8 n = 0; n < players.size(); n++) {
		if (players[n] -> name == name) alreadymade++;
	}
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, SPRITENUM - 100, palettes[alreadymade]);
	if (alreadymade == 0) {
		PA_FatLoadSprite(MYCHAR, ("characters/"+name).c_str());
	}
	PA_CreateSprite(MAIN_SCREEN, SPRITENUM, (void*)sprite_gfx[MYCHAR], OBJ_SIZE_64X64, COLOR256, SPRITENUM - 100, -64, -64);
	PA_CreateSprite(MAIN_SCREEN, SPRITENUM - 4, (void*)sprite_gfx[13], OBJ_SIZE_64X64, COLOR256, 13, -64, -64);
	PA_SetSpriteRotEnable(MAIN_SCREEN, SPRITENUM - 4, SPRITENUM - 100);
}
void Fighter::initFrames() {
	FILE* file = fopen(("/SSBDS_Files/hitboxes/" + name + "/" + name + ".frame").c_str(), "rb");
	if (!file) while (true) {}
	char line[64];
	while (true) {
		fgets(line, 64, file);

		int startframe = atoi(strtok(line, " \t"));
		if (startframe == -1) break;
		int endframe = atoi(strtok(NULL, " \t"));
		int framespeed = atoi(strtok(NULL, " \t"));

		startframes.push_back(startframe);
		endframes.push_back(endframe);
		framespeeds.push_back(framespeed);
	}
	fclose(file);
}//implemented in subclasses
void Fighter::initPalettes() {}//implemented in subclasses
//CPU helper methods
int Fighter::cpu_getTarget() {
	int Cenemy = -1;
	double Cdistance = 10000000; //I don't like this, but it works
	for (int n = 0; n < (int)players.size(); n++) {
		if (players[n] -> charnum != charnum) {
			double deltax = (players[n] -> x) - x;
			double deltay = (players[n] -> y) - y;
			double distance = sqrt(deltax * deltax + deltay * deltay);
			if (distance < Cdistance) {
				Cenemy = n;
				Cdistance = distance;
			}
		}
	}
	cpu_target = Cenemy;
	cpu_target_distance = Cdistance;
	return Cenemy;
}
void Fighter::cpu_obeyRules() {
	if (delay > 0) delay--;
	if (action == BSIDE) bside();
	if (action == BUP) bup();
	if (action == BDOWN) bdown();
	if (action == BNEUT) bneut();
	if (hitstun > k.length*2) {
		aerial = true;
		hitstun--;
		dx = kx;
		dy = ky;
		if (hitstun == 0) {
			shieldbroken = false;
			fall();
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
		aerial = true;
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
			shieldbroken = false;
			fall();
		}
		dy = ky;
		dx = kx;
		if (checkFloorCollision() || checkCeilingCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
		}
		if(checkWallCollision()) {
			kx *= -1;
			dx = kx;
		}
	}
	else {
		if (landinglag > 0) {
			landinglag--;
			if (landinglag == 0) idle();
		}
		if (action == DODGE && delay <= 0) shield();
		if (action == AIRDODGE && delay <= 0) permafall();
		if (action == ROLL && delay <= 0) {
			dx = 0;
			if (direction == LEFT) {
				direction = RIGHT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			}
			else if (direction == RIGHT) {
				direction = LEFT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			}
			shield();
		}//done rolling
		if (action == RELEASED || action == RELEASE) {
			if (delay <= 0) idle();
		}
		if (action == ATTACK && delay <= 0) idle();
		if (action == SLIDE && delay <= 0) idle();
		if (action == SLIDE) {
			if (dx > 0) {
				dx -= traction;
				if (dx <= 0) dx = 0;
			}
			else if (dx < 0) {
				dx += traction;
				if (dx >= 0) dx = 0;
			}
		}
		if (action == DASHATTACK) {
			x += dx;
			if (delay <= 0) idle();
		}
		if (action == BSIDE || action == BUP || action == BDOWN || action == BNEUT) {
			if (delay <= 0) idle();
		}
		if (action == AIRATTACK) {
			if (checkFloorCollision()) {
				if (delay > 0) land();
				else idle();
				delay = 0;
			}
			else if (delay <= 0) fall();
		}// checks for stage collision with aerial
		if ((action == JUMP || action == DOUBLEJUMP) && delay <= 0) fall(); // falls when jump/multijump are finished animating
		if (action == FALL) {
			if (checkFloorCollision()) idle();
		}
		if (action == SHIELD) {
			//shield shrinks/breaks
			shieldstr -= (65 - shieldstr) / 100;
			if (shieldstr <= 0) {
				shieldstr = 64;
				PA_FatPlaySfx("shieldbreak");
				hitstun = 300;
				shieldbroken = true;
				stun();
			}
			PA_SetRotsetNoAngle(MAIN_SCREEN, SPRITENUM - 100, (int)(2048 - 24*shieldstr), (int)(2048 - 24*shieldstr));
		}
	}
}//do all AI actions that would be cheating to skip
void Fighter::actCPU() {
	// temporary variables. Will be changable in future
	int range = 40;
	int level = 1;

	//Pick a target
	int Cenemy = cpu_getTarget();
	double Cx = (players[Cenemy] -> x) - x;
	double Cy = (players[Cenemy] -> y) - y;
	double Cdistance = sqrt(Cx * Cx + Cy * Cy);
	double Cangle = atan2(Cy, Cx) * 180 / M_PI; // from -180 to 180
	cpu_obeyRules(); //do all AI actions that would be cheating to skip
	//do actions that require thinking, strategy, or input
	if (respawntimer > 0) {
		return respawn();
	}
	if (hitstun > 0) {
		if (Cangle < 90 && Cangle > -90) directionalInfluence(-DIval);
		else if (Cangle > 90 || Cangle < -90) directionalInfluence(DIval);
	}
	else {
		if (action == JUMP || action == DOUBLEJUMP) {
			if (x < stage->getFloors()[0].x || x > stage->getFloors()[0].x + stage->getFloors()[0].length) {
				vector<Ledge> ledges = stage->getLedges();
				double mindist = 10000000;
				int minledge = 0;
				for (int i = 0; i < (int)ledges.size(); i++) {
					double tempdist = sqrt((ledges[i].x - x) * (ledges[i].x - x) + (ledges[i].y - y) * (ledges[i].y - y));
					if (tempdist < mindist) {
						mindist = tempdist;
						minledge = i;
					}
				}
				Cx = x - ledges[minledge].x;
				Cy = y - ledges[minledge].y;
				if (Cx > 0) directionalInfluence(-DIval);
				if (Cx < 0) directionalInfluence(DIval);
			}
			else if (Cangle < 90 && Cangle > -90) directionalInfluence(DIval);
			else if (Cangle > 90 || Cangle < -90) directionalInfluence(-DIval);
			// act in air
		}
		if (action == JAB) {}
		if (action == AIRATTACK) {
			if (x < stage->getFloors()[0].x || x > stage->getFloors()[0].x + stage->getFloors()[0].length) {
				vector<Ledge> ledges = stage->getLedges();
				double mindist = 10000000;
				int minledge = 0;
				for (int i = 0; i < (int)ledges.size(); i++) {
					double tempdist = sqrt((ledges[i].x - x) * (ledges[i].x - x) + (ledges[i].y - y) * (ledges[i].y - y));
					if (tempdist < mindist) {
						mindist = tempdist;
						minledge = i;
					}
				}
				Cx = x - ledges[minledge].x;
				Cy = y - ledges[minledge].y;
				if (Cx > 0) directionalInfluence(-DIval);
				if (Cx < 0) directionalInfluence(DIval);
			}

			if (Cangle < 90 && Cangle > -90) directionalInfluence(DIval);
			else if (Cangle > 90 || Cangle < -90) directionalInfluence(-DIval);
		}
		if (aerial && action != AIRATTACK && action != AIRLAG && action != JUMP && action != DOUBLEJUMP) {
			// act air
			// Am I in danger?
			if (x < stage->getFloors()[0].x || x > stage->getFloors()[0].x + stage->getFloors()[0].length) {
				//Find closest ledge
				vector<Ledge> ledges = stage->getLedges();
				double mindist = 1000000;
				int minledge = 0;
				for (int i = 0; i < (int)ledges.size(); i++) {
					double tempdist = sqrt((ledges[i].x - x) * (ledges[i].x - x) + (ledges[i].y - y) * (ledges[i].y - y));
					if (tempdist < mindist) {
						mindist = tempdist;
						minledge = i;
					}
				}
				Cx = x - ledges[minledge].x;
				Cy = y - ledges[minledge].y;
				if (Cx > 0) directionalInfluence(-DIval);
				if (Cx < 0) directionalInfluence(DIval);
				if (Cy > 0) {
					if(!PERMAFALL) {
						if (jumpcount < jumpmax) doubleJump();
						else {
							bup();
							if (Cx < 0) setDirection(RIGHT);
							else if (Cx > 0) setDirection(LEFT);
						}
					}
				} //The ledge is above me
			}
			else if(PERMAFALL) {}
			else if (Cdistance < range) {
				if (Cangle > -45 && Cangle < 45 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					if (direction == RIGHT) fair();
					else bair();
				}
				else if (Cangle < -135 || Cangle > 135 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					if (direction == RIGHT) bair();
					else fair();
				}
				else if (Cangle < -45 && Cangle > -135 && (int)PA_RandMax(100) > 100 - level*10 + 5) uair();
				else if (Cangle > 45 && Cangle < 135 && (int)PA_RandMax(100) > 100 - level*10 + 5) dair();
			}
			else if(Cdistance < range) dair();
			else {
				if (Cangle < -45 && Cangle > -135 && jumpcount < jumpmax && action != JUMP && action != DOUBLEJUMP) {
					if (Cx > 0) setDirection(RIGHT);
					if (Cx < 0) setDirection(LEFT);
					doubleJump();
				}
				else if (Cangle < 45 && Cangle > -45) {
					directionalInfluence(DIval); //right
					fall();
				}
				else if (Cangle > 135 || Cangle < -135) {
					directionalInfluence(-DIval);
					fall();
				}
			}//too far away
		}
		if (action == SHIELD) {
			//What should I do?
			//I dunno. Why don't you guess?

			int randn = PA_Rand() % 10; //0-9
			//should I grab?
			//grab();
			//should I dodge?
			if (randn == 0)
				dodge();
			//should I stop shielding?
			else if (randn == 1 || randn == 2)
				idle();
			//should I roll?
			else if (randn == 3)
				roll();
			//Otherwise, just keep shielding
		}
		if (action == RUN) {
			if(aerial) fall();
			else if ((Cdistance < range) || (dx < 0 && (Cangle < 90 && Cangle > -90)) || (dx > 0 && (Cangle > 90 || Cangle < -90))) slide();
			else if (Cangle < -45 && Cangle > -135 && jumpcount == 0) {
				if (Cx > 0) setDirection(RIGHT);
				if (Cx < 0) setDirection(LEFT);
				jump();
			}
			else {
				if (direction == RIGHT) run(1);
				else run(-1);
			}
			// or dash attack
			// or run more
			// or slide
		}
		if (action == IDLE) {
			int randn = PA_Rand() % 100; //0-99
			double specialweight = cpu_specialweight();
			if (randn < 50*specialweight) { // up to 50/100
				if (Cx > 0) setDirection(RIGHT);
				if (Cx < 0) setDirection(LEFT);
				cpu_dospecial();
			}
			else if (Cdistance < range) {
				if (Cangle > -45 && Cangle < 45 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					setDirection(RIGHT);
					if ((int)PA_RandMax(100) > 75) smashright();
					else ftilt();
				}
				else if (Cangle < -135 || Cangle > 135 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					setDirection(LEFT);
					if ((int)PA_RandMax(100) > 75) smashleft();
					else ftilt();
				}
				else if (Cangle < -45 && Cangle > -135 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					if ((int)PA_RandMax(100) > 75) smashup();
					else utilt();
				}
				else if (Cangle > 45 && Cangle < 135 && (int)PA_RandMax(100) > 100 - level*10 + 5) {
					if ((int)PA_RandMax(100) > 75) smashdown();
					else dtilt();
				}
			}
			else if (Cangle < -45 && Cangle > -135 && jumpcount == 0 && Cdistance > range) {
				if (Cx > 0) setDirection(RIGHT);
				if (Cx < 0) setDirection(LEFT);
				jump();
			}
			else if (Cangle < 45 && Cangle > -45) { //right
				run(1);
			}
			else if (Cangle > 135 || Cangle < -135) { //left
				run(-1);
			}
		}
		if (action == CROUCH) {
			// idle
			// or dtilt
			// or bdown
		}
		if (action == HANG) {//FIXME: make sure AI can fall off ledge if it forgets to come up
			if (PA_RandMax(100) > 98) {
				myledge = -1;
				int randn = PA_Rand() % 3;
				if (randn == 0) rollUp();
				else if (randn == 1) attackUp();
				else jumpUp();
			}
		} //AI is hanging from ledge
	}
	move();
}
void Fighter::act() {
	std::map<int, int> customcontrols = getcustomcontrols();
	if (effectwait > 0) effectwait--;
	if (ledgewait > 0) ledgewait--;
	if (invincibility > 0) invincibility--;
	if (delay <= 0) {
		for (int n = 0; n < (int)allatkbox.size(); n++) {
			allatkbox[n].enabled = true;
		}
	}
	if(freezelen > 0) {
		freezelen--;
		if(freezelen == 0) {
			if(action == STUN) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[STUN], endframes[STUN], framespeeds[STUN], ANIM_LOOP, -1);
			else spriteanims[MAIN_SCREEN][SPRITENUM] = tempanim;
		}
		else PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM), PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM), 1, ANIM_LOOP, -1);
		return;
	}
	if (isCPU) {
		actCPU();
		return;
	}
	if (respawntimer > 0) return respawn();
	if (action == BSIDE) bside();
	if (action == BUP) bup();
	if (action == BDOWN) bdown();
	if (action == BNEUT) bneut();
	if (action == FTHROW) fthrow();
	if (action == BTHROW) bthrow();
	if (action == DTHROW) dthrow();
	if (action == UTHROW) uthrow();
	if (action == CHARGELEFT && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashleft();
	if (action == CHARGERIGHT && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashright();
	if (action == CHARGEUP && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashup();
	if (action == CHARGEDOWN && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashdown();
	if (hitstun > k.length*2) {
		aerial = true;
		hitstun--;
		if(lcancel > 0) lcancel--;
		dx = kx;
		dy = ky;
		if (hitstun == 0) {
			shieldbroken = false;
			fall();
		}
		if (checkCeilingCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
		}		
		if (checkFloorCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
			if(lcancel > 0) {
				tech();
				hitstun = 0;
			}
		}
		if(checkWallCollision()) {
			kx *= -1;
			dx = kx;
		}
		if(custom_action(ACTION_SHIELD, PAD_NEWPRESS) && !shieldbroken) lcancel = 10;
	}
	else if (hitstun > 0) {
		aerial = true;
		hitstun--;
		if(lcancel > 0) lcancel--;
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
			shieldbroken = false;
			fall();	
		}
		dx = kx;
		dy = ky;
		if (checkFloorCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
			if(lcancel > 0) {
				tech();
				hitstun = 0;
			}
		}
		if (checkCeilingCollision()) {
			if(ky > 0) ky = ky * -1 - gravity;
			dy = ky;
		}
		if(checkWallCollision()) {
			kx *= -1;
			dx = kx;
		}
		if(custom_action(ACTION_SHIELD, PAD_NEWPRESS) && !shieldbroken) lcancel = 10;
	}
	else {
		if (lasthitby != -1 && aerial == false) lasthitby = -1;
		if (tiltlag > 0) {
			tiltlag--;
			if(Pad.Released.Right || Pad.Released.Left) iswalking = true;
			if ((custom_action(ACTION_JUMP, PAD_RELEASED) || (Pad.Released.Up && getTapJumpOn()))) {
				shorthop();
				tiltlag = 0;
			}
			else if (tiltlag == 0) {
				bool AB = false;
				if (customcontrols[ACTION_SMASH] == BUTTON_AB) {
					if (custom_action(ACTION_SMASH, PAD_HELD)) {
						if (Pad.Held.Left) chargeleft();
						else if (Pad.Held.Right) chargeright();
						else if (Pad.Held.Up) chargeup();
						else if (Pad.Held.Down) chargedown();
						else jab();
						AB = true;
					}
				}
				if (customcontrols[ACTION_BASIC] == BUTTON_AB) {
					if (custom_action(ACTION_BASIC, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) ftilt();
						else if (Pad.Held.Up) utilt();
						else if (Pad.Held.Down) dtilt();
						else jab();
						AB = true;
					}
				}
				if (customcontrols[ACTION_JUMP] == BUTTON_AB) {
					if ((custom_action(ACTION_JUMP, PAD_HELD) || (Pad.Held.Up && getTapJumpOn())) && jumpcount < jumpmax) {
						jump();
						AB = true;
					}
				}
				if (customcontrols[ACTION_SPECIAL] == BUTTON_AB) {
					if (custom_action(ACTION_SPECIAL, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) bside();
						else if (Pad.Held.Up) bup();
						else if (Pad.Held.Down) bdown();
						else bneut();
						AB = true;
					}
				}
				if (customcontrols[ACTION_SHIELD] == BUTTON_AB) {
					if (custom_action(ACTION_SHIELD, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) roll();
						else shield();
						AB = true;
					}
				}
				if (customcontrols[ACTION_GRAB] == BUTTON_AB) {
					if (custom_action(ACTION_GRAB, PAD_NEWPRESS)) {
						grab();
						AB = true;
					}
				}
				// have to check the AB combo first otherwise it'll register as just A or just B
				if (!AB) {
					if (custom_action(ACTION_SMASH, PAD_HELD)) {
						if (Pad.Held.Left) chargeleft();
						else if (Pad.Held.Right) chargeright();
						else if (Pad.Held.Up) chargeup();
						else if (Pad.Held.Down) chargedown();
						else jab();
					}
					else if (custom_action(ACTION_BASIC, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) ftilt();
						else if (Pad.Held.Up) utilt();
						else if (Pad.Held.Down) dtilt();
						else jab();
					}
					else if (custom_action(ACTION_SPECIAL, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) bside();
						else if (Pad.Held.Up) bup();
						else if (Pad.Held.Down) bdown();
						else bneut();
					}
					else if ((custom_action(ACTION_JUMP, PAD_HELD) || (Pad.Held.Up && getTapJumpOn())) && jumpcount < jumpmax) jump();
					else if (custom_action(ACTION_SHIELD, PAD_HELD)) {
						if (Pad.Held.Right || Pad.Held.Left) roll();
						else shield();
					}
					else if (custom_action(ACTION_GRAB, PAD_HELD)) grab();
					else if ((Pad.Held.Right || Pad.Held.Left) && (iswalking || !getDoubleTapRunOn())) run();
					else if (Pad.Held.Right || Pad.Held.Left) walk();
					else if (Pad.Held.Down) crouch();
					else idle();
				}
			}
		}
		if (airlag > 0) {
			airlag--;
			if (checkFloorCollision()) idle();
			else if (airlag == 0) {
				bool AB = false;
				if (customcontrols[ACTION_BASIC] == BUTTON_AB) {
					if (custom_action(ACTION_BASIC, PAD_HELD) || custom_action(ACTION_BASIC, PAD_RELEASED)) {
						if (Pad.Held.Up) uair();
						else if (Pad.Held.Down) dair();
						else if ((Pad.Held.Left && direction == LEFT) || (Pad.Held.Right && direction == RIGHT)) fair();
						else if ((Pad.Held.Left && direction == RIGHT) || (Pad.Held.Right && direction == LEFT)) bair();
						else nair();
						AB = true;
					}
				}
				if (customcontrols[ACTION_JUMP] == BUTTON_AB) {
					if (((Pad.Held.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_HELD) || custom_action(ACTION_JUMP, PAD_RELEASED)) && jumpcount < jumpmax) {
						doubleJump();
						AB = true;
					}
				}
				if (customcontrols[ACTION_SPECIAL] == BUTTON_AB) {
					if (custom_action(ACTION_SPECIAL, PAD_HELD) || custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
						if (Pad.Held.Up) bup();
						else if (Pad.Held.Down) bdown();
						else if (Pad.Held.Left || Pad.Held.Right) bside();
						else bneut();
						AB = true;
					}
				}
				if (customcontrols[ACTION_SHIELD] == BUTTON_AB) {
					if (custom_action(ACTION_SHIELD, PAD_HELD) || custom_action(ACTION_SHIELD, PAD_RELEASED)) {
						if (airdodgecount < 1) {
							airdodge();
							airdodgecount++;
							AB = true;
						}
					}
				}
				if (!AB) {
					if (custom_action(ACTION_BASIC, PAD_HELD) || custom_action(ACTION_BASIC, PAD_RELEASED)) {
						if (Pad.Held.Up) uair();
						else if (Pad.Held.Down) dair();
						else if ((Pad.Held.Left && direction == LEFT) || (Pad.Held.Right && direction == RIGHT)) fair();
						else if ((Pad.Held.Left && direction == RIGHT) || (Pad.Held.Right && direction == LEFT)) bair();
						else nair();
					}
					else if (custom_action(ACTION_SPECIAL, PAD_HELD) || custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
						if (Pad.Held.Up) bup();
						else if (Pad.Held.Down) bdown();
						else if (Pad.Held.Left || Pad.Held.Right) bside();
						else bneut();
					}
					else if (custom_action(ACTION_SHIELD, PAD_HELD) || custom_action(ACTION_SHIELD, PAD_RELEASED)) {
						if (airdodgecount < 1) {
							airdodge();
							airdodgecount++;
						}
					}
					else if (((Pad.Held.Up && getTapJumpOn()) || (Pad.Released.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_HELD) || custom_action(ACTION_JUMP, PAD_RELEASED)) && jumpcount < jumpmax) doubleJump();
					else if (action != JUMP && action != DOUBLEJUMP) fall();
				}
			}
		}
		if (landinglag > 0) {
			landinglag--;
			if (landinglag == 0) idle();
		} // lags upon landing after an aerial
		if (delay > 0) delay--; // counts down the time before a new animation is set (allows for animations to finish)
		if (action == HANG) {
			hangtime++;
			if (custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
				attackUp();
			}
			else if (custom_action(ACTION_SHIELD, PAD_NEWPRESS)) {
				rollUp();
			}
			else if ((Pad.Newpress.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_NEWPRESS)) {
				jumpUp();
			}
			else if (Pad.Newpress.Down || hangtime >= 300) {
				hangtime = 0;
				ledgewait = 30;
				myledge = -1;
				jumpcount = 1;
				fall();
			}
			else if(Pad.Newpress.Right && direction == RIGHT) {
				y = y+hangy-bottomside;
				myledge = -1;
				x = x + hangx - leftside;
				idle();
			}
			else if(Pad.Newpress.Right && direction == LEFT) {
				hangtime = 0;
				ledgewait = 60;
				myledge = -1;
				jumpcount = 1;
				fall();
			}
			else if(Pad.Newpress.Left && direction == LEFT) {
				y = y+hangy-bottomside;
				myledge = -1;
				x = x + hangx - rightside;
				idle();
			}
			else if(Pad.Newpress.Left && direction == RIGHT) {
				hangtime = 0;
				ledgewait = 60;
				myledge = -1;
				jumpcount = 1;
				fall();
			}
		}
		if (action == DODGE && delay <= 0) shield();
		if (action == AIRDODGE && delay <= 0) fall();
		if (action == ROLL && delay <= 0) {
			dx = 0;
			if (direction == LEFT) {
				direction = RIGHT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			}
			else if (direction == RIGHT) {
				direction = LEFT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			}
			shield();
		}
		if (action == RELEASE || action == RELEASED) {
			if (delay <= 0) idle();
		}
		if (action == SLIDE && delay <= 0) {
			idle();
		}
		if (action == SLIDE) {
			if (dx > 0) {
				dx -= traction;
				if (dx <= 0) dx = 0;
			}
			else if (dx < 0) {
				dx += traction;
				if (dx >= 0) dx = 0;
			}
		}
		if (action == JAB) {
			if ((custom_action(ACTION_BASIC, PAD_RELEASED) && delay > 100) || delay <= 0) {
				idle();
				delay = 0;
			}
		}
		if (action == ATTACK) {
			if (delay <= 0) {
				chargecount = 0;
				if (Pad.Held.Down) crouch();
				else idle(); // idles when attack is done
			}
		}
		if (action == DASHATTACK) {
			x += dx;
			if (delay <= 0) idle();
		}
		if (action == BSIDE || action == BUP || action == BDOWN || action == BNEUT) {
			if (delay <= 0) idle();
		}
		if (action == AIRATTACK) {
			if (lcancel > 0) lcancel--;
			if (checkFloorCollision()) {
				if (delay > 0 && lcancel <= 0) land();
				else idle();
				delay = 0;
			}
			else if (custom_action(ACTION_SHIELD, PAD_RELEASED)) lcancel = 10;
			else if (delay <= 0) fall();
		} // checks for stage collision with aerial
		if ((action == JUMP || action == DOUBLEJUMP) && delay <= 0) fall(); // falls when jump/multijump are finished animating
		if ((action == DOUBLEJUMP || action == JUMP) && (custom_action(ACTION_BASIC, PAD_NEWPRESS) || (Stylus.Newpress && getCStickStylus()) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS))) {
			ymomentum = dy;
			momentumtime = delay;
			dy = 0;
			if (Stylus.Newpress && getCStickStylus()) airAttackStylus();
			else airlag = 2;
		}
		if (action == FALL) {
			if (!isCPU) directionalInfluence();
			if (PERMAFALL) {
				if (checkFloorCollision()) {
					PERMAFALL = false;
					delay = 10;
					land();
				}
			}
			else if (checkFloorCollision()) idle();
			if(dx > 0) {
				dx -= airresistance;
				if(dx < 0) dx = 0;
			}
			if(dx < 0) {
				dx += airresistance;
				if(dx > 0) dx = 0;
			}
		} // checks for stage collision when falling
		if (aerial && action != AIRATTACK && action != AIRLAG && action != FTHROW && action != BTHROW && action != UTHROW && action != DTHROW && !PERMAFALL) {
			actAir();
		} // acts in the air
		if (action == SHIELD) {
			shieldstr -= (65 - shieldstr) / 100;
			if (shieldstr <= 0) {
				shieldstr = 64;
				PA_FatPlaySfx("shieldbreak");
				hitstun = 300;
				shieldbroken = true;
				stun();
			}
			PA_SetRotsetNoAngle(MAIN_SCREEN, SPRITENUM - 100, (int)(2048 - 24*shieldstr), (int)(2048 - 24*shieldstr));
			if (!custom_action(ACTION_SHIELD, PAD_HELD)) idle();
			if (Pad.Newpress.Left || Pad.Newpress.Right) roll();
			if (Pad.Newpress.Down) dodge();
			if (custom_action(ACTION_BASIC, PAD_NEWPRESS) && getShieldGrabOn()) grab();
		}
		else if (shieldstr < 64) shieldstr += .1;
		if (action == GRAB) {
			if (delay <= 0) {
				if (custom_action(ACTION_SHIELD, PAD_HELD)) shield();
				else idle();
			}
		}
		if (action == GRABATK) {
			grabtimeleft--;
			if (delay <= 0) {
				grabbedenemy -> percentage += grabatkdamage;
				hold(grabtimeleft);
			}
		}
		if (action == HOLD) {
			if (delay <= 0) {
				if (direction == RIGHT) {
					release(LEFT);
					grabbedenemy -> released(RIGHT);
				}
				else if (direction == LEFT) {
					release(RIGHT);
					grabbedenemy -> released(LEFT);
				}
				grabbedenemy = NULL;
			}
			else if (custom_action(ACTION_BASIC, PAD_NEWPRESS)) {
				grabtimeleft = delay;
				grabattack();
			}
			else if (Pad.Newpress.Up) uthrow();
			else if (Pad.Newpress.Down) dthrow();
			else if ((direction == RIGHT && Pad.Newpress.Left) || (direction == LEFT && Pad.Newpress.Right)) bthrow();
			else if ((direction == RIGHT && Pad.Newpress.Right) || (direction == LEFT && Pad.Newpress.Left)) fthrow();
		}
		if (action == RUN || action == WALK) {
			if(aerial) fall();
			else if (Pad.Released.Right || Pad.Released.Left || (Pad.Newpress.Up) || custom_action(ACTION_JUMP, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
				tiltlag = 5;
				action = TILTLAG;
			}
			else if (custom_action(ACTION_BASIC, PAD_NEWPRESS)) {
				if(action == RUN) dashAttack();
				else if(action == WALK) {
					tiltlag = 5;
					action = TILTLAG;
				}
			}
			else if (Stylus.Newpress && getCStickStylus()) {
				smashAttack();
			}
			else if ((Pad.Held.Right || Pad.Held.Left) && action == RUN) run();
			else if ((Pad.Held.Right || Pad.Held.Left) && action == WALK) walk();
			else if (Pad.Released.Right || Pad.Released.Left) {
				slide();
			}
			else {
				idle();
			}
		}
		if (action == IDLE) {
			actGround();
			setDirection();
		}
		if (action == CROUCH) {
			if (!Pad.Held.Down) idle();
			if (custom_action(ACTION_BASIC, PAD_NEWPRESS)) dtilt();
			if (custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) bdown();
		}
	}
	move(); // moves
} // usually the acting method, but can be overwritten
void Fighter::actAir() {
	if (action == BUP || action == BDOWN || action == BSIDE || action == BNEUT) return;
	if (action == AIRDODGE) {
		if (checkFloorCollision()) {
			dx *= 3;
			slide();
		}
		return;
	}
	if ((custom_action(ACTION_BASIC, PAD_NEWPRESS) || (custom_action(ACTION_JUMP, PAD_NEWPRESS) && jumpmax <= 2) || (custom_action(ACTION_JUMP, PAD_HELD) && jumpmax > 2) || (Pad.Newpress.Up && getTapJumpOn() && jumpmax <= 2) || (Pad.Held.Up && getTapJumpOn() && jumpmax > 2) /*|| Pad.Held.Down || Pad.Held.Right || Pad.Held.Left */ || Pad.Newpress.B || Pad.Newpress.R || Pad.Newpress.L) && action != JUMP && action != DOUBLEJUMP) {
		airlag = 2;
		action = AIRLAG;
	}
	if ((custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS) || custom_action(ACTION_SHIELD, PAD_NEWPRESS)) && (action == JUMP || action == DOUBLEJUMP)) {
		airlag = 2;
		action = AIRLAG;
	}
	if ((custom_action(ACTION_JUMP, PAD_NEWPRESS)) && jumpcount < jumpmax && action != JUMP && action != DOUBLEJUMP) doubleJump(); // uses second (3rd, 4th, etc) jump(s)
	if (Stylus.Newpress && getCStickStylus()) airAttackStylus();
	if (!isCPU) directionalInfluence();
}
void Fighter::actGround() {
	if (tiltlag <= 0) {
		if (custom_action(ACTION_GRAB, PAD_NEWPRESS) || custom_action(ACTION_SHIELD, PAD_NEWPRESS) || Pad.Newpress.Right || Pad.Newpress.Left || Pad.Newpress.Down || Pad.Newpress.Up || Pad.Held.Right || Pad.Held.Left || Pad.Held.Down || Pad.Held.Up || custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS) || custom_action(ACTION_JUMP, PAD_NEWPRESS)) {
			action = TILTLAG;
			tiltlag = 5;
		}
		else if (custom_action(ACTION_SHIELD, PAD_HELD)) shield();
		else idle();
	}
	else idle();
} // acts on the ground based on key presses
void Fighter::cpu_dospecial() {}
double Fighter::cpu_specialweight() { return 0; }
void Fighter::bside() {}
void Fighter::bup() {}
void Fighter::bdown() {}
void Fighter::bneut() {}
void Fighter::uthrow() {}
void Fighter::dthrow() {}
void Fighter::bthrow() {}
void Fighter::fthrow() {}
void Fighter::release(int dir) {
	action = RELEASE;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	delay = 8;
	if (dir == RIGHT) dx = 2;
	else dx = -2;
}
void Fighter::released(int dir) {
	action = RELEASED;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	delay = 8;
	if (dir == RIGHT) dx = 2;
	else dx = -2;
}
void Fighter::grabbed(int otherx, int othery) {
	action = GRABBED;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	x = otherx;
	y = othery;
	CAPE = false;
	ABSORB = false;
	iswalking = false;
	aerial = false;
	if(MYCHAR == IKE && bottomside == 64) {
		bottomside = 47;
		y += 17;
	}
	dx = 0;
	dy = 0;
	playsound(GRABBED);
}
void Fighter::grab() {
	action = GRAB;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRAB], endframes[GRAB], framespeeds[GRAB], ANIM_LOOP, -1);
	setDirection();
	delay = 60 / framespeeds[GRAB] * (endframes[GRAB] - startframes[GRAB] + 1);
	playsound(GRAB);
}
void Fighter::hold(int d) {
	action = HOLD;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, endframes[GRAB], endframes[GRAB], 20, ANIM_LOOP, -1);
	delay = d;
}
void Fighter::grabattack() {
	action = GRABATK;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABATK], endframes[GRABATK], framespeeds[GRABATK], ANIM_LOOP, -1);
	delay = 60 / framespeeds[GRABATK] * (endframes[GRABATK] - startframes[GRABATK] + 1);
	playsound(GRABATK);
}
void Fighter::land() {
	action = LAND;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[LAND], endframes[LAND], framespeeds[LAND], ANIM_LOOP, -1);
	landinglag = delay;
	startlag = landinglag;
	playsound(LAND);
}
void Fighter::shield() {
	action = SHIELD;
	dx = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHIELD], endframes[SHIELD], framespeeds[SHIELD], ANIM_LOOP, -1);
	playsound(SHIELD);
}
void Fighter::roll(int dir) {
	action = ROLL;
	setDirection();
	if (dir == 0) {
		if (Pad.Held.Left) dir = LEFT;
		else if (Pad.Held.Right) dir = RIGHT;
	}
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[ROLL], endframes[ROLL], framespeeds[ROLL], ANIM_LOOP, -1);
	delay = 60 / framespeeds[ROLL] * (endframes[ROLL] - startframes[ROLL] + 1);
	if (dir == LEFT) dx = -rollspeed;
	if (dir == RIGHT) dx = rollspeed;
	playsound(ROLL);
}
void Fighter::tech() {
	aerial = false;
	dy = 0;
	action = ROLL;
	setDirection();
	if(Pad.Held.Left) dx = -rollspeed;
	if(Pad.Held.Right) dx = rollspeed;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[ROLL], endframes[ROLL], framespeeds[ROLL]*2, ANIM_LOOP, -1);
	delay = 30 / framespeeds[ROLL] * (endframes[ROLL] - startframes[ROLL] + 1);
	playsound(ROLL);
}
void Fighter::rollUp() {
	if (action == HANG) {
		myledge = -1;
		y = y + hangy - bottomside;
		if (direction == LEFT) {
			x = x + hangx - rightside;
			roll(LEFT);
		}
		else {
			x = x + hangx - leftside;
			roll(RIGHT);
		}
	}
}
void Fighter::attackUp() {
	if (action == HANG) {
		myledge = -1;
		y = y + hangy - bottomside;
		if (direction == LEFT) x = x + hangx - rightside;
		else x = x + hangx - leftside;
		ftilt();
	}
}
void Fighter::jumpUp() {
	if (action == HANG) {
		myledge = -1;
		y = y + hangy - bottomside;
		jump();
	}
}
void Fighter::dodge() {
	action = DODGE;
	setDirection();
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DODGE], endframes[DODGE], framespeeds[DODGE], ANIM_LOOP, -1);
	delay = 60 / framespeeds[DODGE] * (endframes[DODGE] - startframes[DODGE] + 1);
	playsound(DODGE);
}
void Fighter::airdodge() {
	action = AIRDODGE;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[AIRDODGE], endframes[AIRDODGE], framespeeds[AIRDODGE], ANIM_LOOP, -1);
	delay = 60 / framespeeds[AIRDODGE] * (endframes[AIRDODGE] - startframes[AIRDODGE] + 1);
	if (Pad.Held.Up) dy = -gravity - 2;
	else if (Pad.Held.Down) dy = -gravity + 2;
	if (Pad.Held.Right) dx = 2;
	else if (Pad.Held.Left) dx = -2;
	if (!Pad.Held.Up && !Pad.Held.Down && !Pad.Held.Right && !Pad.Held.Left) {
		dx = 0;
		dy = -gravity;
	}
	if (!Pad.Held.Up && !Pad.Held.Down) dy = -gravity;
	fastfall = 0;
	DI = 0;
	playsound(AIRDODGE);
}
void Fighter::crouch() {
	action = CROUCH;
	dx = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CROUCH], endframes[CROUCH], framespeeds[CROUCH], ANIM_LOOP, -1);
	playsound(CROUCH);
}
void Fighter::fall() {
	aerial = true;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FALL], endframes[FALL], framespeeds[FALL], ANIM_LOOP, -1);
	if (!isCPU) directionalInfluence();
	dy = 0;
	action = FALL;
	playsound(FALL);
}
void Fighter::permafall() {
	PERMAFALL = true;
	fall();
}
void Fighter::idle() {
	if (action != IDLE) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[IDLE], endframes[IDLE], framespeeds[IDLE], ANIM_LOOP, -1);
	dx = 0;
	dy = 0;
	iswalking = false;
	action = IDLE;
	playsound(IDLE);
}
void Fighter::walk(int d) {
	if(action != WALK) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[RUN], endframes[RUN], framespeeds[RUN], ANIM_LOOP, -1);
	if(d == 0) {
		if(Pad.Held.Left) dx = -walkspeed;
		if(Pad.Held.Right) dx = walkspeed;
		setDirection();
	}
	else {
		dx = walkspeed * d;
		if(d > 0) setDirection(RIGHT);
		if(d < 0) setDirection(LEFT);
	}
	iswalking = true;
	action = WALK;
	playsound(WALK);
}
void Fighter::run(int d) {
	if (action != RUN) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[RUN], endframes[RUN], framespeeds[RUN], ANIM_LOOP, -1);
	if (d == 0) {
		if (Pad.Held.Left) dx = -runspeed;
		if (Pad.Held.Right) dx = runspeed;
		setDirection();
	}
	else {
		dx = runspeed * d;
		if (d > 0) setDirection(RIGHT);
		if (d < 0) setDirection(LEFT);
	}
	iswalking = false;
	action = RUN;
	playsound(RUN);
}
void Fighter::shorthop() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHORTHOP], endframes[SHORTHOP], framespeeds[SHORTHOP], ANIM_LOOP, -1);
	delay = 60 / framespeeds[SHORTHOP] * (endframes[SHORTHOP] - startframes[SHORTHOP] + 1);
	double jumpspeed = jumpheight/delay;
	dy = -jumpspeed / 2;
	fastfall = 0;
	action = JUMP;
	aerial = true;
	jumpcount++;
	playsound(SHORTHOP);
}
void Fighter::jump() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JUMP], endframes[JUMP], framespeeds[JUMP], ANIM_LOOP, -1);
	delay = 60 / framespeeds[JUMP] * (endframes[JUMP] - startframes[JUMP] + 1);
	double jumpspeed = jumpheight/delay;
	dy = -jumpspeed;
	fastfall = 0;
	action = JUMP;
	aerial = true;
	jumpcount++;
	playsound(JUMP);
}
void Fighter::doubleJump() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DOUBLEJUMP], endframes[DOUBLEJUMP], framespeeds[DOUBLEJUMP], ANIM_LOOP, -1);
	delay = 60 / framespeeds[DOUBLEJUMP] * (endframes[DOUBLEJUMP] - startframes[DOUBLEJUMP] + 1);
	double doublejumpspeed = doublejumpheight/delay;
	action = DOUBLEJUMP;
	dy = -doublejumpspeed - .5*(jumpmax - jumpcount - 1) - .5; // only affects characters w/ >2 jumps
	fastfall = 0;
	jumpcount++;
	aerial = true;
	setDirection();
	if (effectwait <= 0) {
		display -> addeffect(Effect(x, y + 32, FX_AIRJUMP, charnum, display->scrollx, display->scrolly));
		effectwait = 15;
	}
	playsound(DOUBLEJUMP);
}
void Fighter::jab() {
	action = JAB;
	dx = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JAB], endframes[JAB], framespeeds[JAB], ANIM_LOOP, -1);
	delay = 60 / framespeeds[JAB] * (endframes[JAB] - startframes[JAB] + 1);
	playsound(JAB);
}
void Fighter::dashAttack() {
	if (direction == LEFT) dx = -1.5;
	if (direction == RIGHT) dx = 1.5;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DASHATTACK], endframes[DASHATTACK], framespeeds[DASHATTACK], ANIM_LOOP, -1);
	delay = 60 / framespeeds[DASHATTACK] * (endframes[DASHATTACK] - startframes[DASHATTACK] + 1);
	action = DASHATTACK;
	playsound(DASHATTACK);
}
void Fighter::ftilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FTILT], endframes[FTILT], framespeeds[FTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60 / framespeeds[FTILT] * (endframes[FTILT] - startframes[FTILT] + 1);
	dx = 0;
	setDirection();
	playsound(FTILT);
}
void Fighter::utilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UTILT], endframes[UTILT], framespeeds[UTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60 / framespeeds[UTILT] * (endframes[UTILT] - startframes[UTILT] + 1);
	dx = 0;
	playsound(UTILT);
}
void Fighter::dtilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DTILT], endframes[DTILT], framespeeds[DTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60 / framespeeds[DTILT] * (endframes[DTILT] - startframes[DTILT] + 1);
	dx = 0;
	playsound(DTILT);
}
void Fighter::chargeleft() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGELEFT], endframes[CHARGELEFT], framespeeds[CHARGELEFT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	action = CHARGELEFT;
	direction = LEFT;
	dx = 0;
	delay = 60 / framespeeds[CHARGELEFT] * (endframes[CHARGELEFT] - startframes[CHARGELEFT] + 1) * 15;
	playsound(CHARGELEFT);
}
void Fighter::chargeright() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGERIGHT], endframes[CHARGERIGHT], framespeeds[CHARGERIGHT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	action = CHARGERIGHT;
	direction = RIGHT;
	dx = 0;
	delay = 60 / framespeeds[CHARGERIGHT] * (endframes[CHARGERIGHT] - startframes[CHARGERIGHT] + 1) * 15;
	playsound(CHARGERIGHT);
}
void Fighter::chargeup() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEUP], endframes[CHARGEUP], framespeeds[CHARGEUP], ANIM_LOOP, -1);
	action = CHARGEUP;
	dx = 0;
	delay = 60 / framespeeds[CHARGEUP] * (endframes[CHARGEUP] - startframes[CHARGEUP] + 1) * 15;
	playsound(CHARGEUP);
}
void Fighter::chargedown() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEDOWN], endframes[CHARGEDOWN], framespeeds[CHARGEDOWN], ANIM_LOOP, -1);
	action = CHARGEDOWN;
	dx = 0;
	delay = 60 / framespeeds[CHARGEDOWN] * (endframes[CHARGEDOWN] - startframes[CHARGEDOWN] + 1) * 15;
	playsound(CHARGEDOWN);
}
void Fighter::smashleft() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHLEFT], endframes[SMASHLEFT], framespeeds[SMASHLEFT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	action = ATTACK;
	direction = LEFT;
	delay = 60 / framespeeds[SMASHLEFT] * (endframes[SMASHLEFT] - startframes[SMASHLEFT] + 1);
	playsound(SMASHLEFT);
}
void Fighter::smashright() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHRIGHT], endframes[SMASHRIGHT], framespeeds[SMASHRIGHT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	action = ATTACK;
	direction = RIGHT;
	delay = 60 / framespeeds[SMASHRIGHT] * (endframes[SMASHRIGHT] - startframes[SMASHRIGHT] + 1);
	playsound(SMASHRIGHT);
}
void Fighter::smashup() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHUP], endframes[SMASHUP], framespeeds[SMASHUP], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60 / framespeeds[SMASHUP] * (endframes[SMASHUP] - startframes[SMASHUP] + 1);
	playsound(SMASHUP);
}
void Fighter::smashdown() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHDOWN], endframes[SMASHDOWN], framespeeds[SMASHDOWN], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60 / framespeeds[SMASHDOWN] * (endframes[SMASHDOWN] - startframes[SMASHDOWN] + 1);
	playsound(SMASHDOWN);
}
void Fighter::fair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FAIR], endframes[FAIR], framespeeds[FAIR], ANIM_LOOP, -1);
	delay = 60 / framespeeds[FAIR] * (endframes[FAIR] - startframes[FAIR] + 1);
	playsound(FAIR);
}
void Fighter::bair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[BAIR], endframes[BAIR], framespeeds[BAIR], ANIM_LOOP, -1);
	delay = 60 / framespeeds[BAIR] * (endframes[BAIR] - startframes[BAIR] + 1);
	if (Pad.Held.Left) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	if (Pad.Held.Right) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	playsound(BAIR);
}
void Fighter::uair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UAIR], endframes[UAIR], framespeeds[UAIR], ANIM_LOOP, -1);
	delay = 60 / framespeeds[UAIR] * (endframes[UAIR] - startframes[UAIR] + 1);
	playsound(UAIR);
}
void Fighter::dair() {
	fastfall = 0;
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DAIR], endframes[DAIR], framespeeds[DAIR], ANIM_LOOP, -1);
	delay = 60 / framespeeds[DAIR] * (endframes[DAIR] - startframes[DAIR] + 1);
	playsound(DAIR);
}
void Fighter::nair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[NAIR], endframes[NAIR], framespeeds[NAIR], ANIM_LOOP, -1);
	delay = 60 / framespeeds[NAIR] * (endframes[NAIR] - startframes[NAIR] + 1);
	playsound(NAIR);
}
void Fighter::stun() {
	myledge = -1;
	action = STUN;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[STUN], endframes[STUN], framespeeds[STUN], ANIM_LOOP, -1);
	playsound(STUN);
}
void Fighter::freeze(int duration) {
	tempanim = spriteanims[MAIN_SCREEN][SPRITENUM];
	freezelen = duration;
	invincibility = duration;
}
void Fighter::slide() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SLIDE], endframes[SLIDE], framespeeds[SLIDE], ANIM_LOOP, -1);
	action = SLIDE;
	delay = 5;
	playsound(SLIDE);
}
void Fighter::hang() {
	invincibility = 60;
	PA_SetSpriteVflip(MAIN_SCREEN, SPRITENUM, 0);
	lasthitby = -1;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[HANG], endframes[HANG], framespeeds[HANG], ANIM_LOOP, -1);
	action = HANG;
	hangtime = 0;
}
// Sound playing
void Fighter::initSounds() {}
void Fighter::playsound(int sndnum) {}
// constant methods
void Fighter::setStage(Stage *s) {
	stage = s;
	x = stage->spawnx[charnum];
	y = stage->spawny[charnum];
	PA_SetSpriteX(MAIN_SCREEN, SPRITENUM, -64);
	startx = x;
	starty = y;
	if (x < stage->width / 2) setDirection(RIGHT);
	else setDirection(LEFT);
}
int Fighter::getHitstun() {
	return hitstun;
}
double Fighter::getDamagePercent() {
	return percentage;
}
void Fighter::takeDamage(Circle other, int mult, int hitter, int charge) {
	if(players[hitter] -> MYCHAR == PIKACHU && PA_GetSpriteAnimFrame(MAIN_SCREEN, players[hitter] -> SPRITENUM) == 64) {
		freeze(15);
		action = STUN;
		myledge = -1;
		playsound(STUN);
		players[hitter] -> freeze(15);
	}
	else if (action != STUN && other.getKnockback().length != 0) stun();
	PERMAFALL = false;
	
	int chargedamage = (int)((charge/225) * (.5*other.damage));
	percentage += (double)(other.damage + chargedamage);

	if (effectwait <= 0) {
		if (other.fx == FX_NONE) {
			if (other.damage + (int)((charge / 225) *(.5*other.damage)) <= 6 / 3) {
				PA_FatPlaySfx("hit1");
				display -> addeffect(Effect(x, y, FX_WEAKERHIT, charnum, display->scrollx, display->scrolly));
			}
			else if (other.damage + (int)((charge / 225) *(.5*other.damage)) > 6 / 3 && other.damage + (int)((charge / 225) *(.5*other.damage)) <= 12 / 3) {
				PA_FatPlaySfx("hit2");
				display -> addeffect(Effect(x, y, FX_WEAKHIT, charnum, display->scrollx, display->scrolly));
			}
			else {
				PA_FatPlaySfx("hit3");
				display -> addeffect(Effect(x, y, FX_STRONGHIT, charnum, display->scrollx, display->scrolly));
			}
		}
		else {
			display -> addeffect(Effect(x, y, other.fx, charnum, display->scrollx, display->scrolly));
			// play sound based on effect
		}
		effectwait = 15;
	}

	if(other.getKnockback().length != 0) {
		k = other.getKnockback();
		hitstun = (int)((k.length*2 + percentage/4.0)/w1);
		if(k.length == 0) hitstun = 0;
		kx = (k.dx * mult * (1 + percentage/200.0))/w1;
		ky = ((k.dy - gravity) * (1 + percentage/200.0))/w1;
	}

	if(players[hitter] -> MYCHAR == FOX && PA_GetSpriteAnimFrame(MAIN_SCREEN, players[hitter] -> SPRITENUM) == 114) {
		hitstun = (int)(k.length*2);
		kx = k.dx * mult;
		if(aerial) ky = k.dy - gravity;
		else ky = 0;
	} // shined

	jumpcount = 1;
	dx = dy = DI = fastfall = 0;
	if(MYCHAR == IKE && bottomside == 64) {
		bottomside = 47;
		y += 17;
	}
	CAPE = false;
	ABSORB = false;
	iswalking = false;
	shieldbroken = false;
	lasthitby = hitter;
}
Fighter* Fighter::checkHits(Fighter* other) {
	if (!allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)].enabled) return other;
	if (other -> respawntimer > 0) return other;
	if (other -> invincibility > 0) return other;
	if(getAtkbox().hits(other -> getAtkbox())) {
		if(getAtkbox().getHitCircle(other -> getAtkbox()).priority > other -> getAtkbox().getHitCircle(getAtkbox()).priority) {
			if (action == HOLD || action == GRABATK) {}
			else if (action == GRAB) {
				if (direction == RIGHT) other -> grabbed((int)(x + handx), (int)y);
				else other -> grabbed((int)(x - handx), (int)y);
				other -> grabbedby = this;
				grabbedenemy = other;
				hold();
			}
			else if (other -> action == AIRDODGE || other -> action == ROLL || other -> action == DODGE) { /*doesn't hit*/
			}
			else if (other -> action == SHIELD) {
				other -> shieldstr -= getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage + (int)((chargecount / 225) * (.5 * getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage));
				if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
			}
			else if (other -> COUNTER) {
				if (direction == LEFT) takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, other -> charnum, chargecount);
				else takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, other -> charnum, chargecount);
				other -> COUNTER = false;
				other -> idle();
				if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
			}
			else {
				if (direction == LEFT) other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, charnum, chargecount);
				else other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, charnum, chargecount);
				if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
			}		
		} // I win priority
		else if(getAtkbox().getHitCircle(other -> getAtkbox()).priority == other -> getAtkbox().getHitCircle(other -> getAtkbox()).priority) {
			PA_FatPlaySfx("clash");
			if(direction == RIGHT) dx = -2;
			else dx = 2;
			slide();
			if(other -> MYCHAR != KIRBY || PA_GetSpriteAnimFrame(MAIN_SCREEN, other->SPRITENUM) != 189) {
				if(other -> direction == RIGHT) other -> dx = -2;
				else other -> dx = 2;
				other -> slide();
			}
		} // clashing hits
		else return other; // they win priority in their checkHits()
	}
	else if (getAtkbox().hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
		if (action == HOLD || action == GRABATK) {}
		else if (action == GRAB) {
			if (direction == RIGHT) other -> grabbed((int)(x + handx), (int)y);
			else other -> grabbed((int)(x - handx), (int)y);
			other -> grabbedby = this;
			grabbedenemy = other;
			hold();
		}
		else if(other -> MYCHAR == KIRBY && PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM) == 189) {
			PA_FatPlaySfx("clash");
			if(direction == RIGHT) dx = -2;
			else dx = 2;
			slide();
		} // Kirby's rock stops attacks!
		else if (other -> action == AIRDODGE || other -> action == ROLL || other -> action == DODGE) { /*doesn't hit*/
		}
		else if (other -> action == SHIELD) {
			other -> shieldstr -= (int)((getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage + (int)((chargecount / 225) * (.5 * getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage)))/3);
			if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
		}
		else if (other -> COUNTER) {
			if (direction == LEFT) takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, other -> charnum, chargecount);
			else takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, other -> charnum, chargecount);
			other -> COUNTER = false;
			other -> idle();
			if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
		}
		else {
			if (direction == LEFT) other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, charnum, chargecount);
			else other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, charnum, chargecount);
			if(action == BSIDE && (MYCHAR == PIKACHU || MYCHAR == IKE)) idle();
		}
	}
	return other;
}
Hitbox Fighter::getAtkbox() {
	Hitbox temp = allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)];
	Hitbox atkbox;
	if (action == LAND || action == SHIELD || action == ROLL || action == DODGE || action == AIRDODGE ||
	        action == CROUCH || action == FALL || action == IDLE || action == RUN || action == WALK || action == SHORTHOP ||
	        action == JUMP || action == DOUBLEJUMP || action == CHARGELEFT || action == CHARGERIGHT ||
	        action == CHARGEUP || action == CHARGEDOWN || action == STUN || action == SLIDE || action == HANG ||
	        action == GRABBED || action == GRABATK || action == FTHROW || action == BTHROW || action == UTHROW ||
	        action == DTHROW || action == DEAD)
		return atkbox; // these actions don't have hitboxes; allows for reusing of sprites for attacking moves and other moves.
	vector<Circle> circles = temp.getCircles();
	for (uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircleft(current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		Circle newcircright((64 - current.getX()) + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		if (direction == LEFT) atkbox.addCircle(newcircleft);
		else atkbox.addCircle(newcircright);
	}
	return atkbox;
}
Hitbox Fighter::getDefbox(int framenum) {
	Hitbox temp = alldefbox[framenum];
	Hitbox defbox;
	vector<Circle> circles = temp.getCircles();
	for (uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircright(current.getX() + x, current.getY() + y, current.getRadius());
		Circle newcircleft(x + (64 - current.getX()), y + (current.getY()), current.getRadius());
		if (direction == LEFT) defbox.addCircle(newcircleft);
		else defbox.addCircle(newcircright);
	}
	return defbox;
}
Circle Fighter::createAtkbox(double extrax, double extray, double radius, Knockback k, double damage, int prior) {
	Circle temp(64 - extrax,  extray, radius, Knockback(k.dx/2.0, k.dy/2.0, k.length*2.0), damage, prior);
	return temp;
}
void Fighter::airAttack() {
	if (Pad.Held.Up) uair();
	else if (Pad.Held.Down) dair();
	else if ((Pad.Held.Right && direction == RIGHT) || (Pad.Held.Left && direction == LEFT)) fair();
	else if ((Pad.Held.Right && direction == LEFT) || (Pad.Held.Left && direction == RIGHT)) bair();
	else nair();
}
void Fighter::airAttackStylus() {
	int touchx = Stylus.X;
	int touchy = Stylus.Y;
	if (touchy < 64 && touchx > 96 && touchx < 160) uair();
	else if (touchy > 128 && touchx > 96 && touchx < 160) dair();
	else if ((touchx > 192 && touchy < 128 && touchy > 64 && direction == RIGHT) || (touchx < 64 && touchy < 128 && touchy > 64 && direction == LEFT)) fair();
	else if ((touchx > 192 && touchy < 128 && touchy > 64 && direction == LEFT) || (touchx < 64 && touchy < 128 && touchy > 64 && direction == RIGHT)) bair();
	else nair();
}
void Fighter::smashAttack() {
	int touchx = Stylus.X; // xpos of touch
	int touchy = Stylus.Y; // ypos of touch
	if (touchx < 64 && touchy < 128 && touchy > 64) smashleft();
	if (touchx > 192 && touchy < 128 && touchy > 64) smashright();
	if (touchy < 64 && touchx > 96 && touchx < 160) smashup();
	if (touchy > 128 && touchx > 96 && touchx < 160) smashdown();
}
void Fighter::move() {
	if (action == HANG) return;
	if (action == GRABBED) {}
	else if (!aerial) {
		if (!checkFloorCollision()) {
			fall();
			jumpcount++;
		}
	}
	else {
		if(dx > 0) {
			dx -= airresistance;
			if(dx < 0) dx = 0;
		}
		if(dx < 0) {
			dx += airresistance;
			if(dx > 0) dx = 0;
		}
		if (!isCPU) directionalInfluence();
		if (action == SHORTHOP || action == JUMP || action == DOUBLEJUMP) fastfall = 0;
		if (action == AIRDODGE || action == FTHROW || action == DTHROW || action == UTHROW || action == BTHROW || action == STUN) ymomentum = DI = fastfall = 0;
		if (MYCHAR == FOX && (action == BUP || action == BSIDE || action == BDOWN)) DI = fastfall = ymomentum = 0;
		if (MYCHAR == MEWTWO && action == BUP) DI = fastfall = ymomentum = 0;
		if (MYCHAR == PIKACHU && (action == BUP || action == BSIDE || action == BDOWN)) DI = fastfall = ymomentum = 0;
		if(action != STUN) {
			checkLedgeCollision();
			checkFloorCollision();
			checkWallCollision();
			checkCeilingCollision();
		}
	}
	lastx = x;
	lasty = y;
	x += dx;
	y += dy;
	if(x > 640 || x < -128 || y < -192) percentage += (1/60.0);
	if (momentumtime > 0) {
		momentumtime--;
		y += ymomentum;
		if (momentumtime == 0) {
			ymomentum = 0;
		}
	}
	if (action == ATTACK) jaywalk();
	if (aerial) y += gravity;
	if (aerial) y += fastfall;
	if (aerial) x += DI;
	if (checkForDeath()) respawn();
	PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x, (int)y); // repositions the sprite
} // moves the sprite
void Fighter::jaywalk() {}
void Fighter::setDirection(int rl) {
	if(action == HANG) return;
	if (action == STUN) {
		if (kx > 0) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			direction = RIGHT;
		}
		else if (kx < 0) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			direction = LEFT;
		}
	}
	if (rl == 0) {
		if (Pad.Held.Right) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			direction = RIGHT;
		}
		if (Pad.Held.Left) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			direction = LEFT;
		}
	}
	else {
		direction = rl;
		if (direction == RIGHT) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
		if (direction == LEFT) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	}
} // flips the direction of the sprite if necessary
void Fighter::directionalInfluence(double deltax) {
	if (deltax != 0) {
		DI = deltax;
		fastfall = 0;
	}
	else if ((MYCHAR == MEWTWO && action == BUP) || (MYCHAR == IKE && action == BSIDE) || (MYCHAR == KIRBY && action == BDOWN)) {
		DI = 0;
		fastfall = 0;
	}
	else {
		if (Pad.Held.Right) DI = DIval;
		if (Pad.Held.Left) DI = -DIval;
		if (Pad.Newpress.Down && airlag == 0) fastfall = 2;
		if (!Pad.Held.Right && !Pad.Held.Left || action == BDOWN) DI = 0;
		// slightly influences direction
	}
} // acts in the air based on key presses
bool Fighter::checkForDeath() {
	if (x > stage->rightdeath || x + 64 < stage->leftdeath || y > stage->bottomdeath || y + 64 < stage->topdeath) {
		display->score->addDeath(lasthitby, charnum);
		int deathx = -64, deathy = -64;
		int deathrot = -1;
		if (x > stage->rightdeath) {
			deathx = 256 - 64 + (int)display->scrollx + 10;
			deathy = (int)y;
			deathrot = 0;
		} // died off of right side
		else if (x + 64 < stage->leftdeath) {
			deathx = 0 + (int)display->scrollx - 10;
			deathy = (int)y;
			deathrot = 180;
		} // died off of left side
		else if (y > stage->bottomdeath) {
			deathx = (int)x;
			deathy = 192 - 64 + (int)display->scrolly + 10;
			deathrot = 90;
		} // died off of bottom
		else if (y + 64 < stage->topdeath) {
			deathx = (int)x;
			deathy = 0 + (int)display->scrolly - 10;
			deathrot = 270;
		} // died off top
		display -> addeffect(Effect(deathx, deathy, FX_DEATH, charnum, display->scrollx, display->scrolly, deathrot));
		PA_FatPlaySfx("death");
		return true;
	}
	return false;
}
void Fighter::respawn() {
	if (respawntimer == 0) {
		lasthitby = -1;
		x = startx;
		y = starty;
		if (x < stage->width / 2) setDirection(RIGHT);
		else setDirection(LEFT);
		respawntimer = 600;
		delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
		dx = dy = fastfall = DI = 0.0;
		percentage = 0.0;
		shieldstr = 64;
		PA_SetSpriteXY(MAIN_SCREEN, 55 + (SPRITENUM - 100), (int)(PA_GetSpriteX(MAIN_SCREEN, SPRITENUM) - 32 + (rightside + leftside) / 2), (int)(PA_GetSpriteY(MAIN_SCREEN, SPRITENUM) + bottomside));
		idle();
	}
	else {
		PA_SetSpriteXY(MAIN_SCREEN, 55 + (SPRITENUM - 100), (int)(PA_GetSpriteX(MAIN_SCREEN, SPRITENUM) - 32 + (rightside + leftside) / 2), (int)(PA_GetSpriteY(MAIN_SCREEN, SPRITENUM) + bottomside));
		respawntimer--;
		if (MYCHAR == SANDBAG || respawntimer == 0 || ((Pad.Newpress.Left || Pad.Newpress.Right || Pad.Newpress.Down) && !isCPU) || (isCPU && PA_RandMax(100) > 97)) {
			respawntimer = 0;
			PA_SetSpriteXY(MAIN_SCREEN, 55 + (SPRITENUM - 100), -64, -64);
			invincibility = 180;
			fall();
		}
		else if (!isCPU && (custom_action(ACTION_JUMP, PAD_NEWPRESS) || (Pad.Newpress.Up && getTapJumpOn()))) {
			respawntimer = 0;
			PA_SetSpriteXY(MAIN_SCREEN, 55+(SPRITENUM-100), -64, -64);
			invincibility = 180;
			jump();
		}
		else if(!isCPU && custom_action(ACTION_SHIELD, PAD_NEWPRESS)) {
			respawntimer = 0;
			PA_SetSpriteXY(MAIN_SCREEN, 55+(SPRITENUM-100), -64, -64);
			invincibility = 180;
			airdodge();
		}
		else if(!isCPU && custom_action(ACTION_BASIC, PAD_NEWPRESS)) {
			respawntimer = 0;
			PA_SetSpriteXY(MAIN_SCREEN, 55+(SPRITENUM-100), -64, -64);
			invincibility = 180;
			nair();
		}
		else if(!isCPU && custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
			respawntimer = 0;
			PA_SetSpriteXY(MAIN_SCREEN, 55+(SPRITENUM-100), -64, -64);
			invincibility = 180;
			bneut();
		}
	}
}
void Fighter::beDead() {
	PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, -64, -64);
	PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM + 30, -64, -64);
	PA_SetSpriteXY(SUB_SCREEN, charnum + 1, -64, -64);
	percentage = 0.0;
	x = 10000;
	y = 10000;
	isdead = true;
}
bool Fighter::ledgenotinuse(int lnum) {
	for (int n = 0; n < (int)players.size(); n++) {
		if (players[n] -> myledge == lnum) return false;
	}
	return true;
}
bool Fighter::checkLedgeCollision() {
	if(MYCHAR == SANDBAG) return false;
	vector<Ledge> ledges = stage->getLedges();
	for (int n = 0; n < (int)ledges.size(); n++) {
		Ledge currledge = ledges[n];
		if (action != STUN && action != HANG) {
			if (currledge.direction == "right") {
				if (ledgenotinuse(n) && ledgewait <= 0 && x + leftside > currledge.x - 20 && x + leftside < currledge.x + 20 && y > currledge.y - 20 && y < currledge.y + 20) {
					aerial = false;
					dx = DI = dy = fastfall = ymomentum = 0;
					airdodgecount = jumpcount = 0;
					x = currledge.x - hangx;
					y = currledge.y - hangy;
					setDirection(LEFT);
					hang();
					myledge = n;
					return true;
				}
			}
			else {
				if (ledgenotinuse(n) && ledgewait <= 0 && x + rightside > currledge.x - 20 && x + rightside < currledge.x + 20 && y > currledge.y - 20 && y < currledge.y + 20) {
					aerial = false;
					dx = DI = dy = fastfall = ymomentum = 0;
					airdodgecount = jumpcount = 0;
					x = currledge.x + hangx - 64;
					y = currledge.y - hangy;
					setDirection(RIGHT);
					hang();
					myledge = n;
					return true;
				}
			}
		}
	}
	return false;
}
bool Fighter::checkFloorCollision() {
	vector<Floor> floors = stage->getFloors();
	for (uint8 n = 0; n < floors.size(); n++) {
		Floor currfloor = floors[n];
		double rise;
		if (currfloor.slopes.size() == 0) rise = 0;
		else rise = currfloor.getrise((int)(x));
		if (aerial) {
			if ((isCPU || (!(Pad.Held.Down && currfloor.isPlatform() && !isCPU))) && x + rightside + dx + DI > currfloor.x && x + leftside + dx + DI < currfloor.x + currfloor.length) {
				if(!((MYCHAR == FOX || MYCHAR == MEWTWO || MYCHAR == PIKACHU) && action == BUP && currfloor.isPlatform())) {
					if(y + bottomside <= currfloor.y && y + bottomside + gravity + fastfall + dy + ymomentum > currfloor.y) {
						if(currfloor.totalrise() == 0) {
							aerial = false;
							y = currfloor.y - bottomside;
							dy = DI = fastfall = ymomentum = 0;
							airdodgecount = 0;
							jumpcount = 0;
							return true;
						}
						else {
							aerial = false;
							y = currfloor.y-bottomside+rise;
							dy = DI = fastfall = ymomentum = 0;
							airdodgecount = 0;
							jumpcount = 0;
							return true;
						}
					}
				} // ignores platforms during BUPs
			} // if you land on the floor
		} // checks for landing
		else {
			if(action == FTHROW || action == BTHROW || action == UTHROW || action == DTHROW || action == GRABBED) return true;
			if (isCPU || (!(Pad.Held.Down && currfloor.isPlatform() && (action == CROUCH || action == IDLE)) && !isCPU)) {
				if (x + rightside + dx > currfloor.x && x + leftside + dx < currfloor.x + currfloor.length) {
					if(currfloor.totalrise() == 0) {
						if(y + bottomside == currfloor.y) {	
							y = currfloor.y - bottomside;
							return true;
						} // stays on the ledge
					}
					else {
						if(y + bottomside == currfloor.y+currfloor.getrise((int)lastx)) {
							y = currfloor.y - bottomside + rise;
							return true;
						}
					}
				}
			}
			if((MYCHAR == FOX || MYCHAR == PIKACHU) && action == BSIDE) {
				aerial = true;
				return true;
			}
		} // checks for falling off
	}
	return false;
}
bool Fighter::checkWallCollision() {
	vector<Wall> walls = stage->getWalls();
	for (uint8 n = 0; n < walls.size(); n++) {
		Wall currwall = walls[n];
		if (currwall.direction == "left") {
			if (x + rightside <= currwall.x && x + rightside + DI + dx > currwall.x && y + bottomside > currwall.y + topside && y < currwall.y + currwall.length) {
				x = currwall.x - rightside;
				dx = DI = 0;
				return true;
			}
		}
		else {
			if (x + leftside >= currwall.x && x + leftside + DI + dx < currwall.x && y + bottomside > currwall.y + topside && y < currwall.y + currwall.length) {
				x = currwall.x - leftside;
				dx = DI = 0;
				return true;
			}
		}
	}
	return false;
}
bool Fighter::checkCeilingCollision() {
	vector<Ceiling> ceilings = stage->getCeilings();
	for (uint8 n = 0; n < ceilings.size(); n++) {
		Ceiling currceil = ceilings[n];
		if (y + topside >= currceil.y && y + topside + gravity + fastfall + dy + ymomentum < currceil.y && x + rightside + dx + DI > currceil.x && x + leftside + dx + DI < currceil.x + currceil.length) {
			y = currceil.y - topside - dy - ymomentum;
			return true;
		}
	}
	return false;
}
void Fighter::scroll(double scrollx, double scrolly) {
	if (x - scrollx > 256 || x - scrollx < 0 - 64 || y - scrolly > 192 || y - scrolly < 0 - 64) {
		PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, -64, -64);
		PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM - 4, -64, -64);
	}
	else {
		PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x - (int)(scrollx), (int)y - (int)(scrolly));
		if (action == SHIELD) {
			if(direction == RIGHT) PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM - 4, (int)x - (int)(scrollx) + shieldx-32, (int)y - (int)(scrolly) + shieldy-32);
			else PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM - 4, (int)x - (int)scrollx + 64-shieldx-32, (int)y - (int)scrolly + shieldy-32);
		}
		else PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM - 4, -64, -64);
	}
}
Fighter::~Fighter() {}

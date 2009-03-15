#define RIGHT "right"
#define LEFT "left"

#include <PA9.h> // DS functions that we use come from here
#include "hitbox.h"
#include "knockback.h"
#include "circle.h"
#include "fighter.h"
#include "stage.h"
#include "effect.h"
#include <vector>
#include <string>
#include <math.h>
#include <map> // maps
#include "gfx/all_gfx.h" //may want to reduce this later
#include "gfx/all_sounds.c" // all sound effects (just small ones, not MP3s)

using std::vector;
using std::string;

Fighter::Fighter(int xpos, int ypos, int num, vector<Fighter*> listplayers, Display *disp, bool AI){
	display=disp;
	players=listplayers;
	shieldstr = 64;
	myledge = -1;
	acceleration = 0;
	x = xpos;
	y = ypos;
	hangtime = 0;
	ledgewait = 0;
	CAPE = false;
	COUNTER = false;
	effectwait = 0;
	chargecount = 0;
	isCPU = AI;
	lasthitby = -1;
	SPRITENUM = num + 100;
	charnum = players.size();
	startx = x;
	starty = y;
	action = FALL;
	aerial = true;
	delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
	dx = dy = fastfall = DI = 0.0;
	percentage = 0;
	ymomentum = 0.0;
	momentumtime = 0;
	grabtimeleft = 0;
	initAtkbox();
	initDefbox();
	initFrames();
	initSprite();
	if(x > stage->width/2) setDirection("right");
	else setDirection("left");
} // initializes all of the variables
// virtual methods
void Fighter::initDefbox() {
	for(int n = 0; n < 250; n++) {
		Hitbox h;
		alldefbox.push_back(h);
	}
	char* start = "/SSBDS_Files/hitboxes/";
	char* end = ".def";
	FILE* file = fopen((start+name+end).c_str(), "rb");
	if(!file) while(true) {}
	char line[64];
	while(true) {
		Hitbox tempbox;
		fgets(line, 64, file);
		int frame = atoi(strtok(line, " \t"));
		if(frame == -1) break;					
		double xpos = atof(strtok(NULL, " \t"));
		double ypos = atof(strtok(NULL, " \t"));
		double radius = atof(strtok(NULL, " \t"));

		alldefbox[frame].addCircle(Circle(xpos, (int)(ypos)%64, radius,FX_NONE));
	}
	fclose(file);
}
void Fighter::initAtkbox() {
	for(int n = 0; n < 250; n++) {
		Hitbox h;
		allatkbox.push_back(h);
	}
	char* start = "/SSBDS_Files/hitboxes/";
	char* end = ".atk";
	FILE* file = fopen((start+name+end).c_str(), "rb");
	if(!file) while(true) {}
	char line[64];
	while(true) {
		Hitbox tempbox;
		fgets(line, 64, file);

		int frame = atoi(strtok(line, " \t"));
		if(frame == -1) break;
		double xpos = atof(strtok(NULL, " \t"));
		double ypos = atof(strtok(NULL, " \t"));
		double radius = atof(strtok(NULL, " \t"));

		double kbx = atof(strtok(NULL, " \t"));
		double kby = atof(strtok(NULL, " \t"));
		int kblen = atoi(strtok(NULL, " \t"));
		int dmg = atoi(strtok(NULL, " \t"));

		allatkbox[frame].addCircle(createAtkbox(xpos, (int)(ypos)%64, radius, Knockback(kbx, kby, kblen), dmg));
	}
	fclose(file);
}
void Fighter::initSprite() {
	PA_FatEasyLoadSpritePal(MAIN_SCREEN, SPRITENUM-100, name.c_str());
	PA_FatLoadSprite(MYCHAR, name.c_str());
	PA_CreateSprite(MAIN_SCREEN, SPRITENUM, (void*)sprite_gfx[MYCHAR], OBJ_SIZE_64X64, COLOR256, SPRITENUM-100, -64, -64);
	PA_LoadSpritePal(MAIN_SCREEN, 14-(SPRITENUM-100), (void*)shield_Pal);
	PA_CreateSprite(MAIN_SCREEN, 30+(SPRITENUM-100), (void*)shield_Sprite, OBJ_SIZE_64X64, COLOR256, 14-(SPRITENUM-100), -64, -64);
	PA_SetSpriteRotEnable(MAIN_SCREEN, 30+(SPRITENUM-100), SPRITENUM-100);
}
void Fighter::initFrames(){}//implemented in subclasses
void Fighter::actCPU() {
	int Cenemy = 0;
	double Cdistance = 10000000;
	double Cangle = 0;
	double Cx = 0;
	double Cy = 0;
	for(int n = 0; n < (int)players.size(); n++) {
		if(players[n] -> charnum != charnum) {
			double dx = (players[n] -> x) - x;
			double dy = (players[n] -> y) - y;
			double distance = sqrt(dx*dx + dy*dy);
			if(distance < Cdistance) {
				Cenemy = n;
				Cdistance = distance;
				Cx = dx;
				Cy = dy;
				Cangle = atan2(dy, dx) * 180 / M_PI; // from -180 to 180
			}
		}
	}
	// finds disttance to it.
	if(action == BSIDE) bside();
	if(action == BUP) bup();
	if(action == BDOWN) bdown();
	if(action == BNEUT) bneut();
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
		if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
		else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
	}
	else if(hitstun > 0) {
		if(y != stage->getFloors()[0].y ) aerial = true;
		hitstun--;
		if(dx > 0) {
			dx -= kx/(hitstun/3);
			if(dx < 0) dx = 0;
		}
		else if(dx < 0) {
			dx -= kx/(hitstun/3);
			if(dx > 0) dx = 0;
		}
		if(dy > 0) {
			dy -= ky/(hitstun/3);
			if(dy < 0) dy = 0;
		}
		else if(dy < 0) {
			dy -= ky/(hitstun/3);
			if(dy > 0) dy = 0;
		}
		if(hitstun == 0) {
			action = STUN;
			if(aerial) fall();
			else idle();
		}
		if(checkFloorCollision()) idle();
		if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
		else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
	}
	else {
		if(landinglag > 0) {
			landinglag--;
			if(landinglag == 0) idle();
		}
		if(delay > 0) delay--;
		if(action == JUMP || action == DOUBLEJUMP) {
			if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
			else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
			// act in air
		}
		if(action == DODGE && delay <= 0) shield();
		if(action == AIRDODGE && delay <= 0) fall();
		if(action == ROLL && delay <= 0) {
			dx = 0;
			if(direction == LEFT) {
				direction = RIGHT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			}
			else if(direction == RIGHT) {
				direction = LEFT;
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			}
			//don't shield when done rolling right now
			//shield();
			idle();
		}
		//FIXME: is this even possible for a CPU?
		if(action == RELEASED || action == RELEASE) {
			if(delay <= 0) idle();
		}
		if(action == SLIDE && delay <= 0) idle();
		if(action == SLIDE) {
			if(dx > 0) {
				dx -= .25;
				if(dx <= 0) dx = 0;
			}
			else if(dx < 0) {
				dx += .25;
				if(dx >= 0) dx = 0;
			}
		}
		if(action == JAB) {

		}
		if(action == ATTACK) {
			if(delay <= 0) {
				idle();
			}
		}
		if(action == DASHATTACK) {
			x += dx;
			if(delay <= 0) idle();
		}
		if(action == BSIDE || action == BUP || action == BDOWN || action == BNEUT) {
			if(delay <= 0) idle();
		}
		if(action == AIRATTACK) {
			if(checkFloorCollision()) {
				if(delay > 0) land();
				else idle();
				delay = 0;
			}
			else if(delay <= 0) fall();
			if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
			else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
		} // checks for stage collision with aerial				
		if((action == JUMP || action == DOUBLEJUMP) && delay <= 0) fall(); // falls when jump/multijump are finished animating
		if(action == FALL) {
			if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
			else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
			if(checkFloorCollision()) idle();
			else if(Cangle < -45 && Cangle > -135 && jumpcount < jumpmax) {
				if(Cx > 0) setDirection("right");
				if(Cx < 0) setDirection("left");
				doubleJump();
			}
		}
		if(aerial && action != AIRATTACK && action != AIRLAG) {
			// act air
			if(Cdistance < 30) {
				if(Cangle > -45 && Cangle < 45){
					if (direction=="right") fair();
					else bair();
				}
				else if(Cangle < -135 || Cangle > 135){ 
					if (direction=="right") bair();
					else fair();
				}
				else if(Cangle < -45 && Cangle > -135) uair();
				else if(Cangle > 45 && Cangle < 135) dair();
			}
			else {
				if(Cangle < -45 && Cangle > -135 && jumpcount < jumpmax && action != JUMP && action != DOUBLEJUMP) {
					if(Cx > 0) setDirection("right");
					if(Cx < 0) setDirection("left");
					doubleJump();
				}
				else if(Cangle < 45 && Cangle > -45) directionalInfluence(1); //right
				else if(Cangle > 135 || Cangle < -135) directionalInfluence(-1); //left
			}//too far away
		}
		if(action == SHIELD) {
			//shield shrinks/breaks
			shieldstr -= (65-shieldstr)/50;
			if(shieldstr <= 0) {
				AS_SoundQuickPlay(shieldbreak);
				hitstun = 300;
				stun();
			}
			PA_SetRotsetNoAngle(MAIN_SCREEN, SPRITENUM-100, (int)(2048-24*shieldstr), (int)(2048-24*shieldstr));
			//should I grab?
			//grab();
			//should I dodge?
			//dodge();
			//should I stop shielding?
			//idle();
			//should I roll?
			//roll();
		}
		if(action == RUN) {
			if((Cdistance < 30) || (dx < 0 && (Cangle < 90 && Cangle > -90)) || (dx > 0 && (Cangle > 90 || Cangle < -90))) slide();					
			else if(Cangle < -45 && Cangle > -135 && jumpcount == 0) {
				if(Cx > 0) setDirection("right");
				if(Cx < 0) setDirection("left");
				jump();
			}
			else{
				if(direction == RIGHT) run(1);
				else run(-1);
			}
			// or dash attack
			// or run more
			// or slide
		}
		if(action == IDLE) {
			if(Cdistance < 30) {
				if(Cangle > -45 && Cangle < 45){
					setDirection("right");
					smashright();
				}
				else if(Cangle < -135 || Cangle > 135){ 
					setDirection("left");
					smashleft();
				}
				else if(Cangle < -45 && Cangle > -135){
					smashup();
				}
				else if(Cangle > 45 && Cangle < 135){
					smashdown();
				}
			}
			else if(Cangle < -45 && Cangle > -135 && jumpcount == 0 && Cdistance > 50) {
				if(Cx > 0) setDirection("right");
				if(Cx < 0) setDirection("left");
				jump();
			}
			else if(Cangle < 45 && Cangle > -45) run(1); //right
			else if(Cangle > 135 || Cangle < -135) run(-1); //left
		}
		if(action == CROUCH) {
			// idle
			// or dtilt
			// or bdown
		}
		if(action == HANG) {
			if(PA_RandMax(100) > 98) {
				myledge = -1;
				int randn=PA_Rand()%3;
				if (randn==0) rollUp();
				else if (randn==1) attackUp();
				else jumpUp();
			}
		} //AI is hanging from ledge
	}
	move();
}
void Fighter::act() {
	std::map<int, int> customcontrols = getcustomcontrols();
	if(effectwait > 0) effectwait--;
	if(ledgewait > 0) ledgewait--;
	if(isCPU) {
		actCPU();
		return;
	}
	if(action == BSIDE) bside();
	if(action == BUP) bup();
	if(action == BDOWN) bdown();
	if(action == BNEUT) bneut();
	if(action == FTHROW) fthrow();
	if(action == BTHROW) bthrow();
	if(action == DTHROW) dthrow();
	if(action == UTHROW) uthrow();
	if(action == CHARGELEFT && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashleft();
	if(action == CHARGERIGHT && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashright();
	if(action == CHARGEUP && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashup();
	if(action == CHARGEDOWN && (custom_action(ACTION_SMASH, PAD_RELEASED) || delay == 0)) smashdown();
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
	else {
		if(lasthitby != -1 && aerial == false) lasthitby = -1;
		if(tiltlag > 0) {
			tiltlag--;
			if((custom_action(ACTION_JUMP, PAD_RELEASED) || (Pad.Released.Up && getTapJumpOn()))) {
				shorthop();
				tiltlag = 0;
			}
			else if(tiltlag == 0) {
				bool AB = false;
				if(customcontrols[ACTION_SMASH] == BUTTON_AB) {
					if(custom_action(ACTION_SMASH, PAD_HELD)) {
						if(Pad.Held.Left) chargeleft();
						else if(Pad.Held.Right) chargeright();
						else if(Pad.Held.Up) chargeup();
						else if(Pad.Held.Down) chargedown();
						else jab();
						AB = true;
					}
				}
				if(customcontrols[ACTION_BASIC] == BUTTON_AB) {
					if(custom_action(ACTION_BASIC, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) ftilt();
						else if(Pad.Held.Up) utilt();
						else if(Pad.Held.Down) dtilt();
						else jab();
						AB = true;
					}	
				}
				if(customcontrols[ACTION_JUMP] == BUTTON_AB) {
					if((custom_action(ACTION_JUMP, PAD_HELD) || (Pad.Held.Up && getTapJumpOn())) && jumpcount < jumpmax) {
						jump();
						AB = true;
					}
				}
				if(customcontrols[ACTION_SPECIAL] == BUTTON_AB) {
					if(custom_action(ACTION_SPECIAL, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) bside();
						else if(Pad.Held.Up) bup();
						else if(Pad.Held.Down) bdown();
						else bneut();
						AB = true;
					}
				}
				if(customcontrols[ACTION_SHIELD] == BUTTON_AB) {
					if(custom_action(ACTION_SHIELD, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) roll();
						else shield();
						AB = true;
					}
				}						
				if(customcontrols[ACTION_GRAB] == BUTTON_AB) {
					if(custom_action(ACTION_GRAB, PAD_NEWPRESS)) {
						grab();
						AB = true;
					}
				}	
				// have to check the AB combo first otherwise it'll register as just A or just B
				if(!AB) {
					if(custom_action(ACTION_SMASH, PAD_HELD)) {
						if(Pad.Held.Left) chargeleft();
						else if(Pad.Held.Right) chargeright();
						else if(Pad.Held.Up) chargeup();
						else if(Pad.Held.Down) chargedown();
						else jab();
					}
					else if(custom_action(ACTION_BASIC, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) ftilt();
						else if(Pad.Held.Up) utilt();
						else if(Pad.Held.Down) dtilt();
						else jab();
					}
					else if(custom_action(ACTION_SPECIAL, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) bside();
						else if(Pad.Held.Up) bup();
						else if(Pad.Held.Down) bdown();
						else bneut();
					}
					else if((custom_action(ACTION_JUMP, PAD_HELD) || (Pad.Held.Up && getTapJumpOn())) && jumpcount < jumpmax) jump();
					else if(custom_action(ACTION_SHIELD, PAD_HELD)) {
						if(Pad.Held.Right || Pad.Held.Left) roll();
						else shield();
					}
					else if(custom_action(ACTION_GRAB, PAD_HELD)) grab();
					else if((Pad.Held.Right || Pad.Held.Left)) run();
					else if(Pad.Held.Down) crouch();
					else idle();
				}
			}
		}
		if(airlag > 0) {
			airlag--;
			if(checkFloorCollision()) idle();
			else if(airlag == 0) {
				bool AB = false;
				if(customcontrols[ACTION_BASIC] == BUTTON_AB) {
					if(custom_action(ACTION_BASIC, PAD_HELD) || custom_action(ACTION_BASIC, PAD_RELEASED)) {
						if(Pad.Held.Up) uair();
						else if(Pad.Held.Down) dair();
						else if((Pad.Held.Left && direction == "left") || (Pad.Held.Right && direction == "right")) fair();
						else if((Pad.Held.Left && direction == "right") || (Pad.Held.Right && direction == "left")) bair();
						else nair();
						AB = true;
					}		
				}
				if(customcontrols[ACTION_JUMP] == BUTTON_AB) {
					if(((Pad.Held.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_HELD) || custom_action(ACTION_JUMP, PAD_RELEASED)) && jumpcount < jumpmax) {
						doubleJump();
						AB = true;
					}
				}
				if(customcontrols[ACTION_SPECIAL] == BUTTON_AB) {
					if(custom_action(ACTION_SPECIAL, PAD_HELD) || custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
						if(Pad.Held.Up) bup();
						else if(Pad.Held.Down) bdown();
						else if(Pad.Held.Left || Pad.Held.Right) bside();
						else bneut();
						AB = true;
					}						
				}
				if(customcontrols[ACTION_SHIELD] == BUTTON_AB) {
					if(custom_action(ACTION_SHIELD, PAD_HELD) || custom_action(ACTION_SHIELD, PAD_RELEASED)) {
						if(airdodgecount < 1) {
							airdodge();
							airdodgecount++;
							AB = true;
						}
					}						
				}
				if(!AB) {
					if(custom_action(ACTION_BASIC, PAD_HELD) || custom_action(ACTION_BASIC, PAD_RELEASED)) {
						if(Pad.Held.Up) uair();
						else if(Pad.Held.Down) dair();
						else if((Pad.Held.Left && direction == "left") || (Pad.Held.Right && direction == "right")) fair();
						else if((Pad.Held.Left && direction == "right") || (Pad.Held.Right && direction == "left")) bair();
						else nair();
					}
					else if(custom_action(ACTION_SPECIAL, PAD_HELD) || custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
						if(Pad.Held.Up) bup();
						else if(Pad.Held.Down) bdown();
						else if(Pad.Held.Left || Pad.Held.Right) bside();
						else bneut();
					}
					else if(custom_action(ACTION_SHIELD, PAD_HELD) || custom_action(ACTION_SHIELD, PAD_RELEASED)) {
						if(airdodgecount < 1) {
							airdodge();
							airdodgecount++;
						}
					}
					else if(((Pad.Held.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_HELD) || custom_action(ACTION_JUMP, PAD_RELEASED)) && jumpcount < jumpmax) doubleJump();
					else fall();
				}
			}
		}
		if(landinglag > 0) {
			landinglag--;
			if(landinglag == 0) idle();
		} // lags upon landing after an aerial
		if(delay > 0) delay--; // counts down the time before a new animation is set (allows for animations to finish)
		if(action == HANG) {
			hangtime++;
			if(custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
				attackUp();
			}
			else if(custom_action(ACTION_SHIELD, PAD_NEWPRESS)) {
				rollUp();
			}
			else if((Pad.Newpress.Up && getTapJumpOn()) || custom_action(ACTION_JUMP, PAD_NEWPRESS)) {
				jumpUp();
			}
			else if(Pad.Newpress.Down || hangtime >= 300) {
				hangtime = 0;
				ledgewait = 60;
				myledge = -1;
				fall();
			}
			else if(Pad.Newpress.Right || Pad.Newpress.Left) {
				y = y+handy-bottomside;
				myledge = -1;
				if(direction == "left") {
					x = x+handx-rightside;
					idle();
					setDirection("left");
				}
				else {
					x = x+handx-leftside;
					idle();
					setDirection("right");
				}
			}
		}
		if(action == DODGE && delay <= 0) shield();
		if(action == AIRDODGE && delay <= 0) fall();
		if(action == ROLL && delay <= 0) {
			dx = 0;
			if(direction == "left") {
				direction = "right";
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			}
			else if(direction == "right") {
				direction = "left";
				PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			}
			shield();
		}
		if(action == RELEASE || action == RELEASED) {
			if(delay <= 0) idle();
		}
		if(action == SLIDE && delay <= 0) {
			idle();
		}
		if(action == SLIDE) {
			if(dx > 0) {
				dx -= .25;
				if(dx <= 0) dx = 0;
			}
			else if(dx < 0) {
				dx += .25;
				if(dx >= 0) dx = 0;
			}
		}
		if(action == JAB) {
			if((custom_action(ACTION_BASIC, PAD_RELEASED) && delay > 100) || delay <= 0) {
				idle();
				delay = 0;
			}
		}
		if(action == ATTACK) {
			if(delay <= 0) {
				chargecount = 0;
				if(Pad.Held.Down) crouch();
				else idle(); // idles when attack is done
			}
		} 
		if(action == DASHATTACK) {
			x += dx;
			if(delay <= 0) idle();
		}
		if(action == BSIDE || action == BUP || action == BDOWN || action == BNEUT) {
			if(delay <= 0) idle();
		}
		if(action == AIRATTACK) {
			if(lcancel > 0) lcancel--;
			if(checkFloorCollision()) {
				if(delay > 0 && lcancel <= 0) land();
				else idle();
				delay = 0;
			}
			else if(custom_action(ACTION_SHIELD, PAD_RELEASED)) lcancel = 10;
			else if(delay <= 0) fall();
		} // checks for stage collision with aerial
		if((action == JUMP || action == DOUBLEJUMP) && delay <= 0) fall(); // falls when jump/multijump are finished animating
		if((action == DOUBLEJUMP || action == JUMP ) && (custom_action(ACTION_BASIC, PAD_NEWPRESS) || (Stylus.Newpress && getCStickStylus()) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS))) {
			ymomentum = dy;
			momentumtime = delay;
			dy = 0;
			if(Stylus.Newpress && getCStickStylus()) airAttackStylus();
			else airlag = 2;
		}
		if(action == FALL) {
			directionalInfluence();
			if(checkFloorCollision()) idle();
		} // checks for stage collision when falling
		if(aerial && action != AIRATTACK && action != AIRLAG && action != FTHROW && action != BTHROW && action != UTHROW && action != DTHROW) {
			actAir();
		} // acts in the air
		if(action == SHIELD) {
			shieldstr -= (65-shieldstr)/50;
			if(shieldstr <= 0) {
				AS_SoundQuickPlay(shieldbreak);
				hitstun = 300;
				stun();
			}
			PA_SetRotsetNoAngle(MAIN_SCREEN, SPRITENUM-100, (int)(2048-24*shieldstr), (int)(2048-24*shieldstr));
			if(!custom_action(ACTION_SHIELD, PAD_HELD)) idle();
			if(Pad.Newpress.Left || Pad.Newpress.Right) roll();
			if(Pad.Newpress.Down) dodge();
			if(custom_action(ACTION_BASIC, PAD_NEWPRESS) && getShieldGrabOn()) grab();
		}
		else if(shieldstr < 64) shieldstr += .1;
		if(action == GRAB) {
			if(delay <= 0) {
				if(custom_action(ACTION_SHIELD, PAD_HELD)) shield();
				else idle();
			}
		}
		if(action == GRABATK) {
			grabtimeleft--;
			if(delay <= 0) {
				grabbedenemy -> percentage += 3;
				hold(grabtimeleft);
			}
		}
		if(action == HOLD) {
			if(delay <= 0) {
				if(direction == "right") {
					release("left");
					grabbedenemy -> released("right");
				}
				else if(direction == "left") {
					release("right");
					grabbedenemy -> released("left");
				}
				grabbedenemy = NULL;
			}
			else if(custom_action(ACTION_BASIC, PAD_NEWPRESS)) {
				grabtimeleft = delay;
				grabattack();
			}
			else if(Pad.Newpress.Up) uthrow();
			else if(Pad.Newpress.Down) dthrow();
			else if((direction == "right" && Pad.Newpress.Left) || (direction == "left" && Pad.Newpress.Right)) bthrow();
			else if((direction == "right" && Pad.Newpress.Right) || (direction == "left" && Pad.Newpress.Left)) fthrow();
		}
		if(action == RUN) {
			if((Pad.Newpress.Up) || custom_action(ACTION_JUMP, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
				tiltlag = 5;
				action = TILTLAG;
			}
			else if(custom_action(ACTION_BASIC, PAD_NEWPRESS)) {
				dashAttack();
			}
			else if(Stylus.Newpress && getCStickStylus()) {
				smashAttack();
			}
			else if(Pad.Held.Right || Pad.Held.Left) run();
			else if(Pad.Released.Right || Pad.Released.Left) {
				slide();
			}
			else {
				idle();
			}
		}
		else {
			acceleration = 0;
#ifdef DEBUG_ON
			PA_OutputText(MAIN_SCREEN, 0, 4, "                                ");
#endif
		}
		if(action == IDLE) {
			actGround();
			setDirection();
		}
		if(action == CROUCH) {
			if(!Pad.Held.Down) idle();
			if(custom_action(ACTION_BASIC, PAD_NEWPRESS)) dtilt();
			if(custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) bdown();
		}
	}
	move(); // moves
} // usually the acting method, but can be overwritten
void Fighter::actAir() {
	if(action == BUP || action == BDOWN || action == BSIDE || action == BNEUT) return;
	if(action == AIRDODGE) {
		if(checkFloorCollision()) {
			dx *= 3;
			slide();
		}
		return;
	}
	if((custom_action(ACTION_BASIC, PAD_NEWPRESS) || (custom_action(ACTION_JUMP, PAD_NEWPRESS) && jumpmax <= 2) || (custom_action(ACTION_JUMP, PAD_HELD) && jumpmax > 2) || (Pad.Newpress.Up && getTapJumpOn() && jumpmax <= 2) || (Pad.Held.Up && getTapJumpOn() && jumpmax > 2) /*|| Pad.Held.Down || Pad.Held.Right || Pad.Held.Left */|| Pad.Newpress.B || Pad.Newpress.R || Pad.Newpress.L) && action != JUMP && action != DOUBLEJUMP) {
		airlag = 2;
		action = AIRLAG;
	}
	if((custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS) || custom_action(ACTION_SHIELD, PAD_NEWPRESS)) && (action == JUMP || action == DOUBLEJUMP)) {
		airlag = 2;
		action = AIRLAG;
	}
	if((custom_action(ACTION_JUMP, PAD_NEWPRESS)) && jumpcount < jumpmax && action != JUMP && action != DOUBLEJUMP) doubleJump();  // uses second (3rd, 4th, etc) jump(s)
	if(Stylus.Newpress && getCStickStylus()) airAttackStylus();
	directionalInfluence();
}
void Fighter::actGround() {
	if(tiltlag <= 0) {
		if(custom_action(ACTION_GRAB, PAD_NEWPRESS) || custom_action(ACTION_SHIELD, PAD_NEWPRESS) || Pad.Held.Right || Pad.Held.Left || Pad.Newpress.Down || Pad.Newpress.Up || custom_action(ACTION_BASIC, PAD_NEWPRESS) || custom_action(ACTION_SPECIAL, PAD_NEWPRESS) || custom_action(ACTION_JUMP, PAD_NEWPRESS)) {
			action = TILTLAG;
			tiltlag = 5;
		}
		else idle();
	}
} // acts on the ground based on key presses	
void Fighter::bside() {}
void Fighter::bup() {}
void Fighter::bdown() {}
void Fighter::bneut() {}
void Fighter::uthrow() {}
void Fighter::dthrow() {}
void Fighter::bthrow() {}
void Fighter::fthrow() {}
void Fighter::release(string dir) {
	action = RELEASE;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	delay = 8;
	if(dir == "right") dx = 2;
	else dx = -2;
}
void Fighter::released(string dir) {
	action = RELEASED;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	delay = 8;
	if(dir == "right") dx = 2;
	else dx = -2;
}
void Fighter::grabbed(int otherx, int othery) {
	action = GRABBED;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRABBED], endframes[GRABBED], framespeeds[GRABBED], ANIM_LOOP, -1);
	x = otherx;
	y = othery;
	aerial = false;
	dx = 0;
	dy = 0;			
	playsound(GRABBED);
}
void Fighter::grab() {
	action = GRAB;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[GRAB], endframes[GRAB], framespeeds[GRAB], ANIM_LOOP, -1);
	setDirection();
	delay = 60/framespeeds[GRAB] * (endframes[GRAB]-startframes[GRAB]+1);
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
	delay = 60/framespeeds[GRABATK] * (endframes[GRABATK]-startframes[GRABATK]+1);
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
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHIELD], endframes[SHIELD], framespeeds[SHIELD], ANIM_LOOP, -1);
	playsound(SHIELD);	
}
void Fighter::roll(string dir) {
	action = ROLL;
	setDirection();
	if(dir == "") {
		if(Pad.Held.Left) dir = "left";
		else if(Pad.Held.Right) dir = "right";
	}	
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[ROLL], endframes[ROLL], framespeeds[ROLL], ANIM_LOOP, -1);
	delay = 60/framespeeds[ROLL] * (endframes[ROLL]-startframes[ROLL]+1);
	if(dir == "left") dx = -2;
	if(dir == "right") dx = 2;
	playsound(ROLL);
}
void Fighter::rollUp(){
	if(action == HANG) {
		myledge = -1;
		y = y+handy-bottomside;
		if(direction == "left") {
			x = x+handx-rightside;
			roll("left");
		}
		else {
			x = x+handx-leftside;
			roll("right");
		}
	}
}
void Fighter::attackUp(){
	if(action == HANG) {
		myledge = -1;
		y=y+handy-bottomside;
		if(direction == "left") x=x+handx-rightside;
		else x=x+handx-leftside;
		ftilt();
	}
}
void Fighter::jumpUp(){
	if(action == HANG) {
		myledge = -1;
		y = y+handy-bottomside;
		jump();
	}
}
void Fighter::dodge() {
	action = DODGE;
	setDirection();
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DODGE], endframes[DODGE], framespeeds[DODGE], ANIM_LOOP, -1);
	delay = 60/framespeeds[DODGE] * (endframes[DODGE]-startframes[DODGE]+1);
	playsound(DODGE);
}
void Fighter::airdodge() {
	action = AIRDODGE;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[AIRDODGE], endframes[AIRDODGE], framespeeds[AIRDODGE], ANIM_LOOP, -1);
	delay = 60/framespeeds[AIRDODGE] * (endframes[AIRDODGE]-startframes[AIRDODGE]+1);
	if(Pad.Held.Up) dy = -gravity -2;
	else if (Pad.Held.Down) dy = -gravity + 2;
	if(Pad.Held.Right) dx = 2;
	else if(Pad.Held.Left) dx = -2;
	if(!Pad.Held.Up && !Pad.Held.Down && !Pad.Held.Right && !Pad.Held.Left) {
		dx = 0;
		dy = -gravity;
	}
	if(!Pad.Held.Up && !Pad.Held.Down) dy = -gravity;
	fastfall = 0;
	DI = 0;
	playsound(AIRDODGE);
}
void Fighter::crouch() {
	action = CROUCH;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CROUCH], endframes[CROUCH], framespeeds[CROUCH], ANIM_LOOP, -1);
	playsound(CROUCH);
}
void Fighter::fall() {
	aerial = true;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FALL], endframes[FALL], framespeeds[FALL], ANIM_LOOP, -1);
	directionalInfluence();
	dy = 0;
	dx = 0;
	action = FALL;
	playsound(FALL);
}
void Fighter::idle() {
	if(action != IDLE) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[IDLE], endframes[IDLE], framespeeds[IDLE], ANIM_LOOP, -1);
	dx = 0;
	dy = 0;
	action = IDLE;
	playsound(IDLE);
}
void Fighter::run(int d) {
	if(action != RUN) PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[RUN], endframes[RUN], framespeeds[RUN], ANIM_LOOP, -1);
	if(d == 0) {
		if(Pad.Held.Left) dx = -runspeed/4-acceleration;
		if(Pad.Held.Right) dx = runspeed/4+acceleration;
		setDirection();
	}
	else {
		dx = (runspeed/4+acceleration)*d;
		if(d > 0) setDirection("right");
		if(d < 0) setDirection("left");
	}
	if(action == RUN) {
		acceleration += .05*runspeed;
		if(acceleration > runspeed) {
			acceleration = runspeed;
#ifdef DEBUG_ON
			PA_OutputText(MAIN_SCREEN, 0, 4, "                                 ");
			PA_OutputText(MAIN_SCREEN, 0, 4, "Max Speed");
#endif
		}
	}
	action = RUN;
	playsound(RUN);
}
void Fighter::shorthop() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHORTHOP], endframes[SHORTHOP], framespeeds[SHORTHOP], ANIM_LOOP, -1);
	dy = -4;
	fastfall = 0;
	dx = 0;
	delay = 60/framespeeds[SHORTHOP] * (endframes[SHORTHOP] - startframes[SHORTHOP] + 1);
	action = JUMP;
	aerial = true;
	jumpcount++;
	playsound(SHORTHOP);
}
void Fighter::jump() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JUMP], endframes[JUMP], framespeeds[JUMP], ANIM_LOOP, -1);
	dy = -6;
	fastfall = 0;
	dx = 0;
	delay = 60/framespeeds[JUMP] * (endframes[JUMP] - startframes[JUMP] + 1);
	action = JUMP;
	aerial = true;
	jumpcount++;
	playsound(JUMP);
}
void Fighter::doubleJump() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DOUBLEJUMP], endframes[DOUBLEJUMP], framespeeds[DOUBLEJUMP], ANIM_LOOP, -1);
	action = DOUBLEJUMP;
	dy = -3.5 - .5 * (jumpmax-jumpcount);
	fastfall = 0;
	delay = 60/framespeeds[DOUBLEJUMP] * (endframes[DOUBLEJUMP] - startframes[DOUBLEJUMP] + 1);
	jumpcount++;
	aerial = true;
	setDirection();
	if(effectwait <= 0) {
		display -> addeffect(Effect(x, y+32, FX_AIRJUMP, charnum, display->scrollx, display->scrolly));
		effectwait = 15;
	}
	playsound(DOUBLEJUMP);
}
void Fighter::jab() {
	action = JAB;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JAB], endframes[JAB], framespeeds[JAB], ANIM_LOOP, -1);
	delay = 60/framespeeds[JAB] * (endframes[JAB]-startframes[JAB]+1);
	playsound(JAB);
}
void Fighter::dashAttack() {
	if(Pad.Held.Left) dx = -1.5;
	if(Pad.Held.Right) dx = 1.5;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DASHATTACK], endframes[DASHATTACK], framespeeds[DASHATTACK], ANIM_LOOP, -1);
	delay = 60/framespeeds[DASHATTACK] * (endframes[DASHATTACK]-startframes[DASHATTACK]+1);
	action = DASHATTACK;
	playsound(DASHATTACK);
}
void Fighter::ftilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FTILT], endframes[FTILT], framespeeds[FTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60/framespeeds[FTILT] * (endframes[FTILT]-startframes[FTILT]+1);
	setDirection();
	playsound(FTILT);
}
void Fighter::utilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UTILT], endframes[UTILT], framespeeds[UTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60/framespeeds[UTILT] * (endframes[UTILT]-startframes[UTILT]+1);		
	playsound(UTILT);
}
void Fighter::dtilt() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DTILT], endframes[DTILT], framespeeds[DTILT], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60/framespeeds[DTILT] * (endframes[DTILT]-startframes[DTILT]+1);		
	playsound(DTILT);		
}
void Fighter::chargeleft() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGELEFT], endframes[CHARGELEFT], framespeeds[CHARGELEFT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	action = CHARGELEFT;
	direction = "left";
	dx = 0;
	delay = 60/framespeeds[CHARGELEFT] * (endframes[CHARGELEFT]-startframes[CHARGELEFT]+1) * 15;
	playsound(CHARGELEFT);
}
void Fighter::chargeright() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGERIGHT], endframes[CHARGERIGHT], framespeeds[CHARGERIGHT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	action = CHARGERIGHT;
	direction = "right";
	dx = 0;
	delay = 60/framespeeds[CHARGERIGHT] * (endframes[CHARGERIGHT]-startframes[CHARGERIGHT]+1) * 15;
	playsound(CHARGERIGHT);
}
void Fighter::chargeup() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEUP], endframes[CHARGEUP], framespeeds[CHARGEUP], ANIM_LOOP, -1);
	action = CHARGEUP;
	delay = 60/framespeeds[CHARGEUP] * (endframes[CHARGEUP]-startframes[CHARGEUP]+1) * 15;
	playsound(CHARGEUP);
}
void Fighter::chargedown() {
	chargecount = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEDOWN], endframes[CHARGEDOWN], framespeeds[CHARGEDOWN], ANIM_LOOP, -1);
	action = CHARGEDOWN;
	dx = 0;
	delay = 60/framespeeds[CHARGEDOWN] * (endframes[CHARGEDOWN]-startframes[CHARGEDOWN]+1) * 15;
	playsound(CHARGEDOWN);
}
void Fighter::smashleft() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHLEFT], endframes[SMASHLEFT], framespeeds[SMASHLEFT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	action = ATTACK;
	direction = "left";
	delay = 60/framespeeds[SMASHLEFT] * (endframes[SMASHLEFT]-startframes[SMASHLEFT]+1);
	playsound(SMASHLEFT);
}
void Fighter::smashright() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHRIGHT], endframes[SMASHRIGHT], framespeeds[SMASHRIGHT], ANIM_LOOP, -1);
	PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	action = ATTACK;
	direction = "right";
	delay = 60/framespeeds[SMASHRIGHT] * (endframes[SMASHRIGHT]-startframes[SMASHRIGHT]+1);
	playsound(SMASHRIGHT);
}
void Fighter::smashup() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHUP], endframes[SMASHUP], framespeeds[SMASHUP], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60/framespeeds[SMASHUP] * (endframes[SMASHUP]-startframes[SMASHUP]+1);
	playsound(SMASHUP);
}
void Fighter::smashdown() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHDOWN], endframes[SMASHDOWN], framespeeds[SMASHDOWN], ANIM_LOOP, -1);
	action = ATTACK;
	delay = 60/framespeeds[SMASHDOWN] * (endframes[SMASHDOWN]-startframes[SMASHDOWN]+1);
	playsound(SMASHDOWN);
}
void Fighter::fair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FAIR], endframes[FAIR], framespeeds[FAIR], ANIM_LOOP, -1);
	delay = 60/framespeeds[FAIR] * (endframes[FAIR] - startframes[FAIR] + 1);
	playsound(FAIR);
}
void Fighter::bair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[BAIR], endframes[BAIR], framespeeds[BAIR], ANIM_LOOP, -1);
	delay = 60/framespeeds[BAIR] * (endframes[BAIR] - startframes[BAIR] + 1);
	if(Pad.Held.Left) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
	if(Pad.Held.Right) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	playsound(BAIR);
}
void Fighter::uair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UAIR], endframes[UAIR], framespeeds[UAIR], ANIM_LOOP, -1);
	delay = 60/framespeeds[UAIR] * (endframes[UAIR] - startframes[UAIR] + 1);
	playsound(UAIR);
}
void Fighter::dair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DAIR], endframes[DAIR], framespeeds[DAIR], ANIM_LOOP, -1);
	delay = 60/framespeeds[DAIR] * (endframes[DAIR] - startframes[DAIR] + 1);
	playsound(DAIR);
}
void Fighter::nair() {
	action = AIRATTACK;
	dy = 0;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[NAIR], endframes[NAIR], framespeeds[NAIR], ANIM_LOOP, -1);
	delay = 60/framespeeds[NAIR] * (endframes[NAIR] - startframes[NAIR] + 1);
	playsound(NAIR);
}
void Fighter::stun() {
	myledge = -1;
	action = STUN;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[STUN], endframes[STUN], framespeeds[STUN], ANIM_LOOP, -1);
	playsound(STUN);
}
void Fighter::slide() {
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SLIDE], endframes[SLIDE], framespeeds[SLIDE], ANIM_LOOP, -1);
	action = SLIDE;
	delay = 5;
	playsound(SLIDE);
}
void Fighter::hang() {
	lasthitby = -1;
	PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[HANG], endframes[HANG], framespeeds[HANG], ANIM_LOOP, -1);
	action = HANG;
	hangtime = 0;
}
// Sound playing
void Fighter::playsound(int sndnum) {}
// constant methods
void Fighter::setStage(Stage *s) { 
	stage = s; 
	x = stage->spawnx[charnum];
	y = stage->spawny[charnum];
	startx = x;
	starty = y;
}
int Fighter::getHitstun() { return hitstun; }
double Fighter::getDamagePercent() { return percentage; }
void Fighter::takeDamage(Circle other, int mult, int hitter, int charge) {
	if(action != STUN) stun();
	percentage += other.damage + (int)((charge/225) * (.5*other.damage));
	if(effectwait <= 0) {	
		if(other.fx == FX_NONE) {		
			if(other.damage + (int)((charge/225) * (.5*other.damage)) <= 6/3) {
				AS_SoundQuickPlay(hit1);
				display -> addeffect(Effect(x, y, FX_WEAKERHIT, charnum, display->scrollx, display->scrolly));
			}
			else if(other.damage + (int)((charge/225) * (.5*other.damage)) > 6/3 && other.damage + (int)((charge/225) * (.5*other.damage)) <= 12/3) {
				AS_SoundQuickPlay(hit2);
				display -> addeffect(Effect(x, y, FX_WEAKHIT, charnum, display->scrollx, display->scrolly));
			}
			else {
				AS_SoundQuickPlay(hit3);
				display -> addeffect(Effect(x, y, FX_STRONGHIT, charnum, display->scrollx, display->scrolly));
			}
		}
		else {
			display -> addeffect(Effect(x, y, other.fx, charnum, display->scrollx, display->scrolly));
			// play sound based on effect
		}
		effectwait = 15;
	}
	k = other.getKnockback();
	hitstun = (int) (k.length * 3 * (1+(percentage/100)));
	kx = (1+(percentage/100)) * k.dx * mult;
	ky = (1+(percentage/100)) * k.dy;
	dx = dy = DI = fastfall = 0;
	CAPE = false;
	lasthitby = hitter;
}
Fighter* Fighter::checkHits(Fighter* other) {
	if(getAtkbox().hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
		if(action == HOLD || action == GRABATK) {}
		else if(action == GRAB) {
			if(direction == "right") other -> grabbed((int)(x+handx), (int)y);
			else other -> grabbed((int)(x-handx), (int)y);
			other -> grabbedby = this;
			grabbedenemy = other;
			hold();
		}
		else if(other -> action == AIRDODGE || other -> action == ROLL || other -> action == DODGE) { /*doesn't hit*/ }
		else if(other -> action == SHIELD) {
			other -> shieldstr -= getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage + (int)((chargecount/225) * (.5*getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage));
		}
		else if(other -> COUNTER) {
			if(direction == "left") takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, other -> charnum, chargecount);
			else takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, other -> charnum, chargecount);
			other -> COUNTER = false;
			other -> idle();
		}
		else {
			if(direction == "left") other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, charnum, chargecount);
			else other -> takeDamage(getAtkbox().getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), -1, charnum, chargecount);
		}
	}
	return other;
}
Hitbox Fighter::getAtkbox() { 
	Hitbox temp = allatkbox[PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM)]; 
	Hitbox atkbox;
	vector<Circle> circles = temp.getCircles();
	for(uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircleft(current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		Circle newcircright((64 - current.getX()) + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		if(direction == "left") atkbox.addCircle(newcircleft);
		else atkbox.addCircle(newcircright);
	}
	return atkbox;
}
Hitbox Fighter::getDefbox(int framenum) { 
	Hitbox temp = alldefbox[framenum];
	Hitbox defbox;
	vector<Circle> circles = temp.getCircles();
	for(uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircright(current.getX() + x, current.getY() + y, current.getRadius());
		Circle newcircleft(x + (64 - current.getX()), y + (current.getY()), current.getRadius());
		if(direction == "left") defbox.addCircle(newcircleft);
		else defbox.addCircle(newcircright);
	}
	return defbox;
}
Circle Fighter::createAtkbox(double extrax, double extray, double radius, Knockback k, int damage) {
	Circle temp(64 - extrax,  extray, radius, k, damage);
	return temp;
}
void Fighter::airAttack() {
	if(Pad.Held.Up) uair();
	else if(Pad.Held.Down) dair();
	else if((Pad.Held.Right && direction == "right") || (Pad.Held.Left && direction == "left")) fair();
	else if((Pad.Held.Right && direction == "left") || (Pad.Held.Left && direction == "right")) bair();
	else nair();
}
void Fighter::airAttackStylus() {
	int touchx = Stylus.X;
	int touchy = Stylus.Y;
	if(touchy < 64 && touchx > 96 && touchx < 160) uair();
	else if(touchy > 128 && touchx > 96 && touchx < 160) dair();
	else if((touchx > 192 && touchy < 128 && touchy > 64 && direction == "right") || (touchx < 64 && touchy < 128 && touchy > 64 && direction == "left")) fair();
	else if((touchx > 192 && touchy < 128 && touchy > 64 && direction == "left") || (touchx < 64 && touchy < 128 && touchy > 64 && direction == "right")) bair();
	else nair();
}
void Fighter::smashAttack() {
	int touchx = Stylus.X; // xpos of touch
	int touchy = Stylus.Y; // ypos of touch
	if(touchx < 64 && touchy < 128 && touchy > 64) smashleft();
	if(touchx > 192 && touchy < 128 && touchy > 64) smashright();
	if(touchy < 64 && touchx > 96 && touchx < 160) smashup();
	if(touchy > 128 && touchx > 96 && touchx < 160) smashdown();
}
void Fighter::move() {
	if(action == HANG) { return; }
	if(action == GRABBED) {}
	else if(!aerial) {
		if(!checkFloorCollision()) {
			fall();
			jumpcount++;
		}
	}
	else {
		if(!isCPU) directionalInfluence();
		if(action == AIRATTACK) {
			fastfall = 0;
		}
		if(action == AIRDODGE || action == FTHROW || action == DTHROW || action == UTHROW || action == BTHROW) {
			fastfall = 0;
			DI = 0;
			ymomentum = 0;
		}
		checkLedgeCollision();
		checkFloorCollision();
		checkWallCollision();
		checkCeilingCollision();
	}
	x += dx;
	y += dy;
	if(momentumtime > 0) {
		momentumtime--;
		y += ymomentum;
		if(momentumtime == 0) {
			ymomentum = 0;
		}
	}
	if(action == ATTACK) jaywalk();
	if(aerial) y += gravity;
	if(aerial) y += fastfall;
	if(aerial) x += DI;
	if(checkForDeath()) respawn();
	PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x, (int)y); // repositions the sprite
} // moves the sprite
void Fighter::jaywalk() {}
void Fighter::setDirection(string rl) {
	string olddirection = direction;
	if(action == STUN) {
		if(kx > 0) direction = "right";
		else if(kx < 0) direction = "left";
	}
	if(rl == "") {
		if(Pad.Held.Right) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			direction = "right";
		}
		if(Pad.Held.Left) {
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			direction = "left";
		}
	}
	else {
		direction = rl;
		if(direction == "right") PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
		if(direction == "left") PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
	}
} // flips the direction of the sprite if necessary
void Fighter::directionalInfluence(int dx) {
	if(dx != 0) {
		DI = dx;
		fastfall = 0;
	}
	else {
		if(Pad.Held.Right) DI = 1;
		if(Pad.Held.Left) DI = -1;
		if(Pad.Held.Down) fastfall = 2;
		else fastfall = 0;
		if(!Pad.Held.Right && !Pad.Held.Left || action == BDOWN) DI = 0;

		//			if(Pad.Held.Down && (action != JUMP && action != DOUBLEJUMP)) fastfall = 1;
		//			if(Pad.Held.Up) fastfall = 0;
		// slightly influences direction
	}
} // acts in the air based on key presses
bool Fighter::checkForDeath() {
	if(x > stage->rightdeath || x+64 < stage->leftdeath || y > stage->bottomdeath || y+64 < stage->topdeath) {
		display->score->addDeath(lasthitby, charnum);
		int deathx = -64, deathy = -64;
		if(x > stage->rightdeath) {
			deathx = 256-64+(int)display->scrollx+10;
			deathy = (int)y;
		} // died off of right side
		else if(x+64 < stage->leftdeath) {
			deathx = 0+(int)display->scrollx-10;
			deathy = (int)y;
		} // died off of left side
		else if(y > stage->bottomdeath) {
			deathx = (int)x;
			deathy = 192-64+(int)display->scrolly+10;
		} // died off of bottom
		else if(y+64 < stage->topdeath) {
			deathx = (int)x;
			deathy = 0+(int)display->scrolly-10;
		} // died off top
		display -> addeffect(Effect(deathx, deathy, FX_DEATH, charnum, display->scrollx, display->scrolly));
		AS_SoundQuickPlay(deathsound);
		return true;
	}
	return false;
}
void Fighter::respawn() {
	lasthitby = -1;
	x = startx;
	y = starty;
	action = FALL;
	fall();
	direction = "";
	aerial = true;
	delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
	dx = dy = fastfall = DI = 0.0;
	percentage = 0;
	shieldstr = 64;
}
bool Fighter::ledgenotinuse(int lnum) {
	for(int n = 0; n < (int)players.size(); n++) {
		if(players[n] -> myledge == lnum) return false;
	}
	return true;
}
bool Fighter::checkLedgeCollision() {
	vector<Ledge> ledges = stage->getLedges();
	for(int n = 0; n < (int)ledges.size(); n++) {
		Ledge currledge = ledges[n];
		if(action != STUN && action != HANG) {
			if(currledge.direction == "right") {
				if(ledgenotinuse(n) && ledgewait <= 0 && x+leftside > currledge.x - 20 && x+leftside < currledge.x + 20 && y > currledge.y - 20 && y < currledge.y + 20) {
					hang();
					aerial = false;
					dx = DI = dy = fastfall = ymomentum = 0;
					airdodgecount = jumpcount = 0;
					x = currledge.x-handx;
					y = currledge.y-handy;
					setDirection("left");
					myledge = n;
					return true;
				}
			}
			else {
				if(ledgenotinuse(n) && ledgewait <= 0 && x+rightside > currledge.x - 20 && x+rightside < currledge.x + 20 && y > currledge.y - 20 && y < currledge.y + 20) {
					hang();
					aerial = false;
					dx = DI = dy = fastfall = ymomentum = 0;
					airdodgecount = jumpcount = 0;
					x = currledge.x+handx-64;
					y = currledge.y-handy;
					setDirection("right");
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
	for(uint8 n = 0; n < floors.size(); n++) {
		Floor currfloor = floors[n];
		double rise;
		if(currfloor.slopes.size() == 0) rise = 0;
		else rise = currfloor.getrise((int)x);
#ifdef DEBUG_ON
		PA_OutputText(MAIN_SCREEN, 0, 1, "Rise: %d", (int)(rise));
#endif
		if(aerial) {
			if(!(Pad.Held.Down && currfloor.isPlatform()) && dy+ymomentum+gravity > 0 && y+bottomside-rise <= currfloor.y && y+bottomside-rise + gravity + fastfall + dy + ymomentum > currfloor.y && x+rightside + dx + DI > currfloor.x && x+leftside + dx + DI < currfloor.x + currfloor.length) {
				aerial = false;
				y = currfloor.y-bottomside+rise;
				dy = DI = fastfall = ymomentum = 0;
				airdodgecount = 0;
				jumpcount = 0;
				return true;
			} // if you land on the floor
		} // checks for landing 
		else {
			if(!(Pad.Held.Down && currfloor.isPlatform())) {
				if(x+rightside + dx >= currfloor.x && x+leftside + dx < currfloor.x + currfloor.length) {
					if(((y+bottomside >= currfloor.y-currfloor.totalrise() && y+bottomside <= currfloor.y+currfloor.totalrise() && currfloor.totalrise() >= 0) 
								|| (y+bottomside <= currfloor.y-currfloor.totalrise() && y+bottomside >= currfloor.y+currfloor.totalrise() && currfloor.totalrise() < 0))) {
						y = currfloor.y-bottomside+rise;
						return true;
					} // stays on the ledge
				}
			}
		} // checks for falling off
	}
	return false;
}
bool Fighter::checkWallCollision() {
	vector<Wall> walls = stage->getWalls();
	for(uint8 n = 0; n < walls.size(); n++) {
		Wall currwall = walls[n];
		if(currwall.direction == "left") {
			if(x+rightside <= currwall.x && x+rightside + DI + dx > currwall.x && y+bottomside > currwall.y+topside && y < currwall.y + currwall.length) {
				x = currwall.x-rightside;
				dx = DI = 0;
				return true;
			}
		}
		else {
			if(x+leftside >= currwall.x && x+leftside + DI + dx < currwall.x && y+bottomside > currwall.y+topside && y < currwall.y + currwall.length) {
				x = currwall.x-leftside;
				dx = DI = 0;
				return true;
			}
		}
	}
	return false;
}
bool Fighter::checkCeilingCollision() {
	vector<Ceiling> ceilings = stage->getCeilings();
	for(uint8 n = 0; n < ceilings.size(); n++) {
		Ceiling currceil = ceilings[n];
		if(y+topside >= currceil.y && y+topside+gravity+fastfall+dy+ymomentum < currceil.y && x+rightside+dx+DI > currceil.x && x+leftside+dx+DI < currceil.x+currceil.length) {
			y = currceil.y-topside-dy-ymomentum;
			return true;
		}
	}
	return false;
}
void Fighter::scroll(double scrollx, double scrolly) {
	if(x - scrollx > 256 || x - scrollx < 0-64 || y - scrolly > 192 || y - scrolly < 0-64) {
		PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, -64, -64);
		PA_SetSpriteXY(MAIN_SCREEN, 30+(SPRITENUM-100), -64, -64);
	}
	else {
		PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x - (int)(scrollx), (int)y - (int)(scrolly));
		if(action == SHIELD) PA_SetSpriteXY(MAIN_SCREEN, 30+(SPRITENUM-100), (int)x - (int)(scrollx), (int)y - (int)(scrolly));
		else PA_SetSpriteXY(MAIN_SCREEN, 30+(SPRITENUM-100), -64, -64);
	}
}
Fighter::~Fighter() {}

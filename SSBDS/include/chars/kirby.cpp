class Kirby: public Fighter {
	int rockcount;
	public:	// constructor
		Kirby(int xpos, int ypos, int num, bool AI = false) {
			myledge = -1;
			acceleration = 0;
			runspeed = 3.5;
			x = xpos;
			y = ypos;
			hangtime = 0;
			ledgewait = 0;
			handx = 24;
			handy = 18;
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = KIRBY;
			chargecount = 0;
			isCPU = AI;
			lasthitby = -1;
			topside = 22;
			bottomside = 44;
			rightside = 41;
			leftside = 21;
			rockcount = 0;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 6;
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
			name = "kirby";
			initAtkbox();
			initDefbox();
			initSprite();
			initFrames();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
	// initializers
		void initFrames() {
			//LAND
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(10);

			// SHIELD
			startframes.push_back(26);
			endframes.push_back(26);
			framespeeds.push_back(20);

			// ROLL
			startframes.push_back(27);
			endframes.push_back(37);
			framespeeds.push_back(20);

			// DODGE
			startframes.push_back(38);
			endframes.push_back(45);
			framespeeds.push_back(20);

			// AIRDODGE
			startframes.push_back(46);
			endframes.push_back(53);
			framespeeds.push_back(20);

			// CROUCH
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(20);

			// FALL
			startframes.push_back(25);
			endframes.push_back(25);
			framespeeds.push_back(20);

			// IDLE
			startframes.push_back(1);
			endframes.push_back(4);
			framespeeds.push_back(10);

			// RUN
			startframes.push_back(5);
			endframes.push_back(12);
			framespeeds.push_back(20);

			// SHORTHOP
			startframes.push_back(13);
			endframes.push_back(19);
			framespeeds.push_back(20);

			// JUMP
			startframes.push_back(13);
			endframes.push_back(19);
			framespeeds.push_back(20);

			// DOUBLEJUMP
			startframes.push_back(20);
			endframes.push_back(24);
			framespeeds.push_back(12);

			// JAB
			startframes.push_back(60);
			endframes.push_back(65);
			framespeeds.push_back(20);

			// DASHATTACK
			startframes.push_back(157);
			endframes.push_back(163);
			framespeeds.push_back(20);

			// FTILT
			startframes.push_back(66);
			endframes.push_back(73);
			framespeeds.push_back(20);

			// UTILT
			startframes.push_back(84);
			endframes.push_back(88);
			framespeeds.push_back(20);

			// DTILT
			startframes.push_back(99);
			endframes.push_back(102);
			framespeeds.push_back(20);

			// CHARGELEFT
			startframes.push_back(74);
			endframes.push_back(75);
			framespeeds.push_back(20);

			// CHARGERIGHT
			startframes.push_back(74);
			endframes.push_back(75);
			framespeeds.push_back(20);

			// CHARGEUP
			startframes.push_back(89);
			endframes.push_back(90);
			framespeeds.push_back(20);

			// CHARGEDOWN
			startframes.push_back(103);
			endframes.push_back(104);
			framespeeds.push_back(20);

			// SMASHLEFT
			startframes.push_back(76);
			endframes.push_back(83);
			framespeeds.push_back(12);

			// SMASHRIGHT
			startframes.push_back(76);
			endframes.push_back(83);
			framespeeds.push_back(12);

			// SMASHUP
			startframes.push_back(91);
			endframes.push_back(98);
			framespeeds.push_back(20);

			// SMASHDOWN
			startframes.push_back(105);
			endframes.push_back(116);
			framespeeds.push_back(20);

			// FAIR
			startframes.push_back(124);
			endframes.push_back(131);
			framespeeds.push_back(20);

			// BAIR
			startframes.push_back(132);
			endframes.push_back(141);
			framespeeds.push_back(20);

			// UAIR
			startframes.push_back(142);
			endframes.push_back(146);
			framespeeds.push_back(20);

			// DAIR
			startframes.push_back(147);
			endframes.push_back(156);
			framespeeds.push_back(20);

			// NAIR
			startframes.push_back(117);
			endframes.push_back(123);
			framespeeds.push_back(20);

			// STUN
			startframes.push_back(164);
			endframes.push_back(171);
			framespeeds.push_back(20);

			// SLIDE
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(20);
			
			// HANG
			startframes.push_back(202);
			endframes.push_back(202);
			framespeeds.push_back(20);
			
			// GRABBED
			startframes.push_back(166);
			endframes.push_back(166);
			framespeeds.push_back(20);
			
			// GRAB
			startframes.push_back(205);
			endframes.push_back(208);
			framespeeds.push_back(20);
			
			// GRABATK
			startframes.push_back(208);
			endframes.push_back(211);
			framespeeds.push_back(10);

			// DEAD
			startframes.push_back(233);
			endframes.push_back(233);
			framespeeds.push_back(20);		
		}
	// sounds
		void playsound(int sndnum) {
			if(sndnum == DOUBLEJUMP) AS_SoundQuickPlay(kirbydoublejump);
			if(sndnum == SMASHDOWN) AS_SoundQuickPlay(kirbydsmash);
			if(sndnum == SMASHLEFT || sndnum == SMASHRIGHT) AS_SoundQuickPlay(kirbyfsmash);
			if(sndnum == FTILT) AS_SoundQuickPlay(kirbyftilt);
			if(sndnum == JUMP) AS_SoundQuickPlay(kirbyjump);
			if(sndnum == SMASHUP) AS_SoundQuickPlay(kirbyusmash);
		}
	// actions
		void bside() {
			if(action != BSIDE) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 178, 186, 15, ANIM_ONESHOT);
#ifdef SFX_ON
				AS_SoundQuickPlay(kirbybside);
#endif
				delay = 60/15 * 9;
				setDirection();
				dx = 0;
				action = BSIDE;
			}
			else {
				if(delay == 1) {
					if(checkFloorCollision()) idle();
					else fall();
				}
			}
		}
		void bup() {
			if(action != BUP) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 190, 199, 20, ANIM_ONESHOT);
#ifdef SFX_ON
				AS_SoundQuickPlay(kirbybup1);
#endif
				aerial = true;
				delay = 60/20 * 10;
				dy = -6;
				dx = 0;
				fastfall = 0;
				DI = 0;
				setDirection();
				action = BUP;
			}
			else {
				if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 199) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 200, 200, 20, ANIM_LOOP, -1);
#ifdef SFX_ON
					AS_SoundQuickPlay(kirbybup2);
#endif
					delay = 60/20 * 1;
					dy = 4;
				}
				else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 200) {
					if(!checkFloorCollision()) {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 200, 200, 20, ANIM_LOOP, -1);
						delay = 60/20 * 1;
						dy = 4;
					}
					else {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 201, 201, 20, ANIM_LOOP, -1);
						delay = 60/20 * 1;
#ifdef PROJECTILES_ON
						int directionmodifier = 1;
						if(direction =="right") directionmodifier = -1;
						Hitbox tempbox;
						tempbox.addCircle(createAtkbox(36, 805%64, 22, Knockback((-1*directionmodifier), -.5, 8), 24));
						tempbox.addCircle(createAtkbox(30, 794%63, 22, Knockback((-1*directionmodifier), -.5, 8), 24));
						tempbox.addCircle(createAtkbox(47, 812%64, 10, Knockback((-1*directionmodifier), -.5, 8), 24));
						projectiles.push_back(Projectile(x, y, -5*directionmodifier, 0, 20, FINALCUTTER, charnum, tempbox, stage));
#endif
					}
				}
			}
		}
		void bdown() {
			if(action != BDOWN) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 187, 188, 20, ANIM_LOOP, -1);
				delay = 60/20 * 2;
				dy = -gravity;
				dx = 0;
				action = BDOWN;
				rockcount = 250;
			}
			else {
				if(rockcount > 0) rockcount--;
				if(rockcount == 0 || custom_action(ACTION_SPECIAL, PAD_NEWPRESS)) {
					rockcount = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 203, 204, 20, ANIM_LOOP, -1);
					delay = 60/20 * 2;
					aerial = false;
					dx = 0;
				}
				else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 204) {
					if(checkFloorCollision()) idle();
					else fall();
				}
				else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 188) {
					if(!checkFloorCollision()) {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 189, 189, 20, ANIM_LOOP, -1);
						delay = 60/20 * 1;
						dy = 5;
					}
					else {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 189, 189, 20, ANIM_LOOP, -1);
						delay = 60/20*1;
						aerial = false;
					}
				}
				else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 189) {
					if(!checkFloorCollision()) {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 189, 189, 20, ANIM_LOOP, -1);
						delay = 60/20 * 1;
						dy = 5;
					}
					else {
						PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 189, 189, 20, ANIM_LOOP, -1);
						delay = 60/20*1;
					}				
				}
			}
		}
		void bneut() {
			if(action != BNEUT) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 172, 172, 10, ANIM_LOOP, -1);
				delay = 60/10 * 1;
				setDirection();
				dx = 0;
				action = BNEUT;
			}
			else {
				if(custom_action(ACTION_SPECIAL, PAD_RELEASED)) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 176, 176, 10, ANIM_LOOP, -1);
					delay = 60/10 * 1;
				}
				else if(delay == 1 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 172 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 174)) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 173, 174, 20, ANIM_LOOP, -1);
					delay = 60/20 * 2;
				}
				else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 176) {
					if(checkFloorCollision()) idle();
					else fall();
				}
			}
		}
		void fthrow() {
			if(action != FTHROW) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 212, 218, 10, ANIM_LOOP, -1);
				playsound(FTHROW);
				delay = 60/10 * (218-212+1);
				action = FTHROW;
			}
			else if(delay <= 0) {
				int mult = -1;
				grabbedenemy -> k = Knockback(1, -2.5, 7);
				if(direction == "right") {
					mult = 1;
				}
				grabbedenemy -> hitstun = (int) (grabbedenemy -> k.length * 3 * (1+(grabbedenemy -> percentage/100)));
				grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
				grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
				grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
				grabbedenemy -> percentage += 6;
				grabbedenemy -> stun();
				grabbedenemy -> lasthitby = charnum;
				grabbedenemy = NULL;
				fall();
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 212) {
				dx = 0;
				dy = 0;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 213) {
				dy = -8-gravity;
				aerial = true;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 214) {
				dy = -4-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 215) {
				dy = 0-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 216) {
				dy = 4-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 217) {
				dy = 8-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 218) {
				dx = 0;
				dy = -6-gravity;
			}
			if(grabbedenemy != NULL) {
				if(direction == "right") grabbedenemy -> grabbed(x+handx+dx, y+dy);
				else grabbedenemy -> grabbed(x-handx+dx, y+dy);
			}	
		}
		void bthrow() {
			if(action != BTHROW) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 219, 223, 10, ANIM_LOOP, -1);
				playsound(BTHROW);
				delay = 60/10 * (223-219+1);
				action = BTHROW;
			}
			else if(delay <= 0) {
				int mult = 1;
				grabbedenemy -> k = Knockback(1.25, -2.5, 7);
				if(direction == "right") {
					mult = -1;
				}
				grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
				grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
				grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
				grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
				grabbedenemy -> percentage += 6;
				grabbedenemy -> stun();
				grabbedenemy -> lasthitby = charnum;
				grabbedenemy = NULL;
				fall();
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 219) {
				if(direction == "right") dx = -1.5;
				else dx = 1.5;
				dy = -8-gravity;
				aerial = true;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 220) {
				dy = 0-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 221) {
				dy = 8-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 222) {
				dy = 0;
				dx = 0;
				aerial = false;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 223) {
				dy = -4-gravity;
				if(direction == "right") dx = -1.5;
				else dx = 1.5;
				aerial = true;
			}
			if(grabbedenemy != NULL) {
				if(direction == "right") grabbedenemy -> grabbed(x+handx+dx, y+dy);
				else grabbedenemy -> grabbed(x-handx+dx, y+dy);
			}	
		}
		void uthrow() {
			if(action != UTHROW) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 223, 226, 10, ANIM_LOOP, -1);
				playsound(UTHROW);
				delay = 60/10 * (226-223+1);
				action = UTHROW;
			}		
			else if(delay <= 0) {
				int mult = -1;
				grabbedenemy -> k = Knockback(.5, -3, 7);
				if(direction == "right") {
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
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 223) {
				dx = 0;
				dy = 0;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 224) {
				dy = -20-gravity;
				aerial = true;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 225) {
				dy = 20-gravity;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 226) {
				aerial = false;
				dy = 0;
			}
			if(grabbedenemy != NULL) {
				if(direction == "right") grabbedenemy -> grabbed(x+handx+dx, y+dy);
				else grabbedenemy -> grabbed(x-handx+dx, y+dy);
			}	
		}
		void dthrow() {
			if(action != DTHROW) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 227, 228, 10, ANIM_LOOP, -1);
				playsound(DTHROW);
				delay = 60/10 * (228-227+1);
				action = DTHROW;
				if(direction == "right") dx = 2;
				else dx = -2;
			}		
			else if(delay == 1 && (PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 228 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 227)) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 229, 230, 10, ANIM_LOOP, -1);
				delay = 60/10 * (230-229+1) * 5;
				dx = 0;
			}
			else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 230) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 231, 232, 10, ANIM_LOOP, -1);
				delay = 60/10 * (232-231+1);
				if(direction == "right") dx = -2;
				else dx = 2;
			}
			else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 232) {
				int mult = -1;
				grabbedenemy -> k = Knockback(.5, -2, 12);
				if(direction == "right") {
					mult = 1;
				}
				grabbedenemy -> hitstun = (int)(grabbedenemy -> k.length*3 * (1+(grabbedenemy -> percentage/100)));
				grabbedenemy -> kx = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dx * mult;
				grabbedenemy -> ky = (1+(grabbedenemy -> percentage/100)) * grabbedenemy -> k.dy;
				grabbedenemy -> dx = grabbedenemy -> dy = grabbedenemy -> DI = grabbedenemy -> fastfall = 0;
				grabbedenemy -> percentage += 10;
				grabbedenemy -> stun();
				grabbedenemy -> lasthitby = charnum;
				grabbedenemy = NULL;
				idle();
			}
		}
		void jaywalk() {
			int frame = PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM);
			if(frame == 82 && direction == "right") x += 8;
			if(frame == 82 && direction == "left") x -= 8;
		}
};

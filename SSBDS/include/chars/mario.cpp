class Mario: public Fighter {
	int fluddcharge;
	public:
	// constructor
		Mario(int xpos, int ypos, int num, bool AI = false) {
			x = xpos;
			y = ypos;
			hangtime = 0;
			ledgewait = 0;
			handx = 29;
			handy = 19;
			fluddcharge = 0;
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = MARIO;
			chargecount = 0;
			isCPU = AI;
			lasthitby = -1;
			topside = 17;
			bottomside = 47;
			rightside = 39;
			leftside = 24;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 2;
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
			name = "mario";
			initAtkbox();
			initDefbox();
			initFrames();
			initSprite();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
	// initializers
		void playsound(int sndnum) {
			if(sndnum == JUMP) AS_SoundQuickPlay(mariojump);
			if(sndnum == DOUBLEJUMP) AS_SoundQuickPlay(mariodoublejump);
			if(sndnum == UTILT) AS_SoundQuickPlay(marioutilt);
			if(sndnum == DASHATTACK) AS_SoundQuickPlay(mariodashattack);
			if(sndnum == SMASHDOWN) AS_SoundQuickPlay(mariodsmash);
			if(sndnum == SMASHUP) AS_SoundQuickPlay(mariousmash);
		}
		void initFrames() {
			// LAND
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(10);

			// SHIELD
			startframes.push_back(31);
			endframes.push_back(31);
			framespeeds.push_back(20);

			// ROLL
			startframes.push_back(25);
			endframes.push_back(30);
			framespeeds.push_back(15);

			// DODGE
			startframes.push_back(84);
			endframes.push_back(86);
			framespeeds.push_back(3);
			// should be updown. fix in sprite

			// AIRDODGE
			startframes.push_back(128);
			endframes.push_back(131);
			framespeeds.push_back(12);

			// CROUCH
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(20);

			// FALL
			startframes.push_back(14);
			endframes.push_back(14);
			framespeeds.push_back(20);

			// IDLE
			startframes.push_back(1);
			endframes.push_back(4);
			framespeeds.push_back(10);

			// RUN
			startframes.push_back(5);
			endframes.push_back(8);
			framespeeds.push_back(15);

			// SHORTHOP
			startframes.push_back(10);
			endframes.push_back(14);
			framespeeds.push_back(12);

			// JUMP
			startframes.push_back(10);
			endframes.push_back(14);
			framespeeds.push_back(12);

			// DOUBLEJUMP
			startframes.push_back(16);
			endframes.push_back(23);
			framespeeds.push_back(20);

			// JAB
			startframes.push_back(35);
			endframes.push_back(46);
			framespeeds.push_back(12);

			// DASHATTACK
			startframes.push_back(64);
			endframes.push_back(64);
			framespeeds.push_back(4);

			// FTILT
			startframes.push_back(47);
			endframes.push_back(51);
			framespeeds.push_back(10);

			// UTILT
			startframes.push_back(58);
			endframes.push_back(62);
			framespeeds.push_back(10);

			// DTILT
			startframes.push_back(52);
			endframes.push_back(57);
			framespeeds.push_back(10);

			// CHARGELEFT
			startframes.push_back(94);
			endframes.push_back(95);
			framespeeds.push_back(20);

			// CHARGERIGHT
			startframes.push_back(94);
			endframes.push_back(95);
			framespeeds.push_back(20);

			// CHARGEUP
			startframes.push_back(102);
			endframes.push_back(102);
			framespeeds.push_back(20);

			// CHARGEDOWN
			startframes.push_back(111);
			endframes.push_back(112);
			framespeeds.push_back(20);

			// SMASHLEFT
			startframes.push_back(96);
			endframes.push_back(101);
			framespeeds.push_back(15);

			// SMASHRIGHT
			startframes.push_back(96);
			endframes.push_back(101);
			framespeeds.push_back(15);

			// SMASHUP
			startframes.push_back(104);
			endframes.push_back(110);
			framespeeds.push_back(15);

			// SMASHDOWN
			startframes.push_back(113);
			endframes.push_back(120);
			framespeeds.push_back(15);

			// FAIR
			startframes.push_back(75);
			endframes.push_back(83);
			framespeeds.push_back(10);

			// BAIR
			startframes.push_back(71);
			endframes.push_back(75);
			framespeeds.push_back(12);

			// UAIR
			startframes.push_back(67);
			endframes.push_back(71);
			framespeeds.push_back(15);

			// DAIR
			startframes.push_back(84);
			endframes.push_back(93);
			framespeeds.push_back(15);

			// NAIR
			startframes.push_back(66);
			endframes.push_back(66);
			framespeeds.push_back(4);

			// STUN
			startframes.push_back(121);
			endframes.push_back(126);
			framespeeds.push_back(20);

			// SLIDE
			startframes.push_back(10);
			endframes.push_back(10);
			framespeeds.push_back(20);
			
			// HANG
			startframes.push_back(24);
			endframes.push_back(24);
			framespeeds.push_back(20);
			
			// GRABBED
			startframes.push_back(121);
			endframes.push_back(121);
			framespeeds.push_back(20);
			
			// GRAB
			startframes.push_back(151);
			endframes.push_back(152);
			framespeeds.push_back(10);
			
			// GRABATK
			startframes.push_back(155);
			endframes.push_back(155);
			framespeeds.push_back(10);
			
			// FTHROW
			startframes.push_back(153);
			endframes.push_back(155);
			framespeeds.push_back(15);
			
			// BTHROW
			startframes.push_back(156);
			endframes.push_back(162);
			framespeeds.push_back(20);
			
			// UTHROW
			startframes.push_back(163);
			endframes.push_back(165);
			framespeeds.push_back(15);
			
			// DTHROW
			startframes.push_back(165);
			endframes.push_back(168);
			framespeeds.push_back(15);
			
			// DEAD
			startframes.push_back(169);
			endframes.push_back(169);
			framespeeds.push_back(20);
		}
	// actions
		void bside() {
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
		void bup() {
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
#ifdef SFX_ON
				AS_SoundQuickPlay(mariobup);
#endif
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
		void bdown() {
			if(action != BDOWN) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 149, 149, 20, ANIM_LOOP, -1);
				delay = 60/20 * 1;
				fluddcharge = 0;
				dx = 0;
				if(aerial) dy = -gravity/2;
				else dy = 0;
				action = BDOWN;
			}
			else if(Pad.Released.B && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 149) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 150, 150, 12, ANIM_LOOP, -1);
				delay = 60/12 * 1;
#ifdef PROJECTILES_ON
				int directionmodifier = 1;
				if(direction == "right") directionmodifier = -1;
				Hitbox tempbox;
				tempbox.addCircle(createAtkbox(58, 32, 5, Knockback((-1*directionmodifier), -.5, 6), 60));
				projectiles.push_back(Projectile(x, y-16, -5*directionmodifier, 0, 2*fluddcharge, FLUDDWATER, charnum, tempbox, stage));
#endif
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
		void bneut() {
			if(action != BNEUT) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 133, 12, ANIM_LOOP, -1);
				delay = 60/12 * 2;
				dx = 0;
				action = BNEUT;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 133 && delay == 1) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 134, 134, 10, ANIM_LOOP, -1);
				delay = 60/10 * 1;
#ifdef SFX_ON
				AS_SoundQuickPlay(mariobneut);
#endif
#ifdef PROJECTILES_ON
				int directionmodifier = 1;
				if(direction == "right") directionmodifier = -1;
				Hitbox tempbox;
				tempbox.addCircle(createAtkbox(32, 32, 6, Knockback(-.25*directionmodifier, 0, 6), 10));
				Projectile p = Projectile(x, y, -3*directionmodifier, 1.5, 90, FIREBALL, charnum, tempbox, stage);
				p.miny = stage.getFloors()[0].y-32;
				p.maxy = y;
				projectiles.push_back(p);
#endif
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
		void jaywalk() {}
};

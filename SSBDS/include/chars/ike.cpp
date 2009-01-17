class Ike: public Fighter {
	int quickdrawcharge;
	int eruptioncharge;
	public:
	// constructor
		Ike(int xpos, int ypos, int num, bool AI = false) {
			x = xpos;
			y = ypos;
			hangtime = 0;
			ledgewait = 0;
			handx = 41;
			handy = 8;
			eruptioncharge = 0;
			quickdrawcharge = 0;
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = IKE;
			chargecount = 0;
			isCPU = AI;
			lasthitby = -1;
			topside = 15;
			bottomside = 47;
			rightside = 37;
			leftside = 13;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 2;
			charnum = players.size();
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
			grabtimeleft = 0;
			name = "ike";
			initAtkbox();
			initDefbox();
			initFrames();
			initSprite();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
	// initializers
		void playsound(int sndnum) {
			if(sndnum == BAIR) AS_SoundQuickPlay(ikebair);
			if(sndnum == DASHATTACK) AS_SoundQuickPlay(ikedashattack);
			if(sndnum == FAIR) AS_SoundQuickPlay(ikefair);
			if(sndnum == SMASHLEFT || sndnum == SMASHRIGHT) AS_SoundQuickPlay(ikefsmash);
			if(sndnum == FTILT) AS_SoundQuickPlay(ikeftilt);
			if(sndnum == JUMP) AS_SoundQuickPlay(ikejump);
			if(sndnum == UTILT) AS_SoundQuickPlay(ikeutilt);
		}
		void initFrames() {
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

			// FTHROW
			startframes.push_back(152);
			endframes.push_back(155);
			framespeeds.push_back(20);

			// BTHROW
			startframes.push_back(156);
			endframes.push_back(159);
			framespeeds.push_back(20);

			// UTHROW
			startframes.push_back(160);
			endframes.push_back(163);
			framespeeds.push_back(20);

			// DTHROW
			startframes.push_back(165);
			endframes.push_back(168);
			framespeeds.push_back(12);

			// DEAD
			startframes.push_back(169);
			endframes.push_back(169);
			framespeeds.push_back(20);
		}
	// actions
		void bside() {
			if(action != "bside") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 108, 108, 12, ANIM_LOOP, -1);
				delay = 60/12 * 1;
				setDirection();
				dx = 0;
				if(aerial) dy = -gravity/2;
				else dy = 0;
				action = "bside";
				quickdrawcharge = 0;
			}
			else if(Pad.Released.B) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 110, 110, 12, ANIM_LOOP, -1);
#ifdef SFX_ON
				AS_SoundQuickPlay(ikebside);
#endif						
				delay = quickdrawcharge/2;
				if(direction == "right") dx = 2*(int)(quickdrawcharge / 20) + 4;
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
		void bup() {
			if(action != "bup") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 118, 122, 15, ANIM_LOOP, -1);
				delay = 60/15 * 5;
				setDirection();
				dx = 0;
				if(aerial) dy = -gravity;
				else dy = 0;
				action = "bup";
				int directionmodifier = 1;
				if(direction == "right") directionmodifier = -1;
				Hitbox tempbox;
				tempbox.addCircle(Circle(32, 32, 15, Knockback(0, 0, 2), 9));
				Projectile p = Projectile(x, y, -.1*directionmodifier, -4, 30, IKESWORD, charnum, tempbox, stage);
				p.miny = y - 120;
				p.maxy = y;
				projectiles.push_back(p);
			}
			else if(delay == 1 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 127, 127, 15, ANIM_LOOP, -1);
#ifdef SFX_ON
				AS_SoundQuickPlay(ikebup);
#endif						
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
		void bdown() {
			if(action != "bdown") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 139, 142, 10, ANIM_UPDOWN, 5);
#ifdef SFX_ON
				AS_SoundQuickPlay(ikebdown);
#endif		
				delay = 60/10 * 5;
				setDirection();
				dx = 0;
				if(aerial) dy = -gravity/2;
				else dy = 0;
				action = "bdown";
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
		void bneut() {
			if(action != "bneut") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 99, 15, ANIM_LOOP, -1);
				delay = 60/15 * 1;
				setDirection();
				dx = 0;
				if(aerial) dy = -gravity/2;
				else dy = 0;
				action = "bneut";
				eruptioncharge = 0;				
			}
			else if((Pad.Released.B && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99) || (eruptioncharge == 300 && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 99)) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 100, 109, 10, ANIM_LOOP, -1);
#ifdef SFX_ON
				AS_SoundQuickPlay(ikebneut);
#endif		
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
		void jaywalk() {}
};

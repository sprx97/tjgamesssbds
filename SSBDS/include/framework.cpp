static const int FX_NONE = -1, FX_WEAKERHIT = 0, FX_WEAKHIT = 1, FX_STRONGHIT = 2, FX_AIRJUMP = 3, FX_DEATH = 4;
// shortcuts for visual effects
class Knockback {
	public:
		double dx; // x knockback pixels per frame
		double dy; // y knockback pixels per frame
		double length; // how many frames the knockback lasts
		Knockback() {} // blank init
		Knockback(double x, double y, double l) {
			dx = x*2;
			dy = y*2;
			length = l;
		} // new knockback, dx, dy, and length for it
		void set(double x, double y, double l) {
			dx = x*2;
			dy = y*2;
			length = l;
		} // sets the knockback's dx, dy, and length
}; // a knockback
class Circle {
	double radius; // radius of the circle
	Knockback k; // what the knockback of the circle is
	public:
		double x, y; // x and y of center of the circle
		double damage; // how much damage the circle deals
		int fx; // what visual effect type
		Circle(double xpos, double ypos, double r, int f = FX_NONE) {
			fx = f;
			x = xpos;
			y = ypos;
			radius = r;
			damage = 0;
			setKnockback(0,0,0);
		} // creates a circle with no knockback and damge -> Defboxes
		Circle(double xpos, double ypos, double r, Knockback knock, double d, int f = FX_NONE) {
			fx = f;
			x = xpos;
			y = ypos;
			radius = r;
			damage = (d/3.0);
			k = knock;
		} // creates a circle with knockback and damage -> Atkboxes
		double getX() { return x; }
		double getY() { return y; }
		double getRadius() { return radius; }
		// gets values for the circle
		bool intersects(Circle other) {
			double dx = other.getX() - x; // change in x
			double dy = other.getY() - y; // change in y
			double dist = sqrt(dx*dx + dy*dy); 
			return dist < (radius+other.getRadius());
		} // checks if this circle intersects Circle other
		void setKnockback(double kx, double ky, double kl) { k.set(kx, ky, kl); }
		// changes the knockback
		Knockback getKnockback() { return k; } // gets the knockback
}; // a circle; used for collisions; deals damage
class Hitbox {
	vector<Circle> circles; // Circles in the hitbox
	int contact; // which circle in circles made contact
	public:
		void addCircle(Circle toadd) { circles.push_back(toadd); }
		// adds a circle
		vector<Circle> getCircles() { return circles; }
		// gets the circles
		bool hits(Hitbox other) {
			for(uint8 n = 0; n < circles.size(); n++) {
				for(uint8 m = 0; m < other.getCircles().size(); m++) {
					if(circles[n].intersects(other.getCircles()[m])) {
						contact = n;
						return true;
					}
				}
			}
			return false;
		}
		// checks if this hitbox intersects hitbox other
		Circle getHitCircle(Hitbox other) {
			for(uint8 n = 0; n < circles.size(); n++) {
				for(uint8 m = 0; m < other.getCircles().size(); m++) {
					if(circles[n].intersects(other.getCircles()[m])) {
						return circles[n];
					}
				} 
			}
			return circles[contact];
		}
		// checks which circle hit hibox other
		void reset() { circles.clear(); } // resets the hirbox
}; // a collection of circles; used to hold all the circles for one frame
class Effect {
	public:
		double x, y; // x and y pos of the effect
		int mynum; // the reference number of the effect
		int delay; // how long the effect waits before changing
		int playernum; // which player the effect is created by
		int type; // what effect type it is
		Effect(double xpos, double ypos, int t, int pn) {
			x = xpos, y = ypos;
			type = t;
			playernum = pn;
			mynum = 5*playernum + 5;
			if(type == FX_AIRJUMP) mynum+=1;
			else if(type == FX_DEATH) mynum+=2;
			else { // all knockback sprites are the same spritenum
				mynum+=3;
			} // creates the effect... avoids overriding multipe sprites
			// different sprite for different types of effects
			if(x-scrollx < 256 && x-scrollx > 0-64 && y-scrolly < 192 && y-scrolly > 0-64) {
				PA_SetSpriteXY(MAIN_SCREEN, mynum, (int)(x-scrollx), (int)(y-scrolly));
			}
			// sets the position of the sprite on the screen
			if(type == FX_WEAKERHIT) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 0, 2, 12, ANIM_ONESHOT);
				delay = 60/12 * 3;
			} // sets animation for a weaker hit
			else if(type == FX_WEAKHIT) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 3, 5, 12, ANIM_ONESHOT);
				delay = 60/12 * 3;
			} // sets animation for a weak hit
			else if(type == FX_STRONGHIT) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 6, 8, 12, ANIM_ONESHOT);
				delay = 60/12 * 3;
			} // sets animation for a strong hit
			else if(type == FX_AIRJUMP) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 9, 10, 15, ANIM_ONESHOT);
				delay = 60/15 * 2;
			} // sets animation for air jump
			else if(type == FX_DEATH) {
				if(x-scrollx == 0-10) {
					PA_SetSpriteHflip(MAIN_SCREEN, mynum, true);
				}
				if(x-scrollx == 256-64+10) {
					PA_SetSpriteHflip(MAIN_SCREEN, mynum, false);
				}
				if(y-scrolly == 0-10) {
					PA_SetRotsetNoZoom(MAIN_SCREEN, mynum, 128);
					PA_SetSpriteRotEnable(MAIN_SCREEN, mynum, mynum);
				}
				if(y-scrolly == 192-64+10) {
					PA_SetRotsetNoZoom(MAIN_SCREEN, mynum, -128);
					PA_SetSpriteRotEnable(MAIN_SCREEN, mynum, mynum);
				}
				PA_StartSpriteAnimEx(MAIN_SCREEN, mynum, 11, 17, 10, ANIM_ONESHOT);
				delay = 60/10 * 7;
			}
			// sets animation for a death
			// else if...
			// else if...
		} // creates a new effect
		void act() {
			delay--;
			if(delay <= 0) {
				vector<Effect> temp;
				for(int n = 0; n < (int)effects.size(); n++) {
					Effect e = effects[n];
					if(e.x != x || e.y != y || e.delay != delay || e.playernum != playernum) {
						temp.push_back(e);
					}
				} // keeps the effect
				PA_SetSpriteRotDisable(MAIN_SCREEN, mynum);
				PA_SetSpriteXY(MAIN_SCREEN, mynum, -64, -64); // hides sprite
				effects = temp;
			} // removes once animation is done
		} // acts; called every frame
}; // a visual effect

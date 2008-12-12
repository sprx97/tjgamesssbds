// Game designed by TJgames of TJHSST
// Head Developer: Jeremy Vercillo
// Assistant Developer(s): Tyler Haines, Daniel Johnson
// 6/08 - ???

// The rights to Super Smash Bros. and all the related characters, stages, and items
// belong to Nintendo and other 3rd party companies. This is a fan made game; it not
// intended for profit, just for fun

/* * * * * * To Do (no specific order) * * * * * *
- Use cursors for character selection
- Make a better (fullscreen) minimap
	- use char faces instead of dots
	- and better smashing buttons (or remove them completely)
	- and better damage icons
- MP3s
- LAN
- remove excess sprites from stips (decrease size)
- Sloped stage floors (and walls and ceilings)
- Fix stage collisions ... make more accurate stage maps
- balace
- menu screen redesign
- pause menu
- more game modes (Training, stock match, LAN, options, extras)
- allow for rule changing on characterSelect Screen
- fix ike sprites (sword tip in previous frame sometimes)
- AI (Dan J)
- Better menus (Tyler)
- Clear players vector properly -- REALLY IMPORTANT
- customizable controls
- Add more characters and stages
- invincibility frames
- shields
* * * * * * * * * * * * * * * * * * * * * * * * * */

#define DEBUG_ON
// turns on printing of information to screen

#define SFX_ON
#define PROJECTILES_ON
//#define SANDBAG_ON
//#define SHADOW_ON
//#define SLOPEDSTAGES_ON // Castle Seige and Corneria
//#define LAN_ON // REMEMBER TO CHANGE MAKEFILE TOO!!!!
//#define MP3_ON
// turns certain features on and off

// Includes
#include <PA9.h> // DS functions that we use come from here
#ifdef LAN_ON
#include <MessageQueue.h>
#include <802.11.h>
#include <lobby.h> // DS <-> DS!
#endif
// LAN functions
#include <math.h> // math!
#include <vector> // vectors!
#include <string> // strings!
#include <sstream> // int to string
#include <stdio.h>
#include <stdlib.h>
// standard C libraries

#include "gfx/all_gfx.c"
// all image files

#ifdef SFX_ON
// include sound clips (just small ones, not MP3s)
#include "one.h"
#include "two.h"
#include "three.h"
#include "go.h"
#include "game.h"
#include "thewinneris.h"
#include "nocontest.h"
#include "kirby.h"
#include "mario.h"
#include "ike.h"
#include "free_for_all.h"
#include "menuconfirm.h"
#include "deathsound.h"
#include "hit1.h"
#include "hit2.h"
#include "hit3.h"

#include "kirbybup1.h"
#include "kirbybup2.h"
#include "kirbybside.h"
#include "kirbydoublejump.h"
#include "kirbydsmash.h"
#include "kirbyfsmash.h"
#include "kirbyftilt.h"
#include "kirbyjump.h"
#include "kirbyusmash.h"

#include "ikebair.h"
#include "ikebdown.h"
#include "ikebneut.h"
#include "ikebside.h"
#include "ikebup.h"
#include "ikedashattack.h"
#include "ikefair.h"
#include "ikefsmash.h"
#include "ikeftilt.h"
#include "ikejump.h"
#include "ikeutilt.h"

#include "mariojump.h"
#include "mariodoublejump.h"
#include "marioutilt.h"
#include "mariodashattack.h"
#include "mariojab.h"
#include "mariodsmash.h"
#include "mariousmash.h"
#include "mariobneut.h"
#include "mariobup.h"
// all sound effects
// combine into a all_sounds.c at some point.
#endif

using namespace std; // standard naming of variables

#define PI 3.1415926 // PI

#ifdef LAN_ON
#define GAMECODE 558 // lol SSB in 1337
#define GAMEVERSION 0 // use to avoid desynching b/c of changes
#define MAX_ROOMS 8
#define ROOM_NAMES 8
// variables for running a LAN game
#define MENU_MAIN 0 
#define MENU_CREATE_ROOM 1
// LAN Rooms
#endif

#define MAIN_SCREEN 1 // top screen
#define SUB_SCREEN 0 // bottom screen
#define COLOR256 1 // 256 color mode

#define CAMERATYPE_FOLLOWUSER 0 
#define CAMERATYPE_FOLLOWALL 1
int cameratype = CAMERATYPE_FOLLOWUSER;
// camera types

static const int SANDBAG = 0, KIRBY = 1, MEWTWO = 2, MARIO = 3, IKE = 4, SHADOW = 5; 
// character shortcuts; used to avoid confusion
static const int FINALDESTINATION = 1, POKEMONSTADIUM = 2, CASTLESEIGE = 3, CORNERIA = 4;
// stage shortcupts just like character shortcuts
static const int UPARR = 1, DOWNARR = 2, LEFTARR = 3, RIGHTARR = 4, P1MINI = 5, P2MINI = 6, P3MINI = 7, P4MINI = 8, MINIBOX = 9; 
// shortcuts for minimap sprites
static const int LAND = 0, SHIELD = 1, ROLL = 2, DODGE = 3, AIRDODGE = 4, CROUCH = 5, FALL = 6, IDLE = 7, RUN = 8, SHORTHOP = 9, JUMP = 10, DOUBLEJUMP = 11, JAB = 12, DASHATTACK = 13, FTILT = 14, UTILT = 15, DTILT = 16, CHARGELEFT = 17, CHARGERIGHT = 18, CHARGEUP = 19, CHARGEDOWN = 20, SMASHLEFT = 21, SMASHRIGHT = 22, SMASHUP = 23, SMASHDOWN = 24, FAIR = 25, BAIR = 26, UAIR = 27, DAIR = 28, NAIR = 29, STUN = 30, SLIDE = 31, HANG = 32;
// shortcuts for actions
#ifdef SFX_ON
static const int FX_NONE = -1, FX_WEAKERHIT = 0, FX_WEAKHIT = 1, FX_STRONGHIT = 2, FX_AIRJUMP = 3, FX_DEATH = 4;
// shortcuts for visual effects
#endif
#ifdef PROJECTILES_ON
static const int SHADOWBALL_SMALL = 0, SHADOWBALL_MEDIUM = 1, SHADOWBALL_LARGE = 2, FINALCUTTER = 3, FIREBALL = 4, FLUDDWATER = 5, IKESWORD = 6;
// shortcuts for projectiles
#endif

#ifdef LAN_ON
bool canlan = true; //if the play is playing LAN
#endif

class Fighter;
vector<Fighter*> players;
// stores all fighters for playing a match

#ifdef PROJECTILES_ON
class Projectile;
vector<Projectile> projectiles;
// stores all projectiles
#endif
#ifdef SFX_ON
class Effect;
vector<Effect> effects;
// stores all visual effects
#endif
class Scoreboard; // keeps score of the game

double scrollx = 0;
double scrolly = 0;
// how far the screen is scrolled (for stages)

class Scoreboard {
	vector<int> kills; // player numbers of the kills (in order) -- -1 is a SD
	vector<int> deaths; // player numbers of the deaths (in order)
	public:
		int playercount; // total number of players
		Scoreboard(int count) {
			playercount = count;
		} // creates a scoreboard
		void addDeath(int killer, int killed) {
			kills.push_back(killer);
			deaths.push_back(killed);
		} // registers a kill
		void clear() {
			kills.clear();
			deaths.clear();
		} // clears the score
		int getKills(int player) {
			int count = 0;
			for(int n = 0; n < kills.size(); n++) {
				if(kills[n] == player) count++;
			}
			return count;
		} // the number of kills by player with number player
		int getDeaths(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(deaths[n] == player && kills[n] != -1) count++;
			}
			return count;
		} // the number of deaths by player with number player
		int getSDs(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(deaths[n] == player && kills[n] == -1) count++;
			}
			return count;
		} // the number of SDs by player with number player
		int getTotal(int player) {
			int count = 0;
			for(int n = 0; n < deaths.size(); n++) {
				if(kills[n] == player) count++;
				if(deaths[n] == player) count--;
			}
			return count;
		} // totals score for a player with number player
}; // tracks all scores
Scoreboard score = Scoreboard(0); // the scoreboard for smash
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
#ifdef SFX_ON
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
				PA_SetSpriteXY(MAIN_SCREEN, mynum, x-scrollx, y-scrolly);
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
				for(int n = 0; n < effects.size(); n++) {
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
#endif
class Ledge {
	public:
		int x, y; // x and y position of the ledge
		string direction;
		Ledge(int xpos, int ypos, string dir) {
			x = xpos;
			y = ypos;
			direction = dir;
		} // creates a new ledge
}; // a ledge on the stage
class Floor {
	public:
		int x, y; // x and y of far left of stage
		int length; // total run of the floor
		bool isplatform; // is a platform of not
		vector<double> slopes; // sloping of the stage for each x position
		Floor(int xpos, int ypos, int l, bool plat) {
			x = xpos;
			y = ypos;
			length = l;
			isplatform = plat;
		} // creates a new floor
		bool isPlatform() { return isplatform; } // returns isplatform
		double totalrise() {
			double total = 0;
			for(int n = 0; n < slopes.size(); n++) {
				total += slopes[n];
			}
			return total;
		} // counts the total rise of the stage from start to finish
		double getrise(int distance) {
			double total = 0;
			for(int n = x; n < distance; n++) {
				total += slopes[n-x];
			}
			return total;
		} // the rise up to distance distance
}; // a floor on the stage
class Ceiling {
	public:
		int x, y; // x and y position of ceiling
		int length; // length of ceiling
		Ceiling(int xpos, int ypos, int l) {
			x = xpos;
			y = ypos;
			length = l;
		} // creates a new ceiling
}; // a ceiling on the stage
class Wall {
	public:
		int x, y; // x and y position of the wall
		int length; // how long the wall is
		string direction; // what direction it is facing
		Wall(int xpos, int ypos, int l, string d) {
			x = xpos;
			y = ypos;
			length = l;
			direction = d;
		} // creates a new wall
}; // a wall on the stage
class Stage {
	public:
	// variables
		int width, height; // width and height of the image of the stage
		int leftdeath, rightdeath, topdeath, bottomdeath;
		// where a person dies of the edges of the stage
		string name; // name of the stage
		vector<Floor> floors; // floors of the stage
		vector<Wall> walls; // walls of the stage
		vector<Ceiling> ceilings; // ceilings of the stage
		vector<Ledge> ledges; // ledges of the stage
		vector<int> spawnx; // where the x coords of the spawn points are
		vector<int> spawny; // where the y coords of the spawn points are
	// methods
		virtual void Initialize() {} 
		// each subclass has it's own initialization code
		string getName() { return name; }
		vector<Floor> getFloors() { return floors; }
		vector<Wall> getWalls() { return walls; }
		vector<Ceiling> getCeilings() { return ceilings; }
		vector<Ledge> getLedges() { return ledges; }
		// gets stuff about the stage
}; // class which all stages extend
class FinalDestination: public Stage {
	public:
		FinalDestination() { Initialize(); }
		void Initialize() {
			name = "finaldestination";
			width = 512;
			height = 256;
			topdeath = 0-192;
			leftdeath = 0-128;
			rightdeath = 512+128;
			bottomdeath = 256;
			spawnx.push_back(512/2 -32 -64);
			spawnx.push_back(512/2 -32 +64);
			spawnx.push_back(512/2 -32 -64 -64);
			spawnx.push_back(512/2 -32 +64 +64);
			spawny.push_back(256/3 -32);
			spawny.push_back(256/3 -32);
			spawny.push_back(256/3 -32);
			spawny.push_back(256/3 -32);
			floors.push_back(Floor(64, 132, 512-64-64, false));
			walls.push_back(Wall(96, 132, 248-132, "left"));
			walls.push_back(Wall(512-96, 132, 248-132, "right"));
			ceilings.push_back(Ceiling(64, 132, 512-64-64));
			ledges.push_back(Ledge(64, 132, "left"));
			ledges.push_back(Ledge(512-64, 132, "right"));
		} // initializes all the variables for final destination
}; // the stage Final D estination
class PokemonStadium: public Stage { 
	public:
		PokemonStadium() { Initialize(); }
		void Initialize() {
			name = "pokemonstadium";
			width = 512;
			height = 256;
			topdeath = 0-192;
			bottomdeath = 256;
			leftdeath = 0-128;
			rightdeath = 512+128;
			spawnx.push_back(512/2 -32 -64);
			spawnx.push_back(512/2 -32 +64);
			spawnx.push_back(512/2 -32 -64 -64);
			spawnx.push_back(512/2 -32 +64 +64);
			spawny.push_back(256/3 -64);
			spawny.push_back(256/3 -64);
			spawny.push_back(256/3 -64);
			spawny.push_back(256/3 -64);
			floors.push_back(Floor(42, 154, 512-42-42, false));
			floors.push_back(Floor(128, 110, 190-128, true));
			floors.push_back(Floor(324, 110, 386-324, true));
			walls.push_back(Wall(50, 154, 216-154, "left"));
			walls.push_back(Wall(512-50, 154, 216-154, "right"));
			ceilings.push_back(Ceiling(42, 216, 512-42-42));
			ledges.push_back(Ledge(42, 154, "right"));
			ledges.push_back(Ledge(512-42, 154, "left"));
		} // initializes the pokemon stadium stage
}; // the stage Pokemon Stadium
#ifdef SLOPEDSTAGES_ON
class CastleSeige: public Stage {
	public:
		CastleSeige() { Initialize(); }
		void Initialize() {
			name = "castleseige";
			width = 512;
			height = 384;
			topdeath = 0-192;
			bottomdeath = 384;
			leftdeath = 0-128;
			rightdeath = 512+128;
			spawnx.push_back(512/2 -32 -64);
			spawnx.push_back(512/2 -32 +64);
			spawnx.push_back(512/2 -32 -64 -64);
			spawnx.push_back(512/2 -32 +64 +64);
			spawny.push_back(384/3 -64);
			spawny.push_back(384/3 -64);
			spawny.push_back(384/3 -64);
			spawny.push_back(384/3 -64);
			floors.push_back(Floor(40, 350-128, 450-40, false));
			for(int n = 40; n < 202; n++) {
				floors[0].slopes.push_back(0);
			}
			for(int n = 202; n < 300; n++) {
				floors[0].slopes.push_back(-.25);
			}
			for(int n = 300; n < 451; n++) {
				floors[0].slopes.push_back(0);
			}
			// this floor is sloped
			floors.push_back(Floor(84, 288-128, 194-84, true));
			floors.push_back(Floor(302, 270-128, 408-302, true));
			walls.push_back(Wall(48, 350-128, 100, "left"));
			walls.push_back(Wall(442, 350-128, 100, "right"));
		} // initializes Castle Seige
}; // the stage casle seige
class Corneria: public Stage {
	public:
		Corneria() { Initialize(); }
		void Initialize() {
			name = "corneria";
			width = 512; height = 256;
			topdeath = 0-192;
			bottomdeath = 256;
			leftdeath = 0-128;
			rightdeath = 512+128;
			spawnx.push_back(512/2 -32 -64);
			spawnx.push_back(512/2 -32 +64);
			spawnx.push_back(512/2 -32 -64 -64);
			spawnx.push_back(512/2 -32 -64 +64);
			spawny.push_back(256/3 -96);
			spawny.push_back(256/3 -96);
			spawny.push_back(256/3 -96);
			spawny.push_back(256/3 -96);
			floors.push_back(Floor(106, 386-256, 140-106, false));			
			floors.push_back(Floor(30, 330-256, 368-30, false));
			for(int n = 30; n < 90; n++) {
				floors[1].slopes.push_back(-.25);
			}
			for(int n = 90; n < 214; n++) {
				floors[1].slopes.push_back(.10);
			}
			for(int n = 214; n < 260; n++) {
				floors[1].slopes.push_back(-.21);
			}
			for(int n = 260; n < 295; n++) {
				floors[1].slopes.push_back(-.8);
			}
			for(int n = 295; n < 369; n++) {
				floors[1].slopes.push_back(-.4);
			}
			floors.push_back(Floor(362, 341-256, 484-362, false));
			for(int n = 362; n < 408; n++) {
				floors[2].slopes.push_back(0);
			}
			for(int n = 408; n < 441; n++) {
				floors[2].slopes.push_back(-.2);
			}
			for(int n = 441; n < 485; n++) {
				floors[2].slopes.push_back(.38);
			}
			// 2 floors are sloped
		} // initializes Corneria
}; // the stage corneria
#endif
class Fighter {
	public:
	// variables
		bool CAPE;
		bool COUNTER;
		int airdodgecount;
		int effectwait;
		int MYCHAR;
		int chargecount;
		int lasthitby;
		vector<Hitbox> allatkbox;
		vector<Hitbox> alldefbox;
		int leftside, rightside, topside, bottomside;
		bool isCPU;
		int SPRITENUM;
		double gravity;
		int jumpmax;
		double x, y, startx, starty;
		double dx, dy;
		string action, direction;
		bool aerial;
		int delay, jumpcount, startlag, landinglag, tiltlag, airlag, lcancel, hitstun;
		double fastfall, DI;
		Knockback k;
		double kx, ky;
		double percentage;
		Stage stage;
		double ymomentum;
		int momentumtime;
		int charnum;
		string name;
		vector<int> startframes;
		vector<int> endframes;
		vector<int> framespeeds;
	// virtual methods
#ifdef LAN_ON
		char* toDataString() {
//			string data = "";
//			data += SPRITENUM;
//			data += " ";
//			data += x;
//			data += " ";
//			data += y;
//			data += " ";
//			data += PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM);
//			data += " ";
//			data += lasthitby;
//			data += " ";
//			data += percentage;
//			data += " ";
//			char* d;
//			strcpy(d, data.c_str());
//			return d;
		}
		void setDataString(char * data) {
//			SPRITENUM = (int)(strtok((char*)data," "));
//			x = (int)(strtok((char*)data," "));
//			y = (int)(strtok((char*)data," "));
//			int frame = (int)(strtok((char*)data," "));
//			PA_SetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM, frame);
//			percentage = (int)(strtok((char*)data," "));
//			lasthitby = (int)(strtok((char*)data," "));
		}
#endif
		void initDefbox() {
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

				alldefbox[frame].addCircle(Circle(xpos, (int)(ypos)%64, radius));
			}
			fclose(file);
		}
		void initAtkbox() {
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
		void initSprite() {
			PA_FatEasyLoadSpritePal(MAIN_SCREEN, SPRITENUM-100, name.c_str());
			PA_FatLoadSprite(MYCHAR, name.c_str());
			PA_CreateSprite(MAIN_SCREEN, SPRITENUM, (void*)sprite_gfx[MYCHAR], OBJ_SIZE_64X64, COLOR256, SPRITENUM-100, -64, -64);
		}
		void deleteSprite() {
			allatkbox.clear();
			alldefbox.clear();
			PA_FatFreeSprite(MYCHAR);
			PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
		}
		virtual void actCPU() {
			int Cenemy = 0;
			double Cdistance = 10000000;
			double Cangle = 0;
			double Cx = 0;
			double Cy = 0;
			for(int n = 0; n < players.size(); n++) {
				if(players[n] -> charnum != charnum) {
					double dx = (players[n] -> x) - x;
					double dy = (players[n] -> y) - y;
					double distance = sqrt(dx*dx + dy*dy);
					if(distance < Cdistance) {
						Cenemy = n;
						Cdistance = distance;
						Cx = dx;
						Cy = dy;
						if(dx != 0 && dy == 0) {
							if(dx > 0) Cangle = 0;
							if(dx < 0) Cangle = 180;
						}
						else if(dy != 0 && dx == 0) {
							if(dy > 0) Cangle = 90;
							if(dy < 0) Cangle = -90;
						}
						else Cangle = atan2(dy, dx) * 180 / PI; // from -180 to 180
					}
				}
			}
			// finds disttance to it.
			if(action == "bside") bside();
			if(action == "bup") bup();
			if(action == "bdown") bdown();
			if(action == "bneut") bneut();
			if(hitstun > k.length*2) {
				if(y != stage.getFloors()[0].y ) aerial = true;
				hitstun--;
				dx = kx;
				dy = ky;
				if(hitstun == 0) {
					action == "hitstun";
					if(aerial) fall();
					else idle();
				}
				if(checkFloorCollision()) idle();
				if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
				else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
			}
			else if(hitstun > 0) {
				if(y != stage.getFloors()[0].y ) aerial = true;
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
					action = "hitstun";
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
				if(action == "jump" || action == "doublejump") {
					if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
					else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
					// act in air
				}
				if(action == "dodge" && delay <= 0) shield();
				if(action == "airdodge" && delay <= 0) fall();
				if(action == "roll" && delay <= 0) {
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
				if(action == "slide" && delay <= 0) idle();
				if(action == "slide") {
					if(dx > 0) {
						dx -= .25;
						if(dx <= 0) dx = 0;
					}
					else if(dx < 0) {
						dx += .25;
						if(dx >= 0) dx = 0;
					}
				}
				if(action == "jab") {
				
				}
				if(action == "attack") {
					if(delay <= 0) {
						idle();
					}
				}
				if(action == "dashattack") {
					x += dx;
					if(delay <= 0) idle();
				}
				if(action == "bside" || action == "bup" || action == "bdown" || action == "bneut") {
					if(delay <= 0) idle();
				}
				if(action == "airattack") {
					if(checkFloorCollision()) {
						if(delay > 0) land();
						else idle();
						delay = 0;
					}
					else if(delay <= 0) fall();
					if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
					else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
				} // checks for stage collision with aerial				
				if((action == "jump" || action == "doublejump") && delay <= 0) fall(); // falls when jump/multijump are finished animating
				if(action == "fall") {
					if(Cangle < 90 && Cangle > -90) directionalInfluence(1);
					else if(Cangle > 90 || Cangle < -90) directionalInfluence(-1);
					if(checkFloorCollision()) idle();
					else if(Cangle < -45 && Cangle > -135 && jumpcount < jumpmax) {
						if(Cx > 0) setDirection("right");
						if(Cx < 0) setDirection("left");
						doubleJump();
					}
				}
				if(aerial && action != "airattack" && action != "airlag") {
					// act air
				}
				if(action == "shield") {
					// idle or roll or dodge
				}
				if(action == "run") {
					if((Cdistance < 30 && Cdistance > -30) || (dx < 0 && (Cangle < 90 && Cangle > -90)) || (dx > 0 && (Cangle > 90 || Cangle < -90))) slide();					
					else if(Cangle < -45 && Cangle > -135 && jumpcount == 0) {
						if(Cx > 0) setDirection("right");
						if(Cx < 0) setDirection("left");
						jump();
					}
					// or dash attack
					// or run more
					// or slide
				}
				if(action == "idle" || action == "") {
					if(Cdistance < 30 && Cdistance > -30) {
						if(Cx > 0) setDirection("right");
						if(Cx < 0) setDirection("left");
						bside();
					}
					else if(Cangle < -45 && Cangle > -135 && jumpcount == 0) {
						if(Cx > 0) setDirection("right");
						if(Cx < 0) setDirection("left");
						jump();
					}
					else if(Cangle < 90 && Cangle > -90) run(1);
					else if(Cangle > 90 || Cangle < -90) run(-1);
				}
				if(action == "crouch") {
					// idle
					// or dtilt
					// or bdown
				}
			}
			move();
		}
		virtual void act() {
			if(effectwait > 0) effectwait--;
			if(isCPU) {
				actCPU();
				return;
			}
			if(action == "bside") bside();
			if(action == "bup") bup();
			if(action == "bdown") bdown();
			if(action == "bneut") bneut();
			if(action == "chargeleft" && (Stylus.Released || delay == 0)) smashleft();
			if(action == "chargeright" && (Stylus.Released || delay == 0)) smashright();
			if(action == "chargeup" && (Stylus.Released || delay == 0)) smashup();
			if(action == "chargedown" && (Stylus.Released || delay == 0)) smashdown();
			if(hitstun > k.length*2) {
				if(y != stage.getFloors()[0].y ) aerial = true;
				hitstun--;
				dx = kx;
				dy = ky;
				if(hitstun == 0) {
					action == "hitstun";
					if(aerial) fall();
					else idle();
				}
				if(checkFloorCollision()) idle();
			}
			else if(hitstun > 0) {
				if(y != stage.getFloors()[0].y ) aerial = true;
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
					action = "hitstun";
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
					if((Pad.Released.X || Pad.Released.Y || Pad.Released.Up)) {
						shorthop();
						tiltlag = 0;
					}
					else if(tiltlag == 0) {
						if(Pad.Held.A) {
							if(Pad.Held.Right || Pad.Held.Left) ftilt();
							else if(Pad.Held.Up) utilt();
							else jab();
						}
						else if(Pad.Held.B) {
							if(Pad.Held.Right || Pad.Held.Left) bside();
							else if(Pad.Held.Up) {
								bup();
							}
							else if(Pad.Held.Down) bdown();
							else bneut();
						}
						else if((Pad.Released.X || Pad.Released.Y || Pad.Released.Up) && jumpcount < jumpmax) shorthop();
						else if((Pad.Held.X || Pad.Held.Y || Pad.Held.Up) && jumpcount < jumpmax) jump();
						else if(Pad.Held.R || Pad.Held.L) {
							if(Pad.Held.Right || Pad.Held.Left) roll();
							else shield();
						}
						else if(Pad.Held.Right || Pad.Held.Left) run();
						else idle();
					}
				}
				if(airlag > 0) {
					airlag--;
					if(checkFloorCollision()) idle();
					else if(airlag == 0) {
						if(Pad.Held.A || Pad.Released.A) {
							if(Pad.Held.Up) uair();
							else if(Pad.Held.Down) dair();
							else if((Pad.Held.Left && direction == "left") || (Pad.Held.Right && direction == "right")) fair();
							else if((Pad.Held.Left && direction == "right") || (Pad.Held.Right && direction == "left")) bair();
							else nair();
						}
						else if(Pad.Held.B || Pad.Released.B) {
							if(Pad.Held.Up) {
								bup();
							}
							else if(Pad.Held.Down) bdown();
							else if(Pad.Held.Left || Pad.Held.Right) bside();
							else bneut();
						}
						else if(Pad.Held.R || Pad.Held.L || Pad.Released.R || Pad.Released.L) {
							if(airdodgecount < 1) {
								airdodge();
								airdodgecount++;
							}
						}
						else if((Pad.Held.Up || Pad.Held.X || Pad.Held.Y || Pad.Released.X || Pad.Released.Y) && jumpcount < jumpmax) doubleJump();
						else fall();
					}
				}
				if(landinglag > 0) {
					landinglag--;
					if(landinglag == 0) idle();
				} // lags upon landing after an aerial
				if(delay > 0) delay--; // counts down the time before a new animation is set (allows for animations to finish)
				if(action == "hang") {
					if(Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.R || Pad.Newpress.L) {
						y=y-bottomside;
						if(direction == "left") x=x-rightside;
						else x=x+leftside;
						idle();
					}
				}
				if(action == "dodge" && delay <= 0) shield();
				if(action == "airdodge" && delay <= 0) fall();
				if(action == "roll" && delay <= 0) {
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
				if(action == "slide" && delay <= 0) {
					idle();
				}
				if(action == "slide") {
					if(dx > 0) {
						dx -= .25;
						if(dx <= 0) dx = 0;
					}
					else if(dx < 0) {
						dx += .25;
						if(dx >= 0) dx = 0;
					}
				}
				if(action == "jab") {
					if((Pad.Released.A && delay > 100) || delay <= 0) {
						idle();
						delay = 0;
					}
				}
				if(action == "attack") {
					if(delay <= 0) {
						chargecount = 0;
						if(Pad.Held.Down) crouch();
						else idle(); // idles when attack is done
					}
				} 
				if(action == "dashattack") {
					x += dx;
					if(delay <= 0) idle();
				}
				if(action == "bside" || action == "bup" || action == "bdown" || action == "bneut") {
					if(delay <= 0) idle();
				}
				if(action == "airattack") {
					if(lcancel > 0) lcancel--;
					if(checkFloorCollision()) {
						if(delay > 0 && lcancel <= 0) land();
						else idle();
						delay = 0;
					}
					else if(Pad.Newpress.R || Pad.Newpress.L) lcancel = 10;
					else if(delay <= 0) fall();
				} // checks for stage collision with aerial
				if((action == "jump" || action == "doublejump") && delay <= 0) fall(); // falls when jump/multijump are finished animating
				if((action == "doublejump" || action == "jump" ) && (Pad.Newpress.A || Stylus.Newpress || Pad.Newpress.B)) {
					ymomentum = dy;
					momentumtime = delay;
					dy = 0;
					if(Stylus.Newpress) airAttackStylus();
					else airlag = 2;
				}
				if(action == "fall") {
					directionalInfluence();
					if(checkFloorCollision()) idle();
				} // checks for stage collision when falling
				if(aerial && action != "airattack" && action != "airlag") {
					actAir();
				} // acts in the air
				if(action == "shield") {
					if(!Pad.Held.R && !Pad.Held.L) idle();
					if(Pad.Newpress.Left || Pad.Newpress.Right) roll();
					if(Pad.Newpress.Down) dodge();
				}
				if(action == "run") {
					if(Pad.Newpress.Up || Pad.Newpress.X || Pad.Newpress.Y || Pad.Newpress.B) {
						tiltlag = 5;
						action = "tiltlag";
					}
					else if(Pad.Newpress.A) dashAttack();
					else if(Stylus.Newpress) smashAttack();
					else if(Pad.Held.Right || Pad.Held.Left) run();
					else if(Pad.Released.Right || Pad.Released.Left) slide();
					else idle();
				}
				if(action == "idle" || action == "") {
					actGround();
					setDirection();
				}
				if(action == "crouch") {
					if(!Pad.Held.Down) idle();
					if(Pad.Newpress.A) dtilt();
					if(Pad.Newpress.B) bdown();
				}
			}
			move(); // moves
		} // usually the acting method, but can be overwritten
		void actAir() {
			if(action == "bup" || action == "bdown" || action == "bside" || action == "bneut") return;
			if(action == "airdodge") {
				if(checkFloorCollision()) {
					dx *= 3;
					slide();
				}
				return;
			}
			if((Pad.Newpress.A || (Pad.Newpress.Up && jumpmax <= 2) || (Pad.Held.Up && jumpmax > 2) /*|| Pad.Held.Down || Pad.Held.Right || Pad.Held.Left */|| Pad.Newpress.B || Pad.Newpress.R || Pad.Newpress.L) && action != "jump" && action != "doublejump") {
				airlag = 2;
				action = "airlag";
			}
			if((Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.R || Pad.Newpress.L) && (action == "jump" || action == "doublejump")) {
				airlag = 2;
				action = "airlag";
			}
			if((Pad.Newpress.X || Pad.Newpress.Y) && jumpcount < jumpmax && action != "jump" && action != "doublejump") doubleJump();  // uses second (3rd, 4th, etc) jump(s)
			if(Stylus.Newpress) airAttackStylus();
			directionalInfluence();
		}
		void actGround() {
			if(tiltlag <= 0) {
				if(Pad.Newpress.Down) crouch();
				else if(Stylus.Newpress) smashAttack();
				else if((Pad.Newpress.R || Pad.Newpress.L)) shield();
				else if(Pad.Held.Right || Pad.Held.Left || Pad.Newpress.Up || Pad.Newpress.A || Pad.Newpress.B || Pad.Newpress.X || Pad.Newpress.Y) {
					action = "tiltlag";
					tiltlag = 5;
				}
				else idle();
			}
		} // acts on the ground based on key presses	
		virtual void bside() {}
		virtual void bup() {}
		virtual void bdown() {}
		virtual void bneut() {}
		void land() {
			action = "land";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[LAND], endframes[LAND], framespeeds[LAND], ANIM_LOOP, -1);
			landinglag = delay;
			startlag = landinglag;
			playsound(LAND);
		}
		void shield() {
			action = "shield";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHIELD], endframes[SHIELD], framespeeds[SHIELD], ANIM_LOOP, -1);
			playsound(SHIELD);	
		}
		void roll() {
			action = "roll";
			setDirection();
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[ROLL], endframes[ROLL], framespeeds[ROLL], ANIM_LOOP, -1);
			delay = 60/framespeeds[ROLL] * (endframes[ROLL]-startframes[ROLL]+1);
			if(Pad.Held.Left) dx = -2;
			if(Pad.Held.Right) dx = 2;
			playsound(ROLL);
		}
		void dodge() {
			action = "dodge";
			setDirection();
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DODGE], endframes[DODGE], framespeeds[DODGE], ANIM_LOOP, -1);
			delay = 60/framespeeds[DODGE] * (endframes[DODGE]-startframes[DODGE]+1);
			playsound(DODGE);
		}
		void airdodge() {
			action = "airdodge";
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
		void crouch() {
			action = "crouch";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CROUCH], endframes[CROUCH], framespeeds[CROUCH], ANIM_LOOP, -1);
			playsound(CROUCH);
		}
		void fall() {
			aerial = true;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FALL], endframes[FALL], framespeeds[FALL], ANIM_LOOP, -1);
			directionalInfluence();
			dy = 0;
			dx = 0;
			action = "fall";
			playsound(FALL);
		}
		void idle() {
			if(action != "idle") PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[IDLE], endframes[IDLE], framespeeds[IDLE], ANIM_LOOP, -1);
			dx = 0;
			dy = 0;
			action = "idle";
			playsound(IDLE);
		}
		void run(int d = 0) {
			if(action != "run") PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[RUN], endframes[RUN], framespeeds[RUN], ANIM_LOOP, -1);
			if(d == 0) {
				if(Pad.Held.Left) dx = -4;
				if(Pad.Held.Right) dx = 4;
				setDirection();
			}
			else {
				dx = 4*d;
				if(d > 0) setDirection("right");
				if(d < 0) setDirection("left");
			}
			action = "run";
			playsound(RUN);
		}
		void shorthop() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SHORTHOP], endframes[SHORTHOP], framespeeds[SHORTHOP], ANIM_LOOP, -1);
			dy = -4;
			fastfall = 0;
			dx = 0;
			delay = 60/framespeeds[SHORTHOP] * (endframes[SHORTHOP] - startframes[SHORTHOP] + 1);
			action = "jump";
			aerial = true;
			jumpcount++;
			playsound(SHORTHOP);
		}
		void jump() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JUMP], endframes[JUMP], framespeeds[JUMP], ANIM_LOOP, -1);
			dy = -6;
			fastfall = 0;
			dx = 0;
			delay = 60/framespeeds[JUMP] * (endframes[JUMP] - startframes[JUMP] + 1);
			action = "jump";
			aerial = true;
			jumpcount++;
			playsound(JUMP);
		}
		void doubleJump() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DOUBLEJUMP], endframes[DOUBLEJUMP], framespeeds[DOUBLEJUMP], ANIM_LOOP, -1);
			action = "doublejump";
			dy = -3.5 - .5 * (jumpmax-jumpcount);
			fastfall = 0;
			delay = 60/framespeeds[DOUBLEJUMP] * (endframes[DOUBLEJUMP] - startframes[DOUBLEJUMP] + 1);
			jumpcount++;
			aerial = true;
			setDirection();
#ifdef SFX_ON
			if(effectwait <= 0) {
				effects.push_back(Effect(x, y+32, FX_AIRJUMP, charnum));
				effectwait = 15;
			}
#endif
			playsound(DOUBLEJUMP);
		}
		void jab() {
			action = "jab";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[JAB], endframes[JAB], framespeeds[JAB], ANIM_LOOP, -1);
			delay = 60/framespeeds[JAB] * (endframes[JAB]-startframes[JAB]+1);
			playsound(JAB);
		}
		void dashAttack() {
			if(Pad.Held.Left) dx = -1.5;
			if(Pad.Held.Right) dx = 1.5;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DASHATTACK], endframes[DASHATTACK], framespeeds[DASHATTACK], ANIM_LOOP, -1);
			delay = 60/framespeeds[DASHATTACK] * (endframes[DASHATTACK]-startframes[DASHATTACK]+1);
			playsound(DASHATTACK);
		}
		void ftilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FTILT], endframes[FTILT], framespeeds[FTILT], ANIM_LOOP, -1);
			action = "attack";
			delay = 60/framespeeds[FTILT] * (endframes[FTILT]-startframes[FTILT]+1);
			setDirection();
			playsound(FTILT);
		}
		void utilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UTILT], endframes[UTILT], framespeeds[UTILT], ANIM_LOOP, -1);
			action = "attack";
			delay = 60/framespeeds[UTILT] * (endframes[UTILT]-startframes[UTILT]+1);		
			playsound(UTILT);
		}
		void dtilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DTILT], endframes[DTILT], framespeeds[DTILT], ANIM_LOOP, -1);
			action = "attack";
			delay = 60/framespeeds[DTILT] * (endframes[DTILT]-startframes[DTILT]+1);		
			playsound(DTILT);		
		}
		void chargeleft() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGELEFT], endframes[CHARGELEFT], framespeeds[CHARGELEFT], ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			action = "chargeleft";
			direction = "left";
			dx = 0;
			delay = 60/framespeeds[CHARGELEFT] * (endframes[CHARGELEFT]-startframes[CHARGELEFT]+1) * 15;
			playsound(CHARGELEFT);
		}
		void chargeright() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGERIGHT], endframes[CHARGERIGHT], framespeeds[CHARGERIGHT], ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			action = "chargeright";
			direction = "right";
			dx = 0;
			delay = 60/framespeeds[CHARGERIGHT] * (endframes[CHARGERIGHT]-startframes[CHARGERIGHT]+1) * 15;
			playsound(CHARGERIGHT);
		}
		void chargeup() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEUP], endframes[CHARGEUP], framespeeds[CHARGEUP], ANIM_LOOP, -1);
			action = "chargeup";
			delay = 60/framespeeds[CHARGEUP] * (endframes[CHARGEUP]-startframes[CHARGEUP]+1) * 15;
			playsound(CHARGEUP);
		}
		void chargedown() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[CHARGEDOWN], endframes[CHARGEDOWN], framespeeds[CHARGEDOWN], ANIM_LOOP, -1);
			action = "chargedown";
			dx = 0;
			delay = 60/framespeeds[CHARGEDOWN] * (endframes[CHARGEDOWN]-startframes[CHARGEDOWN]+1) * 15;
			playsound(CHARGEDOWN);
		}
		void smashleft() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHLEFT], endframes[SMASHLEFT], framespeeds[SMASHLEFT], ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			action = "attack";
			direction = "left";
			delay = 60/framespeeds[SMASHLEFT] * (endframes[SMASHLEFT]-startframes[SMASHLEFT]+1);
			playsound(SMASHLEFT);
		}
		void smashright() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHRIGHT], endframes[SMASHRIGHT], framespeeds[SMASHRIGHT], ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			action = "attack";
			direction = "right";
			delay = 60/framespeeds[SMASHRIGHT] * (endframes[SMASHRIGHT]-startframes[SMASHRIGHT]+1);
			playsound(SMASHRIGHT);
		}
		void smashup() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHUP], endframes[SMASHUP], framespeeds[SMASHUP], ANIM_LOOP, -1);
			action = "attack";
			delay = 60/framespeeds[SMASHUP] * (endframes[SMASHUP]-startframes[SMASHUP]+1);
			playsound(SMASHUP);
		}
		void smashdown() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SMASHDOWN], endframes[SMASHDOWN], framespeeds[SMASHDOWN], ANIM_LOOP, -1);
			action = "attack";
			delay = 60/framespeeds[SMASHDOWN] * (endframes[SMASHDOWN]-startframes[SMASHDOWN]+1);
			playsound(SMASHDOWN);
		}
		void fair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[FAIR], endframes[FAIR], framespeeds[FAIR], ANIM_LOOP, -1);
			delay = 60/framespeeds[FAIR] * (endframes[FAIR] - startframes[FAIR] + 1);
			playsound(FAIR);
		}
		void bair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[BAIR], endframes[BAIR], framespeeds[BAIR], ANIM_LOOP, -1);
			delay = 60/framespeeds[BAIR] * (endframes[BAIR] - startframes[BAIR] + 1);
			if(Pad.Held.Left) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			if(Pad.Held.Right) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			playsound(BAIR);
		}
		void uair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[UAIR], endframes[UAIR], framespeeds[UAIR], ANIM_LOOP, -1);
			delay = 60/framespeeds[UAIR] * (endframes[UAIR] - startframes[UAIR] + 1);
			playsound(UAIR);
		}
		void dair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[DAIR], endframes[DAIR], framespeeds[DAIR], ANIM_LOOP, -1);
			delay = 60/framespeeds[DAIR] * (endframes[DAIR] - startframes[DAIR] + 1);
			playsound(DAIR);
		}
		void nair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[NAIR], endframes[NAIR], framespeeds[NAIR], ANIM_LOOP, -1);
			delay = 60/framespeeds[NAIR] * (endframes[NAIR] - startframes[NAIR] + 1);
			playsound(NAIR);
		}
		void stun() {
			action = "hitstun";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[STUN], endframes[STUN], framespeeds[STUN], ANIM_LOOP, -1);
			playsound(STUN);
		}
		void slide() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[SLIDE], endframes[SLIDE], framespeeds[SLIDE], ANIM_LOOP, -1);
			action = "slide";
			delay = 5;
			playsound(SLIDE);
		}
		void hang() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, startframes[HANG], endframes[HANG], framespeeds[HANG], ANIM_LOOP, -1);
			action = "hang";
		}
	// Sound playing
		virtual void playsound(int sndnum) {}
	// constant methods
		void setStage(Stage s) { 
			stage = s; 
			x = stage.spawnx[charnum];
			y = stage.spawny[charnum];
			startx = x;
			starty = y;
		}
		int getHitstun() { return hitstun; }
		double getDamagePercent() { return percentage; }
		void takeDamage(Circle other, int mult, int hitter, int charge) {
			if(action != "hitstun") stun();
			percentage += other.damage + (int)((charge/225) * (.5*other.damage));
#ifdef SFX_ON
			if(effectwait <= 0) {	
				if(other.fx == FX_NONE) {		
					if(other.damage + (int)((charge/225) * (.5*other.damage)) <= 6/3) {
						AS_SoundQuickPlay(hit1);
						effects.push_back(Effect(x, y, FX_WEAKERHIT, charnum));
					}
					else if(other.damage + (int)((charge/225) * (.5*other.damage)) > 6/3 && other.damage + (int)((charge/225) * (.5*other.damage)) <= 12/3) {
						AS_SoundQuickPlay(hit2);
						effects.push_back(Effect(x, y, FX_WEAKHIT, charnum));
					}
					else {
						AS_SoundQuickPlay(hit3);
						effects.push_back(Effect(x, y, FX_STRONGHIT, charnum));			
					}
				}
				else {
					effects.push_back(Effect(x, y, other.fx, charnum));
					// play sound based on effect
				}
				effectwait = 15;
			}
#endif
			k = other.getKnockback();
			hitstun = (int) (k.length * 3 * (1+(percentage/100)));
			kx = (1+(percentage/100)) * k.dx * mult;
			ky = (1+(percentage/100)) * k.dy;
			dx = dy = DI = fastfall = 0;
			CAPE = false;
			lasthitby = hitter;
		}
		Fighter* checkHits(Fighter* other) {
			if(getAtkbox().hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
				if(other -> COUNTER) {
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
		Hitbox getAtkbox() { 
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
		virtual Hitbox getDefbox(int framenum) { 
			Hitbox temp = alldefbox[framenum];
			Hitbox defbox;
			vector<Circle> circles = temp.getCircles();
			for(uint8 n = 0; n < circles.size(); n++) {
				Circle current = circles[n];
				Circle newcircleft(current.getX() + x, current.getY() + y, current.getRadius());
				Circle newcircright(x + (64 - current.getX()), y + (current.getY()), current.getRadius());
				if(direction == "left") defbox.addCircle(newcircleft);
				else defbox.addCircle(newcircright);
			}
			return defbox;
		}
		Circle createAtkbox(double extrax, double extray, double radius, Knockback k, int damage) {
			Circle temp(64 - extrax, -gravity + extray, radius, k, damage);
			return temp;
		}
		void airAttack() {
			if(Pad.Held.Up) uair();
			else if(Pad.Held.Down) dair();
			else if((Pad.Held.Right && direction == "right") || (Pad.Held.Left && direction == "left")) fair();
			else if((Pad.Held.Right && direction == "left") || (Pad.Held.Left && direction == "right")) bair();
			else nair();
		}
		void airAttackStylus() {
			int touchx = Stylus.X;
			int touchy = Stylus.Y;
			if(touchy < 64 && touchx > 96 && touchx < 160) uair();
			else if(touchy > 128 && touchx > 96 && touchx < 160) dair();
			else if((touchx > 192 && touchy < 128 && touchy > 64 && direction == "right") || (touchx < 64 && touchy < 128 && touchy > 64 && direction == "left")) fair();
			else if((touchx > 192 && touchy < 128 && touchy > 64 && direction == "left") || (touchx < 64 && touchy < 128 && touchy > 64 && direction == "right")) bair();
			else nair();
		}
		void smashAttack() {
			int touchx = Stylus.X; // xpos of touch
			int touchy = Stylus.Y; // ypos of touch
			if(touchx < 64 && touchy < 128 && touchy > 64) chargeleft();
			if(touchx > 192 && touchy < 128 && touchy > 64) chargeright();
			if(touchy < 64 && touchx > 96 && touchx < 160) chargeup();
			if(touchy > 128 && touchx > 96 && touchx < 160) chargedown();
		}
		void move() {
			if(action == "hang") {}
			else if(!aerial) {
				if(!checkFloorCollision()) {
					fall();
					jumpcount++;
				}
			}
			else {
				if(!isCPU) directionalInfluence();
				if(action == "airattacK") {
					fastfall = 0;
				}
				if(action == "airdodge") {
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
			if(action == "attack") jaywalk();
			if(aerial) y += gravity;
			if(aerial) y += fastfall;
			if(aerial) x += DI;
			if(checkForDeath()) respawn();
			PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x, (int)y); // repositions the sprite
		} // moves the sprite
		virtual void jaywalk() {}
		void setDirection(string rl = "") {
			string olddirection = direction;
			if(action == "hitstun") {
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
		virtual void directionalInfluence(int dx = 0) {
			if(dx != 0) {
				DI = dx;
				fastfall = 0;
			}
			else {
				if(Pad.Held.Right) DI = 1;
				if(Pad.Held.Left) DI = -1;
				if(Pad.Held.Down) fastfall = 2;
				else fastfall = 0;
				if(!Pad.Held.Right && !Pad.Held.Left || action == "bdown") DI = 0;
			
	//			if(Pad.Held.Down && (action != "jump" && action != "doublejump")) fastfall = 1;
	//			if(Pad.Held.Up) fastfall = 0;
			// slightly influences direction
			}
		} // acts in the air based on key presses
		bool checkForDeath() {
			if(x > stage.rightdeath || x+64 < stage.leftdeath || y > stage.bottomdeath || y+64 < stage.topdeath) {
				score.addDeath(lasthitby, charnum);
#ifdef SFX_ON
				int deathx = -64, deathy = -64;
				if(x > stage.rightdeath) {
					deathx = 256-64+scrollx+10;
					deathy = y;
				} // died off of right side
				else if(x+64 < stage.leftdeath) {
					deathx = 0+scrollx-10;
					deathy = y;
				} // died off of left side
				else if(y > stage.bottomdeath) {
					deathx = x;
					deathy = 192-64+scrolly+10;
				} // died off of bottom
				else if(y+64 < stage.topdeath) {
					deathx = x;
					deathy = 0+scrolly-10;
				} // died off top
				effects.push_back(Effect(deathx, deathy, FX_DEATH, charnum));
				AS_SoundQuickPlay(deathsound);
#endif
				return true;
			}
			return false;
		}
		void respawn() {
			lasthitby = -1;
			x = startx;
			y = starty;
			action = "fall";
			fall();
			direction = "";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
		}
		bool checkLedgeCollision() {
			vector<Ledge> ledges = stage.getLedges();
			for(int n = 0; n < ledges.size(); n++) {
				Ledge currledge = ledges[n];
				if(action != "hitstun") {
					if(currledge.direction == "right") {
						if(x+leftside > currledge.x - 20 && x+leftside < currledge.x + 20 && y > currledge.y - 10 && y < currledge.y + 10) {
							hang();
							aerial = false;
							dx = DI = dy = fastfall = ymomentum = 0;
							airdodgecount = jumpcount = 0;
							x = currledge.x-leftside;
							y = currledge.y;
							setDirection("left");
							return true;
						}
					}
					else {
						if(x+rightside > currledge.x - 20 && x+rightside < currledge.x + 20 && y > currledge.y - 10 && y < currledge.y + 10) {
							hang();
							aerial = false;
							dx = DI = dy = fastfall = ymomentum = 0;
							airdodgecount = jumpcount = 0;
							x = currledge.x-rightside;
							y = currledge.y;
							setDirection("right");
							return true;				
						}
					}
				}
			}
		}
		bool checkFloorCollision() {
			vector<Floor> floors = stage.getFloors();
			for(uint8 n = 0; n < floors.size(); n++) {
				Floor currfloor = floors[n];
				double rise;
				if(currfloor.slopes.size() == 0) rise = 0;
				else rise = currfloor.getrise(x);
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
		bool checkWallCollision() {
			vector<Wall> walls = stage.getWalls();
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
		bool checkCeilingCollision() {
			vector<Ceiling> ceilings = stage.getCeilings();
			for(uint8 n = 0; n < ceilings.size(); n++) {
				Ceiling currceil = ceilings[n];
				if(y+topside >= currceil.y && y+topside+gravity+fastfall+dy+ymomentum < currceil.y && x+rightside+dx+DI > currceil.x && x+leftside+dx+DI < currceil.x+currceil.length) {
					y = currceil.y-topside-dy-ymomentum;
					return true;
				}
			}
			return false;
		}
		void scroll(double scrollx, double scrolly) {
			if(x - scrollx > 256 || x - scrollx < 0-64 || y - scrolly > 192 || y - scrolly < 0-64) {
				PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, -64, -64);
			}
			else {
				PA_SetSpriteXY(MAIN_SCREEN, SPRITENUM, (int)x - (int)(scrollx), (int)y - (int)(scrolly));
			}
		}
};
#ifdef PROJECTILES_ON
class Projectile {
	public:
		double x, y, dx, dy;
		double maxy, miny; // for bouncing
		Hitbox hit;
		int length;
		int time;
		int TYPE;
		int num;
		int owner;
		Stage mystage;
		Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage mine) {
			mystage = mine;
			num = 50+ob;
			owner = ob;
			TYPE = t;
			x = xpos;
			y = ypos;
			dx = xchange;
			if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
			else PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
			dy = ychange;
			length = l;
			time = 0;
			hit = h;
			PA_SetSpriteXY(MAIN_SCREEN, num, x-scrollx, y-scrolly);
			if(TYPE == SHADOWBALL_SMALL) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 0, 3, 20, ANIM_LOOP, -1);
			}
			if(TYPE == SHADOWBALL_MEDIUM) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 4, 7, 20, ANIM_LOOP, -1);
			}
			if(TYPE == SHADOWBALL_LARGE) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 8, 11, 20, ANIM_LOOP, -1);
			}
			if(TYPE == FINALCUTTER) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 12, 12, 20, ANIM_LOOP, -1);
			}
			if(TYPE == FIREBALL) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 13, 16, 20, ANIM_LOOP, -1);
			}
			if(TYPE == FLUDDWATER) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 17, 17, 20, ANIM_LOOP, -1);
			}
			if(TYPE == IKESWORD) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 18, 24, 20, ANIM_LOOP, -1);
			}
		}
		void act() {
			x += dx;
			y += dy;
			if(y > maxy) dy*=-1;
			if(y < miny) dy*=-1;
			PA_SetSpriteXY(MAIN_SCREEN, num, x-scrollx, y-scrolly);
			if(x+64-scrollx < 0 || x-scrollx > 256 || y+64-scrolly < 0 || y-scrolly > 192) PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
			time++;
			if(time > length) removeSelf();
		}
		void removeSelf() {
			vector<Projectile> temp;
			for(int n = 0; n < projectiles.size(); n++) {
				Projectile p = projectiles[n];
				if(p.x != x || p.y != y) {
					temp.push_back(p);
				}
			}
			projectiles = temp;
			PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
		}
		Fighter* checkHits(Fighter* other) {
			Hitbox temp = hit; 
			Hitbox atkbox;
			vector<Circle> circles = temp.getCircles();
			for(uint8 n = 0; n < circles.size(); n++) {
				Circle current = circles[n];
				Circle newcircright(current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
				Circle newcircleft(64 - current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
				if(dx < 0) atkbox.addCircle(newcircright);
				else atkbox.addCircle(newcircleft);
			}
			if(atkbox.hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
				if(other -> CAPE || other -> COUNTER) {
					dx *= -1;
					owner = other->charnum;
					vector<Circle> temp = hit.getCircles();
					hit.reset();
					for(int n = 0; n < temp.size(); n++) {
						hit.addCircle(Circle(temp[n].getX(), temp[n].getY(), temp[n].getRadius(), Knockback(temp[n].getKnockback().dx * -1, temp[n].getKnockback().dy, temp[n].getKnockback().length), temp[n].damage));
					}
					if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
					if(dx < 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
				}
				else {
					other -> takeDamage(atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, owner, 0);
					if(TYPE != FINALCUTTER && TYPE != IKESWORD) removeSelf();
				}
			}
			return other;
		}
};
#endif
#ifdef SANDBAG_ON
class Sandbag: public Fighter {
	public:
		Sandbag(int xpos, int ypos, int num) {
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = SANDBAG;
			chargecount = 0;
			lasthitby = -1;
			topside = 5;
			bottomside = 48;
			leftside = 16;
			rightside = 46;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 6;
			charnum = players.size();
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0.0;
			kx = ky = 0;
			ymomentum = 0.0;
			momentumtime = 0;
			name = "sandbag";
			initAtkbox();
			initSprite();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
		void act() {
			if(effectwait > 0) effectwait--;
			if(y != stage.getFloors()[0].y ) aerial = true;
			if(hitstun > k.length*2) {
				hitstun--;
				dx = kx;
				dy = ky;
				if(checkFloorCollision()) idle();
			}
			else if(hitstun > 0) {
				hitstun--;				
				if(dx > 0) {
					dx -= kx/k.length;
					if(dx < 0) dx = 0;
				}
				else if(dx < 0) {
					dx -= kx/k.length;
					if(dx > 0) dx = 0;
				}
				if(dy > 0) {
					dy -= ky/k.length;
					if(dy < 0) dy = 0;
				}
				else if(dy < 0) {
					dy -= ky/k.length;
					if(dy > 0) dy = 0;
				}
				if(hitstun == 0) {
					action = "hitstun";
					if(aerial) fall();
					else idle();
				}
				if(checkFloorCollision()) idle();
			}
			else {
			    if(checkFloorCollision()) idle();
				if(!aerial) idle();
				else fall();
				dx = dy = kx = ky = 0;
			}
			move(); // moves
		} // main acting method of the sprite
		void initAtkbox() {
			for(int frame = 0; frame <= 10; frame++) {
				Hitbox tempbox;
				allatkbox.push_back(tempbox);
			}
		}
		Hitbox initDefbox(int frame) {
				Hitbox tempbox;
				if(frame == 0) {
					tempbox.addCircle(Circle(32, 28%64, 13));
					tempbox.addCircle(Circle(32, 36%64, 13));
				}
				if(frame == 1) {
					tempbox.addCircle(Circle(30, 80%64, 9));
					tempbox.addCircle(Circle(38, 95%64, 9));
					tempbox.addCircle(Circle(27, 103%64, 9));
				}
				if(frame == 2) {
					tempbox.addCircle(Circle(20, 147%64, 9));
					tempbox.addCircle(Circle(28, 161%64, 9));
					tempbox.addCircle(Circle(42, 155%64, 9));
				}
				if(frame == 3) {
					tempbox.addCircle(Circle(38, 207%64, 9));
					tempbox.addCircle(Circle(25, 216%64, 9));
					tempbox.addCircle(Circle(31, 229%64, 9));
				}
				if(frame == 4) {
					tempbox.addCircle(Circle(44, 288%64, 9));
					tempbox.addCircle(Circle(26, 276%64, 9));
					tempbox.addCircle(Circle(23, 282%64, 9));
				}
			return tempbox;
		}
		void directionalInfluence() {}
		void fall() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 0, 0, 20, ANIM_LOOP, -1);
			dy = 0;
			dx = 0;
			aerial = true;
			action = "fall";
			hitstun = 0;
		}
		void idle() {
			if(action != "idle") PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 0, 0, 10, ANIM_LOOP, -1);
			dy = 0;
			dx = 0;
			action = "idle";
			hitstun = 0;
		}
		void stun() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 1, 4, 20, ANIM_LOOP, -1);
			action = "hitstun";
		}
		bool checkFloorCollision() {
			vector<Floor> floors = stage.getFloors();
			for(uint8 n = 0; n < floors.size(); n++) {
				Floor currfloor = floors[n];
				if(aerial) {
					// the y+44 is for the location of the feet, the x+32 is for the center of the body
					if(dy+ymomentum+gravity > 0 && y+44 <= currfloor.y && y+44 + gravity + fastfall + dy + ymomentum > currfloor.y && x+32 + dx + DI > currfloor.x && x+32 + dx + DI < currfloor.x + currfloor.length) {
						aerial = false;
						y = currfloor.y-44;
						dy = DI = fastfall = ymomentum = 0;
						airdodgecount = 0;
						jumpcount = 0;
						return true;
					} // if you land on the floor
				} // checks for landing
				else {
					if(dy+ymomentum+gravity > 0 && y+44 == currfloor.y && x+44 + dx + DI >= currfloor.x && x+32 + dx + DI < currfloor.x + currfloor.length) {
						return true;
					 } // stays on the ledge
				} // checks for falling off
			}
			return false;
		}
		void jaywalk() {}
};
#endif
class Kirby: public Fighter {
	int rockcount;
	public:	// constructor
		Kirby(int xpos, int ypos, int num, bool AI = false) {
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
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
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
			if(action != "bside") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 178, 186, 15, ANIM_ONESHOT);
#ifdef SFX_ON
				AS_SoundQuickPlay(kirbybside);
#endif
				delay = 60/15 * 9;
				setDirection();
				dx = 0;
				action = "bside";
			}
			else {
				if(delay == 1) {
					if(checkFloorCollision()) idle();
					else fall();
				}
			}
		}
		void bup() {
			if(action != "bup") {
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
				action = "bup";
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
			if(action != "bdown") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 187, 188, 20, ANIM_LOOP, -1);
				delay = 60/20 * 2;
				dy = -gravity;
				dx = 0;
				action = "bdown";
				rockcount = 250;
			}
			else {
				if(rockcount > 0) rockcount--;
				if(rockcount == 0 || Pad.Newpress.B) {
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
			if(action != "bneut") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 172, 172, 10, ANIM_LOOP, -1);
				delay = 60/10 * 1;
				setDirection();
				dx = 0;
				action = "bneut";
			}
			else {
				if(Pad.Released.B) {
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
		void jaywalk() {
			int frame = PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM);
			if(frame == 82 && direction == "right") x += 8;
			if(frame == 82 && direction == "left") x -= 8;
		}
};
class Mewtwo: public Fighter {
	public:
		int shadowballcharge;
	// constructor
		Mewtwo(int xpos, int ypos, int num, bool AI = false) {
			shadowballcharge = 0;
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = MEWTWO;
			chargecount = 0;
			isCPU = AI;
			lasthitby = -1;
			topside = 8;
			bottomside = 52;
			rightside = 44;
			leftside = 22;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 2;
			charnum = players.size();
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
			name = "mewtwo";
			initSprite();
			initDefbox();
			initAtkbox();
			initFrames();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
	// initializers
		void playsound(int sndnum) {}
		void initFrames() {
			// LAND
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(10);

			// SHIELD
			startframes.push_back(114);
			endframes.push_back(114);
			framespeeds.push_back(20);

			// ROLL
			startframes.push_back(110);
			endframes.push_back(113);
			framespeeds.push_back(20);

			// DODGE
			startframes.push_back(114);
			endframes.push_back(118);
			framespeeds.push_back(10);

			// AIRDODGE
			startframes.push_back(119);
			endframes.push_back(125);
			framespeeds.push_back(30);

			// CROUCH
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(20);

			// FALL
			startframes.push_back(28);
			endframes.push_back(28);
			framespeeds.push_back(20);

			// IDLE
			startframes.push_back(1);
			endframes.push_back(6);
			framespeeds.push_back(5);

			// RUN
			startframes.push_back(7);
			endframes.push_back(12);
			framespeeds.push_back(20);

			// SHORTHOP
			startframes.push_back(13);
			endframes.push_back(20);
			framespeeds.push_back(20);

			// JUMP
			startframes.push_back(13);
			endframes.push_back(20);
			framespeeds.push_back(20);

			// DOUBLEJUMP
			startframes.push_back(21);
			endframes.push_back(21);
			framespeeds.push_back(3);

			// JAB
			startframes.push_back(30);
			endframes.push_back(33);
			framespeeds.push_back(10);

			// DASHATTACK
			startframes.push_back(34);
			endframes.push_back(38);
			framespeeds.push_back(10);

			// FTILT
			startframes.push_back(107);
			endframes.push_back(109);
			framespeeds.push_back(6);

			// UTILT
			startframes.push_back(104);
			endframes.push_back(107);
			framespeeds.push_back(6);

			// DTILT
			startframes.push_back(66);
			endframes.push_back(71);
			framespeeds.push_back(10);

			// CHARGELEFT
			startframes.push_back(39);
			endframes.push_back(40);
			framespeeds.push_back(20);

			// CHARGERIGHT
			startframes.push_back(39);
			endframes.push_back(40);
			framespeeds.push_back(20);

			// CHARGEUP
			startframes.push_back(47);
			endframes.push_back(48);
			framespeeds.push_back(20);

			// CHARGEDOWN
			startframes.push_back(58);
			endframes.push_back(59);
			framespeeds.push_back(20);

			// SMASHLEFT
			startframes.push_back(41);
			endframes.push_back(46);
			framespeeds.push_back(20);

			// SMASHRIGHT
			startframes.push_back(41);
			endframes.push_back(46);
			framespeeds.push_back(20);

			// SMASHUP
			startframes.push_back(49);
			endframes.push_back(57);
			framespeeds.push_back(12);

			// SMASHDOWN
			startframes.push_back(60);
			endframes.push_back(65);
			framespeeds.push_back(10);

			// FAIR
			startframes.push_back(80);
			endframes.push_back(85);
			framespeeds.push_back(10);

			// BAIR
			startframes.push_back(87);
			endframes.push_back(92);
			framespeeds.push_back(10);

			// UAIR
			startframes.push_back(98);
			endframes.push_back(102);
			framespeeds.push_back(10);

			// DAIR
			startframes.push_back(93);
			endframes.push_back(97);
			framespeeds.push_back(10);

			// NAIR
			startframes.push_back(72);
			endframes.push_back(79);
			framespeeds.push_back(10);

			// STUN
			startframes.push_back(116);
			endframes.push_back(116);
			framespeeds.push_back(20);

			// SLIDE
			startframes.push_back(0);
			endframes.push_back(0);
			framespeeds.push_back(20);
		
			// HANG
			startframes.push_back(81);
			endframes.push_back(81);
			framespeeds.push_back(20);
		}
	// actions
		void bside() {
			if(action != "bside") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 135, 136, 10, ANIM_LOOP, -1);
				delay = 60/10 * 2;
				setDirection();
				dx = 0;
				action = "bside";
			}
			else if((PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 141 || PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 136) && delay == 1) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 137, 141, 10, ANIM_LOOP, -1);
				delay = 60/10 * 5;
			}
			else if(Pad.Released.B) {
				if(checkFloorCollision()) idle();
				else fall();
			}
		}
		void bup() {
			if(action != "bup") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 119, 122, 10, ANIM_ONESHOT);
				aerial = true;
				delay = 60/10 * 4;
				dy = 0;
				dx = 0;
				DI = 0;
				fastfall = 0;
				action = "bup";
				setDirection();
			}
			else if(action == "bup" && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 122 && delay == 1) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 125, 10, ANIM_ONESHOT);
				aerial = true;
				delay = 60/10 * 4;
				y += -200;
			}
			else if(action == "bup" && PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 125 && delay == 1) {
				if(!checkFloorCollision()) fall();
				else idle();
			}
		}
		void bdown() {
			if(action != "bdown" || (Pad.Held.B && delay == 1)) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 145, 10, ANIM_LOOP, -1);
				delay = 60/10 * 3;
				if(aerial) dy = -gravity/2;
				dx = 0;
				action = "bdown";
			}
			else if(!Pad.Held.B) {
				if(checkFloorCollision()) idle();
				else fall();
			}
			else if(aerial && checkFloorCollision()) dy = 0;
		}
		void bneut() {
			if(action != "bneut") {
				if(shadowballcharge < 40) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 126, 127, 15, ANIM_LOOP, -1);
					delay = 60/15 * 2 * 5;
					dx = 0;
					action = "bneut";				
				}
				else if(shadowballcharge < 80) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 128, 129, 15, ANIM_LOOP, -1);
					delay = 60/15 * 2 * 5;
					dx = 0;
					action = "bneut";
				}
				else if(shadowballcharge < 120) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 130, 131, 15, ANIM_LOOP, -1);
					delay = 60/15 * 2 * 5;
					dx = 0;
					action = "bneut";
				}
				else if(shadowballcharge == 120) {
					PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 134, 15, ANIM_LOOP, -1);
					delay = 60/15 * 3;
#ifdef PROJECTILES_ON
					int SHADOWBALL_SIZE = -1;
					int directionmodifier = 1;
					if(direction == "right") directionmodifier = -1;
					Hitbox tempbox;
					tempbox.addCircle(createAtkbox(32, 32, 14, Knockback(-3*directionmodifier, -1.5, 8), 240));
					projectiles.push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_LARGE, charnum, tempbox, stage));
#endif
					shadowballcharge = 0;
					action = "bneut";
					dx = 0;
				}
			}
			else if(Pad.Newpress.B) {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 134, 15, ANIM_LOOP, -1);
				delay = 60/15 * 3;
#ifdef PROJECTILES_ON
				int SHADOWBALL_SIZE = -1;
				if(shadowballcharge >= 80) SHADOWBALL_SIZE = SHADOWBALL_LARGE;
				else if(shadowballcharge >= 40) SHADOWBALL_SIZE = SHADOWBALL_MEDIUM;
				else SHADOWBALL_SIZE = SHADOWBALL_SMALL;
				int directionmodifier = 1;
				if(direction == "right") directionmodifier = -1;
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
//				tempbox.addCircle(createAtkbox(32, 32, rad, Knockback((-3*directionmodifier/kbmod), -1.5/kbmod, 8), shadowballcharge*2));
				tempbox.addCircle(createAtkbox(32, 32, rad, Knockback((-3*directionmodifier), -1.5, 8), 120));
				projectiles.push_back(Projectile(x, y, -3*directionmodifier, 0, 100, SHADOWBALL_SIZE, charnum, tempbox, stage));
#endif
				shadowballcharge = 0;
			}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 132) {}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 133) {}
			else if(PA_GetSpriteAnimFrame(MAIN_SCREEN, SPRITENUM) == 134) {
				if(delay == 1) {
					if(checkFloorCollision()) idle();
					else fall();
				}
			}
			else if(Pad.Newpress.R || Pad.Newpress.L) shield();
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
		void jaywalk() {}
};
class Mario: public Fighter {
	int fluddcharge;
	public:
	// constructor
		Mario(int xpos, int ypos, int num, bool AI = false) {
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
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
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
		}
	// actions
		void bside() {
			if(action != "bside") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 143, 148, 10, ANIM_ONESHOT);
				delay = 60/10 * 6;
				setDirection();
				dx = 0;
				action = "bside";
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
			if(action != "bup") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 137, 142, 20, ANIM_ONESHOT);
				aerial = true;
				delay = 60/20 * 6;
				dy = -7;
				dx = 0;
				fastfall = 0;
				DI = 0;
				setDirection();
				action = "bup";
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
			if(action != "bdown") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 149, 149, 20, ANIM_LOOP, -1);
				delay = 60/20 * 1;
				fluddcharge = 0;
				dx = 0;
				if(aerial) dy = -gravity/2;
				else dy = 0;
				action = "bdown";
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
			if(action != "bneut") {
				PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 133, 12, ANIM_LOOP, -1);
				delay = 60/12 * 2;
				dx = 0;
				action = "bneut";
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
class Ike: public Fighter {
	int quickdrawcharge;
	int eruptioncharge;
	public:
	// constructor
		Ike(int xpos, int ypos, int num, bool AI = false) {
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
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
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
			framespeeds.push_back(10);

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
#ifdef SHADOW_ON
class Shadow: public Fighter {
	public:
	// constructor
		Shadow(int xpos, int ypos, int num, bool AI = false) {
			CAPE = false;
			COUNTER = false;
			effectwait = 0;
			MYCHAR = SHADOW;
			chargecount = 0;
			isCPU = AI;
			lasthitby = -1;
			topside = 15;
			bottomside = 47;
			rightside = 40;
			leftside = 22;
			SPRITENUM = num + 100;
			gravity = 3;
			jumpmax = 2;
			charnum = players.size();
			x = xpos;
			y = ypos;
			startx = x;
			starty = y;
			action = "fall";
			aerial = true;
			delay = jumpcount = startlag = landinglag = tiltlag = airlag = lcancel = hitstun = 0;
			dx = dy = fastfall = DI = 0.0;
			percentage = 0;
			ymomentum = 0.0;
			momentumtime = 0;
			name = "shadow";
			initAtkbox();
			initDefbox();
			initSprite();
			if(x > stage.width/2) setDirection("right");
			else setDirection("left");
		} // initializes all of the variables
	// initializers
		void initDefbox() {
			for(int n = 0; n < 175; n++) {
				Hitbox h;
				alldefbox.push_back(h);
			}
			FILE* file = fopen("/SSBDS_Files/hitboxes/shadow.def", "rb");
			if(!file) while(true) {}
			char line[64];
			while(true) {
				Hitbox tempbox;
				fgets(line, 64, file);
			
				int frame = atoi(strtok(line, " \t"));
				double xpos = atof(strtok(NULL, " \t"));
				double ypos = atof(strtok(NULL, " \t"));
				double radius = atof(strtok(NULL, " \t"));

				if(frame == -1) break;					
				alldefbox[frame].addCircle(Circle(xpos, (int)(ypos)%64, radius));
			}
			fclose(file);
		}
		void initAtkbox() {
			for(int frame = 0; frame < 200; frame++) {
				Hitbox tempbox;
				if(frame == 41) {
					tempbox.addCircle(Circle(64.0-53.0, 2663%64, 3.1622776601683795, Knockback(-1, -.25, 5), 12));
					tempbox.addCircle(Circle(64.0-56.5, 2653%64, 3.0413812651491097, Knockback(-1, -.25, 5), 12));
					tempbox.addCircle(Circle(64.0-57.0, 2659%64, 3.5355339059327378, Knockback(-1, -.25, 5), 12));
				}
				if(frame == 42) {
					tempbox.addCircle(Circle(64.0-53.5, 2716%64, 2.0615528128088303, Knockback(-1, -.25, 5), 12));
					tempbox.addCircle(Circle(64.0-56.5, 2727%64, 2.23606797749979, Knockback(-1, -.25, 5), 12));
					tempbox.addCircle(Circle(64.0-58.5, 2718%64, 3.0413812651491097, Knockback(-1, -.25, 5), 12));
					tempbox.addCircle(Circle(64.0-58.5, 2723%64, 2.5, Knockback(-1, -.25, 5), 12));
				}
				if(frame == 47) {
					tempbox.addCircle(Circle(64.0-48.5, 3032%64, 7.0, Knockback(-.5, -1.5, 7), 21));
				}
				if(frame == 53) {
					tempbox.addCircle(Circle(64.0-52.5, 3422%64, 6.800735254367722, Knockback(-1.5, -.5, 7), 24));
				}
				if(frame == 58) {
					tempbox.addCircle(Circle(64.0-34.0, 3758%64, 7.0178344238090995, Knockback(-.5, -1.5, 6), 18));
				}
				if(frame == 70) {
					tempbox.addCircle(Circle(64.0-51.0, 4508%64, 13.82931668593933, Knockback(0, 0, 0), 6));
				}
				if(frame == 71) {
					tempbox.addCircle(Circle(64.0-50.0, 4572%64, 15.0, Knockback(0, 0, 0), 6));
				}
				if(frame == 72) {
					tempbox.addCircle(Circle(64.0-51.5, 4635%64, 14.5, Knockback(-2.5, -1, 7), 30));
				}
				if(frame == 85) {
					tempbox.addCircle(Circle(64.0-21.0, 5468%64, 14.916433890176299, Knockback(0, 0, 0), 6));
					tempbox.addCircle(Circle(64.0-45.5, 5467%64, 15.402921800749363, Knockback(0, 0, 0), 6));
				}
				if(frame == 86) {
					tempbox.addCircle(Circle(64.0-20.0, 5533%64, 14.230249470757707, Knockback(-2, -.75, 7), 39));
					tempbox.addCircle(Circle(64.0-47.5, 5531%64, 14.5, Knockback(2, -.75, 7), 39));
				}
				if(frame == 95) {
					tempbox.addCircle(Circle(64.0-34.5, 6119%64, 10.5, Knockback(0, -1.5, 7), 15));
				}
				if(frame == 96) {
					tempbox.addCircle(Circle(64.0-34.5, 6161%64, 8.139410298049853, Knockback(0, -2.75, 7), 33));
					tempbox.addCircle(Circle(64.0-35.0, 6181%64, 11.543396380615196, Knockback(0, -2.75, 7), 33));
				}
				if(frame == 99) {
					tempbox.addCircle(Circle(64.0-47.0, 6380%64, 6.020797289396148, Knockback(-1.75, -1, 6), 30));
				}
				if(frame == 100) {
					tempbox.addCircle(Circle(64.0-49.5, 6444%64, 7.0710678118654755, Knockback(-1.75, -1, 6), 30));
				}
				if(frame == 101) {
					tempbox.addCircle(Circle(64.0-52.5, 6506%64, 8.631338250816034, Knockback(-1.75, -1, 6), 30));
				}
				if(frame == 111) {
					tempbox.addCircle(Circle(64.0-36.5, 7130%64, 19.5, Knockback(0, 0, 5), 18));
				}
				if(frame == 112) {
					tempbox.addCircle(Circle(64.0-35.5, 7196%64, 24.0, Knockback(0, 0, 5), 18));
				}
				if(frame == 118) {
					tempbox.addCircle(Circle(64.0-35.5, 7599%64, 8.0156097709407, Knockback(0, 1, 5), 21));
				}
				if(frame == 123) {
					tempbox.addCircle(Circle(64.0-47.5, 7885%64, 5.522680508593631, Knockback(-.25, -1.25, 5), 18));
				}
				if(frame == 124) {
					tempbox.addCircle(Circle(64.0-33.0, 7941%64, 7.0, Knockback(0, -1.75, 5), 24));
				}
				if(frame == 129) {
					tempbox.addCircle(Circle(64.0-16.0, 8296%64, 6.0, Knockback(1.5, -.75, 5), 33));
				}
				if(frame == 136) {
					tempbox.addCircle(Circle(64.0-50.0, 8738%64, 14.508618128546908, Knockback(0, 0, 5), 18));
				}
				if(frame == 137) {
					tempbox.addCircle(Circle(64.0-49.0, 8802%64, 12.5, Knockback(-2, -1.25, 7), 21));
				}
				if(frame == 144) {
					tempbox.addCircle(Circle(64.0-38.0, 9260%64, 6.800735254367722, Knockback(-.5, -1.5, 5), 27));
				}
				if(frame == 155) {
					tempbox.addCircle(Circle(64.0-35.0, 9956%64, 25.544079548889602, Knockback(0, 0, 3), 9));
				}
				if(frame == 156) {
					tempbox.addCircle(Circle(64.0-34.5, 10017%64, 25.811818998280614, Knockback(0, 0, 3), 9));
				}
				if(frame == 163) {
					tempbox.addCircle(Circle(64.0-36.0, 10461%64, 12.0, Knockback(0, 0, 5), 9));
				}
				if(frame == 164) {
					tempbox.addCircle(Circle(64.0-35.0, 10524%64, 26.9675731203236, Knockback(0, 0, 5), 9));
				}
				if(frame == 165) {
					tempbox.addCircle(Circle(64.0-37.5, 10588%64, 28.00446392988089, Knockback(0, 0, 5), 9));
				}
				if(frame == 166) {
					tempbox.addCircle(Circle(64.0-36.0, 10653%64, 26.23451924468981, Knockback(0, 0, 5), 9));
				}
				if(frame == 167) {
					tempbox.addCircle(Circle(64.0-37.0, 10717%64, 26.800186566514792, Knockback(-.5, -2.5, 7), 42));
				}
				if(frame == 168) {
					tempbox.addCircle(Circle(64.0-36.5, 10780%64, 26.51886121235224, Knockback(0, 0, 5), 9));
				}
				if(frame == 169) {
					tempbox.addCircle(Circle(64.0-37.5, 10843%64, 16.5, Knockback(0, 0, 5), 9));
				}
				allatkbox.push_back(tempbox);
			}
		}
	// actions
		void land() {
			action = "land";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 172, 172, 10, ANIM_LOOP, -1);
			landinglag = delay;
			startlag = landinglag;
		}
		void shield() {
			action = "shield";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 165, 165, 20, ANIM_LOOP, -1);
		}
		void roll() {
			action = "roll";
			setDirection();
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 28, 32, 12, ANIM_LOOP, -1);
			delay = 60/12 * 5;
			if(Pad.Held.Left) dx = -2;
			if(Pad.Held.Right) dx = 2;
		}
		void dodge() {
			action = "dodge";
			setDirection();
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 32, 35, 15, ANIM_UPDOWN, 1);
			delay = 60/15 * 8;
		}
		void airdodge() {
			action = "airdodge";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 36, 38, 12, ANIM_LOOP, -1);
			delay = 60/12 * 4;
			if(Pad.Held.Up) dy = -gravity - 2;
			else if(Pad.Held.Down) dy = -gravity + 2;
			if(Pad.Held.Right) dx = 2; // accounts for gravity
			else if(Pad.Held.Left) dx = -2;
			if(!Pad.Held.Up && !Pad.Held.Down && !Pad.Held.Right && !Pad.Held.Left) {
				dx = 0;
				dy = -gravity;
			}
			if(!Pad.Held.Up && !Pad.Held.Down) dy = -gravity;
			fastfall = 0;
			DI = 0;
		}
		void crouch() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 172, 172, 20, ANIM_LOOP, -1);
			action = "crouch";
		}
		void fall() {
			aerial = true;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 27, 27, 20, ANIM_LOOP, -1);
			directionalInfluence();
			dy = 0;
			dx = 0;
			action = "fall";
		}
		void idle() {
			if(action != "idle") PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 0, 5, 10, ANIM_LOOP, -1);
			dx = 0;
			dy = 0;
			action = "idle";
		}
		void run(int d = 0) {
			if(action != "run") PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 10, 19, 15, ANIM_LOOP, -1);
			if(d == 0) {
				if(Pad.Held.Left) dx = -4;
				if(Pad.Held.Right) dx = 4;
				setDirection();
			}
			else {
				dx = 4*d;
				if(d > 0) setDirection("right");
				if(d < 0) setDirection("left");
			}
			action = "run";
		}
		void shorthop() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 20, 20, 10, ANIM_ONESHOT);
			dy = -4;
			fastfall = 0;
			dx = 0;
			delay = 60/10 * 4;
			action = "jump";
			aerial = true;
			jumpcount++;
		}
		void jump() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 20, 20, 10, ANIM_ONESHOT);
			dy = -7; 
			fastfall = 0;
			dx = 0;
			delay = 60/10 * 4;
			action = "jump";
			aerial = true;
			jumpcount++;
		}
		void doubleJump() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 21, 26, 15, ANIM_ONESHOT);
#ifdef SFX_ON
			if(effectwait <= 0) {
				effects.push_back(Effect(x, y+32, FX_AIRJUMP, charnum));
				effectwait = 15;
			}
#endif
			action = "doublejump";
			dy = -6;
			fastfall = 0;
			delay = 60/15 * 6;
			jumpcount++;
			aerial = true;
			setDirection();
		}
	// attacks
		void jab() {
			action = "jab";
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 39, 44, 12, ANIM_ONESHOT);
			delay = 60/12 * 6;
		}
		void dashAttack() {
			if(Pad.Held.Left) dx = -1;
			if(Pad.Held.Right) dx = 1;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 99, 103, 15, ANIM_ONESHOT);
			action = "dashattack";
			delay = 60/15 * 5;
		}
		void utilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 44, 48, 10, ANIM_ONESHOT);
			action = "attack";
			delay = 60/10 * 5;
		}
		void ftilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 49, 55, 12, ANIM_ONESHOT);
			action = "attack";
			delay = 60/12 * 7;
			setDirection();
		}
		void dtilt() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 56, 61, 10, ANIM_ONESHOT);
			delay = 60/10 * 6;
			action = "attack";
		}
		void chargeleft() {	
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 62, 63, 20, ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			action = "chargeleft";
			direction = "left";
			dx = 0;
			delay = 60/20 * 2 * 15; // loops through 15 times
		}
		void chargeright() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 62, 63, 20, ANIM_LOOP, -1);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			action = "chargeright";
			direction = "right";
			dx = 0;
			delay = 60/20 * 2 * 15; // loops through 15 times
		}
		void chargeup() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 89, 90, 20, ANIM_LOOP, -1);
			action = "chargeup";
			delay = 60/20 * 2 * 15; // loops through 15 times
		}
		void chargedown() {
			chargecount = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 77, 78, 20, ANIM_LOOP, -1);
			action = "chargedown";
			dx = 0;
			delay = 60/20 * 2 * 15; // loops through 15 times
		}
		void smashleft() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 64, 76, 20, ANIM_ONESHOT);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
			action = "attack";
			direction = "left";
			delay = 60/20 * 13;
		}
		void smashright() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 64, 76, 20, ANIM_ONESHOT);
			PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			action = "attack";
			direction = "right";
			delay = 60/20 * 13;
		}
		void smashup() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 91, 98, 15, ANIM_ONESHOT);
			action = "attack";
			delay = 60/15 * 8;
		}
		void smashdown() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 79, 88, 15, ANIM_ONESHOT);
			action = "attack";
			delay = 60/15 * 10;
		}
		void uair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 122, 127, 15, ANIM_ONESHOT);
			delay = 60/15 * 6;
		}
		void dair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 115, 121, 15, ANIM_ONESHOT);
			delay = 60/15 * 7;
		}
		void fair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 132, 138, 10, ANIM_ONESHOT);
			delay = 60/10 * 5;
		}
		void bair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 126, 131, 12, ANIM_ONESHOT);
			delay = 60/12 * 6;
			if(Pad.Held.Left) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 0);
			if(Pad.Held.Right) PA_SetSpriteHflip(MAIN_SCREEN, SPRITENUM, 1);
		}
		void nair() {
			action = "airattack";
			dy = 0;
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 108, 115, 15, ANIM_ONESHOT);
			delay = 60/15 * 8;
		}
		void stun() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 104, 106, 20, ANIM_LOOP, -1);
			action = "hitstun";
		}
		void slide() {
			PA_StartSpriteAnimEx(MAIN_SCREEN, SPRITENUM, 102, 102, 20, ANIM_LOOP, -1);
			action = "slide";
			delay = 5;
		}
		void bside() {
			if(action != "bside") {
				idle();
			}
			else {
				idle();
			}
		}
		void bup() {
			if(action != "bup") {
				idle();
			}
			else {
				idle();
			}
		}
		void bdown() {
			if(action != "bdown") {
				idle();
			}
			else {
				idle();
			}
		}
		void bneut() {
			if(action != "bneut") {
				idle();
			}
			else {
				idle();
			}
		}
		void jaywalk() {}
};
#endif

void printMemoryUsage() {
#ifdef DEBUG_ON
	PA_OutputText(MAIN_SCREEN, 0, 5, "                                                  ");
	PA_OutputText(MAIN_SCREEN, 0, 7, "                                                  ");
	PA_OutputText(MAIN_SCREEN, 0, 9, "                                                  ");
	struct mallinfo info = mallinfo(); // memory allocation info
	PA_OutputText(MAIN_SCREEN,0,5,"Memory in use: %d bytes", info.usmblks + info.uordblks);
	PA_OutputText(MAIN_SCREEN,0,7,"Total heap size: %d bytes", info.arena);
	PA_OutputText(MAIN_SCREEN,0,9,"Memory in free: %d bytes", info.fsmblks + info.fordblks);
#endif
} 
// prints memory usage to main screen. requires text to be initialized.
// only prints if debugging
void fadeOut() {
   	for(int i = 0; i >= -31; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // slowly darkens the screen into black
} // fades both screens out
void fadeIn() {
   	for(int i = -31; i <= 0; i++) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // slowly brightens the screen to normal
} // fades both screens in
void flash() {
	for(int i = 1; i <= 31; i+=3) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // brightens the screen to white realy quickly
	for(int i = 31; i >= 0; i--) {
		PA_SetBrightness(MAIN_SCREEN, i);
		PA_SetBrightness(SUB_SCREEN, i);
		PA_WaitForVBL();
	} // darkens the screen to normal
} // flashes the screen brightly

char* gifbuffer = NULL; // the array which stores the gif being printed
void openGif(int screen, string path) {
	delete gifbuffer; // clears the gif buffer
	FILE* imgFile = fopen (path.c_str(), "rb"); // opens the file at path for reading	
	if(imgFile){
		u32 imgSize;
		fseek (imgFile, 0 , SEEK_END);
		imgSize = ftell (imgFile);
		rewind (imgFile);
		// obtains file size
	
		gifbuffer = NULL;
		gifbuffer = (char*) malloc (sizeof(char)*imgSize);
		fread (gifbuffer, 1, imgSize, imgFile);
		// allocate memory to store the image file
	
		fclose (imgFile); // close the file
		PA_LoadGif(screen, (void *)gifbuffer); // Show gif on screen
	} // loads the gif if the image file exists
} // opens the gif at path onto screen

string stagename; // the name of the current stage

Stage setStage(string name) {
	PA_ResetBgSysScreen(MAIN_SCREEN); // resets bg on the main screen
	Stage picked; // the stage which is chosen
	if(name == "finaldestination") {
//		PA_Init8bitBg(MAIN_SCREEN, 3);
//		openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/finaldestinationbackground.gif");
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, finaldestination);
		picked = FinalDestination();
	} // loads final destination if it was chosen
	if(name == "pokemonstadium") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, pokemonstadium);
		picked = PokemonStadium();
	} // loads pokemon stadium if it was chosen
#ifdef SLOPEDSTAGES_ON
	if(name == "castleseige") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, castleseige);
		picked = CastleSeige();
	} // loads castle seige if it was chosen
	if(name == "corneria") {
		// background
		PA_LoadPAGfxLargeBg(MAIN_SCREEN, 0, corneria);
		picked = Corneria();
	} // loads corneria if it was chosen
#endif
	for(int n = 0; n < players.size(); n++) {
		players[n] -> setStage(picked);
	} // sets the stage of the players to the picked stage
	return picked; // returns the picked stage
} // displays the stage on the main screen
void initMinimap(string name) {
	PA_ResetBgSysScreen(SUB_SCREEN);
	// initializes the minimap image
//	if(name == "finaldestination") {
//		PA_LoadPAGfxLargeBg(SUB_SCREEN, 0, finaldestinationminimap);
//	}
	
	PA_LoadSpritePal(SUB_SCREEN, P1MINI, (void*)p1minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P1MINI, (void*)p1minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P1MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P2MINI, (void*)p2minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P2MINI, (void*)p2minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P2MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P3MINI, (void*)p3minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P3MINI, (void*)p3minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P3MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, P4MINI, (void*)p4minimap_Pal);
	PA_CreateSprite(SUB_SCREEN, P4MINI, (void*)p4minimap_Sprite, OBJ_SIZE_8X8, COLOR256, P4MINI, -8, -8);
	
	PA_LoadSpritePal(SUB_SCREEN, MINIBOX, (void*)visibleminimap_Pal);
	PA_CreateSprite(SUB_SCREEN, MINIBOX, (void*)visibleminimap_Sprite, OBJ_SIZE_32X32, COLOR256, MINIBOX, -8, -8);
	// loads the sprite icons for the minimap
	
	PA_LoadSpritePal(SUB_SCREEN, UPARR, (void*)ArrowUp_Pal);
	PA_CreateSprite(SUB_SCREEN, UPARR, (void*)ArrowUp_Sprite, OBJ_SIZE_64X64, COLOR256, UPARR, 96, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, UPARR, 0, 2, 1, ANIM_LOOP, -1);

	PA_LoadSpritePal(SUB_SCREEN, DOWNARR, (void*)ArrowDown_Pal);
	PA_CreateSprite(SUB_SCREEN, DOWNARR, (void*)ArrowDown_Sprite, OBJ_SIZE_64X64, COLOR256, DOWNARR, 96, 128);
	PA_StartSpriteAnimEx(SUB_SCREEN, DOWNARR, 0, 2, 1, ANIM_LOOP, -1);
	
	PA_LoadSpritePal(SUB_SCREEN, LEFTARR, (void*)ArrowLeft_Pal);
	PA_CreateSprite(SUB_SCREEN, LEFTARR, (void*)ArrowLeft_Sprite, OBJ_SIZE_64X64, COLOR256, LEFTARR, 0, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, LEFTARR, 0, 2, 1, ANIM_LOOP, -1);
	
	PA_LoadSpritePal(SUB_SCREEN, RIGHTARR, (void*)ArrowRight_Pal);
	PA_CreateSprite(SUB_SCREEN, RIGHTARR, (void*)ArrowRight_Sprite, OBJ_SIZE_64X64, COLOR256, RIGHTARR, 192, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, RIGHTARR, 0, 2, 1, ANIM_LOOP, -1);
	// loads and animates the touch smash arrows on the screen
} // displays the minimap on the sub screen
#ifdef SFX_ON
void initFX() {
	PA_LoadSpritePal(MAIN_SCREEN, 15, (void*)specialFX_Pal);
	for(int n = 5; n < 25; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)specialFX, OBJ_SIZE_64X64, COLOR256, 15, -64, -64);
	}
	// loads all special effect sprites
} // initializes special effects
#endif
#ifdef PROJECTILES_ON
void initProjectiles() {
	PA_LoadSpritePal(MAIN_SCREEN, 14, (void*)projectilesprites_Pal);
	for(int n = 50; n < 55; n++) {
		PA_CreateSprite(MAIN_SCREEN, n, (void*)projectilesprites, OBJ_SIZE_64X64, COLOR256, 14, -64, -64);
	} // loads all projectile sprites
} // initializes projectiles
#endif

// selecting char/stage
void stageSelect() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/blank.gif"); // blank background
	
	fadeIn();
	
	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)stagesel_Pal);
	PA_CreateSprite(SUB_SCREEN, FINALDESTINATION, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, FINALDESTINATION, FINALDESTINATION, FINALDESTINATION, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, POKEMONSTADIUM, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, POKEMONSTADIUM, POKEMONSTADIUM, POKEMONSTADIUM, 1, ANIM_LOOP, -1);
#ifdef SLOPEDSTAGES_ON
	PA_CreateSprite(SUB_SCREEN, CASTLESEIGE, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CASTLESEIGE, CASTLESEIGE, CASTLESEIGE, 1, ANIM_LOOP, -1);
	PA_CreateSprite(SUB_SCREEN, CORNERIA, (void*)stagesel, OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, CORNERIA, CORNERIA, CORNERIA, 1, ANIM_LOOP, -1);
#endif
	// loads sprites just like in characterSelect()
	
	int selected = -1; // which stage has been selected, by the static const ints
	
	while(true) {
		if(Pad.Newpress.Start && selected != -1) {
// if start is pressed and a stage is selected
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif
// menu confirmation sound
			fadeOut();
			
			PA_ResetSpriteSysScreen(SUB_SCREEN); // resets sprites
			
			if(selected == FINALDESTINATION) stagename = "finaldestination";
			if(selected == POKEMONSTADIUM) stagename = "pokemonstadium";
#ifdef SLOPEDSTAGES_ON
			if(selected == CASTLESEIGE) stagename = "castleseige";
			if(selected == CORNERIA) stagename = "corneria";
#endif
			// set stagename based on selected
		
			return;
		}
		if(Stylus.Newpress) {
			for(int n = 0; n < 10; n++) { // through all possible stages
				if(PA_SpriteTouched(n)) {
#ifdef SFX_ON
					AS_SoundQuickPlay(menuconfirm);
#endif
					// menu confirm sound

					if(n == 1) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/finaldestinationprev.gif");
					if(n == 2) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/pokemonstadiumprev.gif");
#ifdef SLOPEDSTAGES_ON
					if(n == 3) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/castleseigeprev.gif");
					if(n == 4) openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/corneriaprev.gif");
#endif
					// stage preview depending on what the new stage is
					
					selected = n; // sets selected stage, just like in characterSelect()

				}
			}
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void characterSelect() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/blank.gif");
	// blank background
	
	PA_InitText(SUB_SCREEN, 0); // init text on sub screen
	PA_SetTextCol(SUB_SCREEN, 0,0,0); // text color of black

	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)charsel_Pal);
	// creates a paleete... see mainMenu() for more details
	// loads palette with all character selection icons, so it only uses 1 palette for all the sprites
	PA_CreateSprite(SUB_SCREEN, KIRBY, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 0, 0);
	// creates a sprite... see mainMenu() 
	PA_StartSpriteAnimEx(SUB_SCREEN, KIRBY, KIRBY, KIRBY, 1, ANIM_LOOP, -1);
	// animates a sprite Arguments are:
	// - screen the sprite is on
	// - reference number to the sprite
	// - starting frame of sprite animation
	// - ending frame of sprite animation
	// - FPS of animation (doesn't matter in this case b/c it's the same frame the whole time)
	// - the type of animation, in this casea loop
	// - how long to loop for (-1 is infinite)	
	PA_CreateSprite(SUB_SCREEN, MEWTWO, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 64, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MEWTWO, MEWTWO, MEWTWO, 1, ANIM_LOOP, -1);
	// another sprite
	PA_CreateSprite(SUB_SCREEN, MARIO, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 128, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, MARIO, MARIO, MARIO, 1, ANIM_LOOP, -1);
	// a third sprite
	PA_CreateSprite(SUB_SCREEN, IKE, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 192, 0);
	PA_StartSpriteAnimEx(SUB_SCREEN, IKE, IKE, IKE, 1, ANIM_LOOP, -1);
#ifdef SHADOW_ON
	PA_CreateSprite(SUB_SCREEN, SHADOW, (void*)charsel, OBJ_SIZE_64X64, COLOR256, 0, 0, 64);
	PA_StartSpriteAnimEx(SUB_SCREEN, SHADOW, SHADOW, SHADOW, 1, ANIM_LOOP, -1);
#endif
	// etc, etc, etc

	PA_LoadSpritePal(MAIN_SCREEN, 0, (void*)charprev_Pal);
	PA_CreateSprite(MAIN_SCREEN, 0, (void*)charprev, OBJ_SIZE_64X64, COLOR256, 0, 0, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 0, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 1, (void*)charprev, OBJ_SIZE_64X64, COLOR256, 0, 64, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 1, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 2, (void*) charprev, OBJ_SIZE_64X64, COLOR256, 0, 128, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 2, 0, 0, 1, ANIM_LOOP, -1);
	PA_CreateSprite(MAIN_SCREEN, 3, (void*) charprev, OBJ_SIZE_64X64, COLOR256, 0, 192, 128);
	PA_StartSpriteAnimEx(MAIN_SCREEN, 3, 0, 0, 1, ANIM_LOOP, -1);	
	// loads and animates character previews on top screen.
	// frame 0 is blank

	fadeIn();
#ifdef SFX_ON	
	AS_SoundQuickPlay(free_for_all);
#endif	
	// plays free for all sound byte

	int humanselected = -1; // which character was chosen for the human
	int cpu1selected = -1; // which character was chosen for computer 1
	int cpu2selected = -1;
	int cpu3selected = -1;
	int selecting = 0; // who is currently selecting the character 
	// (I'll change to cursors like the previous smashes later)
	// 0 = human, 1 = cpu1... 2 and 3 would be for other CPUs if I get that far

	while(true) {
		if(Pad.Newpress.R) selecting++;
		if(Pad.Newpress.L) selecting--;
		// changes who is choosing
		if(selecting < 0) selecting = 3;
		if(selecting > 3) selecting = 0;
		// loops around selection number: max number of players is 1
		PA_OutputText(SUB_SCREEN, 7, 23, "Select for player %d", selecting+1);
		// prints who is being selected for... like I said: cursors will come later
		if(Pad.Newpress.Start && humanselected != -1 && cpu1selected != -1) {
// if start is pressed and both players are ready
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif			
			// menu confirmation sound byte
			fadeOut();
			PA_ResetSpriteSys(); // restes all sprites
			PA_OutputText(SUB_SCREEN, 7, 23, "                     "); // clears text
			if(humanselected == KIRBY) players.push_back(new Kirby(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == MEWTWO) players.push_back(new Mewtwo(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == MARIO) players.push_back(new Mario(512/2 -96 -32, 256/3 -32, 1));
			else if(humanselected == IKE) players.push_back(new Ike(512/2 -96 -32, 256/3 -33, 1));		  
#ifdef SHADOW_ON
			else if(humanselected == SHADOW) players.push_back(new Shadow(512/2 -96 -32, 256/3 -32, 1));
#endif	
			// adds a new player class (fighter*) for the human
					
			if(cpu1selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 2, true));
			else if(cpu1selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 2, true));		 
#ifdef SHADOW_ON
			else if(cpu1selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 2, true));
#endif			

			if(cpu2selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 3, true));
			else if(cpu2selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 3, true));		 
#ifdef SHADOW_ON
			else if(cpu2selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 3, true));
#endif					

			if(cpu3selected == KIRBY) players.push_back(new Kirby(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MEWTWO) players.push_back(new Mewtwo(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == MARIO) players.push_back(new Mario(512/2 +96 -32, 256/3 -32, 4, true));
			else if(cpu3selected == IKE) players.push_back(new Ike(512/2 +96 -32, 256/3 -32, 4, true));		 
#ifdef SHADOW_ON
			else if(cpu3selected == SHADOW) players.push_back(new Shadow(512/2 +96 -32, 256/3 -32, 4, true));
#endif		
			// adds a new player class (fighter*) for the cpu1
			
			return;
		}
		if(Stylus.Newpress) {
// if the screen is touched
			bool spritetouched = false;
			for(int n = 0; n < 10; n++) { // through the last character number
				if(PA_SpriteTouched(n)) {
					spritetouched = true;
#ifdef SFX_ON
					if(n == KIRBY) AS_SoundQuickPlay(kirby);
//					else if(n == MEWTWO) AS_SoundQuickPlay(mewtwo);
					else if(n == MARIO) AS_SoundQuickPlay(mario);
					else if(n == IKE) AS_SoundQuickPlay(ike);
//					else if(n == SHADOW) AS_SoundQuickPlay(shadow);
#endif
					// plays a sound byte of the player's name
					if(selecting == 0) {
						humanselected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 0, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 1) {
						cpu1selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 1, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 2) {
						cpu2selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 2, n, n, 1, ANIM_LOOP, -1);
					}
					else if(selecting == 3) {
						cpu3selected = n;
						PA_StartSpriteAnimEx(MAIN_SCREEN, 3, n, n, 1, ANIM_LOOP, -1);
					}
// changes the character number of the selecting player to the character 
// that was clicked and changes the frame of that player's character preview
// if kirby was clicked, the humanselected/cpu1selected = 1 (or static const int KIRBY)
// I told you those public variables would come in handy
				}
			}
			if(!spritetouched) {
				if(selecting == 0) {
					humanselected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 0, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 1) {
					cpu1selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 1, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 2) {
					cpu2selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 2, -1, -1, 1, ANIM_LOOP, -1);
				}
				else if(selecting == 3) {
					cpu3selected = -1;
					PA_StartSpriteAnimEx(MAIN_SCREEN, 3, -1, -1, 1, ANIM_LOOP, -1);
				}
			}	
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}

// in-game functions
void Pause() {
	for(int n = 0; n < players.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, players[n] -> SPRITENUM, 1);
	} // pauses all player animations
	// - screen
	// - sprite number
	// - on/off (1/0)
	for(int n = 0; n < effects.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, effects[n].mynum, 1);
	} // pauses all effect animations
	for(int n = 0; n < projectiles.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, projectiles[n].num, 1);
	} // pauses all projectile animations
	PA_WaitForVBL();
	PA_OutputText(MAIN_SCREEN, 12, 10, "PAUSED"); // prints that it's paused
	while(!Pad.Newpress.Start) {
		PA_WaitForVBL();
	} // waits for unpause
	PA_OutputText(MAIN_SCREEN, 12, 10, "          "); // clears text
	for(int n = 0; n < players.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, players[n] -> SPRITENUM, 0);
	} // unpauses players
	for(int n = 0; n < effects.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, effects[n].mynum, 0);
	} // unpauses effects
	for(int n = 0; n < projectiles.size(); n++) {
		PA_SpriteAnimPause(MAIN_SCREEN, projectiles[n].num, 0);
	} // unpauses projectiles
} // pauses the game; will add pause menu later
void scrollScreen() {
	if(cameratype == CAMERATYPE_FOLLOWALL) {
		double maxx = players[0] -> x;
		double minx = players[0] -> x;
		double maxy = players[0] -> y;
		double miny = players[0] -> y;
		// initializes min and max x and y
		for(int n = 0; n < players.size(); n++) {
			double x = players[n] -> x;
			double y = players[n] -> y;
			if(x > maxx) maxx = x;
			if(x < minx) minx = x;
			if(y > maxy) maxy = y;
			if(y < miny) miny = y;
		} // finds the minimum and maximum x and y position
		scrollx = (maxx + minx) / 2;
		scrolly = (maxy + miny) / 2;
		// scrolls so that the camera is centred between the min and max x and y
	} // if the camera follows everyone
	else if(cameratype == CAMERATYPE_FOLLOWUSER) {
		scrollx = players[0] -> x;
		scrolly = players[0] -> y;
		// centers camera on user
	} // if the camer follows the user
	scrollx = scrollx - 128 + 32;
	scrolly = scrolly - 96 + 32; 
	// centers the camera on the sprites and screen
	if(scrollx < -128) scrollx = -128;
	if(scrolly < -192) scrolly = -192;
	if(scrollx > 512-256+128) scrollx = 512-256+128;
	if(scrolly > 256-192) scrolly = 256-192;
	// wraps scrolling around
	PA_LargeScrollX(MAIN_SCREEN, 0, 256+(int)scrollx);
	PA_LargeScrollY(MAIN_SCREEN, 0, 256+(int)scrolly);
	// scrolls the screen
	for(int n = 0; n < players.size(); n++) {
		players[n] -> scroll(scrollx, scrolly);
	} // scrolls the players
#ifdef SFX_ON
	for(int n = 0; n < effects.size(); n++) {
		if(effects[n].type == FX_DEATH) {} // don't move sprite
		else if(effects[n].type == FX_AIRJUMP) PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM)+32);
		else PA_SetSpriteXY(MAIN_SCREEN, effects[n].mynum, PA_GetSpriteX(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM), PA_GetSpriteY(MAIN_SCREEN, players[effects[n].playernum] -> SPRITENUM));
	} // scrolls the special effects
#endif
}
void displaySubScreen() {
	if(players.size() >= 1) PA_SetSpriteXY(SUB_SCREEN, P1MINI, (int)((players[0] -> x)/8 + 64 + 256/8), (int)((players[0] -> y)/8 + 64 + 256/8));
	if(players.size() >= 2) PA_SetSpriteXY(SUB_SCREEN, P2MINI, (int)((players[1] -> x)/8 + 64 + 256/8), (int)((players[1] -> y)/8 + 64 + 256/8));
	if(players.size() >= 3) PA_SetSpriteXY(SUB_SCREEN, P3MINI, (int)((players[2] -> x)/8 + 64 + 256/8), (int)((players[2] -> y)/8 + 64 + 256/8));
	if(players.size() >= 4) PA_SetSpriteXY(SUB_SCREEN, P4MINI, (int)((players[3] -> x)/8 + 64 + 256/8), (int)((players[3] -> y)/8 + 64 + 256/8));
	// displays the two sprites in the correct position on a scaled down map
	PA_SetSpriteXY(SUB_SCREEN, MINIBOX, (int)((scrollx/8)+64 + 256/8), (int)((scrolly/8)+64 + 256/8));
	// displays a box representing the current screen
} // displays the minimap
void displayPercentages() {
	PA_OutputText(SUB_SCREEN, 0, 0, "                                           ");
	PA_OutputText(SUB_SCREEN, 0, 23, "                                           ");
	// clears the text
	if(players.size() >=1) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_RED);
		int damage = (int)(players[0]->getDamagePercent());
		PA_OutputText(SUB_SCREEN, 0, 0, "%d %", damage);
	} // displays damage percent of player 1
	if(players.size() >= 2) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_BLUE);
		int damage = (int)(players[1]->getDamagePercent());
		stringstream ss;
		ss << damage;
		int length = ss.str().size();
		PA_OutputText(SUB_SCREEN, 30-length, 0, "%d %", damage);
	} // displays damage percent of player 2
	if(players.size() >= 3) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_YELLOW);
		int damage = (int)(players[2]->getDamagePercent());
		PA_OutputText(SUB_SCREEN, 0, 23, "%d %", damage);
	} // damage of player 3
	if(players.size() >= 4) {
		PA_SetTextTileCol(SUB_SCREEN, TEXT_GREEN);
		int damage = (int)(players[3]->getDamagePercent());
		stringstream ss;
		ss << damage;
		int length = ss.str().size();
		PA_OutputText(SUB_SCREEN, 30-length, 23, "%d %", damage);
	}
}

void displayResults() {		
	PA_ResetBg(MAIN_SCREEN);
	PA_ResetBg(SUB_SCREEN);
	// resets the background for both screens
	
	PA_Init8bitBg(MAIN_SCREEN, 3);
	PA_Init8bitBg(SUB_SCREEN, 3);
	// initializes a gif on both screens

	for(int n = 0; n < MINIBOX+1; n++) {
		PA_StopSpriteAnim(SUB_SCREEN,n);
		PA_DeleteSprite(SUB_SCREEN,n);
	} // stops and deletes minimap object sprites
#ifdef SFX_ON
	for(int n = 5; n < 20; n++) {
		PA_StopSpriteAnim(MAIN_SCREEN, n);
		PA_DeleteSprite(MAIN_SCREEN, n);
	} // stops and deletes special effects
#endif
#ifdef PROJECTILES_ON
	for(int n = 50; n < 55; n++) {
		PA_StopSpriteAnim(MAIN_SCREEN, n);
		PA_DeleteSprite(MAIN_SCREEN, n);
	} // stops and deletes projectile sprites
#endif
	for(int n = 0; n < players.size(); n++) {
		PA_SetSpriteXY(MAIN_SCREEN, players[n] -> SPRITENUM, -64,-64);
	} // stops the characters from moving
	
	int winner = 0; // winner of the game, based on location in players
	bool draw = false; // whether or not the winners are tied
	for(int n = 1; n < players.size(); n++) {
		if(score.getTotal(n) > score.getTotal(winner)) {
			draw = false;
			winner = n;
		} // if the winner's score is surpassed, there is a new leader
		else if(score.getTotal(n) == score.getTotal(winner)) draw = true;
		// or if the winner's score is equaled, there is a draw
	}
	// calculates winner
	
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/smashball.gif");
	// loads an image on the sub screen
	
	if(draw) {} // doesn't display a main screen bg
	else {
		if(players[winner] -> name == "kirby") {
			if(players[winner] -> charnum == 0) { 
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/kirbywin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/kirbywin2.gif");
			}
		}
		if(players[winner] -> name == "mewtwo") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/pokemonwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/pokemonwin2.gif");
			}
 		}
		if(players[winner] -> name == "mario") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/mariowin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/mariowin2.gif");
			}
		}
		if(players[winner] -> name == "ike") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/fireemblemwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/fireemblemwin2.gif");
			}
		}
#ifdef SHADOW_ON
		if(players[winner] -> name == "shadow") {
			if(players[winner] -> charnum == 0) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/shadowwin1.gif");
			}
			if(players[winner] -> charnum == 1) {
				openGif(MAIN_SCREEN, "SSBDS_Files/gifs/shadowwin2.gif");
			}
		}
#endif
	} 
	// displays the series icon of the winner in the winner's color
	
	PA_InitText(MAIN_SCREEN, 1);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	PA_OutputSimpleText(MAIN_SCREEN, 0, 8, "Total:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 11, "Kills:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 14, "Deaths:");
	PA_OutputSimpleText(MAIN_SCREEN, 0, 17, "SDs:");
	for(int n = 0; n < players.size(); n++) {
		int total = score.getTotal(n);
		int kills = score.getKills(n);
		int deaths = score.getDeaths(n);
		int SDs = score.getSDs(n);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 8, "%d", total);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 11, "%d", kills);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 14, "%d", deaths);
		PA_OutputText(MAIN_SCREEN, (n*6)+9, 17, "%d", SDs);
	}
	// prints the scoreboard stats for each player
	
	for(int n = 0; n < players.size(); n++) {
		players[n] -> fall();
		players[n] -> idle(); 
		// ensures that it goes into idling animation
		PA_SetSpriteHflip(MAIN_SCREEN, players[n] -> SPRITENUM, 0);
		PA_SetSpriteXY(MAIN_SCREEN, players[n] -> SPRITENUM, (int)(n*48) +48, 0);
	} 
	// moves character sprites into position above their scores

	fadeIn();

#ifdef SFX_ON
	if(draw) {
		AS_SoundQuickPlay(nocontest);
		for(int n = 0; n < 90; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying nobody won
	else {
		AS_SoundQuickPlay(thewinneris);
		for(int n = 0; n < 80; n++) {
			PA_WaitForVBL();
		}
		if(players[winner] -> name == "kirby") AS_SoundQuickPlay(kirby);
//		else if(players[winner] -> name == "mewtwo") AS_SoundQuickPlay(mewtwo);
		else if(players[winner] -> name == "mario") AS_SoundQuickPlay(mario);
		else if(players[winner] -> name == "ike") AS_SoundQuickPlay(ike);
//		else if(players[winner] -> name == "shadow") AS_SoundQuickPlay(shadow);
						
		for(int n = 0; n < 60; n++) {
			PA_WaitForVBL();
		}
	} // plays a sound clip saying ther winner
#endif

	while(true) {
		if(Stylus.Newpress) {
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
#endif			
			fadeOut();
			score.clear(); // clears the scoreboard
			effects.clear(); // clears the effects
			for(int n = 0; n < players.size(); n++) {
				players[n] -> deleteSprite();
				delete players[n];
			} // deletes the sprites of all players
			players.clear(); // clears players vector
			PA_FatFreeSprBuffers(); // frees sprite loading space
			for(int n = 0; n < 16; n++) AS_SoundStop(n);
			// stop sounds
			scrollx = 0;
			scrolly = 0;
			// resets game scrolls
			return;
		} // returns to title screen
		PA_WaitForVBL();
	}
 }

// game modes
void timeMatch(int minutes) {
	int time = minutes*60*60 + 60; // minutes -> vblanks
	characterSelect(); // select characters
	stageSelect(); // select stage
#ifdef SFX_ON
	initFX(); // inits the special FX
#endif	
#ifdef PROJECTILES_ON
	initProjectiles(); // inits the projectiles
#endif
			
	Stage stage = setStage(stagename); 
	// sets the stage to the stage chosen in stageSelect
	PA_InitText(MAIN_SCREEN,1); // inits text on the main screen (displays time)
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white
	
	initMinimap(stagename); // inits minimap
	PA_InitText(SUB_SCREEN,1); // inits test on sub screen (displays percentages)
	PA_SetTextCol(SUB_SCREEN, 31,31,31); // text color = white

	// initializes scoreboard
	score = Scoreboard(players.size()); // initializes a new scoreboard

	fadeIn();

#ifdef SFX_ON		
	AS_SoundQuickPlay(three);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(two);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(one);
	for(int n = 0; n < 60; n++) {
		PA_WaitForVBL();
	}
	AS_SoundQuickPlay(go);
#endif	
	// counts down to start game
																																										
	while(true) {
		PA_CheckLid(); // if the lid is closed it pauses
		if(Pad.Newpress.Start) Pause(); 
		// checks to see if the game was paused by start button
		if(time-60 == 0) {
#ifdef SFX_ON
			AS_SoundQuickPlay(game);
#endif
		// end of game sound clip
			for(int n = 0; n < players.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, players[n] -> SPRITENUM);
			// stops sprite anim for all players
#ifdef PROJECTILES_ON
			for(int n = 0; n < projectiles.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, projectiles[n].num);
#endif			
			// stops all projectile animations
#ifdef SFX_ON
			for(int n = 0; n < effects.size(); n++) PA_StopSpriteAnim(MAIN_SCREEN, effects[n].mynum);
#endif			
			// stops all effect animations
			PA_OutputText(MAIN_SCREEN, 13, 0, "0:00"); // displays 0 as the time
			for(int n = 0; n < 60; n++) PA_WaitForVBL(); // waits for 1 second
			fadeOut();
			return displayResults();
		}
		for(int n = 0; n < players.size(); n++) {
			players[n] -> act();
		} // all players act
		for(int n = 0; n < players.size(); n++) {
			for(int m = 0; m < players.size(); m++) {
				if(m != n) {
					players[m] = players[n] -> checkHits(players[m]);
				}
			}
		}
		// checks to see if any player hit another
		scrollScreen(); // scrolls the screen
#ifdef PROJECTILES_ON
		for(int n = 0; n < projectiles.size(); n++) {
			projectiles[n].act();
			for(int m = 0; m < players.size(); m++) {
				if(projectiles[n].owner != m) players[m] = projectiles[n].checkHits(players[m]);
			}
		}
#endif
		// acts and checks intersections of all projectiles
#ifdef SFX_ON
		for(int n = 0; n < effects.size(); n++) {
			effects[n].act();
		}
#endif
		// acts all effects
		displaySubScreen(); // changes sub screen display
		displayPercentages(); // displays percentages on sub screen
		PA_OutputText(MAIN_SCREEN, 13,0, "          "); // clears time
		if((int)((time/60)%60) < 10) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:0%d",(int)((time/60)/60), (int)((time/60)%60));
		else if((int)((time/60)%60) == 0) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:00",(int)((time/60)/60));
		else PA_OutputText(MAIN_SCREEN, 13, 0, "%d:%d", (int)((time/60)/60), (int)((time/60)%60));		
		// redisplays time
		printMemoryUsage();
		PA_WaitForVBL();
		time--; // another tick off the clock!
	}
}
void stockMatch(int stockcount) {

} // stock match, uncoded
void trainingMode() {

} // training mode, uncoded
void options() {

} // options menu, uncoded
void extras() {

} // extras menu, uncoded

/*
#ifdef LAN_ON
LPLOBBY_USER roomOwners[MAX_ROOMS];
LPLOBBY_ROOM myRoom;
char* roomNames[ROOM_NAMES] = {
	"SSBDS 1",
	"SSBDS 2",
	"SSBDS 3",
	"SSBDS 4",
	"SSBDS 5",
	"SSBDS 6",
	"SSBDS 7",
	"SSBDS 8"
};
int roomNameIndex = 0;
int roomMaxUsers = 2;
unsigned short roomGameCode = 558;
unsigned short roomGameVersion = 1;
int roomVisible = 1;
int roomCount = 0;
int logCount = 0;
int menuState = MENU_MAIN;
int selected = 0 ;

bool inlan = false;
void customVBL(void) {
	if(inlan) {
		IPC_RcvCompleteCheck();
		LOBBY_Update();
	}
	AS_SoundVBL();
}
void receiveGameData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	// update character based on data string
}
int roomSlotOf(LPLOBBY_USER user) {
	for(int n = 0; n < LOBBY_GetUsercountInRoom(myRoom); n++) {
		if(user==LOBBY_GetRoomUserBySlot(myRoom, n)) return n;
	}
}
void receiveCharData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(data == (unsigned char*)"kirby") {
		players[roomSlotOf(from)] = new Kirby(512/2 -32, 256/3 -32, roomSlotOf(from), 1);
	}
	if(data == (unsigned char*)"mewtwo") {
		players[roomSlotOf(from)] = new Mewtwo(512/2 -32, 256/3 -32, roomSlotOf(from), 1);
	}
}
void receiveUserData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(++logCount > 10) logCount=0;
	PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user msg %s: %s", LOBBY_GetUserName(from), data);
}
void receiveRoomData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	if(++logCount>10) logCount=0;
	PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- room msg %s: %s", LOBBY_GetUserName(from), data);
}
void userCallback(LPLOBBY_USER user, unsigned long reason) {
	if(!inlan) return;
	if(++logCount>10) logCount=0;
	switch(reason) {
		case USERINFO_REASON_REGOGNIZE:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s recognized", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_TIMEOUT:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s timeout", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_RETURN:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s returned", LOBBY_GetUserName(user));
		break;
		case USERINFO_REASON_ROOMCREATED:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s created a room", LOBBY_GetUserName(user));
			for(int i=0;i<roomCount;i++) {
				if(roomOwners[i]==user)
					return;
			}
			roomOwners[roomCount] = user;
			roomCount++;
		break;
		case USERINFO_REASON_ROOMCHANGE:
			PA_OutputText(MAIN_SCREEN, 0, 11+logCount, "- user %s changed room", LOBBY_GetUserName(user));
			if(user==LOBBY_GetUserByID(USERID_MYSELF)) { // join/create room succeeded
				menuState=MENU_LOBBY;
				selected=0;
			}
		break;
	}
}
bool allSelected() {
	for(int n = 0; n < players.size(); n++) {
		if(players[n] == NULL) return false;
	}
	return true;
}
void LANgame() {
	if(!inlan) return;
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SetRoomVisibility(0); // hides room
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SendToRoom(myRoom, 0x0003, (unsigned char*)"STARTED", 10);
	players.clear();
	for(int n = 0; n < LOBBY_GetUsercountInRoom(myRoom); n++) {
		players.push_back(NULL);
	}
	players[roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF))] = new Kirby(512/2 -32, 256/3 -32, roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF)), 1);
	LOBBY_SendToRoom(myRoom, 0x0004, (unsigned char*)"kirby", 10);
	while(!allSelected()) {}
	
	stagename = "finaldestination";
	Stage stage = setStage(stagename);
	PA_InitText(MAIN_SCREEN,1);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	initMinimap(stagename);
	PA_InitText(SUB_SCREEN,1);
	PA_SetTextCol(SUB_SCREEN, 31,31,31);
	
	int time = 1*60*60;
	score = Scoreboard(players.size());
	if(score.playercount == 0) while(true);	

	while(true) {
		if(time-60 == 0) return displayResults();
		players[roomSlotOf(LOBBY_GetUserByID(USERID_MYSELF))] -> act();
		// send character info to room
		// display/update other characters
		// check hits
		
		scrollScreen();
		displaySubScreen();
		displayPercentages();
		PA_OutputText(MAIN_SCREEN, 13,0, "          ");
		if((int)((time/60)%60) < 10) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:0%d",(int)((time/60)/60), (int)((time/60)%60));
		else if((int)((time/60)%60) == 0) PA_OutputText(MAIN_SCREEN, 13, 0, "%d:00",(int)((time/60)/60));
		else PA_OutputText(MAIN_SCREEN, 13, 0, "%d:%d", (int)((time/60)/60), (int)((time/60)%60));		
		PA_WaitForVBL();
		time--;
	}	
	
	if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) LOBBY_SetRoomVisibility(1); // unhides room
}
void receiveStartData(unsigned char *data, int length, LPLOBBY_USER from) {
	if(!inlan) return;
	LANgame();
}
void LAN() {
	int j = 0;

	inlan = true;
	
	IPC_SetChannelCallback(0,&LWIFI_IPC_Callback) ;
	LOBBY_SetStreamHandler(0x0001,&receiveUserData);
	LOBBY_SetStreamHandler(0x0002,&receiveRoomData);
	LOBBY_SetStreamHandler(0x0003,&receiveStartData);
	LOBBY_SetStreamHandler(0x0004,&receiveCharData);
	LOBBY_SetStreamHandler(0x0005,&receiveGameData);
	LOBBY_SetUserInfoCallback(&userCallback);

	PA_VBLFunctionInit(customVBL);

	PA_InitText(MAIN_SCREEN, 0);
	PA_SetTextCol(MAIN_SCREEN, 31,31,31);
	
	int max=1;

	while (1)  {
		// handle input
		switch(menuState) {
			case MENU_MAIN: {
				max = 1+roomCount;
				if(Pad.Newpress.A) {
					if(selected==0) { // go to create room menu
						menuState=MENU_CREATE_ROOM;
						selected=0;
					}
					else { // join existing room
						myRoom = LOBBY_GetRoomByUser(roomOwners[selected-1]);
						LOBBY_JoinRoom(LOBBY_GetRoomByUser(roomOwners[selected-1]));
					}
				}
			}
			break;
	
			case MENU_CREATE_ROOM: {
				max = 5;
				if(Pad.Newpress.Right) {
					switch(selected) {
						case 0:
							if(++roomNameIndex>=ROOM_NAMES) roomNameIndex=0;
						break;

						case 1:
							if(++roomMaxUsers>8) roomMaxUsers=2;
						break;

						case 2:
							if(++roomGameCode>=65535) roomGameCode=1;
						break;

						case 3:
							if(++roomGameVersion>=65535) roomGameVersion=1;
						break;
					}
				}
				if(Pad.Newpress.Left) {
					switch(selected) {
						case 0:
							if(--roomNameIndex<0) roomNameIndex = ROOM_NAMES-1;
						break;

						case 1:
							if(--roomMaxUsers<2) roomMaxUsers=8;
						break;

						case 2:
							if(--roomGameCode<1) roomGameCode=65535;
						break;

						case 3:
							if(--roomGameVersion<1) roomGameVersion=65535;
						break;
					}
				}
				if(Pad.Newpress.B) {
					menuState = MENU_MAIN;
					selected=0;
				}
				if(Pad.Newpress.A) {
					if(selected==4) { // create a new room
						LOBBY_CreateRoom(roomNames[roomNameIndex], roomMaxUsers, roomGameCode, roomGameVersion);
						myRoom = LOBBY_GetRoomByUser(LOBBY_GetUserByID(USERID_MYSELF));
					}
				}
			}
			break;

			case MENU_LOBBY: {
				int inc = (LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)?1:0);
				max = 1 + LOBBY_GetUsercountInRoom(myRoom) + inc;
				if(Pad.Newpress.A) {
					if(inc&&selected==0) { // toggle room visibility
						roomVisible = 1-roomVisible;
						LOBBY_SetRoomVisibility(roomVisible);
					}
					if(selected==inc) { // leave room
						LOBBY_LeaveRoom();
						menuState = MENU_MAIN;
						selected=0;
					}
					else if(selected==inc+1) // send message to all room members
						LOBBY_SendToRoom(myRoom, 0x0002, (unsigned char *)"hi all!", 8);
					else { // send message to a specific room member
						j=selected-inc-2;
						if(LOBBY_GetRoomUserBySlot(myRoom, j)==LOBBY_GetUserByID(USERID_MYSELF)) // skip sending messages to yourself
							j++;
						LOBBY_SendToUser(LOBBY_GetRoomUserBySlot(myRoom, j), 0x0001, (unsigned char *)"hi you!", 8);
					}
				}
				if(Pad.Newpress.Start && LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) {
					LANgame();
				}
			}
			break;
		}
		
		if (Pad.Newpress.Down) {
			if(++selected>=max) selected = 0;
		}
		if (Pad.Newpress.Up) {
			if(--selected<0) selected = max-1;
		}

		// handle output
		for(int i = 0; i < 10; i++) PA_OutputText(MAIN_SCREEN, 0, i, "                                 ");
		j=0;
		switch(menuState) {
			case MENU_MAIN: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== room main ==================");
				PA_OutputText(MAIN_SCREEN, 0, 1, "%s create room", (selected==j?"->":"  ")); j++;
				for(int i=0;i<roomCount;i++) {
					LPLOBBY_ROOM roomInfo = LOBBY_GetRoomByUser(roomOwners[i]);
					if(roomInfo!=NULL) {
						PA_OutputText(MAIN_SCREEN, 0, 2, "%s join room %s: %s [%d/%d]",
							(selected==j?"->":"  "),
							LOBBY_GetUserName(LOBBY_GetRoomUserBySlot(roomInfo, 0)),
							LOBBY_GetRoomName(roomInfo),
							LOBBY_GetUsercountInRoom(roomInfo),
							LOBBY_GetMaxUsercountInRoom(roomInfo)
						);
						j++;
					}
				}
			}
			break;
	
			case MENU_CREATE_ROOM: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== create room ================");
				PA_OutputText(MAIN_SCREEN, 0, 1, "%s room name: %s", (selected==j?"->":"  "), roomNames[roomNameIndex]); j++;
				PA_OutputText(MAIN_SCREEN, 0, 2, "%s max usercount: %d", (selected==j?"->":"  "), roomMaxUsers); j++;
				PA_OutputText(MAIN_SCREEN, 0, 3, "%s gamecode: %d", (selected==j?"->":"  "), roomGameCode); j++;
				PA_OutputText(MAIN_SCREEN, 0, 4, "%s version: %d", (selected==j?"->":"  "), roomGameVersion); j++;
				PA_OutputText(MAIN_SCREEN, 0, 5, "%s create room", (selected==j?"->":"  ")); j++;
			}
			break;

			case MENU_LOBBY: {
				PA_OutputText(MAIN_SCREEN, 0, 0, "=== %s [%d/%d] v%d / code:%d =",
					LOBBY_GetRoomName(myRoom),
					LOBBY_GetUsercountInRoom(myRoom),
					LOBBY_GetMaxUsercountInRoom(myRoom),
					LOBBY_GetRoomGameVersion(myRoom),
					LOBBY_GetRoomGameCode(myRoom)
				);
				if(LOBBY_GetRoomUserBySlot(myRoom, 0)==LOBBY_GetUserByID(USERID_MYSELF)) { // we're leading this room
					PA_OutputText(MAIN_SCREEN, 0, 1, "%s is the room visible: %s", (selected==j?"->":"  "), (roomVisible?"yes":"no")); j++;
				}
				PA_OutputText(MAIN_SCREEN, 0, 2, "%s leave room", (selected==j?"->":"  ")); j++;
				PA_OutputText(MAIN_SCREEN, 0, 3, "%s send msg to all room users", (selected==j?"->":"  ")); j++;
				for(int i=0;i<LOBBY_GetUsercountInRoom(myRoom);i++) {
					if(LOBBY_GetRoomUserBySlot(myRoom, i)!=LOBBY_GetUserByID(USERID_MYSELF)) { // don't display yourself in the list
						PA_OutputText(MAIN_SCREEN, 0, 4+i, "%s send msg to user %s", (selected==j?"->":"  "), LOBBY_GetUserName(LOBBY_GetRoomUserBySlot(myRoom, i))); j++;
					}
				}
			}
			break;
		}
		PA_WaitForVBL() ;
	}
}
#endif
*/

// pre-game menus
void mainMenu() {
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/menu.gif");
	// opens gif background. no need to reinit, just loads over the old gif for this screen.

#ifdef MP3_ON
	AS_MP3StreamPlay("/SSBDS_Files/music/mainmenu.mp3");
	// plays main menu music
#endif
	PA_LoadSpritePal(SUB_SCREEN, 0, (void*)menusolo_Pal);
	// inits a sprite palette Arguments are:
	// - the screen the palette exists on
	// - the reference number to the palette from 0 to 15
	// - a void* cast of the location the paleete exists in... see the all_gfx.c
	PA_CreateSprite(SUB_SCREEN, 0, (void*)menusolo, OBJ_SIZE_64X64, COLOR256, 0, 48, 16);
	// inits a sprite Arguments are:
	// - the screen the sprite exists on
	// - the reference number to the sprite from 0 to 127
	// - a void* cast of the location the sprite exists in... see the all_gfx.c
	// - size of the sprite, almost always we'll use 64X64, but there are other options
	// - the color mode used to display the sprite... always 256 colors that's why I #defined it
	// - the reference number of the palette used to display the sprite, see the palette loading
	// - the x position on the screen from 0 to 255
	// - the y position on the screen form 0 to 191
	
	PA_LoadSpritePal(SUB_SCREEN, 1, (void*)menulan_Pal);
	PA_CreateSprite(SUB_SCREEN, 1, (void*)menulan, OBJ_SIZE_64X64, COLOR256, 1, 256-64-48, 16);
	// another menu button

	PA_LoadSpritePal(SUB_SCREEN, 2, (void*)menuoptions_Pal);
	PA_CreateSprite(SUB_SCREEN, 2, (void*)menuoptions, OBJ_SIZE_64X64, COLOR256, 2, 48, 192-64-16);
	// a third menu button

	PA_LoadSpritePal(SUB_SCREEN, 3, (void*)menuextras_Pal);
	PA_CreateSprite(SUB_SCREEN, 3, (void*)menuextras, OBJ_SIZE_64X64, COLOR256, 3, 256-64-48, 192-64-16);
	// a fourth menu button

	fadeIn();

	while(true) {
		if(Pad.Newpress.B) {
			fadeOut();
			PA_ResetSpriteSysScreen(SUB_SCREEN); // gets rid of menu sprites
			return; // back to title screen
		}
		if(Stylus.Newpress) {
			for(int n = 0; n < 4; n++) { // loops through all sprites (menu buttons)
				if(PA_SpriteTouched(n)) { // checks to see if the referenced sprite was touched
#ifdef SFX_ON			
					AS_SoundQuickPlay(menuconfirm);
					// menu sound byte
#endif
#ifdef MP3_ON
					AS_MP3Stop();
					// stops background music
#endif
					fadeOut();
					PA_ResetSpriteSysScreen(SUB_SCREEN); // resets sprites on sub screen
					if(n == 0) return timeMatch(1);
					if(n == 1) {
#ifdef LAN_ON
						return LAN();
#else
						return;
#endif					
					}
					if(n == 2) {
						return options();
					}
					if(n == 3) {
						return extras();
					}
				}
			}
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}
void titleScreen() {
	PA_ResetBgSys(); // clears all bgs on both screens
	PA_ResetSpriteSys(); // clears all sprites on both screens

	PA_Init8bitBg(SUB_SCREEN, 3); // inits a gif
	openGif(SUB_SCREEN, "/SSBDS_Files/gifs/title2.gif");
	// opens the gif from the path on the sub screen

	PA_Init8bitBg(MAIN_SCREEN, 3);
	openGif(MAIN_SCREEN, "/SSBDS_Files/gifs/blank.gif");
	// opens the gif from the path on the main screen
	PA_InitText(MAIN_SCREEN, 0); // inits text on main screen
	PA_SetTextCol(MAIN_SCREEN, 31,31,31); // text color = white

#ifdef MP3_ON	
	AS_MP3StreamPlay("SSBDS_Files/music/title.mp3");
	// title screen music
#endif
	fadeIn();
	while(true) {
		if(Stylus.Newpress) { // if the stylus is pressed
#ifdef SFX_ON
			AS_SoundQuickPlay(menuconfirm);
			// menu confirm sound byte
#endif
#ifdef MP3_ON
			AS_MP3Stop();
			// stops sound
#endif						
			fadeOut();
			PA_ResetSpriteSys(); // resets sprites
			return mainMenu();
		}
		printMemoryUsage();
		PA_WaitForVBL();
	}
}

int main(int argc, char ** argv) {
	PA_Init();    // Initializes PA_Lib 
	PA_InitVBL(); // Initializes a standard VBlank (FPS handler)

	defaultExceptionHandler(); // "red screen of death" error, hopefully won't happen

	if(!fatInitDefault()) {
		PA_InitText(MAIN_SCREEN, 0);
		PA_SetTextCol(MAIN_SCREEN, 31,31,31);
		PA_OutputText(MAIN_SCREEN, 0,0, "FAT INIT FAILED!!!");
		while(true) {}
	} // Init for libfat. if it fails it freezes with an error
	PA_FatInitAllBuffers(); // Initialize all the memory buffers
	PA_FatSetBasePath("/SSBDS_Files");  // Set a base path
	// initializes external file system. very important!!!

	PA_SetBrightness(MAIN_SCREEN, -31);
	PA_SetBrightness(SUB_SCREEN, -31);
	// screen brightness (not related to backlight setting)
	// -31 = all black
	// 31 = all white
	// 0 = normal

#ifdef LAN_ON
	IPC_Init() ;
	LOBBY_Init() ;
# endif
	// inits LAN functions

	AS_Init(AS_MODE_MP3 | AS_MODE_16CH); // initializes AS_Lib
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND); // or your preferred default sound settings
	AS_SetMP3Loop(true);
	// required both for MP3 and Sound

#ifdef LAN_ON
	PA_VBLFunctionInit(customVBL); // custom VBL function called every frame
#else
	PA_VBLFunctionInit(AS_SoundVBL); // easy way to make sure that AS_SoundVBL() is called every frame
#endif
	
	while(true) titleScreen(); // permanently runs the game
	return 0;
} // End of main()

#ifndef PROJECTILES_H //prevents errors with chained inclusion of headers
#define PROJECTILES_H

#include "hitbox.h"
#include "stage.h"
#include "fighter.h"
#include <PA9.h>
#include <vector>
using std::vector;

static const int SHADOWBALL_SMALL = 0, SHADOWBALL_MEDIUM = 1, SHADOWBALL_LARGE = 2, FINALCUTTER = 3, FIREBALL = 4, FLUDDWATER = 5, IKESWORD = 6, FOXLASER = 7, THUNDERSHOCK = 8, THUNDER1 = 9, THUNDER2 = 10, THUNDER3 = 11, THUNDER4 = 12;
// shortcuts for projectiles

/// Keeps track of anything flying/moving that isn't a Fighter
class Projectile {
	public:
		Display* display;
		double x; ///< x position, in pixels
		double y; ///< y position, in pixels
		double dx; ///< velocity in the x direction, in pixels/frame
		double dy; ///< velocity in the y direction, in pixels/frame
		double maxy, miny;
		Hitbox hit; ///< what part of this projectile can collide with others
		int length;
		int time;
		int TYPE;
		int num;
		int owner; ///< Who fired this projectile? (They're usually immune to it)
		bool enabled;
		Stage* mystage;
		Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage* mine, Display *d);
		bool act();
		void removeSelf();
		Floor checkFloorCollision(int deltax, int deltay);
		Wall checkWallCollision(int deltax, int deltay);
		Ceiling checkCeilingCollision(int deltax, int deltay);
		Fighter* checkHits(Fighter* other);
};
#endif

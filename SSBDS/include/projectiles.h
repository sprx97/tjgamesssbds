#ifndef PROJECTILES_H //prevents errors with chained inclusion of headers
#define PROJECTILES_H

#include "hitbox.h"
#include "stage.h"
#include "fighter.h"
#include <PA9.h>
#include <vector>
using std::vector;

static const int SHADOWBALL_SMALL = 0, SHADOWBALL_MEDIUM = 1, SHADOWBALL_LARGE = 2, FINALCUTTER = 3, FIREBALL = 4, FLUDDWATER = 5, IKESWORD = 6;
// shortcuts for projectiles

/// Keeps track of anything flying/moving that isn't a Fighter
class Projectile {
	public:
		Display* display;
		double x, y, dx, dy;
		double maxy, miny; // for bouncing
		Hitbox hit; ///< what part of this projectile can collide with others
		int length;
		int time;
		int TYPE;
		int num;
		int owner; ///< Who fired this projectile? (They're usually immune to it)
		Stage* mystage;
		Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage* mine, Display *d);
		bool act();
		void removeSelf();
		Fighter* checkHits(Fighter* other);
};
#endif

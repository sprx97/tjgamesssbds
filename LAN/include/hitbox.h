#ifndef HITBOX_H
#define HITBOX_H

#include <vector>
using std::vector;
#include "circle.h"

/// a collection of circles; used to hold all the circles for one frame
class Hitbox {
		int contact; ///< which circle in circles made contact
	public:
		vector<Circle> circles; ///< holds the circles in the hitbox
		bool enabled;
		void addCircle(Circle toadd); ///< adds a circle
		vector<Circle> getCircles(); ///< gets the circles
		bool hits(Hitbox other); ///< checks if this hitbox intersects hitbox other
		Circle getHitCircle(Hitbox other); ///< checks which circle hit hibox other
		void reset();
};
#endif

#ifndef HITBOX_H
#define HITBOX_H

#include <vector> 
using std::vector;
#include "circle.h"

class Hitbox {
	vector<Circle> circles; // Circles in the hitbox
	int contact; // which circle in circles made contact
	public:
		void addCircle(Circle toadd);
		// adds a circle
		vector<Circle> getCircles();
		// gets the circles
		bool hits(Hitbox other);
		// checks if this hitbox intersects hitbox other
		Circle getHitCircle(Hitbox other);
		// checks which circle hit hibox other
		void reset();
}; // a collection of circles; used to hold all the circles for one frame
#endif

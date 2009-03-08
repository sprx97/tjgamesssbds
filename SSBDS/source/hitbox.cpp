#include "hitbox.h"
#include "circle.h"
#include <vector> 
using std::vector;
void Hitbox::addCircle(Circle toadd) { circles.push_back(toadd); }
// adds a circle
vector<Circle> Hitbox::getCircles() { return circles; }
// gets the circles
bool Hitbox::hits(Hitbox other) {
	for(int n = 0; n < (int)circles.size(); n++) {
		for(int m = 0; m < (int)other.getCircles().size(); m++) {
			if(circles[n].intersects(other.getCircles()[m])) {
				contact = n;
				return true;
			}
		}
	}
	return false;
}
// checks if this hitbox intersects hitbox other
Circle Hitbox::getHitCircle(Hitbox other) {
	for(int n = 0; n < (int)circles.size(); n++) {
		for(int m = 0; m < (int)other.getCircles().size(); m++) {
			if(circles[n].intersects(other.getCircles()[m])) {
				return circles[n];
			}
		} 
	}
	return circles[contact];
}
// checks which circle hit hibox other
void Hitbox::reset() { circles.clear(); } // resets the hirbox

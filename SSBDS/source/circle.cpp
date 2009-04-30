#include "circle.h"
#include "global.h"
#include <math.h>
Circle::Circle(double xpos, double ypos, double r, int f) {
	fx = f;
	x = xpos;
	y = ypos;
	radius = r;
	damage = 0;
	setKnockback(0,0,0);
	priority = -1;
} // creates a circle with no knockback and damge -> Defboxes
Circle::Circle(double xpos, double ypos, double r, Knockback knock, double d, int f, int pr) {
	fx = f;
	x = xpos;
	y = ypos;
	radius = r;
	damage = d;
	k = knock;
	priority = pr;
} // creates a circle with knockback and damage -> Atkboxes
double Circle::getX() { return x; }
double Circle::getY() { return y; }
double Circle::getRadius() { return radius; }
// gets values for the circle
bool Circle::intersects(Circle other) {
	double dx = other.getX() - x; // change in x
	double dy = other.getY() - y; // change in y
	double dist = sqrt(dx*dx + dy*dy); 
	return dist < (radius+other.getRadius());
} // checks if this circle intersects Circle other
void Circle::setKnockback(double kx, double ky, double kl) { k.set(kx, ky, kl); }
// changes the knockback
Knockback Circle::getKnockback() { return k; } // gets the knockback

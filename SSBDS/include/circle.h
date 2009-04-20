#ifndef CIRCLE_H
#define CIRCLE_H

#include "knockback.h"
#include "global.h"

class Circle {
	double radius; // radius of the circle
	Knockback k; // what the knockback of the circle is
	public:
		int priority;
		double x, y; // x and y of center of the circle
		double damage; // how much damage the circle deals
		int fx; // what visual effect type
		Circle(double xpos, double ypos, double r, int f=FX_NONE);
		Circle(double xpos, double ypos, double r, Knockback knock, double d, int f=FX_NONE, int pr = 0);
		double getX();
		double getY();
		double getRadius();
		bool intersects(Circle other);
		void setKnockback(double kx, double ky, double kl);
		Knockback getKnockback();
}; // a circle; used for collisions; deals damage

#endif

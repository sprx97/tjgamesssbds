#ifndef CIRCLE_H
#define CIRCLE_H

#include "knockback.h"
#include "global.h"

/// Represents a circle in the code, mostly as part of a hitbox.
class Circle {
	double radius; ///< radius of the circle
	Knockback k; ///< what the knockback of the circle is
	public:
		int priority;
		double x; ///< x position of center of the circle
		double y; ///< y position of center of the circle
		double damage; ///< how much damage the circle deals
		int fx; ///< what visual effect type
		Circle(double xpos, double ypos, double r, int f=FX_NONE);
		Circle(double xpos, double ypos, double r, Knockback knock, double d, int f=FX_NONE, int pr = 0);
		double getX();
		double getY();
		double getRadius();
		bool intersects(Circle other);
		void setKnockback(double kx, double ky, double kl);
		Knockback getKnockback();
};

#endif

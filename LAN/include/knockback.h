#ifndef KNOCKBACK_H
#define KNOCKBACK_H

/// A class which stores how much a fighter will get hit when colliding with a hitbox
class Knockback {
	public:
		double dx; ///< x knockback pixels per frame
		double dy; ///< y knockback pixels per frame
		double length; ///< how many frames the knockback lasts
		Knockback();
		Knockback(double x, double y, double l);
		void set(double x, double y, double l);
};

#endif

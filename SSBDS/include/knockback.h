#ifndef KNOCKBACK_H
#define KNOCKBACK_H

class Knockback {
	public:
		double dx; // x knockback pixels per frame
		double dy; // y knockback pixels per frame
		double length; // how many frames the knockback lasts
		Knockback();
		Knockback(double x, double y, double l);
		void set(double x, double y, double l);
}; // a knockback

#endif

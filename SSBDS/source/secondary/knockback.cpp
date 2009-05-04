#include "knockback.h"
Knockback::Knockback() {} // blank init
Knockback::Knockback(double x, double y, double l) {
	dx = x*2;
	dy = y*2;
	length = l;
} // new knockback, dx, dy, and length for it
void Knockback::set(double x, double y, double l) {
	dx = x*2;
	dy = y*2;
	length = l;
} // sets the knockback's dx, dy, and length

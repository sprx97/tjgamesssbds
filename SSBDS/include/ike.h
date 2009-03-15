#ifndef IKE_H
#define IKE_H

#include "fighter.h"
#include "display.h"
#include <vector>

class Ike: public Fighter {
	int quickdrawcharge;
	int eruptioncharge;
	public:
		// constructor
		Ike(int xpos, int ypos, int num, std::vector<Fighter*> listplayers, Display *disp, bool AI = false);
		// initializes all of the variables
		// initializers
		void playsound(int sndnum);
		void initFrames();
		// actions
		void bside();
		void bup();
		void bdown();
		void bneut();
		void fthrow();
		void bthrow();
		void uthrow();
		void dthrow();
		void jaywalk();
		~Ike();
};
#endif

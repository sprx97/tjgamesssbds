#ifndef MEWTWO_H
#define MEWTWO_H

#include "fighter.h"
#include "display.h"
#include <vector>

class Mewtwo: public Fighter {
	public:
		int shadowballcharge;
		// constructor
		Mewtwo(int xpos, int ypos, int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false);
		// initializes all of the variables
		// initializers
		void playsound(int sndnum);
		// sounds
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
		~Mewtwo();
};
#endif

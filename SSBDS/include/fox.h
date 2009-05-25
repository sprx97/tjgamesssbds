#ifndef FOX_H
#define FOX_H

#include "fighter.h"
#include "display.h"
#include <vector>

class Fox: public Fighter {
	public:
		// constructor
		Fox(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false);
		// initializes all of the variables
		// initializers
		void playsound(int sndnum);
		void initSounds();
		void initPalettes();
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
		~Fox();
};
#endif

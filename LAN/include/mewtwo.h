#ifndef MEWTWO_H
#define MEWTWO_H

#include "fighter.h"
#include "display.h"
#include <vector>

/// A pokemon who likes to get angry for no apparent reason, Mewtwo is a master of psychic energy.
class Mewtwo: public Fighter {
	public:
		int shadowballcharge;
		int upcount;
		int downcount;
		int leftcount;
		int rightcount;
		// constructor
		Mewtwo(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false);
		// initializes all of the variables
		// initializers
		void playsound(int sndnum);
		void initSounds();
		// sounds
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
		void fireshadowball();
		~Mewtwo();
};
#endif

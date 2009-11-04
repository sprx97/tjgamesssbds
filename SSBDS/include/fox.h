#ifndef FOX_H
#define FOX_H

#include "fighter.h"
#include "display.h"
#include <vector>

/// Joining us from Star Fox, Fox McCloud is a fighter who isn't afraid to draw his gun.
class Fox: public Fighter {
	public:
		int upcount;
		int downcount;
		int leftcount;
		int rightcount;
		bool laseragain;
		// constructor
		Fox(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false);
		// initializes all of the variables
		// initializers
		void playsound(int sndnum);
		void initSounds();
		void initPalettes();
		// actions
		void cpu_dospecial();
		double cpu_specialweight();
		void bside();
		void bup();
		void bdown();
		void bneut();
		void fthrow();
		void bthrow();
		void uthrow();
		void dthrow();
		void jaywalk();
		void jabatk();
		~Fox();
};
#endif

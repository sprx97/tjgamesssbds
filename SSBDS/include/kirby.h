#ifndef KIRBY_H
#define KIRBY_H
#include "display.h"
#include "fighter.h"
#include <vector>
class Kirby: public Fighter {
	public:	// constructor
		Kirby(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false) ;// initializes all of the variables
		// initializers
		void initPalettes();
		// sounds
		void playsound(int sndnum);
		void initSounds();
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
		~Kirby();
	private:
		int rockcount;
};
#endif

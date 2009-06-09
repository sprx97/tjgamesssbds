#ifndef MARIO_H
#define MARIO_H
#include "fighter.h"
#include "display.h"
#include <vector>
using std::vector;

class Mario: public Fighter {
	public:
		// constructor
		Mario(int num, vector<Fighter*> *listplayers, Display *disp, bool AI = false); // initializes all of the variables
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
		~Mario();
	private:
		int fluddcharge;
};
#endif

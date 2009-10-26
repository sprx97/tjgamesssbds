#ifndef MARIO_H
#define MARIO_H
#include "fighter.h"
#include "display.h"
#include <vector>
using std::vector;

/// A plumber who never seems to do any plumbing, Mario seems much more interested spending his time saving the princess. Or in this case fighting.
class Mario: public Fighter {
	public:
		// constructor
		Mario(int num, vector<Fighter*> *listplayers, Display *disp, bool AI = false); // initializes all of the variables
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
		~Mario();
	private:
		int fluddcharge;
};
#endif

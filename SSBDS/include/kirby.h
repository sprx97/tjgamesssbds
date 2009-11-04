#ifndef KIRBY_H
#define KIRBY_H
#include "display.h"
#include "fighter.h"
#include <vector>

/// Our favorite puffball. A fighter like the rest
class Kirby: public Fighter {
	public:	// constructor
		Kirby(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false) ;// initializes all of the variables
		// initializers
		void initPalettes();
		// sounds
		void playsound(int sndnum);
		void initSounds();
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
		~Kirby();
	private:
		int rockcount; ///< How much longer the rock (Down-B) will last
};
#endif

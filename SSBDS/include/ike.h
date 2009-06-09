#ifndef IKE_H
#define IKE_H

#include "fighter.h"
#include "display.h"
#include <vector>

/// Ike is a sword wielder who thinks the bigger is better when it comes to swords. And no, that's NOT what she said.
class Ike: public Fighter {
	public:
		// constructor
		Ike(int num, std::vector<Fighter*> *listplayers, Display *disp, bool AI = false);
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
		~Ike();
	private:
		int quickdrawcharge; ///< counter for the charging of ike's quick charge (Side-B)
		int eruptioncharge; ///< counter for the charging of ike's eruption (Neutral-B)
};
#endif

#ifndef SANDBAG_H
#define SANDBAG_H
#include "display.h"
#include "fighter.h"
#include <vector>

///A dummy fighter for testing purposes
class Sandbag: public Fighter {
	public:
		Sandbag(int num, std::vector<Fighter*> *listplayers, Display *display, bool AI = false);
		void act();
		void playsound(int sndnum);
		void initSounds();
		void initPalettes();
		~Sandbag();
};
#endif

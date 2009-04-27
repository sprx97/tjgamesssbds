#ifndef SANDBAG_H
#define SANDBAG_H
#include "display.h"
#include "fighter.h"
#include <vector>
class Sandbag: public Fighter {
	public:
		Sandbag(int num, std::vector<Fighter*> *listplayers, Display *display, bool AI = false);
		void act();
		void initFrames();
		~Sandbag();
};
#endif

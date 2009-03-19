#ifndef DISPLAY_H
#define DISPLAY_H

#include "effect.h"
#include "scoreboard.h"
#include <vector>
using std::vector;
class Display{
	public:
		Display();
		vector<Effect> effects;
		double scrollx;
		double scrolly;
		void addeffect(Effect e);
		void deleffect(Effect e);
		void updateEffects();
		vector<Effect>* getEffects();
		Scoreboard *score;
};
#endif

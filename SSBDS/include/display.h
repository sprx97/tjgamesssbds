#ifndef DISPLAY_H
#define DISPLAY_H

#include "effect.h"
#include "scoreboard.h"
#include <vector>
using std::vector;

/// Keeps track of various aspects of the screen
class Display {
	public:
		Display();
		vector<Effect> effects; ///< list of all the effects displayed onscreen
		double scrollx;
		double scrolly;
		void addeffect(Effect e); ///< add a new effect to be displayed onscreen
		void deleffect(Effect e); ///< remove an effect from the screen
		void updateEffects(); ///< count down and remove old effects which have expired
		vector<Effect>* getEffects(); ///< return the list of effects (effects)
		Scoreboard *score;
};
#endif

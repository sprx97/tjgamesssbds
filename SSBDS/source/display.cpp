#include "effect.h"
#include "display.h"
#include "global.h"
#include <vector>
#include "scoreboard.h"
#include <PA9.h> // DS functions that we use come from here
using std::vector;

Display::Display(){
	score = new Scoreboard(0);
	scrollx=0;
	scrolly=0;
}
void Display::addeffect(Effect e){
	effects.push_back(e);
}
void Display::deleffect(Effect de){
	PA_OutputText(MAIN_SCREEN, 0, 0, "effect deleted");
	vector<Effect> temp;
	for(int n = 0; n < (int)effects.size(); n++) {
		Effect e = effects[n];
		if(e.x != de.x || e.y != de.y || e.delay != de.delay || e.playernum != de.playernum) {
			temp.push_back(e);
		}
	} // keeps the effect
	effects=temp;
}
vector<Effect>* Display::getEffects(){
	return &effects;
}
void Display::updateEffects(){
	PA_OutputText(MAIN_SCREEN, 0, 10, "number of effects in display: %d", effects.size());
	for(int k = 0; k < (int)effects.size();k++){
		if (effects[k].act()){
			deleffect(effects[k]);
			k--; //WARNING: hackish (attempts to keep from skipping an element when one is removed)
		}
	}
}

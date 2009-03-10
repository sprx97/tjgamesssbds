#ifndef EFFECT_H
#define EFFECT_H

// shortcuts for visual effects
class Effect {
	public:
		double x, y; // x and y pos of the effect
		int mynum; // the reference number of the effect
		int delay; // how long the effect waits before changing
		int playernum; // which player the effect is created by
		int type; // what effect type it is
		Effect(double xpos, double ypos, int t, int pn, double scrollx, double scrolly); // creates a new effect
		bool act(); // acts; called every frame
}; // a visual effect
#endif

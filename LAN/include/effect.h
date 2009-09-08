#ifndef EFFECT_H
#define EFFECT_H

/// Stores necessary information to display a visual effect onscreen for more than one frame
class Effect {
	public:
		double x; ///< the x position of the effect
		double y; ///< the y position of the effect
		int mynum; ///< the reference number of the effect
		int delay; ///< how long the effect waits before changing
		int playernum; ///< which player the effect is created by
		int type; ///< what type of effect it is
		Effect(double xpos, double ypos, int t, int pn, double scrollx, double scrolly, int deathrot = -1); ///< creates a new effect
		bool act(); ///< acts; called every frame
};
#endif

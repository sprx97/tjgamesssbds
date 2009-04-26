#ifndef STAGES_H
#define STAGES_H

#include "stage.h"

extern const int FINALDESTINATION, POKEMONSTADIUM, CASTLESIEGE, CORNERIA;
// stage shortcuts just like character shortcuts

class FinalDestination: public Stage {
	public:
		FinalDestination();
		void Initialize(); // initializes all the variables for final destination
}; // the stage Final Destination
class PokemonStadium: public Stage { 
	public:
		PokemonStadium();
		void Initialize(); // initializes the pokemon stadium stage
}; // the stage Pokemon Stadium
#ifdef SLOPEDSTAGES_ON
class CastleSiege: public Stage {
	public:
		CastleSiege();
		void Initialize(); // initializes Castle Siege
}; // the stage Castle Siege
class Corneria: public Stage {
	public:
		Corneria();
		void Initialize(); // initializes Corneria
}; // the stage corneria
#endif
#endif

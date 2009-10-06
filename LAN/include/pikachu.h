#ifndef PIKACHU_H
#define PIKACHU_H
#include "fighter.h"
#include "display.h"
#include <vector>
using std::vector;

/// Mewtwo, I know you're famous, and Imma let you finish, but PIKACHU is the most well-known pokemon OF ALL TIME
class Pikachu: public Fighter {
	public:
		Pikachu(int num, vector<Fighter*> *listplayers, Display *disp, bool AI = false); 
		void playsound(int sndnum);
		void initSounds();
		void initPalettes();
		void bside();
		void bup();
		void bdown();
		void bneut();
		void fthrow();
		void bthrow();
		void uthrow();
		void dthrow();
		void jaywalk();
		~Pikachu();
	private:
		int skullbashcharge;
		int upcount, downcount, leftcount, rightcount, firstdir;
};
#endif

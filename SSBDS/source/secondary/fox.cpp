#include "fox.h"
#include "fighter.h"
#include "display.h"
#include "projectiles.h"
#include <PA9.h>
#include <vector>
using std::vector;

//constructor
Fox::Fox(int num, vector<Fighter*> *listplayers, Display *disp, bool AI) : Fighter(num,listplayers,disp,"fox",AI) {
	weight = .910;
	w2value  = 3.41;
	jumpspeed = 7;
	doublejumpspeed = 4;
	shieldstr = 64;
	runspeed = 5;
	handx = 42;
	handy = 11;
	MYCHAR = FOX;
	topside = 10;
	bottomside = 45;
	rightside = 45;
	leftside = 25;
	gravity = 3;
	jumpmax = 2;
	initPalettes();
	initFrames();
	initSprite();
	idle();
} // initializes all of the variables
// initializers
void Fox::initSounds() {
	int alreadymade = 0;
	for(int n = 0; n < charnum; n++) {
		if(players[n] -> MYCHAR == MYCHAR) alreadymade++;
	}
	if(alreadymade == 0) {
	
	}
}
void Fox::initPalettes() {
	palettes.push_back("fox");
}
// sounds
void Fox::playsound(int sndnum) {

}
// actions
void Fox::bside() {

}
void Fox::bup() {

}
void Fox::bdown() {

}
void Fox::bneut() {

}
void Fox::fthrow() {

}
void Fox::bthrow() {

}
void Fox::uthrow() {


}
void Fox::dthrow() {

}
void Fox::jaywalk() {

}
Fox::~Fox() {
	allatkbox.clear();
	alldefbox.clear();
	PA_DeleteSprite(MAIN_SCREEN, SPRITENUM);
	PA_FatFreeSprite(MYCHAR);
}

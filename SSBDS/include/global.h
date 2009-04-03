#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>

#define MAIN_SCREEN 1 // top screen
#define SUB_SCREEN 0 // bottom screen
#define COLOR256 1 // 256 color mode


extern const int FX_NONE, FX_WEAKERHIT, FX_WEAKHIT, FX_STRONGHIT, FX_AIRJUMP, FX_DEATH;

extern int BUTTON_NONE, BUTTON_A, BUTTON_B, BUTTON_AB, BUTTON_X, BUTTON_Y, BUTTON_L, BUTTON_R; 
// actions (for custom controls)
extern int PAD_HELD, PAD_NEWPRESS, PAD_RELEASED; // Press types (for custom controls)
// buttons (for custom controls)
extern int ACTION_BASIC, ACTION_SPECIAL, ACTION_SMASH, ACTION_JUMP, ACTION_JUMP2, ACTION_SHIELD, ACTION_SHIELD2, ACTION_GRAB;
extern const int SANDBAG, KIRBY, MEWTWO, MARIO, IKE, SHADOW; 
// character shortcuts; used to avoid confusion

bool custom_action(int action, int typecheck); //defined in main.cpp. maps custom controls
#include <map> // maps
std::map<int , int> getcustomcontrols();
bool getTapJumpOn();
bool getCStickStylus();
bool getShieldGrabOn();
void* getProj();//FIXME: Hack
void removeProj(void* projectile);
void Pause();

#endif

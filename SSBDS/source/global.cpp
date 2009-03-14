#include "global.h"

const int FX_NONE = -1, FX_WEAKERHIT = 0, FX_WEAKHIT = 1, FX_STRONGHIT = 2, FX_AIRJUMP = 3, FX_DEATH = 4;

int BUTTON_NONE = -1, BUTTON_A = 0, BUTTON_B = 1, BUTTON_AB = 2, BUTTON_X = 3, BUTTON_Y = 4, BUTTON_L = 5, BUTTON_R = 6; 
// actions (for custom controls)
int PAD_HELD = 0, PAD_NEWPRESS = 1, PAD_RELEASED = 2; // Press types (for custom controls)
// buttons (for custom controls)
int ACTION_BASIC = 0, ACTION_SPECIAL = 1, ACTION_SMASH = 2, ACTION_JUMP = 3, ACTION_JUMP2 = 4, ACTION_SHIELD = 5, ACTION_SHIELD2 = 6, ACTION_GRAB = 7;

const int SANDBAG = 0, KIRBY = 1, MEWTWO = 2, MARIO = 3, IKE = 4, SHADOW = 5; 
// character shortcuts; used to avoid confusion

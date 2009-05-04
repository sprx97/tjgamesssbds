int DAMAGE_LOW = 1, DAMAGE_MID = 12, DAMAGE_HIGH = 23, DAMAGE_HIGHER = 34;
void initMinimap(int selStage) {
	if(selStage == FINALDESTINATION) {
		PA_EasyBgLoad(SUB_SCREEN, 3, finaldestinationminimap);
	}
	else if(selStage == POKEMONSTADIUM) {
		PA_EasyBgLoad(SUB_SCREEN, 3, pokemonstadiumminimap);
	}
//	else if(selStage == CORNERIA) {
//		PA_EasyBgLoad(SUB_SCREEN, 3, corneria);
//	}
//	else if(selStage == CASTLESIEGE) {
//		PA_EasyBgLoad(SUB_SCREEN, 3, castleseige);
//	}

	PA_FatEasyLoadSpritePal(SUB_SCREEN, 1, "charminis");
	PA_FatLoadSprite(31, "charminis");
	for(int n = 0; n < (int)players.size(); n++) {
		PA_CreateSprite(SUB_SCREEN, n+1, (void*)sprite_gfx[31], OBJ_SIZE_16X16, COLOR256, 1, -16, -16);
		if(players[n] -> name == "kirby") PA_StartSpriteAnimEx(SUB_SCREEN, n+1, KIRBY, KIRBY, 1, ANIM_LOOP, -1);
		else if(players[n] -> name == "mewtwo") PA_StartSpriteAnimEx(SUB_SCREEN, n+1, MEWTWO, MEWTWO, 1, ANIM_LOOP, -1);
		else if(players[n] -> name == "mario") PA_StartSpriteAnimEx(SUB_SCREEN, n+1, MARIO, MARIO, 1, ANIM_LOOP, -1);
		else if(players[n] -> name == "ike") PA_StartSpriteAnimEx(SUB_SCREEN, n+1, IKE, IKE, 1, ANIM_LOOP, -1);
	}
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 0, "visibleminimap");
	PA_FatLoadSprite(30, "visibleminimap");
	PA_CreateSprite(SUB_SCREEN, 0, (void*)sprite_gfx[30], OBJ_SIZE_64X64, COLOR256, 0, -64, -64);
	// loads the sprite icons for the minimap

	PA_FatEasyLoadSpritePal(SUB_SCREEN, 11, "damageicons1");
	PA_FatLoadSprite(11, "damageicons1");
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 12, "damageicons2");
	PA_FatLoadSprite(12, "damageicons2");
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 13, "damageicons3");
	PA_FatLoadSprite(13, "damageicons3");
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 14, "damageicons4");
	PA_FatLoadSprite(14, "damageicons4");
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 15, "damageiconsC");
	PA_FatLoadSprite(15, "damageiconsC");

	for(int n = 0; n < (int)players.size(); n++) {
		int x = 0;
		int y = 0;
		int icons = 15;
		int palnum = 0;
		if(players[n]->SPRITENUM-100 == 1) {
			x = 0;
			y = -12;
			icons = 11;
			palnum = 11;
		}
		else if(players[n]->SPRITENUM-100 == 2) {
			x = 64;
			y = -12;
			icons = 12;
			palnum = 12;
		}
		else if(players[n]->SPRITENUM-100 == 3) {
			x = 128;
			y = -12;
			icons = 13;
			palnum = 13;
		}
		else if(players[n]->SPRITENUM-100 == 4) {
			x = 192;
			y = -12;
			icons = 14;
			palnum = 14;
		}
		if(players[n]->isCPU) {
			icons = 15;
			palnum = 15;
		}
		PA_CreateSprite(SUB_SCREEN, 80+n, (void*)sprite_gfx[icons], OBJ_SIZE_64X64, COLOR256, palnum, x, y);
		PA_StartSpriteAnimEx(SUB_SCREEN, 80+n, players[n]->MYCHAR, players[n]->MYCHAR, 20, ANIM_LOOP, -1);
	}
	PA_FatEasyLoadSpritePal(SUB_SCREEN, 2, "numbers");
	PA_FatLoadSprite(29, "numbers");
	for(int n = 0; n < (int)players.size(); n++) {
		int x = 0;
		int y = 0;
		if(players[n]->SPRITENUM-100 == 1) {
			x = 28;
			y = 16;
		}
		else if(players[n]->SPRITENUM-100 == 2) {
			x = 92;
			y = 16;
		}
		else if(players[n]->SPRITENUM-100 == 3) {
			x = 156;
			y = 16;
		}
		else if(players[n]->SPRITENUM-100 == 4) {
			x = 220;
			y = 16;
		}
		PA_CreateSprite(SUB_SCREEN, 50 + (n+1)*4, (void*)sprite_gfx[29], OBJ_SIZE_16X16, COLOR256, 2, x, y);
		PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4, 0, 0, 20, ANIM_LOOP, -1);
		PA_CreateSprite(SUB_SCREEN, 50 + (n+1)*4 + 1, (void*)sprite_gfx[29], OBJ_SIZE_16X16, COLOR256, 2, x+8, y);
		PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+1, 1, 1, 20, ANIM_LOOP, -1);
		PA_CreateSprite(SUB_SCREEN, 50 + (n+1)*4 + 2, (void*)sprite_gfx[29], OBJ_SIZE_16X16, COLOR256, 2, x+16, y);
		PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+2, 1, 1, 20, ANIM_LOOP, -1);
		PA_CreateSprite(SUB_SCREEN, 50 + (n+1)*4 + 3, (void*)sprite_gfx[29], OBJ_SIZE_16X16, COLOR256, 2, x+24, y);		
		PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+3, 1, 1, 20, ANIM_LOOP, -1);	
	}
} // initializes the minimap display on the sub screen
void deleteMinimap() {
	for(int n = 0; n < 128; n++) PA_DeleteSprite(SUB_SCREEN, n);
	
}
void displayMinimap() {
	for(int n = 0; n < (int)players.size(); n++) {
		if(!(players[n] -> isdead)) PA_SetSpriteXY(SUB_SCREEN, n+1, (int)((players[n] -> x)/4) + 64, (int)((players[n] -> y)/4) + 32 + 56 + 4);
	}
	// displays the sprites in the correct position on a scaled down map
	PA_SetSpriteXY(SUB_SCREEN, 0, (int)(scrollx/4) + 64, (int)(scrolly/4) + 24 + 56 + 4);
	// displays a box representing the current screen
} // displays the minimap
void displayPercentages() {
	for(int n = 0; n < (int)players.size(); n++) {
		int damage = (int)(players[n]->getDamagePercent());
		if(damage > 300) {
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4, DAMAGE_HIGHER-1, DAMAGE_HIGHER-1, 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+1, DAMAGE_HIGHER+(int)(damage/100), DAMAGE_HIGHER+(int)(damage%100), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+2, DAMAGE_HIGHER+(int)((damage%100)/10), DAMAGE_HIGHER+(int)((damage/10)%10), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+3, DAMAGE_HIGHER+(int)(damage%10), DAMAGE_HIGHER+(int)(damage%10), 20, ANIM_LOOP, -1);
		}
		else if(damage > 200) {
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4, DAMAGE_HIGH-1, DAMAGE_HIGH-1, 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+1, DAMAGE_HIGH+(int)(damage/100), DAMAGE_HIGH+(int)(damage%100), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+2, DAMAGE_HIGH+(int)((damage%100)/10), DAMAGE_HIGH+(int)((damage/10)%10), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+3, DAMAGE_HIGH+(int)(damage%10), DAMAGE_HIGH+(int)(damage%10), 20, ANIM_LOOP, -1);
		}
		else if(damage > 100) {
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4, DAMAGE_MID-1, DAMAGE_MID-1, 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+1, DAMAGE_MID+(int)(damage/100), DAMAGE_MID+(int)(damage%100), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+2, DAMAGE_MID+(int)((damage%100)/10), DAMAGE_MID+(int)((damage/10)%10), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+3, DAMAGE_MID+(int)(damage%10), DAMAGE_MID+(int)(damage%10), 20, ANIM_LOOP, -1);				
		}
		else {
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4, DAMAGE_LOW-1, DAMAGE_LOW-1, 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+1, DAMAGE_LOW+(int)(damage/100), DAMAGE_LOW+(int)(damage%100), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+2, DAMAGE_LOW+(int)((damage%100)/10), DAMAGE_LOW+(int)((damage/10)%10), 20, ANIM_LOOP, -1);
			PA_StartSpriteAnimEx(SUB_SCREEN, 50+(n+1)*4+3, DAMAGE_LOW+(int)(damage%10), DAMAGE_LOW+(int)(damage%10), 20, ANIM_LOOP, -1);				
		}
	}
} // displays damage percents on the sub screen
void displayLives(int stock) {
	PA_OutputText(SUB_SCREEN, 0, 4, "                                                ");	
	for(int n = 0; n < (int)players.size(); n++) {
		if(players[n] -> SPRITENUM-100 == 1) {
			PA_SetTextTileCol(SUB_SCREEN, TEXT_RED);
			int lives = stock - score.getDeaths(n) - sdcost*score.getSDs(n);
			PA_OutputText(SUB_SCREEN, 0, 5, "x%d", lives);
		}
		else if(players[n] -> SPRITENUM-100 == 2) {
			PA_SetTextTileCol(SUB_SCREEN, TEXT_BLUE);
			int lives = stock-score.getDeaths(n) - sdcost*score.getSDs(n);
			PA_OutputText(SUB_SCREEN, 8, 5, "x%d", lives);
		}
		else if(players[n] -> SPRITENUM-100 == 3) {
			PA_SetTextTileCol(SUB_SCREEN, TEXT_YELLOW);
			int lives = stock-score.getDeaths(n) - sdcost*score.getSDs(n);
			PA_OutputText(SUB_SCREEN, 16, 5, "x%d", lives);
		}
		else if(players[n] -> SPRITENUM-100 == 4) {
			PA_SetTextTileCol(SUB_SCREEN, TEXT_GREEN);
			int lives = stock-score.getDeaths(n) - sdcost*score.getSDs(n);
			PA_OutputText(SUB_SCREEN, 24, 5, "x%d", lives);
		}
	}
}

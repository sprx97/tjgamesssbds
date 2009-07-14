#include "projectiles.h"
#include "global.h"
#include "display.h"

// shortcuts for projectiles
Projectile::Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage* mine, Display *d) {
	display=d;
	mystage = mine;
	num = 50+ob;
	owner = ob;
	TYPE = t;
	x = xpos;
	y = ypos;
	dx = xchange;
	enabled = true;
	if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
	else PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
	dy = ychange;
	length = l;
	time = 0;
	hit = h;
	PA_SetSpriteXY(MAIN_SCREEN, num, (int)(x-display->scrollx), (int)(y-display->scrolly));
	if(TYPE == SHADOWBALL_SMALL) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 0, 3, 20, ANIM_LOOP, -1);
	}
	if(TYPE == SHADOWBALL_MEDIUM) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 4, 7, 20, ANIM_LOOP, -1);
	}
	if(TYPE == SHADOWBALL_LARGE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 8, 11, 20, ANIM_LOOP, -1);
	}
	if(TYPE == FINALCUTTER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 12, 12, 20, ANIM_LOOP, -1);
	}
	if(TYPE == FIREBALL) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 13, 16, 20, ANIM_LOOP, -1);
	}
	if(TYPE == FLUDDWATER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 17, 17, 20, ANIM_LOOP, -1);
	}
	if(TYPE == IKESWORD) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 18, 24, 20, ANIM_LOOP, -1);
	}
	if(TYPE == FOXLASER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 25, 25, 20, ANIM_LOOP, -1);
	}
}
bool Projectile::act() {
	x += dx;
	y += dy;
	if(y > maxy) dy*=-1;
	if(y < miny) dy*=-1;
	PA_SetSpriteXY(MAIN_SCREEN, num, (int)(x-display->scrollx), (int)(y-display->scrolly));
	if(x+64-display->scrollx < 0 || x-display->scrollx > 256 || y+64-display->scrolly < 0 || y-display->scrolly > 192) PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
	time++;
	if(time > length) return true;
	return false;
}
Fighter* Projectile::checkHits(Fighter* other) {
	Hitbox temp = hit; 
	Hitbox atkbox;
	vector<Circle> circles = temp.getCircles();
	for(uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircright(current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		Circle newcircleft(64 - current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		if(dx < 0) atkbox.addCircle(newcircright);
		else atkbox.addCircle(newcircleft);
	}
	if(!enabled) return other;
	if(other -> respawntimer > 0) return other;
	if(atkbox.hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
		if(other -> CAPE || other -> COUNTER) {
			dx *= -1;
			owner = other->charnum;
			vector<Circle> temp = hit.getCircles();
			hit.reset();
			for(int n = 0; n < (int)temp.size(); n++) {
				hit.addCircle(Circle(temp[n].getX(), temp[n].getY(), temp[n].getRadius(), Knockback(temp[n].getKnockback().dx * -1, temp[n].getKnockback().dy, temp[n].getKnockback().length), temp[n].damage));
			}
			if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
			if(dx < 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
		}
		else if(other -> action == AIRDODGE || other -> action == ROLL || other -> action == DODGE) { /*doesn't hit*/ }
		else if(other -> action == SHIELD) {
			other -> shieldstr -= atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage * (.5*atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage);
			enabled = false;
		}
		else {
			other -> takeDamage(atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, owner, 0);
			if(TYPE != FINALCUTTER && TYPE != IKESWORD) removeProj(num);
			else enabled = false;
		}
	}
	return other;
}

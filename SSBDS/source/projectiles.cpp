#include "projectiles.h"
#include "global.h"
#include "display.h"

#include <math.h>

// shortcuts for projectiles
Projectile::Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage* mine, Display *d) {
	display = d;
	mystage = mine;
	num = -1;
	for(int n = 0; n < effproj_used_size; n++) {
		if(!effproj_used[n]) {
			num = n+5;
			effproj_used[n] = true;
			break;
		}
	}
	owner = ob;
	TYPE = t;
	x = xpos;
	y = ypos;
	dx = xchange;
	enabled = true;
	if (dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
	else PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
	dy = ychange;
	length = l;
	time = 0;
	hit = h;
	PA_SetSpriteXY(MAIN_SCREEN, num, (int)(x - display->scrollx), (int)(y - display->scrolly));
	if (TYPE == SHADOWBALL_SMALL) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 18, 21, 20, ANIM_LOOP, -1);
	}
	if (TYPE == SHADOWBALL_MEDIUM) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 22, 25, 20, ANIM_LOOP, -1);
	}
	if (TYPE == SHADOWBALL_LARGE) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 26, 29, 20, ANIM_LOOP, -1);
	}
	if (TYPE == FINALCUTTER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 30, 30, 20, ANIM_LOOP, -1);
	}
	if (TYPE == FIREBALL) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 31, 34, 20, ANIM_LOOP, -1);
	}
	if (TYPE == FLUDDWATER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 35, 35, 20, ANIM_LOOP, -1);
	}
	if (TYPE == IKESWORD) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 36, 42, 20, ANIM_LOOP, -1);
	}
	if (TYPE == FOXLASER) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 43, 43, 20, ANIM_LOOP, -1);
	}
	if(TYPE == THUNDERSHOCK) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 44, 44, 20, ANIM_LOOP, -1);
	}
	if(TYPE == THUNDER1) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 48, 51, 15, ANIM_LOOP, -1);
	}
	if(TYPE == THUNDER2) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 52, 53, 15, ANIM_LOOP, -1);
	}
	if(TYPE == THUNDER3) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 54, 55, 15, ANIM_LOOP, -1);
	}
	if(TYPE == THUNDER4) {
		PA_StartSpriteAnimEx(MAIN_SCREEN, num, 56, 58, 15, ANIM_LOOP, -1);
	}
}
bool Projectile::act() {
	if(TYPE == THUNDERSHOCK && PA_GetSpriteAnimFrame(MAIN_SCREEN, num) == 44) {
		Floor f = checkFloorCollision(32, 64);
		if(f.length != 0) {
			dy = 0;
			y = f.y-64;
			PA_StartSpriteAnimEx(MAIN_SCREEN, num, 45, 47, 20, ANIM_LOOP, -1);
		}
		Wall w = checkWallCollision(32, 64);
		if(w.length != 0) dx *= -1;
	}
	else if(TYPE == THUNDERSHOCK && PA_GetSpriteAnimFrame(MAIN_SCREEN, num) >= 45 && PA_GetSpriteAnimFrame(MAIN_SCREEN, num) <= 47) {
		if(checkWallCollision(32, 64).length != 0) dx *= -1;
		if(dy == 0) {
			dy = 1;
			if(checkFloorCollision(32, 64).length == 0) {
				dy = GLOBALGRAVITY;
				PA_StartSpriteAnimEx(MAIN_SCREEN, num, 44, 44, 20, ANIM_LOOP, -1);
			}
			else dy = 0;
		}
	}
	if(TYPE == FIREBALL) {
		dy += .1;
		if(dy > GLOBALGRAVITY) dy = GLOBALGRAVITY;
		Floor f = checkFloorCollision(32, 32);
		if(f.length != 0) dy *= -1;
		Wall w = checkWallCollision(32, 32);
		if(w.length != 0) dx *= -1;
		Ceiling c = checkCeilingCollision(32, 32);
		if(c.length != 0) dy *= -1;
	}
	x += dx;
	y += dy;
	PA_SetSpriteXY(MAIN_SCREEN, num, (int)(x - display->scrollx), (int)(y - display->scrolly));
	if (x + 64 - display->scrollx < 0 || x - display->scrollx > 256 || y + 64 - display->scrolly < 0 || y - display->scrolly > 192) PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
	time++;
	if (time > length) return true;
	return false;
}
Floor Projectile::checkFloorCollision(int deltax, int deltay) {
	for(int n = 0; n < (int)((mystage -> getFloors()).size()); n++) {
		if(y+deltay <= mystage -> getFloors()[n].y && y+deltay+dy > mystage -> getFloors()[n].y && x+deltax+dx > mystage -> getFloors()[n].x && x+deltax+dx < mystage -> getFloors()[n].x + mystage -> getFloors()[n].length) {
			return mystage -> getFloors()[n];
		}
	}
	return Floor(0, 0, 0, false);
}
Wall Projectile::checkWallCollision(int deltax, int deltay) {
	for(int n = 0; n < (int)((mystage -> getWalls()).size()); n++) {
		if(dx > 0) {
			if(x+deltax <= mystage -> getWalls()[n].x && x+deltax+dx > mystage -> getWalls()[n].x && y+deltay+dy > mystage -> getWalls()[n].y && y+deltax+dy < mystage -> getWalls()[n].y + mystage -> getWalls()[n].length) {
				return mystage -> getWalls()[n];
			}
		}
		else if(dx < 0) {
			if(x+deltax >= mystage -> getWalls()[n].x && x+deltax+dx < mystage -> getWalls()[n].x && y+deltay+dy > mystage -> getWalls()[n].y && y+deltax+dy < mystage -> getWalls()[n].y + mystage -> getWalls()[n].length) {
				return mystage -> getWalls()[n];
			}		
		}
	}
	return Wall(0, 0, 0, "");
}
Ceiling Projectile::checkCeilingCollision(int deltax, int deltay) {
	for(int n = 0; n < (int)((mystage -> getCeilings()).size()); n++) {
		if(y+deltay >= mystage -> getCeilings()[n].y && y+deltay+dy < mystage -> getCeilings()[n].y && x+deltax+dx > mystage -> getCeilings()[n].x && x+deltax+dx < mystage -> getCeilings()[n].x + mystage -> getCeilings()[n].length) {
			return mystage -> getCeilings()[n];
		}
	}
	return Ceiling(0, 0, 0);
}
Fighter* Projectile::checkHits(Fighter* other) {
	Hitbox temp = hit;
	Hitbox atkbox;
	vector<Circle> circles = temp.getCircles();
	for (uint8 n = 0; n < circles.size(); n++) {
		Circle current = circles[n];
		Circle newcircright(current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		Circle newcircleft(64 - current.getX() + x, current.getY() + y, current.getRadius(), current.getKnockback(), current.damage);
		if (dx < 0) atkbox.addCircle(newcircright);
		else atkbox.addCircle(newcircleft);
	}
	if(TYPE == THUNDER1 || TYPE == THUNDER2 || TYPE == THUNDER3 || TYPE == THUNDER4) {
		vector<Projectile> projs = *((vector<Projectile>*)getProj());
		for(int n = 0; n < (int)projs.size(); n++) {
			if(projs[n].owner == owner && (projs[n].TYPE == THUNDER1 || projs[n].TYPE == THUNDER2 || projs[n].TYPE == THUNDER3 || projs[n].TYPE == THUNDER4)) {
				if(!projs[n].enabled) enabled = false;
			}
		}
	}
	if (!enabled) {
		if(TYPE == THUNDER1 || TYPE == THUNDER2 || TYPE == THUNDER3 || TYPE == THUNDER4) {
			vector<Projectile> projs = *((vector<Projectile>*)getProj());
			for(int n = 0; n < (int)projs.size(); n++) {
				if(projs[n].owner == owner && (projs[n].TYPE == THUNDER1 || projs[n].TYPE == THUNDER2 || projs[n].TYPE == THUNDER3 || projs[n].TYPE == THUNDER4)) {
					projs[n].enabled = false;
				}
			}
		}
		return other;
	}
	if (other -> respawntimer > 0) return other;
	if (atkbox.hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
		if (other -> CAPE || other -> COUNTER) {
			dx *= -1;
			owner = other->charnum;
			vector<Circle> temp = hit.getCircles();
			hit.reset();
			for (int n = 0; n < (int)temp.size(); n++) {
				hit.addCircle(Circle(temp[n].getX(), temp[n].getY(), temp[n].getRadius(), Knockback(temp[n].getKnockback().dx * -1, temp[n].getKnockback().dy, temp[n].getKnockback().length), temp[n].damage));
			}
			if (dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
			if (dx < 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
		}
		else if (other -> ABSORB && (TYPE != IKESWORD)) {
			other -> percentage -= atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage;
			if(other -> percentage < 0) other -> percentage = 0;
			if(TYPE != THUNDER1 && TYPE != THUNDER2 && TYPE != THUNDER3 && TYPE != THUNDER4) removeProj(num);
		}
		else if (other -> action == AIRDODGE || other -> action == ROLL || other -> action == DODGE) { /*doesn't hit*/
		}
		else if (other -> action == SHIELD) {
			other -> shieldstr -= atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage * (.5 * atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))).damage);
			if (TYPE != FINALCUTTER && TYPE != IKESWORD && TYPE != THUNDER1 && TYPE != THUNDER2 && TYPE != THUNDER3 && TYPE != THUNDER4) removeProj(num);
			else enabled = false;
			if(TYPE == THUNDER1 || TYPE == THUNDER2 || TYPE == THUNDER3 || TYPE == THUNDER4) {
				vector<Projectile> projs = *((vector<Projectile>*)getProj());
				for(int n = 0; n < (int)projs.size(); n++) {
					if(projs[n].owner == owner && (projs[n].TYPE == THUNDER1 || projs[n].TYPE == THUNDER2 || projs[n].TYPE == THUNDER3 || projs[n].TYPE == THUNDER4)) projs[n].enabled = false;
				}
			}
		}
		else {
			other -> takeDamage(atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, owner, 0);
			if (TYPE != FINALCUTTER && TYPE != IKESWORD && TYPE != THUNDER1 && TYPE != THUNDER2 && TYPE != THUNDER3 && TYPE != THUNDER4) removeProj(num);
			else enabled = false;
			if(TYPE == THUNDER1 || TYPE == THUNDER2 || TYPE == THUNDER3 || TYPE == THUNDER4) {
				vector<Projectile> projs = *((vector<Projectile>*)getProj());
				for(int n = 0; n < (int)projs.size(); n++) {
					if(projs[n].owner == owner && (projs[n].TYPE == THUNDER1 || projs[n].TYPE == THUNDER2 || projs[n].TYPE == THUNDER3 || projs[n].TYPE == THUNDER4)) projs[n].enabled = false;
				}
			}
		}
	}
	return other;
}

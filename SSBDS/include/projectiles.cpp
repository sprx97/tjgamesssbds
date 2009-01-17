#ifdef PROJECTILES_ON
static const int SHADOWBALL_SMALL = 0, SHADOWBALL_MEDIUM = 1, SHADOWBALL_LARGE = 2, FINALCUTTER = 3, FIREBALL = 4, FLUDDWATER = 5, IKESWORD = 6;
// shortcuts for projectiles
class Projectile {
	public:
		double x, y, dx, dy;
		double maxy, miny; // for bouncing
		Hitbox hit;
		int length;
		int time;
		int TYPE;
		int num;
		int owner;
		Stage mystage;
		Projectile(double xpos, double ypos, double xchange, double ychange, int l, int t, int ob, Hitbox h, Stage mine) {
			mystage = mine;
			num = 50+ob;
			owner = ob;
			TYPE = t;
			x = xpos;
			y = ypos;
			dx = xchange;
			if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
			else PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
			dy = ychange;
			length = l;
			time = 0;
			hit = h;
			PA_SetSpriteXY(MAIN_SCREEN, num, x-scrollx, y-scrolly);
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
		}
		void act() {
			x += dx;
			y += dy;
			if(y > maxy) dy*=-1;
			if(y < miny) dy*=-1;
			PA_SetSpriteXY(MAIN_SCREEN, num, x-scrollx, y-scrolly);
			if(x+64-scrollx < 0 || x-scrollx > 256 || y+64-scrolly < 0 || y-scrolly > 192) PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
			time++;
			if(time > length) removeSelf();
		}
		void removeSelf() {
			vector<Projectile> temp;
			for(int n = 0; n < projectiles.size(); n++) {
				Projectile p = projectiles[n];
				if(p.x != x || p.y != y) {
					temp.push_back(p);
				}
			}
			projectiles = temp;
			PA_SetSpriteXY(MAIN_SCREEN, num, -64, -64);
		}
		Fighter* checkHits(Fighter* other) {
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
			if(atkbox.hits(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM)))) {
				if(other -> CAPE || other -> COUNTER) {
					dx *= -1;
					owner = other->charnum;
					vector<Circle> temp = hit.getCircles();
					hit.reset();
					for(int n = 0; n < temp.size(); n++) {
						hit.addCircle(Circle(temp[n].getX(), temp[n].getY(), temp[n].getRadius(), Knockback(temp[n].getKnockback().dx * -1, temp[n].getKnockback().dy, temp[n].getKnockback().length), temp[n].damage));
					}
					if(dx > 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 0);
					if(dx < 0) PA_SetSpriteHflip(MAIN_SCREEN, num, 1);
				}
				else {
					other -> takeDamage(atkbox.getHitCircle(other -> getDefbox(PA_GetSpriteAnimFrame(MAIN_SCREEN, other -> SPRITENUM))), 1, owner, 0);
					if(TYPE != FINALCUTTER && TYPE != IKESWORD) removeSelf();
				}
			}
			return other;
		}
};
#endif

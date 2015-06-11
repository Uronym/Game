
#include<allegro5/allegro.h>
#include<vector>
#include"game.h"
#include"mon.h"

// useful things that probably belong in another file
double limit(double n, double mn, double mx) {return n < mn ? mn : n > mx ? mx : n;}
double lerp(double a, double b, double c) {return a * (1 - c) + b * c;}

// data about all monster types
mon_dat mon_data[MON_IDS] = {
	mon_dat("slime", 10, 10, 4),
};

std::vector<Mon> mons;

void Mon::die() {
	for(unsigned i = 0; i < mons.size(); ++i)
		if(this == &mons[i]) {mons.erase(mons.begin() + i); break;}
}

void Mon::dmg(int dp) {
	hp = limit(hp - dp, 0, hp_max);
	if(hp == 0) die();
}

void Mon::wield(int i) {
	if(item != NULL) inv.push_back(*item);
	if(i < 0 || i >= inv.size()) {
		item = NULL;
	} else {
		item = &inv[i];
		inv.erase(inv.begin() + i);
	}
}

// mon attempts to take a step in dir
// returns success
bool Mon::step(MOVE_DIR dir) {
	// step must follow speed
	double now = al_get_time();
	if(now - ostep < 1.0 / spe) return false;
	// step must not collide
	vec2 n(p); // new position
	switch(dir) {
		case MOVE_UP:
			--n.y; break;
		case MOVE_DOWN:
			++n.y; break;
		case MOVE_LEFT:
			--n.x; break;
		case MOVE_RIGHT:
			++n.x; break;
		default:
			return false;
	}
	if(!n.onsq(mapSize)) return false;
	// step must not collide with map
	if(col[map[n.x + n.y * mapSize]]) return false;
	// if step collides with monster, attack
	for(unsigned i = 0; i < mons.size(); ++i) {
		if(mons[i].p == n) {
			mons[i].dmg(item == NULL ? 1 : item->dat->dmg);
			o = p;
			ostep = now;
			return true;
		}
	}
	// passed all checks!
	o = p;
	ostep = now;
	p = n;
	return true;
}

// get rendering position
void Mon::rpos(double& rx, double& ry) {
	double c = limit((al_get_time() - ostep) * spe, 0, 1);
	rx = lerp(o.x, p.x, c);
	ry = lerp(o.y, p.y, c);
}


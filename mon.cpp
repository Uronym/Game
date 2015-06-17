
#include<allegro5/allegro.h>
#include<utility>
#include"game.h"
#include"mon.h"

// useful things that probably belong in another file
double limit(double n, double mn, double mx) {return n < mn ? mn : n > mx ? mx : n;}
double lerp(double a, double b, double c) {return a * (1 - c) + b * c;}

mon_dat mon_data[MON_IDS] = {
	//       name  tile hp  mp  spe   solid
	mon_dat("bat",   6,  3, 10, 2.00, true),
	mon_dat("ghost", 3,  5, 10, 0.75, false),
	mon_dat("human", 4, 10, 10, 1.00, true),
	mon_dat("slime", 0,  3, 10, 0.25, true),
};

std::vector<Mon> mons;

Mon* get_plyr() {
	for(unsigned i = 0; i < mons.size(); ++i)
		if(mons[i].ai == AI_PLYR)
			return &mons[i];
	assert(false); // maybe not the most friendly way
}

void Mon::die() {
	if(id == MON_BAT)
		Item(ITEM_BAT, p);
	for(unsigned i = 0; i < mons.size(); ++i)
		if(this == &mons[i]) {mons.erase(mons.begin() + i); break;}
}

void Mon::dmg(int dp) {
	hp = limit(hp - dp, 0, hp_max);
	if(hp == 0) die();
}

// use wielded item
void Mon::use(Item& it) {
	std::swap(it, inv.back());
	inv.pop_back();
	if(item == &it) item = NULL;
	switch(it.id) {
	case ITEM_BAT:
		msg = "You hit your head with the bat.";
		dmg(it.dat->atk); break;
	case ITEM_POTION:
		msg = "You quaff the healing potion.";
		dmg(-10); break;
	default:
		assert(false); break;
	}
}

// mon attempts to take a step in dir
// returns success
bool Mon::step(MOVE_DIR dir) {
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
		assert(false); break;
	}
	if(!n.onsq(mapSize)) return false; // must be on map
	// must not collide with map if walls affect monster
	if(dat->solid && col[map[n.x + n.y * mapSize]]) return false;
	// if step collides with monster, attack
	for(unsigned i = 0; i < mons.size(); ++i) {
		if(mons[i].p == n) {
			mons[i].dmg(item == NULL ? 1 : item->dat->atk);
			o = p;
			return true;
		}
	}
	// passed all checks!
	o = p;
	p = n;
	last_step = al_get_time();
	if(this == get_plyr() && map[p.x + p.y * mapSize] == 2) {
		if(currentMap == "main") load_maze();
		else loadMap("main");
	}
	return true;
}

// get rendering position
void Mon::rpos(double& rx, double& ry) {
	double c = limit((al_get_time() - last_step) / TURN_LENGTH, 0, 1);
	rx = lerp(o.x, p.x, c);
	ry = lerp(o.y, p.y, c);
}


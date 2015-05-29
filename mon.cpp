
#include<allegro5/allegro.h>
#include"Game.h"
#include"mon.h"

// useful things that probably belong in another file
double limit(double n, double mn, double mx) {return n < mn ? mn : n > mx ? mx : n;}
double lerp(double a, double b, double c) {return a * (1 - c) + b * c;}

// data about all monster types
mon_dat mon_data[MON_IDS] = {
	mon_dat("slime", 10, 10, 4),
};

// mon attempts to take a step in dir
void Mon::step(MOVE_DIR dir) {
	// step must follow speed
	double now = al_get_time();
	if(now - ostep < 1 / spe) return;
	// step must not collide
	int nx = x; int ny = y; // new position
	switch(dir) {
		case MOVE_UP:
			--ny; break;
		case MOVE_DOWN:
			++ny; break;
		case MOVE_LEFT:
			--nx; break;
		case MOVE_RIGHT:
			++nx; break;
		case MOVE_DIRS: // should probably error
			return;
	}
	if(nx < 0 || nx > mapSize || ny < 0 || ny > mapSize) return;
	if(map[nx + ny * mapSize] == 1) return;
	// passed all checks!
	ox = x; oy = y;
	ostep = now;
	x = nx; y = ny;
}

// get rendering position
void Mon::rpos(double& rx, double& ry) {
	double c = limit((al_get_time() - ostep) * spe, 0, 1);
	rx = lerp(ox, x, c);
	ry = lerp(oy, y, c);
}


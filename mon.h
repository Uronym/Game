#ifndef MON_H
#define MON_H

#include<string>
#include<vector>
#include"item.h"

enum MON_ID { // indices for mon_data
	MON_SLIME,
	MON_IDS, // number of MON_IDs
};

enum MOVE_DIR { // movement directions
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DIRS, // number of MOVE_DIRs
};

struct mon_dat { // data structure of mon_data
	std::string name;
	int hp_base;
	int mp_base;
	double spe_base;
	mon_dat(std::string name, int hp_base, int mp_base, double spe_base):
		name(name), hp_base(hp_base), mp_base(mp_base), spe_base(spe_base) {}
};

extern mon_dat mon_data[MON_IDS]; // data about all monster types

struct Mon { // an individual monster
	MON_ID id; // index in mon_data
	mon_dat* dat; // mon data
	int x; int y; // position
	int ox; int oy; // old step position
	double ostep; // old step time
	int hp_max; int mp_max; int hp; int mp; double spe;
	Item* item; // currently wielded item
	std::vector<Item> inv; // inventory
	void dmg(int dp); // cause hp damage
	void wield(int i); // wield item at i in inv
	void step(MOVE_DIR dir); // mon attempts to take a step in dir
	void rpos(double& rx, double& ry); // get rendering position
	// construct with given id and position
	Mon(MON_ID id, int x, int y):
		id(id), dat(&mon_data[id]), x(x), y(y), ox(x), oy(y), ostep(0),
		hp_max(mon_data[id].hp_base), mp_max(mon_data[id].mp_base),
		hp(hp_max), mp(mp_max), spe(mon_data[id].spe_base), item(NULL) {}
};

extern std::vector<Mon> mons;

#endif

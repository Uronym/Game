#ifndef MON_H
#define MON_H

#include<string>
#include<vector>
#include"item.h"
#include"vec2.h"

struct mon_dat;
struct Mon;

enum MON_ID { // indices for mon_data
	MON_HUMAN,
	MON_SLIME,
	MON_IDS, // number of MON_IDs
};

extern mon_dat mon_data[MON_IDS]; // data about all monster types
extern std::vector<Mon> mons; // vector of all monsters

struct mon_dat { // data structure of mon_data
	std::string name; int tile;
	int hp_base, mp_base; double spe_base;
	mon_dat(std::string name, int tile,
			int hp_base, int mp_base, double spe_base):
		name(name), tile(tile),
		hp_base(hp_base), mp_base(mp_base), spe_base(spe_base) {}
};

struct Mon { // an individual monster
	MON_ID id; // index in mon_data
	mon_dat* dat; // mon data
	vec2 p; // position
	vec2 o; // old step position
	double ostep; // old step time
	int hp_max, mp_max, hp, mp; double spe;
	Item* item; // currently wielded item
	std::vector<Item> inv; // inventory
	void die(); // die (kill?)
	void dmg(int dp); // cause hp damage
	bool step(MOVE_DIR dir); // mon attempts to take a step in dir
	void rpos(double& rx, double& ry); // get rendering position
	// construct with given id and position
	Mon(MON_ID id, vec2 p):
		id(id), dat(&mon_data[id]), p(p), o(p), ostep(0),
		hp_max(mon_data[id].hp_base), mp_max(mon_data[id].mp_base),
		hp(hp_max), mp(mp_max), spe(mon_data[id].spe_base), item(NULL) {
		mons.push_back(*this);}
};

#endif

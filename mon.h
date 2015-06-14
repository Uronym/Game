#ifndef MON_H
#define MON_H

#include<string>
#include<vector>
#include"item.h"
#include"vec2.h"

struct mon_dat;
struct Mon;

enum MON_ID { // indices for mon_data
	MON_GHOST,
	MON_HUMAN,
	MON_SLIME,
	MON_IDS, // number of MON_IDs
};

enum AI_TYPE { // types of ai
	AI_MON,
	AI_PLYR,
	AI_TYPES, // number of AI_TYPEs
};

extern mon_dat mon_data[MON_IDS]; // data about all monster types
extern std::vector<Mon> mons; // vector of all monsters

Mon* get_plyr(); // get pointer to player in mons

struct mon_dat { // data structure of mon_data
	std::string name; int tile;
	int hp_base, mp_base; double spe_base;
	bool solid; // stopped by walls
	mon_dat(std::string name, int tile,
	        int hp_base, int mp_base, double spe_base,
	        bool solid):
		name(name), tile(tile),
		hp_base(hp_base), mp_base(mp_base), spe_base(spe_base),
		solid(solid) {}
};

struct Mon { // an individual monster
	double last_step; // last step time
	MON_ID id; // index in mon_data
	AI_TYPE ai; // type of ai
	mon_dat* dat; // mon data
	vec2 p; // position
	vec2 o; // old step position
	double move_pts; // movement points
	int hp_max, mp_max, hp, mp; double spe;
	Item* item; // currently wielded item
	std::vector<Item> inv; // inventory
	void use(Item& item); // use item
	void die(); // die (kill?)
	void dmg(int dp); // cause hp damage
	bool step(MOVE_DIR dir); // mon attempts to take a step in dir
	void rpos(double& rx, double& ry); // get rendering position
	// construct with given id and position
	Mon(MON_ID id, AI_TYPE ai, vec2 p): last_step(0),
		id(id), ai(ai), dat(&mon_data[id]), p(p), o(p), move_pts(0),
		hp_max(mon_data[id].hp_base), mp_max(mon_data[id].mp_base),
		hp(hp_max), mp(mp_max), spe(mon_data[id].spe_base), item(NULL) {
		mons.push_back(*this);}
};

#endif

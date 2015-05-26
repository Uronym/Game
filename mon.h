#ifndef MON_H
#define MON_H

#include<string>

enum MON_ID { // indices for mon_data
	MON_SLIME,
	MON_IDS, // number of MON_IDs
};

struct mon_dat { // data structure of mon_data
	std::string name;
	int hp_base;
	int mp_base;
	mon_dat(std::string name, int hp_base, int mp_base):
		name(name), hp_base(hp_base), mp_base(mp_base) {}
};

mon_dat mon_data[] = { // data about all monster types
	mon_dat("slime", 10, 10),
};

struct Mon { // an individual monster
	MON_ID id; // index in mon_data
	int hp_max; int mp_max; int hp; int mp;
	int x; int y; // position
	Mon(MON_ID id, int x, int y):
		hp_max(mon_data[id].hp_base), mp_max(mon_data[id].mp_base),
		hp(hp_max), mp(mp_max), x(x), y(y) {}
};

#endif

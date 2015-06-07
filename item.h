#ifndef ITEM_H
#define ITEM_H

#include<string>
#include<vector>
#include"vec2.h"

enum ITEM_ID { // indices for item_data
	ITEM_POTION,
	ITEM_IDS, // number of ITEM_IDs
};

struct item_dat { // data structure of item_data
	std::string name;
	int dmg;
	item_dat(std::string name, int dmg): name(name), dmg(dmg) {}
};

extern item_dat item_data[ITEM_IDS]; // data about all item types

struct Item { // an individual item
	ITEM_ID id; // index in item_data
	item_dat* dat; // item data
	vec2 p; // position
	// construct with given id and position
	Item(ITEM_ID id, vec2 p): id(id), dat(&item_data[id]), p(p) {}
};

extern std::vector<Item> items;

#endif

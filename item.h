#ifndef ITEM_H
#define ITEM_H

#include<string>
#include<vector>

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
	int x; int y; // position
	// construct with given id and position
	Item(ITEM_ID id, int x, int y): id(id), dat(&item_data[id]), x(x), y(y) {}
};

extern std::vector<Item> items;

#endif

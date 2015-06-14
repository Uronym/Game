
#include<vector>
#include"item.h"

item_dat item_data[ITEM_IDS] = {
	//        name    tile atk
	item_dat("bat",     5,  3),
	item_dat("potion",  1,  2),
};

std::vector<Item> items;


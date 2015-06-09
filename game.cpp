
#include<allegro5/allegro.h>
#include<cmath>
#include<fstream>
#include<string>
#include"game.h"

ALLEGRO_BITMAP** sprites;
ALLEGRO_BITMAP** tiles;
short* map = nullptr;
int mapSize = 0;
std::string currentMap;
bool** cmap = NULL;

void update_cmap() {
	delete[] cmap;
	cmap = new bool*[mapSize];
	for(int x = 0; x < mapSize; ++x) cmap[x] = new bool[mapSize];
	for(int x = 0; x < mapSize; ++x) {
		for(int y = 0; y < mapSize; ++y) {
			short tile = map[x + y * mapSize];
			cmap[x][y] = tile == 1 || tile == 4 || tile == 5;
		}
	}
}

void loadMap(std::string name) {
	std::ifstream in;
	in.open("Maps/" + name + ".map", std::ios::in | std::ios::binary);
	if(in.is_open()) {
		in.seekg(0, in.end);
		int length = in.tellg();
		in.seekg(0, in.beg);
		mapSize = sqrt((double)length/2.0);
		if(map!=nullptr) delete [] map;
		map = new short[mapSize*mapSize];
		in.read((char *)map,length);
		currentMap = name;
		in.close();
	}
	update_cmap();
}


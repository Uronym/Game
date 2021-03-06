
#include<algorithm>
#include<allegro5/allegro.h>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<string>
#include<vector>
#include"game.h"
#include"item.h"
#include"mon.h"
#include"vec2.h"

ALLEGRO_BITMAP** sprites;
ALLEGRO_BITMAP** tiles;
short* map = nullptr;
int mapSize = 0;
std::string currentMap;
double last_turn = 0;
std::string msg = "";
vec2 curs(0, 0);
bool curs_mode = false;

bool col[NUM_TILES] = {false, true, false, false, true, true};

struct maze_node { // a node in the maze generation algorithm
	bool closed; // closed
	bool right; bool down; // right/down open
	vec2 parent; // parent position
	maze_node(): closed(), right(), down() {}
};

void load_maze() {
	int msize = 5; // size of map
	int width = 2; // width of halls
	maze_node** nodes; nodes = new maze_node*[msize]; // allocate nodes
	for(int x = 0; x < msize; ++x) nodes[x] = new maze_node[msize];
	vec2 n(rand() % msize, rand() % msize); // start at random position
	nodes[n.x][n.y].parent = n;
	while(true) {
		nodes[n.x][n.y].closed = true;
		std::vector<MOVE_DIR> dirs; // create vector of open positions
		for(int i = 0; i < MOVE_DIRS; ++i) {
			vec2 d(n + MOVE_VEC[i]);
			if(d.onsq(msize) && !nodes[d.x][d.y].closed)
				dirs.push_back((MOVE_DIR)i);
		}
		if(dirs.size() > 0) { // select random open direction
			MOVE_DIR dir = dirs[rand() % dirs.size()];
			switch(dir) {
				case MOVE_UP:
					nodes[n.x][n.y - 1].down = true; break;
				case MOVE_DOWN:
					nodes[n.x][n.y].down = true; break;
				case MOVE_LEFT:
					nodes[n.x - 1][n.y].right = true; break;
				case MOVE_RIGHT:
					nodes[n.x][n.y].right = true; break;
				default: // it's best that the program dies if this happens
					assert(false);
			}
			vec2 d(n + MOVE_VEC[dir]);
			nodes[d.x][d.y].parent = n;
			n = d;
		} else {
			vec2 p = nodes[n.x][n.y].parent;
			if(n != p) n = p; // return to parent position
			else break; // stuck at original position == finished
		}
	}
	// actually put the map
	if(map != NULL) delete[] map;
	mapSize = 16; currentMap = "maze";
	map = new short[mapSize * mapSize]();
	for(int i = 0; i < mapSize * mapSize; ++i) map[i] = 1; // fill map
	for(int x = 0; x < msize; ++x) // carve corridors
		for(int y = 0; y < msize; ++y) {
			int mx = x * (1 + width) + 1; int my = y * (1 + width) + 1; // map coordinates
			for(int cx = 0; cx < width; ++cx) {
				map[mx + width + (my + cx) * mapSize] = nodes[x][y].right ? 0 : 1;
				map[mx + cx + (my + width) * mapSize] = nodes[x][y].down ? 0 : 1;
				for(int cy = 0; cy < width; ++ cy)
					map[mx + cx + (my + cy) * mapSize] = 0;
			}
		}
	map[mapSize - 2] = 2; // exit
	// delete nodes
	for(int x = 0; x < msize; ++x) delete[] nodes[x];
	delete[] nodes;
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
	items.clear(); // remove items
	mons.erase(std::remove_if(mons.begin(), mons.end(), // remove monsters
		[](const Mon& m)->bool {return m.ai == AI_MON;}), mons.end());
	// load monsters
	FILE* mons_file = fopen(("Maps/" + name + ".mons").c_str(), "r");
	if(mons_file != NULL) {
		int id, x, y;
		while(fscanf(mons_file, "%i,%i,%i", &id, &x, &y) == 3)
			Mon((MON_ID)id, AI_MON, vec2(x, y));
		fclose(mons_file);
	}
	// load items
	FILE* items_file = fopen(("Maps/" + name + ".items").c_str(), "r");
	if(items_file != NULL) {
		int id, x, y;
		while(fscanf(items_file, "%i,%i,%i", &id, &x, &y) == 3)
			Item((ITEM_ID)id, vec2(x, y));
		fclose(items_file);
	}
}


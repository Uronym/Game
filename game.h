#ifndef GAME_H
#define GAME_H

#include<allegro5/allegro.h>
#include<string>

const int TILE_SIZE = 64; // size of tiles in pixels
const int NUM_SPRITES = 5;
const int NUM_TILES = 6;
extern ALLEGRO_BITMAP** sprites;
extern ALLEGRO_BITMAP** tiles;
extern short* map;
extern int mapSize;
extern std::string currentMap;
extern bool col[NUM_TILES]; // collision value for each tile

void load_maze();
void loadMap(std::string name);

#endif

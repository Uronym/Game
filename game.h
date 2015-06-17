#ifndef GAME_H
#define GAME_H

#include<allegro5/allegro.h>
#include<string>
#include"vec2.h"

const int TILE_SIZE = 64; // size of tiles in pixels
const int NUM_SPRITES = 7;
const int NUM_TILES = 6;
const double TURN_LENGTH = 0.25;
extern ALLEGRO_BITMAP** sprites;
extern ALLEGRO_BITMAP** tiles;
extern short* map;
extern int mapSize;
extern std::string currentMap;
extern bool col[NUM_TILES]; // collision value for each tile
extern double last_turn; // time of last turn
extern std::string msg;
void load_maze();
void loadMap(std::string name);
extern vec2 curs;
extern bool curs_mode;

#endif

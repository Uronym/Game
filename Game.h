#ifndef GAME_H
#define GAME_H

#include<allegro5/allegro.h>
#include<string>

const int NUM_SPRITES = 4;
const int NUM_TILES = 2;
static ALLEGRO_BITMAP** sprites;
static ALLEGRO_BITMAP** tiles;
extern short* map;
extern int mapSize;
extern std::string currentMap;

void loadMap(std::string name);

#endif

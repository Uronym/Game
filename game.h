#ifndef GAME_H
#define GAME_H

#include<allegro5/allegro.h>
#include<string>

const int NUM_SPRITES = 4;
const int NUM_TILES = 6;
extern ALLEGRO_BITMAP** sprites;
extern ALLEGRO_BITMAP** tiles;
extern short* map;
extern int mapSize;
extern std::string currentMap;
extern bool** cmap;

void loadMap(std::string name);

#endif

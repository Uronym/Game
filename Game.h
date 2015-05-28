#ifndef GAME_H
#define GAME_H

#include<allegro5/allegro.h>
#include <iostream>

struct Player {
	int x, y;
} p;

static const int NUM_SPRITES = 4;
static const int NUM_TILES = 2;
static ALLEGRO_BITMAP** sprites;
static ALLEGRO_BITMAP** tiles;
short *map = nullptr;
int mapSize;
std::string currentMap = "test";

void loadMap(std::string name);

#endif

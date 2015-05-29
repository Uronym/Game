
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include "Game.h"
#include"item.h"
#include "mon.h"

using namespace std;

const int TILE_SIZE = 64; // size of tiles in pixels

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_KEYBOARD_STATE keyboard_state;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   if(!al_init_image_addon()) {
      fprintf(stderr, "failed to initialize image addon!\n");
      return -1;
   }

   display = al_create_display(704, 576);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }

	// create event queue
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		return -1;
	}
	
	// install keyboard
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

   //load sprites
   sprites = new ALLEGRO_BITMAP*[NUM_SPRITES];
   for(int i = 0; i < NUM_SPRITES; i++)
   {
	   string path = "Sprites/" + to_string((long long)i) + ".png";
	   sprites[i] = al_load_bitmap(path.c_str());
	   if(!sprites[i])
		   return -2;
   }

   //load tiles
   tiles = new ALLEGRO_BITMAP*[NUM_TILES];
   for(int i = 0; i < NUM_TILES; i++)
   {
	   string path = "Tiles/" + to_string((long long)i) + ".png";
	   tiles[i] = al_load_bitmap(path.c_str());
	   if(!tiles[i])
		   return -2;
   }

	al_set_window_title(display, "Game");
	al_register_event_source(event_queue, al_get_display_event_source(display));
	
	loadMap("test");

	vector<Mon> mons;
	vector<Item> items;
	// create some monsters for testing
	mons.push_back(Mon(MON_SLIME, 3, 3));
	mons.push_back(Mon(MON_SLIME, 4, 2));
	mons.push_back(Mon(MON_SLIME, 5, 1));
	// create some items for testing, too
	items.push_back(Item(ITEM_POTION, 5, 5));
	items.push_back(Item(ITEM_POTION, 4, 5));
	
	while(true) { // main loop
		double now = al_get_time(); // nice to know
		// listen for events from allegro **BEFORE** rendering
		// so we can display the results of any input on the same frame
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;
		al_init_timeout(&timeout, 1 / 60);
		bool is_event = al_wait_for_event_until(event_queue, &event, &timeout);
		if(is_event && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {break;}
		// keyboard input
		al_get_keyboard_state(&keyboard_state);
		// too lazy to push close button? Good news!
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_Q)) {break;}
		// player movement
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_UP))
			mons[0].step(MOVE_UP);
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN))
			mons[0].step(MOVE_DOWN);
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT))
			mons[0].step(MOVE_LEFT);
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT))
			mons[0].step(MOVE_RIGHT);
		// rendering
		// "camera" position
		double px; double py;
		mons[0].rpos(px, py);
		al_clear_to_color(al_map_rgb(63, 47, 31)); // clear to a soft brown
		//render map
		for(int x = 0; x < mapSize; ++x) {
			for(int y = 0; y < mapSize; ++y) {
				double rx = 5 + x - px; double ry = 4 + y - py;
				al_draw_bitmap(tiles[map[x + y * mapSize]], TILE_SIZE * rx, TILE_SIZE * ry, 0);
			}
		}
		// render monsters
		for(unsigned i = 0; i < mons.size(); ++i) {
			double ix; double iy;
			mons[i].rpos(ix, iy);
			double rx = 5 + ix - px; double ry = 4 + iy - py;
			al_draw_bitmap(sprites[0], TILE_SIZE * rx, TILE_SIZE * ry, 0);
		}
		// render items
		for(unsigned i = 0; i < items.size(); ++i) {
			double rx = 5 + items[i].x - px; double ry = 4 + items[i].y - py;
			al_draw_bitmap(sprites[1], TILE_SIZE * rx, TILE_SIZE * ry, 0);
		}
		// show off all the sprites
		for(int i = 0; i < NUM_SPRITES; ++i) {
			al_draw_bitmap(sprites[i], TILE_SIZE * i, TILE_SIZE * 7, 0);
		}
		// finish rendering
		al_flip_display();
	}
	
	return 0;
}

void loadMap(std::string name)
{
	ifstream in;
	in.open("Maps/" + name + ".map",ios::in|ios::binary);
	if(in.is_open())
	{
		in.seekg(0, in.end);
		int length = in.tellg();
		in.seekg(0, in.beg);
		mapSize = sqrt((double)length/2.0);
		if(map!=nullptr)
			delete [] map;
		map = new short[mapSize*mapSize];
		in.read((char *)map,length);
		currentMap = name;
		in.close();
	}
}

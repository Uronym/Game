
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include "Game.h"
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
	double last_step = 0; // test variable for player movement
	// create some monsters for render testing
	mons.push_back(Mon(MON_SLIME, 3, 3));
	mons.push_back(Mon(MON_SLIME, 4, 2));
	mons.push_back(Mon(MON_SLIME, 5, 1));
	
	//debug stuff
	p.x = 4;
	p.y = 4;
	while(true) { // main loop
		//render stuff before keyboard input
		al_clear_to_color(al_map_rgb(63, 47, 31)); // a soft brown
		//render map
		for(int x = p.x - 5; x <= p.x + 5; x++)
			for(int y = p.y - 4; y <= p.y + 4; y++)
			{
				if(x>=0&&x<mapSize&&y>=0&&y<mapSize)
				{
					al_draw_bitmap(tiles[map[x+mapSize*y]],TILE_SIZE*(x-p.x+5),TILE_SIZE*(y-p.y+4), 0);
				}
			}
		// render monsters
		for(unsigned i = 0; i < mons.size(); ++i) {
			al_draw_bitmap(sprites[0], TILE_SIZE * mons[i].x, TILE_SIZE * mons[i].y, 0);
		}
		// show off all the sprites
		for(int i = 0; i < NUM_SPRITES; ++i) {
			al_draw_bitmap(sprites[i], TILE_SIZE * i, TILE_SIZE * 7, 0);
		}
		// finish rendering
		al_flip_display();

		// listen for events from allegro
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;
		al_init_timeout(&timeout, 0);
		bool is_event = al_wait_for_event_until(event_queue, &event, &timeout);
		if(is_event && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {break;}
		// keyboard input
		al_get_keyboard_state(&keyboard_state);
		// player movement
		double now = al_get_time();
		if(now - last_step > 0.25) {
			if(al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
				last_step = now; --mons[0].y;
			} if(al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
				last_step = now; ++mons[0].y;
			} if(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
				last_step = now; --mons[0].x;
			} if(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
				last_step = now; ++mons[0].x;
			}
		}
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
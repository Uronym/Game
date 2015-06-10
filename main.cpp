
#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<vector>
#include"astar.h"
#include"game.h"
#include"item.h"
#include"mon.h"
#include"vec2.h"

using namespace std;

const int TILE_SIZE = 64; // size of tiles in pixels
const int SCREEN_W = TILE_SIZE * 11;
const int SCREEN_H = TILE_SIZE * 9;

// an extremely awkward way to move the other monsters!
// (assumes player is mons[0], among other things)
void move_others() {
	for(unsigned i = 1; i < mons.size(); ++i) {
		MOVE_DIR dir; // vector to step in
		bool s = pathfind(dir, mons[i].p, mons[0].p, cmap, mapSize);
		if(s) mons[i].step(dir);
	}
}

// an extremely awkward way to move the player...
void move_plyr(MOVE_DIR dir) {
	if(mons[0].step(dir)) {
		move_others();
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));
	
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

   display = al_create_display(SCREEN_W, SCREEN_H);
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

	al_init_primitives_addon();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_set_window_title(display, "Game");
	// music
	al_install_audio();
	al_init_acodec_addon();
	al_restore_default_mixer();
	ALLEGRO_SAMPLE* music = al_load_sample("maze.ogg");
	ALLEGRO_SAMPLE_INSTANCE* music_inst = NULL;
	music_inst = al_create_sample_instance(music);
	al_set_sample_instance_playmode(music_inst, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(music_inst, al_get_default_mixer());
	al_play_sample_instance(music_inst);
	// colors probably should go in another file
	const ALLEGRO_COLOR COLOR_RED = al_map_rgb(255, 0, 0);
	const ALLEGRO_COLOR COLOR_GREEN = al_map_rgb(0, 255, 0);
	
	load_maze();

	// create some monsters for testing
	Mon(MON_SLIME, vec2(3, 3));
	Mon(MON_SLIME, vec2(7, 7));
	// create some items for testing, too
	items.push_back(Item(ITEM_POTION, vec2(5, 5)));
	items.push_back(Item(ITEM_POTION, vec2(4, 5)));
	
	ALLEGRO_TIMEOUT timeout;

	while(true) { // main loop
		// listen for events from allegro **BEFORE** rendering
		// so we can display the results of any input on the same frame
		ALLEGRO_EVENT allegroEvent; //event is a c++ keyword, don't use it as a variable name
		al_init_timeout(&timeout, 1.0/60.0);
		bool is_event = al_wait_for_event_until(event_queue, &allegroEvent, &timeout);
		if(is_event && allegroEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {break;}
		// keyboard input
		al_get_keyboard_state(&keyboard_state);
		// too lazy to push close button? Good news!
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_Q)) {break;}
		// player movement
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
			move_plyr(MOVE_UP);
		} if(al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
			move_plyr(MOVE_DOWN);
		} if(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
			move_plyr(MOVE_LEFT);
		} if(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
			move_plyr(MOVE_RIGHT);
		}
		// pick up items
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_G)) {
			for(unsigned i = 0; i < items.size(); ++i) {
				if(items[i].p == mons[0].p) {
					mons[0].inv.push_back(items[i]);
					items.erase(items.begin() + i);
				}
			}
		}
		// wield items
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_I)) mons[0].wield(0);
		// unwield items
		if(al_key_down(&keyboard_state, ALLEGRO_KEY_U)) mons[0].wield(-1);
		// rendering
		al_clear_to_color(al_map_rgb(0, 0, 0));
		// "camera" position
		double px; double py;
		mons[0].rpos(px, py);
		//render map
		for(int x = mons[0].p.x - 6; x < mons[0].p.x + 7; ++x) {
			for(int y = mons[0].p.y - 5; y < mons[0].p.y + 6; ++y) {
				if(x < 0 || x >= mapSize) continue;
				if(y < 0 || y >= mapSize) continue;
				double rx = TILE_SIZE * (5 + x - px);
				double ry = TILE_SIZE * (4 + y - py);
				al_draw_bitmap(tiles[map[x + y * mapSize]], rx, ry, 0);
			}
		}
		// render items
		for(unsigned i = 0; i < items.size(); ++i) {
			double rx = TILE_SIZE * (5 + items[i].p.x - px);
			double ry = TILE_SIZE * (4 + items[i].p.y - py);
			al_draw_bitmap(sprites[1], rx, ry, 0);
		}
		// render monsters
		for(unsigned i = 0; i < mons.size(); ++i) {
			double ix; double iy;
			mons[i].rpos(ix, iy);
			double rx = TILE_SIZE * (5 + ix - px);
			double ry = TILE_SIZE * (4 + iy - py);
			al_draw_bitmap(sprites[0], rx, ry, 0);
			// and a health bar for each!
			double hp_pc = mons[i].hp / (double)mons[i].hp_max;
			al_draw_filled_rectangle(rx, ry, rx + TILE_SIZE, ry + TILE_SIZE / 8, COLOR_RED);
			al_draw_filled_rectangle(rx, ry, rx + hp_pc * TILE_SIZE, ry + TILE_SIZE / 8, COLOR_GREEN);
		}
		// render wielded item
		if(mons[0].item != NULL) {
			al_draw_bitmap(sprites[1], TILE_SIZE * 5, TILE_SIZE * 4, 0);
		}
		// render inventory
		for(unsigned i = 0; i < mons[0].inv.size(); ++i) {
			al_draw_bitmap(sprites[1], TILE_SIZE * i, TILE_SIZE * 8, 0);
		}
		// finish rendering
		al_flip_display();
	}
	
	return 0;
}



#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<utility>
#include<vector>
#include"game.h"
#include"item.h"
#include"mon.h"
#include"render.h"
#include"turn.h"
#include"vec2.h"

using namespace std;

const int SCREEN_W = TILE_SIZE * 11;
const int SCREEN_H = TILE_SIZE * 9;

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
	al_init_font_addon();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
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
	
	loadMap("main");
	
	Mon(MON_HUMAN, AI_PLYR, vec2(5, 5)); // create the player
	init_render(); // initialize render variables now

	while(true) { // main loop
		// use keyboard state for cases where the state of the
		// key is more important than the action of pressing it
		// (for instance, movement or something)
		al_get_keyboard_state(&keyboard_state);
		// check events **BEFORE** rendering
		// so results are rendered immediately
		// event is not a C++ keyword; get a better editor
		ALLEGRO_EVENT event;
		bool is_event = al_wait_for_event_timed(event_queue, &event, 1 / 60);
		Mon* plyr = get_plyr();
		if(is_event) {
			switch(event.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				goto END; break;
			case ALLEGRO_EVENT_KEY_CHAR:
				if(plyr == NULL) break;
				switch(event.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					--curs.y; break;
				case ALLEGRO_KEY_DOWN:
					++curs.y; break;
				case ALLEGRO_KEY_LEFT:
					--curs.x; break;
				case ALLEGRO_KEY_RIGHT:
					++curs.x; break;
				case ALLEGRO_KEY_G:
					for(unsigned i = 0; i < items.size(); ++i)
						if(items[i].p == plyr->p) {
							plyr->inv.push_back(items[i]);
							plyr->item = &plyr->inv.back();
							swap(items[i], items.back());
							items.pop_back();
							break;
						}
					break;
				case ALLEGRO_KEY_I:
					plyr->item = plyr->item == NULL ? &plyr->inv[0] : NULL;
					break;
				case ALLEGRO_KEY_Q:
					goto END; break;
				case ALLEGRO_KEY_T:
					if(curs_mode && plyr->item != NULL)
						for(unsigned i = 0; i < plyr->inv.size(); ++i)
							if(plyr->item == &plyr->inv[i]) {
								swap(plyr->inv[i], plyr->inv.back());
								plyr->inv.pop_back();
								plyr->item = NULL;
								for(unsigned i = 0; i < mons.size(); ++i)
									if(mons[i].p == curs)
										mons[i].dmg(1);
								break;
							}
					curs_mode = !curs_mode;
					curs = plyr->p;
					break;
				case ALLEGRO_KEY_U:
					plyr->use(*plyr->item); break;
				} break;
			}
		}
		if(plyr != NULL && !curs_mode) {
			if(al_get_time() - last_turn > TURN_LENGTH
				&& (al_key_down(&keyboard_state, ALLEGRO_KEY_UP)
				|| al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)
				|| al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)
				|| al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT))) {
				turn_step(keyboard_state);
				last_turn = al_get_time();
			}
		}
		render();
	}
	END: return 0;
}


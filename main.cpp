#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include<vector>
#include "Game.h"
#include"mon.h"

using namespace std;

const int TILE = 64; // size of tiles in pixels

int main(int argc, char **argv)
{
   ALLEGRO_DISPLAY *display = NULL;

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

   //load sprites
   sprites = new ALLEGRO_BITMAP*[NUM_SPRITES];
   for(int i = 0; i < NUM_SPRITES; i++)
   {
	   string path = "Sprites/" + to_string((long long)i) + ".png";
	   sprites[i] = al_load_bitmap(path.c_str());
	   if(!sprites[i])
		   return -2;
   }

   al_clear_to_color(al_map_rgb(255,0,255));

	vector<Mon> mons;
	// a test for drawing monsters!
	mons.push_back(Mon(MON_SLIME, 3, 3));
	mons.push_back(Mon(MON_SLIME, 4, 2));
	mons.push_back(Mon(MON_SLIME, 5, 1));

	for(unsigned i = 0; i < mons.size(); ++i) {
		al_draw_bitmap(sprites[0], TILE * mons[i].x, TILE * mons[i].y, 0);
	}

	// showing off the other sprites
	for(int i = 0; i < NUM_SPRITES; ++i) {
		al_draw_bitmap(sprites[i], TILE * i, TILE * 7, 0);
	}

   al_flip_display();
 
   al_rest(10.0);
 
   al_destroy_display(display);
 
   return 0;
}

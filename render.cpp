
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include"game.h"
#include"item.h"
#include"mon.h"
#include"render.h"

ALLEGRO_COLOR COLOR_RED, COLOR_GREEN;

void init_colors() {
	COLOR_RED   = al_map_rgb(255, 0, 0);
	COLOR_GREEN = al_map_rgb(0, 255, 0);
}

void render() { // <-- exactly what it says
	al_clear_to_color(al_map_rgb(0, 0, 0));
	double px, py; mons[0].rpos(px, py); // "camera" position
	for(int x = mons[0].p.x - 6; x < mons[0].p.x + 7; ++x) { // render map
		for(int y = mons[0].p.y - 5; y < mons[0].p.y + 6; ++y) {
			if(x < 0 || x >= mapSize || y < 0 || y >= mapSize) continue;
			double rx = TILE_SIZE * (5 + x - px);
			double ry = TILE_SIZE * (4 + y - py);
			al_draw_bitmap(tiles[map[x + y * mapSize]], rx, ry, 0);
		}
	}
	for(unsigned i = 0; i < items.size(); ++i) { // render items
		double rx = TILE_SIZE * (5 + items[i].p.x - px);
		double ry = TILE_SIZE * (4 + items[i].p.y - py);
		al_draw_bitmap(sprites[1], rx, ry, 0);
	}
	for(unsigned i = 0; i < mons.size(); ++i) { // render monsters
		double ix; double iy;
		mons[i].rpos(ix, iy);
		double rx = TILE_SIZE * (5 + ix - px);
		double ry = TILE_SIZE * (4 + iy - py);
		al_draw_bitmap(sprites[mons[i].dat->tile], rx, ry, 0);
		// and a health bar for each!
		double hp_pc = mons[i].hp / (double)mons[i].hp_max;
		al_draw_filled_rectangle(rx, ry, rx + TILE_SIZE, ry + TILE_SIZE / 8, COLOR_RED);
		al_draw_filled_rectangle(rx, ry, rx + hp_pc * TILE_SIZE, ry + TILE_SIZE / 8, COLOR_GREEN);
		if(mons[i].item != NULL) // wielded item (if it exists)
			al_draw_bitmap(sprites[mons[i].item->dat->tile], TILE_SIZE * 5, TILE_SIZE * 4, 0);
	}
	for(unsigned i = 0; i < mons[0].inv.size(); ++i) // render inventory
		al_draw_bitmap(sprites[mons[0].inv[i].dat->tile], TILE_SIZE * i, TILE_SIZE * 8, 0);
	al_flip_display(); // puts to screen
}


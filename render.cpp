
#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include"game.h"
#include"item.h"
#include"mon.h"
#include"render.h"

ALLEGRO_COLOR COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE;
ALLEGRO_FONT* font;

void init_render() {
	COLOR_BLACK = al_map_rgb(0, 0, 0);
	COLOR_RED   = al_map_rgb(255, 0, 0);
	COLOR_GREEN = al_map_rgb(0, 255, 0);
	COLOR_BLUE  = al_map_rgb(0, 0, 255);
	COLOR_WHITE = al_map_rgb(255, 255, 255);
	font = al_load_font("font.tga", 0, 0);
}

void render() { // <-- exactly what it says
	Mon* plyr = get_plyr();
	double px, py; plyr->rpos(px, py); // "camera" position
	al_clear_to_color(al_map_rgb(0, 0, 0));
	for(int x = plyr->p.x - 6; x < plyr->p.x + 7; ++x) // render map
		for(int y = plyr->p.y - 5; y < plyr->p.y + 6; ++y) {
			if(!vec2(x, y).onsq(mapSize)) continue;
			double rx = TILE_SIZE * (5 + x - px);
			double ry = TILE_SIZE * (4 + y - py);
			al_draw_bitmap(tiles[map[x + y * mapSize]], rx, ry, 0);
		}
	for(unsigned i = 0; i < items.size(); ++i) { // render items
		double rx = TILE_SIZE * (5 + items[i].p.x - px);
		double ry = TILE_SIZE * (4 + items[i].p.y - py);
		al_draw_bitmap(sprites[items[i].dat->tile], rx, ry, 0);
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
		// movement bar?
		double mv_pc = mons[i].move_pts;
		al_draw_filled_rectangle(rx, ry + TILE_SIZE / 8, rx + TILE_SIZE, ry + TILE_SIZE / 4, COLOR_BLACK);
		al_draw_filled_rectangle(rx, ry + TILE_SIZE / 8, rx + mv_pc * TILE_SIZE, ry + TILE_SIZE / 4, COLOR_BLUE);
		if(mons[i].item != NULL) // wielded item (if it exists)
			al_draw_bitmap(sprites[mons[i].item->dat->tile], TILE_SIZE * 5, TILE_SIZE * 4, 0);
	}
	if(curs_mode)
		al_draw_bitmap(sprites[2], (5 + curs.x - px) * TILE_SIZE, (4 + curs.y - py) * TILE_SIZE, 0);
	for(unsigned i = 0; i < plyr->inv.size(); ++i) // render inventory
		al_draw_bitmap(sprites[plyr->inv[i].dat->tile], TILE_SIZE * i, TILE_SIZE * 8, 0);
	al_draw_text(font, COLOR_WHITE, 5.5 * TILE_SIZE, 0, ALLEGRO_ALIGN_CENTER, msg.c_str());
	al_flip_display(); // puts to screen
}


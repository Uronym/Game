
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
int min_x, max_x, min_y, max_y;
double rx, ry;

void init_render() {
	COLOR_BLACK = al_map_rgb(0, 0, 0);
	COLOR_RED   = al_map_rgb(255, 0, 0);
	COLOR_GREEN = al_map_rgb(0, 255, 0);
	COLOR_BLUE  = al_map_rgb(0, 0, 255);
	COLOR_WHITE = al_map_rgb(255, 255, 255);
	font = al_load_font("font.tga", 0, 0);
}

void render_map() {
	for(int x = min_x; x < max_x; ++x)
		for(int y = min_y; y < max_y; ++y) {
			if(!vec2(x, y).onsq(mapSize)) continue;
			double dx = TILE_SIZE * (5 + x - rx);
			double dy = TILE_SIZE * (4 + y - ry);
			al_draw_bitmap(tiles[map[x + y * mapSize]], dx, dy, 0);
		}
}

void render_items() {
	for(unsigned i = 0; i < items.size(); ++i) {
		double dx = TILE_SIZE * (5 + items[i].p.x - rx);
		double dy = TILE_SIZE * (4 + items[i].p.y - ry);
		al_draw_bitmap(sprites[items[i].dat->tile], dx, dy, 0);
	}
}

void render_mons() {
	for(unsigned i = 0; i < mons.size(); ++i) {
		double mx; double my;
		mons[i].rpos(mx, my);
		double dx = TILE_SIZE * (5 + mx - rx);
		double dy = TILE_SIZE * (4 + my - ry);
		al_draw_bitmap(sprites[mons[i].dat->tile], dx, dy, 0);
		// and a health bar for each!
		double hp_pc = mons[i].hp / (double)mons[i].hp_max;
		al_draw_filled_rectangle(dx, dy, dx + TILE_SIZE, dy + TILE_SIZE / 8, COLOR_RED);
		al_draw_filled_rectangle(dx, dy, dx + hp_pc * TILE_SIZE, dy + TILE_SIZE / 8, COLOR_GREEN);
		// movement bar?
		double mv_pc = mons[i].move_pts;
		al_draw_filled_rectangle(dx, dy + TILE_SIZE / 8, dx + TILE_SIZE, dy + TILE_SIZE / 4, COLOR_BLACK);
		al_draw_filled_rectangle(dx, dy + TILE_SIZE / 8, dx + mv_pc * TILE_SIZE, dy + TILE_SIZE / 4, COLOR_BLUE);
		if(mons[i].item != NULL) // wielded item (if it exists)
			al_draw_bitmap(sprites[mons[i].item->dat->tile], dx, dy, 0);
	}
}

void render() { // <-- exactly what it says
	al_clear_to_color(COLOR_BLACK);
	Mon* plyr = get_plyr();
	if(plyr != NULL) {
		plyr->rpos(rx, ry); // "camera" position
		min_x = plyr->p.x - 6; max_x = plyr->p.x + 7; // minimum and maximum
		min_y = plyr->p.y - 5; max_y = plyr->p.y + 6; // coordinates to render
		render_map();
		render_items();
		render_mons();
		if(curs_mode) {
			int dx = TILE_SIZE * (5 + curs.x - rx);
			int dy = TILE_SIZE * (4 + curs.y - ry);
			al_draw_bitmap(sprites[2], dx, dy, 0);
		}
		for(unsigned i = 0; i < plyr->inv.size(); ++i) { // render inventory
			int dx = TILE_SIZE * i;
			int dy = TILE_SIZE * 8;
			al_draw_bitmap(sprites[plyr->inv[i].dat->tile], dx, dy, 0);
		}
		al_draw_text(font, COLOR_WHITE, TILE_SIZE * 5.5, 0, ALLEGRO_ALIGN_CENTER, msg.c_str());
	} else {
		al_draw_text(font, COLOR_RED, TILE_SIZE * 5.5, TILE_SIZE * 4, ALLEGRO_ALIGN_CENTER, "Game Over");
	}
	al_flip_display(); // puts to screen
}


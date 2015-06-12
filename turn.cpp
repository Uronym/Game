
#include<algorithm>
#include<allegro5/allegro.h>
#include<vector>
#include"astar.h"
#include"mon.h"
#include"turn.h"

bool move_comp(Mon* mon1, Mon* mon2) {return mon1->move_pts < mon2->move_pts;}

void init_turn_order(std::vector<Mon*>& turn_order) {
	turn_order.clear();
	for(unsigned i = 0; i < mons.size(); ++i) {
		turn_order.push_back(&mons[i]);
		std::sort(turn_order.begin(), turn_order.end(), move_comp);
	}
}

void turn_step(ALLEGRO_KEYBOARD_STATE& keyboard_state) {
	if(mons.size() == 0) return;
	for(unsigned i = 0; i < mons.size(); ++i)
		mons[i].move_pts += mons[i].dat->spe_base; // give move_pt allowances
	std::vector<Mon*> turn_order;
	init_turn_order(turn_order);
	while(true) {
		Mon* mon = NULL;
		for(unsigned i = 0; i < mons.size(); ++i)
			if(turn_order.back() == &mons[i])
				mon = &mons[i];
		// reinitialize turn_order; a monster probably died
		if(mon == NULL) {init_turn_order(turn_order); continue;}
		if(mon->move_pts < 1) break;
		else {
			mon->move_pts -= 1;
			switch(mon->ai) {
				case AI_MON: {
					MOVE_DIR dir; // direction to move
					bool success = pathfind(dir, mon->p, mons[0].p);
					if(success) mon->step(dir);
					break;
				} case AI_PLYR:
					if(al_key_down(&keyboard_state, ALLEGRO_KEY_UP))
						mon->step(MOVE_UP);
					else if(al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN))
						mon->step(MOVE_DOWN);
					else if(al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT))
						mon->step(MOVE_LEFT);
					else if(al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT))
						mon->step(MOVE_RIGHT);
					break;
				default:
					break;
			}
			std::sort(turn_order.begin(), turn_order.end(), move_comp);
		}
	}
}


#ifndef ASTAR_H
#define ASTAR_H

#include"vec2.h"

bool pathfind(MOVE_DIR& dir, int sx, int sy, int gx, int gy,
				bool** cmap, int msize);

#endif

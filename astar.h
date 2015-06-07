#ifndef ASTAR_H
#define ASTAR_H

#include"vec2.h"

bool pathfind(MOVE_DIR& dir, vec2 s, vec2 g, bool** cmap, int msize);

#endif

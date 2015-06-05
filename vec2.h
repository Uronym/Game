#ifndef VEC2_H
#define VEC2_H

enum MOVE_DIR { // movement directions
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DIRS, // number of MOVE_DIRs
};

struct vec2 {
	int x; int y;
	vec2(int x, int y): x(x), y(y) {}
};

#endif

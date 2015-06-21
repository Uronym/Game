#ifndef VEC2_H
#define VEC2_H

#include<cmath>

struct vec2 {
	int x; int y;
	vec2() {}
	vec2(int x, int y): x(x), y(y) {}
	vec2 operator-() const {return vec2(-x, -y);}
	vec2 operator-(const vec2& v) const {return vec2(x - v.x, y - v.y);}
	vec2 operator+(const vec2& v) const {return vec2(x + v.x, y + v.y);}
	bool operator==(const vec2& v) const {return x == v.x && y == v.y;}
	bool operator!=(const vec2& v) const {return x != v.x || y != v.y;}
	// is point within a square of size s? (useful for map bounds checking)
	bool onsq(int s) const {return x >= 0 && x < s && y >= 0 && y < s;}
	double dist(const vec2& p) const {
		return sqrt(pow(x - p.x, 2.0) + pow(y - p.y, 2.0));}
};

enum MOVE_DIR { // movement directions
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DIRS, // number of MOVE_DIRs
};

const vec2 MOVE_VEC[] = { // vector version of each MOVE_DIR
	vec2(0, -1),
	vec2(0, 1),
	vec2(-1, 0),
	vec2(1, 0),
};

#endif

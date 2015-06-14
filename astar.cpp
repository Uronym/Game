
#include<cmath>
#include<queue>
#include<stack>
#include<vector>
#include"astar.h"
#include"game.h"
#include"vec2.h"

struct Node { // a pathfinding node (for A*)
	vec2 p; // coordinates
	double gd; // distance to goal
	Node(const vec2& p, const vec2& g): p(p), gd(p.dist(g)) {}
	bool operator<(const Node& n) const {return gd > n.gd;}
};

bool pathfind(MOVE_DIR& dir, vec2 s, vec2 g, bool walls) {
	bool found = false;
	// allocate closed
	bool** closed;
	closed = new bool*[mapSize];
	for(int x = 0; x < mapSize; ++x) closed[x] = new bool[mapSize]();
	// allocate parent
	MOVE_DIR** parent;
	parent = new MOVE_DIR*[mapSize];
	for(int x = 0; x < mapSize; ++x) parent[x] = new MOVE_DIR[mapSize];
	// start by making a node at the start
	std::priority_queue<Node, std::vector<Node>> open;
	open.push(Node(s, g));
	// expand the nearest node until goal is
	// reached or no more open nodes exist
	do {
		Node node = open.top(); open.pop();
		if(node.p.x == g.x && node.p.y == g.y) { // found goal; trace path
			std::stack<MOVE_DIR> path;
			vec2 p(node.p);
			while(p.x != s.x || p.y != s.y) {
				path.push(parent[p.x][p.y]);
				p = p + MOVE_VEC[parent[p.x][p.y]];
			}
			if(!path.empty()) {
				found = true;
				// sorta ugly
				switch(path.top()) {
					case MOVE_UP:
						dir = MOVE_DOWN; break;
					case MOVE_DOWN:
						dir = MOVE_UP; break;
					case MOVE_LEFT:
						dir = MOVE_RIGHT; break;
					case MOVE_RIGHT:
						dir = MOVE_LEFT; break;
					default: // this case is very bad
						found = false; break;
				}
			}
			break;
		} else { // expand node to adjacent tiles
			for(int i = 0; i < MOVE_DIRS; ++i) {
				vec2 d = vec2(node.p - MOVE_VEC[i]);
				if(d.onsq(mapSize) && !closed[d.x][d.y]
				   && !(walls && col[map[d.x + d.y * mapSize]])) {
					closed[d.x][d.y] = true;
					parent[d.x][d.y] = (MOVE_DIR)i;
					open.push(Node(d, g));
				}
			}
		}
	} while(!open.empty());
	// delete closed
	for(int x = 0; x < mapSize; ++x) delete[] closed[x];
	delete[] closed;
	// delete parent
	for(int x = 0; x < mapSize; ++x) delete[] parent[x];
	delete[] parent;
	return found;
}



#include<cmath>
#include<queue>
#include<stack>
#include<vector>
#include"astar.h"

double dist(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

struct Node {
	int x; int y; // coordinates
	int gd; // distance to goal
	Node(int x, int y, int gx, int gy): x(x), y(y), gd(dist(x, y, gx, gy)) {}
	bool operator<(const Node& n) const {return gd > n.gd;}
};

bool pathfind(MOVE_DIR& dir, int sx, int sy, int gx, int gy,
				bool** cmap, int msize) {
	bool found = false;
	// allocate closed
	bool** closed;
	closed = new bool*[msize];
	for(int x = 0; x < msize; ++x) closed[x] = new bool[msize]();
	// allocate parent
	MOVE_DIR** parent;
	parent = new MOVE_DIR*[msize];
	for(int x = 0; x < msize; ++x) parent[x] = new MOVE_DIR[msize];
	// start by making a node at the start
	std::priority_queue<Node, std::vector<Node>> open;
	open.push(Node(sx, sy, gx, gy));
	// expand the nearest node until goal is
	// reached or no more open nodes exist
	do {
		Node node = open.top(); open.pop();
		if(node.x == gx && node.y == gy) { // found goal; trace path
			std::stack<MOVE_DIR> path;
			vec2 p(node.x, node.y);
			while(p.x != sx || p.y != sy) {
				path.push(parent[p.x][p.y]);
				p = p - MOVE_VEC[parent[p.x][p.y]];
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
				vec2 d = vec2(node.x - MOVE_VEC[i].x, node.y - MOVE_VEC[i].y);
				if(d.onsq(msize) && !closed[d.x][d.y]) {
					closed[d.x][d.y] = true;
					parent[d.x][d.y] = (MOVE_DIR)i;
					open.push(Node(d.x, d.y, gx, gy));
				}
			}
		}
	} while(!open.empty());
	// delete closed
	for(int x = 0; x < msize; ++x) delete[] closed[x];
	delete[] closed;
	// delete parent
	for(int x = 0; x < msize; ++x) delete[] parent[x];
	delete[] parent;
	return found;
}


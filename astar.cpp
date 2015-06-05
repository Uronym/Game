
#include<cmath>
#include<queue>
#include<stack>
#include<vector>
#include"astar.h"

int dirs[][2] = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1},
};

double dist(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

struct Node {
	int x; int y; // coordinates
	int gd; // distance to goal
	Node(int x, int y, int gx, int gy): x(x), y(y), gd(dist(x, y, gx, gy)) {}
	bool operator<(const Node& n2) const {return gd > n2.gd;}
};

void pathfind(MOVE_DIR& dir, int sx, int sy, int gx, int gy,
				bool** cmap, int msize) {
	// allocate closed
	bool** closed;
	closed = new bool*[msize];
	for(int x = 0; x < msize; ++x) closed[x] = new bool[msize];
	// allocate parent
	int*** parent;
	parent = new int**[msize];
	for(int x = 0; x < msize; ++x) {
		parent[x] = new int*[msize];
		for(int y = 0; y < msize; ++y) parent[x][y] = new int[2];
	}
	// start by making a node at the start
	std::priority_queue<Node, std::vector<Node>> open;
	open.push(Node(sx, sy, gx, gy));
	// expand the nearest node until goal is
	// reached or no more open nodes exist
	do {
		Node node = open.top(); open.pop();
		if(node.x == gx && node.y == gy) { // trace path
			std::stack<int> pathx; std::stack<int> pathy;
			int nx = node.x; int ny = node.y;
			do { // two stacks looks a little silly...
				pathx.push(nx); pathy.push(ny);
				nx = parent[nx][ny][0]; ny = parent[nx][ny][1];
			} while(nx != sx || ny != sy);
			int x2 = pathx.top(); pathx.pop(); int x1 = pathx.top();
			int y2 = pathy.top(); pathy.pop(); int y1 = pathy.top();
			if(y2 - y1 == -1) dir = MOVE_UP;
			else if(y2 - y1 == 1) dir = MOVE_DOWN;
			else if(x2 - x1 == -1) dir = MOVE_LEFT;
			else if(x2 - x1 == 1) dir = MOVE_RIGHT;
			break;
		} else { // add another node in each adjacent open tile
			for(int i = 0; i < 4; ++i) {
				int dx = node.x + dirs[i][0];
				int dy = node.y + dirs[i][1];
				if(!closed[dx][dy]) {
					closed[dx][dy] = true;
					open.push(Node(dx, dy, gx, gy));
				}
			}
		}
	} while(!open.empty());
	// delete parent
	for(int x = 0; x < msize; ++x) {
		for(int y = 0; y < msize; ++y) delete[] parent[x][y];
		delete[] parent[x];
	}
	delete[] parent;
	// delete closed
	for(int x = 0; x < msize; ++x) delete[] closed[x];
	delete[] closed;
}


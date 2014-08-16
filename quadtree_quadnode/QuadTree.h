#ifndef QUADTREE_H
#define QUADTREE_H
#include "OpenglContext.h"
#include "Terrain.h"
#include <vector>
#include <string>
#include <sstream>

enum Key
{
	NE,
	NW,
	SW,
	SE,
	PARENT
};

class QuadNode
{
public:
	QuadNode(float _x, float _y, float _width)
	{
		x=_x;
		y=_y;
		width=_width;
		leaf = false;
		for(int i=0; i<4; i++)
			nodes[i] = NULL;
	}

	float x;
	float y;
	float width;
	bool leaf;
	QuadNode* nodes[4];
};

class QuadTree
{
public:
	QuadTree(int maxLevel);
	QuadTree(const QuadTree&);
	~QuadTree();
	void load(Terrain* t);
	bool init();
	void clear();
	void createMesh(Mesh* m);
	string toString();
private:
	void load(QuadNode* node, float x, float y, float width, Key key, int level, float sX, float sY, float eX, float eY);
	void contrasts(int width, bool (&subs)[4], float sX, float sY, float eX, float eY);
	void createMesh(Mesh* m, QuadNode* node, uint heightID[4]);
	float getHeight(int x, int y);
	string toString(QuadNode* qn);
	QuadNode* parent;
	float x;
	float y;
	float width;
	int level;
	int maxLevel;
	float tolerance;
	float* heightmap;
};

#endif

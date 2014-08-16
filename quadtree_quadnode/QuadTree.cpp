#include "QuadTree.h"
#include <cstdlib>

QuadTree::QuadTree(int _maxLevel)
{
	maxLevel = _maxLevel;
	tolerance = 100;
	parent = NULL;
}

QuadTree::QuadTree(const QuadTree& other)
{

}

QuadTree::~QuadTree()
{

}

void QuadTree::load(Terrain* t)
{
	width = t->width;
	heightmap = t->heightmap;
	load(parent, x, y, width, PARENT,0,0,0,width,width);
}

void QuadTree::load(QuadNode* node, float x, float y, float width, Key key, int level, float sX, float sY, float eX, float eY)
{
	switch(key)
	{
	case PARENT:
		node=new QuadNode(0,0,width);break;
	case NE:
		node=new QuadNode(x+width/4, y+width/4, width/2);break;
	case NW:
		node=new QuadNode(x-width/4, y+width/4, width/2);break;
	case SW:
		node=new QuadNode(x-width/4, y-width/4, width/2);break;
	case SE:
		node=new QuadNode(x+width/4, y-width/4, width/2);break;
	}
	if(level >= maxLevel)
		return;

	bool subs[4];
	contrasts(width,subs,sX,sY,eX,eY);
	for(int i=0; i<4; i++)
	{
		//if(subs[i])
		{
			node->leaf=false;
			float len = eX-sX;
			switch(i)
			{
				case NE:
					load(node->nodes[NE], node->x, node->y, node->width, NE,level+1,sX+len/2,sY,eX,eY-len/2);break;
				case NW:
					load(node->nodes[NW], node->x, node->y, node->width, NW,level+1,sX,sY,eX-len/2,eY-len/2);break;
				case SW:
					load(node->nodes[SW], node->x, node->y, node->width, SW,level+1,sX,sY+len/2,eX-len/2,eY);break;
				case SE:
					load(node->nodes[SE], node->x, node->y, node->width, SE,level+1,sX+len/2,sY+len/2,eX,eY);break;
			}
		}
	}
}

void QuadTree::contrasts(int width, bool (&subs)[4], float sX, float sY, float eX, float eY)
{
	int len = eX-sX;
	float mins[4];
	float maxs[4];
	
	for(int i=0; i<4; i++)
	{
		mins[i] = 100000;
		maxs[i] = -100000;
	}

	for(int x=sX; x<eX; x++)
	{
		for(int y=sY; y<eY; y++)
		{
			if(x<len/2&&y<len/2)
			{
				if(heightmap[x+y*width]>maxs[NW])
					maxs[NW]=heightmap[x+y*width];
				if(heightmap[x+y*width]<mins[NW])
					mins[NW]=heightmap[x+y*width];
			}
			if(x>len/2&&y<len/2)
			{
				if(heightmap[x+y*width]>maxs[NE])
					maxs[NE]=heightmap[x+y*width];
				if(heightmap[x+y*width]<mins[NE])
					mins[NE]=heightmap[x+y*width];
			}
			if(x<len/2&&y>len/2)
			{
				if(heightmap[x+y*width]>maxs[SW])
					maxs[SW]=heightmap[x+y*width];
				if(heightmap[x+y*width]<mins[SW])
					mins[SW]=heightmap[x+y*width];
			}
			if(x<len/2&&y<len/2)
			{
				if(heightmap[x+y*width]>maxs[SW])
					maxs[SW]=heightmap[x+y*width];
				if(heightmap[x+y*width]<mins[SW])
					mins[SW]=heightmap[x+y*width];
			}
		}
	}
	for(int i=0; i<4; i++)
	{
		if(maxs[i] - mins[i] < tolerance)
			subs[i] = 1;
		else
			subs[i] = 0;
	}
}


bool QuadTree::init()
{
	return true;
}



void QuadTree::clear()
{
}

void QuadTree::createMesh(Mesh* m)
{
	m->beginID(0);
		createMesh(m,parent,NULL);
	m->endID(0);
}

void QuadTree::createMesh(Mesh* m, QuadNode* node, uint heightID[4])
{
	if(!node)
		return;
	float nx = node->x;
	float ny = node->y;
	float nwidth = node->width;
	Vector3d n(0,0,0);
	Vector3d vNE(nx+nwidth/2, getHeight(nx+nwidth, ny+nwidth) ,ny+nwidth/2);
	Vector3d vNW(nx-nwidth/2, getHeight(nx, ny+nwidth) ,ny+nwidth/2);
	Vector3d vSW(nx-nwidth/2, getHeight(nx, ny) ,ny-nwidth/2);
	Vector3d vSE(nx+nwidth/2, getHeight(nx+width, ny) ,ny-nwidth/2);
	Vector3d vCe(nx,getHeight(nx+width/2, ny+width/2),ny);
	/*
	if(!node->leaf)
	{
		Vector3d vN(nx, getHeight(nx+nwidth/2,ny+nwidth) ,ny+nwidth/2);
		Vector3d vW(nx-nwidth/2, getHeight(nx, ny+width/2) ,ny);
		Vector3d vE(nx+nwidth/2, getHeight(nx+width, ny+width/2) ,ny);
		Vector3d vS(nx, getHeight(nx+nwidth/2, ny) ,ny-nwidth/2);
	}
	*/
	//else
	//{
		int e = m->AddVertex(0,Vertex(vNE, n, 1,0,0));
		int f = m->AddVertex(0,Vertex(vNW, n, .5,.5,0));
		int g = m->AddVertex(0,Vertex(vSW, n, 0,1,0));
		int h = m->AddVertex(0,Vertex(vSE, n, 0,.5,.5));
		int i = m->AddVertex(0,Vertex(vCe, n, 0,0,1));
		m->AddTriangle(0,e,i,f);
		m->AddTriangle(0,f,i,g);
		m->AddTriangle(0,g,i,h);
		m->AddTriangle(0,h,i,e);

	//}
	for(int i=0; i<4; i++)
	{
		if(node->nodes[i])
			createMesh(m,node->nodes[i],heightID);
		/*
		else
		{
			switch(i)
			{
			case NE:
			case NW:
			case N


			}
		}
		*/
	}
}

float QuadTree::getHeight(int x, int y)
{
	if(x<0)
		x=0;
	if(y<0)
		y=0;
	if(x>=width)
		x=width-1;
	if(y>=width)
		y=width-1;
	return heightmap[int(x+y*width)];
}

string QuadTree::toString()
{
	return toString(parent);
}

string QuadTree::toString(QuadNode* qn)
{
	if(!qn)
		return "	NA\n";
	stringstream ss;
	ss << qn->x << ", " << qn->y << ", width= " << qn->width <<endl;
	for(int i=0; i<4;i++)
	{
		ss<<toString(qn->nodes[i]);
	}
	return ss.str();

}

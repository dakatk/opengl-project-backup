#include "AABB.h"

AABB initAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax)
{
	Point3f _min = {.x=xMin, .y=yMin, .z=zMin};
	Point3f _max = {.x=xMax, .y=yMax, .z=zMax};
	AABB _a = {.min=_min, .max=_max};

	return _a;
}

AABB initFromVertices(Vertex* v, int vertAmt)
{
	float xMin, yMin, zMin;
	float xMax, yMax, zMax;

	xMin = v[0].x;
	yMin = v[0].y;
	zMin = v[0].z;
	xMax = v[0].x;
	yMax = v[0].y;
	zMax = v[0].z;

	int i;
	for(i = 1; i < vertAmt; i ++)
	{
		if(v[i].x > xMax) xMax = v[i].x;
		if(v[i].x < xMin) xMin = v[i].x;
		if(v[i].y > yMax) yMax = v[i].y;
		if(v[i].y < yMin) yMin = v[i].y;
		if(v[i].z > zMax) zMax = v[i].z;
		if(v[i].z < zMin) zMin = v[i].z;
	}

	AABB _a = initAABB(xMin, yMin, zMin, xMax, yMax, zMax);
	return _a;
}

bool intersects(AABB* a, AABB* b)
{
	if(a->min.x > b->max.x) return 0;
	else if(b->min.x > a->max.x) return 0;
	else if(a->min.y > b->max.y) return 0;
	else if(b->min.y > a->max.y) return 0;
	else if(a->min.z > b->max.z) return 0;
	else if(b->min.z > a->max.z) return 0;

	return 1;
}

void moveAABB(AABB* a, float dx, float dy, float dz)
{
	a->min.x += dx;
	a->min.y += dy;
	a->min.z += dz;
	a->max.x += dx;
	a->max.y += dy;
	a->max.z += dz;
}

void copyAABB(AABB* to, AABB* from)
{
	to->min.x = from->min.x;
	to->min.y = from->min.y;
	to->min.z = from->min.z;
	to->max.x = from->max.x;
	to->max.y = from->max.y;
	to->max.z = from->max.z;
}

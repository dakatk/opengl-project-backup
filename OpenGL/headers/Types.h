#ifndef TYPES_H__
#define TYPES_H__

#include "../classes/Matrix.h"

typedef char* string;
typedef int bool;

typedef struct {

	float x;
	float y;
	float z;
} Point3f;

typedef struct {

	float u;
	float v;
} Coord2f;

typedef struct {

	float x;
	float y;
	float z;
	float w;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
} Vertex;

typedef struct {

	Point3f min;
	Point3f max;
} AABB;

typedef struct {

	AABB* pool;
	int max;
	int amt;
} ComplexAABB;

typedef struct {

	Point3f* pos;
	AABB* bounds;
	unsigned int vao;
	unsigned int tex;
	int vamt;
	int pamt;
} GameObject;

#endif

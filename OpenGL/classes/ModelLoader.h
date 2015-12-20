#ifndef OBJ_LOADER_H__
#define OBJ_LOADER_H__

#include "AABB.h"

extern Vertex* loadObj(const string filename, float scale, int** indices, int* amount);
extern Vertex initVertex(float x, float y, float z, float w, float u, float v, float nx, float ny, float nz);

#endif

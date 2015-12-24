#ifndef AABB_H__
#define AABB_H__

#include "../headers/Types.h"

extern AABB initAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax);
extern AABB initFromVertices(Vertex* v, int vertAmt);
extern bool intersects(AABB* a, AABB* b);
extern void moveAABB(AABB* a, float dx, float dy, float dz);
extern void copyAABB(AABB* to, AABB* from);

extern ComplexAABB initComplexAABB(int amount);
extern void addAABB(ComplexAABB* ca, AABB a);
extern bool intersectsAll(ComplexAABB* ca, ComplexAABB* cb);
extern bool intersectsAny(ComplexAABB* ca, AABB* a);
extern bool intersectsOne(ComplexAABB* ca, AABB* a, int index);
extern void moveAllAABBs(ComplexAABB* ca, float dx, float dy, float dz);
extern void moveOneAABB(ComplexAABB* ca, int index, float dx, float dy, float dz);
extern void copyComplexAABB(ComplexAABB* to, ComplexAABB* from);
extern void destroyComplexAABB(ComplexAABB ca);

#endif

#ifndef PHYSICS_H__
#define PHYSICS_H__

#include "AABB.h"
#include "../headers/Types.h"
#include "Camera.h"

bool canMove[3];//forward, up, strafe
bool noClip;

extern void checkAABBPhysics(AABB* player, GameObject* pool, int numObjects);
extern void checkAABBHeights(AABB* player, GameObject* pool, int numObjects);

#endif

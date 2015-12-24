#include "Physics.h"
#include <stdio.h>

void checkAABBPhysics(AABB* player, GameObject* pool, int numObjects)
{
	moveAABB(player, fdx, 0.0f, fdz);

	if(noClip)
	{
		moveAABB(player, sdx, 0.0f, sdz);
		return;
	}
	canMove[0] = 1;

	int i, j;
	for(i = 0; i < numObjects; i ++)
		for(j = 0; j < pool[i].pamt; j ++)
			if(intersects(player, &pool[i].bounds[j]) && canMove[0])
			{
				canMove[0] = 0;
				moveAABB(player, -fdx, 0.0f, -fdz);
				break;
			}

	moveAABB(player, sdx, 0.0f, sdz);
	canMove[2] = 1;

	for(i = 0; i < numObjects; i ++)
		for(j = 0; j < pool[i].pamt; j ++)
			if(intersects(player, &pool[i].bounds[j]))
			{
				canMove[2] = 0;
				moveAABB(player, -sdx, 0.0f, -sdz);
				break;
			}
}

void checkAABBHeights(AABB* player, GameObject* pool, int numObjects)
{
	moveAABB(player, 0.0f, fdy, 0.0f);

	if(noClip)
		return;
	canMove[1] = 1;

	int i, j;
	for(i = 0; i < numObjects; i ++)
		for(j = 0; j < pool[i].pamt; j ++)
		{
			if(intersects(player, &pool[i].bounds[j]))
			{
				canMove[1] = 0;
				moveAABB(player, 0.0f, -fdy, 0.0f);
				break;
			}
		}
}

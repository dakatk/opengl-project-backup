#include "AABB.h"
#include <stdlib.h>

ComplexAABB initComplexAABB(int amount)
{
	ComplexAABB _ca;

	_ca.amt = 0;
	_ca.max = amount;
	_ca.pool = calloc(amount, sizeof(AABB));

	return _ca;
}

void addAABB(ComplexAABB* ca, AABB a)
{
	if(ca->amt < ca->max)
	{
		ca->pool[ca->amt] = a;
		ca->amt ++;
	}
}

bool intersectsAll(ComplexAABB* ca, ComplexAABB* cb)
{
	int i, j;
	for(i = 0; i < ca->amt; i ++)
		for(j = 0; j < cb->amt; j ++)
			if(intersects(&(ca->pool[i]), &(cb->pool[j])))
				return 1;
	return 0;
}

bool intersectsAny(ComplexAABB* ca, AABB* a)
{
	int i;
	for(i = 0; i < ca->amt; i ++)
		if(intersects(&(ca->pool[i]), a))
			return 1;

	return 0;
}

bool intersectsOne(ComplexAABB* ca, AABB* a, int index)
{
	return intersects(&(ca->pool[index]), a);
}

void moveAllAABBs(ComplexAABB* ca, float dx, float dy, float dz)
{
	int i;
	for(i = 0; i < ca->amt; i ++)
		moveAABB(&(ca->pool[i]), dx, dy, dz);
}

void moveOneAABB(ComplexAABB* ca, int index, float dx, float dy, float dz)
{
	moveAABB(&(ca->pool[index]), dx, dy, dz);
}

void copyComplexAABB(ComplexAABB* to, ComplexAABB* from)
{
	if(to->amt != from->amt)
		return;

	int i;
	for(i = 0; i < to->amt; i ++)
		copyAABB(&(to->pool[i]), &(from->pool[i]));

}

void destroyComplexAABB(ComplexAABB ca)
{
	free(ca.pool);
}

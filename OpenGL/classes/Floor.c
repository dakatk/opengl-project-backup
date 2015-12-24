#include "Floor.h"
#include "GameObject.h"

#include <stdlib.h>

static Vertex createVertex(float x, float y, float z, float w, float u, float v, float nx, float ny, float nz)
{
	Vertex v_ = {.x=x, .y=y, .z=z, .w=w, .u=u, .v=v, .nx=nx, .ny=ny, .nz=nz};
	return v_;
}

GameObject createFloor(const char* filename, int amt, unsigned int prog)
{
	Vertex* floorVerts = (Vertex*)calloc(8, sizeof(Vertex));
	int* floorInds = (int*)calloc(12, sizeof(int));

	floorVerts[0] = createVertex(-FIELD_WIDTH, -FIELD_HEIGHT - VELOCITY, -FIELD_DEPTH, 1.0f, 0.0f, 1.0f, -0.5f, 0.0f, -0.5f);
	floorVerts[1] = createVertex(FIELD_WIDTH, -FIELD_HEIGHT - VELOCITY, -FIELD_DEPTH, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, -0.5f);
	floorVerts[2] = createVertex(FIELD_WIDTH, -FIELD_HEIGHT - VELOCITY, FIELD_DEPTH, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.5f);
	floorVerts[3] = createVertex(-FIELD_WIDTH, -FIELD_HEIGHT - VELOCITY, FIELD_DEPTH, 1.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f);
	floorVerts[4] = createVertex(-FIELD_WIDTH, -FIELD_HEIGHT - 100.0f + VELOCITY, -FIELD_DEPTH, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	floorVerts[5] = createVertex(FIELD_WIDTH, -FIELD_HEIGHT - 100.0f + VELOCITY, -FIELD_DEPTH, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	floorVerts[6] = createVertex(FIELD_WIDTH, -FIELD_HEIGHT - 100.0f + VELOCITY, FIELD_DEPTH, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	floorVerts[7] = createVertex(-FIELD_WIDTH, -FIELD_HEIGHT - 100.0f + VELOCITY, FIELD_DEPTH, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	floorInds[0] = 3;
	floorInds[1] = 1;
	floorInds[2] = 0;
	floorInds[3] = 3;
	floorInds[4] = 2;
	floorInds[5] = 1;
	floorInds[6] = 4;
	floorInds[7] = 5;
	floorInds[8] = 7;
	floorInds[9] = 5;
	floorInds[10] = 6;
	floorInds[11] = 7;

	GameObject _g = createObject(filename, floorVerts, floorInds, 8, 12, amt, prog);
	free(floorVerts);
	free(floorInds);

	return _g;
}

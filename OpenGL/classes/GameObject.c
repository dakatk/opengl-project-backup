#include "GameObject.h"
#include "AABB.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>

static int isFile(const char* filename)
{
	FILE* file;
	file = fopen(filename, "r");

	return file != NULL;
}

static unsigned int loadTexture(const char* filename)
{
	if(!isFile(filename))
	{
		fprintf(stderr, "File '%s' does not exist!", filename);
		return (unsigned int)0;
	}

	unsigned int texId;
	int width, height;

	glGenTextures(1, &texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	unsigned char* image;
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

static unsigned int initVertexBuffer(Vertex* v, int v_amt)
{
	unsigned int vbo_v;

	glGenBuffers(1, &vbo_v);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
	glBufferData(GL_ARRAY_BUFFER, v_amt * sizeof(Vertex), v, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo_v;
}

static unsigned int initIndexBuffer(int *i, int i_amt)
{
	unsigned int ibo_i;

	glGenBuffers(1, &ibo_i);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_i);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_amt * sizeof(int), i, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ibo_i;
}

static unsigned int initVertexArray(unsigned int vbo_v, unsigned int ibo_i, unsigned int prog)
{
	GLuint vao_a;

	glGenVertexArrays(1, &vao_a);
	glBindVertexArray(vao_a);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
	int posAttr = glGetAttribLocation(prog, "position");
	int texAttr = glGetAttribLocation(prog, "texcoord");
	int normAttr = glGetAttribLocation(prog, "normal");

	glEnableVertexAttribArray(posAttr);
	glVertexAttribPointer(posAttr, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(texAttr);
	glVertexAttribPointer(texAttr, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)offsetof(Vertex, u));
	glEnableVertexAttribArray(normAttr);
	glVertexAttribPointer(normAttr, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)offsetof(Vertex, nx));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_i);
	glBindVertexArray(0);

	return vao_a;
}

GameObject createObject(const char* texFile, Vertex* vertices, int* indices, int vertAmt, int indAmt, int posAmt, unsigned int shadeProg)
{
	GameObject _g;
	_g.vamt = indAmt;
	_g.pamt = posAmt;

	Point3f _p = {0.0f, 0.0f, 0.0f};
	AABB _a = initFromVertices(vertices, vertAmt);

	_g.pos = (Point3f*)calloc(posAmt, sizeof(Point3f));
	_g.bounds = (AABB*)calloc(posAmt, sizeof(AABB));

	int i;
	for(i = 0; i < posAmt; i ++)
	{
		_g.pos[i].x = _p.x;
		_g.pos[i].y = _p.y;
		_g.pos[i].z = _p.z;

		copyAABB(&_g.bounds[i], &_a);
	}

	GLuint vbo = initVertexBuffer(vertices, vertAmt);
	GLuint ibo = initIndexBuffer(indices, indAmt);
	_g.vao = initVertexArray(vbo, ibo, shadeProg);
	_g.tex = loadTexture(texFile);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	return _g;
}

void moveObjectAll(GameObject* o, float dx, float dy, float dz)
{
	int i;
	for(i = 0; i < o->pamt; i ++)
		moveObject(o, i, dx, dy, dz);
}

void moveObject(GameObject* o, int index, float dx, float dy, float dz)
{
	(o->pos[index]).x += dx;
	(o->pos[index]).y += dy;
	(o->pos[index]).z += dz;

	moveAABB(&o->bounds[index], dx, dy, dz);
}

void deleteObjects(GameObject* o, int numObjects)
{
	int i;
	for(i = 0; i < numObjects; i ++)
	{
		free(o[i].bounds);
		free(o[i].pos);

		glDeleteVertexArrays(1, &(o[i].vao));
	}
}

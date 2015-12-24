#include "ModelLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int getVertexAmt(const string filename)
{
	FILE* fp = fopen(filename, "r");
	int count = 0;

	for(;;)
	{
		char lineheader[128];

		int res = fscanf(fp, "%s", lineheader);
		if(res == EOF)
			break;

		if(strcmp(lineheader, "v") == 0)
			count ++;
	}
	fclose(fp);

	return count;
}

static int getTexAmt(const string filename)
{
	FILE* fp = fopen(filename, "r");
	int count = 0;

	for(;;)
	{
		char lineheader[128];

		int res = fscanf(fp, "%s", lineheader);
		if(res == EOF)
			break;

		if(strcmp(lineheader, "vt") == 0)
			count ++;
	}
	fclose(fp);

	return count;
}

static int getNormAmt(const string filename)
{
	FILE* fp = fopen(filename, "r");
	int count = 0;

	for(;;)
	{
		char lineheader[128];

		int res = fscanf(fp, "%s", lineheader);
		if(res == EOF)
			break;

		if(strcmp(lineheader, "vn") == 0)
			count ++;
	}
	fclose(fp);

	return count;
}

static int getIndexAmt(const string filename)
{
	FILE* fp = fopen(filename, "r");
	int count = 0;

	for(;;)
	{
		char lineheader[128];

		int res = fscanf(fp, "%s", lineheader);
		if(res == EOF)
			break;

		if(strcmp(lineheader, "f") == 0)
			count ++;
	}
	fclose(fp);

	return count * 3;
}

Vertex* loadObj(const string filename, float scale, int** indices, int* amount)
{
	int vsize = getVertexAmt(filename);
	int tsize = getTexAmt(filename);
	int nsize = getNormAmt(filename);
	int isize = getIndexAmt(filename);

	Vertex* vertices = (Vertex*)calloc(isize, sizeof(Vertex));
	Point3f* verts = (Point3f*)calloc(vsize, sizeof(Point3f));
	Coord2f* texs = (Coord2f*)calloc(tsize, sizeof(Coord2f));
	Point3f* norms = (Point3f*)calloc(nsize, sizeof(Point3f));

	int* vindices = (int*)calloc(isize, sizeof(int));
	int* tindices = (int*)calloc(isize, sizeof(int));
	int* nindices = (int*)calloc(isize, sizeof(int));

	*indices = (int*)calloc(isize + 1, sizeof(int));
	FILE* file = fopen(filename, "r");

	if(file == NULL)
	{
		fprintf(stderr, "File %s does not exist", filename);
		return NULL;
	}

	int vert_amt, norm_amt, tex_amt, ind_amt;
	vert_amt = norm_amt = tex_amt = ind_amt = 0;

	for(;;)
	{
		char lineheader[128];

		int res = fscanf(file, "%s", lineheader);
		if(res == EOF)
			break;

		if(strcmp(lineheader, "v") == 0)
		{
			Point3f v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			verts[vert_amt ++] = v;
		}

		else if(strcmp(lineheader, "vt") == 0)
		{
			Coord2f t;
			fscanf(file, "%f %f\n", &t.u, &t.v);
			texs[tex_amt ++] = t;
		}

		else if(strcmp(lineheader, "vn") == 0)
		{
			Point3f n;
			fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
			norms[norm_amt ++] = n;
		}

		else if(strcmp(lineheader, "f") == 0)
		{
			int vertexIndex[3], texIndex[3], normalIndex[3];
			int points = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &texIndex[0], &normalIndex[0],
				&vertexIndex[1], &texIndex[1], &normalIndex[1],
				&vertexIndex[2], &texIndex[2], &normalIndex[2]);

			if(points != 9)
			{
				fprintf(stderr, "Faces not triangulated in files %s, cannot load model correctly!", filename);
				return NULL;
			}
			vindices[ind_amt] = vertexIndex[0] - 1;
			vindices[ind_amt + 1] = vertexIndex[1] - 1;
			vindices[ind_amt + 2] = vertexIndex[2] - 1;
			tindices[ind_amt] = texIndex[0] - 1;
			tindices[ind_amt + 1] = texIndex[1] - 1;
			tindices[ind_amt + 2] = texIndex[2] - 1;
			nindices[ind_amt] = normalIndex[0] - 1;
			nindices[ind_amt + 1] = normalIndex[1] - 1;
			nindices[ind_amt + 2] = normalIndex[2] - 1;
			ind_amt += 3;
		}
	}

	int i, nverts = 0;
	for(i = 0; i < ind_amt; i ++)
	{
		Point3f _v = verts[(vindices[i])];
		Coord2f _t = texs[(tindices[i])];
		Point3f _n = norms[(nindices[i])];

		_v.x *= scale;
		_v.y *= scale;
		_v.z *= scale;

		vertices[nverts ++] = initVertex(_v.x, _v.y, _v.z, 1.0f, _t.u, _t.v, _n.x, _n.y, _n.z);
		(*indices)[i] = i;
	}
	*amount = nverts;

	free(verts);
	free(texs);
	free(norms);
	free(vindices);
	free(tindices);
	free(nindices);
	fclose(file);

	return vertices;
}

Vertex initVertex(float x, float y, float z, float w, float u, float v, float nx, float ny, float nz)
{
	Vertex v_ = {.x=x, .y=y, .z=z, .w=w, .u=u, .v=v, .nx=nx, .ny=ny, .nz=nz};
	return v_;
}

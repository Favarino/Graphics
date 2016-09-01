#pragma once


struct Geometry
{
	unsigned vbo, ibo, vao, size;

	//Vertex buffer object : an array of vertices
	//Index buffer object  : an array of indices (triangles)
	//Vertex array object  : groups the two with some formatting
	//size				   : number of triangles
};

Geometry makeGeometry(const struct Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize);

void freeGeometry(Geometry &geo);

Geometry loadOBJ(const char *path);

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);

Shader loadShader(const char *vpath, const char *fpath);

void freeShader(Shader &shader);

void draw(const Shader &shader, const Geometry &geo);

void draw(const Shader &shader, const Geometry &geo, float time);

void draw(const Shader &s, const Geometry &g, const float M[16], const float V[16], const float P[16]);
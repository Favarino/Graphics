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

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);

void freeShader(Shader &shader);

void draw(const Shader &, const Geometry &);
#pragma once

struct Geometry
{
	unsigned vbo, ibo, vao, size;

	//Vertex buffer object : an array of vertices
	//Index buffer object  : an array of indices (triangles)
	//Vertex array object  : groups the two with some formatting
	//size				   : number of triangles
};

struct Shader
{
	unsigned handle;

	bool depthTest;
	bool additiveBlend;
	bool faceCulling;
};

struct Texture
{
	unsigned handle;
	int width, height, format;
};

struct Framebuffer
{
	unsigned handle;
	unsigned width, height, nColors;

	Texture depth;
	Texture colors[8];
};
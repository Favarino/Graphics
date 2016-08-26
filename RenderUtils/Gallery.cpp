#include "Gallery.h"

bool Gallery::makeShader(const char * name, const char * vsource, const char * fsource)
{
	shaders[name] = ::makeShader(vsource, fsource);

	return false;
}

bool Gallery::loadShader(const char * name, const char * vpath, const char * fpath)
{
	shaders[name] = ::loadShader(vpath, fpath);
	return false;
}

bool Gallery::makeObject(const char * name, const Vertex * verts, size_t vsize, const unsigned * tris, size_t tsize)
{
	objects[name] = makeGeometry(verts, vsize, tris, tsize);
	return false;
}

bool Gallery::loadObjectOBJ(const char * name, const char * path)
{
	objects[name] = loadOBJ(path);
	return false;
}

const Geometry & Gallery::getObject(const char * name)
{
	return objects[name];
}

const Shader & Gallery::getShader(const char * name)
{
	return shaders[name];
}

bool Gallery::init()
{
	loadObjectOBJ("Sphere", "../res/models/sphere.obj");
	loadShader("Test", "../res/shaders/simpleVert.txt", "../res/shaders/simpleFrag.txt");
	return false;
}

bool Gallery::term()
{
	for each (auto shader in shaders)
	{
		freeShader(shader.second);
	}
	for each (auto object in objects)
	{
		freeGeometry(object.second);
	}
	return false;
}

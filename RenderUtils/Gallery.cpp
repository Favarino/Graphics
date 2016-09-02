#include "Gallery.h"

bool Gallery::makeShader(const char * name, const char * vsource, const char * fsource)
{
	shaders[name] = ::makeShader(vsource, fsource);

	return false;
}

bool Gallery::loadShader(const char * name, const char * vpath, const char * fpath)
{
	if (!shaders.count(name))
	{
		shaders[name] = ::loadShader(vpath, fpath);
		if (shaders[name].handle == 0)
		{
			fprintf(stderr, "Shader %s failed to load correctly!\n", name);
			shaders.erase(name);
		}
		fprintf(stderr, "Shader %s loaded sucessfully!", name);
		return true;
	}
	else
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

const Texture & Gallery::getTexture(const char * name)
{
	return textures[name];
}



bool Gallery::loadTexture(const char *name, const char * path)
{
	if (!textures.count(name))
	{
		textures[name] = ::loadTexture(path);
		if (textures[name].handle == 0)
		{
			fprintf(stderr, "textures %s failed to load correctly!\n", name);
			textures.erase(name);
		}
		fprintf(stderr, "textures %s loaded sucessfully!", name);
		return true;
	}
	else
		return false;
}

bool Gallery::makeTexture(const char *name, unsigned  width, unsigned  height,
						unsigned  format, const unsigned char * pixels)
{
	textures[name] = ::makeTexture(width, height, format, pixels);
	return true;
}

bool Gallery::init()
{
	loadObjectOBJ("Cube", "../res/models/cube.obj");
	loadShader("Test", "../res/shaders/simpleVert.txt", "../res/shaders/simpleFrag.txt");
	return true;
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
	for each (auto texture in textures)
	{
		freeTexture(texture.second);
	}
	return false;
}

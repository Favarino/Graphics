#include "gldecs.h"
#include "glmake.h"
#include "globjects.h"
#include "Vertex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "OBJ\tiny_obj_loader.h"

#include <fstream>
#include <istream>
#include <string>
#include <algorithm>
#include "glload.h"
std::string cppStyleFileToString(const char *path)
{
	glog("loading shader", path);
	std::ifstream infile{ path };
	std::string file_contents{ std::istreambuf_iterator<char>(infile),
		std::istreambuf_iterator<char>() };
	return file_contents;
}

Shader loadShader(const char *vpath, const char *fpath, bool depth, bool add, bool face)
{
	std::string vs = cppStyleFileToString(vpath);
	std::string fs = cppStyleFileToString(fpath);

	return makeShader(vs.c_str(), fs.c_str(), depth, add, face);
}

Texture loadTexture(const char *path)
{
	glog("loading Texture", path);

	int w, h, f;
	unsigned char *p;

	Texture retval = { 0,0,0,0 };

	stbi_set_flip_vertically_on_load(true); // DirectX or OpenGL
	p = stbi_load(path, &w, &h, &f, STBI_default);

	if (!p) return retval;



	retval = makeTexture(w, h, f, p);
	stbi_image_free(p);
	return retval;
}

CubeTexture loadCubeMap(const char * path_Xpos, const char * path_Xneg, const char * path_Ypos, const char * path_Yneg, const char * path_Zpos, const char * path_Zneg)
{
	stbi_set_flip_vertically_on_load(true);
	int w[6], h[6], f[6];

	unsigned char *pixels[6];
	pixels[0] = stbi_load(path_Xpos, w + 0, h + 0, f + 0, STBI_default);
	pixels[1] = stbi_load(path_Xneg, w + 1, h + 1, f + 1, STBI_default);
	pixels[2] = stbi_load(path_Ypos, w + 2, h + 2, f + 2, STBI_default);
	pixels[3] = stbi_load(path_Yneg, w + 3, h + 3, f + 3, STBI_default);
	pixels[4] = stbi_load(path_Zpos, w + 4, h + 4, f + 4, STBI_default);
	pixels[5] = stbi_load(path_Zneg, w + 5, h + 5, f + 5, STBI_default);

	bool valid =
		std::min(w + 0, w + 6) == std::max(w + 0, w + 6) &&
		std::min(h + 0, h + 6) == std::max(h + 0, h + 6) &&
		std::min(f + 0, f + 6) == std::max(f + 0, f + 6);

	CubeTexture retval = makeCubeTexture(w[0], h[0], f[0], (const void**)pixels, false);

	for (int i = 0; i < 6; ++i) stbi_image_free(pixels[i]);

	return retval;
}
Geometry loadOBJ(const char *path)
{
	glog("TODO", "Eliminate redundant vertices.");
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);
	int vsize = shapes[0].mesh.indices.size();
	Vertex   *verts = new Vertex[vsize];
	unsigned * tris = new unsigned[vsize];

	for (int i = 0; i < vsize; ++i)
	{
		auto ind = shapes[0].mesh.indices[i];

		const float *n = &attrib.normals[ind.normal_index * 3]; // +1, +2, 0
		const float *p = &attrib.vertices[ind.vertex_index * 3]; // +1, +2, 1

		verts[i].position = glm::vec4(p[0], p[1], p[2], 1.f);
		verts[i].normal = glm::vec4(n[0], n[1], n[2], 0.f);

		if (ind.texcoord_index >= 0)
		{
			const float *t = &attrib.texcoords[ind.texcoord_index * 2]; // +1
			verts[i].texcoord = glm::vec2(t[0], t[1]);
		}

		tris[i] = i;
	}

	Geometry retval = makeGeometry(verts, vsize, tris, vsize);

	delete[] verts;
	delete[] tris;
	// then we can call makeGeometry as per normal.
	return retval;
}
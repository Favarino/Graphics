#pragma once
#include "globjects.h"
#include "vertex.h"
#include "window.h"
#include "glmake.h"
#include "glload.h"


//
//Geometry makeGeometry(const struct Vertex *verts, size_t vsize, const unsigned int *tris, size_t tsize);
//
//void freeGeometry(Geometry &geo);
//
//Geometry loadOBJ(const char *path);
//
//Shader makeShader(const char *vsource, const char *fsource);
//
//Shader loadShader(const char *vpath, const char *fpath);
//
//void freeShader(Shader &shader);
//
//void draw(const Shader &shader, const Geometry &geo);
//
//void draw(const Shader &shader, const Geometry &geo, float time);
//
//void draw(const Shader &s, const Geometry &g, const float M[16], const float V[16], const float P[16], float i);
//
//Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned char *pixels);
//
//Texture loadTexture(const char *path);
//
//void freeTexture(Texture & t);
//
//void draw(const Shader &s, const Geometry &g, const Texture &, const float M[16], const float V[16], const float P[16]);
//
//void drawPhong(const Shader & s, const Geometry & g, const float M[16], const float V[16], const float P[16]);
//
//void drawPhong(const Shader & s, const Geometry & g, const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count);
//
//Framebuffer makeFramebuffer(unsigned width, unsigned height,unsigned nColors);
//
//void freeFramebuffer(Framebuffer &);
//
//void drawFB(const Shader &s, const Geometry &g, const Framebuffer &f,
//	const float M[16], const float V[16], const float P[16],
//	const Texture *T, unsigned t_count);
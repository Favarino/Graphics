#pragma once


struct Geometry makeGeometry(const struct Vertex *verts, size_t vsize,
	const unsigned int  *tris, size_t tsize);

void freeGeometry(Geometry &);

struct Shader makeShader(const char *vsource, const char *fsource,
	bool depth = true, bool add = false, bool face = true);

void freeShader(Shader &);


struct Texture makeTexture(unsigned width, unsigned height, unsigned channels,
	const void *pixels, bool isFloat = false);


struct Texture makeTextureF(unsigned square, const float *pixels);
void freeTexture(Texture &);


struct Framebuffer makeFramebuffer(unsigned width, unsigned height, unsigned nColors, bool *isfloat = nullptr, int *channels = nullptr);
void freeFramebuffer(Framebuffer &);
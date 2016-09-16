
#pragma once

#include "globjects.h"

Shader loadShader(const char *vpath, const char *fpath);

Texture loadTexture(const char *path);

Geometry loadOBJ(const char *path);
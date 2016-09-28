
#pragma once

#include "globjects.h"

Shader loadShader(const char *vpath, const char *fpath, bool depth = true, bool add = false, bool face = true);

Texture loadTexture(const char *path);

Geometry loadOBJ(const char *path);
#pragma once
#include "globjects.h"

void clearFramebuffer(const Framebuffer &r);

namespace tdraw_internal
{
	void tdraw_begin(const Shader &s, const Geometry &g, const Framebuffer &r);
	void tdraw_close(const Shader &s, const Geometry &g, const Framebuffer &r);
}

inline void tdraw(const Shader &s, const Geometry &g, const Framebuffer &r)
{
	tdraw_internal::tdraw_begin(s, g, r);
	tdraw_internal::tdraw_close(s, g, r);
}
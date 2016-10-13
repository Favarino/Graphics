#pragma once
#include "globjects.h"
#include <cstring>

#include "GLM\fwd.hpp"
#include <cstring>

void clearFramebuffer(const Framebuffer &r);


namespace tdraw_internal
{
	void tdraw_begin(const Shader &s, const Geometry &g, const Framebuffer &r);
	void tdraw_close(const Shader &s, const Geometry &g, const Framebuffer &r);

	void tdraw_format(size_t &idx, size_t &tex, const glm::mat4 &val);
	void tdraw_format(size_t &idx, size_t &tex, const glm::vec4 &val);
	void tdraw_format(size_t &idx, size_t &tex, const glm::vec3 &val);
	void tdraw_format(size_t &idx, size_t &tex, int   val);
	void tdraw_format(size_t &idx, size_t &tex, float val);
	void tdraw_format(size_t &idx, size_t &tex, const Texture &val);
	void tdraw_format(size_t &idx, size_t &tex, const CubeTexture &val);
	void tdraw_format(size_t &idx, size_t &tex, const Framebuffer &val);


	//= std::enable_if_t<!std::is_pointer<T>::value, T>
	template<typename T, typename ...U>
	void tdraw_unpack(size_t idx, size_t tex, const T &val, U &&...uniforms)
	{
		tdraw_format(idx, tex, val);
		tdraw_unpack(idx, tex, uniforms...);
	}

	//template<typename T, typename ...U>
	//void tdraw_unpack(size_t idx, size_t tex, const T *val, int size, U &&...uniforms)
	//{
	//	//tdraw_format(idx, tex, val, size);
	//	tdraw_unpack(idx, tex, uniforms...);
	//}

	template<typename T>
	void tdraw_unpack(size_t idx, size_t tex, const T &val)
	{
		tdraw_format(idx, tex, val);
	}
}

template<typename ...U>
void tdraw(const Shader &s, const Geometry &g, const Framebuffer &r, U ... uniforms)
{
	tdraw_internal::tdraw_begin(s, g, r);

	tdraw_internal::tdraw_unpack(0, 0, uniforms...);

	tdraw_internal::tdraw_close(s, g, r);
}



inline void tdraw(const Shader &s, const Geometry &g, const Framebuffer &r)
{
	tdraw_internal::tdraw_begin(s, g, r);
	tdraw_internal::tdraw_close(s, g, r);
}
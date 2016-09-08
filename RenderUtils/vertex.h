#pragma once
//#include <GLM\glm.hpp>

#include "../dep/include/GLM/glm.hpp"

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec4 normal;
};

enum 
{
	POSITION = 0,
	COLOR = 16,
	TEXCOORD = 32
};
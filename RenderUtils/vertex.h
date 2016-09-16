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
	TEXCOORD = 32,
	NORM = 40
};

//const Vertex  quad_verts[4] = { { { -1,-1,0,1 },{1,1,1,0},{ 0,0,1,0 },{ 0,0 } },
//							  {   { 1,-1, 0,1 },{1,1,0,1},{ 0,0,1,0 },{ 1,0 } },
//							  {   { 1, 1, 0,1 },{1,0,1,1},{ 0,0,1,0 },{ 1,1 } },
//							  {   { -1, 1,0,1 },{0,1,1,1},{ 0,0,1,0 },{ 0,1 } } };

unsigned quad_tris[6] = { 0,1,2,2,3,0 };
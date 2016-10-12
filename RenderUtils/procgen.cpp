#include "procgen.h"
#include <GLM\glm.hpp>
#include "vertex.h"
#define GLEW_STATIC
#include "GLEW\glew.h"
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>


Geometry generateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);

			aoVertices[r * cols + c].texcoord = glm::vec2(c / (float)(cols - 1), r / (float)(rows - 1));

			glm::vec3 color = glm::vec3(sinf(c / (float)(cols - 1))*(r / (float)(rows - 1)));

			aoVertices[r * cols + c].color = glm::vec4(color, 1);

			aoVertices[r*cols + c].normal = glm::vec4(0, 1, 0, 0);
		}
	}
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;

	for (unsigned r = 0; r < (rows - 1); r++)
	{
		for (unsigned int c = 0; c < (cols - 1); c++)
		{
			//triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//triangle 2
			auiIndices[index++] = r*cols + c;
			auiIndices[index++] = (r + 1)*cols + (c + 1);
			auiIndices[index++] = r* cols + (c + 1);
		}
	}
	Geometry retval = makeGeometry(aoVertices, rows*cols, auiIndices, (rows - 1) * (cols - 1) * 6);
	delete[] aoVertices;
	return retval;
}

Texture Noise(unsigned dims, unsigned octaves)
{
	float *perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{
			float amplitude = 1.f;
			float persistence = 0.25f;
			perlin_data[y*dims + x] = 0;

			for (int o = 0; o < octaves; o++)
			{
				float freq = powf(2,o);
				float perlin_sample = glm::perlin(glm::vec2((float)x, (float)y)*scale*freq)*0.5f + 0.5f;
				perlin_data[y* dims + x] += perlin_sample*amplitude;
				amplitude *= persistence;
			}
		}
	}

	Texture retval;
	retval.height = 64;
	retval.width = 64;
	retval.format = GL_RED;
	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return retval;
}
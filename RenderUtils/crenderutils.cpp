#pragma region Includes
#define GLEW_STATIC
#include "GLEW\glew.h"
#include "vertex.h"
#include "crenderutils.h"
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"
#include <iostream>
#include <fstream>
#include "globjects.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb_image.h>
#include <random>
#include "gldecs.h"
using namespace std;
#pragma endregion

char* fileToArray(const char *path)
{
	int array_size = 5012;
	char * array = new char[array_size];
	int position = 0;

	ifstream fin(path);

	if (fin.is_open())
	{
		cout << "File opened!" << endl;

		while (!fin.eof() && position < array_size)
		{
			fin.get(array[position]);
			position++;
		}
		array[position - 1] = '\0';

		cout << "Displaying array..." << endl << endl;

		for (int i = 0; array[i] != '\0'; i++)
		{
			cout << array[i];
		}
	}
	else
	{
		cout << "File could not be opened." << endl; 
	}
	return array;

}

#pragma region MakeFunctions

Geometry makeGeometry(const Vertex * verts, size_t vsize, const unsigned int * tris, size_t tsize)
{
	Geometry returnVal;

	//define the vaiables
	glGenBuffers(1, &returnVal.vbo);      //store my vertices
	glGenBuffers(1, &returnVal.ibo);      //store indeices
	glGenVertexArrays(1, &returnVal.vao); //store attribute information
	
    //scope the variables
	glBindVertexArray(returnVal.vao);
	glBindBuffer(GL_ARRAY_BUFFER, returnVal.vbo); //scope our vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnVal.ibo); //triangle is scoped

	//intialize the variable
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

	//attributes let us tell opengl how the memory is laid out
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//index of the attribute, number of elements,type,normalized?,size of vertex, offset
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)COLOR);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)TEXCOORD);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)NORM);
	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)32);
	


	/*
		struct Vertex // 32 bytes
		{
			float position[4]; //4, 4, 4, 4
			float color[4];    //4, 4, 4, 4, starts on byte 16
	
		};

		Vertex myVert;

		&myVert + 16
	
	*/



	//unscope the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	returnVal.size = tsize;
	return returnVal;
}

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader retval;
	// create our variables
	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	// initialize our variables
	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);
	// compile the shaders
	glog_glCompileShader(vs);
	glog_glCompileShader(fs);
	// link the shaders into a single program
	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);
	glLinkProgram(retval.handle);

	GLint program_linked;
	glGetProgramiv(retval.handle, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(retval.handle, 1024, &log_length, message);
		// Write the error to a log

		fprintf(stderr, "%s", message);
	}

	// no longer need these! Their functionality has been eaten by the program.
	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned char *pixels)
{
	Texture retval = { 0,width,height,format };
	
	glGenTextures(1, &retval.handle);				//Declaration
	glBindTexture(GL_TEXTURE_2D, retval.handle);	//Scoping

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	

	return retval;
}
#pragma endregion

#pragma region FreeFunctions
void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };

}

void freeShader(Shader & shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0,0,0,0 };
}
#pragma endregion

#pragma region DrawFunctions
void draw(const Shader &shader, const Geometry &geo)
{
	glUseProgram(shader.handle);

	//binding the vao also binds the ibo(tri) and vbo(verts)
	glBindVertexArray(geo.vao);

	//draw elements will draw the vertices that are currently bound
	//using an array of indices
	//if an ibo is bount, we dont need to provide and indices
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader &shader, const Geometry &geo, float time)
{
	glUseProgram(shader.handle);
	glBindVertexArray(geo.vao);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int loc = glGetUniformLocation(shader.handle, "time");

	glUniform1f(loc, time);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader & s, const Geometry & g, const float M[16], const float V[16], const float P[16], float i)
{
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);
	glUniform1f(3, i);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader & s, const Geometry & g, const Texture &t, const float M[16], const float V[16], const float P[16])
{
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.handle);
	int loc = glGetUniformLocation(s.handle, "texMap");
	glUniform1i(loc, 0);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

void drawPhong(const Shader & s, const Geometry & g, const float M[16], const float V[16], const float P[16])
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);


	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}
void drawPhong(const Shader & s, const Geometry & g,
	const float M[16], const float V[16], const float P[16], const Texture *T, unsigned t_count)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	for (int i = 0; i < t_count; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, T[i].handle);
		glUniform1i(3 + i, i);
	}

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}
#pragma endregion

#pragma region LoadFunctions
Shader loadShader(const char * vpath, const char * fpath)
{
	char *Vsource = fileToArray(vpath);
	char *Fsource = fileToArray(fpath);

	return makeShader(Vsource, Fsource);
}

Geometry loadOBJ(const char * path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
std::string err;

bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

int vsize = shapes[0].mesh.indices.size();

Vertex   *verts = new Vertex[vsize];
unsigned *tris = new unsigned[vsize];

for (int i = 0; i < vsize; i++)
{
	auto ind = shapes[0].mesh.indices[i];

	const float *n = &attrib.normals[ind.normal_index * 3];
	const float *p = &attrib.vertices[ind.vertex_index * 3];
	const float *t = &attrib.texcoords[ind.texcoord_index * 2];

	verts[i].position = glm::vec4(p[0], p[1], p[2], 1.f);
	verts[i].normal = glm::vec4(n[0], n[1], n[2], 0.f);
	//verts[i].texcoord = glm::vec2(t[0],t[1]);

	if (ind.texcoord_index >= 0)
	{
		const float *t = &attrib.texcoords[ind.texcoord_index * 2];
		verts[i].texcoord = glm::vec2(t[0],t[1]);
	}

	tris[i] = i;
}

Geometry retval = makeGeometry(verts, vsize,
	tris, vsize);

delete[] verts;
delete[] tris;
// then we can call makeGeometry as per normal.
return retval;
}

Texture loadTexture(const char * path)
{
	glog("loading Texture", "path");
	int w, h, f;
	unsigned char *p;

	Texture retval = { 0,0,0,0 };

	stbi_set_flip_vertically_on_load(true);

	p = stbi_load(path, &w, &h, &f, STBI_default);

	if (!p) return retval;

	switch (f)
	{
	case STBI_grey: f = GL_RED;  break;
	case STBI_grey_alpha: f = GL_RG;   break;
	case STBI_rgb: f = GL_RGB;  break;
	case STBI_rgb_alpha: f = GL_RGBA; break;
	}

	retval = makeTexture(w, h, f, p);
	stbi_image_free(p);
	return retval;
}
#pragma endregion
Framebuffer makeFramebuffer(unsigned width, unsigned height, unsigned nColors)
{
	Framebuffer retval = { 0,width,height,0,0,0,0,0,0,0,0 };

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	const GLenum attachments[8] =
	{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	for (int i = 0; i < nColors && i < 8; ++i)
	{
		retval.colors[i] = makeTexture(width, height, GL_RGBA, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, attachments[i],
			retval.colors[i].handle, 0);
	}
	glDrawBuffers(nColors, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}




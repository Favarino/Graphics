#define GLEW_STATIC

#include "GLEW\glew.h"
#include "vertex.h"
#include "crenderutils.h"


Geometry makeGeometry(const Vertex * verts, size_t vsize, const unsigned int * tris, size_t tsize)
{
	Geometry returnVal;

	//define the vaiables
	glCreateBuffers(1, &returnVal.vbo);      //store my vertices
	glCreateBuffers(1, &returnVal.ibo);      //store indeices
	glCreateVertexArrays(1, &returnVal.vao); //store attribute information
	
    //scope the variables
	glBindVertexArray(returnVal.vao);
	glBindBuffer(GL_ARRAY_BUFFER, returnVal.vbo); //scope our vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnVal.ibo); //triangle is scoped

	//intialize the variables
	new float[4];
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

	//attributes let us tell opengl how the memory is laid out
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//index of the attribute, number of elements,type,normalized?,size of vertex, offset
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)16);

	//unscope the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	returnVal.size = tsize;
	return returnVal;
}

void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };

}

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader returnVal;

	//create variables
	returnVal.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	//intialize variables
	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);

	//compile shaders
	glCompileShader(vs);
	glCompileShader(fs);

	//linkshaders into a single program
	glAttachShader(returnVal.handle, vs);
	glAttachShader(returnVal.handle, fs);
	glLinkProgram(returnVal.handle);

	//no longer need these! their functionality has been eaten by the program
	glDeleteShader(vs);
	glDeleteShader(fs);

	return returnVal;
}

void freeShader(Shader & shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

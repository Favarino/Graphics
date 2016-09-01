#define GLEW_STATIC

#include "GLEW\glew.h"
#include "vertex.h"
#include "crenderutils.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"
#include <iostream>
#include <fstream>
using namespace std;


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
	//glEnableVertexAttribArray(2);

	//index of the attribute, number of elements,type,normalized?,size of vertex, offset
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)16);
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

void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };

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
	glCompileShader(vs);
	glCompileShader(fs);
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

void freeShader(Shader & shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

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

void draw(const Shader & s, const Geometry & g, const float M[16], const float V[16], const float P[16])
{
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}



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

Shader loadShader(const char * vpath, const char * fpath)
{
	char *Vsource = fileToArray(vpath);
	char *Fsource = fileToArray(fpath);

	return makeShader(Vsource, Fsource);
}
#include <random>
Geometry loadOBJ(const char * path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	Vertex   *verts = new Vertex[attrib.vertices.size()/3];
	unsigned * tris = new unsigned[shapes[0].mesh.indices.size()];

	for (int i = 0; i < attrib.vertices.size()/3; ++i)
	{
		verts[i] = { attrib.vertices[i*3],
					 attrib.vertices[i*3+1],
					 attrib.vertices[i*3+2], 1};
		/*verts[i].color[0] = rand() * 1.0f / RAND_MAX;
		verts[i].color[1] = rand() * 1.0f / RAND_MAX;
		verts[i].color[2] = rand() * 1.0f / RAND_MAX;
		verts[i].color[3] = 1;*/
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); ++i)
		tris[i] = shapes[0].mesh.indices[i].vertex_index;

	Geometry retval = makeGeometry(verts, attrib.vertices.size() / 3,
								    tris, shapes[0].mesh.indices.size());

	delete[] verts;
	delete[] tris;
	// then we can call makeGeometry as per normal.
	return retval;
}
#define GLEW_STATIC

#include "GLEW\glew.h"
#include "vertex.h"
#include "crenderutils.h"


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

#include <iostream>
#include <fstream>
using namespace std;

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

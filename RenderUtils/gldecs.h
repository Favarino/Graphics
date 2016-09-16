#pragma once
#define GLEW_STATIC
#include "GLEW\glew.h"
#include "GLFW\glfw3.g"

#ifdef _DEBUG
#include <iostream>
#define glog(detail,extra)\
do\
{\
	std::cout << "In "<<__FILE<<" at "<<__FUNCTION__<<" on line " <<": " << detail << ", " <<extra <<std::endl; \
}while(0)

#define glog_glCompileShader(shader)
do\
{\
glCompileShader(shader); \
GLint success = GL_FALSE; \
glGetShaderiv(shader, GL_COMPILE_STATUS, &success); \
if (success == GL_FALSE)\
{\
int length = 0; \
glGetShaderiv(shader, GL_INFO_LOG_LENGTH); \
char *log = (char*)malloc(length); \
char *logd = new char[length]; \
glGetShaderInfoLog(shader, length, 0, log); \
glog("Shader failed to compile\n", log); \
free(log);\
}\
}while(0)\

#else
#define glog(detail,extra)
#define glog_glCompileShader(shader) glCompileShader(shader)
#endif
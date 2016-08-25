#define GLEW_STATIC

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "window.h"
#include <cstring>

bool Window::init(int a_width, int a_height, char * a_title)
{
	width = a_width;
	height = a_width;
	strcpy_s(title, 64, a_title);

	glfwInit();
	winHandle = glfwCreateWindow(a_width, a_height, a_title, nullptr, nullptr);

	glfwMakeContextCurrent(winHandle);

	isInitialized = true;
	glewExperimental = true;
	glewInit();
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	return true;
}

bool Window::step()

{
	if (!isInitialized) return false;

	glfwPollEvents();
	glfwSwapBuffers(winHandle);

	glClear(GL_COLOR_BUFFER_BIT);

	return !glfwWindowShouldClose(winHandle);
}

bool Window::term()
{
	glfwDestroyWindow(winHandle);
	glfwTerminate();
	winHandle = nullptr;
	isInitialized = false;
	return false;
}


#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>
int main()
{
	Window window;
	window.init();

	Gallery gallery;
	gallery.loadShader("CAMERA","../res/shaders/cameraVert.txt","../res/shaders/cameraFrag.txt");

	gallery.loadObjectOBJ("Cube", "../res/models/cube.obj");

	float IDENTITY[16] = {1,0,0,0,
						  0,1,0,0,
						  0,0,1,0,
						  0,0,0,1};

	glm::mat4 proj, view, model;

	proj = glm::perspective(45.f, 1.f, 0.1f, 100.f);

	view  = glm::lookAt(glm::vec3(5.f,5.f,5.f),
						glm::vec3(0.f,0.f,0.f),
						glm::vec3(0.f,1.f,0.f));
	float time = 0;
	while (window.step())
	{
		time += 0.01666f;

		view = glm::lookAt(glm::vec3(5.f, cosf(time) * 10, 5.f),
						   glm::vec3(0.f, 0.f, 0.f),
						   glm::vec3(0.f, 1.f, 0.f));

		model = glm::rotate(time, glm::vec3(0, 1, 0));

		draw(gallery.getShader("CAMERA"), gallery.getObject("Cube"),
			glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(proj));

		draw(gallery.getShader("CAMERA"), gallery.getObject("Cube"),
			glm::value_ptr(model * glm::translate(glm::vec3(1,1,1))), glm::value_ptr(view), glm::value_ptr(proj));
	}

	gallery.term();
	window.term();

	return 0;
}
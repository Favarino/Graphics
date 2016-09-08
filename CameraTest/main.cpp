
#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>
#include "timer.h"
#include "input.h"
#include "camera.h"
#include "globjects.h"
#include "procgen.h"
int main()
{
	Window window;
	Timer time;
	Input input;
	Gallery gallery;
	FlyCamera cam;

	window.init();
	input.init(window);
	time.init();

	glm::mat4 view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f),
								 glm::vec3(0.f, 0.f, 0.f), 
								 glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 proj = glm::perspective(45.f, 16/9.f, 1.f, 100.f);
	glm::mat4 model;

	Geometry geo = loadOBJ("../res/models/cube.obj");

	Shader shader = loadShader("../res/shaders/phongVert.txt", 
							   "../res/shaders/phongFrag.txt");

	while (window.step())
	{
		drawPhong(shader, geo, glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(proj));
	}

	gallery.term();
	time.term();
	input.term();
	window.term();

	return 0;
}

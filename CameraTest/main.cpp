
#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>
#include "timer.h"
#include "input.h"
#include "camera.h"
int main()
{
	Window window;
	Timer time;
	Input input;
	window.init();
	input.init(window);
	time.init();
	Gallery gallery;
	FlyCamera cam;

	cam.jumpTo(glm::vec3(5, 5, 5));
	cam.lookAt(glm::vec3(0, 0, 0));

	gallery.loadShader("CAMERA", "../res/shaders/cameraVert.txt", "../res/shaders/cameraFrag.txt");

	gallery.loadObjectOBJ("Cube", "../res/models/cube.obj");

	glm::mat4 proj, view, model;

	int i;

	model = glm::mat4();

	while (window.step())
	{
		time.step();
		input.step();
		float tTime = time.getTotalTime();

		view = cam.getView();
		proj = cam.getProjection();
		if (input.getKeyState('T') == Input::DOWN) {
			i = 1;
		}
		else {
			i = 0;
		}

		cam.update(input, time);

		draw(gallery.getShader("CAMERA"), gallery.getObject("Cube"),
			glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(proj),i);
	}

	gallery.term();
	time.term();
	input.term();
	window.term();

	return 0;
}
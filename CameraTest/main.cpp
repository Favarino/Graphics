
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

	glm::mat4 view = glm::lookAt(glm::vec3(2.f, 1.2f, 2.f), //eye
								 glm::vec3(0.f, 1.2f, 0.f), //center
								 glm::vec3(0.f, 1.f, 0.f));//up

	glm::mat4 proj = glm::perspective(45.f, 16/9.f, 1.f, 100.f);
	glm::mat4 model;

	Geometry cube = loadOBJ("../res/models/cube.obj");

	Geometry sphere = loadOBJ("../res/models/sphere.obj");

	Geometry wheezer = loadOBJ("../res/models/mrwheezer.obj");

	Geometry soulspear = loadOBJ("../res/models/soulspear.obj");

	Shader shader = loadShader("../res/shaders/phongVert.vert", 
							   "../res/shaders/phongFrag.frag");

	Texture tarray[] = { loadTexture("../res/textures/soulspear_diffuse.tga"),
		loadTexture("../res/textures/soulspear_specular.tga"),
		loadTexture("../res/textures/soulspear_normal.tga") };

	cam.jumpTo(glm::vec3(0, 0, -10));
	cam.lookAt(glm::vec3(0, 0, 0));

	float timer = 0;
	while (window.step())
	{
		timer += 0.016f;
		
		input.step();
		time.step();
		cam.update(input, time);

		view = cam.getView();
		proj = cam.getProjection();

		glm::mat4 modelC = glm::rotate(timer, glm::normalize(glm::vec3(0, 1, 0)));
		glm::mat4 modelS = glm::translate(glm::vec3(0, cos(timer) * 6, 0));

		drawPhong(shader, soulspear, glm::value_ptr(modelC), glm::value_ptr(view), glm::value_ptr(proj),tarray,1);
		drawPhong(shader, sphere, glm::value_ptr(modelS), glm::value_ptr(view), glm::value_ptr(proj),tarray,1);
	}

	gallery.term();
	time.term();
	input.term();
	window.term();

	return 0;
}

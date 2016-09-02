
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

	unsigned char pixels[] = {255,255,0};
	//Texture tex = makeTexture(1, 1, 0x1907, pixels);

	gallery.loadTexture("Face","../res/textures/face_tex.png");

	Vertex verts[] = { {-1,-1,0,1}, {-1,1,0,1},
					   {1,1,0,1},{1,-1,0,1} };

	unsigned tris[] = { 0,1,2,2,3,0 };

	gallery.makeObject("Quad", verts, 4, tris, 6);
	

	cam.jumpTo(glm::vec3(5, 5, 5));
	cam.lookAt(glm::vec3(0, 0, 0));

	gallery.loadShader("TEXTURE", "../res/shaders/textureVert.txt", "../res/shaders/textureFrag.txt");

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

		draw(gallery.getShader("TEXTURE"),gallery.getObject("Quad"),gallery.getTexture("Face"),
			glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(proj), i);

		//draw(gallery.getShader("TEXTURE"), gallery.getObject("Cube"),tex,
			//glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(proj),i);
	}

	gallery.term();
	time.term();
	input.term();
	window.term();

	return 0;
}
#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include "gldraw.h"
#include "GLM\ext.hpp"
int main()
{
	Window window;
	window.init(1280, 720);



	Geometry spear = loadOBJ("../res/models/soulspear.obj");

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	Shader toon = loadShader("../res/shaders/toon.vert", "../res/shaders/toon.frag");



	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	Framebuffer screen{ 0, 1280, 720 };

	float timer = 0;

	while (window.step())
	{
		timer += 0.016f;

		model = glm::rotate(timer, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, 0, 0));
		
		tdraw(toon, spear, screen,model);

	}
	window.term();

	return 0;
}
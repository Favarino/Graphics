#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include "gldraw.h"
#include "GLM\ext.hpp"
int main()
{
	Window window;
	window.init(1280,720);



	Geometry spear = loadOBJ("../res/models/soulspear.obj");

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	Shader post = loadShader("../res/shaders/quad.vert","../res/shaders/quad.frag");

	Shader gpass = loadShader("../res/shaders/gpass.vert", "../res/shaders/gpass.frag");

	Shader lpass = loadShader("../res/shaders/lpass.vert", "../res/shaders/lpass.frag");

	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	Framebuffer screen{ 0, 1280, 720 };
	Framebuffer gframe = makeFramebuffer(1280, 720, 4);
	Framebuffer lframe = makeFramebuffer(1280, 720, 2);

	float timer = 0;

	while (window.step())
	{
		timer += 0.016f;
		clearFramebuffer(gframe);
		clearFramebuffer(lframe);

		model = glm::rotate(timer, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));
		//geometry pass
		tdraw(gpass, spear, gframe, model, view, proj,
			spear_diffuse, spear_normal, spear_specular);
		tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], gframe.colors[1],
			gframe.colors[2], gframe.colors[3],
			gframe.depth);

		// Debug Rendering Stuff.
		for (int i = 0; i < 4; ++i)
		{
			glm::mat4 mod =
				glm::translate(glm::vec3(-.75f + .5*i, 0.75f, 0)) *
				glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			tdraw(post, quad, screen, gframe.colors[i], mod);
		}

		glm::mat4 mod =
			glm::translate(glm::vec3(-.75f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, gframe.depth, mod);

		mod =
			glm::translate(glm::vec3(-.25f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, lframe.colors[0], mod);

		mod =
			glm::translate(glm::vec3(.25f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, lframe.colors[1], mod);

	}

	window.term();

	return 0;
}
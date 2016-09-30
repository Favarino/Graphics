#include "crenderutils.h"


#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.init(1280, 720);

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadOBJ("../res/models/soulspear.obj");
	Geometry sphere = loadOBJ("../res/models/sphere.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = makeTexture(1, 1, 4, white_pixels);

	Shader qdraw = loadShader("../res/shaders/quad.vert", "../res/shaders/quad.frag", false);
	Shader blur = loadShader("../res/shaders/post.vert", "../res/shaders/post.frag", false);

	Shader gpass = loadShader("../res/shaders/gpass.vert", "../res/shaders/gpass.frag");
	Shader lpass = loadShader("../res/shaders/lpass.vert", "../res/shaders/lpass.frag", false, true);
	Shader toon = loadShader("../res/shaders/toon.vert", "../res/shaders/toon.frag");

	/////////////////////////////////////////
	//////// Shadowy Shading Shaders
	Shader spass = loadShader("../res/shaders/spass.vert", "../res/shaders/spass.frag", true, false, false);
	Shader lspass = loadShader("../res/shaders/lspass.vert", "../res/shaders/lspass.frag", false, true);


	Framebuffer screen = { 0, 1280, 720 };

	bool flTex[] = { false, true, false, true };
	Framebuffer gframe = makeFramebuffer(1280, 720, 4, flTex);
	Framebuffer lframe = makeFramebuffer(1280, 720, 3);
	Framebuffer nframe = makeFramebuffer(1280, 720, 1); // for blurring.

														
	Framebuffer sframe = makeFramebuffer(256, 256, 0); //change shadow resolution here

	// Camera information
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 camProj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	// Model Matrices
	glm::mat4 spearModel; // ROTATES in main
	glm::mat4 sphereModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));
	glm::mat4 wallModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) * glm::translate(glm::vec3(0, 0, -2)) * glm::scale(glm::vec3(2, 2, 1));

	// Light Matrices and data
	glm::mat4 lightProj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);

	glm::mat4 redView = glm::lookAt(glm::normalize(-glm::vec3(1, -1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 redColor = glm::vec4(1, 0, 0, 1);

	glm::mat4 greenView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 greenColor = glm::vec4(0, 1, 0, 1);

	glm::mat4  blueView = glm::lookAt(glm::normalize(-glm::vec3(-1, -1, 1)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 blueColor = glm::vec4(0, 0, 1, 1);

	float time = 0;

	while (context.step())
	{
		time += 0.016f;
		spearModel = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));


		/////////////////////////////////////////////////////
		// Geometry Pass
		//
		clearFramebuffer(gframe);
		tdraw(gpass, spear, gframe, spearModel, camView, camProj, spear_diffuse, spear_normal, spear_specular);
		tdraw(gpass, sphere, gframe, sphereModel, camView, camProj, white, vertex_normals, white);
		tdraw(gpass, quad, gframe, wallModel, camView, camProj, white, vertex_normals, white);

		//tdraw(blur, quad, nframe, gframe.colors[1]);

		/////////////////////////////////////////////////////
		//// Light pass!
		clearFramebuffer(lframe);

		//////////////////////////
		// RED LIGHT

		// Shadow PrePass
		clearFramebuffer(sframe);
		tdraw(spass, spear, sframe, spearModel, redView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, redView, lightProj);
		tdraw(spass, quad, sframe, wallModel, redView, lightProj);
		// Light Aggregation
		tdraw(lspass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, redColor, redView, lightProj);

		//////////////////////////
		// Green light!

		// Reuse the shadow pass!
		clearFramebuffer(sframe);
		tdraw(spass, spear, sframe, spearModel, greenView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, greenView, lightProj);
		tdraw(spass, quad, sframe, wallModel, greenView, lightProj);
		// add the green light now.
		tdraw(lspass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, greenColor, greenView, lightProj);



		//////////////////////////////////////////////////
		// Debug Rendering Stuff. Just single textures to quads-
		// drawing most of the images I've gathered so far.

		// note that the sframe (shadow pass) will only be from the most recent light.
		Texture debug_list[] = { gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			gframe.depth, lframe.colors[1], lframe.colors[2], sframe.depth };

		for (int i = 0; i < sizeof(debug_list) / sizeof(Texture); ++i)
		{
			glm::mat4 mod = glm::translate(glm::vec3(-.75f + .5f*(i % 4), 0.75f - .5f*(i / 4), 0))
				* glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			tdraw(qdraw, quad, screen, debug_list[i], mod);
		}

		glm::mat4 mod =
			glm::translate(glm::vec3(-.5f, -0.5f, 0)) *
			glm::scale(glm::vec3(0.5f, 0.5f, 1.f));
		tdraw(qdraw, quad, screen, lframe.colors[0], mod);

	}

	context.term();
}
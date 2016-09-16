#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
#include "gldraw.h"
int main()
{
	Window window;
	window.init(1280,720);

	

	Framebuffer screen {0, 1280, 720 };

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);

	Shader simple = loadShader("../res/shaders/simpleVert.txt","../res/shaders/simpleFrag.txt");

	while (window.step())
	{
		tdraw(simple, quad, screen);
	}


	window.term();

	return 0;
}
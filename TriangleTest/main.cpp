#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
#include "Gallery.h"
int main()
{
	Window window;
	window.init();

	Gallery gallery;
	gallery.init();

	Vertex vert[3] = { {0.0f,.5f,0,1,1,0,1,1},
					   {.5f,-.5f,0,1,0,1,1,1},
					   {-.5f,-.5f,0,1,0,0,1,1} };
	

	unsigned tris[3] = { 0,1,2 };
							
	gallery.loadObjectOBJ("Sphere","../res/models/sphere.obj");
	gallery.loadShader("Test", "../res/shaders/simpleVert.txt", "../res/shaders/simpleFrag.txt");
	
	while (window.step())
	{
		draw(gallery.getShader("Test"), gallery.getObject("Sphere"));
		
	}

	gallery.term();
	window.term();

	return 0;
}
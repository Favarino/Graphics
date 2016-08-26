#include "window.h"
#include "crenderutils.h"
#include "vertex.h"
int main()
{
	Window window;
	window.init();

	Vertex vert[3] = { {0.0f,.5f,0,1,1,0,0,0},
					   {.5f,-.5f,0,1,0,1,0,0},
					   {-.5f,-.5f,0,1,0,0,1,0} };
	

	unsigned tris[3] = { 0,1,2 };
							

	Geometry geometry1 = makeGeometry(vert, 3, tris, 3);
	Geometry geometry = loadOBJ("../res/modles/cube.obj");
	//Shader shader1 = makeShader(vsource, fsource);
	Shader shader = loadShader("../res/shaders/simpleVert.txt",
								"../res/shaders/simpleFrag.txt");

	while (window.step())
	{
		draw(shader, geometry);
		
	}

	freeGeometry(geometry);
	freeShader(shader);
	window.term();

	return 0;
}
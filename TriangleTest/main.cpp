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

	float time = 0;

	while (window.step())
	{
		time += 0.016667f;
		draw(gallery.getShader("Test"), gallery.getObject("Sphere"),time);		
	}

	gallery.term();
	window.term();

	return 0;
}
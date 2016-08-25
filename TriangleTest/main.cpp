#include "window.h"

int main()
{
	Window window;
	window.init();
	while (window.step());

	window.term();
	//shit bruh
	return 0;
}
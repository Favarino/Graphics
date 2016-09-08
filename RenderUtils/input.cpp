#include <GLFW\glfw3.h>
#include "window.h"
#include "input.h"
#include <cstring>


bool Input::init(const Window &window)
{
	//if (!window.isInitialized) return false;
	memset(keys, 0, 350 * sizeof(KEY_STATE));
	winHandle = window.winHandle;

	glfwSetInputMode(winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mouseX = 0;
	mouseY = 0;
	mouseHorz = 0;
	mouseVert = 0;
	
	return true;
}

bool Input::step()
{
	for (int i = 0; i < 350; i++)
	{
		int res = glfwGetKey(winHandle, i);

		if ((keys[i] == UP || keys[i] == RELEASE) && res == GLFW_PRESS)
		{
			keys[i] = PRESS;
		}
		else if ((keys[i] == DOWN || keys[i] == PRESS) && res == GLFW_RELEASE)
		{
			keys[i] = RELEASE;
		}
		else if (keys[i] == PRESS)
		{
			keys[i] = DOWN;
		}
		else if (keys[i] == RELEASE)
		{
			keys[i] = UP;
		}

		//keys[i] = res == GLFW_RELEASE? UP : DOWN;
	}
	int w, h;
	glfwGetWindowSize(winHandle, &w, &h);
	double prevX = mouseX;
	double prevY = mouseY;
	glfwGetCursorPos(winHandle, &mouseX, &mouseY);

	mouseHorz = mouseX - prevX;
	mouseVert = mouseY - prevY;

	return true;
}

bool Input::term()
{
	return true;
}
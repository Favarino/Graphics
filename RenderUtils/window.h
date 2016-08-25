#pragma once


class Window
{
private:
	int width, height;
	char title[64];
	bool isInitialized = false;

	//forward declaration (telling computer this type exists just not in this file)
	class GLFWwindow *winHandle;

public:
	// a_ stands for arguement
	bool init(int a_width = 800, int a_height = 600, char*a_title= "Window"); //Start
	bool step();															 //Update
	bool term();															//Terminate
};
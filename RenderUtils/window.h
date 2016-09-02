#pragma once


class Window
{
private:
	friend class Input;

	int width, height;
	char title[64];
	bool isInitialized = false;

	//forward declaration (telling computer this type exists just not in this file)
	struct GLFWwindow *winHandle;

public:
	// a_ stands for arguement
	bool init(int a_width = 1280, int a_height = 720, char*a_title= "Window"); //Start
	bool step();															 //Update
	bool term();															//Terminate
};
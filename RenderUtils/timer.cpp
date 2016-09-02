#include <GLFW\glfw3.h>
#include "timer.h"

bool Timer::init()
{
	glfwSetTime(0.0);
	prevTime = 0;
	currTime = 0;
	deltaTime = 0;
	return true;
}

bool Timer::step()
{
	prevTime = currTime;
	currTime = getRealTime();
	deltaTime = currTime - prevTime;
	return true;
}

bool Timer::term(){ return true; }

float Timer::getDeltaTime() const{ return deltaTime; }

float Timer::getTotalTime() const { return currTime; }

float Timer::getRealTime() const { return glfwGetTime(); }


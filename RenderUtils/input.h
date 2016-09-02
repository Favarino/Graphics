#pragma once


class Input
{
public:
	enum KEY_STATE { UP, DOWN, PRESS, RELEASE };

private:
	KEY_STATE keys[350];
	struct GLFWwindow *winHandle;

	double mouseX, mouseY;
	float mouseHorz, mouseVert;
	

public:
	bool init(const class Window&);
	bool step();
	bool term();

	KEY_STATE getKeyState(int key) const
	{
		return keys[key];
	}

	float getMouseAxisHorizontal()const { return mouseHorz; }
	float getMouseAxisVertical()const { return mouseVert; }


};
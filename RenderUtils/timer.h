#pragma once

class Timer
{
	float currTime;
	float prevTime;
	float deltaTime;
public:
	bool init();
	bool step();
	bool term();

	//fixed to start of frame
	float getDeltaTime() const;
	//fixed to start of frame
	float getTotalTime() const;

	//return the real passage of time
	float getRealTime() const;
};
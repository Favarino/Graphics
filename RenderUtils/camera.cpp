#include "camera.h"
#include "timer.h"
#include "input.h"

void FlyCamera::update(const Input &in, const Timer &t)
{	
	yaw -= in.getMouseAxisHorizontal() * t.getDeltaTime() * .1f;
	pitch += in.getMouseAxisVertical() * t.getDeltaTime() * .1f;

	pitch = glm::clamp(pitch, -glm::pi<float>() / 2 + 0.0001f,
							   glm::pi<float>() / 2 - 0.0001f);

	glm::mat4 tbn = glm::eulerAngleYXZ(yaw, pitch, roll);

	glm::vec3 right = glm::vec3(tbn[0]);
	glm::vec3 up = glm::vec3(tbn[1]);
	direction = glm::vec3(tbn[2]);

	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	if (in.getKeyState('W') == Input::DOWN) moveDir += direction;
	if (in.getKeyState('S') == Input::DOWN) moveDir -= direction;
	if (in.getKeyState('A') == Input::DOWN) moveDir += right;
	if (in.getKeyState('D') == Input::DOWN) moveDir -= right;
	if (in.getKeyState('Q') == Input::DOWN) moveDir += up;
	if (in.getKeyState('E') == Input::DOWN) moveDir -= up;

	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.getDeltaTime() * speed;
	}
}

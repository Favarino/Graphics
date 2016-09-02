#pragma once
#define GLM_SWIZZLE
#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class FlyCamera
{	
public:
	FlyCamera(float a_aspect = 16.f / 9.f, float a_fov = 45.f, float a_near = 1.f, float a_far = 100.f)
		: aspect(a_aspect), fov(a_fov), near(a_near), far(a_far), direction(0, 0, 1), speed(10), pitch(0), yaw(0), roll(0)
	{
		
	}
	glm::vec3 position;
	glm::vec3 direction;

	float speed;

	float altitude, azimuth;

	//yaw IS ALONG THE Y
	//pitch is along the x
	//roll is along the z
	float pitch, yaw, roll;

	//make sure near is > 0;
	float aspect, fov, near, far;

	void update(const class Input&in, const class Timer &t);

	void lookAt(const glm::vec3 &target)
	{
		direction = glm::normalize(target - position);
	}
	void jumpTo(glm::vec3 &location)
	{
		position = location;
	}
	glm::mat4 getView() const
	{
		return glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	}
	glm::mat4 getProjection() const
	{
		return glm::perspective(fov, aspect, near, far);
	}
};
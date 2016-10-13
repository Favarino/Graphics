#version 430

layout(location = 0) in vec4 position;

out vec3 vPosition;

layout(location = 0) uniform mat4 mat;


void main()
{
	vPosition = -normalize(position.xyz);
	gl_Position = mat * position;
}
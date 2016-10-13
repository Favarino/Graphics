#version 430

in vec3 vPosition;

layout(location = 1) uniform samplerCube skybox;

out vec4 outColor;

void main()
{
	outColor = texture(skybox, vPosition);
}
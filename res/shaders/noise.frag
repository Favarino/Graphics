#version 450

in vec4 vColor;
in vec4 vPosition;
in float variable;
in vec2 frag_texcoord;

out vec4 outColor;

uniform float time = 0.0f;

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 View;
layout(location = 2) uniform mat4 Model;
layout(location = 3) uniform sampler2D texMap;

void main () 
{ 

	outColor = texture(texMap, frag_texcoord).rrrr;
	outColor.a = 1;
}
#version 430

in vec2 vUV;

layout(location = 0) uniform sampler2D colorMap;
layout(location = 1) uniform sampler2D backgroundMap;

out vec4 outColor;

void main()
{
	vec4 L = texture(colorMap, vUV);
	vec4 S = texture(backgroundMap, vUV);
	
	if(L.a == 0) outColor = S;
	
	else outColor = L;
}

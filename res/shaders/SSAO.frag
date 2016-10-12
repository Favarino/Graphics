#version 430


in vec2 vUV;

layout(location = 0) uniform sampler2D positionMap;
layout(location = 1) uniform sampler2D normalMap;
layout(location = 2) uniform sampler2D randomMap;


out float outTerm;

mat2 rot2deg(float a)
{ a *= 0.0174533; return mat2(cos(a), -sin(a), sin(a), cos(a)); }

uniform vec2 O[4] = 
		vec2[4](vec2(1,0),vec2(-1,0),vec2(0,1),vec2(0,-1));

float occlusionTest(in sampler2D positionMap, in vec2 UV,in vec2 OV, in vec3 P, in vec3 N);

void main()
{
	vec3 P = texture(positionMap, vUV).xyz;
	vec3 N = texture(normalMap, vUV).xyz;
	vec2 R = texture(randomMap, vUV).xy * 2.0f - 1.0f;

	float aoTerm = 0;
	for(int i = 0; i < 4; ++i)
	{
		vec2 C1 = reflect(O[i], R);
		vec2 C2 = rot2deg(45) * C1;

		aoTerm += occlusionTest(positionMap, vUV, C1*.25, P, N);
	    aoTerm += occlusionTest(positionMap, vUV, C2*.50, P, N);
		aoTerm += occlusionTest(positionMap, vUV, C1*.75, P, N);
		aoTerm += occlusionTest(positionMap, vUV, C2*1.0, P, N);
	}
	aoTerm /= 4.f*4.f;

	outTerm = aoTerm;
}

////////////////////
// OV is some arbitrary offset
float occlusionTest(in sampler2D positionMap, in vec2 UV,in vec2 OS, in vec3 P, in vec3 N)
{
	// direction between sampled position and our current position
	vec3 V = normalize(P - texture(positionMap, UV+OS).xyz);

	// if that direction lines up with our normal, we are occluded by that amount.
	// Kinda similar to lambert calculation!
	return max(0, dot(N,V));
}
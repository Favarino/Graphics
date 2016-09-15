#version 450

out vec4 outColor;

in vec3 vNormal;
in vec3 vPosition;
in vec2 UV;

layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;
layout(location = 3)uniform sampler2D diffuseMap;
layout(location = 4)uniform sampler2D specularMap;
layout(location = 5)uniform sampler2D normalMap;

uniform vec3 lDir = normalize(vec3(-1,-1,-1));

mat3 cotangent_frame(in vec3 N, in vec3 p, in vec2 uv);


void main()
{
	mat3 TBN = cotangent_frame(vNormal,vPosition,UV);
	vec3 N  = TBN * (2*texture(normalMap,UV).xyz -1);

	vec3 R = reflect(lDir,N);
	vec3 E = normalize(view[3].xyz + vPosition);

	float lamb = max(0,-dot(lDir,N));
	float spec = max(0,-dot(E,R));

	if(spec > 0)
	{
		spec = pow(spec,4);
	}
	
	outColor = spec*vec4(N,1);
}
mat3 cotangent_frame(in vec3 N, in vec3 p, in vec2 uv)
{
	vec3 dp1 = dFdx(p);
	vec3 dp2 = dFdy(p);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);

	vec3 dp2perp = cross(dp2,N); //determine right
	vec3 dp1perp = cross(N,dp1); //determine up

	//roll the right to line up with the texture's tangent
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
	
	float invmax = inversesqrt(max(dot(T,T),dot(B,B)));
	return mat3(T*invmax,B*invmax,N);
}
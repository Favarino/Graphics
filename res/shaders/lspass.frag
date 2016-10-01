#version 430

// Vertex Data
in vec2 vUV;

// Camera Data
layout(location = 0) uniform mat4 view;

// Gpass Data
layout(location = 1) uniform sampler2D albedoMap;
layout(location = 2) uniform sampler2D normalMap;
layout(location = 3) uniform sampler2D specularMap;
layout(location = 4) uniform sampler2D positionMap;
layout(location = 5) uniform sampler2D roughnessMap;

// Shadow Pass Data
layout(location = 6) uniform sampler2D shadowMap;
uniform float shadowBias = 0.1f;

// Light Data
layout(location = 7) uniform vec4 lCol;
layout(location = 8) uniform mat4 lightView; // lightDirection is the forward now!
layout(location = 9) uniform mat4 lightProj;

// Framebuffer Outputs
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outAlbedo;
layout(location = 2) out vec4 outSpecular;


// Simple matrix that converts from clip space (-1,1) to UV space (0,1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

float OrenNayar(in float roughness, in vec3 L, in vec3 N, in vec3 E);

void main()
{		 
// Helpful camera relative information
	vec3 L = normalize((view * lightView[2]).xyz);   // light direction
	vec3 N = normalize(texture(normalMap, vUV).xyz); // surface normal
	vec4 P = texture(positionMap,vUV);			     // texel position
	vec3 R = reflect(L, N);							 // light/surface reflection
	vec3 E = normalize(view[3].xyz + P.xyz);		 // eye direction

	float roughness = texture(roughnessMap, vUV).r;

	
	/////////////////////////////////////////////////////
	/////// Shadow Map calculations

	// Space transfomrations!
	// VIEW -> WORLD -> LIGHT -> CLIP -> UV
	vec4 sUV = clipToUV * lightProj * lightView * inverse(view) * vec4(P.xyz,1);

	// compare the sampled Z value against our projected Z position.
	// if the sample is closer, we don't draw in the shadow.
	if(texture(shadowMap, sUV.xy).r < sUV.z - shadowBias)
		discard;

	/////////////////////////////////////////////////////
	/////// Phong calculations


	float sP = 2;

	float lamb = OrenNayar(roughness,-L, N, E); // max(0,-dot(L, N));

	if (vUV.y < .5f) lamb = max(0,-dot(L, N));

	//if(lamb > .66) lamb = 1;
	//else if(lamb > .25) lamb = .5;
	//else if(lamb > .1) lamb = .25;


	float spec = max(0,-dot(E, R));
	if(spec > 0)
			spec = pow(spec, sP);


	if(spec > .95) spec = 1;
	else if(spec > .5) spec = .75;
	else if(spec > .25) spec = .5;
	else if(spec > 0) spec = .25;

	outAlbedo   = texture(albedoMap,   vUV) * lamb * lCol;
	outSpecular = texture(specularMap, vUV) * spec * lCol;
	outColor    =  outAlbedo + outSpecular;

	outColor = vec4(lamb,lamb,lamb,lamb);
}




float OrenNayar(in float roughness, in vec3 L, in vec3 N, in vec3 E)
{
float R2 = roughness * roughness;
float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
float B = 0.45f * R2 / (R2 + 0.09f);
float NdL = max(0.0f,dot(N,L));
float NdE = max(0.0f,dot(N,E));

vec3 lightProjected = normalize(L-N*NdL);
vec3 viewProjected = normalize(E-N*NdE);
float CX = max(0.0f,dot(lightProjected,viewProjected));

float alpha = sin(max(acos(NdE),acos(NdL)));
float beta = tan(min(acos(NdE),acos(NdL)));
float DX = alpha * beta;

float OrenNayar = NdL * (A+B*CX*DX);
return OrenNayar;
}
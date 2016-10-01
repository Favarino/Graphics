#version 430

//roughness;
//L; //vector light is coming from
//N; //surface normal
//E; //vector from surface to the viewer

layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;

layout(location = 3)uniform sampler2D diffuseMap;
layout(location = 4)uniform sampler2D specularMap;
layout(location = 5)uniform sampler2D normalMap;


layout(location = 0)out vec4 outColor;
layout(location = 1)out vec4 outDiffuse;
layout(location = 2)out vec4 outSpecular;


in vec3 vNormal;
in vec3 vPosition;
in vec2 UV;

mat3 cotangent_frame(in vec3 N, in vec3 p, in vec2 uv);

uniform vec3 lDir = normalize(vec3(-1,-1,-1));


float OrenNayar(in float roughness, in vec3 L, in ve3 N, in vec3 E);

void main()
{ 
	mat3 TBN = cotangent_frame(vNormal, vPosition, UV);
	vec3 N	 = TBN * (2*texture(normalMap, UV).xyz - 1);
	float oren = OrenNayar(2,);

	//N = vNormal;

	vec3 R = reflect(lDir, N);
	vec3 E = normalize(view[3].xyz + vPosition);

	float sP = 32;

	//float lamb = max(0,-dot(lDir, N));
	float lamb = oren;
	float spec = max(0,-dot(E, R));
	if(spec > 0)
			spec = pow(spec, 2);

	vec4 mDC = texture(diffuseMap, UV);
	vec4 mSC = texture(specularMap, UV);

	outDiffuse  = mDC * lamb;
	outSpecular = mSC * spec;

	vec4 ambient = mDC * .2;
	outColor =  outDiffuse + outSpecular  + ambient;
}


mat3 cotangent_frame( in vec3 N, in vec3 p, in vec2 uv )
{
	// rate of change of position is velocity, which is a direction!
    vec3 dp1  = dFdx( p );
    vec3 dp2  = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

	// get right angles for all of our directions!
    vec3 dp2perp = cross( dp2, N ); // determine right
    vec3 dp1perp = cross( N, dp1 ); // determine up

	// roll the right to line up with the texture's tangent
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// normalization of the Tangent and Bitangent
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
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
float CX = max(0.0f,dot(lightProjected,viewProjected);

float alpha = sin(max(acos(NdE),acos(NdL)));
float beta = tan(min(acos(NdE),acos(NdL)));
float DX = alpha * beta;

float OrenNayar = NdL * (A+B*CX*DX);
return OrenNayar;
}
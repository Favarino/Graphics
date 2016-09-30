#version 430
layout(location=3)uniform vec4 lDir;
in vec4 vNormal;
out vec4 outColor;
void main()
{
	float intensity;
	vec4 color;
	intensity = dot(normalize(lDir),normalize(vNormal));

	if (intensity > 0.95f)
		color = vec4(1.0f,0.5f,0.5f,1.0f);
	else if (intensity > 0.5f)
		color = vec4(0.6f,0.3f,0.3f,1.0f);
	else if (intensity > 0.25f)
		color = vec4(0.4f,0.2f,0.2f,1.0f);
	 else 
		color = vec4(0.2f,0.1f,0.1f,1.0f);
	
	outColor = color;

}
#version 450

//locations are the 'attributes from the vertex
//in brings data from the previous

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 color;
layout(location = 2)in vec4 normal;
layout(location = 3)in vec2 texcoord;


//out sends data to the new step of the pipeline 
out vec4 vColor;
out vec4 vPosition;
out float variable;
out vec2 frag_texcoord;

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 View;
layout(location = 2) uniform mat4 Model;
layout(location = 3) uniform sampler2D texMap;

void main() 
{ 
	vColor = color; 
	vPosition = position; //Model Space

	frag_texcoord = texcoord;

	vec4 pos = position;
	pos.y += texture(texMap,texcoord).r;

	gl_Position = Projection * View * Model * pos;
	
}
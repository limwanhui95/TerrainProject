#version 430 core
layout (location =0) in vec3 aPos;
layout (location =1) in vec2 aTexCoord;
layout (location =2) in float aType;

flat out float type;
out vec2 texCoord;

uniform mat4 pv;

void main()
{
	gl_Position = pv * vec4(aPos,1.0f);
	type = aType;
	texCoord = aTexCoord;
}
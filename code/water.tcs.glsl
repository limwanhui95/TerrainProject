#version 430 core
layout(vertices =4) out;

uniform mat4 model;
uniform mat4 view;

in vec2 texCoord[];
out vec2 aTexCoord[];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    aTexCoord[gl_InvocationID] = texCoord[gl_InvocationID];
	if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 32;
        gl_TessLevelOuter[1] = 32;
        gl_TessLevelOuter[2] = 32;
        gl_TessLevelOuter[3] = 32;

        gl_TessLevelInner[0] = 32;
        gl_TessLevelInner[1] = 32;
    }
}
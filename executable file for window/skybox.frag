#version 430 core
flat in float type;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D front;
uniform sampler2D right;
uniform sampler2D back;
uniform sampler2D left;
uniform sampler2D top;
uniform sampler2D bottom;

void main()
{
	if (type == 0.0)
	{
		fragColor = texture(front,texCoord);
	}
	else if (type == 1.0)
	{
		fragColor = texture(right,texCoord);
	}
	else if (type == 2.0)
	{	
		fragColor = texture(back,texCoord);
	}
	else if (type == 3.0)
	{
		fragColor = texture(left, texCoord);
	}
	else if (type == 4.0)
	{
		fragColor = texture(top, texCoord);
	}
	else if (type == 5.0)
	{
		fragColor = texture(bottom, texCoord);
		fragColor.a = 0.55;
	}
}
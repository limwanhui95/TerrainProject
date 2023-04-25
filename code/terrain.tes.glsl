#version 430 core
layout(quads, fractional_odd_spacing, ccw) in;

uniform sampler2D height;
uniform mat4 model;
uniform mat4 view;
uniform mat4 pv;
uniform float flag;


in vec2 aTexCoord[];

out vec2 texCoord;
out vec3 aNormal;
out vec4 aPos;


void main()
{
	float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

	vec2 t00 = aTexCoord[0];
    vec2 t01 = aTexCoord[1];
    vec2 t10 = aTexCoord[2];
    vec2 t11 = aTexCoord[3];

	vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    texCoord = (t1 - t0) * v + t0;

	float Height = texture(height, texCoord).y * 70.0 - 29.5;

	vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

	vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
	vec4 p = (p1 - p0) * v + p0;
	p.y = flag * Height;

	gl_Position = pv * model * p;
	aPos = model * p;

	// compute vertex normal 
	vec4 deltaX_p = p;
	deltaX_p.x = deltaX_p.x + 0.1;
	vec2 deltaX_texCoord = texCoord;
	deltaX_texCoord.x = deltaX_texCoord.x + 0.1/300.0;
	float deltaX_Height = texture(height, deltaX_texCoord).y * 70.0 - 29.5;
	deltaX_p.y = deltaX_Height;
	
	vec4 deltaY_p = p;
	deltaY_p.z = deltaY_p.z + 0.1;
	vec2 deltaY_texCoord = texCoord;
	deltaY_texCoord.y = deltaY_texCoord.y + 0.1/300.0;
	float deltaY_Height = texture(height, deltaY_texCoord).y * 70.0 - 29.5;
	deltaY_p.y = deltaY_Height;

	vec4 uVec = model * (deltaX_p-p);
	vec4 vVec = model * (deltaY_p-p);
	vec4 normal = normalize(vec4(cross(vVec.xyz,uVec.xyz),0));
	aNormal = normal.xyz;
}
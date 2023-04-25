#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices=10) out;

in vec2 texCoord[];
in vec3 aNormal[]; 
in vec3 fragPos[];

out GS_FS_VERTEX
{
	vec2 texCoord;
	vec3 aNormal;
	vec3 fragPos;
} vertex_out;

vec3 normal(vec4 p1, vec4 p2, vec4 p3)
{
	vec4 U = p2 - p1;
	vec4 V = p3 - p1;
	float nx = U.y * V.z - U.z * V.y;
	float ny = U.z * V.x - U.x * V.z;
	float nz = U.x * V.y - U.y * V.x;
	float magnitude = sqrt(nx*nx+ny*ny+nz*nz);
	return vec3(nx/magnitude,ny/magnitude,nz/magnitude);
}


void main()
{
	// calculate the normal for this triangle_strip
	//vertex_out.aNormal = normal(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_in[2].gl_Position);
	for (int n=0; n< gl_in.length(); n++)
	{
		vertex_out.aNormal = aNormal[n];
		vertex_out.texCoord = texCoord[n];
		vertex_out.fragPos = fragPos[n];
		gl_Position = gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}

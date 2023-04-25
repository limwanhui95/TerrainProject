#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices=30) out;

in vec2 texCoord[];
in vec3 aNormal[];
in vec4 aPos[];

uniform mat4 pv;
uniform mat4 model;
uniform sampler2D terrain;
uniform sampler2D noise;
uniform sampler2D noise_2;
uniform float wind;

out GS_FS_VERTEX
{
	vec2 texCoord;
	vec3 normal;
	flat bool isGrass;
	vec4 aPos;
} vertex_out;

#define M_PI 3.1415926535897932384626433832795

bool judgeGreen(vec4 color)
{
	if (color.r + color.b <= color.g)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vec4 rotateFun(vec3 axis, vec3 origin, vec4 point, float detaR)
{
	// define the rotate matrix
	float cosdeta = cos(detaR);
	float sindeta = sin(detaR);
	vec4 firstColumn = vec4( 
	  cosdeta + axis.x * axis.x * (1.0 - cosdeta),
	  axis.y * axis.x * (1.0 - cosdeta) + axis.z * sindeta,
	  axis.z * axis.x * (1.0 - cosdeta) - axis.y * sindeta,
	  0
	);
	vec4 secondColumn = vec4(
	  axis.x * axis.y * (1.0 - cosdeta) - axis.z * sindeta,
	  cosdeta + axis.y * axis.y *(1.0 - cosdeta),
	  axis.z * axis.y * (1.0 - cosdeta) + axis.x * sindeta,
	  0
	);
	vec4 thirdColumn = vec4(
	  axis.x * axis.z * (1.0 - cosdeta) + axis.y * sindeta,
	  axis.y * axis.z * (1.0 - cosdeta) - axis.x * sindeta,
	  cosdeta + axis.z * axis.z * (1.0 - cosdeta),
	  0
	);
	vec4 fourthColumn = vec4(0.0,0.0,0.0,1.0);
	mat4 rotateMatrix = mat4(firstColumn,secondColumn,thirdColumn,fourthColumn);

	// calculate the rotated point
	vec4 relativePoint = point - vec4(origin,0); // point in relative coordination from origin
	vec4 rotatedPoint = rotateMatrix * relativePoint;
	return rotatedPoint + vec4(origin,0);
}

void generateGrass(vec4 point1, float width, vec4 height, float randomRotation, vec3 rotateAxis)
{
	vertex_out.isGrass = true;

	int layerNumber = 3;
	vec4 pointA, pointB;
	for (int layer = 0; layer <= layerNumber; layer++)
	{
		vertex_out.texCoord = vec2(0.0, 1.0 * layer / float(layerNumber));
		pointA = point1 + float(layer) / layerNumber * height;
		pointA.x += width * wind * (-layer * layer + 8 * layer) / float(layerNumber);
		gl_Position = pv* model * pointA;
		EmitVertex();

		vertex_out.texCoord = vec2(1.0, 1.0* layer / float(layerNumber));
		pointB = point1 + float(layer) / layerNumber * height;
		pointB.x += width + width * wind * (-layer * layer + 8 * layer) / float(layerNumber);
		pointB = rotateFun(rotateAxis, pointA.xyz, pointB, M_PI * randomRotation);
		gl_Position = pv * model * pointB;
		EmitVertex();
	}
	EndPrimitive();
}

int adj_fun(int index)
{
	if (index != 2)
		return index+1;
	else
		return 0;
}

void generateGrass_perPrimitve(vec4 point[3])
{
	// setting for grass
	int grassLayer = 1;
	float grassHeight = 1.2f;
	float grassWidth = 0.15f;
	// generate grass at each point
	for (int i = 0; i < 3; i++)
	{
		int adjIndex = adj_fun(i);
		int adjIndex2 = adj_fun(adjIndex);
		// generate random number based on position
		float random = cos(M_PI / 2.0 * fract(aPos[adjIndex].y * 10) + M_PI / 2.0 * fract(aPos[adjIndex2].x * 10));
		//float random2 = cos(M_PI/ 2.0 * fract(aPos[adjIndex2].y) + M_PI/ 2.0 * fract(aPos[adjIndex].x));
		//float heightRandom = 1.0 + 0.6 * cos(M_PI/ 2.0 * fract(aPos[adjIndex2].x) + M_PI/ 2.0 * fract(aPos[adjIndex].y));
		//float widthRandom = 1.0 + 0.2 * cos(M_PI/ 2.0 * fract(aPos[adjIndex].x) + M_PI/ 2.0 * fract(aPos[i].y));

		// generate random number based on texture
		float txtRandom = texture(noise,texCoord[i] * 2.0).r;
		float txtRandom2 = texture(noise,texCoord[i] * 2.0).r;

		// width & height of the grass
		//float width = grassWidth * widthRandom; 
		float width = grassWidth * (1.0 + 0.2 * txtRandom);
		vec4 height = vec4(aNormal[i] * (1 + 0.6 * txtRandom2) * grassHeight, 0.0);
		//vec4 height = vec4(aNormal[i] * heightRandom * grassHeight, 0.0);

		//vec4 startingPoint = aPos[i] + random2 * (aPos[adjIndex] - aPos[i]);
		vec4 startingPoint = aPos[i] + txtRandom2 * (aPos[adjIndex] - aPos[i]);
		generateGrass(startingPoint, width, height, random, aNormal[i]);
	}
}

void main()
{
	// setting for grass
	int grassLayer = 1;
	float grassHeight = 1.5f;

	// emit the terrain mesh
	vertex_out.isGrass = false;
	for (int n = 0; n < gl_in.length(); n++)
	{
		vertex_out.aPos = aPos[n];
		vertex_out.normal = aNormal[n];
		vertex_out.texCoord = texCoord[n];
		gl_Position = gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();

	// emit the grass mesh
	if (judgeGreen(texture(terrain,texCoord[0])) && judgeGreen(texture(terrain,texCoord[1])) && judgeGreen(texture(terrain,texCoord[2]))) // judge if is grass
	{
		vec4 allPoint[3];
		for (int i =0; i < 3; i++) {
			allPoint[i] = aPos[i]; }
		generateGrass_perPrimitve(allPoint);
	}
}

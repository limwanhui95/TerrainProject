#pragma once
#include "Shader.h"
#include "Utilities.h"
#include "myStruct.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Terrain
{
public:
	Terrain(const char* heightMap, const char* terrain, const char* detail, const char* grassAlpha, const char* grass, GLint patchNumber, Shader shader, Shader shader2);
	GLint patchNumber;
	GLuint texture[7];
	GLuint VAO, VBO;
	void draw(Shader shader, glm::mat4 view, glm::mat4 pv, GLfloat flag);
	void draw(Shader shader, glm::mat4 view, glm::mat4 pv, GLfloat flag, glm::vec3 cameraPosition, directionalLight sun, bool lightingMode);
	glm::mat4 model = glm::mat4(1.0f);
};


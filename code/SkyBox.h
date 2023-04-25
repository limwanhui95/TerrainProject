#pragma once
#include <glad/glad.h>
#include "Shader.h"
class SkyBox
{
public:
	SkyBox(GLfloat height, GLfloat width, GLfloat length) : height(height), width(width), length(length) {}
	void init();	// set all vertex, texture coord data and bind to vao vbo
	void setTexture(const char* front, const char* right, const char* back, const char* left, const char* top, const char* bottom, Shader shader);
	void draw(Shader shader, glm::mat4 pv);
	GLuint VAO, VBO, EBO;
	GLuint texture[6];
	GLfloat height, width, length;
};


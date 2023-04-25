#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "myStruct.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Water
{
public:
	Water(GLfloat width, GLfloat length) : width(width), length(length), VAO(0), VBO(0), EBO(0), texture(0), movement(glm::vec2(0.0f,0.0f)){}
	void init();
	GLuint VAO, VBO, EBO;
	std::vector<GLuint> texture;
	GLfloat width, length;
	glm::vec2 movement;
	void setTexture(const char* texturePath, Shader shader);
	void setTexture(const char* water, const char* height, Shader shader);
	void update(GLfloat deltaTime);
	void draw(Shader shader, glm::mat4 pv);
	void draw(Shader shader, glm::mat4 pv, glm::mat4 view, glm::vec3 viewPos, directionalLight light, bool mode);
	glm::mat4 model = glm::mat4(1.0f);
};


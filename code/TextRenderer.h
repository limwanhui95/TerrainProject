#pragma once
#include<map>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include"Shader.h"

struct Character
{
	GLuint TextureID;
	glm::vec2 Size;
	glm::vec2 Bearing;
	GLuint Advance;
};

class TextRenderer
{
public:
	std::map<GLchar, Character> Characters;
	Shader TextShader;
	TextRenderer(const char* vertexPath, const char* fragmentPath);
	void load(int fontSize);
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 Color);
private:
	GLuint VAO, VBO;
};


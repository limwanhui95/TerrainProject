#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void init(int major, int minor);

void load_and_bind_texture(const char * texturePath, GLuint &texture, GLint parameter);

void load_and_bind_texture(const char* texturePath, GLuint& texture, GLint parameter, GLint& height, GLint& width);
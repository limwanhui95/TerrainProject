#include "SkyBox.h"
#include "Utilities.h"

void SkyBox::init()
{
	GLfloat vertex[] = {
		// back
		-length / 2.0f, -10.0f,  width / 2.0f, 1.0f, 0.0f, 2.0f,
		 length / 2.0f, -10.0f,  width / 2.0f, 0.0f, 0.0f, 2.0f,
		 length / 2.0f, height,  width / 2.0f, 0.0f, 1.0f, 2.0f,
		-length / 2.0f, height,  width / 2.0f, 1.0f, 1.0f, 2.0f,
		// front
		 length / 2.0f, height, -width / 2.0f, 1.0f, 1.0f, 0.0f,
		-length / 2.0f, height, -width / 2.0f, 0.0f, 1.0f, 0.0f,
		-length / 2.0f, -10.0f, -width / 2.0f, 0.0f, 0.0f, 0.0f,
		 length / 2.0f, -10.0f, -width / 2.0f, 1.0f, 0.0f, 0.0f,
		// left
		-length / 2.0f, -10.0f,  width / 2.0f, 0.0f, 0.0f, 3.0f,
		-length / 2.0f, -10.0f, -width / 2.0f, 1.0f, 0.0f, 3.0f,
		-length / 2.0f, height, -width / 2.0f, 1.0f, 1.0f, 3.0f,
		-length / 2.0f, height,  width / 2.0f, 0.0f, 1.0f, 3.0f,
		// right
		 length / 2.0f, -10.0f,  width / 2.0f, 1.0f, 0.0f, 1.0f,
		 length / 2.0f, -10.0f, -width / 2.0f, 0.0f, 0.0f, 1.0f,
		 length / 2.0f, height, -width / 2.0f, 0.0f, 1.0f, 1.0f,
		 length / 2.0f, height,  width / 2.0f, 1.0f, 1.0f, 1.0f,
		// top 
		-length / 2.0f, height,  width / 2.0f, 0.0f, 1.0f, 4.0f,
		 length / 2.0f, height,  width / 2.0f, 1.0f, 1.0f, 4.0f,
		 length / 2.0f, height, -width / 2.0f, 1.0f, 0.0f, 4.0f,
		-length / 2.0f, height, -width / 2.0f, 0.0f, 0.0f, 4.0f,
		// bottom
		-length / 2.0f, -10.0f,  width / 2.0f, 0.0f, 1.0f, 5.0f,
		 length / 2.0f, -10.0f,  width / 2.0f, 1.0f, 1.0f, 5.0f,
		 length / 2.0f, -10.0f, -width / 2.0f, 1.0f, 0.0f, 5.0f,
		-length / 2.0f, -10.0f, -width / 2.0f, 0.0f, 0.0f, 5.0f,
	};
	GLuint index[] = {
		// back
		0, 3, 2,
		0, 2, 1,
		// front
		4, 5, 6,
		4, 6, 7,
		// left
		11, 8, 9,
		11, 9, 10,
		// right
		15, 12, 13,
		15, 13, 14,
		// top
		19, 16, 17,
		19, 17, 18,
		// bottom
		21, 22, 23,
		21, 23, 20
	};
	// bind vao & vbo
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SkyBox::setTexture(const char* front, const char* right, const char* back, const char* left, const char* top, const char* bottom, Shader shader)
{
	load_and_bind_texture(front, texture[0], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(right, texture[1], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(back, texture[2], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(left, texture[3], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(top, texture[4], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(bottom, texture[5], GL_CLAMP_TO_EDGE);
	// setting uniform
	shader.Use();
	shader.SetInteger("front", 0);
	shader.SetInteger("right", 1);
	shader.SetInteger("back", 2);
	shader.SetInteger("left", 3);
	shader.SetInteger("top", 4);
	shader.SetInteger("bottom", 5);
}

void SkyBox::draw(Shader shader, glm::mat4 pv)
{
	shader.Use();
	shader.SetMatrix4("pv", pv);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

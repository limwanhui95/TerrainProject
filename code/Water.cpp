#include "Water.h"
#include "Utilities.h"
#include <iostream>

void Water::init()
{
	int patchNumber = 10;
	std::vector<GLfloat> vertices;
	for (int i = 0; i < patchNumber; i++)
	{
		for (int j = 0; j < patchNumber; j++)
		{
			vertices.emplace_back(-width / 2.0f + width * i / (float)patchNumber);  // x coord
			vertices.emplace_back(-9.0f); // y coord
			vertices.emplace_back(-length / 2.0f + length * j / (float)patchNumber); // z coord
			vertices.emplace_back(15.0 * i / (float)patchNumber); // u
			vertices.emplace_back(15.0 * j / (float)patchNumber); // v

			vertices.emplace_back(-width / 2.0f + width * (i + 1) / (float)patchNumber);  // x coord
			vertices.emplace_back(-9.0f); // y coord
			vertices.emplace_back(-length / 2.0f + length * j / (float)patchNumber); // z coord
			vertices.emplace_back(15.0 * (i + 1) / (float)patchNumber); // u
			vertices.emplace_back(15.0 * j / (float)patchNumber); // v

			vertices.emplace_back(-width / 2.0f + width * i / (float)patchNumber);  // x coord
			vertices.emplace_back(-9.0f); // y coord
			vertices.emplace_back(-length / 2.0f + length * (j + 1) / (float)patchNumber); // z coord
			vertices.emplace_back(15.0 * i / (float)patchNumber); // u
			vertices.emplace_back(15.0 * (j + 1) / (float)patchNumber); // v

			vertices.emplace_back(-width / 2.0f + width * (i + 1) / (float)patchNumber);  // x coord
			vertices.emplace_back(-9.0f); // y coord
			vertices.emplace_back(-length / 2.0f + length * (j + 1) / (float)patchNumber); // z coord
			vertices.emplace_back(15.0 * float(i + 1) / (float)patchNumber); // u
			vertices.emplace_back(15.0 * float(j + 1) / (float)patchNumber); // v
		}
	}
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::setTexture(const char* texturePath, Shader shader)
{
	shader.Use();
	texture.emplace_back(0);
	load_and_bind_texture(texturePath, texture[0], GL_REPEAT);
	// setting uniform
	shader.SetInteger("water", 0);
}

void Water::setTexture(const char* water, const char* height, Shader shader)
{
	shader.Use();
	texture.emplace_back(0);
	load_and_bind_texture(water, texture[0], GL_REPEAT);
	texture.emplace_back(0);
	load_and_bind_texture(height, texture[1], GL_REPEAT);
	// setting uniform
	shader.SetInteger("water", 0);
	shader.SetInteger("height", 1);
}

void Water::update(GLfloat deltaTime)
{
	GLfloat x = deltaTime / 10.0f;
	GLfloat y = deltaTime / 10.0f;
	movement.x += x;
	movement.y += y;
	if (movement.x > 10.0f)
		movement.x = 0.0f;
	if (movement.y > 10.0f)
		movement.y = 0.0f;
}

void Water::draw(Shader shader, glm::mat4 pv)
{
	shader.Use();
	shader.SetMatrix4("pv", pv);
	shader.SetVector2f("movement", movement);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4 * 10 * 10);
}

void Water::draw(Shader shader, glm::mat4 pv, glm::mat4 view, glm::vec3 viewPos, directionalLight light, bool mode)
{
	shader.Use();
	shader.SetMatrix4("pv", pv);
	shader.SetVector2f("movement", movement);
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", this->model);
	shader.SetVector3f("viewPos", viewPos);
	shader.SetVector3f("light.direction", light.direction);
	shader.SetVector3f("light.ambient", light.ambient);
	shader.SetVector3f("light.diffuse", light.diffuse);
	shader.SetVector3f("light.specular", light.specular);
	if (mode)
	{
		shader.SetInteger("mode",1);
	}
	else
	{
		shader.SetInteger("mode",0);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4 * 10 * 10);
}

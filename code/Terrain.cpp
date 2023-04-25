#include "Terrain.h"


Terrain::Terrain(const char* heightMap, const char* terrain, const char* detail, const char* grassAlpha, const char* grass, GLint patchNumber, Shader shader, Shader shader2)
{
	this->patchNumber = patchNumber;
	// bind texture
	glGenTextures(5, texture);
	load_and_bind_texture(heightMap, texture[0], GL_CLAMP_TO_EDGE);
	GLint width, height; // retreive the height and the width of the terrain map
	load_and_bind_texture(terrain, texture[1], GL_CLAMP_TO_EDGE,height,width);
	load_and_bind_texture(detail, texture[2], GL_REPEAT);
	load_and_bind_texture(grassAlpha, texture[3], GL_CLAMP_TO_EDGE);
	load_and_bind_texture(grass, texture[4], GL_CLAMP_TO_EDGE);
	load_and_bind_texture("texture/noise.jpg", texture[5], GL_REPEAT);
	load_and_bind_texture("texture/noise2.jpg", texture[6], GL_REPEAT);
	// setting uniform
	shader.Use();
	shader.SetInteger("height", 0);
	shader.SetInteger("terrain", 1);
	shader.SetInteger("detail", 2);
	shader2.Use();
	shader2.SetInteger("height", 0);
	shader2.SetInteger("terrain", 1);
	shader2.SetInteger("detail", 2);
	shader2.SetInteger("grassAlpha", 3);
	shader2.SetInteger("grass", 4);
	shader2.SetInteger("noise", 5);
	shader2.SetInteger("noise_2", 6);

	// generate vertex and bind to vao & vbo
	std::vector<GLfloat> vertices;
	for (int i = 0; i < patchNumber; i++)
	{
		for (int j = 0; j < patchNumber; j++)
		{
			vertices.push_back(-width / 2.0f + width * i / (float)patchNumber); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)patchNumber); // v.z
			vertices.push_back(i / (float)patchNumber); // u
			vertices.push_back(j / (float)patchNumber); // v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)patchNumber); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)patchNumber); // v.z
			vertices.push_back((i + 1) / (float)patchNumber); // u
			vertices.push_back(j / (float)patchNumber); // v

			vertices.push_back(-width / 2.0f + width * i / (float)patchNumber); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)patchNumber); // v.z
			vertices.push_back(i / (float)patchNumber); // u
			vertices.push_back((j + 1) / (float)patchNumber); // v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)patchNumber); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)patchNumber); // v.z
			vertices.push_back((i + 1) / (float)patchNumber); // u
			vertices.push_back((j + 1) / (float)patchNumber); // v
		}
	}
	// bind data
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::draw(Shader shader, glm::mat4 view, glm::mat4 pv, GLfloat flag)
{
	shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", this->model);
	shader.SetMatrix4("pv", pv);
	shader.SetFloat("flag", flag);
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

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4 * patchNumber * patchNumber);
}

void Terrain::draw(Shader shader, glm::mat4 view, glm::mat4 pv, GLfloat flag, glm::vec3 cameraPosition, directionalLight sun, bool lightingMode)
{
	shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("model", this->model);
	shader.SetMatrix4("pv", pv);
	shader.SetFloat("flag", flag);
	// set light uniform
	shader.SetVector3f("light.direction", sun.direction);
	shader.SetVector3f("light.ambient", sun.ambient);
	shader.SetVector3f("light.diffuse", sun.diffuse);
	shader.SetVector3f("light.specular", sun.specular);
	// set render mode
	if (lightingMode)
	{
		shader.SetInteger("mode", 1);
	}
	else
	{
		shader.SetInteger("mode", 0);
	}

	// set texture
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

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glBindVertexArray(VAO);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays(GL_PATCHES, 0, 4 * patchNumber * patchNumber);
}



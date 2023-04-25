#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "SkyBox.h"
#include "Water.h"
#include "Shader.h"
#include "Utilities.h"
#include "Terrain.h"
#include "myStruct.h"
#include "TextRenderer.h"

#define PI 3.14159265

// setting
GLuint SCRN_WIDTH = 1900, SCRN_HEIGHT = 1060;

// render mode
bool wireMode = false;
bool modeKeyPressed = false;
bool lightingMode = false;
bool lightingModeKeyPressed = false;
bool grass_3DMode = false;
bool grassModeKeyPressed = false;

// camera
Camera camera(glm::vec3(0.0f, 200.0f, 100.0f));
GLfloat lastX = SCRN_WIDTH / 2.0f;
GLfloat lastY = SCRN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// projection matrix
glm::mat4 projection;
glm::mat4 view;
glm::mat4 pv;

// function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//lighting
directionalLight Sun(glm::vec3(0.7f, -1.0f, -0.7f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f));

int main()
{
	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "Final Project : Terrain", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize glad." << std::endl;
		return -1; // end of the main function
	}

	// set openGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// skybox 
	SkyBox skybox(1500.0f, 3000.0f, 3000.0f);
	skybox.init();
	Shader skyboxShader;
	skyboxShader.Compile("skybox.vert", "skybox.frag");
	skybox.setTexture("texture/SkyBox0.bmp", "texture/SkyBox1.bmp", "texture/SkyBox2.bmp", "texture/SkyBox3.bmp", "texture/SkyBox4.bmp", "texture/SkyBox4.bmp",skyboxShader);
	
	// water
	Water water(3000.0f, 3000.0f);
	GLfloat xMovement = 0.0f, yMovement = 0.0f;
	water.init();
	Shader waterShader;
	//waterShader.Compile("water.vert", "water.tcs.glsl", "water.tes.glsl", "water.frag");
	waterShader.Compile("water.vert", "water.tcs.glsl", "water.tes.glsl", "water.geom.glsl", "water.frag");
	water.setTexture("texture/SkyBox5.bmp", "texture/waterHeight.jpg", waterShader);

	// terrain 
	Shader terrainShader, terrainWithGrassShader;
	terrainWithGrassShader.Compile("terrain.vert", "terrain.tcs.glsl", "terrain.tes.glsl", "terrain.geom.glsl", "terraingrass.frag");
	terrainShader.Compile("terrain.vert", "terrain.tcs.glsl", "terrain.tes.glsl", "terrain.frag");
	Terrain terrain("texture/heightmap.bmp", "texture/terrain-texture3.bmp", "texture/detail.bmp", "texture/grassAlpha.png", "texture/grass.jpg", 12, terrainShader, terrainWithGrassShader);

	// text 
	TextRenderer text("text.vert", "text.frag");
	text.load(20);
	glm::mat4 textProjection = glm::ortho(0.0f, (GLfloat)SCRN_WIDTH, 0.0f, (GLfloat)SCRN_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		terrainWithGrassShader.Use();
		terrainWithGrassShader.SetFloat("wind", std::cos(lastFrame * PI / 3.0));

		processInput(window);
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCRN_WIDTH / (GLfloat)SCRN_HEIGHT, 0.1f, 5000.0f);
		view = camera.GetViewMatrix();
		pv = projection * view;
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render command here
		// All opaque object
		terrain.draw(terrainShader, view, pv, -2.0f);		
		// Transparent object in order furthest to closest
		skybox.draw(skyboxShader, pv);
		water.update(deltaTime);
		if (wireMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			water.draw(waterShader, pv, view, camera.Position, Sun, lightingMode);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			water.draw(waterShader, pv, view, camera.Position, Sun, lightingMode);
		}
		// draw item that dont participate in blending
		if (wireMode)
		{
			if (grass_3DMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				terrain.draw(terrainWithGrassShader, view, pv, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			if (grass_3DMode == false)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				terrain.draw(terrainShader, view, pv, 1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else
		{
			if (grass_3DMode == false)
			{
				terrain.draw(terrainShader, view, pv, 1.0f, camera.Position, Sun, lightingMode);
			}
			else
			{
				terrain.draw(terrainWithGrassShader, view, pv, 1.0f, camera.Position, Sun, lightingMode);
			}
		}

		// render some text
		text.TextShader.Use();
		text.TextShader.SetMatrix4("projection", textProjection);
		std::stringstream deltaTimeStream;
		deltaTimeStream << std::fixed << std::setprecision(0) << 1.0f / deltaTime;
		std::string FPS = "FPS: " + deltaTimeStream.str();
		text.renderText(FPS, (GLfloat)SCRN_WIDTH - 45, (GLfloat)SCRN_HEIGHT - 20.0, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		std::string mode;
		if (wireMode)
			mode = "WireMode:On";
		else
			mode = "WireMode:Off";
		text.renderText(mode, (GLfloat)SCRN_WIDTH - 70, (GLfloat)SCRN_HEIGHT - 40.0, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		if (lightingMode)
			mode = "LightingMode:On";
		else
			mode = "LightingMode:Off";
		text.renderText(mode, (GLfloat)SCRN_WIDTH - 85, (GLfloat)SCRN_HEIGHT - 60.0, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		if (grass_3DMode)
			mode = "3DGrassMode:On";
		else
			mode = "3DGrassMode:Off";
		text.renderText(mode, (GLfloat)SCRN_WIDTH - 90, (GLfloat)SCRN_HEIGHT - 80.0, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		// swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime * 20.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime * 20.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime * 20.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime * 20.0f);

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !modeKeyPressed)
	{
		wireMode = !wireMode;
		modeKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		modeKeyPressed = false;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !lightingModeKeyPressed)
	{
		lightingMode = !lightingMode;
		lightingModeKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
		lightingModeKeyPressed = false;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !grassModeKeyPressed)
	{
		grass_3DMode = !grass_3DMode;
		grassModeKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
		grassModeKeyPressed = false;

}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
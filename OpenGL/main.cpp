#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include<glm/vec4.hpp>
#include<glm/glm.hpp>
#include<glm/mat4x4.hpp>
//This is used for ease the calculation of the view and projection matrices
#include<glm/gtc/matrix_transform.hpp>
//This header adds functionality for converting a matrix object into a float array for usage in OpenGL
#include<glm/gtc/type_ptr.hpp>

//Added this to use to_string for type out the values of vectors.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Camera.h"
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace glm;
using namespace std;

GLFWwindow* InitSystem();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void processInput(GLFWwindow *window);
void InitApp();

void SettingUpBufferData(GLfloat* vertices, int vertexCount, GLuint vao, GLuint vbo,GLuint nextIndex,GLuint typeIndex);

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.007f;
float triIncrement = 0.0001f;
float currentAngle;
//Camera System
float radius = 40;
//double camX, camZ, lastX, lastY, YAW, PITCH;
vec3	CamPos, CamUp, CamDirection;
bool firstMouse = true;
//
//Delta Time Frame rate independent code
float  lastFrameTime;
double currentFrameTime ,deltaTime;
//
const float toRadians = 3.14159265f / 180.0f;
GLuint vbo[2], vao[2];// , IBO;
int width, height, nrChannel;
mat4 objectModelMatrix, viewMatrix, projectMatrix;
//
mat4 lightObjectMM;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//IMPORTANT Learning
//Draw Order is Always Anti-Clock Wise.

// Cube Vertex Data Only Position Attibute.For Lamp
GLfloat vertices2[] = {
	-0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,  
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 

	-0.5f, -0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 
	-0.5f, -0.5f,  0.5f, 

	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f, 

	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  

	-0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f, 
	-0.5f, -0.5f, -0.5f, 

	-0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f
};

//For Object.
// Cube Vertex Data Only position Attibute and normal for the cubes face. 
//3 points for position and 3 points for the normal.
float vertices1[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

vec3 lightSourcePostion = vec3(-1.0f, 0.0f, -1.0f);
//vec3 lightSourcePostion = vec3(-1.5f, 1.0f, -1.0f);
vec3 cubePosition[] = {
	vec3(0.0f,  0.0f, -2.0f),
	vec3(0.0f,0.0f,0.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f,  2.0f, -2.5f),
	vec3(1.5f,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
};


GLFWwindow* InitSystem()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	return window;
}


int main()
{
	GLFWwindow* window = InitSystem();
	if (window == NULL)
	{
		return -1;
	}

	InitApp();
	//LampObject
	SettingUpBufferData(vertices2, sizeof(vertices2), vao[0], vbo[0],3,0);
	//Object.
	SettingUpBufferData(vertices1, sizeof(vertices1), vao[1], vbo[1],6,1);

	//Setting Up Camera 
	CamDirection = vec3(0, 0, -1);
	CamPos = vec3(0, 0, 12);
	CamUp = vec3(0, 1, 0);
	//
	Shader	ObjectShader("VertexCode.vs", "FragCode.fs");

	Shader	lampShader("VertexLightSource.vs", "FragLightSource.fs");
	float accumulatedTime = 0,angle = 0, CircleRadius = 0,x= 0,z=0;
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = float(currentFrameTime);
		accumulatedTime += deltaTime;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMatrix = mat4(1.0f);
		viewMatrix = camera.GetViewMatrix();

		projectMatrix = mat4(1.0f);
		projectMatrix = glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 1000.0f);


		lightObjectMM = mat4(1.0f);
		lightObjectMM = translate(lightObjectMM, lightSourcePostion );

		if (accumulatedTime > 0.1f) {
		//std::cout << "deltaTime" << deltaTime << std::endl;

			angle++;
			accumulatedTime = 0;
			CircleRadius = glm::distance(lightSourcePostion, cubePosition[0]);
			x = CircleRadius * float(cos(radians(angle)));
			z = CircleRadius * float(sin(radians(angle)));
			lightSourcePostion = glm::vec3(x + cubePosition[0].x, cubePosition[0].y, cubePosition[0].z + z);
		}
		if (angle > 360)
		{	
			angle = 0;
			//std::cout << "CircularRadius"<< CircleRadius << std::endl;
		}
		//lightObjectMM = rotate(lightObjectMM, (float)glfwGetTime() * radians(-45.0f), vec3(0.0f, 1.0f, 0));

		lightObjectMM = scale(lightObjectMM,vec3(0.4f));
		lampShader.use();

		lampShader.setMatrix4fv("model",lightObjectMM);
		lampShader.setMatrix4fv("view", viewMatrix);
		lampShader.setMatrix4fv("projection", projectMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//////////

		objectModelMatrix = mat4(1.0f);
		objectModelMatrix = translate(objectModelMatrix, cubePosition[0]);
		objectModelMatrix = rotate(objectModelMatrix, radians(-45.0f), vec3(0.0f, 1.0f, 0));
		//objectModelMatrix = rotate(objectModelMatrix, (float)glfwGetTime() * radians(-45.0f), vec3(0.0f, 1.0f, 0));
		ObjectShader.use();

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		ObjectShader.setVec3("lightmaterial.ambient", vec3(1.0f));
		ObjectShader.setVec3("lightmaterial.diffuse", vec3(1.0f));
		ObjectShader.setVec3("lightmaterial.specular", vec3(1.0f));

		//ObjectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//ObjectShader.setVec3("ObjectColor", glm::vec3(1.0f, 0.1f, 0.5f));
		ObjectShader.setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
		ObjectShader.setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
		ObjectShader.setVec3("material.specular",glm::vec3(0.62828f,0.55802f,0.36606f));
		ObjectShader.setFloat("material.shininess", 64.0);

			
		ObjectShader.setMatrix4fv("model", objectModelMatrix);
		ObjectShader.setMatrix4fv("projection", projectMatrix);

		ObjectShader.setMatrix4fv("view", viewMatrix);
		ObjectShader.setVec3("lightPosition", lightSourcePostion);
		ObjectShader.setVec3("viewPosition", camera.Position);

		//To render a single cube we need a total of 36 vertices
		//6 faces * 2 triangles * 3 vertices each
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void InitApp()
{
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	//glPointSize(5);
	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

}


void SettingUpBufferData(GLfloat* vertices, int vertexCount, GLuint vao, GLuint vbo,GLuint nextIndex,GLuint typeIndex) {
	std::cout << "VAO" << vao << "\t";
	std::cout << "VBO" << vbo << "\t";

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nextIndex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	if (typeIndex == 1) {
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nextIndex * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glfwPollEvents();
	float camSpeed = 0.05f;
	//Move Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		CamPos +=camSpeed  * float(deltaTime) * CamDirection;
	}
	//Move Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		CamPos -= camSpeed * CamDirection * float(deltaTime);
	}
	//Move towards left
	// to get the camera right vector use cross product btw up vector and direction vector.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		CamPos -= camSpeed * normalize(cross(CamDirection, CamUp)) * float(deltaTime);
	}

	//Move towards right[right hand rule , positive x axis is thumb right]
	// to get the camera right vector use cross product btw up vector and direction vector.
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		CamPos += camSpeed * normalize(cross(CamDirection, CamUp)) * float(deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if (firstMouse)
	//{
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}
	//float xoffset = xpos - lastX;
	//float yoffset = lastY - ypos;
	//lastX = xpos;
	//lastY = ypos;
	//float sensitivity = 0.05;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;
	//YAW += xoffset;
	//PITCH += yoffset;
	//if (PITCH > 89.0f)
	//	PITCH = 89.0f;
	//if (PITCH < -89.0f)
	//	PITCH = -89.0f;
	//glm::vec3 front;
	//front.x = float(cos(radians(YAW)) * cos(radians(PITCH)));
	//front.y = float(sin(radians(PITCH)));
	//front.z = float(sin(radians(YAW)) * cos(radians(PITCH)));
	//CamDirection = normalize(front);
}	

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//
//The first parameter specifies which vertex attribute we want to configure.Remember that we specified
//the location of the position vertex attribute in the vertex shader with layout(location =
//	0).This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex
//	attribute, we pass in 0.

//	 The next argument specifies the size of the vertex attribute.The vertex attribute is a vec3 so it is
//	composed of 3 values.

//	 The third argument specifies the type of the data which is GL_FLOAT(a vec* in GLSL consists of
//		floating point values).

//	 The next argument specifies if we want the data to be normalized.If we set this to GL_TRUE all the
//	data that has a value not between 0 (or -1 for signed data) and 1 will be mapped to those values.We
//	leave this at GL_FALSE.

//	 The fifth argument is known as the stride and tells us the space between consecutive vertex attribute
//	sets.Since the next set of position data is located exactly 3 times the size of a float away we specify
//	that value as the stride.Note that since we know that the array is tightly packed(there is no space
//		between the next vertex attribute value) we could’ve also specified the stride as 0 to let OpenGL
//	determine the stride(this only works when values are tightly packed).Whenever we have more vertex
//	attributes we have to carefully define the spacing between each vertex attribute but we’ll get to see
//	more examples of that later on.

//	 The last parameter is of type void* and thus requires that weird cast.This is the offset of where the
//	position data begins in the buffer.Since the position data is at the start of the data array this value is
//	just 0. We will explore this parameter in more detail later on
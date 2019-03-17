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
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace glm;
using namespace std;

GLFWwindow* InitSystem();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void processInput(GLFWwindow *window);
void Render(GLFWwindow *window);
void InitApp();
void Update();
//void CompileShaders(GLuint shaderPrograme);
//void AddShaders(GLuint program, GLenum shaderType, const char* sourceCode);
void GenerateTextureObject();

void CreateTriangle(GLfloat* vertices, int vertexCount, GLuint vao, GLuint vbo);
void DrawTriangle(GLuint vao);

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.007f;
float triIncrement = 0.0001f;
float currentAngle;
//Camera System
float radius = 40;
float camX,camZ, lastX,lastY,YAW,PITCH;
vec3	CamPos, CamUp, CamDirection;
bool firstMouse = true;
//
//Delta Time Frame rate independent code
float deltaTime, currentFrameTime, lastFrameTime;
//
GLuint uniformModel, uniformColor;
//mat4 model;
//GLuint shaderPrograme[1];
const float toRadians = 3.14159265f / 180.0f;
unsigned int textureID;
GLuint vbo[2], vao[2];// , IBO;
 int width, height, nrChannel;
 mat4 objectModelMatrix, viewMatrix,projectMatrix;


//IMPORTANT Learning
//Draw Order is Always Anti-Clock Wise.

//GLfloat vertices1[] = {
//	// positions         // colors        //UV's
//	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,0.0f,  // bottom right
//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,0.0f,  // bottom left
//	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  ,0.5f,1.0f  // top 
//};

GLfloat vertices1[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

vec3 cubePosition[] = {
	vec3(0.0f,0.0f,0.0f),
	vec3(2.0f,  5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f,  2.0f, -2.5f),
	vec3(1.5f,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
};

//unsigned int indices[] = {
//	0, 3, 1,
//	1, 3, 2,
//	2, 3, 0,
//	0, 1, 2
//};


//GLfloat vertices2[] = {
//	0.0f, 0.5f, 0.0f,  
//	-0.5f, 0.0f, 0.0f, 
//	0.50f,  0.0f, 0.0f  
//};
//
//float textureUV[] = {
//0.0f,0.0f,
//1.0f,0.0f,
//0.5f,1.0f
//};

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

	Shader	ourShader("VertexCode.vs", "FragCode.fs");

	ourShader.use();

	CreateTriangle(vertices1, sizeof(vertices1), vao[0], vbo[0]);

	GenerateTextureObject();

	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(vao[0]);
	//Setting Up Camera 
	CamDirection = vec3(0,0,-1);
	CamPos = vec3(0,0,3);
	CamUp = vec3(0,1,0);
	//
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 10; i++) {
			objectModelMatrix = mat4(1.0f);
			objectModelMatrix = translate(objectModelMatrix, cubePosition[i]);
			if (i % 3 == 0) {
				objectModelMatrix = rotate(objectModelMatrix, (float)glfwGetTime() * radians(-55.0f), vec3(0.5f, 1.0f, 0));
			}
			ourShader.setMatrix4fv("model", objectModelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//camX = sin(glfwGetTime()) * radius;
		//camZ = cos(glfwGetTime()) * radius;
		viewMatrix = mat4(1.0f);
		viewMatrix = glm::lookAt(CamPos,CamDirection,CamUp);
		ourShader.setMatrix4fv("view", viewMatrix);

		projectMatrix = glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 1000.0f);
		//projectMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
		ourShader.setMatrix4fv("projection", projectMatrix);
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
	//glGenBuffers(1, &IBO);
	//Note::
	// For drawing a triangle one programe is required for both vertex and fragment shader.
	//intialize the matrix model to identity matrix.
	//model = mat4(1.0f);
	//Keep a note of this
	//vec4 testVector = vec4(1.0f, 0, 0, 1);
	// matrix 4*4 multiply with vector results in vector.
	//testVector = model * testVector;
	//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0));

	// the order of the matrix operation is important. i.e 
	// rotation first and then translation second is not the same as 
	// translation first and rotation second.
	//model = scale(model, vec3(1.5, 2, 2));

	//cout << "Test Vector Direction\n" << to_string(testVector) << std::endl;
	//cout << "Test Vector Direction\n" << to_string(model) << std::endl;

	//CreateTriangle(vertices2, sizeof(vertices2), vao[1], vbo[1]);

	// draw our first triangle
}

void PrePareTexture() {
	//Texture Wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Texture Filterring 
	//Need To Understand why near for min and linear for scaling.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//MipMaps are applied only for down scaled images. not for UpScale images.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void GenerateTextureObject() {
	PrePareTexture();
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned char *data = stbi_load("sha.jpg", &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to Load the Image" << endl;
	}
	stbi_image_free(data);
}

void CreateTriangle(GLfloat* vertices, int vertexCount, GLuint vao, GLuint vbo) {
	std::cout << "VAO" << vao << "\t";
	std::cout << "VBO" << vbo << "\t";

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

	//glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof
	(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void Update() {
	if (direction)
	{
		triOffset += triIncrement;
	}
	else {
		triOffset -= triIncrement;
		//std::cout << "Opposite Direction\n" << triOffset << std::endl;
	}

	if (abs(triOffset) >= triMaxOffset)
	{
		direction = !direction;
	}
	//this makes the triangle rotate.
	//currentAngle += 0.01f;
	if (currentAngle >= 360) {
		currentAngle -= 360;
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
		CamPos += camSpeed * CamDirection * deltaTime;
	} 
	//Move Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		CamPos -= camSpeed * CamDirection * deltaTime;
	}
	//Move towards left
	// to get the camera right vector use cross product btw up vector and direction vector.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		CamPos -= camSpeed * normalize(cross(CamDirection,CamUp)) * deltaTime;
	}

	//Move towards right[right hand rule , positive x axis is thumb right]
	// to get the camera right vector use cross product btw up vector and direction vector.
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		CamPos += camSpeed * normalize(cross(CamDirection, CamUp)) * deltaTime;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	YAW += xoffset;
	PITCH += yoffset;
	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;
	glm::vec3 front;
	front.x = cos(radians(YAW)) * cos(radians(PITCH));
	front.y = sin(radians(PITCH));
	front.z = sin(radians(YAW)) * cos(radians(PITCH));
	CamDirection = normalize(front);
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
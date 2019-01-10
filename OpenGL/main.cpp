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
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace glm;
using namespace std;

GLFWwindow* InitSystem();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Render(GLFWwindow *window);
void InitApp();
void Update();
//void CompileShaders(GLuint shaderPrograme);
//void AddShaders(GLuint program, GLenum shaderType, const char* sourceCode);

void CreateTriangle(GLfloat* vertices, int vertexCount, GLuint vao, GLuint vbo);
void DrawTriangle(GLuint vao);

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.007f;
float triIncrement = 0.0001f;
float currentAngle;
GLuint uniformModel, uniformColor;
//mat4 model;
//GLuint shaderPrograme[1];
const float toRadians = 3.14159265f / 180.0f;

GLuint vbo[2], vao[2];// , IBO;

mat4 objectModelMatrix;


//IMPORTANT Learning
//Draw Order is Always Anti-Clock Wise.

GLfloat vertices1[] = {
	// positions         // colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

//unsigned int indices[] = {
//	0, 3, 1,
//	1, 3, 2,
//	2, 3, 0,
//	0, 1, 2
//};


GLfloat vertices2[] = {
	0.0f, 0.5f, 0.0f,  
	-0.5f, 0.0f, 0.0f, 
	0.50f,  0.0f, 0.0f  
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
	Shader  ourShader("VertexCode.vs", "FragCode.fs");

	ourShader.use();
	objectModelMatrix = mat4(1.0f);
	objectModelMatrix = rotate(objectModelMatrix, radians(180.0f), vec3(0, 0, 1.0f));
	ourShader.setMatrix4fv("model", objectModelMatrix);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		//Update
		//Update();
		// render
		Render(window);		
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

	CreateTriangle(vertices1, sizeof(vertices1), vao[0], vbo[0]);
	//CreateTriangle(vertices2, sizeof(vertices2), vao[1], vbo[1]);

	// draw our first triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

//void AddShaders(GLuint program, GLenum shaderType, const char* sourceCode)
//{
//	GLuint shader = glCreateShader(shaderType);
//	const GLchar* shadercode[1];
//	shadercode[0] = sourceCode;
//	GLint codeLength[1];
//	codeLength[0] = strlen(sourceCode);
//
//	glShaderSource(shader, 1, shadercode, codeLength);
//	glCompileShader(shader);
//	// check for shader compile errors
//	GLint success;
//	char infoLog[1024];
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//		return;
//	}
//	glAttachShader(program, shader);
//}
//
//
//void CompileShaders(GLuint shaderPrograme) {
//	if (!shaderPrograme) {
//		std::cout << "ERROR::SHADER::Programe\n" << std::endl;
//		return;
//	}
//
//	GLint results = 0;
//	GLchar buffer[1024];
//	glLinkProgram(shaderPrograme);
//	glGetProgramiv(shaderPrograme, GL_LINK_STATUS, &results);
//	if (!results) {
//		glGetProgramInfoLog(shaderPrograme, 1024, NULL, buffer);
//		std::cout << "ERROR::SHADER::Programe\n" << buffer << std::endl;
//	}
//
//	glValidateProgram(shaderPrograme);
//	glGetProgramiv(shaderPrograme, GL_VALIDATE_STATUS, &results);
//	if (!results) {
//		glGetProgramInfoLog(shaderPrograme, 1024, NULL, buffer);
//		std::cout << "ERROR::SHADER::Programe\n" << buffer << std::endl;
//	}
//}

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


void Render(GLFWwindow *window)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//uniformColor = glGetUniformLocation(shaderPrograme[0],"outColor");

	//uniformModel = glGetUniformLocation(shaderPrograme[0], "model");

	DrawTriangle(vao[0]);
	//DrawTriangle(vao[1],shaderPrograme[1]);
	//model = translate(model,vec3(triOffset,0,0));
	//model = rotate(model, radians(currentAngle * toRadians), vec3(0.0f, 1.0f, 0.0f));

	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//Keep a note of this.
	//The second parameter of the glUniformMatrix4fv function specifies how many matrices are to be uploaded, 
	//	because you can have arrays of matrices in GLSL.
	//	The third parameter specifies whether the specified matrix should be transposed before usage.
	//	This is related to the way matrices are stored as float arrays in memory; 
	//you don't have to worry about it. The last parameter specifies the matrix to upload, 
	//where the glm::value_ptr function converts the matrix class into an array of 16 (4x4) floats.
	//glUniform4f(uniformColor,1.0f,0,0,1.0f);
	glfwSwapBuffers(window);
}


void DrawTriangle(GLuint vao) {
	glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, 
	//but we'll do so to keep things a bit more organized
	//glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindVertexArray(0); // test it

	//glDeleteVertexArrays(1, &vao1);
	//glDeleteBuffers(1, &vbo1);
	//glUseProgram(shaderPrograme);
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	glfwPollEvents();

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
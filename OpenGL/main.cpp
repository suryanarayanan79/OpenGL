#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* InitSystem();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Render(GLFWwindow *window);
void InitApp();
void Update();
// triangle Movement
bool direction = false;
float offSetX = 0.0f, UniformMoveX, offsetMax = 0.7f,incrementOffset = 0.05f;
//

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"uniform float xMove; \n"
"   gl_Position = vec4(aPos.x + xMove, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.1f, 1.0f);\n"
"}\n\0";


GLuint shaderPrograme[2];
void CompileShaders(GLuint shaderPrograme, const char* frag_shadercode);

GLuint vbo[2], vao[2];
void CreateTriangle(GLfloat* vertices, int vertexCount,GLuint vao,GLuint vbo);
void DrawTriangle(GLuint vao, GLuint shaderPrograme);
void Update(GLuint shaderPrograme);


//IMPORTANT Learning
//Draw Order is Always Anti-Clock Wise.

GLfloat vertices1[] = {
	-1.0f,  1.0f, 0.0f,	// top left
	-1.0f, 0.0f, 0.0f, // bottom left
	0.0f, 0.0f, 0.0f  // extream  left
};


GLfloat vertices2[] = {
	0.0f, 0.5f, 0.0f,  
	-0.5f, 0.0f, 0.0f, 
	0.50f,  0.0f, 0.0f  
};


int main()
{
	GLFWwindow* window = InitSystem();
	if (window == NULL)
	{
		return -1;
	}

	InitApp();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//Update
		//Update(shaderPrograme[0]);
		UniformMoveX = glGetUniformLocation(shaderPrograme[0], "xMove");
		if (direction) {
			offSetX += incrementOffset;
		}
		else {
			offSetX -= incrementOffset;
		}
		if (abs(offSetX) > offsetMax) {
			direction = !direction;
		}
		// render
		Render(window);		

	}

	glfwTerminate();
	return 0;
}


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


void InitApp()
{
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glPointSize(5);

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	CreateTriangle(vertices1, sizeof(vertices1), vao[0], vbo[0]);
	CreateTriangle(vertices2, sizeof(vertices2), vao[1], vbo[1]);

	shaderPrograme[0] = glCreateProgram();

	CompileShaders(shaderPrograme[0], fragmentShaderSource1);

	shaderPrograme[1] = glCreateProgram();

	CompileShaders(shaderPrograme[1],fragmentShaderSource2);
}

void Update(GLuint shaderPrograme) {
	UniformMoveX = glGetUniformLocation(shaderPrograme,"xMove");
	if (direction) {
		offSetX += incrementOffset;
	}
	else {
		offSetX -= incrementOffset;
	}
	if (abs(offSetX) > offsetMax) {
		direction = !direction;
	}
}


void Render(GLFWwindow *window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawTriangle(vao[0],shaderPrograme[0]);
	DrawTriangle(vao[1],shaderPrograme[1]);

	glfwSwapBuffers(window);
}


void DrawTriangle(GLuint vao,GLuint shaderPrograme) {
	glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, 
	//but we'll do so to keep things a bit more organized

	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindVertexArray(0); // test it

	//glDeleteVertexArrays(1, &vao1);
	//glDeleteBuffers(1, &vbo1);
	glUseProgram(shaderPrograme);
}


void CreateTriangle(GLfloat* vertices, int vertexCount ,GLuint vao , GLuint vbo) {
	std::cout << "VAO" << vao << "\t" ;
	std::cout << "VBO" << vbo << "\t" ;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);

	//This means we have to specify how OpenGL should interpret the vertex data before rendering.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}


void AddShaders(GLuint program, GLenum shaderType, const char* sourceCode)
{
	GLuint shader = glCreateShader(shaderType);
	const GLchar* shadercode[1];
	shadercode[0] = sourceCode;
	GLint codeLength[1];
	codeLength[0] = strlen(sourceCode);

	glShaderSource(shader, 1, shadercode, codeLength);
	glCompileShader(shader);
	// check for shader compile errors
	GLint success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}
	glAttachShader(program, shader);
}


void CompileShaders(GLuint shaderPrograme, const char* frag_shadercode) {
	if (!shaderPrograme) {
		std::cout << "ERROR::SHADER::Programe\n" << std::endl;
		return;
	}
	AddShaders(shaderPrograme, GL_VERTEX_SHADER, vertexShaderSource);
	AddShaders(shaderPrograme, GL_FRAGMENT_SHADER, frag_shadercode);

	GLint results = 0;
	GLchar buffer[1024];
	glLinkProgram(shaderPrograme);
	glGetProgramiv(shaderPrograme, GL_LINK_STATUS, &results);
	if (!results) {
		glGetProgramInfoLog(shaderPrograme, 1024, NULL, buffer);
		std::cout << "ERROR::SHADER::Programe\n" << buffer << std::endl;
	}

	glValidateProgram(shaderPrograme);
	glGetProgramiv(shaderPrograme, GL_VALIDATE_STATUS, &results);
	if (!results) {
		glGetProgramInfoLog(shaderPrograme, 1024, NULL, buffer);
		std::cout << "ERROR::SHADER::Programe\n" << buffer << std::endl;
	}
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
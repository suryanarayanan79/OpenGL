#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
unsigned int vbo1, vao1, vao2, vbo2;

GLuint shaderPrograme;

void CreateTriangle(GLfloat* vertices, int vertexCount);
void DrawTriangle();

void CompileShaders();
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
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	CreateTriangle(vertices1, sizeof(vertices1));
	//CreateTriangle(vertices2, sizeof(vertices2));

	CompileShaders();

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		DrawTriangle();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void DrawTriangle() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our first triangle
	glUseProgram(shaderPrograme);

	glBindVertexArray(vao1); // seeing as we only have a single VAO there's no need to bind it every time, 
	glBindVertexArray(vao2); // seeing as we only have a single VAO there's no need to bind it every time, 
	//but we'll do so to keep things a bit more organized

	glPointSize(5);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindVertexArray(0); // no need to unbind it every time 

	//glDeleteVertexArrays(1, &vao1);
	//glDeleteBuffers(1, &vbo1);
}

void CreateTriangle(GLfloat* vertices, int vertexCount ) {

	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);

	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);

	//glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);

	//This means we have to specify how OpenGL should interpret the vertex data before rendering.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

void CompileShaders() {
	shaderPrograme = glCreateProgram();
	if (!shaderPrograme) {
		std::cout << "ERROR::SHADER::Programe\n" << std::endl;
		return;
	}
	AddShaders(shaderPrograme, GL_VERTEX_SHADER, vertexShaderSource);
	AddShaders(shaderPrograme, GL_FRAGMENT_SHADER, fragmentShaderSource);
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
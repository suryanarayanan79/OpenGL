#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

class Shader
{
public: 
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragPath) {
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream vFragFile;
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		vFragFile.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			vShaderFile.open(vertexPath);
			vFragFile.open(fragPath);
			stringstream vShaderStream, vFragStream;
			vShaderStream << vShaderFile.rdbuf();
			vFragStream << vFragFile.rdbuf();
			vShaderFile.close();
			vFragFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = vFragStream.str();
		}
		catch (ifstream::failure e) {
			cout << "File not read successfully" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* vFragCode = fragmentCode.c_str();

		//Compile shaders
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex,1,&vShaderCode,NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment,1,&vFragCode,NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment,"FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID,vertex);
		glAttachShader(ID,fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID,"LINKPROGRAME");
		glDeleteShader(fragment);
		glDeleteShader(vertex);
	}

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const string &name, bool value)const {
		glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
	}

	void setInt(const string &name, int value)const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const string &name, float value)const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
};

#endif // 

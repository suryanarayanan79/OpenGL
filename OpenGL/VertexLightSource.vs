#version 330 core
//vertex Shader
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform float offset;
void main()
{
// inverse the trainagle. upside down.
	
	// this also does that but using matrix rotation
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	
}
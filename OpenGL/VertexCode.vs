#version 330 core
//vertex Shader
layout (location = 0) in vec3 aPos;
// I need more information on location.
layout (location = 1) in vec3 aNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 fragPosition;

//uniform float offset;
void main()
{
	// this also does that but using matrix rotation
	//Normal = aNormal;
	fragPosition = vec3(model * vec4(aPos, 1.0f));
	 Normal = mat3(transpose(inverse(model))) * aNormal; 
	gl_Position = projection * view *  vec4(fragPosition, 1.0f);
}
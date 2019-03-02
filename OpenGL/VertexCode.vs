#version 330 core
//vertex Shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;
out vec3 vertexColor;
//out vec4 outPos;
out vec2 textureCord;
uniform mat4 model;
//uniform float offset;
void main()
{
// inverse the trainagle. upside down.
	//gl_Position =   vec4(aPos.x ,aPos.y,aPos.z, 1.0f);
	// this also does that but using matrix rotation
	gl_Position =   model * vec4(aPos, 1.0f);
	//vertexColor = aColor;
	//outPos = gl_Position;
	textureCord = aTexture;
}
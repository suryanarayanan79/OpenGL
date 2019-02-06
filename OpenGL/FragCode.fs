#version 330 core
out vec4 FragColor;
in vec4 outPos;
in vec3 vertexColor;
in vec2 textureCord;

uniform sampler2D ourTexture;

void main()
{
   
   FragColor = texture(ourTexture,textureCord);
}
#version 330 core
out vec4 FragColor;
in vec4 outPos;
in vec3 vertexColor;
void main()
{
   FragColor = vec4(outPos);
}
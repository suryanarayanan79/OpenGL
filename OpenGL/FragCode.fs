#version 330 core
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 lightColor;

void main()
{
   //FragColor = vec4(vertexColor,1.0f);
   float ambientStrength = 0.5f;
   vec3 ambient = ambientStrength * lightColor;
   vec3 result = ambient * ObjectColor;
   FragColor = vec4(result ,1.0f);
}
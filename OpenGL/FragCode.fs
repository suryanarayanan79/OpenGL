#version 330 core
out vec4 FragColor;
in vec4 outPos;
in vec3 vertexColor;
in vec2 textureCord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
   //FragColor = mix(texture(ourTexture1,textureCord),texture(ourTexture2,textureCord),0.2f);
   FragColor = texture(ourTexture1,textureCord);
}
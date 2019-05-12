#version 330 core
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 lightColor;
in vec3 Normal;
uniform vec3 lightPosition;
in vec3 fragPosition;

void main()
{
   //FragColor = vec4(vertexColor,1.0f);
   float ambientStrength = 0.5f;
   vec3 ambient = ambientStrength * lightColor;
  // Diffuse Color calculation.
   vec3 norm = normalize(Normal);
   vec3 lightDirection = normalize(lightPosition - fragPosition);
   float diff = max(dot(norm,lightDirection),0.0f);
   vec3 diffuse = diff * lightColor;

   vec3 result = (ambient + diffuse) * ObjectColor;
   FragColor = vec4(result ,1.0f);

}
#version 330 core
out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
//shininess impacts the scattering/radius of the specular highlight
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

in vec3 Normal;
in vec3 fragPosition;
float specularStrength = 0.5f;
float ambientStrength = 0.5f;

void main()
{
   //FragColor = vec4(vertexColor,1.0f);
   vec3 ambient = ambientStrength * lightColor;
  // Diffuse Color calculation.
   vec3 norm = normalize(Normal);

   vec3 lightDirection = normalize(lightPosition - fragPosition);
   float diff = max(dot(norm,lightDirection),0.0f);
   vec3 diffuse = diff * lightColor;

   vec3 viewDirection = normalize(viewPosition - fragPosition);
   vec3 reflectionVector = reflect(-lightDirection,norm);
   float spec = pow(max(dot(reflectionVector,viewDirection),0.0f),32);
   vec3 specular = specularStrength * spec * lightColor;

   vec3 result = (ambient + diffuse + specular) * ObjectColor;
   FragColor = vec4(result ,1.0f);

}
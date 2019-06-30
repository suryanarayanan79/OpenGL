#version 330 core



uniform vec3 lightPosition;
uniform vec3 viewPosition;
//shininess impacts the scattering/radius of the specular highlight
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct LightMaterial{
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 position;
};

uniform LightMaterial lightmaterial;
uniform Material material;

in vec3 Normal;
in vec3 fragPosition;
out vec4 FragColor;

void main()
{
   vec3 ambient =  lightmaterial.ambient * material.ambient;
  // Diffuse Color calculation.
   vec3 norm = normalize(Normal);
   vec3 lightDirection = normalize(lightPosition - fragPosition);
   float diffuseStrength = max(dot(norm,lightDirection), 0.0f);
   vec3 diffuse = lightmaterial.diffuse * (diffuseStrength  * material.diffuse);

   vec3 viewDirection = normalize(viewPosition - fragPosition);
   vec3 reflectionVector = reflect(-lightDirection, norm);
   float spec = pow(max(dot(viewDirection, reflectionVector),0.0f), material.shininess);
   vec3 specular =	 lightmaterial.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result , 1.0);

}
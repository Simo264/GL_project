#version 330 core

in vec3   Normal;
in vec3   FragPos;  
in vec2   TexCoords;
out vec4  FragColor;

struct material_t {
  sampler2D diffuse;  // texture diffuse
  sampler2D normal;   // texture normal
  sampler2D specular; // texture specular
  float     shininess;
};

struct light_t {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // implementing attenuation
  float constant;
  float linear;
  float quadratic;
};

uniform material_t material;
uniform light_t    light; 
uniform vec3       viewPos;

void main()
{
  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse 
  vec3 norm     = normalize(Normal);
  vec3 lightDir = normalize(-light.direction);
  float diff    = max(dot(norm, lightDir), 0.0);
  vec3 diffuse  = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  

  // specular
  vec3 viewDir    = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular   = light.specular * spec * texture(material.specular, TexCoords).rgb;

  float distance    = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  

  ambient  *= attenuation; 
  diffuse  *= attenuation;
  specular *= attenuation;   

  vec3 result = ambient + diffuse + specular;
  FragColor   = vec4(result, 1.0);
}
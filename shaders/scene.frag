#version 330 core

in vec3   Normal;
in vec3   FragPos;  
in vec2   TexCoords;
out vec4  FragColor;

struct Material {
  vec3  ambient;      // color ambient 
  vec3  diffuse;      // color diffuse 
  vec3  specular;     // color specular 
  float shininess;    // color shininess 
  
  sampler2D texDiffuse;  // texture diffuse
  sampler2D texNormal;   // texture normal
  sampler2D texSpecular; // texture specular
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float linear;
  float quadratic;
};

uniform Material    material;
uniform DirLight    dirLight; 
uniform PointLight  pointLight; 
uniform vec3        viewPos;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-dirLight.direction);
  
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  
  // combine results
  vec3 ambient  = dirLight.ambient  * vec3(texture(material.texDiffuse, TexCoords));
  vec3 diffuse  = dirLight.diffuse  * diff * vec3(texture(material.texDiffuse, TexCoords));
  vec3 specular = dirLight.specular * spec * vec3(texture(material.texSpecular, TexCoords));
  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(pointLight.position - fragPos);
  
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  
  // attenuation
  float distance    = length(pointLight.position - fragPos);
  float attenuation = 1.0 / (1.0 + pointLight.linear * distance + pointLight.quadratic * (distance * distance));    
  
  // combine results
  vec3 ambient  = pointLight.ambient  * vec3(texture(material.texDiffuse, TexCoords));
  vec3 diffuse  = pointLight.diffuse  * diff * vec3(texture(material.texDiffuse, TexCoords));
  vec3 specular = pointLight.specular * spec * vec3(texture(material.texSpecular, TexCoords));
  
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

void main()
{
  // properties
  vec3 norm    = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  // phase 1: Directional lighting
  vec3 result = CalcDirLight(dirLight, norm, viewDir);

  // phase 2: Point lights
  // for(int i = 0; i < NR_POINT_LIGHTS; i++)
  //  result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
  result += CalcPointLight(pointLight, norm, FragPos, viewDir); 

  // phase 3: Spot light
  // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  

  FragColor = vec4(result, 1.0);
}





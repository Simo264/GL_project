#version 330 core

in vec3   Normal;
in vec3   FragPos;  
in vec2   TexCoords;
out vec4  FragColor;

struct Material {
  sampler2D diffuse;  // texture diffuse
  sampler2D normal;   // texture normal
  sampler2D specular; // texture specular

  float shininess;
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

struct SpotLight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float cutOff;
  float outCutOff;

  float linear;
  float quadratic;
};

uniform Material    material;
uniform DirLight    dirLight; 
uniform PointLight  pointLight; 
uniform SpotLight   spotLight; 
uniform vec3        viewPos;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

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
  result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  

  FragColor = vec4(result, 1.0);

  // float near  = 0.1; 
  // float far   = 100.0; 
  // float z     = gl_FragCoord.z * 2.0 - 1.0;
  // float depth = ((2.0 * near * far) / (far + near - z * (far - near)))	 / far;
  // FragColor   = vec4(vec3(depth), 1.0);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-dirLight.direction);
  
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  
  // combine results
  vec3 ambient  = dirLight.ambient  * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse  = dirLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, TexCoords));
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
  vec3 ambient  = pointLight.ambient  * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse  = pointLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
  vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, TexCoords));
  
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(spotLight.position - fragPos);

  // check if lighting is inside the spotlight cone
  float theta = dot(lightDir, normalize(-spotLight.direction));

  // remember that we're working with angles as cosines instead of degrees so a '>' is used.
  if(theta > spotLight.cutOff) 
  {
    // ambient
    vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // diffuse 
    vec3 norm     = normalize(normal);
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = spotLight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = spotLight.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // spotlight (soft edges)
    float intensity = 1.0 - (1.0 - theta) / (1.0 - spotLight.cutOff);
    diffuse  *= intensity;
    specular *= intensity;



    // attenuation
    float distance    = length(spotLight.position - fragPos);
    float attenuation = 1.0 / (1.0 + spotLight.linear * distance + spotLight.quadratic * (distance * distance));    

    // remove attenuation from ambient, as otherwise at large distances the light would be 
    // darker inside than outside the spotlight due the ambient term in the else branch
    // ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
    
    return vec3(ambient + diffuse + specular);
  }
  else 
  {
    return vec3(0.f,0.f,0.f);
  }
}

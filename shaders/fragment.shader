// #version 330 core

// in vec2 TexCoord;
// out vec4 FragColor;

// uniform sampler2D texture1;

// void main()
// {
//   FragColor = texture(texture1, TexCoord);
// }

#version 330 core

in vec3 Normal;  
in vec3 FragPos;

out vec4 FragColor;
  
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

uniform bool ligthObject;

void main()
{
  if(!ligthObject)
  {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
  }
  else
  {
    FragColor = vec4(1.0);
  }
    
}
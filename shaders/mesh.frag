#version 330 core

in vec3   Normal;
in vec2   TexCoords;
out vec4  FragColor;

struct Material {
  sampler2D diffuse;  // texture diffuse
  sampler2D normal;   // texture normal
  sampler2D specular; // texture specular
  float     shininess;
};

uniform Material material;

void main()
{
  // diffuse
  vec3 diffuse = vec3(texture(material.diffuse, TexCoords)); 

  // normal
  vec3 normal  = vec3(texture(material.normal, TexCoords)); 

  // specular
  vec3 specular = vec3(texture(material.specular, TexCoords)); 

  vec3 result = diffuse ;
  FragColor   = vec4(result, 1.0);

}
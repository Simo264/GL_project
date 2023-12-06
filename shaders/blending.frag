#version 330 core

in  vec2  TexCoords;
out vec4  FragColor;

struct Material {
  sampler2D diffuse;
};

uniform Material material;

void main()
{
  //vec4 texColor = texture(material.diffuse, TexCoords);
  //FragColor = texColor;
  
  FragColor = vec4(1.0, 1.0, 0.80, 1.0);
}
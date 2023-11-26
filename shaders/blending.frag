#version 330 core

in vec2   TexCoords;
out vec4  FragColor;

struct Material {
  sampler2D diffuse;
};

uniform Material material;

void main()
{
  // vec4 texColor = texture(material.diffuse, TexCoords);
  // if(texColor.a < 0.1)
  //  discard;
  // FragColor = texColor;

  FragColor = texture(material.diffuse, TexCoords);
  
  // FragColor = vec4(0.25, 0.50, 0.75, 1);
}
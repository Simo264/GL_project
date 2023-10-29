#version 330 core

in vec3   Normal;
in vec2   TexCoords;
out vec4  FragColor;

uniform sampler2D texture_diffuse1;

#define DEBUG_MODE 1

void main()
{
  vec4 vecTexure = texture(texture_diffuse1, TexCoords);

  #if DEBUG_MODE
    FragColor = vecTexure + vec4(1,1,1,1);
  #else
    FragColor = vecTexure;
  #endif

}
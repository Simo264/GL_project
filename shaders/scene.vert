#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;  
out vec2 TexCoords;

uniform vec3 translations[100];

void main()
{
  // Normal      = aNormal;
  // FragPos     = vec3(model * vec4(aPos, 1.0));
  // TexCoords   = aTexCoord;
  // gl_Position = projection * view * model * vec4(aPos, 1.0);

  vec3 trans  = translations[gl_InstanceID];
  Normal      = aNormal;
  FragPos     = vec3(model * vec4(aPos, 1.0));
  TexCoords   = aTexCoord;
  gl_Position = projection * view * model * vec4(aPos + trans, 1.0);
} 
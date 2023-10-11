#ifndef VERTEX_HH
#define VERTEX_HH

#include "glm/glm.hpp"

struct vertex_t
{
  glm::vec3 position;
  glm::vec2 textureCoords;

  static const int VERTEX_COMPONENTS = 5;
  static const int VERTEX_LENGTH = sizeof(position) + sizeof(textureCoords);
};

#endif
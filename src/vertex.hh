#ifndef VERTEX_HH
#define VERTEX_HH

#include "glm/glm.hpp"

struct vertex_t
{
  glm::vec3 position;
  glm::vec3 normal;

  static constexpr int VERTEX_COMPONENTS = 6;
  static constexpr int VERTEX_LENGTH  = sizeof(position) + sizeof(normal);
};

#endif
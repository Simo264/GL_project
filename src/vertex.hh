#ifndef VERTEX_HH
#define VERTEX_HH

#include "glm/glm.hpp"

struct vertex_t
{
  using posComponent      = glm::vec3;  // 8bytes
  using normalComponent   = glm::vec3;  // 8bytes
  using texcoordComponent = glm::vec2;  // 4bytes
  
  posComponent      position;
  normalComponent   normal;
  texcoordComponent texCoord;

  static constexpr int VERTEX_COMPONENTS =  posComponent::length()    +
                                            normalComponent::length() +
                                            texcoordComponent::length();
  
  static constexpr int VERTEX_LENGTH  = sizeof(posComponent)      + 
                                        sizeof(normalComponent)   + 
                                        sizeof(texcoordComponent);
};

#endif
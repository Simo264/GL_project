#ifndef VERTEX_HH
#define VERTEX_HH

#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

#include <array>

struct vertex_t
{
  using posComponent      = glm::vec3;  // 8bytes
  using normalComponent   = glm::vec3;  // 8bytes
  using texcoordComponent = glm::vec2;  // 4bytes
  
  static constexpr int VERTEX_COMPONENTS =  posComponent::length()    +
                                            normalComponent::length() +
                                            texcoordComponent::length();
  
  static constexpr int VERTEX_LENGTH  = sizeof(posComponent)      + 
                                        sizeof(normalComponent)   + 
                                        sizeof(texcoordComponent);
  
  posComponent      position;
  normalComponent   normal;
  texcoordComponent texCoord;

  vertex_t()
  {
    position  = {0.0f, 0.0f, 0.0f}; 
    normal    = {0.0f, 0.0f, 0.0f}; 
    texCoord  = {0.0f, 0.0f}; 
  } 

  vertex_t(posComponent pos, normalComponent nor, texcoordComponent tc)
    : position{pos}, normal{nor}, texCoord{tc} 
  { 
  }
  
  vertex_t(std::array<float, vertex_t::VERTEX_COMPONENTS> l)
  {
    position = {l[0], l[1], l[2]};
    normal   = {l[3], l[4], l[5]};
    texCoord = {l[6], l[7]};
  }

  void print()
  {
    spdlog::info("({},{},{}), ({},{},{}), ({},{})", position.x,position.y,position.z,
                                                    normal.x,normal.y,normal.z,
                                                    texCoord.x,texCoord.y);
  }
};

#endif
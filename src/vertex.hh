#ifndef VERTEX_HH
#define VERTEX_HH

#include "core.hh"

struct Vertex
{
  using posComponent      = vec3f;  
  using normalComponent   = vec3f;  
  using texcoordComponent = vec2f;  
  
  static constexpr int VERTEX_COMPONENTS =  posComponent::length()    +   // 3
                                            normalComponent::length() +   // 3
                                            texcoordComponent::length();  // 2
  
  static constexpr int VERTEX_LENGTH  = sizeof(posComponent)      +  // 12bytes
                                        sizeof(normalComponent)   +  // 12bytes
                                        sizeof(texcoordComponent);   // 8bytes
  
  posComponent      position;
  normalComponent   normal;
  texcoordComponent texCoord;

  Vertex()
  {
    position  = { 0.0f,0.0f,0.0f }; 
    normal    = { 0.0f,0.0f,0.0f }; 
    texCoord  = { 0.0f,0.0f };
  } 

  Vertex(posComponent pos, normalComponent nor, texcoordComponent tc)
    : position{pos}, normal{nor}, texCoord{tc} 
  { 
  }
  
  Vertex(array<float, Vertex::VERTEX_COMPONENTS> l)
  {
    position = { l[0],l[1],l[2] };
    normal   = { l[3],l[4],l[5] };
    texCoord = { l[6],l[7] };
  }
};

#endif
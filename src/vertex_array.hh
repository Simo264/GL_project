#ifndef VERTEX_ARRAY_HH
#define VERTEX_ARRAY_HH

#include "glad/glad.h"

class VertexArray
{


public:
  VertexArray();
  ~VertexArray() = default;

  void bind() const;
  void unbind() const;
  void destroy();
  
  void vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset);
  
  void enableAttribute(uint32_t index);

private:
  uint32_t mVertexArray;

};

#endif
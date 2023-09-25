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
  void attribBinding(uint32_t attribindex, uint32_t bindingindex);
  void bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride);
  void enableAttribute(uint32_t index);

  uint32_t get() const; 

private:
  uint32_t mVertexArray;

};

#endif
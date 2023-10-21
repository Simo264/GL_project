#ifndef VERTEX_ARRAY_HH
#define VERTEX_ARRAY_HH

#include "glad/glad.h"
#include "vertex_buffer.hh"

class VertexArray
{
public:
  VertexArray(VertexBuffer* vBuffer);
  ~VertexArray() = default;

  void bind() const { glBindVertexArray(_vertexArray);}
  void unbind() const { glBindVertexArray(0);}
  void destroy();
  
  void vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset);
  void attribBinding(uint32_t attribindex, uint32_t bindingindex);
  void bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride);
  void enableAttribute(uint32_t index);
  void disableAttribute(uint32_t index);

  uint32_t get() const { return _vertexArray; }

private:
  uint32_t _vertexArray;
};

#endif
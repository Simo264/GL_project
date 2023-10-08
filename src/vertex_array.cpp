#include "vertex_array.hh"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &_vertexArray);
  glBindVertexArray(_vertexArray);
}

void VertexArray::vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset)
{
  glVertexArrayAttribFormat(_vertexArray, index, size, type, GL_FALSE, offset);
}

void VertexArray::attribBinding(uint32_t attribindex, uint32_t bindingindex)
{
  glVertexArrayAttribBinding(_vertexArray, attribindex, bindingindex);
}

void VertexArray::bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride)
{
  glVertexArrayVertexBuffer(_vertexArray, bindingindex, buffer, offset, stride);
}

void VertexArray::enableAttribute(uint32_t index)
{
  glEnableVertexArrayAttrib(_vertexArray, index);
}

void VertexArray::bind() const
{
  glBindVertexArray(_vertexArray);
}

void VertexArray::unbind() const
{
 glBindVertexArray(0);
}

void VertexArray::destroy()
{
  glDeleteVertexArrays(1, &_vertexArray);
}

uint32_t VertexArray::get() const
{
  return _vertexArray;
}

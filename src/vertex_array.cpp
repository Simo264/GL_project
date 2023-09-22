#include "vertex_array.hh"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &mVertexArray);
  glBindVertexArray(mVertexArray);
}

void VertexArray::vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset)
{
  glVertexArrayAttribFormat(mVertexArray, index, size, type, GL_FALSE, offset);
}

void VertexArray::attribBinding(uint32_t attribindex, uint32_t bindingindex)
{
  glVertexArrayAttribBinding(mVertexArray, attribindex, bindingindex);
}

void VertexArray::bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride)
{
  glVertexArrayVertexBuffer(mVertexArray, bindingindex, buffer, offset, stride);
}

void VertexArray::enableAttribute(uint32_t index)
{
  glEnableVertexArrayAttrib(mVertexArray, index);
}

void VertexArray::bind() const
{
  glBindVertexArray(mVertexArray);
}

void VertexArray::unbind() const
{
 glBindVertexArray(0);
}

void VertexArray::destroy()
{
  glDeleteVertexArrays(1, &mVertexArray);
}

uint32_t VertexArray::get()
{
  return mVertexArray;
}

#include "vertex_array.hh"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &mVertexArray);
  glBindVertexArray(mVertexArray);
}

void VertexArray::vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset)
{
  glVertexArrayAttribFormat(mVertexArray, index, size, type, GL_FALSE, offset);
  //glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) offset); 
}

void VertexArray::enableAttribute(uint32_t index)
{
  glEnableVertexAttribArray(index);
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



#include "vertex_array.hh"
#include "spdlog/spdlog.h"

VertexArray::VertexArray(VertexBuffer& vBuffer)
{
  glGenVertexArrays(1, &_vertexArray);
  if(_vertexArray == GL_INVALID_VALUE)
  {
    spdlog::warn("VertexArray object is GL_INVALID_VALUE");
  }
  else
  {
    glBindVertexArray(_vertexArray);
    // 0 -> position
    vertexArray.vertexSpecification(0, 3, GL_FLOAT, 0); 
    vertexArray.bindBuffer(0, vBuffer.get(), 0, VERTEX_LENGTH);
    vertexArray.attribBinding(0, 0);
    vertexArray.enableAttribute(0);
    // 1 -> texture
    vertexArray.vertexSpecification(1, 2, GL_FLOAT, 0); 
    vertexArray.bindBuffer(1, vBuffer.get(), 12, VERTEX_LENGTH);
    vertexArray.attribBinding(1, 1);
    vertexArray.enableAttribute(1);
  }
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

void VertexArray::destroy()
{
  if(_vertexArray != GL_INVALID_VALUE)
    glDeleteVertexArrays(1, &_vertexArray);
}

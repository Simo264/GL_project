#include "vertex_array.hh"
#include "spdlog/spdlog.h"

#include "vertex.hh"

VertexArray::VertexArray(VertexBuffer* vBuffer)
{
  glGenVertexArrays(1, &_vertexArray);
  if(_vertexArray == GL_INVALID_VALUE)
  {
    spdlog::error("VertexArray object is GL_INVALID_VALUE");
  }
  else
  {
    glBindVertexArray(_vertexArray);
    
    int offset;
    int components;

    // 0 -> position (x,y,z)
    offset     = 0;
    components = vertex_t::posComponent::length();
    vertexSpecification(0, components, GL_FLOAT, offset); 
    bindBuffer(0, vBuffer->get(), 0, vertex_t::VERTEX_LENGTH);
    attribBinding(0, 0);
    enableAttribute(0);

    // 1 -> normal (x,y,z)
    offset     = sizeof(vertex_t::posComponent);
    components = vertex_t::normalComponent::length();
    vertexSpecification(1, components, GL_FLOAT, offset); 
    bindBuffer(1, vBuffer->get(), 0, vertex_t::VERTEX_LENGTH);
    attribBinding(1, 1);
    enableAttribute(1);
    
    // 2 -> texture (u,v)
    offset     = sizeof(vertex_t::posComponent) + sizeof(vertex_t::normalComponent);
    components = vertex_t::texcoordComponent::length();
    vertexSpecification(2, components, GL_FLOAT, offset); 
    bindBuffer(2, vBuffer->get(), 0, vertex_t::VERTEX_LENGTH);
    attribBinding(2, 2);
    enableAttribute(2);
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

void VertexArray::disableAttribute(uint32_t index)
{
  glDisableVertexArrayAttrib(_vertexArray, index);
}

void VertexArray::destroy()
{
  if(_vertexArray != GL_INVALID_VALUE)
    glDeleteVertexArrays(1, &_vertexArray);
}

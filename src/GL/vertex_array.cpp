#include "vertex_array.hh"

#include "vertex.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/
namespace GL
{
  VertexArray::VertexArray(VAConfiguration& config, VertexBuffer& vBuffer)
  {
    init(vBuffer);
  }

  void VertexArray::init(VAConfiguration& config, VertexBuffer& vBuffer)
  {
    glGenVertexArrays(1, &_vertexArray);

    bind();
  #if 0
    int offset;
    int components;

    // 0 -> position (x,y,z)
    offset     = 0;
    components = Vertex::posComponent::length();
    vertexSpecification(0, components, GL_FLOAT, offset); 
    bindBuffer(0, vBuffer.get(), 0, Vertex::VERTEX_LENGTH);
    attribBinding(0, 0);
    enableAttribute(0);

    // 1 -> normal (x,y,z)
    offset     = sizeof(Vertex::posComponent);
    components = Vertex::normalComponent::length();
    vertexSpecification(1, components, GL_FLOAT, offset); 
    bindBuffer(1, vBuffer.get(), 0, Vertex::VERTEX_LENGTH);
    attribBinding(1, 1);
    enableAttribute(1);
    
    // 2 -> texture (u,v)
    offset     = sizeof(Vertex::posComponent) + sizeof(Vertex::normalComponent);
    components = Vertex::texcoordComponent::length();
    vertexSpecification(2, components, GL_FLOAT, offset); 
    bindBuffer(2, vBuffer.get(), 0, Vertex::VERTEX_LENGTH);
    attribBinding(2, 2);
    enableAttribute(2);
  #endif
  
  
  
    unbind();
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

}
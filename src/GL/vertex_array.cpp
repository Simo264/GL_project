#include "vertex_array.hh"

#include "vertex.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/
namespace GL
{
  VertexArray::VertexArray(VAConfiguration& config, VertexBuffer& vBuffer)
  {
    init(config, vBuffer);
  }

  void VertexArray::init(VAConfiguration& config, VertexBuffer& vBuffer)
  {
    glGenVertexArrays(1, &_vertexArray);

    bind();

    int stride = std::accumulate(config.layout.begin(), config.layout.end(), 0);
    stride *= 4;

    int offset = 0;
    for(uint32_t i = 0; config.layout[i] != 0 && i < config.layout.size(); i++)
    {
      vertexSpecification(i, config.layout[i], GL_FLOAT, offset);
      bindBuffer(i, vBuffer.get(), 0, stride);
      attribBinding(i, i);
      enableAttribute(i);
      offset += config.layout[i] * sizeof(float);
    }

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
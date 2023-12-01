#include "vertex_array.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/
namespace GL
{
  // VAConfiguration
  // ------------------------------------------------
  VAConfiguration::VAConfiguration()
  {
    layout.fill(0);
    numAttrs = 0;
  }

  void VAConfiguration::pushAttribute(uint32_t attribute)
  {
    if(numAttrs >= layout.size())
    {
      spdlog::error("VAConfiguration::pushAttribute can't push more attributes");
      return;
    }

    layout[numAttrs++] = attribute;
  }

  // VertexArray
  // ------------------------------------------------
  void VertexArray::init(VAConfiguration& config, VertexBuffer& vBuffer)
  {
    glGenVertexArrays(1, &vertexArrayID);

    bind();

    int stride = std::accumulate(config.layout.begin(), config.layout.end(), 0);
    stride *= 4;

    int offset = 0;
    for(uint32_t i = 0; i < config.numAttrs; i++)
    {
      vertexSpecification(i, config.layout[i], GL_FLOAT, offset);
      bindBuffer(i, vBuffer.bufferID, 0, stride);
      attribBinding(i, i);
      enableAttribute(i);
      offset += config.layout[i] * 4; // config.layout[i] * sizeof(float)
    }

    unbind();
  }

  void VertexArray::vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset)
  {
    glVertexArrayAttribFormat(vertexArrayID, index, size, type, GL_FALSE, offset);
  }

  void VertexArray::attribBinding(uint32_t attribindex, uint32_t bindingindex)
  {
    glVertexArrayAttribBinding(vertexArrayID, attribindex, bindingindex);
  }

  void VertexArray::bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride)
  {
    glVertexArrayVertexBuffer(vertexArrayID, bindingindex, buffer, offset, stride);
  }

  void VertexArray::enableAttribute(uint32_t index)
  {
    glEnableVertexArrayAttrib(vertexArrayID, index);
  }

  void VertexArray::disableAttribute(uint32_t index)
  {
    glDisableVertexArrayAttrib(vertexArrayID, index);
  }

}
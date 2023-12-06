#include "element_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

namespace GL
{
  void ElementBuffer::init(uint64_t size, uint32_t* data)
  {
    glGenBuffers(1, &bufferID);

    bind();

    glNamedBufferData(bufferID, size, data, GL_STATIC_DRAW);

    nIndices = size / 4; // size / sizeof(uint32_t)
  }
}
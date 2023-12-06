#include "vertex_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

namespace GL
{
  void VertexBuffer::init(uint64_t size, float* rawdata)
  {
    glGenBuffers(1, &bufferID);

    bind();

    glNamedBufferData(bufferID, size, rawdata, GL_STATIC_DRAW);
  }
}
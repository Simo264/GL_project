#include "vertex_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

namespace GL
{
  VertexBuffer::VertexBuffer(uint64_t size, float* rawdata, int usage)
  {
    init(size, rawdata, usage);
  }

  void VertexBuffer::init(uint64_t size, float* rawdata, int usage)
  {
    glGenBuffers(1, &_buffer);

    bind();

    glNamedBufferData(_buffer, size, rawdata, usage);
  }
}
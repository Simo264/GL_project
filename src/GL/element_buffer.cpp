#include "element_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

namespace GL
{
  ElementBuffer::ElementBuffer(uint64_t size, uint32_t* data, int usage)
  {
    init(size, data, usage);
  }

  void ElementBuffer::init(uint64_t size, uint32_t* data, int usage)
  {
    glGenBuffers(1, &_buffer);

    bind();

    glNamedBufferData(_buffer, size, data, usage);

    _nIndices = size / 4; // size / sizeof(uint32_t)
  }
}
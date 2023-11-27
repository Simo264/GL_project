#include "element_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

namespace GL
{
  ElementBuffer::ElementBuffer(uint64_t nIndices, uint32_t* data, int usage)
  {
    init(nIndices, data, usage);
  }

  void ElementBuffer::init(uint64_t nIndices, uint32_t* data, int usage)
  {
    glGenBuffers(1, &_buffer);

    bind();

    uint64_t size = nIndices * sizeof(uint32_t); 
    glNamedBufferData(_buffer, size, data, usage);

    _nIndices = nIndices;
  }

  void ElementBuffer::destroy() 
  { 
    if(_buffer)
      glDeleteBuffers(1, &_buffer);
  }

}
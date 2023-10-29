#include "element_buffer.hh"
#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

ElementBuffer::ElementBuffer(uint64_t nIndices, uint32_t* data, int usage)
{
  glGenBuffers(1, &_buffer);

  bind();

  uint64_t size = nIndices * sizeof(uint32_t); 
  glNamedBufferData(_buffer, size, data, usage);
}

void ElementBuffer::destroy() 
{ 
  if(_buffer)
    glDeleteBuffers(1, &_buffer);
}

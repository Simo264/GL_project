#include "element_buffer.hh"
#include "spdlog/spdlog.h"

ElementBuffer::ElementBuffer(uint64_t nIndices, IndexDataType* data, int usage)
{
  glGenBuffers(1, &_buffer);
  if(_buffer == GL_INVALID_VALUE)
  {
    spdlog::error("ElementBuffer object is GL_INVALID_VALUE");
  } 
  else
  {
    uint64_t size = nIndices * sizeof(IndexDataType); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
    glNamedBufferData(_buffer, size, data, usage);
  }
}

void ElementBuffer::destroy() 
{ 
  if(_buffer != GL_INVALID_VALUE)
    glDeleteBuffers(1, &_buffer);
}

#include "vertex_buffer.hh"
#include "spdlog/spdlog.h"

VertexBuffer::VertexBuffer(uint32_t size, float* data, uint32_t usage)
{
  glGenBuffers(1, &_buffer);
  if(_buffer == GL_INVALID_VALUE)
  {
    spdlog::error("VertexBuffer object is GL_INVALID_VALUE");
  } 
  else
  {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glNamedBufferData(_buffer, size, data, usage);
  }
}

void VertexBuffer::destroy() 
{ 
  if(_buffer != GL_INVALID_VALUE)
    glDeleteBuffers(1, &_buffer);
}

#include "vertex_buffer.hh"

VertexBuffer::VertexBuffer(uint32_t size, float* data, uint32_t usage)
{
  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  glNamedBufferData(_buffer, size, data, usage);
}

void VertexBuffer::bind() const
{ 
  glBindBuffer(GL_ARRAY_BUFFER, _buffer); 
}

void VertexBuffer::unbind() const
{ 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::destroy() 
{ 
  glDeleteBuffers(1, &_buffer);
}

uint32_t VertexBuffer::get() const
{ 
  return _buffer;
}
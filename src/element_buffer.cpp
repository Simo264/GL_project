#include "element_buffer.hh"

ElementBuffer::ElementBuffer(uint32_t size, uint32_t* data, uint32_t usage)
{
  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
  glNamedBufferData(_buffer, size, data, usage);
}

void ElementBuffer::bind() const
{ 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer); 
}

void ElementBuffer::unbind() const
{ 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

void ElementBuffer::destroy() 
{ 
  glDeleteBuffers(1, &_buffer);
}

uint32_t ElementBuffer::get() const
{ 
  return _buffer; 
}
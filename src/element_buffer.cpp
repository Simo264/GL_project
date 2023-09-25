#include "element_buffer.hh"

ElementBuffer::ElementBuffer(uint32_t size, uint32_t* data, uint32_t usage)
{
  glGenBuffers(1, &mBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
  glNamedBufferData(mBuffer, size, data, usage);
}

void ElementBuffer::bind() const
{ 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer); 
}

void ElementBuffer::unbind() const
{ 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

void ElementBuffer::destroy() 
{ 
  glDeleteBuffers(1, &mBuffer);
}

uint32_t ElementBuffer::get() const
{ 
  return mBuffer; 
}
#include "vertex_buffer.hh"

VertexBuffer::VertexBuffer(uint32_t size, float* data, uint32_t usage)
{
  glGenBuffers(1, &mBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
  glNamedBufferData(mBuffer, size, data, usage);
}

void VertexBuffer::bind() 
{ 
  glBindBuffer(GL_ARRAY_BUFFER, mBuffer); 
}

void VertexBuffer::unbind() 
{ 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::destroy() 
{ 
  glDeleteBuffers(1, &mBuffer);
}

uint32_t VertexBuffer::get() 
{ 
  return mBuffer;
}
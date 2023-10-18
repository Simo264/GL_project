#include "vertex_buffer.hh"
#include "spdlog/spdlog.h"

VertexBuffer::VertexBuffer(uint64_t size, float* rawdata, int usage)
{
  glGenBuffers(1, &_buffer);
  if(_buffer == GL_INVALID_VALUE)
  {
    spdlog::error("VertexBuffer object is GL_INVALID_VALUE");
  } 
  else
  {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glNamedBufferData(_buffer, size, rawdata, usage);
  }
}

VertexBuffer::VertexBuffer(uint64_t nVertices, vertex_t* vertices, int usage)
{
  glGenBuffers(1, &_buffer);
  if(_buffer == GL_INVALID_VALUE)
  {
    spdlog::error("VertexBuffer object is GL_INVALID_VALUE");
  } 
  else
  {
    uint64_t size = nVertices * sizeof(vertex_t);
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glNamedBufferData(_buffer, size, (void*) vertices, usage);
  }
}

void VertexBuffer::destroy() 
{ 
  if(_buffer != GL_INVALID_VALUE)
    glDeleteBuffers(1, &_buffer);
}

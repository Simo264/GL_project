#include "vertex_buffer.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

VertexBuffer::VertexBuffer(uint64_t size, float* rawdata, int usage)
{
  glGenBuffers(1, &_buffer);

  bind();

  glNamedBufferData(_buffer, size, rawdata, usage);
}

VertexBuffer::VertexBuffer(uint64_t nVertices, Vertex* vertices, int usage)
{
  glGenBuffers(1, &_buffer);
  
  bind();

  uint64_t size = nVertices * sizeof(Vertex);
  glNamedBufferData(_buffer, size, (void*) vertices, usage);
}

void VertexBuffer::destroy() 
{ 
  if(_buffer)
    glDeleteBuffers(1, &_buffer);
}

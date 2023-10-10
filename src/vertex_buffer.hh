#ifndef VERTEX_BUFFER_HH
#define VERTEX_BUFFER_HH

#include "glad/glad.h"

class VertexBuffer
{
public:
  VertexBuffer(uint32_t size, float* data, uint32_t usage = GL_STATIC_DRAW);
  ~VertexBuffer() = default;

  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, _buffer); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
  void destroy();

  uint32_t get() const { return _buffer; }
  
private:
  uint32_t _buffer;
};

#endif
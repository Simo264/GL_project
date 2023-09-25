#ifndef VERTEX_BUFFER_HH
#define VERTEX_BUFFER_HH

#include "glad/glad.h"

class VertexBuffer
{
public:
  VertexBuffer(uint32_t size, float* data, uint32_t usage = GL_STATIC_DRAW);
  ~VertexBuffer() = default;

  void bind() const;
  void unbind() const;
  void destroy();

  uint32_t get() const; 
  
private:
  uint32_t mBuffer;
};

#endif
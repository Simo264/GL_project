#ifndef ELEMENT_BUFFER_HH
#define ELEMENT_BUFFER_HH

#include "glad/glad.h"

class ElementBuffer
{
public:
  ElementBuffer(uint32_t size, uint32_t* data, uint32_t usage = GL_STATIC_DRAW);
  ~ElementBuffer() = default;

  void bind() const;
  void unbind() const;
  void destroy();

  uint32_t get() const;

private:
  uint32_t mBuffer;
};

#endif
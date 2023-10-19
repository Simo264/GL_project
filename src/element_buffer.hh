#ifndef ELEMENT_BUFFER_HH
#define ELEMENT_BUFFER_HH

#include "glad/glad.h"

class ElementBuffer
{
public:
  ElementBuffer(uint64_t nIndices, uint32_t* data, int usage = GL_STATIC_DRAW);
  ~ElementBuffer() = default;

  void bind() const   { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer); }
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
  void destroy();

  uint32_t get() const { return _buffer; }

private:
  uint32_t _buffer;
};

#endif
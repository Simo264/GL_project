#ifndef ELEMENT_BUFFER_HH
#define ELEMENT_BUFFER_HH

#include "../core.hh"

namespace GL
{
  class ElementBuffer
  {
  public:
    ElementBuffer() = default;
    ElementBuffer(uint64_t nIndices, uint32_t* data, int usage = GL_STATIC_DRAW);
    ~ElementBuffer() = default;
    
    ElementBuffer(const ElementBuffer&) = delete;            // delete copy constructor
    ElementBuffer& operator=(const ElementBuffer&) = delete; // delete assign op

    void init(uint64_t nIndices, uint32_t* data, int usage = GL_STATIC_DRAW);
    
    void bind() const   { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
    void destroy();

    uint32_t get() const { return _buffer; }

    uint32_t numIndices() const { return _nIndices; }

  private:
    uint32_t _buffer;
    uint32_t _nIndices; 
  };
}
#endif
#ifndef ELEMENT_BUFFER_HH
#define ELEMENT_BUFFER_HH

#include "../core.hh"

namespace GL
{
  class ElementBuffer
  {
  public:
    ElementBuffer() = default;
    ~ElementBuffer() = default;
    
    ElementBuffer(const ElementBuffer&) = delete;            // delete copy constructor
    ElementBuffer& operator=(const ElementBuffer&) = delete; // delete assign op

    void init(uint64_t size, uint32_t* data);
    
    void bind() const   { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
    void destroy()      { glDeleteBuffers(1, &bufferID); }

    uint32_t bufferID;
    uint32_t nIndices; 
  };
}
#endif
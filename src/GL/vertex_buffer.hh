#ifndef VERTEX_BUFFER_HH
#define VERTEX_BUFFER_HH

#include "../core.hh"

#include "vertex.hh"

namespace GL
{
  class VertexBuffer
  {
  public:
    VertexBuffer() = default;
    VertexBuffer(uint64_t size, float* rawdata, int usage = GL_STATIC_DRAW);
    ~VertexBuffer() = default;

    VertexBuffer(const VertexBuffer&) = delete;            // delete copy constructor
    VertexBuffer& operator=(const VertexBuffer&) = delete; // delete assign op

    void init(uint64_t size, float* rawdata, int usage = GL_STATIC_DRAW);

    void bind() const   { glBindBuffer(GL_ARRAY_BUFFER, _buffer); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    void destroy()      { glDeleteBuffers(1, &_buffer); }

    uint32_t get() const { return _buffer; }
    
  private:
    uint32_t _buffer;
  };

}
#endif
#ifndef VERTEX_BUFFER_HH
#define VERTEX_BUFFER_HH

#include "../core.hh"

namespace GL
{
  class VertexBuffer
  {
  public:
    VertexBuffer() = default;
    ~VertexBuffer() = default;

    VertexBuffer(const VertexBuffer&) = delete;            // delete copy constructor
    VertexBuffer& operator=(const VertexBuffer&) = delete; // delete assign op

    void init(uint64_t size, float* rawdata);

    void bind() const   { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
    void destroy()      { glDeleteBuffers(1, &bufferID); }

    uint32_t bufferID;
  };

}
#endif
#ifndef VERTEX_ARRAY_HH
#define VERTEX_ARRAY_HH

#include "../core.hh"

#include "vertex_buffer.hh"

namespace GL
{
  enum class VALayout : uint32_t
  {
    POSITION  = 1,
    NORMALS   = 2,
    TEXTCOORDS= 3
  };

  struct VAConfiguration
  {
    // ex: mapAttributes = {<POSITION,3>,<NORMALS,3>,<TEXTCOORDS,2>}
    map<VALayout, uint32_t> mapLayout;

  };


  class VertexArray
  {
  public:
    VertexArray() = default;
    VertexArray(VAConfiguration& config, VertexBuffer& vBuffer);
    ~VertexArray() = default;

    VertexArray(const VertexArray&) = delete;            // delete copy constructor
    VertexArray& operator=(const VertexArray&) = delete; // delete assign op

    void init(VAConfiguration& config, VertexBuffer& vBuffer);

    void bind() const   { glBindVertexArray(_vertexArray);}
    void unbind() const { glBindVertexArray(0);}
    void destroy()      { glDeleteVertexArrays(1, &_vertexArray); }
    
    void vertexSpecification(uint32_t index, uint32_t size, uint32_t type, int offset);
    void attribBinding(uint32_t attribindex, uint32_t bindingindex);
    void bindBuffer(uint32_t bindingindex, uint32_t buffer, int offset, int stride);
    void enableAttribute(uint32_t index);
    void disableAttribute(uint32_t index);

    uint32_t get() const { return _vertexArray; }

  private:
    uint32_t _vertexArray;
  };
}
#endif
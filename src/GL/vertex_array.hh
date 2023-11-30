#ifndef VERTEX_ARRAY_HH
#define VERTEX_ARRAY_HH

#include "../core.hh"

#include "vertex_buffer.hh"

namespace GL
{
  struct VAConfiguration
  {
    // ex: layout = [3, 3, 2]
    // i=0: position   3 components (x,y,z)
    // i=1: normals    3 components (x,y,z)
    // i=2: textcoords 2 components (x,y)
    vector<uint32_t> layout; 

    VAConfiguration()
    {
      layout.reserve(5); // default size 5
    }
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
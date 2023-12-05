#ifndef INTERFACE_MESH_HH
#define INTERFACE_MESH_HH

#include "core.hh"

#include "GL/vertex_buffer.hh"
#include "GL/element_buffer.hh"
#include "GL/vertex_array.hh"

class IMesh
{
public:
  IMesh() = default;
  ~IMesh() = default;

  IMesh(const IMesh&) = delete;             // delete copy constructor
  IMesh& operator=(const IMesh&) = delete;  // delete assign op

  virtual void draw() = 0;
  virtual void preDraw() = 0;
  virtual void postDraw() = 0;
  
  void destroy()
  {
    _vertexArray.destroy();
    _vertexBuffer.destroy();
    _elementBuffer.destroy();
  }

  GL::VertexBuffer&  VBO() { return _vertexBuffer; }
  GL::ElementBuffer& EBO() { return _elementBuffer; }
  GL::VertexArray&   VAO() { return _vertexArray; }

protected:
  GL::VertexBuffer  _vertexBuffer;
  GL::ElementBuffer _elementBuffer;
  GL::VertexArray   _vertexArray;
};
#endif

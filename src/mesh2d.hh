#ifndef MESH2D_HH
#define MESH2D_HH

#include "core.hh"

#include "GL/vertex_buffer.hh"
#include "GL/element_buffer.hh"
#include "GL/vertex_array.hh"

#include "texture2d.hh"

class Mesh2D
{
public:
  Mesh2D();
  ~Mesh2D() = default;

  Mesh2D(const Mesh2D&) = delete;             // delete copy constructor
  Mesh2D& operator=(const Mesh2D&) = delete;  // delete assign op

  void draw(uint32_t drawmode = GL_TRIANGLES);
  void destroy();

  Texture2D* texture;

private:
  GL::VertexBuffer  _vertexBuffer;
  GL::ElementBuffer _elementBuffer;
  GL::VertexArray   _vertexArray;

};




#endif
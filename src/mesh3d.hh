#ifndef MESH3D_HH
#define MESH3D_HH

#include "core.hh"

#include "GL/vertex_buffer.hh"
#include "GL/element_buffer.hh"
#include "GL/vertex_array.hh"

#include "texture2d.hh"


// A mesh represents a single drawable 3d entity
// ------------------------------------------------
class Mesh3D
{
public:
  Mesh3D() = default;
  ~Mesh3D() = default;

  Mesh3D(const Mesh3D&) = delete;             // delete copy constructor
  Mesh3D& operator=(const Mesh3D&) = delete;  // delete assign op

  void init(vector<float>& vertices, vector<uint32_t>& indices);
  void draw(uint32_t drawmode); // GL_TRIANGLES | GL_LINE_STRIP
  void destroy();

  Texture2D* diffuse;
  Texture2D* normal;
  Texture2D* specular;

private:
  GL::VertexBuffer  _vertexBuffer;
  GL::ElementBuffer _elementBuffer;
  GL::VertexArray   _vertexArray;
};

#endif
#ifndef MESH_HH
#define MESH_HH

#include "core.hh"

#include "GL/vertex.hh"
#include "GL/vertex_buffer.hh"
#include "GL/element_buffer.hh"
#include "GL/vertex_array.hh"

#include "texture.hh"
#include "shader.hh"

#include "lighting/material.hh"

// A mesh represents a single drawable entity
// ------------------------------------------------
class Mesh
{
public:
  Mesh() = default;
  Mesh(vector<GL::Vertex>& vertices, vector<uint32_t>& indices);

  ~Mesh() = default;

  Mesh(const Mesh&) = delete;             // delete copy constructor
  Mesh& operator=(const Mesh&) = delete;  // delete assign op

  void init(vector<GL::Vertex>& vertices, vector<uint32_t>& indices);

  void draw(Shader* shader, uint32_t drawmode); // GL_TRIANGLES | GL_LINE_STRIP

  void destroy();

  Texture* diffuse;
  Texture* normal;
  Texture* specular;

private:
  GL::VertexBuffer  _vertexBuffer;
  GL::ElementBuffer _elementBuffer;
  GL::VertexArray   _vertexArray;
};

#endif
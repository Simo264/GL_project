#ifndef MESH_HH
#define MESH_HH

#include "core.hh"

#include "vertex.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"

#include "texture.hh"
#include "shader.hh"

#include "lighting/material.hh"

// A mesh represents a single drawable entity
// ------------------------------------------------
class Mesh
{
public:
  Mesh() = default;
  Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices);
  Mesh(const Mesh&) = delete;             // delete copy constructor
  Mesh& operator=(const Mesh&) = delete;  // delete assign op

  ~Mesh() = default;

  void draw(Shader* shader, uint32_t drawmode); // GL_TRIANGLES | GL_LINE_STRIP

  void destroy();

  Texture* diffuse;
  Texture* normal;
  Texture* specular;

private:
  unique_ptr<char[]> _ptrData;
  VertexBuffer*      _vertexBuffer;
  ElementBuffer*     _elementBuffer;
  VertexArray*       _vertexArray;
  
  vector<uint32_t>          _indices;
};

#endif
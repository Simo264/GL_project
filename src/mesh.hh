#ifndef MESH_HH
#define MESH_HH

#include "core.hh"

#include "vertex.hh"
#include "texture.hh"
#include "shader.hh"

#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"

#include "lighting/material.hh"

// A mesh represents a single drawable entity
// ------------------------------------------------
class Mesh
{
public:
  Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices);
  ~Mesh() = default;

  void draw(Shader* shader, uint32_t drawmode); // GL_TRIANGLES | GL_LINE_STRIP

  void destroy();

  Texture* diffuse;
  Texture* normal;
  Texture* specular;

private:
  vector<uint32_t> _indices;

  unique_ptr<VertexBuffer>  _vertexBuffer;
  unique_ptr<ElementBuffer> _elementBuffer;
  unique_ptr<VertexArray>   _vertexArray;
};

#endif
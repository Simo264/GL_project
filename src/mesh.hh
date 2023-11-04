#ifndef MESH_HH
#define MESH_HH

#include "core.hh"

#include "vertex.hh"
#include "texture.hh"
#include "shader.hh"

#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"


// A mesh represents a single drawable entity
// ------------------------------------------------
class Mesh
{
public:
  Mesh(vector<vertex_t>& vertices, vector<uint32_t>& indices, vector<Texture*>& textures);
  ~Mesh() = default;

  void draw(Shader* shader);

  void destroy();

private:
  vector<uint32_t> _indices;
  vector<Texture*> _textures;

  uint32_t _drawMode;

  unique_ptr<VertexBuffer>  _vertexBuffer;
  unique_ptr<ElementBuffer> _elementBuffer;
  unique_ptr<VertexArray>   _vertexArray;
};

#endif
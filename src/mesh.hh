#ifndef MESH_HH
#define MESH_HH

#include "vertex.hh"
#include "texture.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"

#include <vector>
#include <memory>

// A mesh represents a single drawable entity
// ------------------------------------------------
class Mesh
{
public:
  Mesh(std::vector<vertex_t>& vertices, std::vector<uint32_t>& indices, std::vector<Texture*>& textures);
  ~Mesh() = default;

  void draw(Shader* shader);

  void destroy();

private:
  // mesh data
  std::vector<uint32_t> _indices;
  std::vector<Texture*> _textures;

  std::unique_ptr<VertexBuffer>  _vertexBuffer;
  std::unique_ptr<ElementBuffer> _elementBuffer;
  std::unique_ptr<VertexArray>   _vertexArray;
};

#endif
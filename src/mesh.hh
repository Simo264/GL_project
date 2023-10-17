#ifndef MESH_HH
#define MESH_HH

#include "vertex.hh"
#include "texture.hh"
#include "shader.hh"
#include "vertex_buffer.hh"
#include "element_buffer.hh"
#include "vertex_array.hh"

#include <vector>

class Mesh
{
public:
  Mesh(std::vector<vertex_t> vertices, std::vector<uint64_t> indices, std::vector<Texture> textures);
  ~Mesh() = default;

  void draw(Shader& shader);

  void destroy();

  VertexBuffer*   getVertexBuffer()   const  { return _vertexBuffer; }
  ElementBuffer*  getElementBuffer()  const  { return _elementBuffer; }
  VertexArray*    getVertexArray()    const  { return _vertexArray; }

private:
  // mesh data
  std::vector<vertex_t>   _vertices;
  std::vector<uint64_t>   _indices;
  std::vector<Texture>    _textures;

  VertexBuffer*    _vertexBuffer;
  ElementBuffer*   _elementBuffer;
  VertexArray*     _vertexArray;

  void setupMesh();
};





#endif
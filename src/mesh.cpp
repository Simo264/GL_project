#include "mesh.hh"

#include <string>

Mesh::Mesh(std::vector<vertex_t> vertices, std::vector<uint64_t> indices, std::vector<Texture*> textures)
{
  _vertices = vertices;
  _indices  = indices;
  _textures = textures;

  setupMesh();
}

void Mesh::setupMesh()
{
  _vertexBuffer   = new VertexBuffer(_vertices.size(), _vertices.data());
  
  if(!_indices.empty())
    _elementBuffer  = new ElementBuffer(_indices.size(), _indices.data());
  
  _vertexArray    = new VertexArray(*_vertexBuffer);

  _vertexArray->unbind();
}

void Mesh::draw(Shader& shader)
{
  // bind textures
  // for (auto& [texture, unit] : _textures)
  // {
  //   Texture::activeTextUnit(unit);
  //   texture->bind();
  // }

  Texture::activeTextUnit(0);
  _textures[0]->bind();

  Texture::activeTextUnit(1);
  _textures[1]->bind();

  shader.setInt("some_attribute", 0);

  // draw mesh
  _vertexArray->bind();
  
  if(!_indices.empty())
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
  
  _vertexArray->unbind();
}

void Mesh::destroy()
{
  _vertexBuffer->destroy();
  _elementBuffer->destroy();
  _vertexArray->destroy();

  delete _vertexArray;
  delete _vertexBuffer;
  delete _elementBuffer;
}
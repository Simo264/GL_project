#include "mesh.hh"

#include "spdlog/spdlog.h"

#include <string>

Mesh::Mesh(std::vector<vertex_t>& vertices, std::vector<uint32_t>& indices, std::vector<Texture*>& textures)
{
  _indices  = indices;
  _textures = textures;

  _vertexBuffer   = std::make_unique<VertexBuffer>(vertices.size(), vertices.data());
  _elementBuffer  = std::make_unique<ElementBuffer>(indices.size(), indices.data());
  _vertexArray    = std::make_unique<VertexArray>(_vertexBuffer.get());
  _vertexArray.get()->unbind();
}

void Mesh::draw(Shader* shader)
{
  (void) shader;

  for(uint32_t i = 0; i < _textures.size(); i++)
  {
    Texture::activeTextUnit(i);
    _textures[i]->bind();
    shader->setInt("texture_diffuse" + std::to_string(i+1) , i);
  }

  _vertexArray.get()->bind();
  _elementBuffer.get()->bind();

  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
  
  _elementBuffer.get()->unbind();
  _vertexArray.get()->unbind();
}

void Mesh::destroy()
{
  _vertexArray.get()->destroy();
  _vertexBuffer.get()->destroy();
  
  if(_elementBuffer)
    _elementBuffer.get()->destroy();

  for(auto& texture : _textures)
  {
    texture->destroy();
    delete texture;
  }
}


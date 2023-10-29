#include "mesh.hh"

#include "spdlog/spdlog.h"

#include <string>

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Mesh::Mesh(std::vector<vertex_t>& vertices, std::vector<uint32_t>& indices, std::vector<Texture*>& textures)
{
  _indices  = indices;
  _textures = textures;

  _drawMode = GL_TRIANGLES;
  if(_textures.empty())
    _drawMode = GL_LINE_STRIP;

  _vertexBuffer   = std::make_unique<VertexBuffer>(vertices.size(), vertices.data());
  _elementBuffer  = std::make_unique<ElementBuffer>(indices.size(), indices.data());
  _vertexArray    = std::make_unique<VertexArray>(_vertexBuffer.get());
}

void Mesh::draw(Shader* shader)
{
  for(uint32_t i = 0; i < _textures.size(); i++)
  {
    Texture::activeTextUnit(i);
    shader->setInt("texture_diffuse" + std::to_string(i+1) , i);
    _textures[i]->bind();
  }

  _vertexArray.get()->bind();
  _elementBuffer.get()->bind();
  
  glDrawElements(_drawMode, _indices.size(), GL_UNSIGNED_INT, 0);

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


#include "mesh.hh"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>

Mesh::Mesh(std::vector<vertex_t> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
{
  _vertices = vertices;
  _indices  = indices;
  _textures = textures;

  _vertexBuffer = std::make_unique<VertexBuffer>(_vertices.size(), _vertices.data());
  
  if(!_indices.empty())
    _elementBuffer = std::make_unique<ElementBuffer>(_indices.size(), _indices.data());
  
  _vertexArray = std::make_unique<VertexArray>(*_vertexBuffer.get());

  _vertexArray.get()->unbind();
}

void Mesh::load(const std::string& filename)
{
  
}

void Mesh::draw(Shader& shader)
{
  for(uint32_t i = 0; i < _textures.size(); i++)
  {
    Texture::activeTextUnit(i);
    _textures[i].bind();
    shader.setInt("texture" + std::to_string(i) , i);
  }

  _vertexArray.get()->bind();
  
  if(_indices.empty())
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
  else
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

  _vertexArray.get()->unbind();
}

void Mesh::destroy()
{
  _vertexArray.get()->destroy();
  _vertexBuffer.get()->destroy();
  
  if(_elementBuffer)
    _elementBuffer.get()->destroy();
}
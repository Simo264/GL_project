#include "mesh.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Mesh::Mesh(vector<vertex_t>& vertices, vector<uint32_t>& indices, vector<Texture*>& textures)
{
  _indices  = indices;
  _textures = textures;

  _drawMode = GL_TRIANGLES;
  if(_textures.empty())
    _drawMode = GL_LINE_STRIP;

  _vertexBuffer   = make_unique<VertexBuffer>(vertices.size(), vertices.data());
  _elementBuffer  = make_unique<ElementBuffer>(indices.size(), indices.data());
  _vertexArray    = make_unique<VertexArray>(_vertexBuffer.get());
}

void Mesh::draw(Shader* shader)
{
  for(uint32_t i = 0; i < _textures.size(); i++)
  {
    auto texture = _textures[i];
    switch (texture->type())
    {
    case TextureType::TEX_DIFFUSE:
      Texture::activeTextUnit(0);
      shader->setInt("material.diffuse" , 0);
      break;
    
    case TextureType::TEX_NORMAL:
      Texture::activeTextUnit(1);
      shader->setInt("material.normal" , 1);
      break;
    
    case TextureType::TEX_SPECULAR:
      Texture::activeTextUnit(2);
      shader->setInt("material.specular" , 2);
      break;

    default:
      break;
    }

    texture->bind();
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


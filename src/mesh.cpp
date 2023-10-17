#include "mesh.hh"

#include <string>

Mesh::Mesh(std::vector<vertex_t> vertices, std::vector<uint64_t> indices, std::vector<Texture> textures)
{
  _vertices = vertices;
  _indices  = indices;
  _textures = textures;

  setupMesh();
}

void Mesh::setupMesh()
{
  _vertexBuffer   = new VertexBuffer(_vertices.size(), _vertices.data());
  _elementBuffer  = new ElementBuffer(_indices.size(), _indices.data());
  _vertexArray    = new VertexArray(*vb_ptr);

  _vertexArray->unbind();
}

void Mesh::draw(Shader& shader)
{
  for(int i = 0; i < _textures.size(); i++)
  {
    std::string samplerName;

    Texture::activeTextUnit(i);
    TextureType type = _textures[i].getType();

    if(type == TextureType::TEX_DIFFUSE)
      samplerName = "material.diffuse";

    else if(type == TextureType::TEX_SPECULAR)
      samplerName = "material.specular";

    shader.setInt(samplerName, i);
    _textures[i].bind();
  }

  Texture::activeTextUnit(0);

  // draw mesh
  _vertexArray->bind();
  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
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
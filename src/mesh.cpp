#include "mesh.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Mesh::Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices)
{
  _indices  = indices;
  
  diffuse   = nullptr;
  normal    = nullptr;
  specular  = nullptr;

  _vertexBuffer   = make_unique<VertexBuffer>(vertices.size(), vertices.data());
  _elementBuffer  = make_unique<ElementBuffer>(indices.size(), indices.data());
  _vertexArray    = make_unique<VertexArray>(_vertexBuffer.get());
}

void Mesh::draw(Shader* shader, uint32_t drawmode)
{
  if(diffuse)
  {
    Texture::activeTextUnit(0);
    shader->setInt("material.diffuse", 0);
    diffuse->bind();
  }
  if(normal)
  {
    Texture::activeTextUnit(1);
    shader->setInt("material.normal", 1);
    normal->bind();
  }
  if(specular)
  {
    Texture::activeTextUnit(2);
    shader->setInt("material.specular", 2);
    specular->bind();
  }

  shader->setFloat("material.shininess", 32.0f);

  _vertexArray.get()->bind();
  _elementBuffer.get()->bind();
  
  glDrawElements(drawmode, _indices.size(), GL_UNSIGNED_INT, 0);

  _elementBuffer.get()->unbind();
  _vertexArray.get()->unbind();
}

void Mesh::destroy()
{
  _vertexArray.get()->destroy();
  _vertexBuffer.get()->destroy();
  _elementBuffer.get()->destroy();
}


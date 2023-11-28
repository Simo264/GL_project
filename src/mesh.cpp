#include "mesh.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Mesh::Mesh(vector<GL::Vertex>& vertices, vector<uint32_t>& indices)
{
  init(vertices, indices);
}

void Mesh::draw(Shader* shader, uint32_t drawmode)
{
  (void) shader;
  
  if(diffuse)
  {
    Texture::activeTextUnit(0);
    diffuse->bind();
  }
  if(normal)
  {
    Texture::activeTextUnit(1);
    normal->bind();
  }
  if(specular)
  {
    Texture::activeTextUnit(2);
    specular->bind();
  }

  _vertexArray.bind();
  _elementBuffer.bind();
  
  glDrawElements(drawmode, _elementBuffer.numIndices(), GL_UNSIGNED_INT, 0);

  _vertexArray.unbind();
  _elementBuffer.unbind();
}

void Mesh::init(vector<GL::Vertex>& vertices, vector<uint32_t>& indices)
{
  diffuse   = nullptr;
  normal    = nullptr;
  specular  = nullptr;

  _vertexBuffer.init(vertices.size(), vertices.data());
  _elementBuffer.init(indices.size(), indices.data());
  _vertexArray.init(_vertexBuffer);
}

void Mesh::destroy()
{
  _vertexArray.destroy();
  _vertexBuffer.destroy();
  _elementBuffer.destroy();
}


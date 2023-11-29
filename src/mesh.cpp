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

void Mesh::draw(uint32_t drawmode)
{ 
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

  // always good practice to set everything back to defaults once configured.
  Texture::activeTextUnit(0);
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


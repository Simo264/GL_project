#include "mesh.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Mesh::Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices)
{
  diffuse   = nullptr;
  normal    = nullptr;
  specular  = nullptr;

  _vertexBuffer.init(vertices.size(), vertices.data());
  _elementBuffer.init(indices.size(), indices.data());
  _vertexArray.init(_vertexBuffer);
}

void Mesh::draw(Shader* shader, uint32_t drawmode)
{
  (void) shader;
  
  if(diffuse)
  {
    Texture::activeTextUnit(0);
    //shader->setInt("material.diffuse", 0);
    diffuse->bind();
  }
  if(normal)
  {
    Texture::activeTextUnit(1);
    //shader->setInt("material.normal", 1);
    normal->bind();
  }
  if(specular)
  {
    Texture::activeTextUnit(2);
    //shader->setInt("material.specular", 2);
    specular->bind();
  }

  // shader->setFloat("material.shininess", 32.0f);

  _vertexArray.bind();
  _elementBuffer.bind();
  
  glDrawElements(drawmode, _elementBuffer.numIndices(), GL_UNSIGNED_INT, 0);

  _vertexArray.unbind();
  _elementBuffer.unbind();
}

void Mesh::destroy()
{
  _vertexArray.destroy();
  _vertexBuffer.destroy();
  _elementBuffer.destroy();
}


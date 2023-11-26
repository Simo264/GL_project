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


  // reserve memory for VB, EB, VA and allocate them sequentially
  _ptrData       = make_unique<char[]>(sizeof(VertexBuffer) + sizeof(ElementBuffer) + sizeof(VertexArray));
  _vertexBuffer  = new(&_ptrData[0]) VertexBuffer(vertices.size(), vertices.data());
  _elementBuffer = new(&_ptrData[0] + sizeof(VertexBuffer)) ElementBuffer(indices.size(), indices.data());
  _vertexArray   = new(&_ptrData[0] + sizeof(VertexBuffer) + sizeof(ElementBuffer)) VertexArray(_vertexBuffer);
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

  _vertexArray->bind();
  _elementBuffer->bind();
  
  glDrawElements(drawmode, _indices.size(), GL_UNSIGNED_INT, 0);

  _vertexArray->unbind();
  _elementBuffer->unbind();
}

void Mesh::destroy()
{
  _vertexArray->destroy();
  _vertexBuffer->destroy();
  _elementBuffer->destroy();
  _ptrData.reset(); // deallocate memory 
}


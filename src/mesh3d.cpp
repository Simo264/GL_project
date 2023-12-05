#include "mesh3d.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

void Mesh3D::init(GL::VAConfiguration& configuration, vector<float>& vertices, vector<uint32_t>& indices)
{
  _vertexBuffer.init(vertices.size() * 4, vertices.data()); // vertices.size() * sizeof(float)
  _elementBuffer.init(indices.size() * 4, indices.data());  // indices.size() * sizeof(uint32_t)
  _vertexArray.init(configuration, _vertexBuffer);

  diffuse   = nullptr;
  normal    = nullptr;
  specular  = nullptr;
}

void Mesh3D::preDraw()
{
  if(diffuse)
  {
    glActiveTexture(GL_TEXTURE0);
    diffuse->bind();
  }
  if(normal)
  {
    glActiveTexture(GL_TEXTURE1);
    normal->bind();
  }
  if(specular)
  {
    glActiveTexture(GL_TEXTURE2);
    specular->bind();
  }

  _vertexArray.bind();
  _elementBuffer.bind();
}

void Mesh3D::postDraw()
{
  _vertexArray.unbind();
  _elementBuffer.unbind();

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}

void Mesh3D::draw()
{ 
  glDrawElements(GL_TRIANGLES, _elementBuffer.nIndices, GL_UNSIGNED_INT, 0);
}
  

#include "mesh3d.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

void Mesh3D::init(vector<float>& vertices, vector<uint32_t>& indices)
{
  GL::VAConfiguration config;
  config.pushAttribute(3); //layout=0 vec3 position
  config.pushAttribute(3); //layout=1 vec3 normals
  config.pushAttribute(2); //layout=2 vec2 textcoords

  _vertexBuffer.init(vertices.size() * 4, vertices.data()); // vertices.size() * sizeof(float)
  _elementBuffer.init(indices.size() * 4, indices.data());  // indices.size() * sizeof(uint32_t)
  _vertexArray.init(config, _vertexBuffer);

  diffuse   = nullptr;
  normal    = nullptr;
  specular  = nullptr;
}

void Mesh3D::draw(uint32_t drawmode)
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
  
  glDrawElements(drawmode, _elementBuffer.nIndices, GL_UNSIGNED_INT, 0);

  _vertexArray.unbind();
  _elementBuffer.unbind();

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}
  
void Mesh3D::destroy()
{
  _vertexArray.destroy();
  _vertexBuffer.destroy();
  _elementBuffer.destroy();
}


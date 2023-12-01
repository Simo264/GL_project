#include "mesh2d.hh"

Mesh2D::Mesh2D()
{
  static array<float, 16> vertices = {
    // positions       // texture coords
     1.0f,  1.0f,      1.0f, 1.0f, // top right
     1.0f, -1.0f,      1.0f, 0.0f, // bottom right
    -1.0f, -1.0f,      0.0f, 0.0f, // bottom left
    -1.0f,  1.0f,      0.0f, 1.0f  // top left 
  };
  static array<uint32_t, 6> indices = {0,1,3, 1,2,3};

  GL::VAConfiguration config;
  config.pushAttribute(2); //layout=0 vec3 position
  config.pushAttribute(2); //layout=1 vec2 textcoords

  _vertexBuffer.init(vertices.size() * 4, vertices.data()); // vertices.size() * sizeof(float)
  _elementBuffer.init(indices.size() * 4, indices.data());  // indices.size() * sizeof(uint32_t)
  _vertexArray.init(config, _vertexBuffer);

  texture = nullptr;
}

void Mesh2D::draw(uint32_t drawmode)
{
  if(texture)
  {
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
  }

  _vertexArray.bind();
  _elementBuffer.bind();
  
  glDrawElements(drawmode, _elementBuffer.nIndices, GL_UNSIGNED_INT, 0);

  _vertexArray.unbind();
  _elementBuffer.unbind();

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}

void Mesh2D::destroy()
{
  _vertexArray.destroy();
  _vertexBuffer.destroy();
  _elementBuffer.destroy();
}

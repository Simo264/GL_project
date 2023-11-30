#include "mesh2d.hh"

Mesh2D::Mesh2D()
{
  static array<float, 16> vertices = {
    // positions       // texture coords
     0.5f,  0.5f,      1.0f, 1.0f, // top right
     0.5f, -0.5f,      1.0f, 0.0f, // bottom right
    -0.5f, -0.5f,      0.0f, 0.0f, // bottom left
    -0.5f,  0.5f,      0.0f, 1.0f  // top left 
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
    Texture::activeTextUnit(0);
    texture->bind();
  }

  _vertexArray.bind();
  _elementBuffer.bind();
  
  glDrawElements(drawmode, _elementBuffer.numIndices(), GL_UNSIGNED_INT, 0);

  _vertexArray.unbind();
  _elementBuffer.unbind();

  // always good practice to set everything back to defaults once configured.
  Texture::activeTextUnit(0);
}

void Mesh2D::destroy()
{
  _vertexArray.destroy();
  _vertexBuffer.destroy();
  _elementBuffer.destroy();
}

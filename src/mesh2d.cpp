#include "mesh2d.hh"

Mesh2D::Mesh2D()
{
  static array<float, 20> vertices = {
    // positions          // texture coords
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f, // top left 
  };
  static array<uint32_t, 6> indices = {0,1,3, 1,2,3};

  GL::VAConfiguration config;
  config.pushAttribute(3); //layout=0 vec3 position
  config.pushAttribute(2); //layout=1 vec2 textcoords

  _vertexBuffer.init(vertices.size() * 4, vertices.data()); // vertices.size() * sizeof(float)
  _elementBuffer.init(indices.size() * 4, indices.data());  // indices.size() * sizeof(uint32_t)
  _vertexArray.init(config, _vertexBuffer);

  texture = nullptr;
}

void Mesh2D::preDraw()
{
  if(texture)
  {
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
  }
  else
    glBindTexture(GL_TEXTURE_2D, 0);

  _vertexArray.bind();
  _elementBuffer.bind();
}

void Mesh2D::postDraw()
{
  _vertexArray.unbind();
  _elementBuffer.unbind();

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}


void Mesh2D::draw()
{
  glDrawElements(GL_TRIANGLES, _elementBuffer.nIndices, GL_UNSIGNED_INT, 0);
}


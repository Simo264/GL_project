#ifndef SKYBOX_HH
#define SKYBOX_HH

#include "core.hh"
#include "GL/vertex_array.hh"

class SkyBox
{
public:
  SkyBox();
  ~SkyBox() = default;

  void draw(uint32_t drawmode = GL_TRIANGLES);

  uint32_t skyboxTexture;  

private:
  GL::VertexBuffer _vertexBuffer;
  GL::VertexArray  _vertexArray;
};

#endif
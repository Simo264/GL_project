#ifndef SKYBOX_HH
#define SKYBOX_HH

#include "core.hh"
#include "GL/vertex_array.hh"
#include "texture_cubemap.hh"

class SkyBox
{
public:
  SkyBox(const array<string, 6>& images);
  ~SkyBox() = default;

  void draw(uint32_t drawmode = GL_TRIANGLES);
  void destroy();

private:
  TextureCubeMap _texture;

  GL::VertexBuffer  _vertexBuffer;
  GL::ElementBuffer _elementBuffer;
  GL::VertexArray   _vertexArray;
};

#endif
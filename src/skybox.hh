#ifndef SKYBOX_HH
#define SKYBOX_HH

#include "imesh.hh"
#include "texture_cubemap.hh"

class SkyBox : public IMesh
{
public:
  SkyBox(const array<string, 6>& images);

  void preDraw();
  void postDraw();
  void draw();
  
  void destroy();

private:
  TextureCubeMap _textureCubeMap;
};
#endif
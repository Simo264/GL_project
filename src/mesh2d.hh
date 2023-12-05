#ifndef MESH2D_HH
#define MESH2D_HH

#include "imesh.hh"
#include "texture2d.hh"

class Mesh2D : public IMesh
{
public:
  Mesh2D();

  void preDraw();
  void postDraw();
  void draw();

  Texture2D* texture;
};
#endif
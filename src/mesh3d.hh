#ifndef MESH3D_HH
#define MESH3D_HH

#include "imesh.hh"
#include "texture2d.hh"


// A mesh represents a single drawable 3d entity
// ------------------------------------------------
class Mesh3D : public IMesh
{
public:
  void init(GL::VAConfiguration& configuration, vector<float>& vertices, vector<uint32_t>& indices);
  
  void preDraw();
  void postDraw();
  void draw(); 

  Texture2D* diffuse;
  Texture2D* normal;
  Texture2D* specular;
};

#endif
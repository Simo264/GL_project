#ifndef STENCIL_HH
#define STENCIL_HH

#include "core.hh"
#include "shader.hh"
#include "model.hh"

class Stencil
{
public:
  Stencil(Shader* shaderOutline);
  ~Stencil() = default;

  void drawOutline(Model* model, Shader* shaderScene);

  vec3f color;
  float thickness;

private:
  Shader* _shaderOutline;

};



#endif
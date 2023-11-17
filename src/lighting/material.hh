#ifndef MATERIAL_HH
#define MATERIAL_HH

#include "../core.hh"

namespace lighting
{

  #if 0

  class Material
  {
    public:
      Material()
      {
        colorAmbient = vec3f(0.25f, 0.25f, 0.25); // default grey
        colorDiffuse = vec3f(0.5f, 0.5f, 0.5);    // default light grey;
        colorSpecular= vec3f(1.0f, 1.0f, 1.0f);   // default white;
        shininess    = 32.0f;

        samplerTexDiffuse  = 0;
        samplerTexNormal   = 1;
        samplerTexSpecular = 2;
      }

      vec3f colorAmbient;
      vec3f colorDiffuse;
      vec3f colorSpecular;
      float shininess;

      int samplerTexDiffuse;
      int samplerTexNormal;
      int samplerTexSpecular;
  };

  #endif

}

#endif
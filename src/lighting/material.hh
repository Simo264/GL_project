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
        shininess          = 32.0f;
        samplerTexDiffuse  = 0;
        samplerTexNormal   = 1;
        samplerTexSpecular = 2;
      }

      int samplerTexDiffuse;
      int samplerTexNormal;
      int samplerTexSpecular;
      
      float shininess;
  };

  #endif

}

#endif
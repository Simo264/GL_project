#ifndef SPOT_LIGHT_HH
#define SPOT_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

namespace lighting
{
  class SpotLight
  {
    public:
      SpotLight();
      
      void render(Shader* shader);

      vec3f direction;

      vec3f color;

      vec3f ambient;
      vec3f diffuse;
      vec3f specular;
  };

}



#endif
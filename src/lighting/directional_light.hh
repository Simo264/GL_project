#ifndef DIRECTIONAL_LIGHT_HH
#define DIRECTIONAL_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

namespace lighting
{
  class DirectionalLight
  {
    public:
      DirectionalLight();
      
      void render(Shader* shader);

      vec3f direction;

      vec3f color;
      vec3f ambient;
      vec3f diffuse;
      vec3f specular;
  };

}



#endif
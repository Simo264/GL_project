#ifndef POINT_LIGHT_HH
#define POINT_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

namespace lighting
{
  class PointLight
  {
    public:
      PointLight();

      void render(Shader* shader);
      
      vec3f position;
      vec3f direction;

      vec3f color;

      vec3f ambient;
      vec3f diffuse;
      vec3f specular;

      float linear;
      float quadratic;
  };

}



#endif
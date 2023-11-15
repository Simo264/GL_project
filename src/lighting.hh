#ifndef LIGHTING_HH
#define LIGHTING_HH

#include "core.hh"

namespace lighting
{
  #if 0
  class Material
  {
    public:
      Material()
      {

      }

      vec3f ambient;
      vec3f diffuse;
      vec3f specular;
      float shininess;
  };
  #endif

  class Light
  {
    public:
      Light()
      {
        position  = vec3f(0.0f,0.0f,0.0f);  // default on origin
        direction = vec3f(0.0f,-1.0f,0.0f); // default from top to bottom

        color     = vec3f(1.0f,1.0f,1.0f);  // default white color

        ambient   = color * vec3f(0.25f,0.25f,0.25f); // default ambient
        diffuse   = color * vec3f(0.5f,0.50f,0.50f);  // default diffuse
        specular  = color * vec3f(1.0f,1.0f,1.0f);    // default specular
      }

      vec3f position;
      vec3f direction;

      vec3f color;

      vec3f ambient;
      vec3f diffuse;
      vec3f specular;
  };

}



#endif
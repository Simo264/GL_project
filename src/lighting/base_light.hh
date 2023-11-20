#ifndef BASE_LIGHT_HH
#define BASE_LIGHT_HH

#include "../core.hh"

namespace lighting
{
  class BaseLight
  {
  public:
    BaseLight()
    {
      color     = vec3f(1.0f,1.0f,1.0f);    // default white color
      ambient   = 0.25f;                    // default ambient intensity
      diffuse   = 0.50f;                    // default diffuse intensity
      specular  = 0.75f;                    // default specular intensity
    }
    ~BaseLight() = default;

    virtual void render(class Shader* shader) = 0;

    vec3f color;
    float ambient;  // ambient intensity
    float diffuse;  // diffuse intensity
    float specular; // specular intensity
  };

}

#endif
#ifndef POINT_LIGHT_HH
#define POINT_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

#include "base_light.hh"

namespace lighting
{
  struct Attenuation
  {
    Attenuation()
    {
      // default values
      linear    = 0.14f; // a distance of 32 to 100 is generally enough for most lights
      quadratic = 0.07f; 
    }

    float linear;
    float quadratic;
  };

  // A point light is a light source with a given position somewhere in a world that illuminates in all directions, 
  // where the light rays fade out over distance.
  // Think of light bulbs and torches as light casters that act as a point light.
  class PointLight : public BaseLight
  {
    public:
      PointLight(string uniformName);
      ~PointLight() = default;
      void render(Shader* shader);

      string uniformName;

      vec3f position;

      Attenuation attenuation;
  };
}
#endif
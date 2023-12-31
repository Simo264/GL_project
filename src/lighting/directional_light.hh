#ifndef DIRECTIONAL_LIGHT_HH
#define DIRECTIONAL_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

#include "base_light.hh"

namespace lighting
{
  // When a light source is modeled to be infinitely far away it is called a directional light 
  // since all its light rays have the same direction; it is independent of the location of the light source. 
  // A fine example of a directional light source is the sun as we know it. The sun is not infinitely far away from us, 
  // but it is so far away that we can perceive it as being infinitely far away in the lighting calculations.
  class DirectionalLight : public BaseLight
  {
    public:
      DirectionalLight(string uniformName);
      ~DirectionalLight() = default;

      void render(Shader* shader);

      string uniformName;

      vec3f direction;
  };

}

#endif
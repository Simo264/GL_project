#ifndef POINT_LIGHT_HH
#define POINT_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

namespace lighting
{
  // A point light is a light source with a given position somewhere in a world that illuminates in all directions, 
  // where the light rays fade out over distance.
  // Think of light bulbs and torches as light casters that act as a point light.
  class PointLight
  {
    public:
      PointLight(string uniformName);

      void render(Shader* shader);

      string uniformName;

      vec3f position;
      
      vec3f color;
      float ambient;  // ambient intensity
      float diffuse;  // diffuse intensity
      float specular; // specular intensity

      float linear;
      float quadratic;

}
#endif
#ifndef SPOT_LIGHT_HH
#define SPOT_LIGHT_HH

#include "../core.hh"
#include "../shader.hh"

namespace lighting
{
  // A spotlight is a light source that is located somewhere in the environment that, 
  // instead of shooting light rays in all directions, only shoots them in a specific direction.
  // A good example of a spotlight would be a street lamp or a flashlight. 
  class SpotLight
  {
    public:
      SpotLight(string uniformName);
      
      void render(Shader* shader);

      string uniformName;

      vec3f direction;
      vec3f position;

      vec3f color;
      float ambient;  // ambient intensity
      float diffuse;  // diffuse intensity
      float specular; // specular intensity

      // attenuation
      float linear;
      float quadratic;

      // spotlight (soft edges)
      float cutOff;    // the cutoff angle that specifies the spotlight's radius.
  };

}



#endif
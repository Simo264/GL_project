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
  };

}



#endif
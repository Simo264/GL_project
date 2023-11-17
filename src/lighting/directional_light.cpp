#include "directional_light.hh"

namespace lighting
{
  DirectionalLight::DirectionalLight()
  {
    direction = vec3f(0.0f,-1.0f,0.0f);             // default from top to bottom
    
    color     = vec3f(1.0f,1.0f,1.0f);                // default white color
    ambient   = color * vec3f(0.125f,0.125f,0.125f);  // default ambient
    diffuse   = color * vec3f(0.125f,0.125f,0.125f);  // default diffuse
    specular  = color * vec3f(0.250,0.250,0.250);     // default specular
  }

  void DirectionalLight::render(Shader* shader)
  {
    shader->setVec3f("light.direction", direction);
    shader->setVec3f("light.ambient",   ambient);
    shader->setVec3f("light.diffuse",   diffuse);
    shader->setVec3f("light.specular",  specular);
  }
}

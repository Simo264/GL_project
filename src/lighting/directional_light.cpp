#include "directional_light.hh"

namespace lighting
{
  DirectionalLight::DirectionalLight(string uniformName)
  {
    this->uniformName = uniformName;

    direction = vec3f(0.0f,-1.0f,0.0f);   // default from top to bottom
    color     = vec3f(1.0f,1.0f,1.0f);    // default white color
    ambient   = 0.25f;                    // default ambient intensity
    diffuse   = 0.50f;                    // default diffuse intensity
    specular  = 1.0f;                     // default specular intensity
  }

  void DirectionalLight::render(Shader* shader)
  {
    // char shaderUName[50] = { uniformName.c_str() };
    shader->setVec3f("dirLight.direction", direction);
    shader->setVec3f("dirLight.ambient",   color * ambient);
    shader->setVec3f("dirLight.diffuse",   color * diffuse);
    shader->setVec3f("dirLight.specular",  color * specular);
  }
}

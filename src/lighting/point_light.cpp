#include "point_light.hh"

namespace lighting
{
  PointLight::PointLight(string uniformName)
  {
    this->uniformName = uniformName;

    position  = vec3f(0.0f,0.0f,0.0f);  // default on origin
    color     = vec3f(1.0f,1.0f,1.0f);  // default white color
    ambient   = 0.25f; // default ambient intensity
    diffuse   = 0.50f; // default diffuse intensity
    specular  = 1.0f;  // default specular intensity
    linear    = 0.14f; // a distance of 32 to 100 is generally enough for most lights
    quadratic = 0.07f; 
  }

  void PointLight::render(Shader* shader)
  {
    // char shaderUName[50];

    shader->setVec3f("pointLight.position",  position);
    shader->setVec3f("pointLight.ambient",   color * ambient);
    shader->setVec3f("pointLight.diffuse",   color * diffuse);
    shader->setVec3f("pointLight.specular",  color * specular);
    shader->setFloat("pointLight.linear",    linear);
    shader->setFloat("pointLight.quadratic", quadratic);
  }
}

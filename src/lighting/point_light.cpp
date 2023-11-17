#include "point_light.hh"

namespace lighting
{
  PointLight::PointLight()
  {
    position  = vec3f(0.0f,0.0f,0.0f);  // default on origin
    direction = vec3f(0.0f,-1.0f,0.0f); // default from top to bottom

    color     = vec3f(1.0f,1.0f,1.0f);  // default white color
    ambient   = color * vec3f(0.25f,0.25f,0.25f); // default ambient
    diffuse   = color * vec3f(0.50f,0.50f,0.50f);  // default diffuse
    specular  = color * vec3f(1.0f,1.0f,1.0f);    // default specular

    linear    = 0.14f; // a distance of 32 to 100 is generally enough for most lights
    quadratic = 0.07f; 
  }

  void PointLight::render(Shader* shader)
  {
    shader->setVec3f("light.position",  position);
    shader->setVec3f("light.direction", direction);
    shader->setVec3f("light.ambient",   ambient);
    shader->setVec3f("light.diffuse",   diffuse);
    shader->setVec3f("light.specular",  specular);
    shader->setFloat("light.linear",    linear);
    shader->setFloat("light.quadratic", quadratic);
  }
}

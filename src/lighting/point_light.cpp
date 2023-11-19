#include "point_light.hh"

namespace lighting
{
  PointLight::PointLight(string uniformName) : uniformName{uniformName}
  {
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
    const int uniformNameSz = uniformName.size();
    char shaderUName[50] = { };
    
    strcpy(shaderUName, uniformName.c_str());         // shaderUName = "{uniformName}"
    
    strcpy(shaderUName+uniformNameSz, ".position");   // shaderUName = "{uniformName}.position"
    shader->setVec3f(shaderUName, position);
    
    strcpy(shaderUName+uniformNameSz, ".ambient");    // shaderUName = "{uniformName}.ambient"
    shader->setVec3f(shaderUName, color * ambient);
    
    strcpy(shaderUName+uniformNameSz, ".diffuse");    // shaderUName = "{uniformName}.diffuse"
    shader->setVec3f(shaderUName, color * diffuse);
    
    strcpy(shaderUName+uniformNameSz, ".specular");   // shaderUName = "{uniformName}.specular"
    shader->setVec3f(shaderUName, color * specular);
    
    strcpy(shaderUName+uniformNameSz, ".linear");     // shaderUName = "{uniformName}.linear"
    shader->setFloat(shaderUName, linear);

    strcpy(shaderUName+uniformNameSz, ".quadratic");  // shaderUName = "{uniformName}.quadratic"
    shader->setFloat(shaderUName, quadratic);
  }
}

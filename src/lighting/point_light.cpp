#include "point_light.hh"

#include "spdlog/spdlog.h"

namespace lighting
{
  PointLight::PointLight(string uniformName) : BaseLight(), uniformName{uniformName}
  {
    position  = vec3f(0.0f,0.0f,0.0f);  // default on origin
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
    shader->setFloat(shaderUName, attenuation.linear);

    strcpy(shaderUName+uniformNameSz, ".quadratic");  // shaderUName = "{uniformName}.quadratic"
    shader->setFloat(shaderUName, attenuation.quadratic);
  }
}

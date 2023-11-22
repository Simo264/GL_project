#include "directional_light.hh"

namespace lighting
{
  DirectionalLight::DirectionalLight(string uniformName) : BaseLight(), uniformName{uniformName}
  {
    direction = vec3f(0.0f,-1.0f,0.0f);   // default from top to bottom
  }

  void DirectionalLight::render(Shader* shader)
  {
    shader->use();
    
    const int uniformNameSz = uniformName.size();
    char shaderUName[50] = { };

    strcpy(shaderUName, uniformName.c_str());          // shaderUName = "{uniformName}"

    strcpy(shaderUName+uniformNameSz, ".direction");   // shaderUName = "{uniformName}.direction"
    shader->setVec3f(shaderUName, direction);
    
    strcpy(shaderUName+uniformNameSz, ".ambient");     // shaderUName = "{uniformName}.ambient"
    shader->setVec3f(shaderUName, color * ambient);
    
    strcpy(shaderUName+uniformNameSz, ".diffuse");     // shaderUName = "{uniformName}.diffuse"
    shader->setVec3f(shaderUName, color * diffuse);
    
    strcpy(shaderUName+uniformNameSz, ".specular");    // shaderUName = "{uniformName}.specular"
    shader->setVec3f(shaderUName, color * specular);
  }
}

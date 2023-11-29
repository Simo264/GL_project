#include "directional_light.hh"

namespace lighting
{
  DirectionalLight::DirectionalLight(string uniformName) : BaseLight(), uniformName{uniformName}
  {
    direction = vec3f(0.0f,-1.0f,0.0f);   // default from top to bottom
  }

  void DirectionalLight::render(Shader* shader)
  {
    const int uniformNameSz = uniformName.size();
    char shaderUName[30]    = { };

    strcpy(shaderUName, uniformName.c_str());          // shaderUName = "dirLight"

    strcpy(shaderUName+uniformNameSz, ".direction");   // shaderUName = "dirLight.direction"
    shader->setVec3f(shaderUName, direction);
    
    strcpy(shaderUName+uniformNameSz, ".ambient");     // shaderUName = "dirLight.ambient"
    shader->setVec3f(shaderUName, color * ambient);
    
    strcpy(shaderUName+uniformNameSz, ".diffuse");     // shaderUName = "dirLight.diffuse"
    shader->setVec3f(shaderUName, color * diffuse);
    
    strcpy(shaderUName+uniformNameSz, ".specular");    // shaderUName = "dirLight.specular"
    shader->setVec3f(shaderUName, color * specular);
  }
}

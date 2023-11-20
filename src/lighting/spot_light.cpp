// #include "spot_light.hh"

// namespace lighting
// {
//   SpotLight::SpotLight(string uniformName) : PointLight(uniformName)
//   {
//     direction = vec3f(0.0f,-1.0f,0.0f);   // default from top to bottom
//     cutOff    = 12.0f;
//   }
  
//   void SpotLight::render(Shader* shader)
//   {
//     const int uniformNameSz = uniformName.size();
//     char shaderUName[50] = { };

//     strcpy(shaderUName, uniformName.c_str());         // shaderUName = "{uniformName}"
//     strcpy(shaderUName+uniformNameSz, ".position");   // shaderUName = "{uniformName}.position"
//     shader->setVec3f(shaderUName, position);
    
//     strcpy(shaderUName+uniformNameSz, ".direction");  // shaderUName = "{uniformName}.direction"
//     shader->setVec3f(shaderUName, direction);

//     strcpy(shaderUName+uniformNameSz, ".ambient");     // shaderUName = "{uniformName}.ambient"
//     shader->setVec3f(shaderUName, color * ambient);
    
//     strcpy(shaderUName+uniformNameSz, ".diffuse");     // shaderUName = "{uniformName}.diffuse"
//     shader->setVec3f(shaderUName, color * diffuse);
    
//     strcpy(shaderUName+uniformNameSz, ".specular");    // shaderUName = "{uniformName}.specular"
//     shader->setVec3f(shaderUName, color * specular);

//     strcpy(shaderUName+uniformNameSz, ".linear");     // shaderUName = "{uniformName}.linear"
//     shader->setFloat(shaderUName, attenuation.linear);

//     strcpy(shaderUName+uniformNameSz, ".quadratic");  // shaderUName = "{uniformName}.quadratic"
//     shader->setFloat(shaderUName, attenuation.quadratic);
    
//     strcpy(shaderUName+uniformNameSz, ".cutOff");     // shaderUName = "{uniformName}.cutOff"
//     shader->setFloat(shaderUName, glm::cos(radians(cutOff)));
//   }
// }

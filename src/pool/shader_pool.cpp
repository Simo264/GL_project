#include "shader_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  map<string, Shader*> ShaderPool::_shaderMapping = map<string, Shader*>();

  void ShaderPool::loadShader(string label, Shader* shader)
  {
    auto it = _shaderMapping.find(label);
    if(it != _shaderMapping.end())
    {
      spdlog::warn("Shader {} is already loaded", label);
      return;
    } 

    _shaderMapping.insert(make_pair(label,shader));
  }

  Shader* ShaderPool::getShader(const string& label)
  {
    auto it = _shaderMapping.find(label);
    if(it == _shaderMapping.end())
      return nullptr;
    
    return it->second;
  }


  void ShaderPool::clear()
  {
    for (auto& [path, shader] : _shaderMapping)
    {
      shader->destroy();
      
      // remove this if Shader objects are not on the heap
      // delete shader;
    }

    _shaderMapping.clear();
  }

}
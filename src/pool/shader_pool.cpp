#include "shader_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  array<Shader, 10> ShaderPool::_shaderBuffer;
  uint32_t          ShaderPool::_nShaders;

  void ShaderPool::initBuffer()
  {
    _nShaders = 0;
  }

  Shader* ShaderPool::loadShader(const string& label, const string& vFilename, const string& fFilename)
  {
    if(_nShaders >= _shaderBuffer.size())
    {
      spdlog::warn("Can't load more shaders. Buffer is full");
      return nullptr;
    }

    Shader& shader = _shaderBuffer[_nShaders++];
    shader.init(label, vFilename, fFilename);
    return &shader;
  }

  Shader* ShaderPool::getShader(const string& label)
  {
    for(uint32_t i = 0; i < _nShaders; i++)
    {
      auto& s = _shaderBuffer[i];
      if(s.label().compare(label) == 0)
        return &s;
    }
    return nullptr;
  }

  void ShaderPool::freeBuffer()
  {
    for(uint32_t i = 0; i < _nShaders; i++)
    {
      auto& shader = _shaderBuffer[i];
      shader.destroy();
    }
  }

}
#include "shader_pool.hh"

#include "spdlog/spdlog.h"

#define MAX_SHADER_BUFFER_SIZE 10 

namespace pool
{
  unique_ptr<Shader[]> ShaderPool::_shaderBuffer;
  uint32_t ShaderPool::_bufferSz;
  uint32_t ShaderPool::_bufferCapacity;

  void ShaderPool::initialize()
  {
    _bufferSz = 0;
    _bufferCapacity = MAX_SHADER_BUFFER_SIZE;
    _shaderBuffer  = make_unique<Shader[]>(_bufferCapacity);
  }

  Shader* ShaderPool::loadShader(const string& label, const string& vFilename, const string& fFilename)
  {
    if(_bufferSz >= _bufferCapacity)
    { 
      spdlog::warn("Can't load more shaders. Buffer is full");
      return nullptr;
    }

    auto address = &_shaderBuffer.get()[_bufferSz];
    Shader* shader = new(address) Shader(label, vFilename, fFilename);
    _bufferSz++;

    return shader;
  }

  Shader* ShaderPool::getShader(const string& label)
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* s = &_shaderBuffer.get()[i];
      if(s->label().compare(label) == 0)
        return s;
    }

    return nullptr;
  }


  void ShaderPool::clear()
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* shader = &_shaderBuffer.get()[i];
      shader->destroy();
    }
    
    _shaderBuffer.release();
  }

}
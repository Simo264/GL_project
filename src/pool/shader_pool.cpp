#include "shader_pool.hh"

#include "spdlog/spdlog.h"

#define MAX_SHADER_BUFFER_SIZE 10 

namespace pool
{
  unique_ptr<Shader[]> ShaderPool::_shaderBuffer;
  uint32_t ShaderPool::_bufferSz;
  uint32_t ShaderPool::_bufferCapacity;

  void ShaderPool::initBuffer()
  {
    // allocate a bunch of memory on the heap big enough to
    // contain 10 contiguous shader objects
    _bufferSz = 0;
    _bufferCapacity = MAX_SHADER_BUFFER_SIZE;
    _shaderBuffer   = make_unique<Shader[]>(_bufferCapacity);
  }

  Shader* ShaderPool::loadShader(const string& label, const string& vFilename, const string& fFilename)
  {
    if(_bufferSz >= _bufferCapacity)
    { 
      spdlog::warn("Can't load more shaders. Buffer is full");
      return nullptr;
    }

    Shader* shader = new(&_shaderBuffer[_bufferSz]) Shader(label, vFilename, fFilename);
    _bufferSz++;

    return shader;
  }

  Shader* ShaderPool::getShader(const string& label)
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* s = &_shaderBuffer[i];
      if(s->label().compare(label) == 0)
        return s;
    }

    return nullptr;
  }

  void ShaderPool::freeBuffer()
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* shader = &_shaderBuffer[i];
      shader->destroy();
    }
    
    _shaderBuffer.reset();
  }

}
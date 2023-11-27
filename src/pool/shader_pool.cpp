#include "shader_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  uint32_t ShaderPool::_bufferSz;
  uint32_t ShaderPool::_bufferCapacity;

#ifdef DYNAMIC_SHADER_BUFFER_ALLOCATION
  unique_ptr<Shader[]> ShaderPool::_shaderPool;
#endif

#ifdef STATIC_SHADER_BUFFER_ALLOCATION
  array<Shader, MAX_SHADER_BUFFER_SIZE> ShaderPool::_shaderPool;
#endif

  void ShaderPool::initBuffer()
  {
    _bufferSz       = 0;
    _bufferCapacity = MAX_SHADER_BUFFER_SIZE;
  
  #ifdef DYNAMIC_SHADER_BUFFER_ALLOCATION
    // allocate a bunch of memory on the heap big enough to
    // contain `MAX_SHADER_BUFFER_SIZE` contiguous shader objects
    _shaderPool = make_unique<Shader[]>(_bufferCapacity);
  #endif
  }

  Shader* ShaderPool::loadShader(const string& label, const string& vFilename, const string& fFilename)
  {
  #ifdef DYNAMIC_SHADER_BUFFER_ALLOCATION
    if(_bufferSz >= _bufferCapacity)
    { 
      spdlog::warn("Can't load more shaders. Buffer is full");
      return nullptr;
    }

    Shader* shader = new(&_shaderPool[_bufferSz]) Shader(label, vFilename, fFilename);
    _bufferSz++;

    return shader;
  #endif

  #ifdef STATIC_SHADER_BUFFER_ALLOCATION
    if(_bufferSz >= MAX_SHADER_BUFFER_SIZE)
    {
      spdlog::warn("Can't load more shaders. Buffer is full");
      return nullptr;
    }
  
    Shader* shader = &_shaderPool[_bufferSz];
    shader->init(label, vFilename, fFilename);
    _bufferSz++;
    
    return shader;
  #endif
  }

  Shader* ShaderPool::getShader(const string& label)
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* s = &_shaderPool[i];
      if(s->label().compare(label) == 0)
        return s;
    }
    return nullptr;
  }

  void ShaderPool::freeBuffer()
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* shader = &_shaderPool[i];
      shader->destroy();
    }
    
  #ifdef DYNAMIC_SHADER_BUFFER_ALLOCATION
    _shaderPool.reset();
  #endif
  
  }

}
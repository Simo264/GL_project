#include "texture_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  #ifdef DYNAMIC_TEXTURE_BUFFER_ALLOCATION
    unique_ptr<Texture[]> TexturePool::_textureBuffer;
  #endif

  #ifdef STATIC_TEXTURE_BUFFER_ALLOCATION
    array<Texture, MAX_TEXTURE_BUFFER_SIZE> TexturePool::_textureBuffer;
  #endif
 
  uint32_t              TexturePool::_bufferSz;
  uint32_t              TexturePool::_bufferCapacity;

  void TexturePool::initBuffer()
  {
    _bufferSz = 0;
    _bufferCapacity = MAX_TEXTURE_BUFFER_SIZE;

    #ifdef DYNAMIC_TEXTURE_BUFFER_ALLOCATION
      // allocate a bunch of memory on the heap big enough to
      // contain 200 contiguous texture objects
      _textureBuffer  = make_unique<Texture[]>(_bufferCapacity);
    #endif
  }

  Texture* TexturePool::loadTexture(const string& path, TextureType type, bool immutable)
  {
    if(_bufferSz >= _bufferCapacity)
    { 
      spdlog::warn("Can't load more textures. Buffer is full");
      return nullptr;
    }

    Texture* texture = &_textureBuffer[_bufferSz];
    texture->init(path, type, immutable);
    _bufferSz++;
    return texture;
  }

  Texture* TexturePool::getTexture(const string& path)
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* t = &_textureBuffer[i];
      if(t->path().compare(path) == 0)
        return t;
    }

    return nullptr;
  }

  void TexturePool::freeBuffer()
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* texture = &_textureBuffer[i];
      texture->destroy();
    }

  #ifdef DYNAMIC_TEXTURE_BUFFER_ALLOCATION
    _textureBuffer.reset();
  #endif
  }
}

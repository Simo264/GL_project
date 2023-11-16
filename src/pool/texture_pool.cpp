#include "texture_pool.hh"

#include "spdlog/spdlog.h"

#define MAX_TEXTURE_BUFFER_SIZE 200

namespace pool
{
  unique_ptr<Texture[]> TexturePool::_textureBuffer;
  uint32_t              TexturePool::_bufferSz;
  uint32_t              TexturePool::_bufferCapacity;

  void TexturePool::initialize()
  {
    _bufferSz = 0;
    _bufferCapacity = MAX_TEXTURE_BUFFER_SIZE;
    _textureBuffer  = make_unique<Texture[]>(_bufferCapacity);
  }

  Texture* TexturePool::loadTexture(const string& path, TextureType type, bool immutable)
  {
    if(_bufferSz >= _bufferCapacity)
    { 
      spdlog::warn("Can't load more textures. Buffer is full");
      return nullptr;
    }

    auto address = &_textureBuffer.get()[_bufferSz];
    Texture* texture = new(address) Texture(path, type, immutable);
    _bufferSz++;

    return texture;
  }

  Texture* TexturePool::getTexture(const string& path)
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* t = &_textureBuffer.get()[i];
      if(t->path().compare(path) == 0)
        return t;
    }

    return nullptr;
  }

  void TexturePool::clear()
  {
    for(uint32_t i = 0; i < _bufferSz; i++)
    {
      auto* texture = &_textureBuffer.get()[i];
      texture->destroy();
    }
    
    _textureBuffer.release();
  }
}

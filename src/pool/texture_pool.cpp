#include "texture_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  array<Texture2D, 100> TexturePool::_textureBuffer;
  uint32_t              TexturePool::_nTextures;

  void TexturePool::initBuffer()
  {
    _nTextures = 0;
  }

  Texture2D* TexturePool::loadTexture(const string& path)
  {
    if(_nTextures >= _textureBuffer.size())
    { 
      spdlog::warn("Can't load more textures. Buffer is full");
      return nullptr;
    }

    Texture2D& texture = _textureBuffer[_nTextures++];
    texture.init(path);
    
    return &texture;
  }

  Texture2D* TexturePool::getTexture(const string& path)
  {
    for(uint32_t i = 0; i < _nTextures; i++)
    {
      auto& t = _textureBuffer[i];
      if(t.path().compare(path) == 0)
        return &t;
    }

    return nullptr;
  }

  void TexturePool::freeBuffer()
  {
    for(uint32_t i = 0; i < _nTextures; i++)
    {
      auto& texture = _textureBuffer[i];
      texture.destroy();
    }
  }
}

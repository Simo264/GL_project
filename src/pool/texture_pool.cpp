#include "texture_pool.hh"

#include "spdlog/spdlog.h"

namespace pool
{
  map<string, Texture*> TexturePool::_textureMapping = map<string, Texture*>();

  void TexturePool::loadTexture(string path, Texture* texture)
  {
    auto it = _textureMapping.find(path);
    if(it != _textureMapping.end())
    {
      spdlog::warn("Texture {} is already loaded", path);
      return;
    } 

    _textureMapping.insert(make_pair(path,texture));
  }

  Texture* TexturePool::getTexture(const string& path)
  {
    auto it = _textureMapping.find(path);
    
    if (it == _textureMapping.end())
      return nullptr;

    return it->second;
  }

  void TexturePool::clear()
  {
    for (auto& [path, texture] : _textureMapping)
    {
      texture->destroy();
      
      // remove this if Texture objects are not on the heap
      delete texture;
    }

    _textureMapping.clear();
  }
}



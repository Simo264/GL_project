#include "texture_pool.hh"

namespace pool
{
  map<string, Texture*> TexturePool::_textureMapping = map<string, Texture*>();

  void TexturePool::loadTexture(string path, Texture* texture)
  {
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
      delete texture;
    }

    _textureMapping.clear();
  }
}



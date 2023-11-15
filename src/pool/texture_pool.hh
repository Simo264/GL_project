#ifndef TEXTURE_POOL_HH
#define TEXTURE_POOL_HH

#include "../core.hh"
#include "../texture.hh"

namespace pool
{
  class TexturePool
  {
  public:
    TexturePool() = default;
    ~TexturePool() = default;

    static void     loadTexture(string path, Texture* texture);
    static Texture* getTexture(const string& path);

    static void clear();

  private:
    static map<string, Texture*> _textureMapping; // <path, Texture*>
  };
}

#endif
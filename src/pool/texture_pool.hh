#ifndef TEXTURE_POOL_HH
#define TEXTURE_POOL_HH

#include "../core.hh"
#include "../texture2d.hh"

namespace pool
{
  class TexturePool
  {
  public:
    TexturePool() = delete;
    ~TexturePool() = delete;

    static void initBuffer();

    static Texture2D* loadTexture(const string& path);
    static Texture2D* getTexture(const string& path);

    static void freeBuffer();

  private:
    static array<Texture2D, 100> _textureBuffer;
    static uint32_t              _nTextures;
  };
}

#endif
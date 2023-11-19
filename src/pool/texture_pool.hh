#ifndef TEXTURE_POOL_HH
#define TEXTURE_POOL_HH

#include "../core.hh"
#include "../texture.hh"

namespace pool
{
  class TexturePool
  {
  public:
    TexturePool() = delete;
    ~TexturePool() = delete;

    static void initBuffer();

    static Texture* loadTexture(const string& path, TextureType type, bool immutable);
    static Texture* getTexture(const string& path);

    static void freeBuffer();

  private:
    static unique_ptr<Texture[]> _textureBuffer;
    static uint32_t _bufferSz;
    static uint32_t _bufferCapacity;
  };
}

#endif
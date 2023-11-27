#ifndef TEXTURE_POOL_HH
#define TEXTURE_POOL_HH

#include "../core.hh"
#include "../texture.hh"

#define MAX_TEXTURE_BUFFER_SIZE 100

#define STATIC_TEXTURE_BUFFER_ALLOCATION 
// #define DYNAMIC_TEXTURE_BUFFER_ALLOCATION

namespace pool
{
  class TexturePool
  {
  public:
    TexturePool() = delete;
    ~TexturePool() = delete;

    static void initBuffer();

    static Texture* loadTexture(const string& path, TextureType type = TextureType::DIFFUSE, bool immutable = false);
    static Texture* getTexture(const string& path);

    static void freeBuffer();

  private:
  #ifdef DYNAMIC_TEXTURE_BUFFER_ALLOCATION
    static unique_ptr<Texture[]> _textureBuffer;
  #endif

  #ifdef STATIC_TEXTURE_BUFFER_ALLOCATION
    static array<Texture, MAX_TEXTURE_BUFFER_SIZE> _textureBuffer;
  #endif

    static uint32_t _bufferSz;
    static uint32_t _bufferCapacity;
  };
}

#endif
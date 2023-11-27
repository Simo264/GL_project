#ifndef SHADER_POOL_HH
#define SHADER_POOL_HH

#include "../core.hh"
#include "../shader.hh"

#define MAX_SHADER_BUFFER_SIZE 10

#define STATIC_SHADER_BUFFER_ALLOCATION 
// #define DYNAMIC_SHADER_BUFFER_ALLOCATION

namespace pool
{
  class ShaderPool
  {
  public:
    ShaderPool() = delete;
    ~ShaderPool() = delete;

    static void initBuffer();

    static Shader* loadShader(const string& label, const string& vFilename, const string& fFilename);
    static Shader* getShader(const string& label);

    static void freeBuffer();

  private:
  #ifdef DYNAMIC_SHADER_BUFFER_ALLOCATION
    static unique_ptr<Shader[]> _shaderPool;
    static uint32_t _bufferSz;
    static uint32_t _bufferCapacity;
  #endif

  #ifdef STATIC_SHADER_BUFFER_ALLOCATION
    static array<Shader, MAX_SHADER_BUFFER_SIZE> _shaderPool;
    static uint32_t _bufferSz;
    static uint32_t _bufferCapacity;
  #endif

  };
}

#endif
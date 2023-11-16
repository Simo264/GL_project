#ifndef SHADER_POOL_HH
#define SHADER_POOL_HH

#include "../core.hh"
#include "../shader.hh"

namespace pool
{
  class ShaderPool
  {
  public:
    ShaderPool() = default;
    ~ShaderPool() = default;

    static void initialize();

    static Shader* loadShader(const string& label, const string& vFilename, const string& fFilename);
    static Shader* getShader(const string& label);

    static void clear();

  private:
    static unique_ptr<Shader[]> _shaderBuffer;
    static uint32_t _bufferSz;
    static uint32_t _bufferCapacity;

  };
}

#endif
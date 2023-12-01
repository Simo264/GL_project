#ifndef SHADER_POOL_HH
#define SHADER_POOL_HH

#include "../core.hh"
#include "../shader.hh"

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
    static array<Shader, 10> _shaderBuffer;
    static uint32_t _nShaders;
  };
}

#endif
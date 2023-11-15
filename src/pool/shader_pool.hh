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

    static void    loadShader(string label, Shader* shader);
    static Shader* getShader(const string& label);

    static void clear();

  private:
    static map<string, Shader*> _shaderMapping; // <label, shader>

  };
}

#endif
#ifndef SHADER_HH
#define SHADER_HH

#include "glad/glad.h"

class Shader
{
public:
  Shader(const char* vShaderSrc, const char* fShaderSrc);
  ~Shader() = default;

  void use() const { glUseProgram(mShader); }

  void destroy() { glDeleteProgram(mShader); }

private:
  uint64_t mShader; 

  bool compile(uint64_t& shader, const char* src);
  bool createAndLink(uint64_t vShader, uint64_t fShader);
};




#endif
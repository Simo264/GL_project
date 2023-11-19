#ifndef SHADER_HH
#define SHADER_HH

#include "core.hh"

class Shader
{
public:
  Shader(const string& label, const string& vFilename, const string& fFilename);
  Shader()  = default;
  ~Shader() = default;

  void use() const { glUseProgram(_shader); }

  void destroy() { glDeleteProgram(_shader); }

  uint32_t get() const { return _shader; }

  const string& label() const { return _label; }

  void setBool(const char* name, bool value) const;
  void setInt(const char* name, int value) const;
  void setFloat(const char* name, float value) const;

  void setVec2f(const char* name, const vec2f& value) const;
  void setVec3f(const char* name, const vec3f& value) const;
  void setVec4f(const char* name, const vec4f& value) const;
  void setMat2f(const char* name, const mat2f& mat) const;
  void setMat3f(const char* name, const mat3f& mat) const;
  void setMat4f(const char* name, const mat4f& mat) const;

private:
  uint32_t _shader;
  string   _label;
  
  void getSourceFromFile(const string& filename, string& dest);
  bool compile(uint32_t& shader, const char* src);
  bool createAndLink(uint32_t vShader, uint32_t fShader);
};


#endif
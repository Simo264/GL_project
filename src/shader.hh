#ifndef SHADER_HH
#define SHADER_HH

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <string>

class Shader
{
public:
  Shader(const std::string& vFilename, const std::string& fFilename);
  ~Shader() = default;

  void use() const { glUseProgram(mShader); }

  void destroy() { glDeleteProgram(mShader); }

  uint32_t get() const { return mShader; }


  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;

  void setVec2(const std::string& name, const glm::vec2& value) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec4(const std::string& name, const glm::vec4& value) const;
  void setMat2(const std::string& name, const glm::mat2& mat) const;
  void setMat3(const std::string& name, const glm::mat3& mat) const;
  void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
  uint32_t mShader; 

  void getSourceFromFile(const std::string& filename, std::string& dest);
  bool compile(uint32_t& shader, const char* src);
  bool createAndLink(uint32_t vShader, uint32_t fShader);
};




#endif
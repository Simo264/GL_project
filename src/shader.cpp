#include "shader.hh"
#include "spdlog/spdlog.h"

#include <fstream>
#include <sstream>

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/


Shader::Shader(const std::string& vFilename, const std::string& fFilename)
{
  std::string vShaderSrc;
  std::string fShaderSrc;
  getSourceFromFile(vFilename, vShaderSrc);
  getSourceFromFile(fFilename, fShaderSrc);

  char infoLog[512];

  // vertex shader
  uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if(!compile(vertexShader, vShaderSrc.c_str()))
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED [{}]", infoLog);
    return;
  }
  
  // fragment shader
  uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if(!compile(fragmentShader, fShaderSrc.c_str()))
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED [{}]", infoLog);
    return;
  }

  // build and compile our shader program
  if(!createAndLink(vertexShader, fragmentShader))
  {
    glGetProgramInfoLog(_shader, 512, NULL, infoLog);
    spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED [{}]",infoLog);
    return;
  }
}

void Shader::setBool(const std::string& name, bool value) const
{         
  glUniform1i(glGetUniformLocation(_shader, name.c_str()), (int) value); 
}

void Shader::setInt(const std::string& name, int value) const
{ 
  glUniform1i(glGetUniformLocation(_shader, name.c_str()), value); 
}

void Shader::setFloat(const std::string& name, float value) const
{ 
  glUniform1f(glGetUniformLocation(_shader, name.c_str()), value); 
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{ 
  glUniform2fv(glGetUniformLocation(_shader, name.c_str()), 1, &value[0]); 
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{ 
  glUniform3fv(glGetUniformLocation(_shader, name.c_str()), 1, &value[0]); 
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{ 
  glUniform4fv(glGetUniformLocation(_shader, name.c_str()), 1, &value[0]); 
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(_shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(_shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(_shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/


void Shader::getSourceFromFile(const std::string& filename, std::string& dest)
{
  std::stringstream buffer;
  std::ifstream file(filename);
  if(!file.is_open())
  {
    spdlog::error("Shader::getSourceFromFile error on opening file in ");
    return;
  }

  buffer << file.rdbuf();
  
  dest = buffer.str();

  file.close();
}

bool Shader::compile(uint32_t& shader, const char* src)
{
  int success;
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  return (bool) success;
}

bool Shader::createAndLink(uint32_t vShader, uint32_t fShader)
{
  int success;

  _shader = glCreateProgram();
  glAttachShader(_shader, vShader);
  glAttachShader(_shader, fShader);
  glLinkProgram(_shader);

  glGetProgramiv(_shader, GL_LINK_STATUS, &success);

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  return (bool) success;
}


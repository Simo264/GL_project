#include "shader.hh"
#include "spdlog/spdlog.h"



Shader::Shader(const char* vShaderSrc, const char* fShaderSrc)
{
  char infoLog[512];

  // vertex shader
  uint64_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if(!compile(vertexShader, vShaderSrc))
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    spdlog::error<std::string>("ERROR::SHADER::VERTEX::COMPILATION_FAILED [{}]", infoLog);
    return;
  }
  
  // fragment shader
  uint64_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  if(!compile(fragmentShader, fShaderSrc))
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    spdlog::error<std::string>("ERROR::SHADER::VERTEX::COMPILATION_FAILED [{}]", infoLog);
    return;
  }

  // build and compile our shader program
  if(!createAndLink(vertexShader, fragmentShader))
  {
    glGetProgramInfoLog(mShader, 512, NULL, infoLog);
    spdlog::error<std::string>("ERROR::SHADER::PROGRAM::LINKING_FAILED [{}]",infoLog);
    return;
  }
}

bool Shader::compile(uint64_t& shader, const char* src)
{
  int success;
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  return (bool) success;
}

bool Shader::createAndLink(uint64_t vShader, uint64_t fShader)
{
  int success;

  mShader = glCreateProgram();
  glAttachShader(mShader, vShader);
  glAttachShader(mShader, fShader);
  glLinkProgram(mShader);

  glGetProgramiv(mShader, GL_LINK_STATUS, &success);

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  return (bool) success;
}

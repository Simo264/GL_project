#include "shader.hh"
#include "spdlog/spdlog.h"

#include <fstream>
#include <sstream>

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
    glGetProgramInfoLog(mShader, 512, NULL, infoLog);
    spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED [{}]",infoLog);
    return;
  }
}

void Shader::getSourceFromFile(const std::string& filename, std::string& dest)
{
  std::stringstream buffer;
  std::ifstream file(filename);
  if(!file.is_open())
  {
    spdlog::error("Error on opening file");
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

  mShader = glCreateProgram();
  glAttachShader(mShader, vShader);
  glAttachShader(mShader, fShader);
  glLinkProgram(mShader);

  glGetProgramiv(mShader, GL_LINK_STATUS, &success);

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  return (bool) success;
}


void Shader::setBool(const std::string &name, bool value) const
{         
  glUniform1i(glGetUniformLocation(mShader, name.c_str()), (int) value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
  glUniform1i(glGetUniformLocation(mShader, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
  glUniform1f(glGetUniformLocation(mShader, name.c_str()), value); 
}



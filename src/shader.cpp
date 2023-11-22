#include "shader.hh"
#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/


Shader::Shader(const string& label, const string& vFilename, const string& fFilename)
  : _label{label}
{
  string vShaderSrc;
  string fShaderSrc;
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
    spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED [{}]", infoLog);
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

void Shader::setBool(const char* name, bool value) const
{         
  glUniform1i(glGetUniformLocation(_shader, name), (int) value); 
}

void Shader::setInt(const char* name, int value) const
{ 
  glUniform1i(glGetUniformLocation(_shader, name), value); 
}

void Shader::setFloat(const char* name, float value) const
{ 
  glUniform1f(glGetUniformLocation(_shader, name), value); 
}

void Shader::setVec2f(const char* name, const vec2f& value) const
{ 
  glUniform2fv(glGetUniformLocation(_shader, name), 1, &value[0]); 
}

void Shader::setVec3f(const char* name, const vec3f& value) const
{ 
  glUniform3fv(glGetUniformLocation(_shader, name), 1, &value[0]); 
}

void Shader::setVec4f(const char* name, const vec4f& value) const
{ 
  glUniform4fv(glGetUniformLocation(_shader, name), 1, &value[0]); 
}

void Shader::setMat2f(const char* name, const mat2f& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(_shader, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3f(const char* name, const mat3f& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(_shader, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4f(const char* name, const mat4f& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(_shader, name), 1, GL_FALSE, &mat[0][0]);
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/


void Shader::getSourceFromFile(const string& filename, string& dest)
{
  stringstream buffer;
  ifstream file(filename);
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


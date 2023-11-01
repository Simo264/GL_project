#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "glad/glad.h"
#include <string>

enum class TextureType { 
  TEX_DIFFUSE = 0, 
  TEX_NORMAL, 
  TEX_SPECULAR,
};

class Texture
{
public:
  Texture(const std::string& path, TextureType type = TextureType::TEX_DIFFUSE, bool immutable = false);
  ~Texture() = default;

  void bind() const { glBindTexture(GL_TEXTURE_2D, _texture); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  void destroy() { glDeleteTextures(1, &_texture); }
  static void activeTextUnit(uint16_t index) { glActiveTexture(GL_TEXTURE0 + index); }
  void setParameteri(int pname, int param) { glTextureParameteri(_texture, pname, param); }
  
  uint32_t get() const  { return _texture; }

  int width() const  { return _width; }
  int height() const { return _height; }

  TextureType type() const { return _type; }

  std::string path() const { return _path; }

private:
  uint32_t _texture;
  int _width, _height;
  std::string _path;

  TextureType _type;

  void load(const std::string& path, bool immutable);
  bool alpha(const std::string& path);
};

#endif
#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "glad/glad.h"
#include <string>

enum class TextureType { 
  TEX_NONE = 0,
  TEX_DIFFUSE, 
  TEX_SPECULAR,
};

class Texture
{
public:
  Texture(const std::string& path, TextureType type = TextureType::TEX_NONE, bool immutable = true);
  ~Texture() = default;

  void bind() const { glBindTexture(GL_TEXTURE_2D, _texture); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  void destroy() { glDeleteTextures(1, &_texture); }
  static void activeTextUnit(uint16_t index) { glActiveTexture(GL_TEXTURE0 + index); }
  void setParameteri(int pname, int param) { glTextureParameteri(_texture, pname, param); }
  
  uint32_t get() const  { return _texture; }

  int getWidth() const  { return _width; }
  int getHeight() const { return _height; }

  TextureType getType() const { return _type; }

  std::string path() const { return _path; }

private:
  uint32_t _texture;
  int _width;
  int _height;
  std::string _path;

  TextureType _type;

  void load(const std::string& path, bool immutable);
};

#endif
#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "glad/glad.h"
#include <string>

class Texture
{
public:
  Texture(const std::string& filename, bool immutable = false);
  ~Texture() = default;

  void bind() const { glBindTexture(GL_TEXTURE_2D, _texture); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  void destroy() { glDeleteTextures(1, &_texture); }
  void activeTextUnit(uint32_t index) const { glActiveTexture(GL_TEXTURE0 + index); }
  void setParameteri(int pname, int param) { glTextureParameteri(_texture, pname, param); }
  
  uint32_t get() const { return _texture; }

  int getWidth() const { return _width; }
  int getHeight() const { return _height; }

private:
  uint32_t _texture;
  int _width;
  int _height;

  bool _isImmutable;

  void load(const std::string& filename);
};

#endif
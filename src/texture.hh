#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "core.hh"

enum class TextureType { 
  DIFFUSE = 0, 
  NORMAL, 
  SPECULAR,
};

class Texture
{
public:
  Texture(const string& path, TextureType type, bool immutable = false);
  Texture()  = default;
  ~Texture() = default;

  Texture(const Texture&) = delete;            // delete copy constructor
  Texture& operator=(const Texture&) = delete; // delete assign op

  void init(const string& path, TextureType type, bool immutable = false);

  void bind() const { glBindTexture(GL_TEXTURE_2D, _texture); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  void destroy() { glDeleteTextures(1, &_texture); }
  static void activeTextUnit(uint16_t index) { glActiveTexture(GL_TEXTURE0 + index); }
  void setParameteri(int pname, int param) { glTextureParameteri(_texture, pname, param); }
  
  uint32_t get() const  { return _texture; }

  TextureType type() const { return _type; }

  string path() const { return _path; }

private:
  uint32_t _texture;
  string   _path;

  TextureType _type;

  void loadImage(const string& path, bool immutable);
  bool alpha(const string& path);
};

#endif
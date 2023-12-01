#ifndef TEXTURE2D_HH
#define TEXTURE2D_HH

#include "core.hh"

class Texture2D 
{
public:
  Texture2D() = default;
  ~Texture2D() = default;

  Texture2D(const Texture2D&) = delete;            // delete copy constructor
  Texture2D& operator=(const Texture2D&) = delete; // delete assign op

  void init(const string& path);
  void bind()   const { glBindTexture(GL_TEXTURE_2D, textureID); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

  const string& path() const { return _path; }

  void destroy() { glDeleteTextures(1, &textureID); }

  uint32_t textureID;
  
private:
  string _path;

  void loadImage();

};

#endif
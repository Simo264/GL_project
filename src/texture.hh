#ifndef TEXTURE_HH
#define TEXTURE_HH

#include "glad/glad.h"
#include <string>

class Texture
{
public:
  Texture(const std::string& filename);
  ~Texture() = default;

  void bind() const;
  void unbind() const;
  void destroy();
  void activeTextUnit(uint32_t index) const;
  void setParameteri(int pname, int param);
  
  uint32_t get() const;

  int getWidth() const;
  int getHeight() const;

private:
  uint32_t mTexture;
  int mWidth;
  int mHeight;

  void load(const std::string& filename);
};

#endif
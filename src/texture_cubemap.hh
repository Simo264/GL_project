#ifndef TEXTURE_CUBEMAP_HH
#define TEXTURE_CUBEMAP_HH

#include "core.hh"

class TextureCubeMap 
{
public:
  TextureCubeMap() = default;
  ~TextureCubeMap() = default;

  TextureCubeMap(const TextureCubeMap&) = delete;            // delete copy constructor
  TextureCubeMap& operator=(const TextureCubeMap&) = delete; // delete assign op

  void init(const array<string, 6>& images);
  
  void bind()   const { glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }
  void unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

  void destroy() { glDeleteTextures(1, &textureID); }

  uint32_t textureID;
};

#endif
#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

#include <array>

Texture::Texture(const std::string& path, TextureType type, bool immutable)
  : _path{path}, _type {type}
{
  (void) immutable;

  glGenTextures(1, &_texture);

  bind();

  setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  load(path, immutable);

  unbind();
}

void Texture::load(const std::string& path, bool immutable)
{
  int nrChannels;
  
  u_char* data = stbi_load(path.c_str(), &_width, &_height, &nrChannels, 0);
  if (data)
  {
    int format = GL_RGB;

    const std::array<char, 3> suffix = {'p', 'n', 'g'};
    bool isPNG = std::equal(suffix.rbegin(), suffix.rend(), path.rbegin());
    
    if(isPNG)
      format = GL_RGBA;

    if(immutable)
    {
      // immutable object
      glTextureStorage2D(_texture, 1, format, _width, _height);
      glTextureSubImage2D(_texture, 0, 0, 0, _width, _height, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      // mutable object
      glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    spdlog::error("Failed to load texture {}", path);
  }
  stbi_image_free(data);
}

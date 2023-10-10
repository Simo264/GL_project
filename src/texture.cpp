#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

Texture::Texture(const std::string& filename, bool immutable) : _isImmutable{immutable}
{
  glGenTextures(1, &_texture);
  if(_texture == GL_INVALID_VALUE)
  {
    spdlog::error("Texture object is GL_INVALID_VALUE");
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D, _texture); 
    load(filename);

    setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

}

void Texture::load(const std::string& filename)
{
  int nrChannels;
  //stbi_set_flip_vertically_on_load(true); 
  u_char* data = stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 0);
  if (data)
  {
    if(_isImmutable)
    {
      // immutable object
      glTextureStorage2D(_texture, 1, GL_RGB8, _width, _height);
      glTextureSubImage2D(_texture, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      // mutable object
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
        
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    spdlog::error("Failed to load texture");
  }
  stbi_image_free(data);
}

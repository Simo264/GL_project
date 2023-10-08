#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

Texture::Texture(const std::string& filename, bool isImmutable) : _isImmutable{isImmutable}
{
  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture); 
  load(filename);
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, _texture); 
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0); 
}

void Texture::destroy()
{
  glDeleteTextures(1, &_texture);
}

void Texture::activeTextUnit(uint32_t index) const
{
  glActiveTexture(GL_TEXTURE0 + index);
}

void Texture::setParameteri(int pname, int param)
{
  glTextureParameteri(_texture, pname, param);
}

void Texture::load(const std::string& filename)
{
  int nrChannels;
  //stbi_set_flip_vertically_on_load(true); 
  unsigned char *data = stbi_load(filename.c_str(), &_width, &_height, &nrChannels, 0);
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

int Texture::getWidth() const
{
  return _width;
}

int Texture::getHeight() const
{
  return _height;
}

uint32_t Texture::get() const
{
  return _texture;
}


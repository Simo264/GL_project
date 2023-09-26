#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

Texture::Texture(const std::string& filename, bool isImmutable) : mIsImmutable{isImmutable}
{
  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture); 
  load(filename);
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, mTexture); 
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0); 
}

void Texture::destroy()
{
  glDeleteTextures(1, &mTexture);
}

void Texture::activeTextUnit(uint32_t index) const
{
  glActiveTexture(GL_TEXTURE0 + index);
}

void Texture::setParameteri(int pname, int param)
{
  glTextureParameteri(mTexture, pname, param);
}

void Texture::load(const std::string& filename)
{
  int nrChannels;
  //stbi_set_flip_vertically_on_load(true); 
  unsigned char *data = stbi_load(filename.c_str(), &mWidth, &mHeight, &nrChannels, 0);
  if (data)
  {
    if(mIsImmutable)
    {
      // immutable object
      glTextureStorage2D(mTexture, 1, GL_RGB8, mWidth, mHeight);
      glTextureSubImage2D(mTexture, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      // mutable object
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
  return mWidth;
}

int Texture::getHeight() const
{
  return mHeight;
}

uint32_t Texture::get() const
{
  return mTexture;
}


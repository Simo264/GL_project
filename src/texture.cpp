#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Texture::Texture(const string& path, TextureType type, bool immutable)
{
  init(path, type, immutable);
}

void Texture::init(const string& path, TextureType type, bool immutable)
{
  _path = path;
  _type = type;

  glGenTextures(1, &_texture);

  bind();

  setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
  setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  loadImage(path, immutable);
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/


void Texture::loadImage(const string& path, bool immutable)
{
  stbi_set_flip_vertically_on_load(true);
  
  int width, height, nrChannels;
  auto data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
    int format = GL_RGB;
    if(alpha(path))
      format = GL_RGBA;

    if(immutable)
    {
      // immutable object
      glTextureStorage2D(_texture, 1, format, width, height);
      glTextureSubImage2D(_texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      // mutable object
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    spdlog::error("Failed to load texture `{}`", path);
  }
  stbi_image_free(data);
}

bool Texture::alpha(const string& path)
{
  auto pos    = path.find_last_of('.');
  auto suffix = path.substr(pos).c_str();
  auto alpha  = (strcmp(suffix, ".png") == 0);
  return alpha;
}
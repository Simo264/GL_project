#include "texture2d.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

void Texture2D::init(const string& path)
{
  _path = path;

  glGenTextures(1, &textureID);

  bind();

  glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  loadImage();
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/


void Texture2D::loadImage()
{
  stbi_set_flip_vertically_on_load(true);
  
  int width, height, nrChannels;
  auto data = stbi_load(_path.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
    int format = GL_RGB;
    if(nrChannels == 4)
      format = GL_RGBA;
    
    // mutable storage
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

  #if 0
    // immutable storage
    glTextureStorage2D(textureID, 1, format, width, height);
    glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
  #endif
    
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    spdlog::error("Failed to load texture `{}`", _path);
  }
  stbi_image_free(data);
}

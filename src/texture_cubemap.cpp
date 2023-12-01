#include "texture_cubemap.hh"

#include "stb_image.h"

#include "spdlog/spdlog.h"


void TextureCubeMap::init(const array<string, 6>& images)
{
  glGenTextures(1, &textureID);
  
  bind();

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  int width, height, nrChannels;
  for(uint32_t i = 0; i < images.size(); i++)
  {
    const auto& path = images[i];

    int format = GL_RGB;
    auto suffix = path.substr(path.find_last_of('.')).c_str();
    if(strcmp(suffix, ".png") == 0)
      format = GL_RGBA;

    stbi_set_flip_vertically_on_load(false);
    u_char* data = stbi_load(images[i].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else
    {
      spdlog::error("Cubemap tex failed to load at path: {}", images[i]);
    }
    stbi_image_free(data);
  }

  unbind();
}

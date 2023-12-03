#include "skybox.hh"

SkyBox::SkyBox(const array<string, 6>& images)
{
  _texture.init(images);

  float skyboxVertices[] ={
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
  };

  uint32_t skyboxIndices[] = {
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
  };


  GL::VAConfiguration config;
  config.pushAttribute(3);

  _vertexBuffer.init(sizeof(skyboxVertices), skyboxVertices);
  _elementBuffer.init(sizeof(skyboxIndices), skyboxIndices);
  _vertexArray.init(config, _vertexBuffer);

 
}


void SkyBox::draw(uint32_t drawmode)
{
  _vertexArray.bind();
  _elementBuffer.bind();

  glActiveTexture(GL_TEXTURE0);
  _texture.bind();
  glDrawElements(drawmode, _elementBuffer.nIndices, GL_UNSIGNED_INT, 0);

  _elementBuffer.unbind();
  _vertexArray.unbind();
}
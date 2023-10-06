#ifndef CAMERA_HH
#define CAMERA_HH

#include "glm/glm.hpp"

class Camera
{
public:
  Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up = glm::vec3(0.0f, 1.0f,  0.0f));
  ~Camera() { }

  glm::mat4 lookAt(glm::vec3 target);
  glm::mat4 lookAround();

public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
};

#endif
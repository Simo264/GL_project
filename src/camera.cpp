#include "camera.hh"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
: position{pos}, front{front}, up{up} { }

glm::mat4 Camera::lookAt(glm::vec3 target)
{
  return glm::lookAt(position, target, up);
}

glm::mat4 Camera::lookAround()
{
  return glm::lookAt(position, position + front, up);
}

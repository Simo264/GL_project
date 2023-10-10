#include "camera.hh"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
: position{pos}, front{front}, up{up} 
{ 
  yaw   = -90;
  pitch = 0;
  speed = 2.5;
  sensitivity = 0.05;

  _lastX = 0;
  _lastY = 0;
}

glm::mat4 Camera::lookAt(glm::vec3 target)
{
  return glm::lookAt(position, target, up);
}

glm::mat4 Camera::lookAround()
{
  return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(Window& window, double deltaTime)
{
  const float movementSpeed = static_cast<float>(speed * deltaTime);

  if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
    position += movementSpeed * front;
  
  if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
    position -= movementSpeed * front;
  
  if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
    position -= glm::normalize(glm::cross(front, up)) * movementSpeed;
  
  if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
    position += glm::normalize(glm::cross(front, up)) * movementSpeed;
}

void Camera::processMouseMovement(Window& window)
{
  double cposx, cposy;
  window.getCursorPosition(cposx, cposy);

  // if cursor is outside of window
  if(!(cposx >= 0 && cposx <= window.width() && cposy >= 0 && cposy <= window.height()))
    return;

  double xoffset = cposx - _lastX;
  double yoffset = _lastY - cposy;

  _lastX = cposx;
  _lastY = cposy;

  yaw   += (xoffset * sensitivity);
  pitch += (yoffset * sensitivity);
  front = glm::normalize(glm::vec3(
    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
    sin(glm::radians(pitch)),
    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
  ));

  auto right  = glm::normalize(glm::cross(front, {0.f,1.f,0.f}));

  up = glm::normalize(glm::cross(right, front));
}
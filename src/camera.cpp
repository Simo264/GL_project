#include "camera.hh"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Camera::Camera(vec3f pos, vec3f front, vec3f up)
: position{pos}, front{front}, up{up} 
{ 
  yaw   = -90;
  pitch = 0;
  speed = 2.5;
  sensitivity = 0.25;

  _lastX = 0;
  _lastY = 0;
}

mat4f Camera::lookAtTarget(vec3f target)
{
  return lookAt(position, target, up);
}

mat4f Camera::lookAround()
{
  return lookAt(position, position + front, up);
}

void Camera::processKeyboardInput(Window* window, double deltaTime)
{
  const float velocity = static_cast<float>(speed * deltaTime);

  if (window->getKey(GLFW_KEY_W) == GLFW_PRESS)
    position += front * velocity;
  
  if (window->getKey(GLFW_KEY_S) == GLFW_PRESS)
    position -= front * velocity;
  
  if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
    position -= right * velocity;
  
  if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
    position += right * velocity;

  if (window->getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
    position += up * velocity;
  
  if (window->getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    position -= up * velocity;

  
}

void Camera::processMouseMovement(Window* window)
{
  double cposx, cposy;
  window->getCursorPosition(cposx, cposy);

  if(window->getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) 
    return;

  // if cursor is outside of window
  // if(!(cposx >= 0 && cposx <= window.width() && cposy >= 0 && cposy <= window.height()))
  //   return;

  double xoffset = cposx - _lastX;
  double yoffset = _lastY - cposy;

  _lastX = cposx;
  _lastY = cposy;

  yaw   += (xoffset * sensitivity);
  pitch += (yoffset * sensitivity);
  front = normalize(vec3f(
    cos(radians(yaw)) * cos(radians(pitch)),
    sin(radians(pitch)),
    sin(radians(yaw)) * cos(radians(pitch))
  ));

  right = normalize(cross(front, {0.f, 1.f, 0.f}));
  up    = normalize(cross(right, front));
}
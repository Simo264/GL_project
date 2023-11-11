#include "camera.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Camera::Camera(vec3f pos, vec3f front, vec3f up)
: position{pos}, front{front}, up{up} 
{ 
  yaw   = 90;
  pitch = 0;
  speed = 2.5;
  sensitivity = 0.25;
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
  (void) deltaTime;

  const float sensitivity = 1.f;
  static float rotation   = 0.f;
  
  const float angle = radians(90.f);
  const float distance = 1.f;
  
 
  if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
  { 
    rotation -= deltaTime * sensitivity;

    const float senx  = glm::sin(rotation);
    const float cosx  = glm::cos(rotation);
    position.x = (senx * angle * distance);
    position.z = (cosx * angle * distance);
  }
  if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
  { 
    rotation += deltaTime * sensitivity;

    const float senx  = glm::sin(rotation);
    const float cosx  = glm::cos(rotation);
    position.x = (senx * angle * distance);
    position.z = (cosx * angle * distance);
  }

}

#if 0
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
#endif
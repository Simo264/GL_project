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

  //const float velocity = static_cast<float>(speed * deltaTime);
  static float rotation = 0.f;
 
  if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
  {
    rotation += 0.01f;
    const float senx = glm::sin(rotation);
    const float cosx = glm::cos(rotation);
    spdlog::info("{}, {}, {}", rotation, senx, cosx);

    position.x = (senx * 90 * 0.01f) + 1.f;
    position.z = (cosx * 90 * 0.01f) + 1.f;
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
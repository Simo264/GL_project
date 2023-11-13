#include "camera.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Camera::Camera(vec3f position)
{
  this->position = position;
  front = vec3f(0.0f, 0.0f, -1.0f), 
  up    = vec3f(0.0f, 1.0f,  0.0f);

  sensitivity = 1.f;
  distanceToTarget = -1.f;
  target = nullptr;
}

Camera::Camera(vec3f* target, float distance)
{
  position.x = target->x + distance;
  position.y = target->y + distance;
  position.z = target->z + distance;

  front = vec3f(0.0f, 0.0f, -1.0f), 
  up    = vec3f(0.0f, 1.0f,  0.0f);

  sensitivity = 1.f;
  this->target = target;
  distanceToTarget = distance;
}

mat4f Camera::getViewMatrix() const
{
  return lookAt(position, *target, up);

#if 0
  if(target)
    return lookAt(position, *target, up);
  else
    return lookAt(position, position + front, up);
#endif
}

void Camera::processKeyboardInput(Window* window, double deltaTime)
{
  const float velocity     = deltaTime * sensitivity;
  static float rotation    = 0.f;
  static const float angle = radians(90.f);
  if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
  { 
    rotation -= velocity;
    position.x = glm::sin(rotation) * angle * distanceToTarget;
    position.z = glm::cos(rotation) * angle * distanceToTarget;
  }
  else if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
  { 
    rotation += velocity;
    position.x = glm::sin(rotation) * angle * distanceToTarget;
    position.z = glm::cos(rotation) * angle * distanceToTarget;
  }

  if (window->getKey(GLFW_KEY_W) == GLFW_PRESS)
  { 
    position.y += velocity*10;
  }
  else if (window->getKey(GLFW_KEY_S) == GLFW_PRESS)
  { 
    position.y -= velocity*10;
  }
}


#include "camera.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Camera::Camera(vec3f position) : position{position}
{
  _front = vec3f(0.0f, 0.0f, -1.0f), 
  _up    = vec3f(0.0f, 1.0f,  0.0f);

  sensitivity    = 0.75f;
  targetDistance = 10.f;
  fov            = 45.0f;
  target         = nullptr;
}

mat4f Camera::getViewMatrix() const
{
  return lookAt(position, *target, _up);

#if 0
  if(target)
    return lookAt(position, *target, _up);
  else
    return lookAt(position, position + _front, _up);
#endif
}


void Camera::processInput(Window* window)
{
  const double delta    = window->delta();     (void)delta;
  const float velocity  = delta * sensitivity; (void)velocity;

  if(window->getMouseKey(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    vec2d mousePos;
    window->getCursorPosition(mousePos);
    
    rotateAroundTarget(mousePos, velocity);    
  }

  if(window->getKey(GLFW_KEY_W) == GLFW_PRESS)
  {
    if(targetDistance > 5)
    {
      targetDistance -= velocity*4;
      position.z     -= velocity*4;
    }

  }
  else if(window->getKey(GLFW_KEY_S) == GLFW_PRESS)
  {
    targetDistance += velocity*4;
    position.z     += velocity*4;
  }
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

void Camera::rotateAroundTarget(const vec2d& mousePos, float velocity)
{
  static float rotationX = 0.0f;
  static float rotationY = 0.0f;
  static double prevXPos = 0.0f;
  static double prevYPos = 0.0f;

  // rotate X axis
  auto diffX = prevXPos - mousePos.x;
  if (diffX > 0) // left
  {
    rotationX += velocity; 
    position.x = glm::sin(rotationX) * targetDistance;
    position.z = glm::cos(rotationX) * targetDistance;
    
    prevXPos = mousePos.x;
  }
  else if(diffX < 0) // right
  {
    rotationX -= velocity;
    position.x = glm::sin(rotationX) * targetDistance;
    position.z = glm::cos(rotationX) * targetDistance;
    prevXPos = mousePos.x;
  }

  // rotate Y axis
  auto diffY = prevYPos - mousePos.y;
  const float rads = radians(85.0f);
  if (diffY > 0) // up
  {
    if(rotationY < -rads) 
      return;

    rotationY -= velocity;
    position.y = glm::sin(rotationY) * (targetDistance/2);
    prevYPos = mousePos.y;
  }
  else if(diffY < 0) // down
  {
    if(rotationY > rads) 
      return;

    rotationY += velocity;
    position.y = glm::sin(rotationY) * (targetDistance/2);
    prevYPos = mousePos.y;
  }
}
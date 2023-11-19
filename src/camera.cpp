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

  sensitivity    = 1.f;
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
  const double delta    = window->delta(); (void)delta;
  const float velocity  = delta * sensitivity; (void)velocity;

  if(window->getMouseKey(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    static float rotationX = 0.0f;
    static float rotationY = 0.0f;
    static double prevXPos = 0.0f;
    static double prevYPos = 0.0f;

    vec2d mousePos;
    window->getCursorPosition(mousePos);

    // rotate X axis
    auto diffX = prevXPos - mousePos.x;
    if (diffX > 0) // left
    {
      rotationX += 0.025f;
      position.x = glm::sin(rotationX) * targetDistance;
      position.z = glm::cos(rotationX) * targetDistance;
      
      prevXPos = mousePos.x;
    }
    else if(diffX < 0) // right
    {
      rotationX -= 0.025f;
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

      rotationY -= 0.025f;
      position.y = glm::sin(rotationY) * (targetDistance/2);
      prevYPos = mousePos.y;
    }
    else if(diffY < 0) // down
    {
      if(rotationY > rads) 
        return;

      rotationY += 0.025f;
      position.y = glm::sin(rotationY) * (targetDistance/2);
      prevYPos = mousePos.y;
    }
    
  }
}



#if 0
void Camera::processKeyboardInput(Window* window, double deltaTime)
{
  (void) window;
  (void) deltaTime;

  const float velocity     = deltaTime * sensitivity;
  static float rotation    = 0.f;
  static const float angle = radians(90.f);
  if (window->getKey(GLFW_KEY_A) == GLFW_PRESS)
  { 
    rotation -= velocity;
    position.x = glm::sin(rotation) * angle * distance;
    position.z = glm::cos(rotation) * angle * distance;
  }
  else if (window->getKey(GLFW_KEY_D) == GLFW_PRESS)
  { 
    rotation += velocity;
    position.x = glm::sin(rotation) * angle * distance;
    position.z = glm::cos(rotation) * angle * distance;
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

void Camera::processMouseScrollInput(Window* window, double deltaTime)
{
  (void) window;
  (void) deltaTime;
}
#endif

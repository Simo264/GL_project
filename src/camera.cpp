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

  _pitch = 0.0f;
  _yaw = 0.0f;

  sensitivity    = 64.0f;
  speed          = 8.0f;
  fov            = 45.0f;
  target         = nullptr;
}

mat4f Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + _front, _up);

#if 0
  if(target)
    return glm::lookAt(position, *target, _up);           // look at target
  else
    return glm::lookAt(position, position + _front, _up); // walk around
#endif
}


void Camera::processInput(Window* window)
{
  const double delta = window->delta();     
  _right = glm::cross(_front, _up);

  freeCameraWalk(window, delta);

  if(window->getMouseKey(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
  {
    vec2d mousePos;
    window->getCursorPosition(mousePos);
    freeCameraRotation(mousePos, delta);
  }

#if 0
  // move around target
  if(target)
  {
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
#endif
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

void Camera::rotateAroundTarget(const vec2d& mousePos, float velocity)
{
  (void) mousePos;
  (void) velocity;

#if 0
  static float rotationX = 0.0f;
  static float rotationY = 0.0f;
  static double prevXPos = 0.0f;
  static double prevYPos = 0.0f;
  static const float targetDistance = 10.0f;

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
  const float rads = glm::radians(85.0f);
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
#endif
}

void Camera::freeCameraWalk(const Window* window, float delta)
{
  const float cameraSpeed = delta * speed;

  if(window->getKey(GLFW_KEY_W) == GLFW_PRESS)
  {
    position += _front * cameraSpeed;
  }
  else if(window->getKey(GLFW_KEY_S) == GLFW_PRESS)
  {
    position -= _front * cameraSpeed;
  }
  
  if(window->getKey(GLFW_KEY_A) == GLFW_PRESS)
  {
    position -= _right * cameraSpeed;
  }
  else if(window->getKey(GLFW_KEY_D) == GLFW_PRESS)
  {
    position += _right * cameraSpeed;
  }

  if(window->getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    position += _up * cameraSpeed;
  }
  else if(window->getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    position -= _up * cameraSpeed;
  }
}

void Camera::freeCameraRotation(vec2d& mousePos, float delta)
{
  const float cameraSens  = delta * sensitivity;
  static float prevXPos = 0.0f;
  static float prevYPos = 0.0f;

  // rotate X axis
  float diffX = prevXPos - mousePos.x;
  if (diffX > 0) // left
  {
    _yaw -= cameraSens;
    prevXPos = mousePos.x;
  }
  else if(diffX < 0) // right
  {
    _yaw += cameraSens; 
    prevXPos = mousePos.x;
  }

  // rotate Y axis
  auto diffY = prevYPos - mousePos.y;
  if (diffY > 0) // up
  {
    _pitch += cameraSens;
    prevYPos = mousePos.y;
  }
  else if(diffY < 0) // down
  {
    _pitch -= cameraSens; 
    prevYPos = mousePos.y;
  }

  vec3f front;
  front.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  front.y = glm::sin(glm::radians(_pitch));
  front.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  _front  = glm::normalize(front);
}




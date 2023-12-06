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
  distance       = 10.0f;
  speed          = 8.0f;
  fov            = 45.0f;
  target         = nullptr;

  _rotationX = 0.0f;
  _rotationY = 0.0f;
  _prevXPos = 0.0f;
  _prevYPos = 0.0f;
}

mat4f Camera::getViewMatrix() const
{
  if(target)
    return glm::lookAt(position, *target, _up);           // look at target
  else
    return glm::lookAt(position, position + _front, _up); // walk around
}

void Camera::processInput(Window& window, double deltaTime)
{
  _right = glm::cross(_front, _up); // update right vector

  vec2d mousePos;
  window.getCursorPosition(mousePos);
  
  if(target)
  {
    if(window.getMouseKey(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
      rotateAroundTarget(mousePos, deltaTime);
    }
  }
  else
  {
    freeCameraWalk(window, deltaTime);
    
    if(window.getMouseKey(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
     freeCameraRotation(mousePos, deltaTime);
    }
  }
}


/* -----------------------------------------------------
 *          PRIVATE METHODS
 * -----------------------------------------------------
*/

void Camera::rotateAroundTarget(const vec2d& mousePos, double deltaTime)
{
  const float velocity = deltaTime * speed;

  // rotate X axis
  auto diffX = _prevXPos - mousePos.x;
  if (diffX > 0) // left
  {
    _rotationX += velocity; 
    position.x  = glm::sin(_rotationX) * distance;
    position.z  = glm::cos(_rotationX) * distance;
    _prevXPos   = mousePos.x;
  }
  else if(diffX < 0) // right
  {
    _rotationX -= velocity;
    position.x  = glm::sin(_rotationX) * distance;
    position.z  = glm::cos(_rotationX) * distance;
    _prevXPos   = mousePos.x;
  }

  // rotate Y axis
  auto diffY = _prevYPos - mousePos.y;
  const float rads = glm::radians(85.0f);
  if (diffY > 0) // up
  {
    if(_rotationY < -rads) 
      return;

    _rotationY -= velocity;
    position.y  = glm::sin(_rotationY) * (distance/2);
    _prevYPos   = mousePos.y;
  }
  else if(diffY < 0) // down
  {
    if(_rotationY > rads) 
      return;

    _rotationY += velocity;
    position.y  = glm::sin(_rotationY) * (distance/2);
    _prevYPos   = mousePos.y;
  }

}

void Camera::freeCameraWalk(const Window& window, double deltaTime)
{
  const float cameraSpeed = deltaTime * speed;

  if(window.getKey(GLFW_KEY_W) == GLFW_PRESS)
  {
    position += _front * cameraSpeed;
  }
  else if(window.getKey(GLFW_KEY_S) == GLFW_PRESS)
  {
    position -= _front * cameraSpeed;
  }
  
  if(window.getKey(GLFW_KEY_A) == GLFW_PRESS)
  {
    position -= _right * cameraSpeed;
  }
  else if(window.getKey(GLFW_KEY_D) == GLFW_PRESS)
  {
    position += _right * cameraSpeed;
  }

  if(window.getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    position += _up * cameraSpeed;
  }
  else if(window.getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    position -= _up * cameraSpeed;
  }
}

void Camera::freeCameraRotation(vec2d& mousePos, double deltaTime)
{
  const float cameraSens = deltaTime * sensitivity;

  // rotate X axis
  float diffX = _prevXPos - mousePos.x;
  if (diffX > 0) // left
  {
    _yaw -= cameraSens;
    _prevXPos = mousePos.x;
  }
  else if(diffX < 0) // right
  {
    _yaw += cameraSens; 
    _prevXPos = mousePos.x;
  }

  // rotate Y axis
  auto diffY = _prevYPos - mousePos.y;
  if (diffY > 0) // up
  {
    _pitch += cameraSens;
    _prevYPos = mousePos.y;
  }
  else if(diffY < 0) // down
  {
    _pitch -= cameraSens; 
    _prevYPos = mousePos.y;
  }

  vec3f front;
  front.x = glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  front.y = glm::sin(glm::radians(_pitch));
  front.z = glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch));
  _front  = glm::normalize(front);
}



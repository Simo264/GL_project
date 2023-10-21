#ifndef CAMERA_HH
#define CAMERA_HH

#include "glm/glm.hpp"

#include "window.hh"

class Camera
{
public:
  Camera(
    glm::vec3 pos   = glm::vec3(0.0f, 0.0f,  0.0f), 
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), 
    glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f));
  ~Camera() { }

  glm::mat4 lookAt(glm::vec3 target);
  glm::mat4 lookAround();

  void processKeyboardInput(Window* window, double deltaTime);
  void processMouseMovement(Window* window);

public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 up;

  double yaw;
  double pitch;
  double speed;
  double sensitivity;

private:
  double _lastX;
  double _lastY;

};

#endif
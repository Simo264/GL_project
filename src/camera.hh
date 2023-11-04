#ifndef CAMERA_HH
#define CAMERA_HH

#include "core.hh"

#include "window.hh"

class Camera
{
public:
  Camera(
    vec3f pos   = vec3f(0.0f, 0.0f,  0.0f), 
    vec3f front = vec3f(0.0f, 0.0f, -1.0f), 
    vec3f up    = vec3f(0.0f, 1.0f,  0.0f));
  ~Camera() { }

  mat4f lookAtTarget(vec3f target);
  mat4f lookAround();

  void processKeyboardInput(Window* window, double deltaTime);
  void processMouseMovement(Window* window);

public:
  vec3f position;
  vec3f front;
  vec3f right;
  vec3f up;

  double yaw;
  double pitch;
  double speed;
  double sensitivity;

private:
  double _lastX;
  double _lastY;

};

#endif
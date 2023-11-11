#ifndef CAMERA_HH
#define CAMERA_HH

#include "core.hh"

#include "window.hh"

class Camera
{
public:
  Camera(vec3f* target, float distance);
  ~Camera() { }

  mat4f getViewMatrix() const;

  void processKeyboardInput(Window* window, double deltaTime);

  vec3f position;
  vec3f front;
  vec3f right;
  vec3f up;

  vec3f* target;

  float sensitivity;
  float distanceToTarget;
};

#endif
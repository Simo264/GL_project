#ifndef CAMERA_HH
#define CAMERA_HH

#include "core.hh"

#include "window.hh"

class Camera
{
public:
  Camera(vec3f position = vec3f(0.0f,0.0f,0.0f));
  ~Camera() = default;

  mat4f getViewMatrix() const;

  void processInput(Window& window);

  // void processKeyboardInput(Window* window, double deltaTime);
  // void processMouseScrollInput(Window* window, double deltaTime);
  
  vec3f   position;
  vec3f*  target;

  float sensitivity;
  float distance;
  float speed;
  float fov;

private:
  // camera vectors
  vec3f _front;
  vec3f _up;
  vec3f _right;

  // Euler angles
  float _pitch; // y-axis
  float _yaw;   // x-axis

  // camera rotation 
  float _rotationX;
  float _rotationY;
  float _prevXPos;
  float _prevYPos;

  void rotateAroundTarget(const vec2d& mousePos, float velocity);

  void freeCameraWalk(const Window& window, float delta);
  void freeCameraRotation(vec2d& mousePos, float delta);
};

#endif
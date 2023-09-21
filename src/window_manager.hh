#ifndef WINDOW_MANAGER_HH
#define WINDOW_MANAGER_HH

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <functional>

class WindowManager
{
public:
  WindowManager() = default;
  ~WindowManager() = default;

  static WindowManager* getInstance();
  
  // glfw: initialize and configure
  // glad: load all OpenGL function pointers
  GLFWwindow* createWindow(const char* title, uint16_t w=720, uint16_t h=720);
  
  // return GLFWwindow* object
  GLFWwindow* get() const { return mWindow; }

  // render loop: input events, render scene, swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
  void loop(std::function<void()> inputCallback, std::function<void()> renderCallback);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  void destroyWindow();

  // release all resources
  void release();

private:
  GLFWwindow* mWindow;

};


#endif
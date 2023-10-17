#ifndef WINDOW_HH
#define WINDOW_HH

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

class Window
{
public:
  Window();
  ~Window() = default;
  
  // creating a window and context
  void create(std::string title, uint16_t width=720, uint16_t heigth=720, bool fullscreen = false);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  void terminate();
  
  // set close flag TRUE
  void close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }

  // render loop
  bool loop() const { return !glfwWindowShouldClose(_window); }

  // return GLFWwindow* object
  GLFWwindow* get() const { return _window; }

  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
  void swapBuffersAndProcessEvents();

  // specify clear values for the color buffers
  void clearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

  // clear buffers to preset values
  void clearBuffers(uint32_t mask) { glClear(mask); }

  // keyboard input
  int getKey(uint32_t key) const { return glfwGetKey(_window, key); }

  // mouse input
  int getMouseButton(uint32_t key) const { return glfwGetMouseButton(_window, key); }


  // window size
  uint16_t width() const { return _width; }
  uint16_t height() const { return _height; }

  // keyboard input
  void processKeyboardInput();

  // window position
  void setPosition(int x, int y) { glfwSetWindowPos(_window, x, y); }
  void getPosition(int& x, int& y) { glfwGetWindowPos(_window, &x, &y); }

  // cursor position
  void getCursorPosition(double& x, double& y) { glfwGetCursorPos(_window, &x, &y); }

  // value = GLFW_CURSOR_NORMAL | GLFW_CURSOR_HIDDEN | GLFW_CURSOR_DISABLED
  void setCursorMode(int value) { glfwSetInputMode(_window, GLFW_CURSOR, value); }

  // update delta time
  void update();

  double delta() const;

private:
  GLFWwindow* _window;
  
  uint16_t _width;
  uint16_t _height;

  // time
  double prevFrame;
  double currFrame;

  static void errorCallback(int error, const char* description);
  static void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#endif
#ifndef WINDOW_HH
#define WINDOW_HH

#include "core.hh"

class Window
{
public:
  Window()  = default;
  ~Window() = default;

  // creating a window and context
  void create(vec2u dim, vec2u pos, const char* title);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  void destroy();
  
  // glfw: set close flag TRUE
  void close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }

  // glfw: render loop
  bool loop() const { return !glfwWindowShouldClose(_window); }

  // return GLFWwindow* object
  GLFWwindow* get() const { return _window; }

  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
  void swapBuffers() { glfwSwapBuffers(_window); }

  // glfw: retrieves the size, in pixels, of the framebuffer of the specified window
  void getFramebufferSize(vec2i& sz) { glfwGetFramebufferSize(_window, &sz.x, &sz.y); }

  // window dimension
  const uint32_t& width() const  { return _width; }
  const uint32_t& height() const { return _height; }

  // glfw: keyboard key
  int getKey(uint32_t key) const { return glfwGetKey(_window, key); }

  // glfw: mouse Key
  int getMouseKey(uint32_t key) const { return glfwGetMouseButton(_window, key); }

  // glfw: mouse position 
  void getCursorPosition(vec2d& pos) { glfwGetCursorPos(_window, &pos.x, &pos.y); }
  
  // glfw: value = GLFW_CURSOR_NORMAL | GLFW_CURSOR_HIDDEN | GLFW_CURSOR_DISABLED
  void setCursorMode(int value) { glfwSetInputMode(_window, GLFW_CURSOR, value); }

  // glfw: window position
  void setPosition(vec2i pos)  { glfwSetWindowPos(_window, pos.x, pos.y); }
  void getPosition(vec2i& pos) { glfwGetWindowPos(_window, &pos.x, &pos.y); }

  void processKeyboardInput();

private:
  GLFWwindow* _window;
  
  uint32_t _width;
  uint32_t _height;
};
#endif
#ifndef WINDOW_HH
#define WINDOW_HH

#include "core.hh"

class Window
{
public:
  Window();
  ~Window() = default;

  // creating a window and context
  void create(vec2u dim, vec2u pos, const char* title, bool fullscreen = false);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  void destroy();
  
  // set close flag TRUE
  void close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }

  // render loop
  bool loop() const { return !glfwWindowShouldClose(_window); }

  // return GLFWwindow* object
  GLFWwindow* get() const { return _window; }

  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
  void swapBuffers() { glfwSwapBuffers(_window); }

  // window dimension
  uint32_t width() const  { return _width; }
  uint32_t height() const { return _height; }

  // keyboard key
  int getKey(uint32_t key) const { return glfwGetKey(_window, key); }

  // mouse Key
  int getMouseKey(uint32_t key) const { return glfwGetMouseButton(_window, key); }

  // mouse position 
  void getCursorPosition(vec2d& pos) { glfwGetCursorPos(_window, &pos.x, &pos.y); }
  
  // value = GLFW_CURSOR_NORMAL | GLFW_CURSOR_HIDDEN | GLFW_CURSOR_DISABLED
  void setCursorMode(int value) { glfwSetInputMode(_window, GLFW_CURSOR, value); }

  // window position
  void setPosition(vec2i pos)  { glfwSetWindowPos(_window, pos.x, pos.y); }
  void getPosition(vec2i& pos) { glfwGetWindowPos(_window, &pos.x, &pos.y); }

  void processKeyboardInput();

  // update delta time
  void update();
  double delta() const;

  // display milliseconds-per-frames
  void msPerFrame();

private:
  GLFWwindow* _window;
  
  uint32_t _width;
  uint32_t _height;

  // delta time
  double _prevFrame;
  double _currFrame;

  // measure speed per frame
  double _lastTime;
  int    _nbFrames;
};
#endif
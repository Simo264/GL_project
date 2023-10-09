#ifndef WINDOW_HH
#define WINDOW_HH

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <functional>

class Window
{
public:
  Window();
  ~Window() = default;
  
  // creating a window and context
  void create(std::string title, uint16_t width=720, uint16_t heigth=720);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  void terminate();
  
  // set close flag TRUE
  void close();

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

  void setCursorPosCallback(GLFWcursorposfun callback);

  // window size
  uint16_t getWidth() const { return _width; }
  uint16_t getHeight() const { return _height; }

private:
  GLFWwindow* _window;
  
  uint16_t _width;
  uint16_t _height;

  static void errorCallback(int error, const char* description);
  static void defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  // static void defaultCursoPosCallback(GLFWwindow* window, double xpos, double ypos);
};


#endif
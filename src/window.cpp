#include "window.hh"
#include "spdlog/spdlog.h"

#include<iterator> 
#include<algorithm>

Window::Window()
{
  if(!glfwInit())
  {
    spdlog::error("GLFW failed to initialize.");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(errorCallback);
}

void Window::create(std::string title, uint16_t width, uint16_t heigth)
{
  _width = width; 
  _height = heigth;

  _window = glfwCreateWindow(width, heigth, title.c_str(), NULL, NULL);
  if (!_window)
  {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, this);
  glfwSetKeyCallback(_window, defaultKeyCallback);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
}

void Window::terminate()
{
  if (_window)
    glfwDestroyWindow(_window);

  glfwTerminate();
}

void Window::close()
{
  glfwSetWindowShouldClose(_window, GLFW_TRUE);
} 

void Window::swapBuffersAndProcessEvents()
{
  glfwSwapBuffers(_window);  
  glfwPollEvents();
}

void Window::errorCallback(int error, const char* description)
{
  (void)error; // suppress werror

  spdlog::error(description);
}

void Window::defaultKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  (void) window;    // suppress werror
  (void) scancode;  // suppress werror
  (void) mods;      // suppress werror

  // Window* pw = (Window*)glfwGetWindowUserPointer(window);

  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(_window, callback);
}



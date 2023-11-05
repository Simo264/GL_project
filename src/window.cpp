#include "window.hh"

#include "spdlog/spdlog.h"

static void errorCallback(int error, const char* description)
{
  (void)error; // suppress werror 

  spdlog::error(description);
}


/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Window::Window()
{
  glfwSetErrorCallback(errorCallback);

  if(!glfwInit())
  {
    spdlog::error("GLFW failed to initialize.");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  

  _prevFrame = 0;
  _currFrame = 0;	
}

void Window::create(vec2u dim, string title, bool fullscreen)
{
  _width = dim.x; 
  _height = dim.y;

  GLFWmonitor* monitor = nullptr;

  if(fullscreen)
    monitor = glfwGetPrimaryMonitor();

  _window = glfwCreateWindow(_width, _height, title.c_str(), monitor, NULL);
  if (!_window)
  {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, this);
  glfwSwapInterval(1); // Enable vsync

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

void Window::swapBuffersAndProcessEvents()
{
  glfwSwapBuffers(_window);  
  glfwPollEvents();
}

void Window::processKeyboardInput()
{
  if(getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    close();
}

double Window::delta() const
{
  return _currFrame - _prevFrame;
}

void Window::update()
{
  _prevFrame = _currFrame;
  _currFrame = glfwGetTime();
}




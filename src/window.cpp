#include "window.hh"

#include "spdlog/spdlog.h"

/* -----------------------------------------------------
 *          PUBLIC METHODS
 * -----------------------------------------------------
*/

Window::Window()
{
  _prevFrame = 0; 
  _currFrame = 0;

  _lastTime  = glfwGetTime();
  _nbFrames  = 0;
}

void Window::create(vec2u dim, vec2u pos, const char* title, bool fullscreen)
{
  _width = dim.x;
  _height= dim.y;

  GLFWmonitor* monitor = nullptr;
  if(fullscreen)
    monitor = glfwGetPrimaryMonitor();

  _window = glfwCreateWindow(_width, _height, title, monitor, NULL);
  glfwMakeContextCurrent(_window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  setPosition(pos);
}

void Window::destroy()
{
  if (_window)
    glfwDestroyWindow(_window);
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

void Window::msPerFrame()
{
  double currentTime = glfwGetTime();
  _nbFrames++;

  if ( currentTime - _lastTime >= 1.0 )
  {
    spdlog::info("{} ms/frame", 1000.0/double(_nbFrames));
    _nbFrames = 0;
    _lastTime += 1.0f;
  }
}

